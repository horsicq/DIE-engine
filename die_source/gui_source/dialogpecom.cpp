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
#include "dialogpecom.h"
#include "ui_dialogpecom.h"

DialogPECOM::DialogPECOM(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPECOM)
{
    ui->setupUi(this);

    ui->tableWidgetExtra->setRowCount(7);
    ui->tableWidgetExtra->setColumnCount(5);

    QStringList labels;

    labels.append("Name");
    labels.append("Address");
    labels.append("Size");
    labels.append("");
    labels.append("");

    ui->tableWidgetExtra->setHorizontalHeaderLabels(labels);

    lineEditMetaDataAddress=new QLineEditX;
    lineEditResourcesAddress=new QLineEditX;
    lineEditStrongNameSignatureAddress=new QLineEditX;
    lineEditCodeManagerTableAddress=new QLineEditX;
    lineEditVTableFixupsAddress=new QLineEditX;
    lineEditExportAddressTableJumpsAddress=new QLineEditX;
    lineEditManagedNativeHeaderAddress=new QLineEditX;
    lineEditMetaDataSize=new QLineEditX;
    lineEditResourcesSize=new QLineEditX;
    lineEditStrongNameSignatureSize=new QLineEditX;
    lineEditCodeManagerTableSize=new QLineEditX;
    lineEditVTableFixupsSize=new QLineEditX;
    lineEditExportAddressTableJumpsSize=new QLineEditX;
    lineEditManagedNativeHeaderSize=new QLineEditX;

    lineEditMetaDataAddress->setAlignment(Qt::AlignCenter);
    lineEditResourcesAddress->setAlignment(Qt::AlignCenter);
    lineEditStrongNameSignatureAddress->setAlignment(Qt::AlignCenter);
    lineEditCodeManagerTableAddress->setAlignment(Qt::AlignCenter);
    lineEditVTableFixupsAddress->setAlignment(Qt::AlignCenter);
    lineEditExportAddressTableJumpsAddress->setAlignment(Qt::AlignCenter);
    lineEditManagedNativeHeaderAddress->setAlignment(Qt::AlignCenter);
    lineEditMetaDataSize->setAlignment(Qt::AlignCenter);
    lineEditResourcesSize->setAlignment(Qt::AlignCenter);
    lineEditStrongNameSignatureSize->setAlignment(Qt::AlignCenter);
    lineEditCodeManagerTableSize->setAlignment(Qt::AlignCenter);
    lineEditVTableFixupsSize->setAlignment(Qt::AlignCenter);
    lineEditExportAddressTableJumpsSize->setAlignment(Qt::AlignCenter);
    lineEditManagedNativeHeaderSize->setAlignment(Qt::AlignCenter);

    QString sLabels[]= { "MetaData",
                         "Resources",
                         "StrongNameSignature",
                         "CodeManagerTable",
                         "VTableFixups",
                         "ExportAddressTableJumps",
                         "ManagedNativeHeader"
                       };

    QTableWidgetItem *newItem;

    for(int i=0; i<7; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(sLabels[i]);
        ui->tableWidgetExtra->setItem(i,0,newItem);
        ui->tableWidgetExtra->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetExtra->setRowHeight(i,20);
    }

    pushButtonMetaDataExtra=new QPushButton;
    pushButtonMetaDataExtra->setText("...");
    pushButtonMetaDataExtra->setFixedWidth(30);
    pushButtonResourcesExtra=new QPushButton;
    pushButtonResourcesExtra->setText("...");
    pushButtonResourcesExtra->setFixedWidth(30);
    pushButtonStrongNameSignatureExtra=new QPushButton;
    pushButtonStrongNameSignatureExtra->setText("...");
    pushButtonStrongNameSignatureExtra->setFixedWidth(30);
    pushButtonCodeManagerTableExtra=new QPushButton;
    pushButtonCodeManagerTableExtra->setText("...");
    pushButtonCodeManagerTableExtra->setFixedWidth(30);
    pushButtonVTableFixupsExtra=new QPushButton;
    pushButtonVTableFixupsExtra->setText("...");
    pushButtonVTableFixupsExtra->setFixedWidth(30);
    pushButtonExportAddressTableJumpsExtra=new QPushButton;
    pushButtonExportAddressTableJumpsExtra->setText("...");
    pushButtonExportAddressTableJumpsExtra->setFixedWidth(30);
    pushButtonManagedNativeHeaderExtra=new QPushButton;
    pushButtonManagedNativeHeaderExtra->setText("...");
    pushButtonManagedNativeHeaderExtra->setFixedWidth(30);
    pushButtonMetaDataHEX=new QPushButton;
    pushButtonMetaDataHEX->setText(tr("H"));
    pushButtonMetaDataHEX->setFixedWidth(30);
    pushButtonResourcesHEX=new QPushButton;
    pushButtonResourcesHEX->setText(tr("H"));
    pushButtonResourcesHEX->setFixedWidth(30);
    pushButtonStrongNameSignatureHEX=new QPushButton;
    pushButtonStrongNameSignatureHEX->setText(tr("H"));
    pushButtonStrongNameSignatureHEX->setFixedWidth(30);
    pushButtonCodeManagerTableHEX=new QPushButton;
    pushButtonCodeManagerTableHEX->setText(tr("H"));
    pushButtonCodeManagerTableHEX->setFixedWidth(30);
    pushButtonVTableFixupsHEX=new QPushButton;
    pushButtonVTableFixupsHEX->setText(tr("H"));
    pushButtonVTableFixupsHEX->setFixedWidth(30);
    pushButtonExportAddressTableJumpsHEX=new QPushButton;
    pushButtonExportAddressTableJumpsHEX->setText(tr("H"));
    pushButtonExportAddressTableJumpsHEX->setFixedWidth(30);
    pushButtonManagedNativeHeaderHEX=new QPushButton;
    pushButtonManagedNativeHeaderHEX->setText(tr("H"));
    pushButtonManagedNativeHeaderHEX->setFixedWidth(30);

    connect(pushButtonMetaDataExtra,SIGNAL(clicked()),this,SLOT(pushButtonMetaDataExtra_clicked()));
    connect(pushButtonResourcesExtra,SIGNAL(clicked()),this,SLOT(pushButtonResourcesExtra_clicked()));
    connect(pushButtonStrongNameSignatureExtra,SIGNAL(clicked()),this,SLOT(pushButtonStrongNameSignatureExtra_clicked()));
    connect(pushButtonCodeManagerTableExtra,SIGNAL(clicked()),this,SLOT(pushButtonCodeManagerTableExtra_clicked()));
    connect(pushButtonVTableFixupsExtra,SIGNAL(clicked()),this,SLOT(pushButtonVTableFixupsExtra_clicked()));
    connect(pushButtonExportAddressTableJumpsExtra,SIGNAL(clicked()),this,SLOT(pushButtonExportAddressTableJumpsExtra_clicked()));
    connect(pushButtonManagedNativeHeaderExtra,SIGNAL(clicked()),this,SLOT(pushButtonManagedNativeHeaderExtra_clicked()));
    connect(pushButtonMetaDataHEX,SIGNAL(clicked()),this,SLOT(pushButtonMetaDataHEX_clicked()));
    connect(pushButtonResourcesHEX,SIGNAL(clicked()),this,SLOT(pushButtonResourcesHEX_clicked()));
    connect(pushButtonStrongNameSignatureHEX,SIGNAL(clicked()),this,SLOT(pushButtonStrongNameSignatureHEX_clicked()));
    connect(pushButtonCodeManagerTableHEX,SIGNAL(clicked()),this,SLOT(pushButtonCodeManagerTableHEX_clicked()));
    connect(pushButtonVTableFixupsHEX,SIGNAL(clicked()),this,SLOT(pushButtonVTableFixupsHEX_clicked()));
    connect(pushButtonExportAddressTableJumpsHEX,SIGNAL(clicked()),this,SLOT(pushButtonExportAddressTableJumpsHEX_clicked()));
    connect(pushButtonManagedNativeHeaderHEX,SIGNAL(clicked()),this,SLOT(pushButtonManagedNativeHeaderHEX_clicked()));

    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(0,1),lineEditMetaDataAddress);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(1,1),lineEditResourcesAddress);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(2,1),lineEditStrongNameSignatureAddress);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(3,1),lineEditCodeManagerTableAddress);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(4,1),lineEditVTableFixupsAddress);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(5,1),lineEditExportAddressTableJumpsAddress);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(6,1),lineEditManagedNativeHeaderAddress);

    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(0,2),lineEditMetaDataSize);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(1,2),lineEditResourcesSize);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(2,2),lineEditStrongNameSignatureSize);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(3,2),lineEditCodeManagerTableSize);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(4,2),lineEditVTableFixupsSize);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(5,2),lineEditExportAddressTableJumpsSize);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(6,2),lineEditManagedNativeHeaderSize);

    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(0,3),pushButtonMetaDataExtra);
    //    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(1,3),pushButtonResourcesExtra);
    //    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(2,3),pushButtonStrongNameSignatureExtra);
    //    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(3,3),pushButtonCodeManagerTableExtra);
    //    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(4,3),pushButtonVTableFixupsExtra);
    //    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(5,3),pushButtonExportAddressTableJumpsExtra);
    //    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(6,3),pushButtonManagedNativeHeaderExtra);

    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(0,4),pushButtonMetaDataHEX);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(1,4),pushButtonResourcesHEX);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(2,4),pushButtonStrongNameSignatureHEX);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(3,4),pushButtonCodeManagerTableHEX);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(4,4),pushButtonVTableFixupsHEX);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(5,4),pushButtonExportAddressTableJumpsHEX);
    ui->tableWidgetExtra->setIndexWidget(ui->tableWidgetExtra->model()->index(6,4),pushButtonManagedNativeHeaderHEX);

    ui->tableWidgetExtra->resizeColumnToContents(0);
    ui->tableWidgetExtra->resizeColumnToContents(3);
    ui->tableWidgetExtra->resizeColumnToContents(4);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonEntryPointExtra->setShortcut(QKeySequence(__KeySequence_ctrlE));
}

DialogPECOM::~DialogPECOM()
{
    delete ui;
}

void DialogPECOM::on_pushButtonOK_clicked()
{
    this->close();
}
bool DialogPECOM::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        //        ui->textEditCOM->setToolTip(".NET");
        ui->lineEditEntryPoint->setToolTip(tr("EntryPoint"));
        ui->pushButtonEntryPointExtra->setToolTip(tr("EntryPoint"));
        ui->lineEditFlags->setToolTip(tr("Flags"));
        ui->lineEditMajorRuntimeVersion->setToolTip(tr("MajorRuntimeVersion"));
        ui->lineEditMinorRuntimeVersion->setToolTip(tr("MinorRuntimeVersion"));
        ui->checkBox32BITREQUIRED->setToolTip(tr("32 bit required"));
        ui->checkBoxILONLY->setToolTip(tr("IL only"));
        ui->checkBoxIL_LIBRARY->setToolTip(tr("IL library"));
        ui->checkBoxNATIVE_ENTRYPOINT->setToolTip(tr("Native EntryPoint"));
        ui->checkBoxSTRONGNAMESIGNED->setToolTip(tr("Strong name signed"));
        ui->checkBoxTRACKDEBUGDATA->setToolTip(tr("Track debug data"));
    }

    S_IMAGE_DATA_DIRECTORY idd={};

    if(pefile->isNETPresent())
    {
        //        if(pefile->getCLI_Flags()&COMIMAGE_FLAGS_NATIVE_ENTRYPOINT)
        //        {
        //             ui->lineEditEntryPoint->setText(QString("%1").arg(pefile->getCLI_EntryPoint(),8,16,QChar('0')));
        //        }
        //        else
        //        {
        //            ui->lineEditEntryPoint->setString(true);
        //            ui->lineEditEntryPoint->setDec(true);
        //            ui->lineEditEntryPoint->setHex(true);

        //            unsigned int nEntryPoint=pefile->getCLI_EntryPoint();

        //            if((nEntryPoint>>24)==0x06)
        //            {
        //                nEntryPoint=pefile->getCLI_MetaData_Tables_MethodDef_RVA(nEntryPoint&0xFFFFFF);
        //                ui->lineEditEntryPoint->setText(QString("%1").arg(nEntryPoint,8,16,QChar('0')));
        //            }
        //        }
        ui->lineEditEntryPoint->setText(QString("%1").arg(pefile->getCLIEntryPoint(),8,16,QChar('0')));

        ui->lineEditMajorRuntimeVersion->setText(QString("%1").arg(pefile->getCLI_MajorRuntimeVersion(),4,16,QChar('0')));
        ui->lineEditMinorRuntimeVersion->setText(QString("%1").arg(pefile->getCLI_MinorRuntimeVersion(),4,16,QChar('0')));
        ui->lineEditFlags->setText(QString("%1").arg(pefile->getCLI_Flags(),8,16,QChar('0')));

        pefile->getCLI_MetaData(&idd);
        lineEditMetaDataAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
        lineEditMetaDataSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));
        pushButtonMetaDataHEX->setEnabled(idd.VirtualAddress!=0);

        pefile->getCLI_Resources(&idd);
        lineEditResourcesAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
        lineEditResourcesSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));
        pushButtonResourcesHEX->setEnabled(idd.VirtualAddress!=0);

        pefile->getCLI_StrongNameSignature(&idd);
        lineEditStrongNameSignatureAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
        lineEditStrongNameSignatureSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));
        pushButtonStrongNameSignatureHEX->setEnabled(idd.VirtualAddress!=0);

        pefile->getCLI_CodeManagerTable(&idd);
        lineEditCodeManagerTableAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
        lineEditCodeManagerTableSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));
        pushButtonCodeManagerTableHEX->setEnabled(idd.VirtualAddress!=0);

        pefile->getCLI_VTableFixups(&idd);
        lineEditVTableFixupsAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
        lineEditVTableFixupsSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));
        pushButtonVTableFixupsHEX->setEnabled(idd.VirtualAddress!=0);

        pefile->getCLI_ExportAddressTableJumps(&idd);
        lineEditExportAddressTableJumpsAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
        lineEditExportAddressTableJumpsSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));
        pushButtonExportAddressTableJumpsHEX->setEnabled(idd.VirtualAddress!=0);

        pefile->getCLI_ManagedNativeHeader(&idd);
        lineEditManagedNativeHeaderAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
        lineEditManagedNativeHeaderSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));
        pushButtonManagedNativeHeaderHEX->setEnabled(idd.VirtualAddress!=0);
    }

    return true;
}

void DialogPECOM::on_lineEditFlags_textChanged(const QString &arg1)
{
    bool bOK;
    unsigned int nValue=arg1.toUInt(&bOK,16);

    ui->checkBox32BITREQUIRED->setChecked(nValue&COMIMAGE_FLAGS_32BITREQUIRED);
    ui->checkBoxILONLY->setChecked(nValue&COMIMAGE_FLAGS_ILONLY);
    ui->checkBoxIL_LIBRARY->setChecked(nValue&COMIMAGE_FLAGS_IL_LIBRARY);
    ui->checkBoxNATIVE_ENTRYPOINT->setChecked(nValue&0x00000010); // COMIMAGE_FLAGS_NATIVE_ENTRYPOINT
    ui->checkBoxSTRONGNAMESIGNED->setChecked(nValue&COMIMAGE_FLAGS_STRONGNAMESIGNED);
    ui->checkBoxTRACKDEBUGDATA->setChecked(nValue&COMIMAGE_FLAGS_TRACKDEBUGDATA);
}

void DialogPECOM::pushButtonMetaDataExtra_clicked()
{
    DialogPEMetaData dialogpemetadata(this);
    dialogpemetadata.setData(pOptions,pefile);

    dialogpemetadata.exec();
}

void DialogPECOM::pushButtonResourcesExtra_clicked()
{

}

void DialogPECOM::pushButtonStrongNameSignatureExtra_clicked()
{

}

void DialogPECOM::pushButtonCodeManagerTableExtra_clicked()
{

}

void DialogPECOM::pushButtonVTableFixupsExtra_clicked()
{

}

void DialogPECOM::pushButtonExportAddressTableJumpsExtra_clicked()
{

}

void DialogPECOM::pushButtonManagedNativeHeaderExtra_clicked()
{

}

void DialogPECOM::pushButtonMetaDataHEX_clicked()
{
    S_IMAGE_DATA_DIRECTORY idd={};
    pefile->getCLI_MetaData(&idd);

    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("MetaData"));

    dialoghde.exec();
}

void DialogPECOM::pushButtonResourcesHEX_clicked()
{
    S_IMAGE_DATA_DIRECTORY idd={};
    pefile->getCLI_Resources(&idd);

    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Resource"));

    dialoghde.exec();
}

void DialogPECOM::pushButtonStrongNameSignatureHEX_clicked()
{
    S_IMAGE_DATA_DIRECTORY idd={};
    pefile->getCLI_StrongNameSignature(&idd);

    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Strong name signature"));

    dialoghde.exec();
}

void DialogPECOM::pushButtonCodeManagerTableHEX_clicked()
{
    S_IMAGE_DATA_DIRECTORY idd={};
    pefile->getCLI_CodeManagerTable(&idd);

    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Code manager table"));

    dialoghde.exec();
}

void DialogPECOM::pushButtonVTableFixupsHEX_clicked()
{
    S_IMAGE_DATA_DIRECTORY idd={};
    pefile->getCLI_VTableFixups(&idd);

    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("VTable fixups"));

    dialoghde.exec();
}

void DialogPECOM::pushButtonExportAddressTableJumpsHEX_clicked()
{
    S_IMAGE_DATA_DIRECTORY idd={};
    pefile->getCLI_ExportAddressTableJumps(&idd);

    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Export address table jumps"));

    dialoghde.exec();
}

void DialogPECOM::pushButtonManagedNativeHeaderHEX_clicked()
{
    S_IMAGE_DATA_DIRECTORY idd={};
    pefile->getCLI_ManagedNativeHeader(&idd);

    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Managed native header"));

    dialoghde.exec();
}

void DialogPECOM::on_pushButtonEntryPointExtra_clicked()
{
    bool bOK;
    unsigned int nEntryPoint=ui->lineEditEntryPoint->text().toUInt(&bOK,16);

    PEFile _pefile;
    connect(&_pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(_pefile.setFileName(pefile->getFileName()))
    {
        DialogHDE dialoghde(this);

        if(pefile->getCLI_Flags()&0x00000010) // COMIMAGE_FLAGS_NATIVE_ENTRYPOINT
        {
            dialoghde.setData(pOptions,_pefile.getFileName(),0,0,"x86-ASM",_pefile.getDisasmMode(),"MASM",_pefile.RVAToOffset(nEntryPoint),1,tr(".NET EntryPoint"));
        }
        else
        {
            dialoghde.setData(pOptions,_pefile.getFileName(),0,0,"HEX","","",_pefile.RVAToOffset(nEntryPoint),1,tr(".NET EntryPoint"));
        }

        dialoghde.exec();
    }
}
