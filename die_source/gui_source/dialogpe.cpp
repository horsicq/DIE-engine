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
#include "dialogpe.h"
#include "ui_dialogpe.h"

DialogPE::DialogPE(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPE)
{
    ui->setupUi(this);

    ui->tableWidgetBasic->setRowCount(6);
    ui->tableWidgetBasic->setColumnCount(3);

    for(int i=0; i<6; i++)
    {
        ui->tableWidgetBasic->setRowHeight(i,20);
    }

    QTableWidgetItem *newItem;

    lineEditEntryPoint=new QLineEditX;
    lineEditImageBase=new QLineEditX;
    lineEditSizeOfImage=new QLineEditX;
    lineEditTimeDateStamp=new QLineEditX;
    lineEditCheckSum=new QLineEditX;
    lineEditSubsystem=new QLineEditX;

    lineEditEntryPoint->setAlignment(Qt::AlignCenter);
    lineEditImageBase->setAlignment(Qt::AlignCenter);
    lineEditSizeOfImage->setAlignment(Qt::AlignCenter);
    lineEditTimeDateStamp->setAlignment(Qt::AlignCenter);
    lineEditCheckSum->setAlignment(Qt::AlignCenter);
    lineEditSubsystem->setAlignment(Qt::AlignCenter);

    lineEditEntryPoint->setInputMask("HHHHHHHH");
    lineEditImageBase->setInputMask("HHHHHHHH");
    lineEditSizeOfImage->setInputMask("HHHHHHHH");
    lineEditTimeDateStamp->setInputMask("HHHHHHHH");
    lineEditCheckSum->setInputMask("HHHHHHHH");
    lineEditSubsystem->setInputMask("HH");

    newItem = new QTableWidgetItem;
    newItem->setText(tr("AddressOfEntryPoint"));
    ui->tableWidgetBasic->setItem(0,0,newItem);
    ui->tableWidgetBasic->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText(tr("ImageBase"));
    ui->tableWidgetBasic->setItem(1,0,newItem);
    ui->tableWidgetBasic->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText(tr("SizeOfImage"));
    ui->tableWidgetBasic->setItem(2,0,newItem);
    ui->tableWidgetBasic->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText(tr("TimeDateStamp"));
    ui->tableWidgetBasic->setItem(3,0,newItem);
    ui->tableWidgetBasic->item(3,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText(tr("CheckSum"));
    ui->tableWidgetBasic->setItem(4,0,newItem);
    ui->tableWidgetBasic->item(4,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText(tr("Subsystem"));
    ui->tableWidgetBasic->setItem(5,0,newItem);
    ui->tableWidgetBasic->item(5,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(0,1),lineEditEntryPoint);
    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(1,1),lineEditImageBase);
    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(2,1),lineEditSizeOfImage);
    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(3,1),lineEditTimeDateStamp);
    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(4,1),lineEditCheckSum);
    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(5,1),lineEditSubsystem);

    connect(lineEditEntryPoint,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditImageBase,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfImage,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditTimeDateStamp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditCheckSum,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSubsystem,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    pushButtonAddressOfEntryPointExtra=new QPushButtonX;
    pushButtonAddressOfEntryPointExtra->setText("...");
    pushButtonAddressOfEntryPointExtra->setFixedWidth(30);

    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(0,2),pushButtonAddressOfEntryPointExtra);
    connect(pushButtonAddressOfEntryPointExtra,SIGNAL(clicked()),this,SLOT(pushButtonAddressOfEntryPointExtra_clicked()));

    pushButtonTimeDateStampExtra=new QPushButtonX;
    pushButtonTimeDateStampExtra->setText("...");
    pushButtonTimeDateStampExtra->setFixedWidth(30);

    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(3,2),pushButtonTimeDateStampExtra);
    connect(pushButtonTimeDateStampExtra,SIGNAL(clicked()),this,SLOT(pushButtonTimeDateStampExtra_clicked()));

    pushButtonCheckSumExtra=new QPushButtonX;
    pushButtonCheckSumExtra->setText("...");
    pushButtonCheckSumExtra->setFixedWidth(30);

    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(4,2),pushButtonCheckSumExtra);
    connect(pushButtonCheckSumExtra,SIGNAL(clicked()),this,SLOT(pushButtonCheckSumExtra_clicked()));

    pushButtonSubsystemExtra=new QPushButtonX;
    pushButtonSubsystemExtra->setText("...");
    pushButtonSubsystemExtra->setFixedWidth(30);

    ui->tableWidgetBasic->setIndexWidget(ui->tableWidgetBasic->model()->index(5,2),pushButtonSubsystemExtra);
    connect(pushButtonSubsystemExtra,SIGNAL(clicked()),this,SLOT(pushButtonSubsystemExtra_clicked()));

    ui->tableWidgetBasic->resizeColumnsToContents();
    ui->tableWidgetBasic->setColumnWidth(1,120);

    ui->checkBoxReadOnly->setChecked(true);
    ui->pushButtonApply->setEnabled(false);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPE::~DialogPE()
{
    delete ui;
}

bool DialogPE::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));

        ui->pushButtonDirectories->setToolTip(tr("Directories"));
        ui->pushButtonDosHeader->setToolTip(tr("MS DOS Header"));
        ui->pushButtonNtHeaders->setToolTip(tr("NT Headers"));
        ui->pushButtonOverlay->setToolTip(tr("Overlay"));
        ui->pushButtonSections->setToolTip(tr("Sections table"));
        ui->pushButtonStub->setToolTip(tr("MS DOS Stub"));

        pushButtonAddressOfEntryPointExtra->setToolTip(tr("Address of EntryPoint"));
        pushButtonCheckSumExtra->setToolTip(tr("CheckSum"));
        pushButtonSubsystemExtra->setToolTip(tr("Subsystem"));
        pushButtonTimeDateStampExtra->setToolTip(tr("TimeDateStamp"));
    }

    lineEditEntryPoint->setText(QString("%1").arg(pefile->getOptionalHeader_AddressOfEntryPoint(),8,16,QChar('0')));

    if(pefile->isPEPlus())
    {
        lineEditImageBase->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditImageBase->setText(QString("%1").arg(pefile->getOptionalHeader_ImageBase64(),16,16,QChar('0')));
    }
    else
    {
        lineEditImageBase->setInputMask("HHHHHHHH");
        lineEditImageBase->setText(QString("%1").arg(pefile->getOptionalHeader_ImageBase(),8,16,QChar('0')));
    }

    lineEditSizeOfImage->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfImage(),8,16,QChar('0')));
    lineEditTimeDateStamp->setText(QString("%1").arg(pefile->getFileHeader_TimeDateStamp(),8,16,QChar('0')));
    lineEditCheckSum->setText(QString("%1").arg(pefile->getOptionalHeader_CheckSum(),8,16,QChar('0')));
    lineEditSubsystem->setText(QString("%1").arg(pefile->getOptionalHeader_Subsystem(),2,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPE::on_pushButtonDosHeader_clicked()
{
    DialogPEDosHeader dialogpedosheader(this);

    dialogpedosheader.setData(pOptions,pefile);

    dialogpedosheader.exec();
}

void DialogPE::on_pushButtonStub_clicked()
{
    DialogPEStub dialogpestub(this);

    dialogpestub.setData(pOptions,pefile);

    dialogpestub.exec();
}

void DialogPE::on_pushButtonOverlay_clicked()
{
    DialogPEOverlay dialogpeoverlay(this);

    dialogpeoverlay.setData(pOptions,pefile);

    dialogpeoverlay.exec();
}

void DialogPE::on_pushButtonNtHeaders_clicked()
{
    DialogPENtHeaders dialogpentheaders(this);

    dialogpentheaders.setData(pOptions,pefile);

    dialogpentheaders.exec();
}

void DialogPE::on_pushButtonSections_clicked()
{
    DialogPESections dialogpesections(this);

    dialogpesections.setData(pOptions,pefile);

    dialogpesections.exec();
}

void DialogPE::on_pushButtonDirectories_clicked()
{
    DialogPEDirectories dialogpedirectories(this);

    dialogpedirectories.setData(pOptions,pefile);

    dialogpedirectories.exec();
}

void DialogPE::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogPE::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    lineEditEntryPoint->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditImageBase->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfImage->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditTimeDateStamp->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditCheckSum->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSubsystem->setReadOnly(ui->checkBoxReadOnly->checkState());
}

void DialogPE::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPE::on_pushButtonApply_clicked()
{
    bool bTemp;

    ui->pushButtonApply->setEnabled(false);

    //PE _pe(pbaData->data(),pbaData->size());

    pefile->setOptionalHeader_AddressOfEntryPoint(lineEditEntryPoint->text().toUInt(&bTemp,16));

    if(pefile->isPEPlus())
    {
        pefile->setOptionalHeader_ImageBase64(lineEditImageBase->text().toULongLong(&bTemp,16));
    }
    else
    {
        pefile->setOptionalHeader_ImageBase(lineEditImageBase->text().toUInt(&bTemp,16));
    }

    pefile->setOptionalHeader_SizeOfImage(lineEditSizeOfImage->text().toUInt(&bTemp,16));
    pefile->setFileHeader_TimeDateStamp(lineEditTimeDateStamp->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_CheckSum(lineEditCheckSum->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_Subsystem(lineEditSubsystem->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}
void DialogPE::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogPE::pushButtonTimeDateStampExtra_clicked()
{
    DialogPETimeDateStamp dialogpetimedatestamp(this);

    dialogpetimedatestamp.setData(pOptions,pefile);

    dialogpetimedatestamp.exec();
}

void DialogPE::pushButtonAddressOfEntryPointExtra_clicked()
{
    DialogHDE dialoghde(this);
    //    dialoghde.setData(pefile->getFileName(),pefile->RVAToOffset(pefile->getOptionalHeader_AddressOfEntryPoint()),0,pefile->getDisasmMode(),pOptions);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"x86-ASM",pefile->getDisasmMode(),"MASM",pefile->RVAToOffset(pefile->getOptionalHeader_AddressOfEntryPoint()),1,tr("EntryPoint"));

    dialoghde.exec();
}

void DialogPE::pushButtonCheckSumExtra_clicked()
{
    DialogPECheckSum dialogchecksum(this);

    dialogchecksum.setData(pOptions,pefile);

    dialogchecksum.exec();

    //    reload();
}

void DialogPE::pushButtonSubsystemExtra_clicked()
{
    DialogPESubsystem dialogpesubsystem(this);

    dialogpesubsystem.setData(pOptions,pefile);

    dialogpesubsystem.exec();
}
