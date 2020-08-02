// Copyright (c) 2020 hors<horsicq@gmail.com>
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

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));

    setAcceptDrops(true);

    xOptions.setName(X_OPTIONSFILE);

    QList<XOptions::ID> listIDs;

    listIDs.append(XOptions::ID_STYLE);
    listIDs.append(XOptions::ID_QSS);
    listIDs.append(XOptions::ID_LANG);
    listIDs.append(XOptions::ID_STAYONTOP);
    listIDs.append(XOptions::ID_SCANAFTEROPEN);
    listIDs.append(XOptions::ID_SAVELASTDIRECTORY);
    listIDs.append(XOptions::ID_LASTDIRECTORY);
    listIDs.append(XOptions::ID_SAVEBACKUP);
    listIDs.append(XOptions::ID_DBPATH);
    listIDs.append(XOptions::ID_INFOPATH);
    listIDs.append(XOptions::ID_SCANENGINE);

    xOptions.setValueIDs(listIDs);
    xOptions.load();

    adjust();

    if(QCoreApplication::arguments().count()>1)
    {
        QString sFileName=QCoreApplication::arguments().at(1);

        processFile(sFileName,true);
    }
}

GuiMainWindow::~GuiMainWindow()
{
    xOptions.save();

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
    DialogOptions dialogOptions(this,&xOptions);

    dialogOptions.exec();

    adjust();
    adjustFile();
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
            QHexView::OPTIONS hexOptions={};
            hexOptions.sBackupFileName=XBinary::getBackupName(&file);

            DialogHex dialogHex(this,&file,&hexOptions);

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
            // TODO options
            DialogSearchStrings dialogSearchStrings(this,&file,0,true);

            dialogSearchStrings.exec();

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
            DialogHash dialogHash(this,&file);

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
    xOptions.adjustStayOnTop(this);

    ui->widgetFormats->setDIEDatabasePath(xOptions.getDbPath());
    ui->widgetFormats->setDIEInfoPath(xOptions.getInfoPath());
    ui->widgetFormats->setScanEngine(xOptions.getScanEngine());
}

void GuiMainWindow::adjustFile()
{
    QString sFileName=getCurrentFileName();

    if(xOptions.isSaveBackup())
    {
        ui->widgetFormats->setBackupFileName(XBinary::getBackupName(sFileName));
    }
    else
    {
        ui->widgetFormats->setBackupFileName("");
    }

    xOptions.setLastDirectory(sFileName);
}

void GuiMainWindow::processFile(QString sFileName, bool bScan)
{
    ui->lineEditFileName->setText(sFileName);
    ui->widgetFormats->setData(sFileName,bScan);

    adjustFile();
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

            if(xOptions.isScanAfterOpen())
            {
                processFile(sFileName,xOptions.isScanAfterOpen());
            }
        }
    }
}

void GuiMainWindow::on_pushButtonOpenFile_clicked()
{
    QString sDirectory=xOptions.getLastDirectory();

    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file")+QString("..."),sDirectory,tr("All files")+QString(" (*)"));

    if(!sFileName.isEmpty())
    {
        if(xOptions.isScanAfterOpen())
        {
            processFile(sFileName,xOptions.isScanAfterOpen());
        }
    }
}
