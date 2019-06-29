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
#include "dialogmachuuid.h"
#include "ui_dialogmachuuid.h"

DialogMACHUUID::DialogMACHUUID(QWidget *parent) :
    DialogMACHGeneric(parent),
    ui(new Ui::DialogMACHUUID)
{
    ui->setupUi(this);

    ui->lineEditUUID->setInputMask("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogMACHUUID::~DialogMACHUUID()
{
    delete ui;
}

void DialogMACHUUID::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogMACHUUID::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditUUID->setToolTip(tr("UUID"));
    }

    ui->lineEditUUID->setText(machfile->getUUIDAsString());

    ui->checkBoxReadOnly->setEnabled(!machfile->isReadOnly());

    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogMACHUUID::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->lineEditUUID->setReadOnly(ui->checkBoxReadOnly->checkState());
}

void DialogMACHUUID::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogMACHUUID::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    machfile->setUUIDFromString(ui->lineEditUUID->text());

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogMACHUUID::on_lineEditUUID_textChanged(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}
