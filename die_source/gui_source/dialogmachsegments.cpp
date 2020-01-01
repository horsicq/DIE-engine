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
#include "dialogmachsegments.h"
#include "ui_dialogmachsegments.h"

DialogMACHSegments::DialogMACHSegments(QWidget *parent) :
    DialogMACHGeneric(parent),
    ui(new Ui::DialogMACHSegments)
{
    ui->setupUi(this);

    actEditHeader=new QAction(tr("Edit header"), this);
    actHex=new QAction(tr("Hex"), this);
    actDump=new QAction(tr("Dump"), this);
    actEntropy=new QAction(tr("Entropy"), this);
    actCopyAsString=new QAction("", this);
    actSearch=new QAction(tr("Search"), this);

    connect(actEditHeader,SIGNAL(triggered()),this,SLOT(editHeader()));
    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actDump,SIGNAL(triggered()),this,SLOT(dump()));
    connect(actEntropy,SIGNAL(triggered()),this,SLOT(entropy()));
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogMACHSegments::~DialogMACHSegments()
{
    delete ui;
}
//void DialogMACHSegments::setData(ELFFile *elffile, __OPTIONS *pOptions)
//{
//    this->elffile=elffile;
//    this->pOptions=pOptions;

//    reload();
//}

bool DialogMACHSegments::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        //        ui->checkBoxReadOnly->setToolTip("Set/unset read only mode");
    }

    ui->tableWidgetSections->clear();

    QTableWidgetItem *newItem;

    QStringList labels;

    labels.append("segname");
    labels.append("vmaddr");
    labels.append("vmsize");
    labels.append("fileoff");
    labels.append("filesize");
    labels.append("maxprot");
    labels.append("initprot");
    labels.append("nsects");
    labels.append("flags");

    ui->tableWidgetSections->setColumnCount(labels.count());
    ui->tableWidgetSections->setHorizontalHeaderLabels(labels);

    int nNumberOfSections=machfile->getNumberOfSegments();

    ui->tableWidgetSections->setRowCount(nNumberOfSections);

    for(int i=0; i<nNumberOfSections; i++)
    {
        if(machfile->isMACH64())
        {
            newItem = new QTableWidgetItem;
            newItem->setText(machfile->getSegment_segname32(i));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_vmaddr64(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_vmsize64(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_fileoff64(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_filesize64(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_maxprot64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,5,newItem);
            ui->tableWidgetSections->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_initprot64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,6,newItem);
            ui->tableWidgetSections->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_nsects64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,7,newItem);
            ui->tableWidgetSections->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_flags64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,8,newItem);
            ui->tableWidgetSections->item(i,8)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        else
        {
            newItem = new QTableWidgetItem;
            newItem->setText(machfile->getSegment_segname32(i));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_vmaddr32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_vmsize32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_fileoff32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_filesize32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_maxprot32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,5,newItem);
            ui->tableWidgetSections->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_initprot32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,6,newItem);
            ui->tableWidgetSections->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_nsects32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,7,newItem);
            ui->tableWidgetSections->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSegment_flags32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,8,newItem);
            ui->tableWidgetSections->item(i,8)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }

        ui->tableWidgetSections->setRowHeight(i,20);
    }

    ui->tableWidgetSections->resizeColumnsToContents();

    return false;
}

void DialogMACHSegments::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogMACHSegments::on_tableWidgetSections_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetSections->rowCount())
    {
        return;
    }

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    unsigned int nSize;

    if(machfile->isMACH64())
    {
        nSize=machfile->getSegment_filesize64(nSection);
    }
    else
    {
        nSize=machfile->getSegment_filesize32(nSection);
    }

    if(nSize)
    {
        actSearch->setEnabled(true);
        actHex->setEnabled(true);
        actDump->setEnabled(true);
        actEntropy->setEnabled(true);
    }
    else
    {
        actSearch->setEnabled(false);
        actHex->setEnabled(false);
        actDump->setEnabled(false);
        actEntropy->setEnabled(false);
    }

    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));

    menu.addSeparator();
    //menu.addAction(actEditHeader);
    menu.addAction(actSearch);
    menu.addAction(actEntropy);
    menu.addAction(actHex);
    menu.addAction(actDump);

    menu.exec(ui->tableWidgetSections->viewport()->mapToGlobal(pos));

}
void DialogMACHSegments::editHeader()
{
    //    DialogElfProgramHeader dialogprogramheader(this);

    //    int nProgram=ui->tableWidgetSections->selectedItems().at(0)->row();

    //    dialogprogramheader.setData(elffile,pOptions,nProgram);

    //    dialogprogramheader.exec();
}

void DialogMACHSegments::hex()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(machfile->isMACH64())
    {
        nOffset=machfile->getSegment_fileoff64(nSection);
        nSize=machfile->getSegment_filesize64(nSection);
    }
    else
    {
        nOffset=machfile->getSegment_fileoff32(nSection);
        nSize=machfile->getSegment_filesize32(nSection);
    }

    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,machfile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Segment(%1)").arg(nSection));

    dialoghde.exec();
}

void DialogMACHSegments::dump()
{
    int nSegment=ui->tableWidgetSections->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Segment(%1)").arg(nSegment)), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        machfile->dumpSegment(sFileName,nSegment);
    }
}

void DialogMACHSegments::entropy()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSegment=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(machfile->isMACH64())
    {
        nOffset=machfile->getSegment_fileoff64(nSegment);
        nSize=machfile->getSegment_filesize64(nSegment);
    }
    else
    {
        nOffset=machfile->getSegment_fileoff32(nSegment);
        nSize=machfile->getSegment_filesize32(nSegment);
    }

    DialogEntropy dialogentropy(this);

    dialogentropy.setData(pOptions,machfile,nOffset,nSize);

    dialogentropy.exec();
}

void DialogMACHSegments::search()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSegment=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(machfile->isMACH64())
    {
        nOffset=machfile->getSegment_fileoff64(nSegment);
        nSize=machfile->getSegment_filesize64(nSegment);
    }
    else
    {
        nOffset=machfile->getSegment_fileoff32(nSegment);
        nSize=machfile->getSegment_filesize32(nSegment);
    }

    DialogSearch dialog(this);

    dialog.setData(pOptions,machfile,nOffset,nSize);

    dialog.exec();
}

void DialogMACHSegments::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));
}
