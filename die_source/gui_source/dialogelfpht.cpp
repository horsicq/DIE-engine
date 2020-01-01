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
#include "dialogelfpht.h"
#include "ui_dialogelfpht.h"

DialogELFPHT::DialogELFPHT(QWidget *parent) :
    DialogELFGeneric(parent),
    ui(new Ui::DialogELFPHT)
{
    ui->setupUi(this);

    actEditHeader=new QAction(tr("Edit header"), this);
    actHex=new QAction(tr("Hex"), this);
    actDump=new QAction(tr("Dump"), this);
    actEntropy=new QAction(tr("Entropy"), this);
    actSearch=new QAction(tr("Search"), this);
    actCopyAsString=new QAction("", this);

    connect(actEditHeader,SIGNAL(triggered()),this,SLOT(editHeader()));
    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actDump,SIGNAL(triggered()),this,SLOT(dump()));
    connect(actEntropy,SIGNAL(triggered()),this,SLOT(entropy()));
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogELFPHT::~DialogELFPHT()
{
    delete ui;
}

bool DialogELFPHT::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    ui->tableWidgetSections->clear();

    QTableWidgetItem *newItem;

    ui->tableWidgetSections->setColumnCount(5);

    QStringList labels;

    labels.append("Type");
    labels.append("V.Address");
    labels.append("V.Size");
    labels.append("Offset");
    labels.append("R.Size");

    ui->tableWidgetSections->setHorizontalHeaderLabels(labels);

    int nNumberOfprograms=elffile->getElfHeader_phnum();

    ui->tableWidgetSections->setRowCount(nNumberOfprograms);

    if(elffile->isELF64())
    {
        for(int i=0; i<nNumberOfprograms; i++)
        {
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->programHeaderTypeToString(elffile->getProgramHeader64_type(i)).section("_",1,1)));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader64_vaddr(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader64_memsz(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader64_offset(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader64_filesz(i),16,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            ui->tableWidgetSections->setRowHeight(i,20);
        }
    }
    else
    {
        for(int i=0; i<nNumberOfprograms; i++)
        {
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->programHeaderTypeToString(elffile->getProgramHeader32_type(i)).section("_",1,1)));
            ui->tableWidgetSections->setItem(i,0,newItem);
            ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader32_vaddr(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,1,newItem);
            ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader32_memsz(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,2,newItem);
            ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader32_offset(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,3,newItem);
            ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(elffile->getProgramHeader32_filesz(i),8,16,QChar('0')));
            ui->tableWidgetSections->setItem(i,4,newItem);
            ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            ui->tableWidgetSections->setRowHeight(i,20);
        }
    }

    ui->tableWidgetSections->setColumnWidth(0,60);
    ui->tableWidgetSections->setColumnWidth(1,115);
    ui->tableWidgetSections->setColumnWidth(2,115);
    ui->tableWidgetSections->setColumnWidth(3,115);
    ui->tableWidgetSections->setColumnWidth(4,115);

    //    ui->tableWidgetSections->resizeColumnsToContents();

    return false;
}

void DialogELFPHT::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogELFPHT::on_tableWidgetSections_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetSections->rowCount())
    {
        return;
    }

    int nprogram=ui->tableWidgetSections->selectedItems().at(0)->row();
    bool bIsFile=false;

    if(elffile->isELF64())
    {
        if(elffile->getProgramHeader64_filesz(nprogram))
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
        if(elffile->getProgramHeader32_filesz(nprogram))
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

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));

    menu.addSeparator();
    menu.addAction(actEditHeader);
    menu.addAction(actSearch);
    menu.addAction(actEntropy);
    menu.addAction(actHex);
    menu.addAction(actDump);

    menu.exec(ui->tableWidgetSections->viewport()->mapToGlobal(pos));
}
void DialogELFPHT::editHeader()
{
    DialogELFProgramHeader dialogprogramheader(this);

    int nProgram=ui->tableWidgetSections->selectedItems().at(0)->row();

    dialogprogramheader.setData(pOptions,elffile,nProgram);

    dialogprogramheader.exec();
}

void DialogELFPHT::hex()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nprogram=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(elffile->isELF64())
    {
        nOffset=elffile->getProgramHeader64_offset(nprogram);
        nSize=elffile->getProgramHeader64_filesz(nprogram);
    }
    else
    {
        nOffset=elffile->getProgramHeader32_offset(nprogram);
        nSize=elffile->getProgramHeader32_filesz(nprogram);
    }

    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,elffile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Program(%1)").arg(nprogram));

    dialoghde.exec();
}

void DialogELFPHT::dump()
{
    int nprogram=ui->tableWidgetSections->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Program(%1)").arg(nprogram)), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        elffile->dumpProgram(sFileName,nprogram);
    }
}
void DialogELFPHT::entropy()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nprogram=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(elffile->isELF64())
    {
        nOffset=elffile->getProgramHeader64_offset(nprogram);
        nSize=elffile->getProgramHeader64_filesz(nprogram);
    }
    else
    {
        nOffset=elffile->getProgramHeader32_offset(nprogram);
        nSize=elffile->getProgramHeader32_filesz(nprogram);
    }

    DialogEntropy dialogentropy(this);

    dialogentropy.setData(pOptions,elffile,nOffset,nSize);

    dialogentropy.exec();
}

void DialogELFPHT::search()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nprogram=ui->tableWidgetSections->selectedItems().at(0)->row();

    if(elffile->isELF64())
    {
        nOffset=elffile->getProgramHeader64_offset(nprogram);
        nSize=elffile->getProgramHeader64_filesz(nprogram);
    }
    else
    {
        nOffset=elffile->getProgramHeader32_offset(nprogram);
        nSize=elffile->getProgramHeader32_filesz(nprogram);
    }

    DialogSearch dialog(this);

    dialog.setData(pOptions,elffile,nOffset,nSize);

    dialog.exec();
}
void DialogELFPHT::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));
}
