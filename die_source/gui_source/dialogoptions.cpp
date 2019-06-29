// Copyright (c) 2012-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "dialogoptions.h"
#include "ui_dialogoptions.h"

DialogOptions::DialogOptions(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

#if  defined(_WIN32)

#else
    ui->pushButtonExeClear->setEnabled(false);
    ui->pushButtonExeSet->setEnabled(false);
    ui->pushButtonDllClear->setEnabled(false);
    ui->pushButtonDllSet->setEnabled(false);
    ui->pushButtonSysClear->setEnabled(false);
    ui->pushButtonSysSet->setEnabled(false);
    ui->pushButtonAllClear->setEnabled(false);
    ui->pushButtonAllSet->setEnabled(false);
#endif

    ui->listWidgetTabs->setCurrentRow(0);

    shortcut1 = new QShortcut(QKeySequence(__KeySequence_tab1), this);
    QObject::connect(shortcut1, SIGNAL(activated()), this, SLOT(tab1()));
    shortcut2 = new QShortcut(QKeySequence(__KeySequence_tab2), this);
    QObject::connect(shortcut2, SIGNAL(activated()), this, SLOT(tab2()));
    shortcut3 = new QShortcut(QKeySequence(__KeySequence_tab3), this);
    QObject::connect(shortcut3, SIGNAL(activated()), this, SLOT(tab3()));
    shortcut4 = new QShortcut(QKeySequence(__KeySequence_tab4), this);
    QObject::connect(shortcut4, SIGNAL(activated()), this, SLOT(tab4()));
    shortcut5 = new QShortcut(QKeySequence(__KeySequence_tab5), this);
    QObject::connect(shortcut5, SIGNAL(activated()), this, SLOT(tab5()));
    shortcut6 = new QShortcut(QKeySequence(__KeySequence_tab6), this);
    QObject::connect(shortcut6, SIGNAL(activated()), this, SLOT(tab6()));
    shortcut7 = new QShortcut(QKeySequence(__KeySequence_tab7), this);
    QObject::connect(shortcut7, SIGNAL(activated()), this, SLOT(tab7()));
    shortcut8 = new QShortcut(QKeySequence(__KeySequence_tab8), this);
    QObject::connect(shortcut8, SIGNAL(activated()), this, SLOT(tab8()));

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));

#ifndef USE_NFD
    ui->groupBoxNFD->hide();
#endif

#ifndef USE_YARA
    ui->groupBoxYARA->hide();
#endif
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogOptions::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogOptions::on_pushButtonApply_clicked()
{
    bool bAppRestart=false;

    pOptions->bStayOnTop=ui->checkBoxStayOnTop->isChecked();
    pOptions->bScanAfterOpen=ui->checkBoxScanAfterOpen->isChecked();
    pOptions->bScanShowVersionDIE=ui->checkBoxVersionDIE->isChecked();
    pOptions->bScanShowOptionsDIE=ui->checkBoxOptionsDIE->isChecked();
    pOptions->bScanDeepScanDIE=ui->checkBoxDeepScanDIE->isChecked();
    pOptions->bScanResizeToContentsDIE=ui->checkBoxResizeColumnsToContent->isChecked();

    if(pOptions->sm!=(SCAN_METHODS)ui->comboBoxMethod->itemData(ui->comboBoxMethod->currentIndex()).toInt())
    {
        pOptions->sm=(SCAN_METHODS)ui->comboBoxMethod->itemData(ui->comboBoxMethod->currentIndex()).toInt();
    }

#ifdef USE_NFD
    pOptions->bScanDeepScanNFD=ui->checkBoxDeepScanNFD->isChecked();
    pOptions->bScanScanOverlayNFD=ui->checkBoxScanOverlayNFD->isChecked();
#endif
    pOptions->bShowScanTime=ui->checkBoxScanTime->isChecked();
    pOptions->bSaveLastDirectory=ui->checkBoxSaveLastDirectory->isChecked();
    pOptions->bShowTooltips=ui->checkBoxShowTooltips->isChecked();
    pOptions->bSingleApplication=ui->checkBoxSingleApplication->isChecked();
    pOptions->bShowErrors=ui->checkBoxShowErrors->isChecked();
    pOptions->bSectionsEntropyAfterOpen=ui->checkBoxSectionsEntropyAfterOpen->isChecked();
    pOptions->bResizeableMainWindow=ui->checkBoxResizeableMainWindow->isChecked();
    pOptions->fThreshold=ui->doubleSpinBox->value();
    pOptions->nEntropyGraph=ui->spinBoxEntropy->value();
    pOptions->nAddressWidth=ui->spinBoxAddressWidth->value();

    if(pOptions->bEnablePlugins!=ui->checkBoxEnablePlugins->isChecked())
    {
        pOptions->bEnablePlugins=ui->checkBoxEnablePlugins->isChecked();
        bAppRestart=true;
    }

    if(pOptions->sCodec!=ui->comboBoxCodec->currentText())
    {
#if (QT_VERSION_MAJOR<5)
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(ui->comboBoxCodec->currentText().toAscii().data()));
#endif
        pOptions->sCodec=ui->comboBoxCodec->currentText();
    }

    if(pOptions->sStyle!=ui->comboBoxStyle->currentText())
    {
        qApp->setStyle(ui->comboBoxStyle->currentText());
        pOptions->sStyle=ui->comboBoxStyle->currentText();
        bAppRestart=true;
    }

    if(pOptions->sStyleSheet!=ui->comboBoxStyleSheet->currentText())
    {
        QString sStyleSheet=ui->comboBoxStyleSheet->currentText();

        if(sStyleSheet!="default")
        {
            QFile file;
            file.setFileName(Utils::getQSSPath(pOptions)+QDir::separator()+QString("%1.qss").arg(sStyleSheet));

            if(file.open(QIODevice::ReadOnly))
            {
                QByteArray baQss=file.readAll();
                qApp->setStyleSheet(baQss.data());

                file.close();
            }
        }
        else
        {
            qApp->setStyleSheet("");
        }

        pOptions->sStyleSheet=ui->comboBoxStyleSheet->currentText();
        bAppRestart=true;
    }

    bool bLanguageEnable=ui->checkBoxLanguage->isChecked();

    if(pOptions->bEnableLanguage!=bLanguageEnable)
    {
        pOptions->bEnableLanguage=bLanguageEnable;
        bAppRestart=true;
    }

    QString sLang=ui->comboBoxLanguage->itemData(ui->comboBoxLanguage->currentIndex()).toString();

    if(pOptions->sLanguage!=sLang)
    {
        pOptions->sLanguage=sLang;

        //        QTranslator translator;
        //        bool bLoad=translator.load(pOptions->sLanguage,pOptions->sLangsPath);

        //        if(bLoad)
        //        {
        //            qApp->installTranslator(&translator);
        //        }

        bAppRestart=true;
    }

    if(pOptions->bEnableFont!=ui->groupBoxFont->isChecked())
    {
        pOptions->bEnableFont=ui->groupBoxFont->isChecked();

        if(!pOptions->bEnableFont)
        {
            QFont font2;
            font2.fromString(Utils::getDefaultFont());
            QApplication::setFont(font2);
            this->setFont(font2);
        }

        bAppRestart=true;
    }

    QFont _font;

    if(pOptions->sFont!=ui->lineEditFont->text())
    {
        pOptions->sFont=ui->lineEditFont->text();

        if(pOptions->bEnableFont)
        {
            _font.fromString(ui->lineEditFont->text());
            QApplication::setFont(_font);

            this->setFont(_font);
        }

        bAppRestart=true;
    }

    if(pOptions->sFontHEX!=ui->lineEditFontHEX->text())
    {
        _font.fromString(ui->lineEditFontHEX->text());
        pOptions->sFontHEX=ui->lineEditFontHEX->text();
        //        bAppRestart=true;
    }

    if(pOptions->sEndianness!=ui->comboBoxEndianness->currentText())
    {
        pOptions->sEndianness=ui->comboBoxEndianness->currentText();
    }

    // Check directory exists
    QDir dir;

    if(!dir.exists(Utils::convertPath(ui->lineEditDataBase->text())))
    {
        ui->lineEditDataBase->setText(__DefaultDataBasePath);
    }

    if(!dir.exists(Utils::convertPath(ui->lineEditEditor->text())))
    {
        ui->lineEditEditor->setText(__DefaultEditorPath);
    }

    if(!dir.exists(Utils::convertPath(ui->lineEditHelp->text())))
    {
        ui->lineEditHelp->setText(__DefaultHelpPath);
    }

    if(!dir.exists(Utils::convertPath(ui->lineEditInfo->text())))
    {
        ui->lineEditInfo->setText(__DefaultInfoPath);
    }

    if(!dir.exists(Utils::convertPath(ui->lineEditPlugins->text())))
    {
        ui->lineEditPlugins->setText(__DefaultPluginsPath);
    }

    if(!dir.exists(Utils::convertPath(ui->lineEditQSS->text())))
    {
        ui->lineEditQSS->setText(__DefaultQSSPath);
    }

    if(!dir.exists(Utils::convertPath(ui->lineEditScripts->text())))
    {
        ui->lineEditScripts->setText(__DefaultScriptsPath);
    }

    if(!dir.exists(Utils::convertPath(ui->lineEditSearch->text())))
    {
        ui->lineEditSearch->setText(__DefaultSearchPath);
    }

    if(pOptions->sDataBasePath!=ui->lineEditDataBase->text())
    {
        pOptions->sDataBasePath=ui->lineEditDataBase->text();
        bAppRestart=true;
    }

    if(pOptions->sEditorPath!=ui->lineEditEditor->text())
    {
        pOptions->sEditorPath=ui->lineEditEditor->text();
        bAppRestart=true;
    }

    if(pOptions->sHelpPath!=ui->lineEditHelp->text())
    {
        pOptions->sHelpPath=ui->lineEditHelp->text();
        bAppRestart=true;
    }

    if(pOptions->sInfoPath!=ui->lineEditInfo->text())
    {
        pOptions->sInfoPath=ui->lineEditInfo->text();
        bAppRestart=true;
    }

    if(pOptions->sPluginsPath!=ui->lineEditPlugins->text())
    {
        pOptions->sPluginsPath=ui->lineEditPlugins->text();
        bAppRestart=true;
    }

    if(pOptions->sQSSPath!=ui->lineEditQSS->text())
    {
        pOptions->sQSSPath=ui->lineEditQSS->text();
        bAppRestart=true;
    }

    if(pOptions->sScriptsPath!=ui->lineEditScripts->text())
    {
        pOptions->sScriptsPath=ui->lineEditScripts->text();
        bAppRestart=true;
    }

    if(pOptions->sSearchPath!=ui->lineEditSearch->text())
    {
        pOptions->sSearchPath=ui->lineEditSearch->text();
        bAppRestart=true;
    }

    if(pOptions->sLangsPath!=ui->lineEditLangs->text())
    {
        pOptions->sLangsPath=ui->lineEditLangs->text();
        bAppRestart=true;
    }

#ifdef USE_YARA

    if(pOptions->sDataBaseYARAPath!=ui->lineEditDataBaseYARA->text())
    {
        pOptions->sDataBaseYARAPath=ui->lineEditDataBaseYARA->text();
        bAppRestart=true;
    }

#endif

    if(bAppRestart)
    {
        QMessageBox::information(this,tr("Information"),tr("Need to restart application!"));
    }

    setTooltips(ui->checkBoxShowTooltips->isChecked());

    HandleOptions::save(pOptions);

    ui->pushButtonApply->setEnabled(false);
}

void DialogOptions::on_checkBoxStayOnTop_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxVersionDIE_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxOptionsDIE_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxScanAfterOpen_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxSaveLastDirectory_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxShowTooltips_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_comboBoxStyle_currentIndexChanged(int index)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_comboBoxStyleSheet_currentIndexChanged(int index)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_pushButtonExeSet_clicked()
{
    setExtension("exefile");
}

void DialogOptions::on_pushButtonExeClear_clicked()
{
    clearExtension("exefile");
}

void DialogOptions::on_pushButtonDllSet_clicked()
{
    setExtension("dllfile");
}

void DialogOptions::on_pushButtonDllClear_clicked()
{
    clearExtension("dllfile");
}

void DialogOptions::on_pushButtonSysSet_clicked()
{
    setExtension("sysfile");
}

void DialogOptions::on_pushButtonSysClear_clicked()
{
    clearExtension("sysfile");
}

void DialogOptions::reloadShellState()
{
#if  defined(_WIN32)
    QSettings settings1("HKEY_CLASSES_ROOT\\exefile\\shell",QSettings::NativeFormat);
    QSettings settings2("HKEY_CLASSES_ROOT\\dllfile\\shell",QSettings::NativeFormat);
    QSettings settings3("HKEY_CLASSES_ROOT\\sysfile\\shell",QSettings::NativeFormat);
    QSettings settings4("HKEY_CLASSES_ROOT\\*\\shell",QSettings::NativeFormat);

    if(settings1.value("Detect it easy/command/Default").toString()=="")
    {
        ui->pushButtonExeSet->setEnabled(true);
        ui->pushButtonExeClear->setEnabled(false);
    }
    else
    {
        ui->pushButtonExeSet->setEnabled(false);
        ui->pushButtonExeClear->setEnabled(true);
    }

    if(settings2.value("Detect it easy/command/Default").toString()=="")
    {
        ui->pushButtonDllSet->setEnabled(true);
        ui->pushButtonDllClear->setEnabled(false);
    }
    else
    {
        ui->pushButtonDllSet->setEnabled(false);
        ui->pushButtonDllClear->setEnabled(true);
    }

    if(settings3.value("Detect it easy/command/Default").toString()=="")
    {
        ui->pushButtonSysSet->setEnabled(true);
        ui->pushButtonSysClear->setEnabled(false);
    }
    else
    {
        ui->pushButtonSysSet->setEnabled(false);
        ui->pushButtonSysClear->setEnabled(true);
    }

    if(settings4.value("Detect it easy/command/Default").toString()=="")
    {
        ui->pushButtonAllSet->setEnabled(true);
        ui->pushButtonAllClear->setEnabled(false);
    }
    else
    {
        ui->pushButtonAllSet->setEnabled(false);
        ui->pushButtonAllClear->setEnabled(true);
    }

#endif
}

bool DialogOptions::getShellState(QString sEntry)
{
    QSettings settings1("HKEY_CLASSES_ROOT\\"+sEntry+"\\shell",QSettings::NativeFormat);

    return settings1.value("Detect it easy/command/Default").toString()!="";
}

void DialogOptions::clearEntry(QString sEntry)
{
    QSettings settings(sEntry,QSettings::NativeFormat);
    settings.clear();
}

void DialogOptions::setExtension(QString sExtension)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\Detect It Easy\\command").arg(sExtension),QSettings::NativeFormat);
    settings.setValue(".","\""+QCoreApplication::applicationFilePath().replace("/","\\")+"\" \"%1\"");

    QSettings settingsIcon(QString("HKEY_CLASSES_ROOT\\%1\\shell\\Detect It Easy").arg(sExtension),QSettings::NativeFormat);
    settingsIcon.setValue("Icon","\""+QCoreApplication::applicationFilePath().replace("/","\\")+"\"");

    reloadShellState();

    if(!getShellState(sExtension))
    {
        QMessageBox::critical(this,tr("Register error"),tr("Cannot register %1 extension.\nNeed administrator privileges").arg(sExtension));
    }
}

void DialogOptions::clearExtension(QString sExtension)
{
    clearEntry(QString("HKEY_CLASSES_ROOT\\%1\\shell\\Detect It Easy").arg(sExtension));

    reloadShellState();

    if(getShellState(sExtension))
    {
        QMessageBox::critical(this,tr("Register error"),tr("Cannot unregister %1 extension.\nNeed administrator privileges").arg(sExtension));
    }
}

void DialogOptions::on_checkBoxSingleApplication_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxShowErrors_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxResizeableMainWindow_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxSectionsEntropyAfterOpen_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_spinBoxEntropy_valueChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::tab1()
{
    ui->listWidgetTabs->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);
}
void DialogOptions::tab2()
{
    ui->listWidgetTabs->setCurrentRow(1);
    ui->stackedWidget->setCurrentIndex(1);
}
void DialogOptions::tab3()
{
    ui->listWidgetTabs->setCurrentRow(2);
    ui->stackedWidget->setCurrentIndex(2);
}
void DialogOptions::tab4()
{
    ui->listWidgetTabs->setCurrentRow(3);
    ui->stackedWidget->setCurrentIndex(3);
}
void DialogOptions::tab5()
{
    ui->listWidgetTabs->setCurrentRow(4);
    ui->stackedWidget->setCurrentIndex(4);
}

void DialogOptions::tab6()
{
    ui->listWidgetTabs->setCurrentRow(5);
    ui->stackedWidget->setCurrentIndex(5);
}

void DialogOptions::tab7()
{
    ui->listWidgetTabs->setCurrentRow(6);
    ui->stackedWidget->setCurrentIndex(6);
}

void DialogOptions::tab8()
{
    ui->listWidgetTabs->setCurrentRow(7);
    ui->stackedWidget->setCurrentIndex(7);
}

void DialogOptions::setTooltips(bool bIsEnable)
{
    if(bIsEnable)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxOptionsDIE->setToolTip(tr("Show compiler/linker/packer/protector options"));
        ui->checkBoxVersionDIE->setToolTip(tr("Show compiler/linker/packer/protector version"));
        ui->checkBoxDeepScanDIE->setToolTip(tr("Deep scan"));
        ui->checkBoxDeepScanNFD->setToolTip(tr("Deep scan"));
        ui->checkBoxScanOverlayNFD->setToolTip(tr("Scan overlay"));
        ui->checkBoxScanTime->setToolTip(tr("Show scan time in log"));
        ui->checkBoxSaveLastDirectory->setToolTip(tr("Save last directory"));
        ui->checkBoxScanAfterOpen->setToolTip(tr("Scan file after open or drop"));
        ui->checkBoxShowTooltips->setToolTip(tr("Show tooltips"));
        ui->comboBoxStyle->setToolTip(tr("Set style"));
        ui->comboBoxStyleSheet->setToolTip(tr("Set style sheet"));
        ui->comboBoxLanguage->setToolTip(tr("Set language"));
        //        ui->comboBoxCodec->setToolTip(tr("Set text codec"));
        ui->checkBoxSectionsEntropyAfterOpen->setToolTip(tr("Get sections entropy after open"));
        ui->doubleSpinBox->setToolTip(tr("Threshold of entropy"));
        ui->spinBoxEntropy->setToolTip(tr("Entropy graph"));
        ui->checkBoxSingleApplication->setToolTip(tr("Run only one application instance"));

        ui->pushButtonExeSet->setToolTip(tr("Register context menu entry for %1 files").arg("exe"));
        ui->pushButtonDllSet->setToolTip(tr("Register context menu entry for %1 files").arg("dll"));
        ui->pushButtonSysSet->setToolTip(tr("Register context menu entry for %1 files").arg("sys"));
        ui->pushButtonExeClear->setToolTip(tr("Remove context menu entry for %1 files").arg("exe"));
        ui->pushButtonDllClear->setToolTip(tr("Remove context menu entry for %1 files").arg("dll"));
        ui->pushButtonSysClear->setToolTip(tr("Remove context menu entry for %1 files").arg("sys"));

        ui->pushButtonAllSet->setToolTip(tr("Register context menu entry for %1 files").arg("all"));
        ui->pushButtonAllClear->setToolTip(tr("Register context menu entry for %1 files").arg("all"));

        ui->checkBoxShowErrors->setToolTip(tr("Show errors output in log"));
        ui->checkBoxResizeableMainWindow->setToolTip(tr("Resizeable main window"));

        ui->lineEditFont->setToolTip(tr("Current font"));
        ui->pushButtonFont->setToolTip(tr("Change font"));

        ui->lineEditFontHEX->setToolTip(tr("Current font"));
        ui->pushButtonFontHEX->setToolTip(tr("Change font"));

        ui->spinBoxAddressWidth->setToolTip(tr("Address width"));
        ui->comboBoxEndianness->setToolTip(tr("Endianness"));

        ui->checkBoxEnablePlugins->setToolTip(tr("Enable plugins"));

        ui->pushButtonSetPathDefault->setToolTip(tr("Set default paths"));
        ui->pushButtonDataBase->setToolTip(tr("Set database path"));
        ui->lineEditDataBase->setToolTip(tr("Current database path"));
        ui->pushButtonEditor->setToolTip(tr("Set editor path"));
        ui->lineEditEditor->setToolTip(tr("Current editor path"));
        ui->pushButtonHelp->setToolTip(tr("Set help path"));
        ui->lineEditHelp->setToolTip(tr("Current help path"));
        ui->pushButtonInfo->setToolTip(tr("Set info path"));
        ui->lineEditInfo->setToolTip(tr("Current info path"));
        ui->pushButtonPlugins->setToolTip(tr("Set plugins path"));
        ui->lineEditPlugins->setToolTip(tr("Current plugins path"));
        ui->pushButtonQSS->setToolTip(tr("Set QSS path"));
        ui->lineEditQSS->setToolTip(tr("Current QSS path"));
        ui->pushButtonScripts->setToolTip(tr("Set scripts path"));
        ui->lineEditScripts->setToolTip(tr("Current scripts path"));
        ui->pushButtonSearch->setToolTip(tr("Set search path"));
        ui->lineEditSearch->setToolTip(tr("Current search path"));
        ui->pushButtonLangs->setToolTip(tr("Set lang path"));
        ui->lineEditLangs->setToolTip(tr("Current lang path"));
    }
    else
    {
        ui->pushButtonOK->setToolTip("");
        ui->pushButtonCancel->setToolTip("");
        ui->pushButtonApply->setToolTip("");
        ui->checkBoxOptionsDIE->setToolTip("");
        ui->checkBoxVersionDIE->setToolTip("");
        ui->checkBoxSaveLastDirectory->setToolTip("");
        ui->checkBoxScanAfterOpen->setToolTip("");
        ui->checkBoxShowTooltips->setToolTip("");
        ui->comboBoxStyle->setToolTip("");
        ui->comboBoxStyleSheet->setToolTip("");
        ui->comboBoxLanguage->setToolTip("");
        //        ui->comboBoxCodec->setToolTip("");
        ui->checkBoxSingleApplication->setToolTip("");
        ui->pushButtonExeSet->setToolTip("");
        ui->pushButtonDllSet->setToolTip("");
        ui->pushButtonSysSet->setToolTip("");
        ui->pushButtonExeClear->setToolTip("");
        ui->pushButtonDllClear->setToolTip("");
        ui->pushButtonSysClear->setToolTip("");
        ui->pushButtonAllSet->setToolTip("");
        ui->pushButtonAllClear->setToolTip("");
        ui->doubleSpinBox->setToolTip("");
        ui->spinBoxEntropy->setToolTip("");
        ui->checkBoxSingleApplication->setToolTip("");

        ui->checkBoxShowErrors->setToolTip("");
        ui->checkBoxResizeableMainWindow->setToolTip("");
        ui->lineEditFont->setToolTip("");
        ui->pushButtonFont->setToolTip("");
        ui->lineEditFontHEX->setToolTip("");
        ui->pushButtonFontHEX->setToolTip("");
        ui->spinBoxAddressWidth->setToolTip("");
        ui->comboBoxEndianness->setToolTip("");

        ui->checkBoxEnablePlugins->setToolTip("");

        ui->pushButtonSetPathDefault->setToolTip("");
        ui->pushButtonDataBase->setToolTip("");
        ui->lineEditDataBase->setToolTip("");
        ui->pushButtonEditor->setToolTip("");
        ui->lineEditEditor->setToolTip("");
        ui->pushButtonHelp->setToolTip("");
        ui->lineEditHelp->setToolTip("");
        ui->pushButtonInfo->setToolTip("");
        ui->lineEditInfo->setToolTip("");
        ui->pushButtonPlugins->setToolTip("");
        ui->lineEditPlugins->setToolTip("");
        ui->pushButtonQSS->setToolTip("");
        ui->lineEditQSS->setToolTip("");
        ui->pushButtonScripts->setToolTip("");
        ui->lineEditScripts->setToolTip("");
        ui->pushButtonSearch->setToolTip("");
        ui->lineEditSearch->setToolTip("");
        ui->pushButtonLangs->setToolTip(tr(""));
        ui->lineEditLangs->setToolTip(tr(""));
    }
}

void DialogOptions::on_listWidgetTabs_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void DialogOptions::on_pushButtonFont_clicked()
{
    bool ok;
    QFont initFont;
    initFont.fromString(ui->lineEditFont->text());

    QFont font = QFontDialog::getFont(&ok, initFont, this);

    if(ok)
    {
        ui->lineEditFont->setText(font.key());
    }
}

void DialogOptions::on_lineEditFont_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}
void DialogOptions::on_lineEditFontHEX_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_pushButtonFontHEX_clicked()
{
    bool ok;
    QFont initFont(ui->lineEditFontHEX->text());
    initFont.fromString(ui->lineEditFontHEX->text());
    QFont font = QFontDialog::getFont(&ok,initFont, this);

    if(ok)
    {
        ui->lineEditFontHEX->setText(font.key());

    }
}

void DialogOptions::on_spinBoxAddressWidth_valueChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_comboBoxEndianness_currentIndexChanged(int index)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxScanTime_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

bool DialogOptions::reload()
{
    ui->checkBoxStayOnTop->setChecked(pOptions->bStayOnTop);
    ui->checkBoxScanAfterOpen->setChecked(pOptions->bScanAfterOpen);
    ui->checkBoxVersionDIE->setChecked(pOptions->bScanShowVersionDIE);
    ui->checkBoxOptionsDIE->setChecked(pOptions->bScanShowOptionsDIE);
    ui->checkBoxDeepScanDIE->setChecked(pOptions->bScanDeepScanDIE);
    ui->checkBoxResizeColumnsToContent->setChecked(pOptions->bScanResizeToContentsDIE);

    ui->comboBoxMethod->addItem("Detect It Easy",SM_DIE);
#ifdef USE_NFD
    ui->comboBoxMethod->addItem("Nauz File Detector",SM_NFD);
#endif
#ifdef USE_YARA
    ui->comboBoxMethod->addItem("YARA",SM_YARA);
#endif

    int nMethodIndex=0;

    for(int i=0; i<ui->comboBoxMethod->count(); i++)
    {
        if(ui->comboBoxMethod->itemData(i)==(int)pOptions->sm)
        {
            nMethodIndex=i;
            break;
        }
    }

    ui->comboBoxMethod->setCurrentIndex(nMethodIndex);

#ifdef USE_NFD
    ui->checkBoxDeepScanNFD->setChecked(pOptions->bScanDeepScanNFD);
    ui->checkBoxScanOverlayNFD->setChecked(pOptions->bScanScanOverlayNFD);
#endif
    ui->checkBoxSaveLastDirectory->setChecked(pOptions->bSaveLastDirectory);
    ui->checkBoxShowTooltips->setChecked(pOptions->bShowTooltips);
    ui->checkBoxSingleApplication->setChecked(pOptions->bSingleApplication);
    ui->checkBoxShowErrors->setChecked(pOptions->bShowErrors);
    ui->checkBoxScanTime->setChecked(pOptions->bShowScanTime);
    ui->checkBoxSectionsEntropyAfterOpen->setChecked(pOptions->bSectionsEntropyAfterOpen);
    ui->doubleSpinBox->setValue(pOptions->fThreshold);
    ui->spinBoxEntropy->setValue(pOptions->nEntropyGraph);
    ui->checkBoxResizeableMainWindow->setChecked(pOptions->bResizeableMainWindow);

    ui->groupBoxFont->setChecked(pOptions->bEnableFont);

    ui->lineEditFont->setText(pOptions->sFont);
    ui->lineEditFontHEX->setText(pOptions->sFontHEX);
    ui->spinBoxAddressWidth->setValue(pOptions->nAddressWidth);

    ui->checkBoxEnablePlugins->setChecked(pOptions->bEnablePlugins);

    setTooltips(pOptions->bShowTooltips);

    QList<QByteArray> listCodecs=QTextCodec::availableCodecs();
    QString sCurrentCodec=pOptions->sCodec;
    int nIndex=0;

    ui->comboBoxCodec->addItem("");

    for(int i=0; i<listCodecs.count(); i++)
    {
        ui->comboBoxCodec->addItem(listCodecs.at(i).data());

        if(QString(listCodecs.at(i).data())==sCurrentCodec)
        {
            nIndex=i+1;
        }
    }

    ui->comboBoxCodec->setCurrentIndex(nIndex);

    QStringList listStyles=QStyleFactory::keys();
    QString sCurrentStyle=pOptions->sStyle;
    nIndex=0;

    for(int i=0; i<listStyles.count(); i++)
    {
        if(listStyles.at(i)==sCurrentStyle)
        {
            nIndex=i;
            break;
        }
    }

    ui->comboBoxStyle->addItems(listStyles);
    ui->comboBoxStyle->setCurrentIndex(nIndex);


    sCurrentStyle=pOptions->sStyleSheet;
    nIndex=0;

    QDir dir(Utils::getQSSPath(pOptions)+QDir::separator());
    QFileInfoList dircontent=dir.entryInfoList(QStringList()<<"*.qss");

    ui->comboBoxStyleSheet->addItem("default");

    for(int i=0; i<dircontent.count(); i++)
    {
        QString sBaseName=dircontent.at(i).baseName();

        ui->comboBoxStyleSheet->addItem(sBaseName);

        if(sBaseName==sCurrentStyle)
        {
            nIndex=i+1;
            // No break!!!
        }
    }

    ui->comboBoxStyleSheet->setCurrentIndex(nIndex);

    ui->checkBoxLanguage->setChecked(pOptions->bEnableLanguage);

    //////////////////
    QString sCurrentLanguage=pOptions->sLanguage;
    nIndex=0;

    QDir dir2(Utils::getLangsPath(pOptions)+QDir::separator());
    QFileInfoList dircontent2=dir2.entryInfoList(QStringList()<<"*.qm");

    ui->comboBoxLanguage->addItem("default");

    for(int i=0; i<dircontent2.count(); i++)
    {
        QLocale locale(dircontent2.at(i).baseName().section("_",1,-1));
        //        QLocale::Language lang=locale.language();
        //        QString sLocale=QLocale::languageToString(lang);
        QString sLocale=locale.nativeLanguageName();

        if(dircontent2.at(i).baseName().count("_")==2)
        {
            //            QLocale::Country country=locale.country();
            //            sLocale+=QString("(%1)").arg(QLocale::countryToString(country));
            sLocale+=QString("(%1)").arg(locale.nativeCountryName());
        }

        ui->comboBoxLanguage->addItem(sLocale,dircontent2.at(i).baseName());

        if(dircontent2.at(i).baseName()==sCurrentLanguage)
        {
            nIndex=i+1;
            // No break!!!
        }
    }

    ui->comboBoxLanguage->setCurrentIndex(nIndex);
    //////////////////

    ui->comboBoxEndianness->addItem("Big Endian");
    ui->comboBoxEndianness->addItem("Little Endian");


    for(int i=0; i<ui->comboBoxEndianness->count(); i++)
    {
        if(ui->comboBoxEndianness->itemText(i)==pOptions->sEndianness)
        {
            nIndex=i;
            break;
        }
    }

    ui->comboBoxEndianness->setCurrentIndex(nIndex);

    ui->lineEditDataBase->setText(pOptions->sDataBasePath);
    ui->lineEditEditor->setText(pOptions->sEditorPath);
    ui->lineEditHelp->setText(pOptions->sHelpPath);
    ui->lineEditInfo->setText(pOptions->sInfoPath);
    ui->lineEditPlugins->setText(pOptions->sPluginsPath);
    ui->lineEditQSS->setText(pOptions->sQSSPath);
    ui->lineEditScripts->setText(pOptions->sScriptsPath);
    ui->lineEditSearch->setText(pOptions->sSearchPath);
    ui->lineEditLangs->setText(pOptions->sLangsPath);
#ifdef USE_YARA
    ui->lineEditDataBaseYARA->setText(pOptions->sDataBaseYARAPath);
#endif
    ui->pushButtonApply->setEnabled(false);
    reloadShellState();


    return true;
}

void DialogOptions::on_pushButtonAllSet_clicked()
{
    setExtension("*");
}

void DialogOptions::on_pushButtonAllClear_clicked()
{
    clearExtension("*");
}

void DialogOptions::on_checkBoxEnablePlugins_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_comboBoxCodec_currentIndexChanged(int index)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_pushButtonSetPathDefault_clicked()
{
    ui->lineEditDataBase->setText(__DefaultDataBasePath);
    ui->lineEditEditor->setText(__DefaultEditorPath);
    ui->lineEditHelp->setText(__DefaultHelpPath);
    ui->lineEditInfo->setText(__DefaultInfoPath);
    ui->lineEditPlugins->setText(__DefaultPluginsPath);
    ui->lineEditQSS->setText(__DefaultQSSPath);
    ui->lineEditScripts->setText(__DefaultScriptsPath);
    ui->lineEditSearch->setText(__DefaultSearchPath);
    ui->lineEditLangs->setText(__DefaultLangsPath);
}

void DialogOptions::on_lineEditDataBase_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_lineEditEditor_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_lineEditHelp_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_lineEditInfo_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_lineEditPlugins_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_lineEditQSS_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_lineEditScripts_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_lineEditSearch_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_pushButtonDataBase_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getDataBasePath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditDataBase->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonEditor_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getEditorPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditEditor->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonHelp_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getHelpPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditHelp->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonInfo_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getInfoPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditInfo->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonPlugins_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getPluginsPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditPlugins->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonQSS_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getQSSPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditQSS->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonScripts_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getScriptsPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditScripts->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonSearch_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getSearchPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditSearch->setText(sDirectoryName);
    }
}

void DialogOptions::on_comboBoxLanguage_currentIndexChanged(int index)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_pushButtonLangs_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),Utils::getLangsPath(pOptions),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditLangs->setText(sDirectoryName);
    }
}

void DialogOptions::on_lineEditLangs_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxLanguage_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
    ui->comboBoxLanguage->setEnabled(ui->checkBoxLanguage->isChecked());
}

void DialogOptions::on_groupBoxFont_toggled(bool arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxLanguage_toggled(bool checked)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_comboBoxMethod_currentIndexChanged(int index)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxDeepScanNFD_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxScanOverlayNFD_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxDeepScanDIE_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogOptions::on_checkBoxResizeColumnsToContent_stateChanged(int arg1)
{
    ui->pushButtonApply->setEnabled(true);
}
#ifdef USE_YARA
void DialogOptions::on_pushButtonYARADataBase_clicked()
{
    QString sFileName=QFileDialog::getOpenFileName(this, tr("Open file"),Utils::getDataBaseYARAPath(pOptions),tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        ui->lineEditDataBaseYARA->setText(sFileName);
    }
}
#endif
