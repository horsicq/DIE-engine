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
#include "dialogpentheaders.h"
#include "ui_dialogpentheaders.h"

DialogPENtHeaders::DialogPENtHeaders(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPENtHeaders)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);

    QTableWidgetItem *newItem;

    QString sLabels[]= { "Machine",
                         "NumberOfSections",
                         "TimeDateStamp",
                         "PointerToSymbolTable",
                         "NumberOfSymbols",
                         "SizeOfOptionalHeader",
                         "Characteristics"
                       };

    lineEditMachine=new QLineEditX;
    lineEditMachine->setAlignment(Qt::AlignCenter);
    lineEditMachine->setInputMask("HHHH");
    lineEditNumberOfSections=new QLineEditX;
    lineEditNumberOfSections->setAlignment(Qt::AlignCenter);
    lineEditNumberOfSections->setInputMask("HHHH");
    lineEditTimeDateStamp=new QLineEditX;
    lineEditTimeDateStamp->setAlignment(Qt::AlignCenter);
    lineEditTimeDateStamp->setInputMask("HHHHHHHH");
    lineEditPointerToSymbolTable=new QLineEditX;
    lineEditPointerToSymbolTable->setAlignment(Qt::AlignCenter);
    lineEditPointerToSymbolTable->setInputMask("HHHHHHHH");
    lineEditNumberOfSymbols=new QLineEditX;
    lineEditNumberOfSymbols->setAlignment(Qt::AlignCenter);
    lineEditNumberOfSymbols->setInputMask("HHHHHHHH");
    lineEditSizeOfOptionalHeader=new QLineEditX;
    lineEditSizeOfOptionalHeader->setAlignment(Qt::AlignCenter);
    lineEditSizeOfOptionalHeader->setInputMask("HHHH");
    lineEditCharacteristics=new QLineEditX;
    lineEditCharacteristics->setAlignment(Qt::AlignCenter);
    lineEditCharacteristics->setInputMask("HHHH");

    ui->tableWidgetFileHeader->setRowCount(7);
    ui->tableWidgetFileHeader->setColumnCount(3);

    for(int i=0; i<7; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(sLabels[i]);
        ui->tableWidgetFileHeader->setItem(i,0,newItem);
        ui->tableWidgetFileHeader->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidgetFileHeader->setRowHeight(i,20);
    }

    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(0,1),lineEditMachine);
    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(1,1),lineEditNumberOfSections);
    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(2,1),lineEditTimeDateStamp);
    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(3,1),lineEditPointerToSymbolTable);
    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(4,1),lineEditNumberOfSymbols);
    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(5,1),lineEditSizeOfOptionalHeader);
    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(6,1),lineEditCharacteristics);


    connect(lineEditMachine,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditNumberOfSections,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditTimeDateStamp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditPointerToSymbolTable,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditNumberOfSymbols,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfOptionalHeader,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    QString sLabels2[]= { "Magic",
                          "MajorLinkerVersion",
                          "MinorLinkerVersion",
                          "SizeOfCode",
                          "SizeOfInitializedData",
                          "SizeOfUninitializedData",
                          "AddressOfEntryPoint",
                          "BaseOfCode",
                          "BaseOfData",
                          "ImageBase",
                          "SectionAlignment",
                          "FileAlignment",
                          "MajorOperatingSystemVersion",
                          "MinorOperatingSystemVersion",
                          "MajorImageVersion",
                          "MinorImageVersion",
                          "MajorSubsystemVersion",
                          "MinorSubsystemVersion",
                          "Win32VersionValue",
                          "SizeOfImage",
                          "SizeOfHeaders",
                          "CheckSum",
                          "Subsystem",
                          "DllCharacteristics",
                          "SizeOfStackReserve",
                          "SizeOfStackCommit",
                          "SizeOfHeapReserve",
                          "SizeOfHeapCommit",
                          "LoaderFlags",
                          "NumberOfRvaAndSizes"
                        };

    lineEditMagic=new QLineEditX;
    lineEditMagic->setAlignment(Qt::AlignCenter);
    lineEditMagic->setInputMask("HHHH");
    lineEditMajorLinkerVersion=new QLineEditX;
    lineEditMajorLinkerVersion->setAlignment(Qt::AlignCenter);
    lineEditMajorLinkerVersion->setInputMask("HH");
    lineEditMinorLinkerVersion=new QLineEditX;
    lineEditMinorLinkerVersion->setAlignment(Qt::AlignCenter);
    lineEditMinorLinkerVersion->setInputMask("HH");
    lineEditSizeOfCode=new QLineEditX;
    lineEditSizeOfCode->setAlignment(Qt::AlignCenter);
    lineEditSizeOfCode->setInputMask("HHHHHHHH");
    lineEditSizeOfInitializedData=new QLineEditX;
    lineEditSizeOfInitializedData->setAlignment(Qt::AlignCenter);
    lineEditSizeOfInitializedData->setInputMask("HHHHHHHH");
    lineEditSizeOfUninitializedData=new QLineEditX;
    lineEditSizeOfUninitializedData->setAlignment(Qt::AlignCenter);
    lineEditSizeOfUninitializedData->setInputMask("HHHHHHHH");
    lineEditAddressOfEntryPoint=new QLineEditX;
    lineEditAddressOfEntryPoint->setAlignment(Qt::AlignCenter);
    lineEditAddressOfEntryPoint->setInputMask("HHHHHHHH");
    lineEditBaseOfCode=new QLineEditX;
    lineEditBaseOfCode->setAlignment(Qt::AlignCenter);
    lineEditBaseOfCode->setInputMask("HHHHHHHH");
    lineEditBaseOfData=new QLineEditX;
    lineEditBaseOfData->setAlignment(Qt::AlignCenter);
    lineEditBaseOfData->setInputMask("HHHHHHHH");
    lineEditImageBase=new QLineEditX;
    lineEditImageBase->setAlignment(Qt::AlignCenter);
    lineEditImageBase->setInputMask("HHHHHHHH");
    lineEditSectionAlignment=new QLineEditX;
    lineEditSectionAlignment->setAlignment(Qt::AlignCenter);
    lineEditSectionAlignment->setInputMask("HHHHHHHH");
    lineEditFileAlignment=new QLineEditX;
    lineEditFileAlignment->setAlignment(Qt::AlignCenter);
    lineEditFileAlignment->setInputMask("HHHHHHHH");
    lineEditMajorOperatingSystemVersion=new QLineEditX;
    lineEditMajorOperatingSystemVersion->setAlignment(Qt::AlignCenter);
    lineEditMajorOperatingSystemVersion->setInputMask("HHHH");
    lineEditMinorOperatingSystemVersion=new QLineEditX;
    lineEditMinorOperatingSystemVersion->setAlignment(Qt::AlignCenter);
    lineEditMinorOperatingSystemVersion->setInputMask("HHHH");
    lineEditMajorImageVersion=new QLineEditX;
    lineEditMajorImageVersion->setAlignment(Qt::AlignCenter);
    lineEditMajorImageVersion->setInputMask("HHHH");
    lineEditMinorImageVersion=new QLineEditX;
    lineEditMinorImageVersion->setAlignment(Qt::AlignCenter);
    lineEditMinorImageVersion->setInputMask("HHHH");
    lineEditMajorSubsystemVersion=new QLineEditX;
    lineEditMajorSubsystemVersion->setAlignment(Qt::AlignCenter);
    lineEditMajorSubsystemVersion->setInputMask("HHHH");
    lineEditMinorSubsystemVersion=new QLineEditX;
    lineEditMinorSubsystemVersion->setAlignment(Qt::AlignCenter);
    lineEditMinorSubsystemVersion->setInputMask("HHHH");
    lineEditWin32VersionValue=new QLineEditX;
    lineEditWin32VersionValue->setAlignment(Qt::AlignCenter);
    lineEditWin32VersionValue->setInputMask("HHHHHHHH");
    lineEditSizeOfImage=new QLineEditX;
    lineEditSizeOfImage->setAlignment(Qt::AlignCenter);
    lineEditSizeOfImage->setInputMask("HHHHHHHH");
    lineEditSizeOfHeaders=new QLineEditX;
    lineEditSizeOfHeaders->setAlignment(Qt::AlignCenter);
    lineEditSizeOfHeaders->setInputMask("HHHHHHHH");
    lineEditCheckSum=new QLineEditX;
    lineEditCheckSum->setAlignment(Qt::AlignCenter);
    lineEditCheckSum->setInputMask("HHHHHHHH");
    lineEditSubsystem=new QLineEditX;
    lineEditSubsystem->setAlignment(Qt::AlignCenter);
    lineEditSubsystem->setInputMask("HHHH");
    lineEditDllCharacteristics=new QLineEditX;
    lineEditDllCharacteristics->setAlignment(Qt::AlignCenter);
    lineEditDllCharacteristics->setInputMask("HHHH");
    lineEditSizeOfStackReserve=new QLineEditX;
    lineEditSizeOfStackReserve->setAlignment(Qt::AlignCenter);
    lineEditSizeOfStackReserve->setInputMask("HHHHHHHH");
    lineEditSizeOfStackCommit=new QLineEditX;
    lineEditSizeOfStackCommit->setAlignment(Qt::AlignCenter);
    lineEditSizeOfStackCommit->setInputMask("HHHHHHHH");
    lineEditSizeOfHeapReserve=new QLineEditX;
    lineEditSizeOfHeapReserve->setAlignment(Qt::AlignCenter);
    lineEditSizeOfHeapReserve->setInputMask("HHHHHHHH");
    lineEditSizeOfHeapCommit=new QLineEditX;
    lineEditSizeOfHeapCommit->setAlignment(Qt::AlignCenter);
    lineEditSizeOfHeapCommit->setInputMask("HHHHHHHH");
    lineEditLoaderFlags=new QLineEditX;
    lineEditLoaderFlags->setAlignment(Qt::AlignCenter);
    lineEditLoaderFlags->setInputMask("HHHHHHHH");
    lineEditNumberOfRvaAndSizes=new QLineEditX;
    lineEditNumberOfRvaAndSizes->setAlignment(Qt::AlignCenter);
    lineEditNumberOfRvaAndSizes->setInputMask("HHHHHHHH");

    ui->tableWidgetOptionalHeader->setRowCount(30);
    ui->tableWidgetOptionalHeader->setColumnCount(3);

    for(int i=0; i<30; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(sLabels2[i]);
        ui->tableWidgetOptionalHeader->setItem(i,0,newItem);
        ui->tableWidgetOptionalHeader->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidgetOptionalHeader->setRowHeight(i,20);
    }

    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(0,1),lineEditMagic);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(1,1),lineEditMajorLinkerVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(2,1),lineEditMinorLinkerVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(3,1),lineEditSizeOfCode);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(4,1),lineEditSizeOfInitializedData);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(5,1),lineEditSizeOfUninitializedData);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(6,1),lineEditAddressOfEntryPoint);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(7,1),lineEditBaseOfCode);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(8,1),lineEditBaseOfData);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(9,1),lineEditImageBase);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(10,1),lineEditSectionAlignment);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(11,1),lineEditFileAlignment);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(12,1),lineEditMajorOperatingSystemVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(13,1),lineEditMinorOperatingSystemVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(14,1),lineEditMajorImageVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(15,1),lineEditMinorImageVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(16,1),lineEditMajorSubsystemVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(17,1),lineEditMinorSubsystemVersion);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(18,1),lineEditWin32VersionValue);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(19,1),lineEditSizeOfImage);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(20,1),lineEditSizeOfHeaders);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(21,1),lineEditCheckSum);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(22,1),lineEditSubsystem);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(23,1),lineEditDllCharacteristics);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(24,1),lineEditSizeOfStackReserve);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(25,1),lineEditSizeOfStackCommit);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(26,1),lineEditSizeOfHeapReserve);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(27,1),lineEditSizeOfHeapCommit);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(28,1),lineEditLoaderFlags);
    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(29,1),lineEditNumberOfRvaAndSizes);

    connect(lineEditMagic,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMajorLinkerVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMinorLinkerVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfCode,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfInitializedData,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfUninitializedData,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditAddressOfEntryPoint,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditBaseOfCode,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditBaseOfData,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditImageBase,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSectionAlignment,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditFileAlignment,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMajorOperatingSystemVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMinorOperatingSystemVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMajorImageVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMinorImageVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMajorSubsystemVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMinorSubsystemVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditWin32VersionValue,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfImage,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfHeaders,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditCheckSum,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSubsystem,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditDllCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfStackReserve,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfStackCommit,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfHeapReserve,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfHeapCommit,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditLoaderFlags,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditNumberOfRvaAndSizes,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    pushButtonMachineExtra=new QPushButtonX;
    pushButtonMachineExtra->setText("...");
    pushButtonMachineExtra->setFixedWidth(30);

    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(0,2),pushButtonMachineExtra);
    connect(pushButtonMachineExtra,SIGNAL(clicked()),this,SLOT(pushButtonMachineExtra_clicked()));

    pushButtonNumberOfSectionsExtra=new QPushButtonX;
    pushButtonNumberOfSectionsExtra->setText("...");
    pushButtonNumberOfSectionsExtra->setFixedWidth(30);

    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(1,2),pushButtonNumberOfSectionsExtra);
    connect(pushButtonNumberOfSectionsExtra,SIGNAL(clicked()),this,SLOT(pushButtonNumberOfSectionsExtra_clicked()));

    pushButtonTimeDateStampExtra=new QPushButtonX;
    pushButtonTimeDateStampExtra->setText("...");
    pushButtonTimeDateStampExtra->setFixedWidth(30);

    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(2,2),pushButtonTimeDateStampExtra);
    connect(pushButtonTimeDateStampExtra,SIGNAL(clicked()),this,SLOT(pushButtonTimeDateStampExtra_clicked()));

    pushButtonCharacteristicsExtra=new QPushButtonX;
    pushButtonCharacteristicsExtra->setText("...");
    pushButtonCharacteristicsExtra->setFixedWidth(30);

    ui->tableWidgetFileHeader->setIndexWidget(ui->tableWidgetFileHeader->model()->index(6,2),pushButtonCharacteristicsExtra);
    connect(pushButtonCharacteristicsExtra,SIGNAL(clicked()),this,SLOT(pushButtonCharacteristicsExtra_clicked()));

    pushButtonMagicExtra=new QPushButtonX;
    pushButtonMagicExtra->setText("...");
    pushButtonMagicExtra->setFixedWidth(30);

    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(0,2),pushButtonMagicExtra);
    connect(pushButtonMagicExtra,SIGNAL(clicked()),this,SLOT(pushButtonMagicExtra_clicked()));

    pushButtonAddressOfEntryPointExtra=new QPushButtonX;
    pushButtonAddressOfEntryPointExtra->setText("...");
    pushButtonAddressOfEntryPointExtra->setFixedWidth(30);

    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(6,2),pushButtonAddressOfEntryPointExtra);
    connect(pushButtonAddressOfEntryPointExtra,SIGNAL(clicked()),this,SLOT(pushButtonAddressOfEntryPointExtra_clicked()));

    pushButtonSizeOfHeadersExtra=new QPushButtonX;
    pushButtonSizeOfHeadersExtra->setText("...");
    pushButtonSizeOfHeadersExtra->setFixedWidth(30);

    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(20,2),pushButtonSizeOfHeadersExtra);
    connect(pushButtonSizeOfHeadersExtra,SIGNAL(clicked()),this,SLOT(pushButtonSizeOfHeadersExtra_clicked()));

    pushButtonCheckSumExtra=new QPushButtonX;
    pushButtonCheckSumExtra->setText("...");
    pushButtonCheckSumExtra->setFixedWidth(30);

    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(21,2),pushButtonCheckSumExtra);
    connect(pushButtonCheckSumExtra,SIGNAL(clicked()),this,SLOT(pushButtonCheckSumExtra_clicked()));

    pushButtonSubsystemExtra=new QPushButtonX;
    pushButtonSubsystemExtra->setText("...");
    pushButtonSubsystemExtra->setFixedWidth(30);

    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(22,2),pushButtonSubsystemExtra);
    connect(pushButtonSubsystemExtra,SIGNAL(clicked()),this,SLOT(pushButtonSubsystemExtra_clicked()));

    pushButtonDllCharacteristicsExtra=new QPushButtonX;
    pushButtonDllCharacteristicsExtra->setText("...");
    pushButtonDllCharacteristicsExtra->setFixedWidth(30);

    ui->tableWidgetOptionalHeader->setIndexWidget(ui->tableWidgetOptionalHeader->model()->index(23,2),pushButtonDllCharacteristicsExtra);
    connect(pushButtonDllCharacteristicsExtra,SIGNAL(clicked()),this,SLOT(pushButtonDllCharacteristicsExtra_clicked()));
    //    ui->tableWidgetFileHeader->resizeColumnToContents(2);
    //    ui->tableWidgetOptionalHeader->resizeColumnToContents(2);

    ui->tableWidgetFileHeader->setColumnWidth(0,170);
    ui->tableWidgetFileHeader->setColumnWidth(1,120);
    ui->tableWidgetOptionalHeader->setColumnWidth(0,170);
    ui->tableWidgetOptionalHeader->setColumnWidth(1,120);
    ui->tableWidgetFileHeader->resizeColumnToContents(2);
    ui->tableWidgetOptionalHeader->resizeColumnToContents(2);

    ui->checkBoxReadOnly->setChecked(true);

    //connect(&pefile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    //connect(this, SIGNAL(reloadSignal()), this, SLOT(reload()));

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_hex));
}

DialogPENtHeaders::~DialogPENtHeaders()
{
    delete ui;
}

void DialogPENtHeaders::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogPENtHeaders::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPENtHeaders::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    //PE pefile(pbaData->data(),pbaData->size());

    pefile->setNTHeaders_Signature(ui->lineEditSignature->text().toUInt(&bTemp,16));

    pefile->setFileHeader_Machine(lineEditMachine->text().toUInt(&bTemp,16));
    pefile->setFileHeader_NumberOfSections(lineEditNumberOfSections->text().toUInt(&bTemp,16));
    pefile->setFileHeader_TimeDateStamp(lineEditTimeDateStamp->text().toUInt(&bTemp,16));
    pefile->setFileHeader_PointerToSymbolTable(lineEditPointerToSymbolTable->text().toUInt(&bTemp,16));
    pefile->setFileHeader_NumberOfSymbols(lineEditNumberOfSymbols->text().toUInt(&bTemp,16));
    pefile->setFileHeader_SizeOfOptionalHeader(lineEditSizeOfOptionalHeader->text().toUInt(&bTemp,16));
    pefile->setFileHeader_Characteristics(lineEditCharacteristics->text().toUInt(&bTemp,16));

    pefile->setOptionalHeader_Magic(lineEditMagic->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MajorLinkerVersion(lineEditMajorLinkerVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MinorLinkerVersion(lineEditMinorLinkerVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_SizeOfCode(lineEditSizeOfCode->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_SizeOfInitializedData(lineEditSizeOfInitializedData->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_SizeOfUninitializedData(lineEditSizeOfUninitializedData->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_AddressOfEntryPoint(lineEditAddressOfEntryPoint->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_BaseOfCode(lineEditBaseOfCode->text().toUInt(&bTemp,16));

    if(!pefile->isPEPlus())
    {
        pefile->setOptionalHeader_BaseOfData(lineEditBaseOfData->text().toUInt(&bTemp,16));
    }

    if(pefile->isPEPlus())
    {
        pefile->setOptionalHeader_ImageBase64(lineEditImageBase->text().toULongLong(&bTemp,16));
    }
    else
    {
        pefile->setOptionalHeader_ImageBase(lineEditImageBase->text().toUInt(&bTemp,16));
    }

    pefile->setOptionalHeader_SectionAlignment(lineEditSectionAlignment->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_FileAlignment(lineEditFileAlignment->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MajorOperatingSystemVersion(lineEditMajorOperatingSystemVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MinorOperatingSystemVersion(lineEditMinorOperatingSystemVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MajorImageVersion(lineEditMajorImageVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MinorImageVersion(lineEditMinorImageVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MajorSubsystemVersion(lineEditMajorSubsystemVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_MinorSubsystemVersion(lineEditMinorSubsystemVersion->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_Win32VersionValue(lineEditWin32VersionValue->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_SizeOfImage(lineEditSizeOfImage->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_SizeOfHeaders(lineEditSizeOfHeaders->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_CheckSum(lineEditCheckSum->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_Subsystem(lineEditSubsystem->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_DllCharacteristics(lineEditDllCharacteristics->text().toUInt(&bTemp,16));

    if(pefile->isPEPlus())
    {
        pefile->setOptionalHeader_SizeOfStackReserve64(lineEditSizeOfStackReserve->text().toULongLong(&bTemp,16));
    }
    else
    {
        pefile->setOptionalHeader_SizeOfStackReserve(lineEditSizeOfStackReserve->text().toUInt(&bTemp,16));
    }

    if(pefile->isPEPlus())
    {
        pefile->setOptionalHeader_SizeOfStackCommit64(lineEditSizeOfStackCommit->text().toULongLong(&bTemp,16));
    }
    else
    {
        pefile->setOptionalHeader_SizeOfStackCommit(lineEditSizeOfStackCommit->text().toUInt(&bTemp,16));
    }

    if(pefile->isPEPlus())
    {
        pefile->setOptionalHeader_SizeOfHeapReserve64(lineEditSizeOfHeapReserve->text().toULongLong(&bTemp,16));
    }
    else
    {
        pefile->setOptionalHeader_SizeOfHeapReserve(lineEditSizeOfHeapReserve->text().toUInt(&bTemp,16));
    }

    if(pefile->isPEPlus())
    {
        pefile->setOptionalHeader_SizeOfHeapCommit(lineEditSizeOfHeapCommit->text().toULongLong(&bTemp,16));
    }
    else
    {
        pefile->setOptionalHeader_SizeOfHeapCommit(lineEditSizeOfHeapCommit->text().toUInt(&bTemp,16));
    }

    pefile->setOptionalHeader_LoaderFlags(lineEditLoaderFlags->text().toUInt(&bTemp,16));
    pefile->setOptionalHeader_NumberOfRvaAndSizes(lineEditNumberOfRvaAndSizes->text().toUInt(&bTemp,16));

    //    baLocalCopy.clear();
    //    baLocalCopy.append(pefile->data(),pefile->size());

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

bool DialogPENtHeaders::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));

        pushButtonAddressOfEntryPointExtra->setToolTip(tr("Address of EntryPoint"));
        pushButtonCheckSumExtra->setToolTip(tr("CheckSum"));
        pushButtonSubsystemExtra->setToolTip(tr("Subsystem"));
        pushButtonTimeDateStampExtra->setToolTip(tr("TimeDateStamp"));

        pushButtonMachineExtra->setToolTip(tr("Machine"));
        pushButtonNumberOfSectionsExtra->setToolTip(tr("Sections table"));
        pushButtonCharacteristicsExtra->setToolTip(tr("Characteristics"));
        pushButtonMagicExtra->setToolTip(tr("Magic"));
        pushButtonSizeOfHeadersExtra->setToolTip(tr("Headers"));
        pushButtonDllCharacteristicsExtra->setToolTip(tr("DLL characteristics"));

        ui->pushButtonHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("NT Headers")));
        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg(tr("NT Headers")));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg(tr("NT Headers")));

        ui->lineEditSignature->setToolTip(tr("NT Header Signature"));
    }

    nOffset=pefile->getNtHeadersOffset();
    nSize=pefile->getNtHeadersSize()-16*sizeof(IMAGE_DATA_DIRECTORY);

    ui->lineEditOffset->setText(QString("%1").arg(nOffset));
    ui->lineEditSize->setText(QString("%1").arg(nSize));

    ui->lineEditSignature->setText(QString("%1").arg(pefile->getNTHeaders_Signature(),8,16,QChar('0')));

    lineEditMachine->setText(QString("%1").arg(pefile->getFileHeader_Machine(),4,16,QChar('0')));
    lineEditNumberOfSections->setText(QString("%1").arg(pefile->getFileHeader_NumberOfSections(),4,16,QChar('0')));
    lineEditTimeDateStamp->setText(QString("%1").arg(pefile->getFileHeader_TimeDateStamp(),8,16,QChar('0')));
    lineEditPointerToSymbolTable->setText(QString("%1").arg(pefile->getFileHeader_PointerToSymbolTable(),8,16,QChar('0')));
    lineEditNumberOfSymbols->setText(QString("%1").arg(pefile->getFileHeader_NumberOfSymbols(),8,16,QChar('0')));
    lineEditSizeOfOptionalHeader->setText(QString("%1").arg(pefile->getFileHeader_SizeOfOptionalHeader(),4,16,QChar('0')));
    lineEditCharacteristics->setText(QString("%1").arg(pefile->getFileHeader_Characteristics(),4,16,QChar('0')));

    lineEditMagic->setText(QString("%1").arg(pefile->getOptionalHeader_Magic(),4,16,QChar('0')));
    lineEditMajorLinkerVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MajorLinkerVersion(),2,16,QChar('0')));
    lineEditMinorLinkerVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MinorLinkerVersion(),2,16,QChar('0')));
    lineEditSizeOfCode->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfCode(),8,16,QChar('0')));
    lineEditSizeOfInitializedData->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfInitializedData(),8,16,QChar('0')));
    lineEditSizeOfUninitializedData->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfUninitializedData(),8,16,QChar('0')));
    lineEditAddressOfEntryPoint->setText(QString("%1").arg(pefile->getOptionalHeader_AddressOfEntryPoint(),8,16,QChar('0')));
    lineEditBaseOfCode->setText(QString("%1").arg(pefile->getOptionalHeader_BaseOfCode(),8,16,QChar('0')));

    if(pefile->isPEPlus())
    {
        lineEditBaseOfData->setEnabled(false);
    }
    else
    {
        lineEditBaseOfData->setText(QString("%1").arg(pefile->getOptionalHeader_BaseOfData(),8,16,QChar('0')));
    }

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

    lineEditSectionAlignment->setText(QString("%1").arg(pefile->getOptionalHeader_SectionAlignment(),8,16,QChar('0')));
    lineEditFileAlignment->setText(QString("%1").arg(pefile->getOptionalHeader_FileAlignment(),8,16,QChar('0')));
    lineEditMajorOperatingSystemVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MajorOperatingSystemVersion(),4,16,QChar('0')));
    lineEditMinorOperatingSystemVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MinorOperatingSystemVersion(),4,16,QChar('0')));
    lineEditMajorImageVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MajorImageVersion(),4,16,QChar('0')));
    lineEditMinorImageVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MinorImageVersion(),4,16,QChar('0')));
    lineEditMajorSubsystemVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MajorSubsystemVersion(),4,16,QChar('0')));
    lineEditMinorSubsystemVersion->setText(QString("%1").arg(pefile->getOptionalHeader_MinorSubsystemVersion(),4,16,QChar('0')));
    lineEditWin32VersionValue->setText(QString("%1").arg(pefile->getOptionalHeader_Win32VersionValue(),8,16,QChar('0')));
    lineEditSizeOfImage->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfImage(),8,16,QChar('0')));
    lineEditSizeOfHeaders->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfHeaders(),8,16,QChar('0')));
    lineEditCheckSum->setText(QString("%1").arg(pefile->getOptionalHeader_CheckSum(),8,16,QChar('0')));
    lineEditSubsystem->setText(QString("%1").arg(pefile->getOptionalHeader_Subsystem(),4,16,QChar('0')));
    lineEditDllCharacteristics->setText(QString("%1").arg(pefile->getOptionalHeader_DllCharacteristics(),4,16,QChar('0')));

    if(pefile->isPEPlus())
    {
        lineEditSizeOfStackReserve->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditSizeOfStackReserve->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfStackReserve64(),16,16,QChar('0')));
    }
    else
    {
        lineEditSizeOfStackReserve->setInputMask("HHHHHHHH");
        lineEditSizeOfStackReserve->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfStackReserve(),8,16,QChar('0')));
    }

    if(pefile->isPEPlus())
    {
        lineEditSizeOfStackCommit->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditSizeOfStackCommit->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfStackCommit64(),16,16,QChar('0')));
    }
    else
    {
        lineEditSizeOfStackCommit->setInputMask("HHHHHHHH");
        lineEditSizeOfStackCommit->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfStackCommit(),8,16,QChar('0')));
    }

    if(pefile->isPEPlus())
    {
        lineEditSizeOfHeapReserve->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditSizeOfHeapReserve->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfHeapReserve64(),16,16,QChar('0')));
    }
    else
    {
        lineEditSizeOfHeapReserve->setInputMask("HHHHHHHH");
        lineEditSizeOfHeapReserve->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfHeapReserve(),8,16,QChar('0')));
    }

    if(pefile->isPEPlus())
    {
        lineEditSizeOfHeapCommit->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditSizeOfHeapCommit->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfHeapCommit64(),16,16,QChar('0')));
    }
    else
    {
        lineEditSizeOfHeapCommit->setInputMask("HHHHHHHH");
        lineEditSizeOfHeapCommit->setText(QString("%1").arg(pefile->getOptionalHeader_SizeOfHeapCommit(),8,16,QChar('0')));
    }

    lineEditLoaderFlags->setText(QString("%1").arg(pefile->getOptionalHeader_LoaderFlags(),8,16,QChar('0')));
    lineEditNumberOfRvaAndSizes->setText(QString("%1").arg(pefile->getOptionalHeader_NumberOfRvaAndSizes(),8,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPENtHeaders::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    ui->lineEditSignature->setReadOnly(ui->checkBoxReadOnly->checkState());

    lineEditMachine->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditNumberOfSections->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditTimeDateStamp->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditPointerToSymbolTable->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditNumberOfSymbols->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfOptionalHeader->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditCharacteristics->setReadOnly(ui->checkBoxReadOnly->checkState());

    lineEditMagic->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMajorLinkerVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMinorLinkerVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfCode->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfInitializedData->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfUninitializedData->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditAddressOfEntryPoint->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditBaseOfCode->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditBaseOfData->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditImageBase->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSectionAlignment->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditFileAlignment->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMajorOperatingSystemVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMinorOperatingSystemVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMajorImageVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMinorImageVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMajorSubsystemVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditMinorSubsystemVersion->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditWin32VersionValue->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfImage->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfHeaders->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditCheckSum->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSubsystem->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditDllCharacteristics->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfStackReserve->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfStackCommit->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfHeapReserve->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditSizeOfHeapCommit->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditLoaderFlags->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEditNumberOfRvaAndSizes->setReadOnly(ui->checkBoxReadOnly->checkState());
}

void DialogPENtHeaders::on_pushButtonHex_clicked()
{
    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("NT Headers"));

    dialoghde.exec();
}

void DialogPENtHeaders::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogPENtHeaders::on_lineEditSignature_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

    ui->pushButtonApply->setEnabled(true);
}

void DialogPENtHeaders::pushButtonMachineExtra_clicked()
{
    DialogPEMachine dialogpemachine(this);

    dialogpemachine.setData(pOptions,pefile);

    dialogpemachine.exec();
}

void DialogPENtHeaders::pushButtonNumberOfSectionsExtra_clicked()
{
    DialogPESections dialogpesections(this);

    dialogpesections.setData(pOptions,pefile);

    dialogpesections.exec();
}

void DialogPENtHeaders::pushButtonTimeDateStampExtra_clicked()
{
    DialogPETimeDateStamp dialogpetimedatestamp(this);

    dialogpetimedatestamp.setData(pOptions,pefile);

    dialogpetimedatestamp.exec();
}

void DialogPENtHeaders::pushButtonCharacteristicsExtra_clicked()
{
    DialogPECharacteristics dialogpecharacteristics(this);

    dialogpecharacteristics.setData(pOptions,pefile);

    dialogpecharacteristics.exec();
}

void DialogPENtHeaders::pushButtonMagicExtra_clicked()
{
    DialogPEMagic dialogpemagic(this);

    dialogpemagic.setData(pOptions,pefile);

    dialogpemagic.exec();
}

void DialogPENtHeaders::pushButtonAddressOfEntryPointExtra_clicked()
{
    DialogHDE dialoghde(this);

    //    dialoghde.setData(pefile->getFileName(),pefile->RVAToOffset(pefile->getOptionalHeader_AddressOfEntryPoint()),0,pefile->getDisasmMode(),pOptions);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"x86-ASM",pefile->getDisasmMode(),"MASM",pefile->RVAToOffset(pefile->getOptionalHeader_AddressOfEntryPoint()),1,tr("EntryPoint"));

    dialoghde.exec();
}

void DialogPENtHeaders::pushButtonSizeOfHeadersExtra_clicked()
{
    DialogHDE dialoghde(this);

    //    dialoghde.setData(pefile->getFileName(),0,pefile->getOptionalHeader_SizeOfHeaders(),pefile->getDisasmMode(),pOptions);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",0,pefile->getOptionalHeader_SizeOfHeaders(),tr("Headers"));

    dialoghde.exec();
}

void DialogPENtHeaders::pushButtonCheckSumExtra_clicked()
{
    DialogPECheckSum dialogpechecksum(this);

    dialogpechecksum.setData(pOptions,pefile);

    dialogpechecksum.exec();
}

void DialogPENtHeaders::pushButtonSubsystemExtra_clicked()
{
    DialogPESubsystem dialogpesubsystem(this);

    dialogpesubsystem.setData(pOptions,pefile);

    dialogpesubsystem.exec();
}

void DialogPENtHeaders::pushButtonDllCharacteristicsExtra_clicked()
{
    DialogPEDllCharacteristics dialogpedllcharacteristics(this);

    dialogpedllcharacteristics.setData(pOptions,pefile);

    dialogpedllcharacteristics.exec();
}
