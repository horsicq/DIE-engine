// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#include "qxhexgoto.h"
#include "ui_qxhexgoto.h"

QXHexGoTo::QXHexGoTo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QXHexGoTo)
{
    ui->setupUi(this);

    ui->radioButtonHex->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence("Alt+X"));
    ui->pushButtonOK->setShortcut(QKeySequence("Ctrl+S"));
}

QXHexGoTo::~QXHexGoTo()
{
    delete ui;
}

//void QXHexGoTo::setData(unsigned long long nStartAddress, unsigned long long nMaxSize,bool bIsHEX)
//{
//    this->nStartAddress=nStartAddress;
//    this->nMaxSize=nMaxSize;

//    if(bIsHEX)
//    {
//        ui->radioButtonHex->setChecked(true);
//    }
//    else
//    {
//        ui->radioButtonDec->setChecked(true);
//    }


//}

void QXHexGoTo::setData(QList<__DATARECORD> *pList, bool bIsHEX)
{
    this->pList=pList;

    if(bIsHEX)
    {
        ui->radioButtonHex->setChecked(true);
    }
    else
    {
        ui->radioButtonDec->setChecked(true);
    }

    ui->lineEditAddress->setFocus();
}

void QXHexGoTo::_showToolTips(bool bState)
{
    if(bState)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->lineEditAddress->setToolTip(tr("Address"));
    }
    else
    {
        ui->pushButtonOK->setToolTip("");
        ui->pushButtonCancel->setToolTip("");
        ui->lineEditAddress->setToolTip("");
    }
}


void QXHexGoTo::on_pushButtonOK_clicked()
{
    unsigned long long nValue=0;
    QString sValue=ui->lineEditAddress->text();
    bool bResult;
    int nPos=0;
    QValidator *validator;

    if(ui->radioButtonHex->isChecked())
    {
        for(int i=0; i<pList->count(); i++)
        {
            validator = new QRegExpValidator(QRegExp("[0-9a-fA-F]{0,16}"), this);

            if(validator->validate(sValue,nPos)==QValidator::Acceptable)
            {
                nValue=sValue.toULongLong(&bResult,16);

                if((nValue>=pList->at(i).nAddress)&&(nValue<=pList->at(i).nAddress+pList->at(i).nSize))
                {
                    emit valueChanged(nValue-pList->at(i).nAddress+pList->at(i).nOffset);
                    this->close();
                }

            }
        }
    }
    else
    {
        for(int i=0; i<pList->count(); i++)
        {
            validator = new QIntValidator(pList->at(i).nAddress, pList->at(i).nAddress+pList->at(i).nSize, this);

            if(validator->validate(sValue,nPos)==QValidator::Acceptable)
            {
                nValue=sValue.toULongLong(&bResult,10);
                emit valueChanged(nValue-pList->at(i).nAddress+pList->at(i).nOffset);
                this->close();
            }
        }
    }

    ui->labelResult->setText(tr("Invalid"));
}

void QXHexGoTo::on_pushButtonCancel_clicked()
{
    this->close();
}


