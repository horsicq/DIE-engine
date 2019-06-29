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
#include "dialogelfsectionheader.h"
#include "ui_dialogelfsectionheader.h"

DialogELFSectionHeader::DialogELFSectionHeader(QWidget *parent) :
    DialogELFGeneric(parent),
    ui(new Ui::DialogELFSectionHeader)
{
    ui->setupUi(this);

    QTableWidgetItem *newItem;

    ui->tableWidgetSectionHeader->setRowCount(10);
    ui->tableWidgetSectionHeader->setColumnCount(2);

    for(int i=0; i<10; i++)
    {
        ui->tableWidgetSectionHeader->setRowHeight(i,20);
    }

    lineEditsh_name=new QLineEditX;
    lineEditsh_type=new QLineEditX;
    lineEditsh_flags=new QLineEditX;
    lineEditsh_addr=new QLineEditX;
    lineEditsh_offset=new QLineEditX;
    lineEditsh_size=new QLineEditX;
    lineEditsh_link=new QLineEditX;
    lineEditsh_info=new QLineEditX;
    lineEditsh_addralign=new QLineEditX;
    lineEditsh_entsize=new QLineEditX;

    lineEditsh_name->setAlignment(Qt::AlignCenter);
    lineEditsh_type->setAlignment(Qt::AlignCenter);
    lineEditsh_flags->setAlignment(Qt::AlignCenter);
    lineEditsh_addr->setAlignment(Qt::AlignCenter);
    lineEditsh_offset->setAlignment(Qt::AlignCenter);
    lineEditsh_size->setAlignment(Qt::AlignCenter);
    lineEditsh_link->setAlignment(Qt::AlignCenter);
    lineEditsh_info->setAlignment(Qt::AlignCenter);
    lineEditsh_addralign->setAlignment(Qt::AlignCenter);
    lineEditsh_entsize->setAlignment(Qt::AlignCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("name");
    ui->tableWidgetSectionHeader->setItem(0,0,newItem);
    ui->tableWidgetSectionHeader->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("type");
    ui->tableWidgetSectionHeader->setItem(1,0,newItem);
    ui->tableWidgetSectionHeader->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("flags");
    ui->tableWidgetSectionHeader->setItem(2,0,newItem);
    ui->tableWidgetSectionHeader->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("addr");
    ui->tableWidgetSectionHeader->setItem(3,0,newItem);
    ui->tableWidgetSectionHeader->item(3,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("offset");
    ui->tableWidgetSectionHeader->setItem(4,0,newItem);
    ui->tableWidgetSectionHeader->item(4,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("size");
    ui->tableWidgetSectionHeader->setItem(5,0,newItem);
    ui->tableWidgetSectionHeader->item(5,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("link");
    ui->tableWidgetSectionHeader->setItem(6,0,newItem);
    ui->tableWidgetSectionHeader->item(6,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("info");
    ui->tableWidgetSectionHeader->setItem(7,0,newItem);
    ui->tableWidgetSectionHeader->item(7,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("addralign");
    ui->tableWidgetSectionHeader->setItem(8,0,newItem);
    ui->tableWidgetSectionHeader->item(8,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("entsize");
    ui->tableWidgetSectionHeader->setItem(9,0,newItem);
    ui->tableWidgetSectionHeader->item(9,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetSectionHeader->setCellWidget(0,1,lineEditsh_name);
    ui->tableWidgetSectionHeader->setCellWidget(1,1,lineEditsh_type);
    ui->tableWidgetSectionHeader->setCellWidget(2,1,lineEditsh_flags);
    ui->tableWidgetSectionHeader->setCellWidget(3,1,lineEditsh_addr);
    ui->tableWidgetSectionHeader->setCellWidget(4,1,lineEditsh_offset);
    ui->tableWidgetSectionHeader->setCellWidget(5,1,lineEditsh_size);
    ui->tableWidgetSectionHeader->setCellWidget(6,1,lineEditsh_link);
    ui->tableWidgetSectionHeader->setCellWidget(7,1,lineEditsh_info);
    ui->tableWidgetSectionHeader->setCellWidget(8,1,lineEditsh_addralign);
    ui->tableWidgetSectionHeader->setCellWidget(9,1,lineEditsh_entsize);

    connect(lineEditsh_name,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_type,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_flags,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_addr,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_offset,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_size,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_link,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_info,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_addralign,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditsh_entsize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    ui->tableWidgetSectionHeader->setColumnWidth(0,90);
    ui->tableWidgetSectionHeader->setColumnWidth(1,120);

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogELFSectionHeader::~DialogELFSectionHeader()
{
    delete ui;
}

void DialogELFSectionHeader::setData(__DIE_OPTIONS *pOptions,ELFFile *elffile,int nSection)
{
    this->nSection=nSection;

    DialogELFGeneric::setData(pOptions,elffile);
}

bool DialogELFSectionHeader::reload()
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
        lineEditsh_name->setText(QString("%1").arg(elffile->getSectionHeader64_name(nSection),8,16,QChar('0')));
        lineEditsh_type->setText(QString("%1").arg(elffile->getSectionHeader64_type(nSection),8,16,QChar('0')));
        lineEditsh_flags->setText(QString("%1").arg(elffile->getSectionHeader64_flags(nSection),16,16,QChar('0')));
        lineEditsh_addr->setText(QString("%1").arg(elffile->getSectionHeader64_addr(nSection),16,16,QChar('0')));
        lineEditsh_offset->setText(QString("%1").arg(elffile->getSectionHeader64_offset(nSection),16,16,QChar('0')));
        lineEditsh_size->setText(QString("%1").arg(elffile->getSectionHeader64_size(nSection),16,16,QChar('0')));
        lineEditsh_link->setText(QString("%1").arg(elffile->getSectionHeader64_link(nSection),8,16,QChar('0')));
        lineEditsh_info->setText(QString("%1").arg(elffile->getSectionHeader64_info(nSection),8,16,QChar('0')));
        lineEditsh_addralign->setText(QString("%1").arg(elffile->getSectionHeader64_addralign(nSection),16,16,QChar('0')));
        lineEditsh_entsize->setText(QString("%1").arg(elffile->getSectionHeader64_entsize(nSection),16,16,QChar('0')));

        lineEditsh_name->setInputMask("HHHHHHHH");
        lineEditsh_type->setInputMask("HHHHHHHH");
        lineEditsh_flags->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_addr->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_offset->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_size->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_link->setInputMask("HHHHHHHH");
        lineEditsh_info->setInputMask("HHHHHHHH");
        lineEditsh_addralign->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditsh_entsize->setInputMask("HHHHHHHHHHHHHHHH");
    }
    else
    {
        lineEditsh_name->setText(QString("%1").arg(elffile->getSectionHeader32_name(nSection),8,16,QChar('0')));
        lineEditsh_type->setText(QString("%1").arg(elffile->getSectionHeader32_type(nSection),8,16,QChar('0')));
        lineEditsh_flags->setText(QString("%1").arg(elffile->getSectionHeader32_flags(nSection),8,16,QChar('0')));
        lineEditsh_addr->setText(QString("%1").arg(elffile->getSectionHeader32_addr(nSection),8,16,QChar('0')));
        lineEditsh_offset->setText(QString("%1").arg(elffile->getSectionHeader32_offset(nSection),8,16,QChar('0')));
        lineEditsh_size->setText(QString("%1").arg(elffile->getSectionHeader32_size(nSection),8,16,QChar('0')));
        lineEditsh_link->setText(QString("%1").arg(elffile->getSectionHeader32_link(nSection),8,16,QChar('0')));
        lineEditsh_info->setText(QString("%1").arg(elffile->getSectionHeader32_info(nSection),8,16,QChar('0')));
        lineEditsh_addralign->setText(QString("%1").arg(elffile->getSectionHeader32_addralign(nSection),8,16,QChar('0')));
        lineEditsh_entsize->setText(QString("%1").arg(elffile->getSectionHeader32_entsize(nSection),8,16,QChar('0')));

        lineEditsh_name->setInputMask("HHHHHHHH");
        lineEditsh_type->setInputMask("HHHHHHHH");
        lineEditsh_flags->setInputMask("HHHHHHHH");
        lineEditsh_addr->setInputMask("HHHHHHHH");
        lineEditsh_offset->setInputMask("HHHHHHHH");
        lineEditsh_size->setInputMask("HHHHHHHH");
        lineEditsh_link->setInputMask("HHHHHHHH");
        lineEditsh_info->setInputMask("HHHHHHHH");
        lineEditsh_addralign->setInputMask("HHHHHHHH");
        lineEditsh_entsize->setInputMask("HHHHHHHH");
    }

    ui->checkBoxReadOnly->setEnabled(!elffile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogELFSectionHeader::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineEditsh_name->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_type->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_flags->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_addr->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_offset->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_size->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_link->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_info->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_addralign->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditsh_entsize->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogELFSectionHeader::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogELFSectionHeader::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogELFSectionHeader::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    if(elffile->isELF64())
    {
        elffile->setSectionHeader64_name(nSection,lineEditsh_name->text().toUInt(&bTemp,16));
        elffile->setSectionHeader64_type(nSection,lineEditsh_type->text().toUInt(&bTemp,16));
        elffile->setSectionHeader64_flags(nSection,lineEditsh_flags->text().toULongLong(&bTemp,16));
        elffile->setSectionHeader64_addr(nSection,lineEditsh_addr->text().toULongLong(&bTemp,16));
        elffile->setSectionHeader64_offset(nSection,lineEditsh_offset->text().toULongLong(&bTemp,16));
        elffile->setSectionHeader64_size(nSection,lineEditsh_size->text().toULongLong(&bTemp,16));
        elffile->setSectionHeader64_link(nSection,lineEditsh_link->text().toUInt(&bTemp,16));
        elffile->setSectionHeader64_info(nSection,lineEditsh_info->text().toUInt(&bTemp,16));
        elffile->setSectionHeader64_addralign(nSection,lineEditsh_addralign->text().toULongLong(&bTemp,16));
        elffile->setSectionHeader64_entsize(nSection,lineEditsh_entsize->text().toULongLong(&bTemp,16));

    }
    else
    {
        elffile->setSectionHeader32_name(nSection,lineEditsh_name->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_type(nSection,lineEditsh_type->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_flags(nSection,lineEditsh_flags->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_addr(nSection,lineEditsh_addr->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_offset(nSection,lineEditsh_offset->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_size(nSection,lineEditsh_size->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_link(nSection,lineEditsh_link->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_info(nSection,lineEditsh_info->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_addralign(nSection,lineEditsh_addralign->text().toUInt(&bTemp,16));
        elffile->setSectionHeader32_entsize(nSection,lineEditsh_entsize->text().toUInt(&bTemp,16));

    }



    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}
void DialogELFSectionHeader::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}
