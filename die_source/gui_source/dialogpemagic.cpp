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
#include "dialogpemagic.h"
#include "ui_dialogpemagic.h"

DialogPEMagic::DialogPEMagic(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEMagic)
{
    ui->setupUi(this);

    ui->comboBoxMagic->addItem("");
    ui->comboBoxMagic->addItem("NT_OPTIONAL_HDR32_MAGIC");
    ui->comboBoxMagic->addItem("NT_OPTIONAL_HDR64_MAGIC");
    ui->comboBoxMagic->addItem("ROM_OPTIONAL_HDR_MAGIC");

    ui->lineEditMagic->setInputMask("HHHH");

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPEMagic::~DialogPEMagic()
{
    delete ui;
}

void DialogPEMagic::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPEMagic::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditMagic->setToolTip(tr("Magic"));
        ui->comboBoxMagic->setToolTip(tr("Set magic"));
    }

    unsigned short sMagic=pefile->getOptionalHeader_Magic();

    ui->lineEditMagic->setText(QString("%1").arg(sMagic,4,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPEMagic::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->lineEditMagic->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->comboBoxMagic->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogPEMagic::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEMagic::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setOptionalHeader_Magic(ui->lineEditMagic->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPEMagic::on_lineEditMagic_textChanged(const QString &arg1)
{
    bool bTemp;

    switch(arg1.toInt(&bTemp,16))
    {
        case(IMAGE_NT_OPTIONAL_HDR32_MAGIC):
            ui->comboBoxMagic->setCurrentIndex(1);
            break;

        case(IMAGE_NT_OPTIONAL_HDR64_MAGIC):
            ui->comboBoxMagic->setCurrentIndex(2);
            break;

        case(IMAGE_ROM_OPTIONAL_HDR_MAGIC):
            ui->comboBoxMagic->setCurrentIndex(3);
            break;

        default:
            ui->comboBoxMagic->setCurrentIndex(0);
    }

    ui->pushButtonApply->setEnabled(true);
}

void DialogPEMagic::on_comboBoxMagic_currentIndexChanged(int index)
{
    switch(index)
    {
        case(1):
            ui->lineEditMagic->setText("010b");
            break;

        case(2):
            ui->lineEditMagic->setText("020b");
            break;

        case(3):
            ui->lineEditMagic->setText("0107");
            break;
    }
}
