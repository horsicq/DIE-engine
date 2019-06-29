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
#include "dialogmachsections.h"
#include "ui_dialogmachsections.h"

DialogMACHSections::DialogMACHSections(QWidget *parent) :
    DialogMACHGeneric(parent),
    ui(new Ui::DialogMACHSections)
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

DialogMACHSections::~DialogMACHSections()
{
    delete ui;
}
//void DialogMACHSections::setData(ELFFile *elffile, __OPTIONS *pOptions)
//{
//    this->elffile=elffile;
//    this->pOptions=pOptions;

//    reload();
//}

bool DialogMACHSections::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        //        ui->checkBoxReadOnly->setToolTip("Set/unset read only mode");
    }

    ui->tableWidgetSections->clear();

    QTableWidgetItem *newItem;



    QStringList labels;

    labels.append("sectname");
    labels.append("segname");
    labels.append("addr");
    labels.append("size");
    labels.append("offset");
    labels.append("align");
    labels.append("reloff");
    labels.append("nreloc");
    labels.append("flags");


    ui->tableWidgetSections->setColumnCount(labels.count());
    ui->tableWidgetSections->setHorizontalHeaderLabels(labels);


    int nNumberOfSections=machfile->getNumberOfSections();

    ui->tableWidgetSections->setRowCount(nNumberOfSections);

    for(int i=0; i<nNumberOfSections; i++)
    {
        if(machfile->isMACH64())
        {
            newItem = new QTableWidgetItem;
            newItem->setText(machfile->getSection_sectname64(i));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(machfile->getSection_segname64(i));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_addr64(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_size64(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_offset64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_align64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,5,newItem);
            ui->tableWidgetSections->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_reloff64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,6,newItem);
            ui->tableWidgetSections->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_nreloc64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,7,newItem);
            ui->tableWidgetSections->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_flags64(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,8,newItem);
            ui->tableWidgetSections->item(i,8)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        else
        {
            newItem = new QTableWidgetItem;
            newItem->setText(machfile->getSection_sectname32(i));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(machfile->getSection_segname32(i));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_addr32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_size32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_offset32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_align32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,5,newItem);
            ui->tableWidgetSections->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_reloff32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,6,newItem);
            ui->tableWidgetSections->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_nreloc32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,7,newItem);
            ui->tableWidgetSections->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(machfile->getSection_flags32(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,8,newItem);
            ui->tableWidgetSections->item(i,8)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }

        ui->tableWidgetSections->setRowHeight(i,20);
    }

    ui->tableWidgetSections->resizeColumnsToContents();


    return false;
}

void DialogMACHSections::on_pushButtonOK_clicked()
{
    this->close();
}


void DialogMACHSections::on_tableWidgetSections_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetSections->rowCount())
    {
        return;
    }

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    unsigned int nSize;

    if(machfile->isMACH64())
    {
        nSize=machfile->getSection_size64(nSection);
    }
    else
    {
        nSize=machfile->getSection_size32(nSection);
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


    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetSections->mapToGlobal(pos2));

}
void DialogMACHSections::editHeader()
{
    //    DialogElfProgramHeader dialogprogramheader(this);

    //    int nProgram=ui->tableWidgetSections->selectedItems().at(0)->row();

    //    dialogprogramheader.setData(elffile,pOptions,nProgram);

    //    dialogprogramheader.exec();
}

void DialogMACHSections::hex()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(machfile->isMACH64())
    {
        nOffset=machfile->getSection_offset64(nSection);
        nSize=machfile->getSection_size64(nSection);
    }
    else
    {
        nOffset=machfile->getSection_offset32(nSection);
        nSize=machfile->getSection_size32(nSection);
    }

    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,machfile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Section(%1)").arg(nSection));

    dialoghde.exec();
}

void DialogMACHSections::dump()
{
    int nSegment=ui->tableWidgetSections->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Section(%1)").arg(nSegment)), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        machfile->dumpSection(sFileName,nSegment);
    }
}

void DialogMACHSections::entropy()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(machfile->isMACH64())
    {
        nOffset=machfile->getSection_offset64(nSection);
        nSize=machfile->getSection_size64(nSection);
    }
    else
    {
        nOffset=machfile->getSection_offset32(nSection);
        nSize=machfile->getSection_size32(nSection);
    }

    DialogEntropy dialogentropy(this);

    dialogentropy.setData(pOptions,machfile,nOffset,nSize);

    dialogentropy.exec();
}

void DialogMACHSections::search()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(machfile->isMACH64())
    {
        nOffset=machfile->getSection_offset64(nSection);
        nSize=machfile->getSection_size64(nSection);
    }
    else
    {
        nOffset=machfile->getSection_offset32(nSection);
        nSize=machfile->getSection_size32(nSection);
    }

    DialogSearch dialog(this);

    dialog.setData(pOptions,machfile,nOffset,nSize);

    dialog.exec();
}

void DialogMACHSections::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));
}

