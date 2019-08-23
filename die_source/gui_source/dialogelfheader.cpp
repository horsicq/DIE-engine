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
#include "dialogelfheader.h"
#include "ui_dialogelfheader.h"

DialogELFHeader::DialogELFHeader(QWidget *parent) :
    DialogELFGeneric(parent),
    ui(new Ui::DialogELFHeader)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);

    QTableWidgetItem *newItem;

    ui->tableWidgetELFHeader->setRowCount(26);
    ui->tableWidgetELFHeader->setColumnCount(3);

    QString sLabels[]= { "ei_magic",
                         "ei_class",
                         "ei_data",
                         "ei_version",
                         "ei_osabi",
                         "ei_abiversion",
                         "ei_pad[0]",
                         "ei_pad[1]",
                         "ei_pad[2]",
                         "ei_pad[3]",
                         "ei_pad[4]",
                         "ei_pad[5]",
                         "ei_pad[6]",
                         "ei_pad[7]",
                         "ei_pad[8]",
                         "e_type",
                         "e_machine",
                         "e_version",
                         "e_entry",
                         "e_phoff",
                         "e_shoff",
                         "e_flags",
                         "e_ehsize",
                         "e_phentsize",
                         "e_phnum",
                         "e_shentsize",
                         "e_shnum",
                         "e_shstrndx"
                       };

    lineEdite_magic=new QLineEditX;
    lineEdite_class=new QLineEditX;
    lineEdite_data=new QLineEditX;
    lineEdite_iversion=new QLineEditX;
    lineEdite_osabi=new QLineEditX;
    lineEdite_abiversion=new QLineEditX;
    lineEdite_pad0=new QLineEditX;
    lineEdite_pad1=new QLineEditX;
    lineEdite_pad2=new QLineEditX;
    lineEdite_pad3=new QLineEditX;
    lineEdite_pad4=new QLineEditX;
    lineEdite_pad5=new QLineEditX;
    lineEdite_pad6=new QLineEditX;
    lineEdite_pad7=new QLineEditX;
    lineEdite_pad8=new QLineEditX;

    lineEdite_type=new QLineEditX;
    lineEdite_machine=new QLineEditX;
    lineEdite_version=new QLineEditX;
    lineEdite_entry=new QLineEditX;
    lineEdite_phoff=new QLineEditX;
    lineEdite_shoff=new QLineEditX;
    lineEdite_flags=new QLineEditX;
    lineEdite_ehsize=new QLineEditX;
    lineEdite_phentsize=new QLineEditX;
    lineEdite_phnum=new QLineEditX;
    lineEdite_shentsize=new QLineEditX;
    lineEdite_shnum=new QLineEditX;
    lineEdite_shstrndx=new QLineEditX;

    lineEdite_magic->setAlignment(Qt::AlignCenter);
    lineEdite_class->setAlignment(Qt::AlignCenter);
    lineEdite_data->setAlignment(Qt::AlignCenter);
    lineEdite_iversion->setAlignment(Qt::AlignCenter);
    lineEdite_osabi->setAlignment(Qt::AlignCenter);
    lineEdite_abiversion->setAlignment(Qt::AlignCenter);
    lineEdite_pad0->setAlignment(Qt::AlignCenter);
    lineEdite_pad1->setAlignment(Qt::AlignCenter);
    lineEdite_pad2->setAlignment(Qt::AlignCenter);
    lineEdite_pad3->setAlignment(Qt::AlignCenter);
    lineEdite_pad4->setAlignment(Qt::AlignCenter);
    lineEdite_pad5->setAlignment(Qt::AlignCenter);
    lineEdite_pad6->setAlignment(Qt::AlignCenter);
    lineEdite_pad7->setAlignment(Qt::AlignCenter);
    lineEdite_pad8->setAlignment(Qt::AlignCenter);

    lineEdite_type->setAlignment(Qt::AlignCenter);
    lineEdite_machine->setAlignment(Qt::AlignCenter);
    lineEdite_version->setAlignment(Qt::AlignCenter);
    lineEdite_entry->setAlignment(Qt::AlignCenter);
    lineEdite_phoff->setAlignment(Qt::AlignCenter);
    lineEdite_shoff->setAlignment(Qt::AlignCenter);
    lineEdite_flags->setAlignment(Qt::AlignCenter);
    lineEdite_ehsize->setAlignment(Qt::AlignCenter);
    lineEdite_phentsize->setAlignment(Qt::AlignCenter);
    lineEdite_phnum->setAlignment(Qt::AlignCenter);
    lineEdite_shentsize->setAlignment(Qt::AlignCenter);
    lineEdite_shnum->setAlignment(Qt::AlignCenter);
    lineEdite_shstrndx->setAlignment(Qt::AlignCenter);

    lineEdite_magic->setInputMask("HHHHHHHH");
    lineEdite_class->setInputMask("HH");
    lineEdite_data->setInputMask("HH");
    lineEdite_iversion->setInputMask("HH");
    lineEdite_osabi->setInputMask("HH");
    lineEdite_abiversion->setInputMask("HH");
    lineEdite_pad0->setInputMask("HH");
    lineEdite_pad1->setInputMask("HH");
    lineEdite_pad2->setInputMask("HH");
    lineEdite_pad3->setInputMask("HH");
    lineEdite_pad4->setInputMask("HH");
    lineEdite_pad5->setInputMask("HH");
    lineEdite_pad6->setInputMask("HH");
    lineEdite_pad7->setInputMask("HH");
    lineEdite_pad8->setInputMask("HH");

    for(int i=0; i<26; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(sLabels[i]);
        ui->tableWidgetELFHeader->setItem(i,0,newItem);
        ui->tableWidgetELFHeader->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetELFHeader->setRowHeight(i,20);
    }

    ui->tableWidgetELFHeader->setCellWidget(0,1,lineEdite_magic);
    ui->tableWidgetELFHeader->setCellWidget(1,1,lineEdite_class);
    ui->tableWidgetELFHeader->setCellWidget(2,1,lineEdite_data);
    ui->tableWidgetELFHeader->setCellWidget(3,1,lineEdite_iversion);
    ui->tableWidgetELFHeader->setCellWidget(4,1,lineEdite_osabi);
    ui->tableWidgetELFHeader->setCellWidget(5,1,lineEdite_abiversion);
    ui->tableWidgetELFHeader->setCellWidget(6,1,lineEdite_pad0);
    ui->tableWidgetELFHeader->setCellWidget(7,1,lineEdite_pad1);
    ui->tableWidgetELFHeader->setCellWidget(8,1,lineEdite_pad2);
    ui->tableWidgetELFHeader->setCellWidget(9,1,lineEdite_pad3);
    ui->tableWidgetELFHeader->setCellWidget(10,1,lineEdite_pad4);
    ui->tableWidgetELFHeader->setCellWidget(11,1,lineEdite_pad5);
    ui->tableWidgetELFHeader->setCellWidget(12,1,lineEdite_pad6);
    ui->tableWidgetELFHeader->setCellWidget(13,1,lineEdite_pad7);
    ui->tableWidgetELFHeader->setCellWidget(14,1,lineEdite_pad8);

    ui->tableWidgetELFHeader->setCellWidget(15,1,lineEdite_type);
    ui->tableWidgetELFHeader->setCellWidget(16,1,lineEdite_machine);
    ui->tableWidgetELFHeader->setCellWidget(17,1,lineEdite_version);
    ui->tableWidgetELFHeader->setCellWidget(18,1,lineEdite_entry);
    ui->tableWidgetELFHeader->setCellWidget(19,1,lineEdite_phoff);
    ui->tableWidgetELFHeader->setCellWidget(20,1,lineEdite_shoff);
    ui->tableWidgetELFHeader->setCellWidget(21,1,lineEdite_flags);
    ui->tableWidgetELFHeader->setCellWidget(22,1,lineEdite_ehsize);
    ui->tableWidgetELFHeader->setCellWidget(23,1,lineEdite_phentsize);
    ui->tableWidgetELFHeader->setCellWidget(24,1,lineEdite_phnum);
    ui->tableWidgetELFHeader->setCellWidget(25,1,lineEdite_shentsize);
    ui->tableWidgetELFHeader->setCellWidget(26,1,lineEdite_shnum);
    ui->tableWidgetELFHeader->setCellWidget(27,1,lineEdite_shstrndx);

    ui->tableWidgetELFHeader->setColumnWidth(0,90);
    ui->tableWidgetELFHeader->setColumnWidth(1,120);

    connect(lineEdite_magic,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_class,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_data,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_iversion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_osabi,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_abiversion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad0,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad1,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad2,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad3,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad4,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad5,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad6,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad7,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_pad8,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    connect(lineEdite_type,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_machine,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_version,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_entry,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_phoff,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_shoff,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_flags,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_ehsize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_phentsize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_phnum,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_shentsize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_shnum,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_shstrndx,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    pushButtonClassExtra=new QPushButtonX;
    pushButtonClassExtra->setText("...");
    pushButtonClassExtra->setFixedWidth(30);

    ui->tableWidgetELFHeader->setIndexWidget(ui->tableWidgetELFHeader->model()->index(1,2),pushButtonClassExtra);
    connect(pushButtonClassExtra,SIGNAL(clicked()),this,SLOT(pushButtonClassExtra_clicked()));

    pushButtonDataExtra=new QPushButtonX;
    pushButtonDataExtra->setText("...");
    pushButtonDataExtra->setFixedWidth(30);

    ui->tableWidgetELFHeader->setIndexWidget(ui->tableWidgetELFHeader->model()->index(2,2),pushButtonDataExtra);
    connect(pushButtonDataExtra,SIGNAL(clicked()),this,SLOT(pushButtonDataExtra_clicked()));

    pushButtonTypeExtra=new QPushButtonX;
    pushButtonTypeExtra->setText("...");
    pushButtonTypeExtra->setFixedWidth(30);

    ui->tableWidgetELFHeader->setIndexWidget(ui->tableWidgetELFHeader->model()->index(13,2),pushButtonTypeExtra);
    connect(pushButtonTypeExtra,SIGNAL(clicked()),this,SLOT(pushButtonTypeExtra_clicked()));

    pushButtonMACHineExtra=new QPushButtonX;
    pushButtonMACHineExtra->setText("...");
    pushButtonMACHineExtra->setFixedWidth(30);

    ui->tableWidgetELFHeader->setIndexWidget(ui->tableWidgetELFHeader->model()->index(14,2),pushButtonMACHineExtra);
    connect(pushButtonMACHineExtra,SIGNAL(clicked()),this,SLOT(pushButtonMACHineExtra_clicked()));

    pushButtonEntryExtra=new QPushButtonX;
    pushButtonEntryExtra->setText("...");
    pushButtonEntryExtra->setFixedWidth(30);

    ui->tableWidgetELFHeader->setIndexWidget(ui->tableWidgetELFHeader->model()->index(16,2),pushButtonEntryExtra);
    connect(pushButtonEntryExtra,SIGNAL(clicked()),this,SLOT(pushButtonEntryExtra_clicked()));

    ui->tableWidgetELFHeader->resizeColumnToContents(2);

    ui->checkBoxReadOnly->setChecked(true);

    //    connect(this, SIGNAL(reloadSignal()), this, SLOT(reload()));
    //    connect(this, SIGNAL(reloadSignal()), parent, SIGNAL(reloadSignal()));
    //    connect(this, SIGNAL(appendError(QString)), parent, SIGNAL(appendError(QString)));

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_hex));
}

DialogELFHeader::~DialogELFHeader()
{
    delete ui;
}
void DialogELFHeader::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogELFHeader::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    lineEdite_magic->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_class->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_data->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_iversion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_osabi->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_abiversion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad0->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad1->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad2->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad3->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad4->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad5->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad6->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad7->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_pad8->setReadOnly(ui->checkBoxReadOnly->checkState());

    lineEdite_type->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_machine->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_version->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_entry->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_phoff->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_shoff->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_flags->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_ehsize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_phentsize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_phnum->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_shentsize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_shnum->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_shstrndx->setReadOnly(ui->checkBoxReadOnly->checkState());
}
//void DialogELFHeader::setData(ELFFile *elffile, __OPTIONS *pOptions)
//{
//    this->elffile=elffile;
//    this->pOptions=pOptions;

//    reload();
//}

bool DialogELFHeader::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));

        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->pushButtonHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("ELF Header")));
        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg(tr("ELF Header")));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg(tr("ELF Header")));
    }

    ui->lineEditOffset->setText(QString("%1").arg(elffile->getElfHeaderOffset()));
    ui->lineEditSize->setText(QString("%1").arg(elffile->getElfHeaderSize()));

    lineEdite_magic->setText(QString("%1").arg(elffile->getElfHeader_magic(),8,16,QChar('0')));
    lineEdite_class->setText(QString("%1").arg(elffile->getElfHeader_class(),2,16,QChar('0')));
    lineEdite_data->setText(QString("%1").arg(elffile->getElfHeader_data(),2,16,QChar('0')));
    lineEdite_iversion->setText(QString("%1").arg(elffile->getElfHeader_iversion(),2,16,QChar('0')));
    lineEdite_osabi->setText(QString("%1").arg(elffile->getElfHeader_osabi(),2,16,QChar('0')));
    lineEdite_abiversion->setText(QString("%1").arg(elffile->getElfHeader_abiversion(),2,16,QChar('0')));
    lineEdite_pad0->setText(QString("%1").arg(elffile->getElfHeader_pad(0),2,16,QChar('0')));
    lineEdite_pad1->setText(QString("%1").arg(elffile->getElfHeader_pad(1),2,16,QChar('0')));
    lineEdite_pad2->setText(QString("%1").arg(elffile->getElfHeader_pad(2),2,16,QChar('0')));
    lineEdite_pad3->setText(QString("%1").arg(elffile->getElfHeader_pad(3),2,16,QChar('0')));
    lineEdite_pad4->setText(QString("%1").arg(elffile->getElfHeader_pad(4),2,16,QChar('0')));
    lineEdite_pad5->setText(QString("%1").arg(elffile->getElfHeader_pad(5),2,16,QChar('0')));
    lineEdite_pad6->setText(QString("%1").arg(elffile->getElfHeader_pad(6),2,16,QChar('0')));
    lineEdite_pad7->setText(QString("%1").arg(elffile->getElfHeader_pad(7),2,16,QChar('0')));
    lineEdite_pad8->setText(QString("%1").arg(elffile->getElfHeader_pad(8),2,16,QChar('0')));

    if(elffile->isELF64())
    {
        lineEdite_type->setInputMask("HHHH");
        lineEdite_machine->setInputMask("HHHH");
        lineEdite_version->setInputMask("HHHHHHHH");
        lineEdite_entry->setInputMask("HHHHHHHHHHHHHHHH");
        lineEdite_phoff->setInputMask("HHHHHHHHHHHHHHHH");
        lineEdite_shoff->setInputMask("HHHHHHHHHHHHHHHH");
        lineEdite_flags->setInputMask("HHHHHHHH");
        lineEdite_ehsize->setInputMask("HHHH");
        lineEdite_phentsize->setInputMask("HHHH");
        lineEdite_phnum->setInputMask("HHHH");
        lineEdite_shentsize->setInputMask("HHHH");
        lineEdite_shnum->setInputMask("HHHH");
        lineEdite_shstrndx->setInputMask("HHHH");

        lineEdite_type->setText(QString("%1").arg(elffile->getElfHeader_type(),4,16,QChar('0')));
        lineEdite_machine->setText(QString("%1").arg(elffile->getElfHeader_machine(),4,16,QChar('0')));
        lineEdite_version->setText(QString("%1").arg(elffile->getElfHeader_version(),8,16,QChar('0')));
        lineEdite_entry->setText(QString("%1").arg(elffile->getElfHeader_entry64(),16,16,QChar('0')));
        lineEdite_phoff->setText(QString("%1").arg(elffile->getElfHeader_phoff64(),16,16,QChar('0')));
        lineEdite_shoff->setText(QString("%1").arg(elffile->getElfHeader_shoff64(),16,16,QChar('0')));
        lineEdite_flags->setText(QString("%1").arg(elffile->getElfHeader_flags(),8,16,QChar('0')));
        lineEdite_ehsize->setText(QString("%1").arg(elffile->getElfHeader_ehsize(),4,16,QChar('0')));
        lineEdite_phentsize->setText(QString("%1").arg(elffile->getElfHeader_phentsize(),4,16,QChar('0')));
        lineEdite_phnum->setText(QString("%1").arg(elffile->getElfHeader_phnum(),4,16,QChar('0')));
        lineEdite_shentsize->setText(QString("%1").arg(elffile->getElfHeader_shentsize(),4,16,QChar('0')));
        lineEdite_shnum->setText(QString("%1").arg(elffile->getElfHeader_shnum(),4,16,QChar('0')));
        lineEdite_shstrndx->setText(QString("%1").arg(elffile->getElfHeader_shstrndx(),4,16,QChar('0')));
    }
    else
    {
        lineEdite_type->setInputMask("HHHH");
        lineEdite_machine->setInputMask("HHHH");
        lineEdite_version->setInputMask("HHHHHHHH");
        lineEdite_entry->setInputMask("HHHHHHHH");
        lineEdite_phoff->setInputMask("HHHHHHHH");
        lineEdite_shoff->setInputMask("HHHHHHHH");
        lineEdite_flags->setInputMask("HHHHHHHH");
        lineEdite_ehsize->setInputMask("HHHH");
        lineEdite_phentsize->setInputMask("HHHH");
        lineEdite_phnum->setInputMask("HHHH");
        lineEdite_shentsize->setInputMask("HHHH");
        lineEdite_shnum->setInputMask("HHHH");
        lineEdite_shstrndx->setInputMask("HHHH");

        lineEdite_type->setText(QString("%1").arg(elffile->getElfHeader_type(),4,16,QChar('0')));
        lineEdite_machine->setText(QString("%1").arg(elffile->getElfHeader_machine(),4,16,QChar('0')));
        lineEdite_version->setText(QString("%1").arg(elffile->getElfHeader_version(),8,16,QChar('0')));
        lineEdite_entry->setText(QString("%1").arg(elffile->getElfHeader_entry(),8,16,QChar('0')));
        lineEdite_phoff->setText(QString("%1").arg(elffile->getElfHeader_phoff(),8,16,QChar('0')));
        lineEdite_shoff->setText(QString("%1").arg(elffile->getElfHeader_shoff(),8,16,QChar('0')));
        lineEdite_flags->setText(QString("%1").arg(elffile->getElfHeader_flags(),8,16,QChar('0')));
        lineEdite_ehsize->setText(QString("%1").arg(elffile->getElfHeader_ehsize(),4,16,QChar('0')));
        lineEdite_phentsize->setText(QString("%1").arg(elffile->getElfHeader_phentsize(),4,16,QChar('0')));
        lineEdite_phnum->setText(QString("%1").arg(elffile->getElfHeader_phnum(),4,16,QChar('0')));
        lineEdite_shentsize->setText(QString("%1").arg(elffile->getElfHeader_shentsize(),4,16,QChar('0')));
        lineEdite_shnum->setText(QString("%1").arg(elffile->getElfHeader_shnum(),4,16,QChar('0')));
        lineEdite_shstrndx->setText(QString("%1").arg(elffile->getElfHeader_shstrndx(),4,16,QChar('0')));
    }

    ui->checkBoxReadOnly->setEnabled(!elffile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}
void DialogELFHeader::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogELFHeader::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogELFHeader::on_pushButtonApply_clicked()
{
    bool bTemp;

    ui->pushButtonApply->setEnabled(false);

    elffile->setElfHeader_magic(lineEdite_magic->text().toUInt(&bTemp,16));
    elffile->setElfHeader_class(lineEdite_class->text().toUInt(&bTemp,16));
    elffile->setElfHeader_data(lineEdite_data->text().toUInt(&bTemp,16));
    elffile->setElfHeader_iversion(lineEdite_iversion->text().toUInt(&bTemp,16));
    elffile->setElfHeader_osabi(lineEdite_osabi->text().toUInt(&bTemp,16));
    elffile->setElfHeader_abiversion(lineEdite_abiversion->text().toUInt(&bTemp,16));

    elffile->setElfHeader_pad(lineEdite_pad0->text().toUInt(&bTemp,16),0);
    elffile->setElfHeader_pad(lineEdite_pad1->text().toUInt(&bTemp,16),1);
    elffile->setElfHeader_pad(lineEdite_pad2->text().toUInt(&bTemp,16),2);
    elffile->setElfHeader_pad(lineEdite_pad3->text().toUInt(&bTemp,16),3);
    elffile->setElfHeader_pad(lineEdite_pad4->text().toUInt(&bTemp,16),4);
    elffile->setElfHeader_pad(lineEdite_pad5->text().toUInt(&bTemp,16),5);
    elffile->setElfHeader_pad(lineEdite_pad6->text().toUInt(&bTemp,16),6);
    elffile->setElfHeader_pad(lineEdite_pad7->text().toUInt(&bTemp,16),7);
    elffile->setElfHeader_pad(lineEdite_pad8->text().toUInt(&bTemp,16),8);

    if(elffile->isELF64())
    {
        elffile->setElfHeader_type(lineEdite_type->text().toUInt(&bTemp,16));
        elffile->setElfHeader_machine(lineEdite_machine->text().toUInt(&bTemp,16));
        elffile->setElfHeader_version(lineEdite_version->text().toUInt(&bTemp,16));
        elffile->setElfHeader_entry64(lineEdite_entry->text().toULongLong(&bTemp,16));
        elffile->setElfHeader_phoff64(lineEdite_phoff->text().toULongLong(&bTemp,16));
        elffile->setElfHeader_shoff64(lineEdite_shoff->text().toULongLong(&bTemp,16));
        elffile->setElfHeader_flags(lineEdite_flags->text().toUInt(&bTemp,16));
        elffile->setElfHeader_ehsize(lineEdite_ehsize->text().toUInt(&bTemp,16));
        elffile->setElfHeader_phentsize(lineEdite_phentsize->text().toUInt(&bTemp,16));
        elffile->setElfHeader_phnum(lineEdite_phnum->text().toUInt(&bTemp,16));
        elffile->setElfHeader_shentsize(lineEdite_shentsize->text().toUInt(&bTemp,16));
        elffile->setElfHeader_shnum(lineEdite_shnum->text().toUInt(&bTemp,16));
        elffile->setElfHeader_shstrndx(lineEdite_shstrndx->text().toUInt(&bTemp,16));
    }
    else
    {
        elffile->setElfHeader_type(lineEdite_type->text().toUInt(&bTemp,16));
        elffile->setElfHeader_machine(lineEdite_machine->text().toUInt(&bTemp,16));
        elffile->setElfHeader_version(lineEdite_version->text().toUInt(&bTemp,16));
        elffile->setElfHeader_entry(lineEdite_entry->text().toUInt(&bTemp,16));
        elffile->setElfHeader_phoff(lineEdite_phoff->text().toUInt(&bTemp,16));
        elffile->setElfHeader_shoff(lineEdite_shoff->text().toUInt(&bTemp,16));
        elffile->setElfHeader_flags(lineEdite_flags->text().toUInt(&bTemp,16));
        elffile->setElfHeader_ehsize(lineEdite_ehsize->text().toUInt(&bTemp,16));
        elffile->setElfHeader_phentsize(lineEdite_phentsize->text().toUInt(&bTemp,16));
        elffile->setElfHeader_phnum(lineEdite_phnum->text().toUInt(&bTemp,16));
        elffile->setElfHeader_shentsize(lineEdite_shentsize->text().toUInt(&bTemp,16));
        elffile->setElfHeader_shnum(lineEdite_shnum->text().toUInt(&bTemp,16));
        elffile->setElfHeader_shstrndx(lineEdite_shstrndx->text().toUInt(&bTemp,16));
    }

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogELFHeader::on_pushButtonHex_clicked()
{
    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,elffile->getFileName(),0,0,"HEX","","",elffile->getElfHeaderOffset(),elffile->getElfHeaderSize(),tr("Header"));

    dialoghde.exec();
}

void DialogELFHeader::pushButtonClassExtra_clicked()
{
    DialogELFClass dialogec(this);

    dialogec.setData(pOptions,elffile);

    dialogec.exec();
}

void DialogELFHeader::pushButtonDataExtra_clicked()
{
    DialogELFData dialoged(this);

    dialoged.setData(pOptions,elffile);

    dialoged.exec();
}

void DialogELFHeader::pushButtonTypeExtra_clicked()
{
    DialogELFType dialoget(this);

    dialoget.setData(pOptions,elffile);

    dialoget.exec();
}

void DialogELFHeader::pushButtonMACHineExtra_clicked()
{
    DialogELFMachine dialogem(this);

    dialogem.setData(pOptions,elffile);

    dialogem.exec();
}

void DialogELFHeader::pushButtonEntryExtra_clicked()
{
    DialogHDE dialoghde(this);

    if(elffile->isELF64())
    {
        dialoghde.setData(pOptions,elffile->getFileName(),0,0,"x86-ASM",elffile->getDisasmMode(),"MASM",elffile->RVAToOffset64(elffile->getElfHeader_entry64()),1,tr("EntryPoint"));
    }
    else
    {
        dialoghde.setData(pOptions,elffile->getFileName(),0,0,"x86-ASM",elffile->getDisasmMode(),"MASM",elffile->RVAToOffset(elffile->getElfHeader_entry()),1,tr("EntryPoint"));
    }

    dialoghde.exec();
}
