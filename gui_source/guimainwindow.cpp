/* Copyright (c) 2020-2021 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *pParent)
    : QMainWindow(pParent)
    , ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(XOptions::getTitle(X_APPLICATIONDISPLAYNAME,X_APPLICATIONVERSION));

    setAcceptDrops(true);

    g_xOptions.setName(X_OPTIONSFILE);

#ifdef Q_OS_LINUX
    g_xOptions.addID(XOptions::ID_VIEW_QSS);
#else
    g_xOptions.addID(XOptions::ID_VIEW_QSS,"veles");
#endif
    g_xOptions.addID(XOptions::ID_VIEW_STYLE,"Fusion");
    g_xOptions.addID(XOptions::ID_VIEW_LANG,"System");
    g_xOptions.addID(XOptions::ID_VIEW_STAYONTOP,false);
    g_xOptions.addID(XOptions::ID_VIEW_SINGLEAPPLICATION,false);
    g_xOptions.addID(XOptions::ID_FILE_SAVELASTDIRECTORY,true);
    g_xOptions.addID(XOptions::ID_FILE_SAVEBACKUP,true);

#ifdef Q_OS_WIN32
    g_xOptions.addID(XOptions::ID_FILE_CONTEXT,"*");
#endif

    DIEOptionsWidget::setDefaultValues(&g_xOptions);
    SearchSignaturesOptionsWidget::setDefaultValues(&g_xOptions);
    XHexViewOptionsWidget::setDefaultValues(&g_xOptions);
    XDisasmViewOptionsWidget::setDefaultValues(&g_xOptions);

    g_xOptions.load();

    g_xShortcuts.setName(X_SHORTCUTSFILE);
    g_xShortcuts.setNative(g_xOptions.isNative());

    g_xShortcuts.addGroup(XShortcuts::GROUPID_STRINGS);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_SIGNATURES);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_HEX);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_DISASM);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_ARCHIVE);
    g_xShortcuts.load();

    ui->widgetFormats->setGlobal(&g_xShortcuts,&g_xOptions);

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
    ui->widgetFormats->adjustView();
    adjustFile();
}

void GuiMainWindow::on_pushButtonFileInfo_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            DialogXFileInfo dialogFileInfo(this);
            dialogFileInfo.setGlobal(&g_xShortcuts,&g_xOptions);

            dialogFileInfo.setData(&file,XBinary::FT_UNKNOWN,true);

            dialogFileInfo.exec();

            file.close();
        }
    }
}

void GuiMainWindow::on_pushButtonMIME_clicked()
{
    QString sFileName=getCurrentFileName();

    if(sFileName!="")
    {
        QFile file;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadOnly))
        {
            DialogMIME dialogMIME(this,&file);
            dialogMIME.setGlobal(&g_xShortcuts,&g_xOptions);

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

            DialogHexView dialogHex(this,&file,hexOptions);
            dialogHex.setGlobal(&g_xShortcuts,&g_xOptions);

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
            stringsOptions.bUTF8=false;
            stringsOptions.bUnicode=true;
            stringsOptions.bCStrings=true;

            DialogSearchStrings dialogSearchStrings(this);
            dialogSearchStrings.setData(&file,stringsOptions,true);
            dialogSearchStrings.setGlobal(&g_xShortcuts,&g_xOptions);

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

            DialogSearchSignatures dialogSearchSignatures(this);
            dialogSearchSignatures.setData(&file,XBinary::FT_UNKNOWN,signaturesOptions);
            dialogSearchSignatures.setGlobal(&g_xShortcuts,&g_xOptions);

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
            DialogEntropy dialogEntropy(this);
            dialogEntropy.setData(&file);
            dialogEntropy.setGlobal(&g_xShortcuts,&g_xOptions);

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
            DialogHash dialogHash(this);
            dialogHash.setData(&file,XBinary::FT_UNKNOWN);
            dialogHash.setGlobal(&g_xShortcuts,&g_xOptions);

            dialogHash.exec();

            file.close();
        }
    }
}

void GuiMainWindow::on_pushButtonDemangle_clicked()
{
    DialogDemangle dialogDemangle(this);

    dialogDemangle.exec();
}

QString GuiMainWindow::getCurrentFileName()
{
    return ui->lineEditFileName->text();
}

void GuiMainWindow::adjust()
{
    g_xOptions.adjustStayOnTop(this);

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
