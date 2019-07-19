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
#include "dialogpechecksum.h"
#include "ui_dialogpechecksum.h"

DialogPECheckSum::DialogPECheckSum(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPECheckSum)
{
    ui->setupUi(this);

    ui->lineEditCheckSum->setInputMask("HHHHHHHH");

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));

    ui->pushButtonCalculate->setShortcut(QKeySequence(__KeySequence_ctrlF));
}

DialogPECheckSum::~DialogPECheckSum()
{
    delete ui;
}

void DialogPECheckSum::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPECheckSum::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditCheckSum->setToolTip(tr("CheckSum"));
        ui->pushButtonCalculate->setToolTip(tr("Calculate CheckSum"));
    }

    ui->lineEditCheckSum->setText(QString("%1").arg(pefile->getOptionalHeader_CheckSum(),8,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());

    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPECheckSum::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->lineEditCheckSum->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->pushButtonCalculate->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogPECheckSum::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPECheckSum::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setOptionalHeader_CheckSum(ui->lineEditCheckSum->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPECheckSum::on_lineEditCheckSum_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogPECheckSum::on_pushButtonCalculate_clicked()
{
    ui->lineEditCheckSum->setText(QString("%1").arg(pefile->CalculateCheckSum(),8,16,QChar('0')));
}
