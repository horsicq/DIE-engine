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
#include "dialogelfprogramheader.h"
#include "ui_dialogelfprogramheader.h"

DialogELFProgramHeader::DialogELFProgramHeader(QWidget *parent) :
    DialogELFGeneric(parent),
    ui(new Ui::DialogELFProgramHeader)
{
    ui->setupUi(this);

    QTableWidgetItem *newItem;

    ui->tableWidgetSectionHeader->setRowCount(8);
    ui->tableWidgetSectionHeader->setColumnCount(2);

    for(int i=0; i<8; i++)
    {
        ui->tableWidgetSectionHeader->setRowHeight(i,20);
    }

    lineEditsh_type=new QLineEditX;
    lineEditsh_flags=new QLineEditX;
    lineEditsh_offset=new QLineEditX;
    lineEditsh_vaddr=new QLineEditX;
    lineEditsh_paddr=new QLineEditX;
    lineEditsh_filesz=new QLineEditX;
    lineEditsh_memsz=new QLineEditX;
    lineEditsh_align=new QLineEditX;

    lineEditsh_type->setAlignment(Qt::AlignCenter);
    lineEditsh_flags->setAlignment(Qt::AlignCenter);
    lineEditsh_offset->setAlignment(Qt::AlignCenter);
    lineEditsh_vaddr->setAlignment(Qt::AlignCenter);
    lineEditsh_paddr->setAlignment(Qt::AlignCenter);
    lineEditsh_filesz->setAlignment(Qt::AlignCenter);
    lineEditsh_memsz->setAlignment(Qt::AlignCenter);
    lineEditsh_align->setAlignment(Qt::AlignCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("type");
    ui->tableWidgetSectionHeader->setItem(0,0,newItem);
    ui->tableWidgetSectionHeader->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("flags");
    ui->tableWidgetSectionHeader->setItem(1,0,newItem);
    ui->tableWidgetSectionHeader->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("offset");
    ui->tableWidgetSectionHeader->setItem(2,0,newItem);
    ui->tableWidgetSectionHeader->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("vaddr");
    ui->tableWidgetSectionHeader->setItem(3,0,newItem);
    ui->tableWidgetSectionHeader->item(3,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("paddr");
    ui->tableWidgetSectionHeader->setItem(4,0,newItem);
    ui->tableWidgetSectionHeader->item(4,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("filesz");
    ui->tableWidgetSectionHeader->setItem(5,0,newItem);
    ui->tableWidgetSectionHeader->item(5,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("memsz");
    ui->tableWidgetSectionHeader->setItem(6,0,newItem);
    ui->tableWidgetSectionHeader->item(6,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("align");
    ui->tableWidgetSectionHeader->setItem(7,0,newItem);
    ui->tableWidgetSectionHeader->item(7,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetSectionHeader->setCellWidget(0,1,lineEditsh_type);
    ui->tableWidgetSectionHeader->setCellWidget(1,1,lineEditsh_flags);
    ui->tableWidgetSectionHeader->setCellWidget(2,1,lineEditsh_offset);
    ui->tableWidgetSectionHeader->setCellWidget(3,1,lineEditsh_vaddr);
    ui->tableWidgetSectionHeader->setCellWidget(4,1,lineEditsh_paddr);
    ui->tableWidgetSectionHeader->setCellWidget(5,1,lineEditsh_filesz);
    ui->tableWidgetSectionHeader->setCellWidget(6,1,lineEditsh_memsz);
    ui->tableWidgetSectionHeader->setCellWidget(7,1,lineEditsh_align);

    connect(lineEditsh_type,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_flags,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_offset,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_vaddr,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_paddr,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_filesz,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_memsz,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_align,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    ui->tableWidgetSectionHeader->setColumnWidth(0,90);
    ui->tableWidgetSectionHeader->setColumnWidth(1,120);

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogELFProgramHeader::~DialogELFProgramHeader()
{
    delete ui;
}

void DialogELFProgramHeader::setData(__DIE_OPTIONS *pOptions,ELFFile *pelffile,int nProgram)
{
    this->nProgram=nProgram;

    DialogELFGeneric::setData(pOptions,pelffile);
}

bool DialogELFProgramHeader::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
    }

    if(elffile->isELF64())
    {
        lineEditsh_type->setText(QString("%1").arg(elffile->getProgramHeader64_type(nProgram),8,16,QChar('0')));
        lineEditsh_flags->setText(QString("%1").arg(elffile->getProgramHeader64_flags(nProgram),8,16,QChar('0')));
        lineEditsh_offset->setText(QString("%1").arg(elffile->getProgramHeader64_offset(nProgram),16,16,QChar('0')));
        lineEditsh_vaddr->setText(QString("%1").arg(elffile->getProgramHeader64_vaddr(nProgram),16,16,QChar('0')));
        lineEditsh_paddr->setText(QString("%1").arg(elffile->getProgramHeader64_paddr(nProgram),16,16,QChar('0')));
        lineEditsh_filesz->setText(QString("%1").arg(elffile->getProgramHeader64_filesz(nProgram),16,16,QChar('0')));
        lineEditsh_memsz->setText(QString("%1").arg(elffile->getProgramHeader64_memsz(nProgram),16,16,QChar('0')));
        lineEditsh_align->setText(QString("%1").arg(elffile->getProgramHeader64_align(nProgram),16,16,QChar('0')));

        lineEditsh_type->setInputMask("HHHHHHHH");
        lineEditsh_flags->setInputMask("HHHHHHHH");
        lineEditsh_offset->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_vaddr->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_paddr->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_filesz->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_memsz->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_align->setInputMask("HHHHHHHHHHHHHHHH");
    }
    else
    {
        lineEditsh_type->setText(QString("%1").arg(elffile->getProgramHeader32_type(nProgram),8,16,QChar('0')));
        lineEditsh_flags->setText(QString("%1").arg(elffile->getProgramHeader32_flags(nProgram),8,16,QChar('0')));
        lineEditsh_offset->setText(QString("%1").arg(elffile->getProgramHeader32_offset(nProgram),8,16,QChar('0')));
        lineEditsh_vaddr->setText(QString("%1").arg(elffile->getProgramHeader32_vaddr(nProgram),8,16,QChar('0')));
        lineEditsh_paddr->setText(QString("%1").arg(elffile->getProgramHeader32_paddr(nProgram),8,16,QChar('0')));
        lineEditsh_filesz->setText(QString("%1").arg(elffile->getProgramHeader32_filesz(nProgram),8,16,QChar('0')));
        lineEditsh_memsz->setText(QString("%1").arg(elffile->getProgramHeader32_memsz(nProgram),8,16,QChar('0')));
        lineEditsh_align->setText(QString("%1").arg(elffile->getProgramHeader32_align(nProgram),8,16,QChar('0')));

        lineEditsh_type->setInputMask("HHHHHHHH");
        lineEditsh_flags->setInputMask("HHHHHHHH");
        lineEditsh_offset->setInputMask("HHHHHHHH");
        lineEditsh_vaddr->setInputMask("HHHHHHHH");
        lineEditsh_paddr->setInputMask("HHHHHHHH");
        lineEditsh_filesz->setInputMask("HHHHHHHH");
        lineEditsh_memsz->setInputMask("HHHHHHHH");
        lineEditsh_align->setInputMask("HHHHHHHH");
    }

    ui->checkBoxReadOnly->setEnabled(!elffile->isReadOnly());

    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogELFProgramHeader::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineEditsh_type->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_flags->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_offset->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_vaddr->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_paddr->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_filesz->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_memsz->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_align->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogELFProgramHeader::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogELFProgramHeader::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogELFProgramHeader::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    if(elffile->isELF64())
    {
        elffile->setProgramHeader64_type(nProgram,lineEditsh_type->text().toUInt(&bTemp,16));
        elffile->setProgramHeader64_flags(nProgram,lineEditsh_flags->text().toUInt(&bTemp,16));
        elffile->setProgramHeader64_offset(nProgram,lineEditsh_offset->text().toULongLong(&bTemp,16));
        elffile->setProgramHeader64_vaddr(nProgram,lineEditsh_vaddr->text().toULongLong(&bTemp,16));
        elffile->setProgramHeader64_paddr(nProgram,lineEditsh_paddr->text().toULongLong(&bTemp,16));
        elffile->setProgramHeader64_filesz(nProgram,lineEditsh_filesz->text().toULongLong(&bTemp,16));
        elffile->setProgramHeader64_memsz(nProgram,lineEditsh_memsz->text().toULongLong(&bTemp,16));
        elffile->setProgramHeader64_align(nProgram,lineEditsh_align->text().toULongLong(&bTemp,16));
    }
    else
    {
        elffile->setProgramHeader32_type(nProgram,lineEditsh_type->text().toUInt(&bTemp,16));
        elffile->setProgramHeader32_flags(nProgram,lineEditsh_flags->text().toUInt(&bTemp,16));
        elffile->setProgramHeader32_offset(nProgram,lineEditsh_offset->text().toUInt(&bTemp,16));
        elffile->setProgramHeader32_vaddr(nProgram,lineEditsh_vaddr->text().toUInt(&bTemp,16));
        elffile->setProgramHeader32_paddr(nProgram,lineEditsh_paddr->text().toUInt(&bTemp,16));
        elffile->setProgramHeader32_filesz(nProgram,lineEditsh_filesz->text().toUInt(&bTemp,16));
        elffile->setProgramHeader32_memsz(nProgram,lineEditsh_memsz->text().toUInt(&bTemp,16));
        elffile->setProgramHeader32_align(nProgram,lineEditsh_align->text().toUInt(&bTemp,16));
    }

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}
void DialogELFProgramHeader::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}
