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
#include "dialogmachheader.h"
#include "ui_dialogmachheader.h"

DialogMACHHeader::DialogMACHHeader(QWidget *parent) :
    DialogMACHGeneric(parent),
    ui(new Ui::DialogMACHHeader)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);

    QTableWidgetItem *newItem;

    ui->tableWidgetMACHHeader->setRowCount(8);
    ui->tableWidgetMACHHeader->setColumnCount(3);

    QString sLabels[]= { "magic",
                         "cputype",
                         "cpusubtype",
                         "filetype",
                         "ncmds",
                         "sizeofcmds",
                         "flags",
                         "reserved"
                       };

    lineEdite_magic=new QLineEditX;
    lineEdite_cputype=new QLineEditX;
    lineEdite_cpusubtype=new QLineEditX;
    lineEdite_filetype=new QLineEditX;
    lineEdite_ncmds=new QLineEditX;
    lineEdite_sizeofcmds=new QLineEditX;
    lineEdite_flags=new QLineEditX;
    lineEdite_reserved=new QLineEditX;

    lineEdite_magic->setAlignment(Qt::AlignCenter);
    lineEdite_cputype->setAlignment(Qt::AlignCenter);
    lineEdite_cpusubtype->setAlignment(Qt::AlignCenter);
    lineEdite_filetype->setAlignment(Qt::AlignCenter);
    lineEdite_ncmds->setAlignment(Qt::AlignCenter);
    lineEdite_sizeofcmds->setAlignment(Qt::AlignCenter);
    lineEdite_flags->setAlignment(Qt::AlignCenter);
    lineEdite_reserved->setAlignment(Qt::AlignCenter);

    lineEdite_magic->setInputMask("HHHHHHHH");
    lineEdite_cputype->setInputMask("HHHHHHHH");
    lineEdite_cpusubtype->setInputMask("HHHHHHHH");
    lineEdite_filetype->setInputMask("HHHHHHHH");
    lineEdite_ncmds->setInputMask("HHHHHHHH");
    lineEdite_sizeofcmds->setInputMask("HHHHHHHH");
    lineEdite_flags->setInputMask("HHHHHHHH");
    lineEdite_reserved->setInputMask("HHHHHHHH");

    for(int i=0; i<8; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(sLabels[i]);
        ui->tableWidgetMACHHeader->setItem(i,0,newItem);
        ui->tableWidgetMACHHeader->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetMACHHeader->setRowHeight(i,20);
    }

    ui->tableWidgetMACHHeader->setCellWidget(0,1,lineEdite_magic);
    ui->tableWidgetMACHHeader->setCellWidget(1,1,lineEdite_cputype);
    ui->tableWidgetMACHHeader->setCellWidget(2,1,lineEdite_cpusubtype);
    ui->tableWidgetMACHHeader->setCellWidget(3,1,lineEdite_filetype);
    ui->tableWidgetMACHHeader->setCellWidget(4,1,lineEdite_ncmds);
    ui->tableWidgetMACHHeader->setCellWidget(5,1,lineEdite_sizeofcmds);
    ui->tableWidgetMACHHeader->setCellWidget(6,1,lineEdite_flags);
    ui->tableWidgetMACHHeader->setCellWidget(7,1,lineEdite_reserved);

    ui->tableWidgetMACHHeader->setColumnWidth(0,90);
    ui->tableWidgetMACHHeader->setColumnWidth(1,120);

    connect(lineEdite_magic,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_cputype,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_cpusubtype,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_filetype,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_ncmds,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_sizeofcmds,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_flags,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_reserved,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    ui->tableWidgetMACHHeader->resizeColumnToContents(2);

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_hex));
}

DialogMACHHeader::~DialogMACHHeader()
{
    delete ui;
}
void DialogMACHHeader::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogMACHHeader::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    lineEdite_magic->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_cputype->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_cpusubtype->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_filetype->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_ncmds->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_sizeofcmds->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_flags->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_reserved->setReadOnly(ui->checkBoxReadOnly->checkState());

}
void DialogMACHHeader::setData(MACHFile *machfile, __DIE_OPTIONS *pOptions)
{
    this->machfile=machfile;
    this->pOptions=pOptions;

    reload();
}

bool DialogMACHHeader::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));

        ui->pushButtonHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("MACH Header")));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg(tr("MACH Header")));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg(tr("MACH Header")));
    }

    ui->lineEditOffset->setText(QString("%1").arg(machfile->getMachHeaderOffset()));
    ui->lineEditSize->setText(QString("%1").arg(machfile->getMachHeaderSize()));

    lineEdite_magic->setText(QString("%1").arg(machfile->getHeader_magic(),8,16,QChar('0')));
    lineEdite_cputype->setText(QString("%1").arg(machfile->getHeader_cputype(),8,16,QChar('0')));
    lineEdite_cpusubtype->setText(QString("%1").arg(machfile->getHeader_cpusubtype(),8,16,QChar('0')));
    lineEdite_filetype->setText(QString("%1").arg(machfile->getHeader_filetype(),8,16,QChar('0')));
    lineEdite_ncmds->setText(QString("%1").arg(machfile->getHeader_ncmds(),8,16,QChar('0')));
    lineEdite_sizeofcmds->setText(QString("%1").arg(machfile->getHeader_sizeofcmds(),8,16,QChar('0')));
    lineEdite_flags->setText(QString("%1").arg(machfile->getHeader_flags(),8,16,QChar('0')));

    if(machfile->isMACH64())
    {
        lineEdite_reserved->setText(QString("%1").arg(machfile->getHeader_reserved(),8,16,QChar('0')));
    }
    else
    {
        lineEdite_reserved->setEnabled(false);
    }

    ui->checkBoxReadOnly->setEnabled(!machfile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}
void DialogMACHHeader::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogMACHHeader::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogMACHHeader::on_pushButtonApply_clicked()
{
    bool bTemp;

    ui->pushButtonApply->setEnabled(false);

    machfile->setHeader_magic(lineEdite_magic->text().toUInt(&bTemp,16));
    machfile->setHeader_cputype(lineEdite_cputype->text().toUInt(&bTemp,16));
    machfile->setHeader_cpusubtype(lineEdite_cpusubtype->text().toUInt(&bTemp,16));
    machfile->setHeader_filetype(lineEdite_filetype->text().toUInt(&bTemp,16));
    machfile->setHeader_ncmds(lineEdite_ncmds->text().toUInt(&bTemp,16));
    machfile->setHeader_sizeofcmds(lineEdite_sizeofcmds->text().toUInt(&bTemp,16));
    machfile->setHeader_flags(lineEdite_flags->text().toUInt(&bTemp,16));

    if(machfile->isMACH64())
    {
        machfile->setHeader_reserved(lineEdite_reserved->text().toUInt(&bTemp,16));
    }

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogMACHHeader::on_pushButtonHex_clicked()
{
    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,machfile->getFileName(),0,0,"HEX","","",machfile->getMachHeaderOffset(),machfile->getMachHeaderSize(),tr("Header"));

    dialoghde.exec();
}
