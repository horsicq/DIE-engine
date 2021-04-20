// Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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

DialogOptions::DialogOptions(QWidget *pParent, XOptions *pOptions) :
    QDialog(pParent),
    ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

    this->pOptions=pOptions;

    ui->listWidgetOptions->insertItem(TAB_GENERAL,      tr("General"));
    ui->listWidgetOptions->insertItem(TAB_SCAN,         tr("Scan"));
    ui->listWidgetOptions->insertItem(TAB_APPEARANCE,   tr("Appearance"));
#ifdef Q_OS_WIN
    ui->listWidgetOptions->insertItem(TAB_CONTEXT,      tr("Context"));
#endif

    pOptions->setCheckBox(ui->checkBoxStayOnTop,XOptions::ID_STAYONTOP);
    pOptions->setCheckBox(ui->checkBoxSaveLastDirectory,XOptions::ID_SAVELASTDIRECTORY);
    pOptions->setCheckBox(ui->checkBoxSaveBackup,XOptions::ID_SAVEBACKUP);
    pOptions->setCheckBox(ui->checkBoxSingleApplication,XOptions::ID_SINGLEAPPLICATION);

    pOptions->setCheckBox(ui->checkBoxScanAfterOpen,XOptions::ID_SCANAFTEROPEN);
    pOptions->setComboBox(ui->comboBoxScanEngine,XOptions::ID_SCANENGINE);
    pOptions->setLineEdit(ui->lineEditDIEDatabase,XOptions::ID_DATABASEPATH);
    pOptions->setLineEdit(ui->lineEditDIEInfo,XOptions::ID_INFOPATH);
    pOptions->setLineEdit(ui->lineEditSearchSignatures,XOptions::ID_SEARCHSIGNATURESPATH);

    pOptions->setComboBox(ui->comboBoxStyle,XOptions::ID_STYLE);
    pOptions->setComboBox(ui->comboBoxQss,XOptions::ID_QSS);
    pOptions->setComboBox(ui->comboBoxLanguage,XOptions::ID_LANG);

    ui->stackedWidgetOptions->setCurrentIndex(TAB_GENERAL);
#ifdef Q_OS_WIN
    contextState();
#endif
}

DialogOptions::~DialogOptions()
{
    delete ui;
}
#ifdef Q_OS_WIN
void DialogOptions::contextState()
{
    bool bState=pOptions->checkContext(X_APPLICATIONNAME,"*");

    ui->pushButtonRegister->setEnabled(!bState);
    ui->pushButtonClear->setEnabled(bState);
}
#endif
void DialogOptions::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogOptions::on_pushButtonOK_clicked()
{
    pOptions->getCheckBox(ui->checkBoxStayOnTop,XOptions::ID_STAYONTOP);
    pOptions->getCheckBox(ui->checkBoxSaveLastDirectory,XOptions::ID_SAVELASTDIRECTORY);
    pOptions->getCheckBox(ui->checkBoxSaveBackup,XOptions::ID_SAVEBACKUP);
    pOptions->getCheckBox(ui->checkBoxSingleApplication,XOptions::ID_SINGLEAPPLICATION);

    pOptions->getCheckBox(ui->checkBoxScanAfterOpen,XOptions::ID_SCANAFTEROPEN);
    pOptions->getComboBox(ui->comboBoxScanEngine,XOptions::ID_SCANENGINE);
    pOptions->getLineEdit(ui->lineEditDIEDatabase,XOptions::ID_DATABASEPATH);
    pOptions->getLineEdit(ui->lineEditDIEInfo,XOptions::ID_INFOPATH);
    pOptions->getLineEdit(ui->lineEditSearchSignatures,XOptions::ID_SEARCHSIGNATURESPATH);

    pOptions->getComboBox(ui->comboBoxStyle,XOptions::ID_STYLE);
    pOptions->getComboBox(ui->comboBoxQss,XOptions::ID_QSS);
    pOptions->getComboBox(ui->comboBoxLanguage,XOptions::ID_LANG);

    if(pOptions->isRestartNeeded())
    {
        QMessageBox::information(this,tr("Information"),tr("Please restart the application"));
    }

    this->close();
}

void DialogOptions::on_listWidgetOptions_currentRowChanged(int nCurrentRow)
{
    ui->stackedWidgetOptions->setCurrentIndex(nCurrentRow);
}

void DialogOptions::on_toolButtonDIEDatabase_clicked()
{
    QString sText=ui->lineEditDIEDatabase->text();
    QString sInitDirectory=XBinary::convertPathName(sText);

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory")+QString("..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditDIEDatabase->setText(sDirectoryName);
    }
}

void DialogOptions::on_pushButtonRegister_clicked()
{
#ifdef Q_OS_WIN
    pOptions->registerContext(X_APPLICATIONDISPLAYNAME,"*",qApp->applicationFilePath());

    contextState();
#endif
}

void DialogOptions::on_pushButtonClear_clicked()
{
#ifdef Q_OS_WIN
    pOptions->clearContext(X_APPLICATIONDISPLAYNAME,"*");

    contextState();
#endif
}
void DialogOptions::on_toolButtonDIEInfo_clicked()
{
    QString sText=ui->lineEditDIEInfo->text();
    QString sInitDirectory=XBinary::convertPathName(sText);

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory")+QString("..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditDIEInfo->setText(sDirectoryName);
    }
}

void DialogOptions::on_toolButtonSearchSignatures_clicked()
{
    QString sText=ui->lineEditSearchSignatures->text();
    QString sInitDirectory=XBinary::convertPathName(sText);

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory")+QString("..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditSearchSignatures->setText(sDirectoryName);
    }
}
