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
#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *pParent)
    : QMainWindow(pParent)
    , ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONDISPLAYNAME).arg(X_APPLICATIONVERSION));

    setAcceptDrops(true);

    g_xOptions.setName(X_OPTIONSFILE);

    QList<XOptions::ID> listOptionsIDs;

    listOptionsIDs.append(XOptions::ID_STYLE);
    listOptionsIDs.append(XOptions::ID_QSS);
    listOptionsIDs.append(XOptions::ID_LANG);
    listOptionsIDs.append(XOptions::ID_STAYONTOP);
    listOptionsIDs.append(XOptions::ID_SCANAFTEROPEN);
    listOptionsIDs.append(XOptions::ID_SAVELASTDIRECTORY);
    listOptionsIDs.append(XOptions::ID_SINGLEAPPLICATION);
    listOptionsIDs.append(XOptions::ID_LASTDIRECTORY);
    listOptionsIDs.append(XOptions::ID_SAVEBACKUP);
    listOptionsIDs.append(XOptions::ID_DATABASEPATH);
    listOptionsIDs.append(XOptions::ID_INFOPATH);
    listOptionsIDs.append(XOptions::ID_SCANENGINE);
    listOptionsIDs.append(XOptions::ID_SEARCHSIGNATURESPATH);

    g_xOptions.setValueIDs(listOptionsIDs);
    g_xOptions.load();

    g_xShortcuts.setName(X_SHORTCUTSFILE);
    g_xShortcuts.setNative(g_xOptions.isNative());

    g_xShortcuts.addGroup(XShortcuts::ID_STRINGS);
    g_xShortcuts.addGroup(XShortcuts::ID_SIGNATURES);
    g_xShortcuts.addGroup(XShortcuts::ID_HEX);
    g_xShortcuts.addGroup(XShortcuts::ID_DISASM);
    g_xShortcuts.addGroup(XShortcuts::ID_ARCHIVE);
    g_xShortcuts.load();

    adjust();

    if(QCoreApplication::arguments().count()>1)
    {
        QString sFileName=QCoreApplication::arguments().at(1);

        processFile(sFileName);
    }
}

GuiMainWindow::~GuiMainWindow()
{
    g_xOptions.save();
    g_xShortcuts.save();

    delete ui;
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonAbout_clicked()
{
    DialogAbout dialogAbout(this);

    dialogAbout.exec();
}

void GuiMainWindow::on_pushButtonOptions_clicked()
{
    DialogOptions dialogOptions(this,&g_xOptions);

    dialogOptions.exec();

    adjust();
    adjustFile();
}

void GuiMainWindow::on_pushButtonMIME_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(XBinary::tryToOpen(&file))
        {
            DialogMIME dialogMIME(this,&file);
            dialogMIME.setShortcuts(&g_xShortcuts);

            dialogMIME.exec();

            file.close();
        }
    }
}

void GuiMainWindow::on_pushButtonHex_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(XBinary::tryToOpen(&file))
        {
            XHexView::OPTIONS hexOptions={};
            hexOptions.sSignaturesPath=g_xOptions.getSearchSignaturesPath();
//            hexOptions.sBackupFileName=XBinary::getBackupName(&file);

            DialogHexView dialogHex(this,&file,hexOptions);
            dialogHex.setShortcuts(&g_xShortcuts);

            dialogHex.exec();

            file.close();
        }
    }
}

void GuiMainWindow::on_pushButtonStrings_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            SearchStringsWidget::OPTIONS stringsOptions={};
            stringsOptions.bAnsi=true;
            stringsOptions.bUnicode=true;

            DialogSearchStrings dialogSearchStrings(this,&file,stringsOptions,true);
            dialogSearchStrings.setShortcuts(&g_xShortcuts);

            dialogSearchStrings.exec();

            file.close();
        }
    }
}

void GuiMainWindow::on_pushButtonSignatures_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            SearchSignaturesWidget::OPTIONS signaturesOptions={};
            signaturesOptions.sSignaturesPath=g_xOptions.getSearchSignaturesPath();

            DialogSearchSignatures dialogSearchSignatures(this,&file,XBinary::FT_UNKNOWN,signaturesOptions);
            dialogSearchSignatures.setShortcuts(&g_xShortcuts);

            dialogSearchSignatures.exec();

            file.close();
        }
    }
}

void GuiMainWindow::on_pushButtonEntropy_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            DialogEntropy dialogEntropy(this,&file);
            dialogEntropy.setShortcuts(&g_xShortcuts);

            dialogEntropy.exec();

            file.close();
        }
    }
}

void GuiMainWindow::on_pushButtonHash_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            DialogHash dialogHash(this,&file,XBinary::FT_UNKNOWN);
            dialogHash.setShortcuts(&g_xShortcuts);

            dialogHash.exec();

            file.close();
        }
    }
}

QString GuiMainWindow::getCurrentFileName()
{
    return ui->lineEditFileName->text();
}

void GuiMainWindow::adjust()
{
    g_xOptions.adjustStayOnTop(this);

    FormatsWidget::OPTIONS options={};

    options.sDatabasePath=g_xOptions.getDatabasePath();
    options.sInfoPath=g_xOptions.getInfoPath();
    options.bSaveBackup=g_xOptions.isSaveBackup();
    options.sSearchSignaturesPath=g_xOptions.getSearchSignaturesPath();

    ui->widgetFormats->setOptions(options);

    ui->widgetFormats->setScanEngine(g_xOptions.getScanEngine());

    ui->widgetFormats->setShortcuts(&g_xShortcuts);
    // TODO setShortcuts for mainWindow ...
}

void GuiMainWindow::adjustFile()
{
    QString sFileName=getCurrentFileName();

    g_xOptions.setLastDirectory(sFileName);
}

void GuiMainWindow::processFile(QString sFileName)
{
    ui->lineEditFileName->setText(sFileName);

    if(sFileName!="")
    {
        ui->widgetFormats->setFileName(sFileName,g_xOptions.isScanAfterOpen());

        adjustFile();
    }
}

void GuiMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData=event->mimeData();

    if(mimeData->hasUrls())
    {
        QList<QUrl> urlList=mimeData->urls();

        if(urlList.count())
        {
            QString sFileName=urlList.at(0).toLocalFile();

            sFileName=XBinary::convertFileName(sFileName);

            processFile(sFileName);
        }
    }
}

void GuiMainWindow::on_pushButtonOpenFile_clicked()
{
    QString sDirectory=g_xOptions.getLastDirectory();

    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file")+QString("..."),sDirectory,tr("All files")+QString(" (*)"));

    if(!sFileName.isEmpty())
    {
        processFile(sFileName);
    }
}

void GuiMainWindow::on_pushButtonShortcuts_clicked()
{
    DialogShortcuts dialogShortcuts(this);

    dialogShortcuts.setData(&g_xShortcuts);

    dialogShortcuts.exec();

    adjust();
}
