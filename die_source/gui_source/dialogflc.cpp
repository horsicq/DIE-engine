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
#include "dialogflc.h"
#include "ui_dialogflc.h"

DialogFLC::DialogFLC(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogFLC)
{
    ui->setupUi(this);

    ui->pushButtonOK->setShortcut(QKeySequence("Quit"));

    bConvert=false;
}

DialogFLC::~DialogFLC()
{
    delete ui;
}

void DialogFLC::setData(__DIE_OPTIONS *pOptions, Binary *_binary)
{
    setOptions(pOptions);
    this->_binary=_binary;

    ui->radioButtonVirtualAddress->setChecked(true);
    ui->checkBoxAddressAsHex->setChecked(true);

    //    ui->lineEditVirtualAddress->setText("0");
    ui->lineEditFileOffset->setText("0");

    sType=_binary->getType();

    if((sType=="ELF")||(sType=="ELF64"))
    {
        elffile.setFileName(_binary->getFileName());
        ui->lineEditBaseAddress->setEnabled(false);
        ui->radioButtonRVA->setEnabled(false);
        ui->lineEditRVA->setEnabled(false);

        ui->lineEditVirtualAddress->setText(valueToString(elffile.OffsetToVA(0)));
    }
    else if((sType=="PE")||(sType=="PE+(64)"))
    {
        pefile.setFileName(_binary->getFileName());
        ui->lineEditBaseAddress->setEnabled(false);

        ui->lineEditVirtualAddress->setText(valueToString(pefile.OffsetToVA(0)));

        //        ui->lineEditRVA->setText("0");
    }
    else if((sType=="MACH")||(sType=="MACH64"))
    {
        machfile.setFileName(_binary->getFileName());
        ui->lineEditBaseAddress->setEnabled(false);
        ui->radioButtonRVA->setEnabled(false);
        ui->lineEditRVA->setEnabled(false);

        ui->lineEditVirtualAddress->setText(valueToString(machfile.OffsetToVA(0)));
    }
    else if(sType=="MSDOS")
    {
        msdosfile.setFileName(_binary->getFileName());
        ui->lineEditBaseAddress->setEnabled(false);
        ui->radioButtonRVA->setEnabled(false);
        ui->lineEditRVA->setEnabled(false);

        ui->lineEditVirtualAddress->setText(valueToString(msdosfile.OffsetToVA(0)));
    }
    else if((sType=="Binary")||(sType=="Text"))
    {
        ui->lineEditBaseAddress->setEnabled(true);
        ui->radioButtonRVA->setEnabled(false);
        ui->lineEditRVA->setEnabled(false);
    }

    if(sType=="PE")
    {
        ui->lineEditBaseAddress->setText(valueToString(pefile.getOptionalHeader_ImageBase()));
    }
    else if(sType=="PE+(64)")
    {
        ui->lineEditBaseAddress->setText(valueToString(pefile.getOptionalHeader_ImageBase64()));
    }
    else if((sType=="Binary")||(sType=="Text"))
    {
        ui->lineEditBaseAddress->setText("0");
    }

    ui->radioButtonFileOffset->setChecked(true);
}

void DialogFLC::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogFLC::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->checkBoxAddressAsHex->setToolTip(tr("Values as HEX"));
        ui->lineEditBaseAddress->setToolTip(tr("Base address"));
        ui->lineEditFileOffset->setToolTip(tr("File offset"));
        ui->lineEditRVA->setToolTip(tr("Relative virtual address"));
        ui->lineEditVirtualAddress->setToolTip(tr("Virtual address"));
        ui->pushButtonFileOffset->setToolTip(tr("File offset"));
    }

    return true;
}

void DialogFLC::on_pushButtonFileOffset_clicked()
{
    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,_binary->getFileName(),0,0,"HEX","","",stringToValue(ui->lineEditFileOffset->text(),ui->checkBoxAddressAsHex->isChecked()?16:10),1,tr("Offset"));

    dialoghde.exec();
}

QString DialogFLC::valueToString(unsigned long long nValue)
{
    QString sResult;

    sResult=QString("%1").arg(nValue,0,ui->checkBoxAddressAsHex->isChecked()?16:10,QChar('0'));

    return sResult;
}

unsigned long long DialogFLC::stringToValue(QString sValue,int nBits)
{
    unsigned long long nResult;

    nResult=sValue.toULongLong(0,nBits);

    return nResult;
}

void DialogFLC::on_radioButtonFileOffset_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEditFileOffset->setReadOnly(false);
        ui->lineEditVirtualAddress->setReadOnly(true);
        ui->lineEditRVA->setReadOnly(true);

        ui->lineEditFileOffset->setFocus();
        ui->lineEditFileOffset->setText("0");
    }
}

void DialogFLC::on_radioButtonVirtualAddress_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEditFileOffset->setReadOnly(true);
        ui->lineEditVirtualAddress->setReadOnly(false);
        ui->lineEditRVA->setReadOnly(true);

        ui->lineEditVirtualAddress->setFocus();
        ui->lineEditVirtualAddress->setText("0");
    }
}

void DialogFLC::on_radioButtonRVA_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEditFileOffset->setReadOnly(true);
        ui->lineEditVirtualAddress->setReadOnly(true);
        ui->lineEditRVA->setReadOnly(false);

        ui->lineEditRVA->setFocus();
        ui->lineEditRVA->setText("0");
    }
}

void DialogFLC::on_lineEditBaseAddress_textChanged(const QString &arg1)
{
    if(bConvert)
    {
        return;
    }

    if(!ui->lineEditBaseAddress->isReadOnly())
    {
        unsigned long long nBaseAddress=stringToValue(ui->lineEditBaseAddress->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);
        unsigned long long nRVA=stringToValue(ui->lineEditRVA->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);
        ui->lineEditVirtualAddress->setText(valueToString(nBaseAddress+nRVA));
    }
}

void DialogFLC::on_lineEditFileOffset_textChanged(const QString &arg1)
{
    if(bConvert)
    {
        return;
    }

    bool bIsValid=false;

    if(!ui->lineEditFileOffset->isReadOnly())
    {
        unsigned long long nFileOffset=stringToValue(ui->lineEditFileOffset->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);
        unsigned long long nVA=0;
        unsigned long long nRVA=0;

        if(_binary->isOffsetValid(nFileOffset))
        {
            if((sType=="ELF")||(sType=="ELF64"))
            {
                nVA=elffile.OffsetToVA(nFileOffset);
            }
            else if((sType=="PE")||(sType=="PE+(64)"))
            {
                nVA=pefile.OffsetToVA(nFileOffset);
                nRVA=pefile.OffsetToRVA(nFileOffset);
            }
            else if((sType=="MACH")||(sType=="MACH64"))
            {
                nVA=machfile.OffsetToVA(nFileOffset);
            }
            else if(sType=="MSDOS")
            {
                nVA=msdosfile.OffsetToVA(nFileOffset);
            }
            else if((sType=="Binary")||(sType=="Text"))
            {
                nRVA=nFileOffset;
            }

            bIsValid=((long long)nVA!=-1);
        }

        if(bIsValid)
        {
            if((sType=="PE")||(sType=="PE+(64)"))
            {
                ui->lineEditRVA->setText(valueToString(nRVA));
            }

            //            unsigned long long nBaseAddress=stringToValue(ui->lineEditBaseAddress->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);
            ui->lineEditVirtualAddress->setText(valueToString(nVA));
        }
        else
        {
            if((sType=="PE")||(sType=="PE+(64)"))
            {
                ui->lineEditRVA->setText(QString("[%1]").arg(tr("Invalid")));
            }

            ui->lineEditVirtualAddress->setText(QString("[%1]").arg(tr("Invalid")));
        }
    }

    if(!bIsValid)
    {
        unsigned long long _nFileOffset=stringToValue(ui->lineEditFileOffset->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);

        ui->pushButtonFileOffset->setEnabled(_binary->isOffsetValid(_nFileOffset));
    }
}

void DialogFLC::on_lineEditVirtualAddress_textChanged(const QString &arg1)
{
    if(bConvert)
    {
        return;
    }

    if(!ui->lineEditVirtualAddress->isReadOnly())
    {
        unsigned long long nVirtualAddress=stringToValue(ui->lineEditVirtualAddress->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);
        unsigned long long nBaseAddress=stringToValue(ui->lineEditBaseAddress->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);

        unsigned long long nFileOffset=0;
        unsigned long long nRVA=0;
        bool bIsFileOffsetValid=false;
        bool bIsRVAValid=false;

        if(nVirtualAddress>=nBaseAddress)
        {
            if((sType=="ELF")||(sType=="ELF64"))
            {
                nFileOffset=elffile.VAToOffset(nVirtualAddress);
            }
            else if((sType=="PE")||(sType=="PE+(64)"))
            {
                nFileOffset=pefile.VAToOffset(nVirtualAddress);
                nRVA=pefile.VAToRVA(nVirtualAddress);
                bIsRVAValid=pefile.isRVAValid(nRVA);
            }
            else if((sType=="MACH")||(sType=="MACH64"))
            {
                nFileOffset=machfile.VAToOffset(nVirtualAddress);
            }
            else if(sType=="MSDOS")
            {
                nFileOffset=msdosfile.VAToOffset(nVirtualAddress);
            }
            else if((sType=="Binary")||(sType=="Text"))
            {
                nFileOffset=nVirtualAddress-nBaseAddress;
            }

            bIsFileOffsetValid=_binary->isOffsetValid(nFileOffset);
        }

        if(bIsFileOffsetValid)
        {
            if((sType=="PE")||(sType=="PE+(64)"))
            {
                ui->lineEditRVA->setText(valueToString(nFileOffset));
            }

            ui->lineEditFileOffset->setText(valueToString(nFileOffset));
        }
        else
        {
            if((sType=="PE")||(sType=="PE+(64)"))
            {
                ui->lineEditRVA->setText(QString("[%1]").arg(tr("Invalid")));
            }

            ui->lineEditFileOffset->setText(QString("[%1]").arg(tr("Invalid")));
        }

        if((sType=="PE")||(sType=="PE+(64)"))
        {
            if(bIsRVAValid)
            {
                ui->lineEditRVA->setText(valueToString(nRVA));
            }
            else
            {
                ui->lineEditRVA->setText(QString("[%1]").arg(tr("Invalid")));
            }
        }
    }

    if(ui->lineEditFileOffset->text().contains("["))
    {
        ui->pushButtonFileOffset->setEnabled(false);
    }
}

void DialogFLC::on_lineEditRVA_textChanged(const QString &arg1)
{
    if(bConvert)
    {
        return;
    }

    if(!ui->lineEditRVA->isReadOnly())
    {
        unsigned long long nRVA=stringToValue(ui->lineEditRVA->text(),ui->checkBoxAddressAsHex->isChecked()?16:10);

        unsigned long long nFileOffset=0;
        unsigned long long nVA=0;
        bool bIsOffsetValid=false;
        bool bIsRVAValid=false;

        if((sType=="PE")||(sType=="PE+(64)"))
        {
            nFileOffset=pefile.RVAToOffset(nRVA);
            nVA=pefile.RVAToVA(nRVA);
        }

        bIsOffsetValid=_binary->isOffsetValid(nFileOffset);
        bIsRVAValid=pefile.isRVAValid(nRVA);

        if(bIsOffsetValid)
        {
            ui->lineEditFileOffset->setText(valueToString(nFileOffset));
        }
        else
        {
            ui->lineEditFileOffset->setText(QString("[%1]").arg(tr("Invalid")));

        }

        if(bIsRVAValid)
        {
            ui->lineEditVirtualAddress->setText(valueToString(nVA));
        }
        else
        {
            ui->lineEditVirtualAddress->setText(QString("[%1]").arg(tr("Invalid")));
        }
    }

    if(ui->lineEditFileOffset->text().contains("["))
    {
        ui->pushButtonFileOffset->setEnabled(false);
    }
}

void DialogFLC::on_checkBoxAddressAsHex_toggled(bool checked)
{
    bConvert=true;
    unsigned long long nValue=0;

    if((sType=="PE")||(sType=="PE+(64)")||(sType=="Binary")||(sType=="Text"))
    {
        nValue=stringToValue(ui->lineEditBaseAddress->text(),checked?10:16);
        ui->lineEditBaseAddress->setText(valueToString(nValue));
    }

    if(!(ui->lineEditFileOffset->text().contains("[")))
    {
        nValue=stringToValue(ui->lineEditFileOffset->text(),checked?10:16);
        ui->lineEditFileOffset->setText(valueToString(nValue));
    }

    if((sType=="PE")||(sType=="PE+(64)"))
    {
        if(!(ui->lineEditRVA->text().contains("[")))
        {
            nValue=stringToValue(ui->lineEditRVA->text(),checked?10:16);
            ui->lineEditRVA->setText(valueToString(nValue));
        }
    }

    if(!(ui->lineEditVirtualAddress->text().contains("[")))
    {
        nValue=stringToValue(ui->lineEditVirtualAddress->text(),checked?10:16);
        ui->lineEditVirtualAddress->setText(valueToString(nValue));
    }

    ui->lineEditBaseAddress->setHex(checked);
    ui->lineEditBaseAddress->setDec(!checked);
    ui->lineEditFileOffset->setHex(checked);
    ui->lineEditFileOffset->setDec(!checked);
    ui->lineEditRVA->setHex(checked);
    ui->lineEditRVA->setDec(!checked);
    ui->lineEditVirtualAddress->setHex(checked);
    ui->lineEditVirtualAddress->setDec(!checked);

    if(checked)
    {
        ui->lineEditBaseAddress->setValidator(new QRegExpValidator(QRegExp("^[0-9a-fA-F]{0,16}$")));
        ui->lineEditFileOffset->setValidator(new QRegExpValidator(QRegExp("^[0-9a-fA-F]{0,16}$")));
        ui->lineEditRVA->setValidator(new QRegExpValidator(QRegExp("^[0-9a-fA-F]{0,16}$")));
        ui->lineEditVirtualAddress->setValidator(new QRegExpValidator(QRegExp("^[0-9a-fA-F]{0,16}$")));
    }
    else
    {
        ui->lineEditBaseAddress->setValidator(new QRegExpValidator(QRegExp("^[0-9]{0,16}$")));
        ui->lineEditFileOffset->setValidator(new QRegExpValidator(QRegExp("^[0-9]{0,16}$")));
        ui->lineEditRVA->setValidator(new QRegExpValidator(QRegExp("^[0-9]{0,16}$")));
        ui->lineEditVirtualAddress->setValidator(new QRegExpValidator(QRegExp("^[0-9]{0,16}$")));
    }

    bConvert=false;
}
