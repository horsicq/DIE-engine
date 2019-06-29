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
#include "dialogdirectoryheuristicscan.h"
#include "ui_dialogdirectoryheuristicscan.h"


QString seconds_to_DHMS(quint32 duration)
{
    QString res;
    int seconds = (int)(duration % 60);
    duration /= 60;
    int minutes = (int)(duration % 60);
    duration /= 60;
    int hours = (int)(duration % 24);
    int days = (int)(duration / 24);

    if((hours == 0)&&(days == 0))
    {
        return res.sprintf("%02d:%02d", minutes, seconds);
    }

    if(days == 0)
    {
        return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
    }

    return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

DialogDirectoryHeuristicScan::DialogDirectoryHeuristicScan(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogDirectoryHeuristicScan)
{
    ui->setupUi(this);

    ui->lineEditDirectory->setHex(false);
    ui->lineEditTime->setHex(false);
    ui->lineEditScanned->setHex(false);
    ui->lineEditScanned->setDec(true);
    ui->lineEditTotal->setHex(false);
    ui->lineEditTotal->setDec(true);

    this->setWindowFlags(this->windowFlags()|Qt::WindowMinMaxButtonsHint);

    connect(this,SIGNAL(pushButtonScan_click()),this,SLOT(on_pushButtonScan_clicked()),Qt::QueuedConnection);

    timer.setInterval(1000);
    connect(&timer,SIGNAL(timeout()),this,SLOT(setEnabled()));

    setProgressBar1(1,1);
    setProgressBar2(1,1);

    bIsScan=false;

    ui->lineEditTotal->setText("0");
    ui->lineEditScanned->setText("0");
    ui->lineEditTime->setText("00:00");

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonSave->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonScan->setShortcut(QKeySequence("Ctrl+F"));
    ui->pushButtonDirectory->setShortcut(QKeySequence(__KeySequence_open));
}

DialogDirectoryHeuristicScan::~DialogDirectoryHeuristicScan()
{
    emit reset();

    delete ui;
}

void DialogDirectoryHeuristicScan::setData(__DIE_OPTIONS *pOptions)
{
    setOptions(pOptions);
}

bool DialogDirectoryHeuristicScan::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->pushButtonSave->setToolTip(tr("Save scan log"));
        ui->pushButtonDirectory->setToolTip(tr("Open directory for scan"));
        ui->pushButtonScan->setToolTip(tr("Scan directory"));
        ui->textEditResult->setToolTip(tr("Scan log"));
        ui->progressBar1->setToolTip(tr("File progress"));
        ui->progressBar2->setToolTip(tr("Directory progress"));
        ui->lineEditDirectory->setToolTip(tr("Directory for scan"));
        ui->lineEditScanned->setToolTip(tr("Number of scanned files"));
        ui->lineEditTime->setToolTip(tr("Scan time"));
        ui->lineEditTotal->setToolTip(tr("Number of files in directory"));
        ui->checkBoxShowErrors->setToolTip(tr("Write errors to log"));
        ui->checkBoxSubdirectories->setToolTip(tr("Scan with subdirectories"));
    }

    ui->lineEditDirectory->setText(Utils::getLastDirecory(pOptions));

    return true;
}

void DialogDirectoryHeuristicScan::singleScanCompleteSlot(int nMs)
{
    QString sText;

    if(pOptions->sm==SM_DIE)
    {
        QList<__DIE_RESULT> listResult=pOptions->die_listResult;

        for(int i=0; i<listResult.count(); i++)
        {
            __DIE_RESULT record=listResult.at(i);

            sText+=QString("%1: %2: %3\r\n").arg(record.sFileType).arg(record.sType).arg(record.sName);
        }
    }

#ifdef USE_NFD
    else if(pOptions->sm==SM_NFD)
    {
        QList<SpecAbstract::SCAN_STRUCT> listResult=pOptions->nfd_result.listRecords;

        StaticScanItemModel model(&listResult);
        sText=model.toFormattedString();
    }

#endif
#ifdef USE_YARA
    else if(pOptions->sm==SM_YARA)
    {
        QList<QString> listResult=pOptions->yara_result.listRecords;

        int nCount=listResult.count();

        for(int i=0; i<nCount; i++)
        {
            sText+=QString("    %1\r\n").arg(listResult.at(i));
        }
    }

#endif
    ui->textEditResult->append(sText);

    if(pOptions->pMutexResult)
    {
        pOptions->pMutexResult->unlock();
    }
}

void DialogDirectoryHeuristicScan::setDirectory(QString sText)
{
    ui->lineEditDirectory->setText(sText);
    emit pushButtonScan_click();
}

void DialogDirectoryHeuristicScan::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogDirectoryHeuristicScan::on_pushButtonDirectory_clicked()
{
    QString sDirectoryName=QFileDialog::getExistingDirectory(this, tr("Open directory"),ui->lineEditDirectory->text(),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(!sDirectoryName.isEmpty())
    {
        sDirectoryName+=QDir::separator();
        ui->lineEditDirectory->setText(sDirectoryName);
    }
}

void DialogDirectoryHeuristicScan::setProgressBar1(int nMax, int nValue)
{
    ui->progressBar1->setMaximum(nMax);
    ui->progressBar1->setValue(nValue);
}

void DialogDirectoryHeuristicScan::setProgressBar2(int nMax, int nValue)
{
    ui->progressBar2->setMaximum(nMax);
    ui->progressBar2->setValue(nValue);

    if(nMax==nValue)
    {
        ui->pushButtonScan->setText(tr("Scan"));
        bIsScan=false;
        ui->pushButtonScan->setEnabled(true);
        ui->pushButtonDirectory->setEnabled(true);
    }
}

void DialogDirectoryHeuristicScan::on_pushButtonScan_clicked()
{
    if(bIsScan)
    {
        timer.stop();
        ui->pushButtonScan->setEnabled(false);
        timer.start();

        _reset();

        ui->pushButtonScan->setText(tr("Scan"));
        ui->pushButtonDirectory->setEnabled(true);

        bIsScan=false;
    }
    else
    {
        ui->lineEditTotal->setText("0");
        ui->lineEditScanned->setText("0");
        nScanned=0;
        starttime=QDateTime::currentDateTime();

        pOptions->sLastDirectory=ui->lineEditDirectory->text();

        dirContent.clear();

        td=new ThreadDirectory;

        connect(td,SIGNAL(setTotal(int)),this,SLOT(setTotal(int)));
        connect(td,SIGNAL(_finished()),this,SLOT(startScan()));

        QThread* thread =new QThread;
        td->moveToThread(thread);

        connect(this, SIGNAL(reset()), td, SLOT(_stop()),Qt::DirectConnection);

        connect(thread, SIGNAL(started()), td, SLOT(process()));
        connect(td, SIGNAL(_finished()), thread, SLOT(quit()));
        connect(td, SIGNAL(_finished()), td, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        td->setData(ui->lineEditDirectory->text(),ui->checkBoxSubdirectories->isChecked(),&dirContent);
        thread->start();

        ui->textEditResult->clear();
        ui->pushButtonScan->setText(tr("Stop"));

        ui->pushButtonScan->setEnabled(false);
        timer.start();

        ui->pushButtonDirectory->setEnabled(false);

        bIsScan=true;
    }
}

// DialogDirectoryHeuristicScan::appendText(QString sText) // Obsolete
//{
//    ui->textEditResult->append(sText);
//}

void DialogDirectoryHeuristicScan::appendError(QString sText)
{
    if(ui->checkBoxShowErrors->isChecked())
    {
        ui->textEditResult->append(QString("<font color=\"red\">[%1]%2</font>").arg(tr("Error")).arg(sText));
    }

}
void DialogDirectoryHeuristicScan::appendFileName(QString sText)
{
    nScanned++;
    ui->lineEditScanned->setText(QString("%1").arg(nScanned));

    ui->lineEditTime->setText(seconds_to_DHMS(starttime.secsTo(QDateTime::currentDateTime())));
    //    ui->textEditResult->append(QString("<font color=\"green\">%1</font>").arg(sText));
    ui->textEditResult->append(sText);
}

void DialogDirectoryHeuristicScan::setEnabled()
{
    timer.stop();
    ui->pushButtonScan->setEnabled(true);
}

void DialogDirectoryHeuristicScan::on_pushButtonSave_clicked()
{
    QFile file;
    QString sFileName;
    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.txt").arg("dirscan"), tr("Text files (*.txt);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            file.resize(0);
            file.write(ui->textEditResult->toPlainText().replace("\n","\r\n").toLatin1());
            file.close();
        }
    }
}

void DialogDirectoryHeuristicScan::startScan()
{
    scan=new Scan;

    QThread* thread =new QThread;
    scan->moveToThread(thread);

    connect(this, SIGNAL(reset()), scan, SLOT(_stop()),Qt::DirectConnection);

    connect(scan,SIGNAL(appendError(QString)),this,SLOT(appendError(QString)));
    connect(scan,SIGNAL(appendFileName(QString)),this,SLOT(appendFileName(QString)));
    connect(scan,SIGNAL(singleScanComplete(int)),this,SLOT(singleScanCompleteSlot(int)));
    connect(scan,SIGNAL(setProgressBar(int,int)),this,SLOT(setProgressBar1(int,int)));
    connect(scan,SIGNAL(setProgressBar2(int,int)),this,SLOT(setProgressBar2(int,int)));

    connect(thread, SIGNAL(started()), scan, SLOT(process()));
    connect(scan, SIGNAL(_finished()), thread, SLOT(quit()));
    connect(scan, SIGNAL(_finished()), scan, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    scan->setData(&dirContent,pOptions);

    thread->start();
}

void DialogDirectoryHeuristicScan::setTotal(int nTotal)
{
    ui->lineEditTotal->setText(QString("%1").arg(nTotal));
}

void DialogDirectoryHeuristicScan::_reset()
{
    emit reset();
}
