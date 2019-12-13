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
#include "dialogsignatures.h"
#include "ui_dialogsignatures.h"

DialogSignatures::DialogSignatures(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogSignatures)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()|Qt::WindowMinMaxButtonsHint);

    ui->pushButtonDebug->setShortcut(QKeySequence(__KeySequence_ctrlD));
    ui->pushButtonNew->setShortcut(QKeySequence(__KeySequence_ctrlN));
    ui->pushButtonRun->setShortcut(QKeySequence(__KeySequence_ctrlR));
    ui->pushButtonSave->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonClear->setShortcut(QKeySequence(__KeySequence_ctrlL));

    ui->checkBoxReadOnly->setChecked(true);
    ui->pushButtonSave->setEnabled(false);

    connect(&pluginscript,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&pefile,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&binary,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&elffile,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&msdosfile,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&machfile,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&text,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));

    connect(&scriptpe,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&scriptelf,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&scriptmsdos,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&scriptbinary,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&scriptmach,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));
    connect(&scripttext,SIGNAL(appendError(QString)),this,SLOT(appendLog(QString)));

    ui->lineEditResult->setHex(false);
    ui->lineEditTime->setHex(false);
    ui->lineEditTimeNumberOfSignatures->setHex(false);
    ui->lineEditType->setHex(false);

    actDelete=new QAction(QString("%1(Alt+D)").arg(tr("Delete")), this);
    connect(actDelete,SIGNAL(triggered()),this,SLOT(deleteSignature()));

    shortcutDelete = new QShortcut(QKeySequence(__KeySequence_altD), this);
    QObject::connect(shortcutDelete, SIGNAL(activated()), this, SLOT(deleteSignature()));

    actRename=new QAction(QString("%1(Alt+N)").arg(tr("Rename")), this);
    connect(actRename,SIGNAL(triggered()),this,SLOT(renameSignature()));

    shortcutRename = new QShortcut(QKeySequence(__KeySequence_altN), this);
    QObject::connect(shortcutRename, SIGNAL(activated()), this, SLOT(renameSignature()));

    pModel=0;

    bHighlightChanged=false;

    ui->checkBoxHighlight->setChecked(false);
}
QAbstractItemModel *DialogSignatures::modelFromFile(const QString& fileName)
{
    QString sLine;
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly))
    {
        return new QStringListModel(completer);
    }
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();

        if(!line.isEmpty())
        {
            sLine=line.trimmed();
            words << sLine.section(";",0,0);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words);
}

bool DialogSignatures::reload()
{
    return reload("");
}

DialogSignatures::~DialogSignatures()
{
    Scan::die_loadScripts(pOptions);

    if(pModel)
    {
        delete pModel;
    }

    delete ui;
}
void DialogSignatures::setData(__DIE_OPTIONS *pOptions,QString sFileName,QString sCurrentSignature)
{
    this->sFileName=sFileName;
    this->sCurrentSignature=sCurrentSignature;

    setOptions(pOptions);
}

bool DialogSignatures::reload(QString sNewCurrent)
{
    this->setWindowTitle(sFileName);

    if(pOptions->bShowTooltips)
    {
        ui->pushButtonDebug->setToolTip(tr("Debug signature"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->pushButtonNew->setToolTip(tr("Create new signature"));
        ui->pushButtonRun->setToolTip(tr("Run signature"));
        ui->pushButtonSave->setToolTip(tr("Save signature"));
        ui->pushButtonClear->setToolTip(tr("Clear log"));
    }

    ui->listWidgetSignatures->clear();

    sCurrent=sNewCurrent;

    Binary binary;

    QString sType;
    QListWidgetItem *pListWidget;

    if(binary.setFileName(sFileName))
    {
        sType=binary.getType();

        if(sType=="PE+(64)")
        {
            sFolder="PE";
        }
        else if(sType=="PE")
        {
            sFolder="PE";
        }
        else if(sType=="MSDOS")
        {
            sFolder="MSDOS";
        }
        else if(sType=="ELF")
        {
            sFolder="ELF";
        }
        else if(sType=="ELF64")
        {
            sFolder="ELF";
        }
        else if(sType=="MACH")
        {
            sFolder="MACH";
        }
        else if(sType=="MACH64")
        {
            sFolder="MACH";
        }
        else if(sType=="Binary")
        {
            sFolder="Binary";
        }
//        else if(sType=="Text")
//        {
//            sFolder="Text";
//        }
    }

    if(sFolder!="")
    {
        QDir dir(Utils::getDataBasePath(pOptions)+QDir::separator()+sFolder);
        QFileInfoList dircontent=dir.entryInfoList(QStringList()<<"*.sg");

        for(int i=0; i<dircontent.count(); i++)
        {
            pListWidget=new QListWidgetItem;

            if(dircontent.at(i).fileName()==sCurrentSignature)
            {
                sCurrentSignature="";
                sCurrent=dircontent.at(i).absoluteFilePath();
            }

            pListWidget->setText(dircontent.at(i).fileName());
            pListWidget->setData(Qt::UserRole,dircontent.at(i).absoluteFilePath());
            ui->listWidgetSignatures->addItem(pListWidget);
        }

        ui->lineEditTimeNumberOfSignatures->setText(QString("%1").arg(dircontent.count()));
        ui->lineEditType->setText(sFolder);

        completer = new QCompleter(this);

        pModel=modelFromFile(Utils::getEditorPath(pOptions)+QDir::separator()+"autocomplete"+QDir::separator()+sFolder+".lst");
        completer->setModel(pModel);
        completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
        completer->setCaseSensitivity(Qt::CaseSensitive);
        completer->setWrapAround(true);
        ui->textEditSignature->setCompleter(completer);

        QFile file(Utils::getEditorPath(pOptions)+QDir::separator()+"autocomplete"+QDir::separator()+sFolder+".lst");
        QString sLine;

        if(file.open(QFile::ReadOnly))
        {
            while(!file.atEnd())
            {
                QByteArray line = file.readLine();

                if(!line.isEmpty())
                {
                    sLine=line.trimmed();
                    listTooltips.append(sLine);
                }
            }

            file.close();
        }

        ui->textEditSignature->setData(&listTooltips,sFolder);
    }

    if(ui->listWidgetSignatures->count())
    {
        if(sCurrent=="")
        {
            ui->listWidgetSignatures->setCurrentRow(0);
        }
        else
        {
            for(int i=0; i<ui->listWidgetSignatures->count(); i++)
            {
                if(ui->listWidgetSignatures->item(i)->data(Qt::UserRole).toString()==sCurrent)
                {
                    ui->listWidgetSignatures->setCurrentRow(i);
                    break;
                }
            }
        }
    }

    return true;
}

void DialogSignatures::on_listWidgetSignatures_currentRowChanged(int currentRow)
{
    if(currentRow==-1)
    {
        return;
    }

    if(ui->pushButtonSave->isEnabled())
    {
        if(QMessageBox::information(this,tr("Information"),tr("Save signature(%1)?").arg(sCurrent),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        {
            save();
        }
    }

    QListWidgetItem *item=ui->listWidgetSignatures->currentItem();
    sCurrent=item->data(Qt::UserRole).toString();

    QFile file;

    ui->textEditSignature->clear();

    file.setFileName(sCurrent);

    if(file.open(QIODevice::ReadOnly))
    {
        ui->checkBoxReadOnly->setChecked(true);
        ui->textEditSignature->setReadOnly(true);
        ui->textEditSignature->setPlainText(file.readAll());
        file.close();
        ui->pushButtonSave->setEnabled(false);
    }
    else
    {
        appendLog(tr("Cannot open file: %1").arg(sFileName));
    }
}

void DialogSignatures::on_pushButtonNew_clicked()
{
    DialogNewSignature dns(this);
    QString sNew;

    dns.setData(pOptions,sFolder,&sNew);

    dns.exec();

    if(sNew!="")
    {
        sNew+=".sg";

        for(int i=0; i<ui->listWidgetSignatures->count(); i++)
        {
            if(ui->listWidgetSignatures->item(i)->text()==sNew)
            {
                delete ui->listWidgetSignatures->item(i);
                break;
            }
        }

        QListWidgetItem *pListWidget=new QListWidgetItem;
        pListWidget->setText(sNew);
        pListWidget->setData(Qt::UserRole,Utils::getDataBasePath(pOptions)+QDir::separator()+sFolder+QDir::separator()+sNew);
        ui->listWidgetSignatures->insertItem(0,pListWidget);

        ui->listWidgetSignatures->setCurrentRow(0);

        ui->checkBoxReadOnly->setChecked(false);
    }
}

void DialogSignatures::on_pushButtonSave_clicked()
{
    save();
}

void DialogSignatures::on_textEditSignature_textChanged()
{
    if(bHighlightChanged)
    {
        bHighlightChanged=false;
    }
    else
    {
        ui->pushButtonSave->setEnabled(true);
    }
}

void DialogSignatures::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    if(ui->listWidgetSignatures->count())
    {
        ui->textEditSignature->setReadOnly(ui->checkBoxReadOnly->isChecked());
    }
}

void DialogSignatures::on_pushButtonDebug_clicked()
{
    _run(true);
}

void DialogSignatures::on_pushButtonRun_clicked()
{
    _run(false);
}

void DialogSignatures::_run(bool bIsDebug)
{
    if(sCurrent!="")
    {
        if(ui->pushButtonSave->isEnabled())
        {
            if(QMessageBox::information(this,tr("Information"),tr("Save signature(%1)?").arg(sCurrent),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
            {
                save();
            }
        }

        if(sFolder=="PE")
        {
            pefile.setFileName(sFileName);

            pefile.entryPointLoad();

            if(pefile.isNETPresent())
            {
                pefile.entryPointLoad_NET();
            }

            scriptpe.setData(&pefile);
            pluginscript.addClass(&scriptpe,"PE",Utils::getDataBasePath(pOptions));
        }
        else if(sFolder=="ELF")
        {
            elffile.setFileName(sFileName);
            elffile.entryPointLoad();
            scriptelf.setData(&elffile);

            pluginscript.addClass(&scriptelf,"ELF",Utils::getDataBasePath(pOptions));
        }
        else if(sFolder=="MACH")
        {
            machfile.setFileName(sFileName);
            machfile.entryPointLoad();
            scriptmach.setData(&machfile);

            pluginscript.addClass(&scriptmach,"MACH",Utils::getDataBasePath(pOptions));
        }
        else if(sFolder=="MSDOS")
        {
            msdosfile.setFileName(sFileName);
            msdosfile.entryPointLoad();
            scriptmsdos.setData(&msdosfile);

            pluginscript.addClass(&scriptmsdos,"MSDOS",Utils::getDataBasePath(pOptions));
        }
        else if(sFolder=="Text")
        {
            text.setFileName(sFileName);
            scripttext.setData(&text);
            pluginscript.addClass(&scripttext,"Text",Utils::getDataBasePath(pOptions));
        }
        else if(sFolder=="Binary")
        {
            binary.setFileName(sFileName);
            scriptbinary.setData(&binary);
            pluginscript.addClass(&scriptbinary,"Binary",Utils::getDataBasePath(pOptions));
        }
        //        if(bIsDebug)
        //        {
        //            QScriptEngineDebugger debugger(this);
        //            QMainWindow *debugWindow=debugger.standardWindow();
        //            debugWindow->setWindowModality(Qt::ApplicationModal);
        //            debugWindow->setWindowTitle("DIEsignature debugger");
        //        //        debugWindow->resize(600,350);
        //            debugger.attachTo(&pluginscript);
        //            debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
        //        }

        QScriptValueList arg;

        arg<<ui->checkBoxType->isChecked()<<ui->checkBoxVersion->isChecked()<<ui->checkBoxOptions->isChecked();

        QFileInfo tfi;

        tfi.setFile(sCurrent);

        QString sInit=tfi.absolutePath()+QDir::separator()+"_init";

        QString sResult;

        if(bIsDebug)
        {
            QScriptEngineDebuggerX debugger(this);
            QMainWindow *debugWindow=debugger.standardWindow();
            debugWindow->setWindowModality(Qt::ApplicationModal);
            debugWindow->setWindowTitle("DIEsignature debugger");
            //        debugWindow->resize(600,350);
            debugger.attachTo(&pluginscript);

            if(QFile::exists(sInit))
            {
                pluginscript.evalFromFile(sInit);
            }

            debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();

            sResult=pluginscript.callFromFile(sCurrent,"detect",arg).toString();
        }
        else
        {
            QTime scanTime=QTime::currentTime();

            //            pluginscript.evalFromFile("F:\\prepare\\qt\\DIE_source\\db\\Binary\\_init");
            if(QFile::exists(sInit))
            {
                pluginscript.evalFromFile(sInit);
            }

            sResult=pluginscript.callFromFile(sCurrent,"detect",arg).toString();
            ui->lineEditTime->setText(tr("%1 ms").arg(scanTime.msecsTo(QTime::currentTime())));
        }

        ui->lineEditResult->setText(sResult);
    }
}

void DialogSignatures::appendLog(QString sString)
{
    ui->textEditLog->append(sString);
}


void DialogSignatures::on_pushButtonClear_clicked()
{
    ui->textEditLog->clear();
    ui->lineEditResult->clear();
}

void DialogSignatures::save()
{
    QByteArray baData;
    QFile file;

    file.setFileName(sCurrent);

    if(file.open(QIODevice::ReadWrite))
    {
        baData.append(ui->textEditSignature->toPlainText());

        file.resize(0);
        file.write(baData);
        file.close();

        ui->pushButtonSave->setEnabled(false);
    }
    else
    {
        ui->textEditLog->append(tr("Cannot save file: %1").arg(sCurrent));
    }
}

void DialogSignatures::deleteSignature()
{
    if(QMessageBox::information(this,tr("Information"),tr("Delete signature(%1)?").arg(sCurrent),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
    {
        if(QFile::remove(sCurrent))
        {
            reload();
        }
        else
        {
            appendLog(tr("Cannot delete signature: %1").arg(sCurrent));
        }
    }
}

void DialogSignatures::renameSignature()
{
    bool ok;
    QString sRenamed=ui->listWidgetSignatures->currentItem()->text();
    int nDots=sRenamed.count(".");

    if(!nDots)
    {
        nDots++;
    }

    sRenamed=sRenamed.section(".",0,nDots-1);
    QString text = QInputDialog::getText(this,tr("Rename signature"),
                                         tr("New name"), QLineEdit::Normal,sRenamed, &ok);

    if(ok && !text.isEmpty())
    {
        QString sNewName=ui->listWidgetSignatures->currentItem()->data(Qt::UserRole).toString().replace(ui->listWidgetSignatures->currentItem()->text(),text+".sg");

        if(QFile::rename(sCurrent,sNewName))
        {
            reload(sNewName);
        }
        else
        {
            appendLog(tr("Cannot rename signature: %1").arg(sCurrent));
        }

        //         qDebug(sNewName.toAscii().data());
    }

    //         textLabel->setText(text);

    //    if(QInputDialog::textValue()this,"Information",QString("Rename signature(%1)?").arg(sCurrent),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
    //    {
    //        if(QFile::rename(sCurrent) )
    //        {
    //            reload();
    //        }
    //        else
    //        {
    //            appendLog(QString("Cannot rename signature: %1").arg(sCurrent));
    //        }


    //    }
}

void DialogSignatures::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)

    if(ui->pushButtonSave->isEnabled())
    {
        if(QMessageBox::information(this,tr("Information"),tr("Save signature(%1)?").arg(sCurrent),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        {
            save();
        }
    }
}

void DialogSignatures::on_listWidgetSignatures_customContextMenuRequested(const QPoint &pos)
{
    if(sCurrent=="")
    {
        return;
    }

    QMenu menu;

    menu.addAction(actDelete);
    menu.addAction(actRename);

    menu.exec(ui->listWidgetSignatures->mapToGlobal(pos));
}

void DialogSignatures::on_checkBoxHighlight_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    bHighlightChanged=true;
    ui->textEditSignature->enableHighlight(ui->checkBoxHighlight->isChecked());
}
