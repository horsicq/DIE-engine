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
#include "dialogpeloadconfig.h"
#include "ui_dialogpeloadconfig.h"

DialogPELoadConfig::DialogPELoadConfig(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPELoadConfig)
{
    ui->setupUi(this);


    ui->tableWidgetLoadConfig->setRowCount(20);
    ui->tableWidgetLoadConfig->setColumnCount(2);

    for(int i=0; i<20; i++)
    {
        ui->tableWidgetLoadConfig->setRowHeight(i,20);
    }

    QTableWidgetItem *newItem;

    //    lineEditSize;
    //    lineEditTimeDateStamp;
    //    lineEditMajorVersion;
    //    lineEditMinorVersion;
    //    lineEditGlobalFlagsClear;
    //    lineEditGlobalFlagsSet;
    //    lineEditCriticalSectionDefaultTimeout;
    //    lineEditDeCommitFreeBlockThreshold;
    //    lineEditDeCommitTotalFreeThreshold;
    //    lineEditLockPrefixTable;
    //    lineEditMaximumAllocationSize;
    //    lineEditVirtualMemoryThreshold;
    //    lineEditProcessHeapFlags;
    //    lineEditProcessAffinityMask;
    //    lineEditCSDVersion;
    //    lineEditReserved1;
    //    lineEditEditList;
    //    lineEditSecurityCookie;
    //    lineEditSEHandlerTable;
    //    lineEditSEHandlerCount;
    lineEditSize=new QLineEditX;
    lineEditTimeDateStamp=new QLineEditX;
    lineEditMajorVersion=new QLineEditX;
    lineEditMinorVersion=new QLineEditX;
    lineEditGlobalFlagsClear=new QLineEditX;
    lineEditGlobalFlagsSet=new QLineEditX;
    lineEditCriticalSectionDefaultTimeout=new QLineEditX;
    lineEditDeCommitFreeBlockThreshold=new QLineEditX;
    lineEditDeCommitTotalFreeThreshold=new QLineEditX;
    lineEditLockPrefixTable=new QLineEditX;
    lineEditMaximumAllocationSize=new QLineEditX;
    lineEditVirtualMemoryThreshold=new QLineEditX;
    lineEditProcessHeapFlags=new QLineEditX;
    lineEditProcessAffinityMask=new QLineEditX;
    lineEditCSDVersion=new QLineEditX;
    lineEditReserved1=new QLineEditX;
    lineEditEditList=new QLineEditX;
    lineEditSecurityCookie=new QLineEditX;
    lineEditSEHandlerTable=new QLineEditX;
    lineEditSEHandlerCount=new QLineEditX;

    lineEditSize->setAlignment(Qt::AlignCenter);
    lineEditTimeDateStamp->setAlignment(Qt::AlignCenter);
    lineEditMajorVersion->setAlignment(Qt::AlignCenter);
    lineEditMinorVersion->setAlignment(Qt::AlignCenter);
    lineEditGlobalFlagsClear->setAlignment(Qt::AlignCenter);
    lineEditGlobalFlagsSet->setAlignment(Qt::AlignCenter);
    lineEditCriticalSectionDefaultTimeout->setAlignment(Qt::AlignCenter);
    lineEditDeCommitFreeBlockThreshold->setAlignment(Qt::AlignCenter);
    lineEditDeCommitTotalFreeThreshold->setAlignment(Qt::AlignCenter);
    lineEditLockPrefixTable->setAlignment(Qt::AlignCenter);
    lineEditMaximumAllocationSize->setAlignment(Qt::AlignCenter);
    lineEditVirtualMemoryThreshold->setAlignment(Qt::AlignCenter);
    lineEditProcessHeapFlags->setAlignment(Qt::AlignCenter);
    lineEditProcessAffinityMask->setAlignment(Qt::AlignCenter);
    lineEditCSDVersion->setAlignment(Qt::AlignCenter);
    lineEditReserved1->setAlignment(Qt::AlignCenter);
    lineEditEditList->setAlignment(Qt::AlignCenter);
    lineEditSecurityCookie->setAlignment(Qt::AlignCenter);
    lineEditSEHandlerTable->setAlignment(Qt::AlignCenter);
    lineEditSEHandlerCount->setAlignment(Qt::AlignCenter);


    QList<QString> listLabels;

    listLabels.append("Size");
    listLabels.append("TimeDateStamp");
    listLabels.append("MajorVersion");
    listLabels.append("MinorVersion");
    listLabels.append("GlobalFlagsClear");
    listLabels.append("GlobalFlagsSet");
    listLabels.append("CriticalSectionDefaultTimeout");
    listLabels.append("DeCommitFreeBlockThreshold");
    listLabels.append("DeCommitTotalFreeThreshold");
    listLabels.append("LockPrefixTable");
    listLabels.append("MaximumAllocationSize");
    listLabels.append("VirtualMemoryThreshold");
    listLabels.append("ProcessHeapFlags");
    listLabels.append("ProcessAffinityMask");
    listLabels.append("CSDVersion");
    listLabels.append("Reserved1");
    listLabels.append("EditList");
    listLabels.append("SecurityCookie");
    listLabels.append("SEHandlerTable");
    listLabels.append("SEHandlerCount");


    for(int i=0; i<listLabels.count(); i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(listLabels.at(i));
        ui->tableWidgetLoadConfig->setItem(i,0,newItem);
        ui->tableWidgetLoadConfig->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }


    ui->tableWidgetLoadConfig->setCellWidget(0,1,lineEditSize);
    ui->tableWidgetLoadConfig->setCellWidget(1,1,lineEditTimeDateStamp);
    ui->tableWidgetLoadConfig->setCellWidget(2,1,lineEditMajorVersion);
    ui->tableWidgetLoadConfig->setCellWidget(3,1,lineEditMinorVersion);
    ui->tableWidgetLoadConfig->setCellWidget(4,1,lineEditGlobalFlagsClear);
    ui->tableWidgetLoadConfig->setCellWidget(5,1,lineEditGlobalFlagsSet);
    ui->tableWidgetLoadConfig->setCellWidget(6,1,lineEditCriticalSectionDefaultTimeout);
    ui->tableWidgetLoadConfig->setCellWidget(7,1,lineEditDeCommitFreeBlockThreshold);
    ui->tableWidgetLoadConfig->setCellWidget(8,1,lineEditDeCommitTotalFreeThreshold);
    ui->tableWidgetLoadConfig->setCellWidget(9,1,lineEditLockPrefixTable);
    ui->tableWidgetLoadConfig->setCellWidget(10,1,lineEditMaximumAllocationSize);
    ui->tableWidgetLoadConfig->setCellWidget(11,1,lineEditVirtualMemoryThreshold);
    ui->tableWidgetLoadConfig->setCellWidget(12,1,lineEditProcessHeapFlags);
    ui->tableWidgetLoadConfig->setCellWidget(13,1,lineEditProcessAffinityMask);
    ui->tableWidgetLoadConfig->setCellWidget(14,1,lineEditCSDVersion);
    ui->tableWidgetLoadConfig->setCellWidget(15,1,lineEditReserved1);
    ui->tableWidgetLoadConfig->setCellWidget(16,1,lineEditEditList);
    ui->tableWidgetLoadConfig->setCellWidget(17,1,lineEditSecurityCookie);
    ui->tableWidgetLoadConfig->setCellWidget(18,1,lineEditSEHandlerTable);
    ui->tableWidgetLoadConfig->setCellWidget(19,1,lineEditSEHandlerCount);


    connect(lineEditSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditTimeDateStamp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMajorVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMinorVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditGlobalFlagsClear,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditGlobalFlagsSet,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditCriticalSectionDefaultTimeout,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditDeCommitFreeBlockThreshold,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditDeCommitTotalFreeThreshold,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditLockPrefixTable,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMaximumAllocationSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditVirtualMemoryThreshold,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditProcessHeapFlags,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditProcessAffinityMask,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditCSDVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditReserved1,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditEditList,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSecurityCookie,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSEHandlerTable,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSEHandlerCount,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));


    ui->tableWidgetLoadConfig->resizeColumnsToContents();

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPELoadConfig::~DialogPELoadConfig()
{
    delete ui;
}

void DialogPELoadConfig::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPELoadConfig::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
    }

    lineEditSize->setInputMask("HHHHHHHH");
    lineEditTimeDateStamp->setInputMask("HHHHHHHH");
    lineEditMajorVersion->setInputMask("HHHH");
    lineEditMinorVersion->setInputMask("HHHH");
    lineEditGlobalFlagsClear->setInputMask("HHHHHHHH");
    lineEditGlobalFlagsSet->setInputMask("HHHHHHHH");
    lineEditCriticalSectionDefaultTimeout->setInputMask("HHHHHHHH");
    lineEditCSDVersion->setInputMask("HHHH");
    lineEditReserved1->setInputMask("HHHH");
    lineEditProcessHeapFlags->setInputMask("HHHHHHHH");

    lineEditSize->setText(QString("%1").arg(pefile->getLoadConfig_Size(),8,16,QChar('0')));
    lineEditTimeDateStamp->setText(QString("%1").arg(pefile->getLoadConfig_TimeDateStamp(),8,16,QChar('0')));
    lineEditMajorVersion->setText(QString("%1").arg(pefile->getLoadConfig_MajorVersion(),4,16,QChar('0')));
    lineEditMinorVersion->setText(QString("%1").arg(pefile->getLoadConfig_MinorVersion(),4,16,QChar('0')));
    lineEditGlobalFlagsClear->setText(QString("%1").arg(pefile->getLoadConfig_GlobalFlagsClear(),8,16,QChar('0')));
    lineEditGlobalFlagsSet->setText(QString("%1").arg(pefile->getLoadConfig_GlobalFlagsSet(),8,16,QChar('0')));
    lineEditCriticalSectionDefaultTimeout->setText(QString("%1").arg(pefile->getLoadConfig_CriticalSectionDefaultTimeout(),8,16,QChar('0')));
    lineEditProcessHeapFlags->setText(QString("%1").arg(pefile->getLoadConfig_ProcessHeapFlags(),8,16,QChar('0')));
    lineEditCSDVersion->setText(QString("%1").arg(pefile->getLoadConfig_CSDVersion(),4,16,QChar('0')));
    lineEditReserved1->setText(QString("%1").arg(pefile->getLoadConfig_Reserved1(),4,16,QChar('0')));


    if(pefile->isPEPlus())
    {
        lineEditDeCommitFreeBlockThreshold->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditDeCommitTotalFreeThreshold->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditLockPrefixTable->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditMaximumAllocationSize->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditVirtualMemoryThreshold->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditProcessAffinityMask->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditEditList->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditSecurityCookie->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditSEHandlerTable->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditSEHandlerCount->setInputMask("HHHHHHHHHHHHHHHH");

        lineEditDeCommitFreeBlockThreshold->setText(QString("%1").arg(pefile->getLoadConfig_DeCommitFreeBlockThreshold64(),16,16,QChar('0')));
        lineEditDeCommitTotalFreeThreshold->setText(QString("%1").arg(pefile->getLoadConfig_DeCommitTotalFreeThreshold64(),16,16,QChar('0')));
        lineEditLockPrefixTable->setText(QString("%1").arg(pefile->getLoadConfig_LockPrefixTable64(),16,16,QChar('0')));
        lineEditMaximumAllocationSize->setText(QString("%1").arg(pefile->getLoadConfig_MaximumAllocationSize64(),16,16,QChar('0')));
        lineEditVirtualMemoryThreshold->setText(QString("%1").arg(pefile->getLoadConfig_VirtualMemoryThreshold64(),16,16,QChar('0')));
        lineEditProcessAffinityMask->setText(QString("%1").arg(pefile->getLoadConfig_ProcessAffinityMask64(),16,16,QChar('0')));
        lineEditEditList->setText(QString("%1").arg(pefile->getLoadConfig_EditList64(),16,16,QChar('0')));
        lineEditSecurityCookie->setText(QString("%1").arg(pefile->getLoadConfig_SecurityCookie64(),16,16,QChar('0')));
        lineEditSEHandlerTable->setText(QString("%1").arg(pefile->getLoadConfig_SEHandlerTable64(),16,16,QChar('0')));
        lineEditSEHandlerCount->setText(QString("%1").arg(pefile->getLoadConfig_SEHandlerCount64(),16,16,QChar('0')));

    }
    else
    {
        lineEditDeCommitFreeBlockThreshold->setInputMask("HHHHHHHH");
        lineEditDeCommitTotalFreeThreshold->setInputMask("HHHHHHHH");
        lineEditLockPrefixTable->setInputMask("HHHHHHHH");
        lineEditMaximumAllocationSize->setInputMask("HHHHHHHH");
        lineEditVirtualMemoryThreshold->setInputMask("HHHHHHHH");
        lineEditProcessAffinityMask->setInputMask("HHHHHHHH");
        lineEditEditList->setInputMask("HHHHHHHH");
        lineEditSecurityCookie->setInputMask("HHHHHHHH");
        lineEditSEHandlerTable->setInputMask("HHHHHHHH");
        lineEditSEHandlerCount->setInputMask("HHHHHHHH");

        lineEditDeCommitFreeBlockThreshold->setText(QString("%1").arg(pefile->getLoadConfig_DeCommitFreeBlockThreshold(),8,16,QChar('0')));
        lineEditDeCommitTotalFreeThreshold->setText(QString("%1").arg(pefile->getLoadConfig_DeCommitTotalFreeThreshold(),8,16,QChar('0')));
        lineEditLockPrefixTable->setText(QString("%1").arg(pefile->getLoadConfig_LockPrefixTable(),8,16,QChar('0')));
        lineEditMaximumAllocationSize->setText(QString("%1").arg(pefile->getLoadConfig_MaximumAllocationSize(),8,16,QChar('0')));
        lineEditVirtualMemoryThreshold->setText(QString("%1").arg(pefile->getLoadConfig_VirtualMemoryThreshold(),8,16,QChar('0')));
        lineEditProcessAffinityMask->setText(QString("%1").arg(pefile->getLoadConfig_ProcessAffinityMask(),8,16,QChar('0')));
        lineEditEditList->setText(QString("%1").arg(pefile->getLoadConfig_EditList(),8,16,QChar('0')));
        lineEditSecurityCookie->setText(QString("%1").arg(pefile->getLoadConfig_SecurityCookie(),8,16,QChar('0')));
        lineEditSEHandlerTable->setText(QString("%1").arg(pefile->getLoadConfig_SEHandlerTable(),8,16,QChar('0')));
        lineEditSEHandlerCount->setText(QString("%1").arg(pefile->getLoadConfig_SEHandlerCount(),8,16,QChar('0')));

    }

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}


void DialogPELoadConfig::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineEditSize->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditTimeDateStamp->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditMajorVersion->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditMinorVersion->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditGlobalFlagsClear->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditGlobalFlagsSet->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditCriticalSectionDefaultTimeout->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditDeCommitFreeBlockThreshold->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditDeCommitTotalFreeThreshold->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditLockPrefixTable->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditMaximumAllocationSize->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditVirtualMemoryThreshold->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditProcessHeapFlags->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditProcessAffinityMask->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditCSDVersion->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditReserved1->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditEditList->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditSecurityCookie->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditSEHandlerTable->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditSEHandlerCount->setReadOnly(ui->checkBoxReadOnly->isChecked());


}

void DialogPELoadConfig::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPELoadConfig::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setLoadConfig_Size(lineEditSize->text().toUInt(&bTemp,16));
    pefile->setLoadConfig_TimeDateStamp(lineEditTimeDateStamp->text().toUInt(&bTemp,16));
    pefile->setLoadConfig_MajorVersion(lineEditMajorVersion->text().toUShort(&bTemp,16));
    pefile->setLoadConfig_MinorVersion(lineEditMinorVersion->text().toUShort(&bTemp,16));
    pefile->setLoadConfig_GlobalFlagsClear(lineEditGlobalFlagsClear->text().toUInt(&bTemp,16));
    pefile->setLoadConfig_GlobalFlagsSet(lineEditGlobalFlagsSet->text().toUInt(&bTemp,16));
    pefile->setLoadConfig_CriticalSectionDefaultTimeout(lineEditCriticalSectionDefaultTimeout->text().toUInt(&bTemp,16));
    pefile->setLoadConfig_ProcessHeapFlags(lineEditProcessHeapFlags->text().toUInt(&bTemp,16));
    pefile->setLoadConfig_CSDVersion(lineEditCSDVersion->text().toUShort(&bTemp,16));
    pefile->setLoadConfig_Reserved1(lineEditReserved1->text().toUShort(&bTemp,16));


    if(pefile->isPEPlus())
    {
        pefile->setLoadConfig_DeCommitFreeBlockThreshold64(lineEditDeCommitFreeBlockThreshold->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_DeCommitTotalFreeThreshold64(lineEditDeCommitTotalFreeThreshold->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_LockPrefixTable64(lineEditLockPrefixTable->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_MaximumAllocationSize64(lineEditMaximumAllocationSize->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_VirtualMemoryThreshold64(lineEditVirtualMemoryThreshold->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_ProcessAffinityMask64(lineEditProcessAffinityMask->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_EditList64(lineEditEditList->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_SecurityCookie64(lineEditSecurityCookie->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_SEHandlerTable64(lineEditSEHandlerTable->text().toULongLong(&bTemp,16));
        pefile->setLoadConfig_SEHandlerCount64(lineEditSEHandlerCount->text().toULongLong(&bTemp,16));

    }
    else
    {
        pefile->setLoadConfig_DeCommitFreeBlockThreshold(lineEditDeCommitFreeBlockThreshold->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_DeCommitTotalFreeThreshold(lineEditDeCommitTotalFreeThreshold->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_LockPrefixTable(lineEditLockPrefixTable->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_MaximumAllocationSize(lineEditMaximumAllocationSize->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_VirtualMemoryThreshold(lineEditVirtualMemoryThreshold->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_ProcessAffinityMask(lineEditProcessAffinityMask->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_EditList(lineEditEditList->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_SecurityCookie(lineEditSecurityCookie->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_SEHandlerTable(lineEditSEHandlerTable->text().toUInt(&bTemp,16));
        pefile->setLoadConfig_SEHandlerCount(lineEditSEHandlerCount->text().toUInt(&bTemp,16));

    }


    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPELoadConfig::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

//void DialogPETLS::pushButtonData_clicked()
//{
//    bool bTemp;
//    DialogHDE dialoghde(this);
//    unsigned int nOffset;

//    if(pefile->isPEPlus())
//    {
//        nOffset=pefile->VAToOffset64(lineEditStartAddressOfRawData->text().toULongLong(&bTemp,16));
//    }
//    else
//    {
//        nOffset=pefile->VAToOffset(lineEditStartAddressOfRawData->text().toUInt(&bTemp,16));
//    }


//    dialoghde.setData(pefile->getFileName(),pOptions,0,0,"HEX","","",nOffset,lineEditEndAddressOfRawData->text().toULongLong(&bTemp,16)-lineEditStartAddressOfRawData->text().toULongLong(&bTemp,16));

//    dialoghde.exec();
//}

//void DialogPETLS::pushButtonCallbacks_clicked()
//{
//    bool bTemp;
//    DialogHDE dialoghde(this);
//    unsigned int nOffset;

//    if(pefile->isPEPlus())
//    {
//        nOffset=pefile->VAToOffset64(lineEditAddressOfCallBacks->text().toULongLong(&bTemp,16));
//    }
//    else
//    {
//        nOffset=pefile->VAToOffset(lineEditAddressOfCallBacks->text().toUInt(&bTemp,16));
//    }


//    dialoghde.setData(pefile->getFileName(),pOptions,0,0,"HEX","","",nOffset,0);

//    dialoghde.exec();
//}
