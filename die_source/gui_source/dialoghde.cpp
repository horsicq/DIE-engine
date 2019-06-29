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
#include "dialoghde.h"
#include "ui_dialoghde.h"

DialogHDE::DialogHDE(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogHDE)
{

    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags()|Qt::WindowMinMaxButtonsHint);

    pHexEdit=new QXHexForm;

    connect(pHexEdit,SIGNAL(closeDialog()),this,SLOT(closeDialog()));

    ui->verticalLayoutHex->addWidget(pHexEdit);
}

void DialogHDE::setData(__DIE_OPTIONS *pOptions,QString sFileName,  unsigned long long nAddress, unsigned long long nSize, QString sType, QString sMode, QString sSyntax, unsigned long long nSelectionStart, unsigned long long nSelectionSize,QString sName)
{

    this->sFileName=sFileName;
    this->pOptions=pOptions;
    this->nAddress=nAddress;
    this->nSize=nSize;
    this->sType=sType;
    this->sMode=sMode;
    this->sSyntax=sSyntax;
    this->nSelectionStart=nSelectionStart;
    this->nSelectionSize=nSelectionSize;
    this->sName=sName;

    setOptions(pOptions);
}

DialogHDE::~DialogHDE()
{
    delete ui;
}


bool DialogHDE::reload()
{
    pHexEdit->setData(&QXHexView::ReadFromFile,&QXHexView::WriteToFile,sFileName,0,0);
    pHexEdit->setType(sType,sMode,sSyntax);
    pHexEdit->setSelection(nSelectionStart,nSelectionSize);
    pHexEdit->goToAddress(nSelectionStart);
    pHexEdit->setName(sName);

    QFont font;

    font.fromString(pOptions->sFontHEX);

    pHexEdit->_setFont(font);
    pHexEdit->_showToolTips(pOptions->bShowTooltips);
    pHexEdit->setAddressWidth(pOptions->nAddressWidth);

    if((nSelectionStart==0)&&(nSelectionSize==0))
    {
        pHexEdit->setImage("Binary");
    }

    pHexEdit->setLittleEndian(pOptions->sEndianness=="Little Endian");


    return true;
}

void DialogHDE::closeDialog()
{
    this->close();
}
