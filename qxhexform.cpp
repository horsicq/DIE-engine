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
#include "qxhexform.h"
#include "ui_qxhexform.h"

QXHexForm::QXHexForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QXHexForm)
{
    ui->setupUi(this);

    //    sysCodec=QTextCodec::codecForCStrings();
    //    listCodecs=QTextCodec::availableCodecs();

    ui->comboBoxType->addItem("HEX");
    ui->comboBoxType->addItem("x86-ASM");

    connect(ui->scrollAreaHEX,SIGNAL(selectionStartChanged(unsigned long long)),this,SLOT(setSelectionStart(unsigned long long)));
    connect(ui->scrollAreaHEX,SIGNAL(selectionSizeChanged(unsigned long long)),this,SLOT(setSelectionSize(unsigned long long)));
    connect(ui->scrollAreaHEX,SIGNAL(cursorChanged(unsigned long long)),this,SLOT(setCursor(unsigned long long)));
    connect(ui->scrollAreaHEX,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(ui->scrollAreaHEX,SIGNAL(closeDialog()),this,SIGNAL(closeDialog()));
    connect(ui->scrollAreaHEX,SIGNAL(setReadOnly(bool)),this,SLOT(setReadOnly(bool)));

    //    connect(ui->scrollAreaHEX,SIGNAL(_reloadMemoryMap(QList<__MEMORYMAP>*)),this,SLOT(reloadMemoryMap(QList<__MEMORYMAP>*)));
    //    ui->scrollAreaHEX->setFocus();

    ui->scrollAreaHEX->setReadOnlyMode(true);

    ui->pushButtonExtra->setEnabled(false);
    setHEXfocus();
}

QXHexForm::~QXHexForm()
{
    //    QTextCodec::setCodecForCStrings(sysCodec);
    delete ui;
}

void QXHexForm::setData(QByteArray(*__ReadMemory)(QVariant, unsigned long long, unsigned long long,bool *),bool (*__WriteMemory)(QVariant,unsigned long long,QByteArray), QVariant parameter, unsigned long long nStartAddress, unsigned long long nMaxSize)
{
    QList<QString> listImages=ui->scrollAreaHEX->setData(__ReadMemory,__WriteMemory,parameter,nStartAddress,nMaxSize);

    ui->comboBoxImage->clear();
    ui->comboBoxImage->addItems(listImages);

    setHEXfocus();
}

void QXHexForm::setType(QString sType, QString sMode, QString sSyntax)
{
    this->sType=sType;
    this->sMode=sMode;
    this->sSyntax=sSyntax;

    setComboBoxType(sType);
    setComboBoxMode(sMode);
    setComboBoxSyntax(sSyntax);
}

void QXHexForm::setAddress(unsigned long long nAddress)
{
    //    if(sizeof(void *)==4)
    //    {
    //        nAddress=nAddress&0xFFFFFFFF;
    //    }

    ui->scrollAreaHEX->goToAddress(nAddress);
}

void QXHexForm::setSelection(unsigned long long nSelectionStart, unsigned long long nSelectionSize)
{
    //    if(sizeof(void *)==4)
    //    {
    //        nSelectionStart=nSelectionStart&0xFFFFFFFF;
    //    }
    this->nSelectionStart=nSelectionStart;
    this->nSelectionSize=nSelectionSize;
    ui->scrollAreaHEX->setSelection(nSelectionStart,nSelectionSize);
}

void QXHexForm::goToAddress(unsigned long long nAddress)
{
    ui->scrollAreaHEX->goToAddress(nAddress);
}

void QXHexForm::setName(QString sName)
{
    ui->labelName->setText(sName);

    ui->pushButtonExtra->setEnabled(sName!="");
}

void QXHexForm::setImage(QString sImage)
{
    ui->scrollAreaHEX->setImage(sImage);

    for(int i=0; i<ui->comboBoxImage->count(); i++)
    {
        if(ui->comboBoxImage->itemText(i)==sImage)
        {
            ui->comboBoxImage->setCurrentIndex(i);
            break;
        }
    }
}

void QXHexForm::_setFont(QFont font)
{
    ui->scrollAreaHEX->_setFont(font);
}

void QXHexForm::setAddressWidth(unsigned int nAddressWidth)
{
    ui->scrollAreaHEX->setAddressWidth(nAddressWidth);
}

void QXHexForm::_showToolTips(bool bState)
{
    if(bState)
    {
        ui->lineEditCursor->setToolTip(tr("Address of cursor"));
        ui->lineEditStart->setToolTip(tr("Address of selection"));
        ui->checkBoxAddressAsHex->setToolTip(tr("Set/unset show address as HEX"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditSize->setToolTip(tr("Size of selection in bytes"));
        ui->comboBoxImage->setToolTip(tr("Image"));
        ui->comboBoxMode->setToolTip(tr("Mode"));
        ui->comboBoxSyntax->setToolTip(tr("Syntax"));
        ui->comboBoxType->setToolTip(tr("Type"));
        ui->pushButtonExtra->setToolTip(tr("Restore"));
        ui->pushButtonReload->setToolTip(tr("Reload"));
    }
}

void QXHexForm::setLittleEndian(bool bState)
{
    ui->scrollAreaHEX->setLittleEndian(bState);
}

void QXHexForm::setVorBuffer(bool bState)
{
    ui->scrollAreaHEX->setVorBuffer(bState);
}

void QXHexForm::setCursor(unsigned long long nValue)
{
    QString sValue;

    if(ui->checkBoxAddressAsHex->isChecked())
    {
        sValue=QString("%1").arg(nValue,0,16);
    }
    else
    {
        sValue=QString("%1").arg(nValue,0,10);
    }

    ui->lineEditCursor->setText(sValue);
}

void QXHexForm::on_checkBoxAddressAsHex_stateChanged(int arg1)
{
    bool bIsChecked=ui->checkBoxAddressAsHex->isChecked();
    ui->scrollAreaHEX->setAddressAsHEX(bIsChecked);

    ui->lineEditCursor->setHex(bIsChecked);
    ui->lineEditCursor->setDec(!bIsChecked);

    //    handleMemoryMap(bIsChecked);

    setHEXfocus();
}

void QXHexForm::setSelectionStart(unsigned long long nValue)
{
    QString sValue;

    if(ui->checkBoxAddressAsHex->isChecked())
    {
        sValue=QString("%1").arg(nValue,0,16);
    }
    else
    {
        sValue=QString("%1").arg(nValue,0,10);
    }

    ui->lineEditStart->setText(sValue);
}

void QXHexForm::setSelectionSize(unsigned long long nValue)
{

    QString sValue;

    if(ui->checkBoxAddressAsHex->isChecked())
    {
        sValue=QString("%1").arg(nValue,0,16);
    }
    else
    {
        sValue=QString("%1").arg(nValue,0,10);
    }

    ui->lineEditSize->setText(sValue);
}

void QXHexForm::on_comboBoxType_currentIndexChanged(const QString &arg1)
{
    ui->comboBoxMode->clear();
    ui->comboBoxSyntax->clear();

    if(arg1=="HEX")
    {

        //        for(int i=0;i<listCodecs.count();i++)
        //        {
        //            ui->comboBoxMode->addItem(listCodecs.at(i));
        //        }

    }
    else if(arg1=="x86-ASM")
    {
        ui->comboBoxMode->addItem("16");
        ui->comboBoxMode->addItem("32");
        ui->comboBoxMode->addItem("64");

        ui->comboBoxSyntax->addItem("MASM");
        ui->comboBoxSyntax->addItem("GOASM");
        ui->comboBoxSyntax->addItem("NASM");
        ui->comboBoxSyntax->addItem("AT");
    }

    if(arg1==sType)
    {
        setComboBoxMode(sMode);
        setComboBoxSyntax(sSyntax);
    }

    ui->scrollAreaHEX->setType(ui->comboBoxType->currentText(),ui->comboBoxMode->currentText(),ui->comboBoxSyntax->currentText());

    setHEXfocus();
}

void QXHexForm::on_comboBoxMode_currentIndexChanged(const QString &arg1)
{
    ui->scrollAreaHEX->setType(ui->comboBoxType->currentText(),ui->comboBoxMode->currentText(),ui->comboBoxSyntax->currentText());
    setHEXfocus();
}

void QXHexForm::on_comboBoxSyntax_currentIndexChanged(const QString &arg1)
{
    ui->scrollAreaHEX->setType(ui->comboBoxType->currentText(),ui->comboBoxMode->currentText(),ui->comboBoxSyntax->currentText());
    setHEXfocus();
}

void QXHexForm::setComboBoxType(QString sType)
{
    for(int i=0; i<ui->comboBoxType->count(); i++)
    {
        if(ui->comboBoxType->itemText(i)==sType)
        {
            ui->comboBoxType->setCurrentIndex(i);
            break;
        }
    }
}

void QXHexForm::setComboBoxMode(QString sMode)
{
    for(int i=0; i<ui->comboBoxMode->count(); i++)
    {
        if(ui->comboBoxMode->itemText(i)==sMode)
        {
            ui->comboBoxMode->setCurrentIndex(i);
            break;
        }
    }
}

void QXHexForm::setComboBoxSyntax(QString sSyntax)
{
    for(int i=0; i<ui->comboBoxSyntax->count(); i++)
    {
        if(ui->comboBoxSyntax->itemText(i)==sSyntax)
        {
            ui->comboBoxSyntax->setCurrentIndex(i);
            break;
        }
    }
}

void QXHexForm::on_pushButtonExtra_clicked()
{
    setSelection(nSelectionStart,nSelectionSize);
    goToAddress(nSelectionStart);

    ui->scrollAreaHEX->__update();

    setHEXfocus();
}

void QXHexForm::on_comboBoxImage_currentIndexChanged(const QString &arg1)
{
    ui->scrollAreaHEX->setImage(ui->comboBoxImage->currentText());

    setHEXfocus();
}

void QXHexForm::setReadOnly(bool bState)
{
    if(bState)
    {
        ui->checkBoxReadOnly->setChecked(bState);
    }

    ui->checkBoxReadOnly->setEnabled(!bState);
}

void QXHexForm::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->scrollAreaHEX->setReadOnlyMode(ui->checkBoxReadOnly->isChecked());

    setHEXfocus();
}

void QXHexForm::setHEXfocus()
{
    ui->scrollAreaHEX->setFocus();
}

void QXHexForm::on_pushButtonReload_clicked()
{
    ui->scrollAreaHEX->reload();
}

//void QXHexForm::reloadMemoryMap(QList<__MEMORYMAP> *pMemoryMap)
//{
//    ui->comboBoxMemoryMap->clear();
//    QString sRecord;
//    QString sItemData;

//    for(int i=0;i<pMemoryMap->count();i++)
//    {

//        sItemData=QString("%1:%2").arg(pMemoryMap->at(i).nAddress).arg(pMemoryMap->at(i).nSize);
//        ui->comboBoxMemoryMap->addItem(sRecord,sItemData);

//    }

//    handleMemoryMap(true);
//}

//void QXHexForm::handleMemoryMap(bool bIsHEX)
//{
//    QString sData;
//    unsigned long long nAddress;
//    unsigned long long nSize;

//    for(int i=0;i<ui->comboBoxMemoryMap->count();i++)
//    {
//        sData=ui->comboBoxMemoryMap->itemData(i).toString();
//        nAddress=sData.section(":",0,0).toULongLong();
//        nSize=sData.section(":",1,1).toULongLong();

//        if(bIsHEX)
//        {
//            ui->comboBoxMemoryMap->setItemText(i,QString("%1 - %2 ( Size: %3 )").arg(nAddress,0,16).arg(nAddress+nSize,0,16).arg(nSize,0,16));
//        }
//        else
//        {
//            ui->comboBoxMemoryMap->setItemText(i,QString("%1 - %2 ( Size: %3 )").arg(nAddress).arg(nAddress+nSize).arg(nSize));
//        }

//    }
//}
