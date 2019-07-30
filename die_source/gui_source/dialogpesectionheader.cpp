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
#include "dialogpesectionheader.h"
#include "ui_dialogpesectionheader.h"

DialogPESectionHeader::DialogPESectionHeader(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPESectionHeader)
{
    ui->setupUi(this);

    QTableWidgetItem *newItem;

    ui->tableWidgetSectionHeader->setRowCount(10);
    ui->tableWidgetSectionHeader->setColumnCount(3);

    for(int i=0; i<10; i++)
    {
        ui->tableWidgetSectionHeader->setRowHeight(i,20);
    }

    lineEditName=new QLineEditX;
    lineEditName->setHex(false);
    lineEditVirtualSize=new QLineEditX;
    lineEditVirtualAddress=new QLineEditX;
    lineEditSizeOfRawData=new QLineEditX;
    lineEditPointerToRawData=new QLineEditX;
    lineEditPointerToRelocations=new QLineEditX;
    lineEditPointerToLinenumbers=new QLineEditX;
    lineEditNumberOfRelocations=new QLineEditX;
    lineEditNumberOfLinenumbers=new QLineEditX;
    lineEditCharacteristics=new QLineEditX;

    lineEditName->setAlignment(Qt::AlignCenter);
    lineEditVirtualSize->setAlignment(Qt::AlignCenter);
    lineEditVirtualAddress->setAlignment(Qt::AlignCenter);
    lineEditSizeOfRawData->setAlignment(Qt::AlignCenter);
    lineEditPointerToRawData->setAlignment(Qt::AlignCenter);
    lineEditPointerToRelocations->setAlignment(Qt::AlignCenter);
    lineEditPointerToLinenumbers->setAlignment(Qt::AlignCenter);
    lineEditNumberOfRelocations->setAlignment(Qt::AlignCenter);
    lineEditNumberOfLinenumbers->setAlignment(Qt::AlignCenter);
    lineEditCharacteristics->setAlignment(Qt::AlignCenter);

    lineEditName->setInputMask("xxxxxxxx");
    lineEditVirtualSize->setInputMask("HHHHHHHH");
    lineEditVirtualAddress->setInputMask("HHHHHHHH");
    lineEditSizeOfRawData->setInputMask("HHHHHHHH");
    lineEditPointerToRawData->setInputMask("HHHHHHHH");
    lineEditPointerToRelocations->setInputMask("HHHHHHHH");
    lineEditPointerToLinenumbers->setInputMask("HHHHHHHH");
    lineEditNumberOfRelocations->setInputMask("HHHH");
    lineEditNumberOfLinenumbers->setInputMask("HHHH");
    lineEditCharacteristics->setInputMask("HHHHHHHH");

    newItem = new QTableWidgetItem;
    newItem->setText("Name");
    ui->tableWidgetSectionHeader->setItem(0,0,newItem);
    ui->tableWidgetSectionHeader->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("VirtualSize");
    ui->tableWidgetSectionHeader->setItem(1,0,newItem);
    ui->tableWidgetSectionHeader->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("VirtualAddress");
    ui->tableWidgetSectionHeader->setItem(2,0,newItem);
    ui->tableWidgetSectionHeader->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("SizeOfRawData");
    ui->tableWidgetSectionHeader->setItem(3,0,newItem);
    ui->tableWidgetSectionHeader->item(3,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("PointerToRawData");
    ui->tableWidgetSectionHeader->setItem(4,0,newItem);
    ui->tableWidgetSectionHeader->item(4,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("PointerToRelocations");
    ui->tableWidgetSectionHeader->setItem(5,0,newItem);
    ui->tableWidgetSectionHeader->item(5,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("PointerToLinenumbers");
    ui->tableWidgetSectionHeader->setItem(6,0,newItem);
    ui->tableWidgetSectionHeader->item(6,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("NumberOfRelocations");
    ui->tableWidgetSectionHeader->setItem(7,0,newItem);
    ui->tableWidgetSectionHeader->item(7,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("NumberOfLinenumbers");
    ui->tableWidgetSectionHeader->setItem(8,0,newItem);
    ui->tableWidgetSectionHeader->item(8,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("Characteristics");
    ui->tableWidgetSectionHeader->setItem(9,0,newItem);
    ui->tableWidgetSectionHeader->item(9,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(0,1),lineEditName);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(1,1),lineEditVirtualSize);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(2,1),lineEditVirtualAddress);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(3,1),lineEditSizeOfRawData);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(4,1),lineEditPointerToRawData);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(5,1),lineEditPointerToRelocations);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(6,1),lineEditPointerToLinenumbers);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(7,1),lineEditNumberOfRelocations);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(8,1),lineEditNumberOfLinenumbers);
    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(9,1),lineEditCharacteristics);

    connect(lineEditName,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditVirtualSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditVirtualAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfRawData,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditPointerToRawData,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditPointerToRelocations,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditPointerToLinenumbers,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditNumberOfRelocations,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditNumberOfLinenumbers,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    pushButtonCharacteristicsExtra=new QPushButtonX;
    pushButtonCharacteristicsExtra->setText("...");
    pushButtonCharacteristicsExtra->setFixedWidth(30);

    ui->tableWidgetSectionHeader->setIndexWidget(ui->tableWidgetSectionHeader->model()->index(9,2),pushButtonCharacteristicsExtra);
    connect(pushButtonCharacteristicsExtra,SIGNAL(clicked()),this,SLOT(pushButtonCharacteristicsExtra_clicked()));

    ui->tableWidgetSectionHeader->resizeColumnsToContents();

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPESectionHeader::~DialogPESectionHeader()
{
    delete ui;
}

void DialogPESectionHeader::setData(__DIE_OPTIONS *pOptions,PEFile *pefile, int nSection)
{
    this->nSection=nSection;

    DialogPEGeneric::setData(pOptions,pefile);
}

void DialogPESectionHeader::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPESectionHeader::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        pushButtonCharacteristicsExtra->setToolTip(tr("Section Characteristics"));
    }

    QString sTemp;

    sTemp=pefile->getSectionNameAsString(nSection);

    lineEditName->setText(sTemp);
    lineEditVirtualSize->setText(QString("%1").arg(pefile->getSection_VirtualSize(nSection),8,16,QChar('0')));
    lineEditVirtualAddress->setText(QString("%1").arg(pefile->getSection_VirtualAddress(nSection),8,16,QChar('0')));
    lineEditSizeOfRawData->setText(QString("%1").arg(pefile->getSection_SizeOfRawData(nSection),8,16,QChar('0')));
    lineEditPointerToRawData->setText(QString("%1").arg(pefile->getSection_PointerToRawData(nSection),8,16,QChar('0')));
    lineEditPointerToRelocations->setText(QString("%1").arg(pefile->getSection_PointerToRelocations(nSection),8,16,QChar('0')));
    lineEditPointerToLinenumbers->setText(QString("%1").arg(pefile->getSection_PointerToLinenumbers(nSection),8,16,QChar('0')));
    lineEditNumberOfRelocations->setText(QString("%1").arg(pefile->getSection_NumberOfRelocations(nSection),4,16,QChar('0')));
    lineEditNumberOfLinenumbers->setText(QString("%1").arg(pefile->getSection_NumberOfLinenumbers(nSection),4,16,QChar('0')));
    lineEditCharacteristics->setText(QString("%1").arg(pefile->getSection_Characteristics(nSection),8,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}


void DialogPESectionHeader::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineEditName->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditVirtualSize->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditVirtualAddress->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditSizeOfRawData->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditPointerToRawData->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditPointerToRelocations->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditPointerToLinenumbers->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditNumberOfRelocations->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditNumberOfLinenumbers->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditCharacteristics->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogPESectionHeader::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPESectionHeader::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setSectionNameAsString(nSection,lineEditName->text());
    pefile->setSection_VirtualSize(nSection,lineEditVirtualSize->text().toUInt(&bTemp,16));
    pefile->setSection_VirtualAddress(nSection,lineEditVirtualAddress->text().toUInt(&bTemp,16));
    pefile->setSection_SizeOfRawData(nSection,lineEditSizeOfRawData->text().toUInt(&bTemp,16));
    pefile->setSection_PointerToRawData(nSection,lineEditPointerToRawData->text().toUInt(&bTemp,16));
    pefile->setSection_PointerToRelocations(nSection,lineEditPointerToRelocations->text().toUInt(&bTemp,16));
    pefile->setSection_PointerToLinenumbers(nSection,lineEditPointerToLinenumbers->text().toUInt(&bTemp,16));
    pefile->setSection_NumberOfRelocations(nSection,lineEditNumberOfRelocations->text().toUInt(&bTemp,16));
    pefile->setSection_NumberOfLinenumbers(nSection,lineEditNumberOfLinenumbers->text().toUInt(&bTemp,16));
    pefile->setSection_Characteristics(nSection,lineEditCharacteristics->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPESectionHeader::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogPESectionHeader::pushButtonCharacteristicsExtra_clicked()
{
    DialogPESectionCharacteristics dialogpesectioncharacteristics(this);

    dialogpesectioncharacteristics.setData(pOptions,pefile,nSection);

    dialogpesectioncharacteristics.exec();
}
