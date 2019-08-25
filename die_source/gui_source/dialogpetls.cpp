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
#include "dialogpetls.h"
#include "ui_dialogpetls.h"

DialogPETLS::DialogPETLS(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPETLS)
{
    ui->setupUi(this);

    ui->tableWidgetTLS->setRowCount(6);
    ui->tableWidgetTLS->setColumnCount(2);

    for(int i=0; i<6; i++)
    {
        ui->tableWidgetTLS->setRowHeight(i,20);
    }

    QTableWidgetItem *newItem;

    lineEditStartAddressOfRawData=new QLineEditX;
    lineEditEndAddressOfRawData=new QLineEditX;
    lineEditAddressOfIndex=new QLineEditX;
    lineEditAddressOfCallBacks=new QLineEditX;
    lineEditSizeOfZeroFill=new QLineEditX;
    lineEditCharacteristics=new QLineEditX;

    lineEditStartAddressOfRawData->setAlignment(Qt::AlignCenter);
    lineEditEndAddressOfRawData->setAlignment(Qt::AlignCenter);
    lineEditAddressOfIndex->setAlignment(Qt::AlignCenter);
    lineEditAddressOfCallBacks->setAlignment(Qt::AlignCenter);
    lineEditSizeOfZeroFill->setAlignment(Qt::AlignCenter);
    lineEditCharacteristics->setAlignment(Qt::AlignCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("StartAddressOfRawData");
    ui->tableWidgetTLS->setItem(0,0,newItem);
    ui->tableWidgetTLS->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("EndAddressOfRawData");
    ui->tableWidgetTLS->setItem(1,0,newItem);
    ui->tableWidgetTLS->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("AddressOfIndex");
    ui->tableWidgetTLS->setItem(2,0,newItem);
    ui->tableWidgetTLS->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("AddressOfCallBacks");
    ui->tableWidgetTLS->setItem(3,0,newItem);
    ui->tableWidgetTLS->item(3,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("SizeOfZeroFill");
    ui->tableWidgetTLS->setItem(4,0,newItem);
    ui->tableWidgetTLS->item(4,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("Characteristics");
    ui->tableWidgetTLS->setItem(5,0,newItem);
    ui->tableWidgetTLS->item(5,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetTLS->setCellWidget(0,1,lineEditStartAddressOfRawData);
    ui->tableWidgetTLS->setCellWidget(1,1,lineEditEndAddressOfRawData);
    ui->tableWidgetTLS->setCellWidget(2,1,lineEditAddressOfIndex);
    ui->tableWidgetTLS->setCellWidget(3,1,lineEditAddressOfCallBacks);
    ui->tableWidgetTLS->setCellWidget(4,1,lineEditSizeOfZeroFill);
    ui->tableWidgetTLS->setCellWidget(5,1,lineEditCharacteristics);

    connect(lineEditStartAddressOfRawData,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditEndAddressOfRawData,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditAddressOfIndex,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditAddressOfCallBacks,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditSizeOfZeroFill,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    //    pushButtonData=new QPushButtonX;
    //    pushButtonData->setText("...");
    //    pushButtonData->setFixedWidth(30);

    //    ui->tableWidgetTLS->setIndexWidget(ui->tableWidgetTLS->model()->index(0,2),pushButtonData);
    //    connect(pushButtonData,SIGNAL(clicked()),this,SLOT(pushButtonData_clicked()));

    //    pushButtonCallbacks=new QPushButtonX;
    //    pushButtonCallbacks->setText("...");
    //    pushButtonCallbacks->setFixedWidth(30);

    //    ui->tableWidgetTLS->setIndexWidget(ui->tableWidgetTLS->model()->index(3,2),pushButtonCallbacks);
    //    connect(pushButtonCallbacks,SIGNAL(clicked()),this,SLOT(pushButtonCallbacks_clicked()));

    ui->tableWidgetTLS->resizeColumnsToContents();

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPETLS::~DialogPETLS()
{
    delete ui;
}

void DialogPETLS::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPETLS::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
    }

    if(pefile->isPEPlus())
    {
        lineEditStartAddressOfRawData->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditEndAddressOfRawData->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditAddressOfIndex->setInputMask("HHHHHHHHHHHHHHHH");
        lineEditAddressOfCallBacks->setInputMask("HHHHHHHHHHHHHHHH");

        lineEditStartAddressOfRawData->setText(QString("%1").arg(pefile->getTLS_StartAddressOfRawData64(),16,16,QChar('0')));
        lineEditEndAddressOfRawData->setText(QString("%1").arg(pefile->getTLS_EndAddressOfRawData64(),16,16,QChar('0')));
        lineEditAddressOfIndex->setText(QString("%1").arg(pefile->getTLS_AddressOfIndex64(),16,16,QChar('0')));
        lineEditAddressOfCallBacks->setText(QString("%1").arg(pefile->getTLS_AddressOfCallBacks64(),16,16,QChar('0')));
    }
    else
    {
        lineEditStartAddressOfRawData->setInputMask("HHHHHHHH");
        lineEditEndAddressOfRawData->setInputMask("HHHHHHHH");
        lineEditAddressOfIndex->setInputMask("HHHHHHHH");
        lineEditAddressOfCallBacks->setInputMask("HHHHHHHH");

        lineEditStartAddressOfRawData->setText(QString("%1").arg(pefile->getTLS_StartAddressOfRawData(),8,16,QChar('0')));
        lineEditEndAddressOfRawData->setText(QString("%1").arg(pefile->getTLS_EndAddressOfRawData(),8,16,QChar('0')));
        lineEditAddressOfIndex->setText(QString("%1").arg(pefile->getTLS_AddressOfIndex(),8,16,QChar('0')));
        lineEditAddressOfCallBacks->setText(QString("%1").arg(pefile->getTLS_AddressOfCallBacks(),8,16,QChar('0')));
    }

    lineEditSizeOfZeroFill->setInputMask("HHHHHHHH");
    lineEditCharacteristics->setInputMask("HHHHHHHH");

    lineEditSizeOfZeroFill->setText(QString("%1").arg(pefile->getTLS_SizeOfZeroFill(),8,16,QChar('0')));
    lineEditCharacteristics->setText(QString("%1").arg(pefile->getTLS_Characteristics(),8,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPETLS::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    lineEditStartAddressOfRawData->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditEndAddressOfRawData->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditAddressOfIndex->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditAddressOfCallBacks->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditSizeOfZeroFill->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditCharacteristics->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogPETLS::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPETLS::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    if(pefile->isPEPlus())
    {
        pefile->setTLS_StartAddressOfRawData64(lineEditStartAddressOfRawData->text().toULongLong(&bTemp,16));
        pefile->setTLS_EndAddressOfRawData64(lineEditEndAddressOfRawData->text().toULongLong(&bTemp,16));
        pefile->setTLS_AddressOfIndex64(lineEditAddressOfIndex->text().toULongLong(&bTemp,16));
        pefile->setTLS_AddressOfCallBacks64(lineEditAddressOfCallBacks->text().toULongLong(&bTemp,16));

    }
    else
    {
        pefile->setTLS_StartAddressOfRawData(lineEditStartAddressOfRawData->text().toUInt(&bTemp,16));
        pefile->setTLS_EndAddressOfRawData(lineEditEndAddressOfRawData->text().toUInt(&bTemp,16));
        pefile->setTLS_AddressOfIndex(lineEditAddressOfIndex->text().toUInt(&bTemp,16));
        pefile->setTLS_AddressOfCallBacks(lineEditAddressOfCallBacks->text().toUInt(&bTemp,16));
    }

    pefile->setTLS_SizeOfZeroFill(lineEditSizeOfZeroFill->text().toUInt(&bTemp,16));
    pefile->setTLS_Characteristics(lineEditCharacteristics->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPETLS::applyEnable()
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
