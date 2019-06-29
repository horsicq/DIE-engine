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
#include "qxhexsearch.h"
#include "ui_qxhexsearch.h"

QXHexSearch::QXHexSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QXHexSearch)
{
    ui->setupUi(this);

    ui->lineEditValue->setHex(false);
    ui->lineEditValue->setDec(false);
    ui->lineEditValue->setString(true);

    ui->lineEditHEX->setHex(false);
    ui->lineEditHEX->setDec(false);
    ui->lineEditHEX->setString(true);

    ui->lineEditValue->setText("0");

    ui->radioButtonBigEndian->setChecked(true);
    ui->radioButtonUInt->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence("Alt+X"));
    ui->pushButtonOK->setShortcut(QKeySequence("Ctrl+S"));
}

QXHexSearch::~QXHexSearch()
{
    delete ui;
}

void QXHexSearch::_showToolTips(bool bState)
{
    if(bState)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->comboBoxSearchFrom->setToolTip(tr("Search from"));
    }
    else
    {
        ui->pushButtonCancel->setToolTip("");
        ui->pushButtonOK->setToolTip("");
        ui->comboBoxSearchFrom->setToolTip("");
    }
}

void QXHexSearch::setLittleEndian(bool bState)
{
    if(bState)
    {
        ui->radioButtonLittleEndian->setChecked(true);
    }
    else
    {
        ui->radioButtonBigEndian->setChecked(true);
    }
}

void QXHexSearch::setTab(int nTab)
{
    ui->tabWidget->setCurrentIndex(nTab);

    if(nTab==0)
    {
        ui->textEditData->setFocus();
    }
    else if(nTab==1)
    {
        ui->textEditText->setFocus();
    }
    else if(nTab==2)
    {
        ui->lineEditValue->setFocus();
    }
}

void QXHexSearch::toHEX()
{
    QString sValue=ui->lineEditValue->text();

    //    qDebug("double size %d",sizeof(double));
    //    qDebug("float size %d",sizeof(float));

    unsigned long long nValue=0;
    double dValue=0;
    float fValue=0;
    bool bSuccess=false;
    QByteArray baData;
    int nTest=0;
    int nSize=0;

    if(ui->radioButtonChar->isChecked())
    {
        nTest=sValue.toShort(&bSuccess);

        if((nTest>127)||(nTest<-128))
        {
            nValue=0;
            bSuccess=false;
        }
        else
        {
            nValue=nTest;
            nSize=1;
        }
    }
    else if(ui->radioButtonUChar->isChecked())
    {
        nTest=sValue.toUShort(&bSuccess);

        if(nTest>0xFF)
        {
            nValue=0;
            bSuccess=false;
        }
        else
        {
            nValue=nTest;
            nSize=1;
        }
    }
    else if(ui->radioButtonShort->isChecked())
    {
        nValue=sValue.toShort(&bSuccess);

        nSize=2;
    }
    else if(ui->radioButtonUShort->isChecked())
    {
        nValue=sValue.toUShort(&bSuccess);

        nSize=2;
    }
    else if(ui->radioButtonInt->isChecked())
    {
        nValue=sValue.toInt(&bSuccess);

        nSize=4;
    }
    else if(ui->radioButtonUInt->isChecked())
    {
        nValue=sValue.toUInt(&bSuccess);

        nSize=4;
    }
    else if(ui->radioButtonInt64->isChecked())
    {
        nValue=sValue.toLongLong(&bSuccess);

        nSize=8;
    }
    else if(ui->radioButtonUInt64->isChecked())
    {
        nValue=sValue.toULongLong(&bSuccess);

        nSize=8;
    }
    else if(ui->radioButtonFloat->isChecked())
    {
        fValue=sValue.toFloat(&bSuccess);

        nSize=4;
    }
    else if(ui->radioButtonDouble->isChecked())
    {
        dValue=sValue.toDouble(&bSuccess);

        nSize=8;
    }

    if(ui->radioButtonBigEndian->isChecked())
    {
        if(nSize==2)
        {
            qSwap(((unsigned char *)&nValue)[0],((unsigned char *)&nValue)[1]);
        }

        if(nSize==4)
        {
            qSwap(((unsigned char *)&nValue)[0],((unsigned char *)&nValue)[3]);
            qSwap(((unsigned char *)&nValue)[1],((unsigned char *)&nValue)[2]);

            qSwap(((unsigned char *)&fValue)[0],((unsigned char *)&fValue)[3]);
            qSwap(((unsigned char *)&fValue)[1],((unsigned char *)&fValue)[2]);
        }

        if(nSize==8)
        {
            qSwap(((unsigned char *)&nValue)[0],((unsigned char *)&nValue)[7]);
            qSwap(((unsigned char *)&nValue)[1],((unsigned char *)&nValue)[6]);
            qSwap(((unsigned char *)&nValue)[2],((unsigned char *)&nValue)[5]);
            qSwap(((unsigned char *)&nValue)[3],((unsigned char *)&nValue)[4]);

            qSwap(((unsigned char *)&dValue)[0],((unsigned char *)&dValue)[7]);
            qSwap(((unsigned char *)&dValue)[1],((unsigned char *)&dValue)[6]);
            qSwap(((unsigned char *)&dValue)[2],((unsigned char *)&dValue)[5]);
            qSwap(((unsigned char *)&dValue)[3],((unsigned char *)&dValue)[4]);
        }
    }



    if(nSize)
    {
        if(dValue)
        {
            baData.setRawData((char *)&dValue,nSize);
        }
        else if(fValue)
        {
            baData.setRawData((char *)&fValue,nSize);
        }
        else
        {
            baData.setRawData((char *)&nValue,nSize);
        }

    }

    if(!bSuccess)
    {
        ui->lineEditHEX->setText(tr("Invalid"));
    }
    else
    {
        ui->lineEditHEX->setText(baData.toHex().data());
    }


    ui->pushButtonOK->setEnabled(bSuccess);
}

void QXHexSearch::on_lineEditValue_textChanged(const QString &arg1)
{
    toHEX();
}

void QXHexSearch::on_radioButtonBigEndian_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonLittleEndian_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonInt64_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonChar_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonUChar_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonShort_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonUShort_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonInt_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonUInt_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonUInt64_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonDouble_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_radioButtonFloat_toggled(bool checked)
{
    toHEX();
}

void QXHexSearch::on_pushButtonOK_clicked()
{
    QByteArray baBuffer;
    bool bIsCursor=false;
    QString sBuffer;

    if(ui->comboBoxSearchFrom->currentIndex()==1)
    {
        bIsCursor=true;
    }

    if(ui->tabWidget->currentIndex()==0)
    {
        sBuffer=ui->textEditData->toPlainText().toLower().remove(" ");

        if(sBuffer.size()%2)
        {
            sBuffer.append("0");
        }

        if(sBuffer.size()>0x200)
        {
            sBuffer.resize(0x200);
        }

        emit signature(sBuffer,bIsCursor);
    }
    else if(ui->tabWidget->currentIndex()==1)
    {

        sBuffer=ui->textEditText->toPlainText();

        if(sBuffer.size()>0x100)
        {
            sBuffer.resize(0x100);
        }

        if(ui->radioButtonANSI->isChecked())
        {
            baBuffer.append(sBuffer.toLatin1().data());
        }
        else
        {
            baBuffer.append((char *)(sBuffer.utf16()),sBuffer.size()*2);
        }

        baBuffer=baBuffer.toHex();

        sBuffer.clear();

        if(!(ui->checkBoxMatchCase->isChecked()))
        {
            sBuffer.append("#");
        }

        sBuffer.append(baBuffer.data());
        emit signature(sBuffer,bIsCursor);

    }
    else if(ui->tabWidget->currentIndex()==2)
    {
        emit signature(ui->lineEditHEX->text(),bIsCursor);
    }

    this->close();
}

void QXHexSearch::on_pushButtonCancel_clicked()
{
    this->close();
}

void QXHexSearch::on_tabWidget_currentChanged(int index)
{
    ui->pushButtonOK->setEnabled(true);

    if(index==2)
    {
        ui->lineEditValue->setText("0");
    }
}
