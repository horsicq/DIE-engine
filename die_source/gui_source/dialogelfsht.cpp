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
#include "dialogelfsht.h"

#include "ui_dialogelfsht.h"

DialogELFSHT::DialogELFSHT(QWidget *parent) :
    DialogELFGeneric(parent),
    ui(new Ui::DialogELFSHT)
{
    ui->setupUi(this);

    actEditHeader=new QAction(tr("Edit header"), this);
    actHex=new QAction(tr("Hex"), this);
    actDump=new QAction(tr("Dump"), this);
    actEntropy=new QAction(tr("Entropy"), this);
    actCopyAsString=new QAction("", this);
    actStringTable=new QAction(tr("String Table"), this);
    actSearch=new QAction(tr("Search"), this);

    connect(actEditHeader,SIGNAL(triggered()),this,SLOT(editHeader()));
    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actDump,SIGNAL(triggered()),this,SLOT(dump()));
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));
    connect(actEntropy,SIGNAL(triggered()),this,SLOT(entropy()));
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));
    connect(actStringTable,SIGNAL(triggered()),this,SLOT(stringTable()));

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogELFSHT::~DialogELFSHT()
{
    delete ui;
}

bool DialogELFSHT::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    elffile->getStringTableAsList();

    QTableWidgetItem *newItem;

    ui->tableWidgetSections->setColumnCount(5);

    QStringList labels;

    labels.append("Name");
    labels.append("Type");
    labels.append("V.Address");
    labels.append("Offset");
    labels.append("Size");

    ui->tableWidgetSections->setHorizontalHeaderLabels(labels);

    int nNumberOfSections=elffile->getElfHeader_shnum();

    ui->tableWidgetSections->setRowCount(nNumberOfSections);

    if(elffile->isELF64())
    {
        for(int i=0; i<nNumberOfSections; i++)
        {
            newItem = new QTableWidgetItem;
            newItem->setText(elffile->getSectionName(i));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->sectionHeaderTypeToString(elffile->getSectionHeader64_type(i)).section("_",1,1)));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getSectionHeader64_addr(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getSectionHeader64_offset(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getSectionHeader64_size(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            ui->tableWidgetSections->setRowHeight(i,20);
        }
    }
    else
    {
        for(int i=0; i<nNumberOfSections; i++)
        {
            newItem = new QTableWidgetItem;
            newItem->setText(elffile->getSectionName(i));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->sectionHeaderTypeToString(elffile->getSectionHeader32_type(i)).section("_",1,1)));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getSectionHeader32_addr(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getSectionHeader32_offset(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getSectionHeader32_size(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            ui->tableWidgetSections->setRowHeight(i,20);
        }
    }

    ui->tableWidgetSections->setColumnWidth(0,100);
    ui->tableWidgetSections->setColumnWidth(1,60);
    ui->tableWidgetSections->setColumnWidth(2,115);
    ui->tableWidgetSections->setColumnWidth(3,115);
    ui->tableWidgetSections->setColumnWidth(4,115);
    //    ui->tableWidgetSections->resizeColumnsToContents();

    return true;
}

void DialogELFSHT::on_pushButtonOK_clicked()
{
    this->close();
}
void DialogELFSHT::on_tableWidgetSections_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetSections->selectedItems().count())
    {
        return;
    }

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();
    bool bIsFile=false;

    if(elffile->isELF64())
    {
        if(elffile->getSectionHeader64_size(nSection))
        {
            bIsFile=true;
        }
        else
        {
            bIsFile=false;
        }
    }
    else
    {
        if(elffile->getSectionHeader32_size(nSection))
        {
            bIsFile=true;
        }
        else
        {
            bIsFile=false;
        }
    }

    if(bIsFile)
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

    if(ui->tableWidgetSections->selectedItems().at(1)->text()=="STRTAB")
    {
        menu.addAction(actStringTable);
    }


    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));

    menu.addSeparator();
    menu.addAction(actEditHeader);
    menu.addAction(actSearch);
    menu.addAction(actEntropy);
    menu.addAction(actHex);
    menu.addAction(actDump);

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetSections->mapToGlobal(pos2));
}
void DialogELFSHT::editHeader()
{
    DialogELFSectionHeader dialogpesectionheader(this);

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    dialogpesectionheader.setData(pOptions,elffile,nSection);

    dialogpesectionheader.exec();
}

void DialogELFSHT::hex()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(elffile->isELF64())
    {
        nOffset=elffile->getSectionHeader64_offset(nSection);
        nSize=elffile->getSectionHeader64_size(nSection);
    }
    else
    {
        nOffset=elffile->getSectionHeader32_offset(nSection);
        nSize=elffile->getSectionHeader32_size(nSection);
    }

    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,elffile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Section(%1)").arg(nSection));

    dialoghde.exec();
}

void DialogELFSHT::dump()
{

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Section(%1)").arg(nSection)), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        elffile->dumpSection(sFileName,nSection);
    }
}
void DialogELFSHT::entropy()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(elffile->isELF64())
    {
        nOffset=elffile->getSectionHeader64_offset(nSection);
        nSize=elffile->getSectionHeader64_size(nSection);
    }
    else
    {
        nOffset=elffile->getSectionHeader32_offset(nSection);
        nSize=elffile->getSectionHeader32_size(nSection);
    }

    DialogEntropy dialogentropy(this);

    dialogentropy.setData(pOptions,elffile,nOffset,nSize);

    dialogentropy.exec();
}

void DialogELFSHT::search()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(elffile->isELF64())
    {
        nOffset=elffile->getSectionHeader64_offset(nSection);
        nSize=elffile->getSectionHeader64_size(nSection);
    }
    else
    {
        nOffset=elffile->getSectionHeader32_offset(nSection);
        nSize=elffile->getSectionHeader32_size(nSection);
    }

    DialogSearch dialog(this);

    dialog.setData(pOptions,elffile,nOffset,nSize);

    dialog.exec();
}
void DialogELFSHT::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));
}

void DialogELFSHT::stringTable()
{
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    DialogELFStrings dialog(this);

    dialog.setData(pOptions,elffile,nSection);

    dialog.exec();
}
