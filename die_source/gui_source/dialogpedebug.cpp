// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#include "dialogpedebug.h"
#include "ui_dialogpedebug.h"

DialogPEDebug::DialogPEDebug(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEDebug)
{
    ui->setupUi(this);

    actHex=new QAction(tr("Hex"), this);
    actDump=new QAction(tr("Dump"), this);
    actEditEntropy=new QAction(tr("Entropy"), this);
    actCopyAsString=new QAction("", this);

    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actDump,SIGNAL(triggered()),this,SLOT(dump()));
    connect(actEditEntropy,SIGNAL(triggered()),this,SLOT(entropy()));
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEDebug::~DialogPEDebug()
{
    delete ui;
}

void DialogPEDebug::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogPEDebug::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    QTableWidgetItem *newItem;
    //    pefile->setFileName(sFileName);
    ui->tableWidgetDebug->clear();

    QStringList labels;

    labels.append("Characteristics");
    labels.append("TimeDateStamp");
    labels.append("MajorVer");
    labels.append("MinorVer");
    labels.append("Type");
    labels.append("SizeOfData");
    labels.append("V.Address");
    labels.append("Offset");

    nNumberOfDebugs=pefile->getDebug_NumberOfDirectories();
    ui->tableWidgetDebug->setRowCount(nNumberOfDebugs);
    ui->tableWidgetDebug->setColumnCount(labels.count());

    ui->tableWidgetDebug->setHorizontalHeaderLabels(labels);

    for(int i=0; i<nNumberOfDebugs; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getDebug_Characteristics(i),8,16,QChar('0')));
        ui->tableWidgetDebug->setItem(i,0,newItem);
        ui->tableWidgetDebug->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getDebug_TimeDateStamp(i),8,16,QChar('0')));
        ui->tableWidgetDebug->setItem(i,1,newItem);
        ui->tableWidgetDebug->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getDebug_MajorVersion(i),4,16,QChar('0')));
        ui->tableWidgetDebug->setItem(i,2,newItem);
        ui->tableWidgetDebug->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getDebug_MinorVersion(i),4,16,QChar('0')));
        ui->tableWidgetDebug->setItem(i,3,newItem);
        ui->tableWidgetDebug->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(numberToType(pefile->getDebug_Type(i)));
        ui->tableWidgetDebug->setItem(i,4,newItem);
        ui->tableWidgetDebug->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getDebug_SizeOfData(i),8,16,QChar('0')));
        ui->tableWidgetDebug->setItem(i,5,newItem);
        ui->tableWidgetDebug->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getDebug_AddressOfRawData(i),8,16,QChar('0')));
        ui->tableWidgetDebug->setItem(i,6,newItem);
        ui->tableWidgetDebug->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getDebug_PointerToRawData(i),8,16,QChar('0')));
        ui->tableWidgetDebug->setItem(i,7,newItem);
        ui->tableWidgetDebug->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetDebug->setRowHeight(i,20);
    }

    ui->tableWidgetDebug->setColumnWidth(0,85);
    ui->tableWidgetDebug->setColumnWidth(1,85);
    ui->tableWidgetDebug->setColumnWidth(2,85);
    ui->tableWidgetDebug->setColumnWidth(3,85);
    ui->tableWidgetDebug->setColumnWidth(4,85);
    ui->tableWidgetDebug->setColumnWidth(5,85);
    ui->tableWidgetDebug->setColumnWidth(6,85);
    ui->tableWidgetDebug->setColumnWidth(7,85);

    return true;
}

QString DialogPEDebug::numberToType(unsigned int nType)
{
    QString sResult=QString::number(nType);

    if(nType==0)
    {
        sResult="UNKNOWN";
    }
    else if(nType==1)
    {
        sResult="COFF";
    }
    else if(nType==2)
    {
        sResult="CODEVIEW";
    }
    else if(nType==3)
    {
        sResult="FPO";
    }
    else if(nType==4)
    {
        sResult="MISC";
    }
    else if(nType==5)
    {
        sResult="EXCEPTION";
    }
    else if(nType==6)
    {
        sResult="FIXUP";
    }
    else if(nType==9)
    {
        sResult="BORLAND";
    }

    return sResult;
}

void DialogPEDebug::hex()
{
    DialogHDE dialoghde(this);

    int nDebug=ui->tableWidgetDebug->selectedItems().at(0)->row();

    //    dialoghde.setData(pefile->getFileName(),pefile->getSection_PointerToRawData(nSection),pefile->getSection_SizeOfRawData(nSection),pefile->getDisasmMode(),pOptions);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->getDebug_PointerToRawData(nDebug),pefile->getDebug_SizeOfData(nDebug),tr("Debug"));

    dialoghde.exec();
}

void DialogPEDebug::dump()
{
    int nDebug=ui->tableWidgetDebug->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("DebugDirectory(%1)").arg(nDebug)), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        pefile->dump(sFileName,pefile->getDebug_PointerToRawData(nDebug),pefile->getDebug_SizeOfData(nDebug));
    }
}
void DialogPEDebug::entropy()
{
    DialogEntropy dialogentropy(this);

    int nDebug=ui->tableWidgetDebug->selectedItems().at(0)->row();
    dialogentropy.setData(pOptions,pefile,pefile->getDebug_PointerToRawData(nDebug),pefile->getDebug_SizeOfData(nDebug));

    dialogentropy.exec();
}
void DialogPEDebug::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetDebug->selectedItems().at(0)->text()));
}

void DialogPEDebug::on_tableWidgetDebug_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetDebug->selectedItems().count())
    {
        return;
    }

    int nDebug=ui->tableWidgetDebug->selectedItems().at(0)->row();

    //     QString sString=((QTableWidgetItem *)(ui->tableWidgetSections->focusWidget()))->text();
    //     qDebug(sString.toAscii().data());

    if(pefile->getDebug_SizeOfData(nDebug))
    {
        actHex->setEnabled(true);
        actDump->setEnabled(true);
        actEditEntropy->setEnabled(true);
    }
    else
    {
        actHex->setEnabled(false);
        actDump->setEnabled(false);
        actEditEntropy->setEnabled(false);
    }

    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetDebug->selectedItems().at(0)->text()));

    menu.addSeparator();
    menu.addAction(actEditEntropy);
    menu.addAction(actHex);
    menu.addAction(actDump);

    menu.exec(ui->tableWidgetDebug->viewport()->mapToGlobal(pos));
}
