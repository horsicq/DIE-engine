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
#include "dialogelfclass.h"
#include "ui_dialogelfclass.h"

DialogELFClass::DialogELFClass(QWidget *parent) :
    DialogELFGeneric(parent),
    ui(new Ui::DialogELFClass)
{
    ui->setupUi(this);

    ui->comboBoxValue->addItem("");
    ui->comboBoxValue->addItem("ELFCLASSNONE");
    ui->comboBoxValue->addItem("ELFCLASS32");
    ui->comboBoxValue->addItem("ELFCLASS64");

    ui->lineEditValue->setInputMask("HH");

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogELFClass::~DialogELFClass()
{
    delete ui;
}

void DialogELFClass::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogELFClass::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditValue->setToolTip(tr("Class"));
        ui->comboBoxValue->setToolTip(tr("Set class"));
    }

    unsigned char cClass=elffile->getElfHeader_class();

    ui->lineEditValue->setText(QString("%1").arg(cClass,2,16,QChar('0')));


    ui->checkBoxReadOnly->setEnabled(!elffile->isReadOnly());

    ui->pushButtonApply->setEnabled(false);

    return true;
}


void DialogELFClass::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->lineEditValue->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->comboBoxValue->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogELFClass::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogELFClass::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    elffile->setElfHeader_class(ui->lineEditValue->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogELFClass::on_lineEditValue_textChanged(const QString &arg1)
{
    bool bTemp;

    switch(arg1.toInt(&bTemp,16))
    {
        case(0):
            ui->comboBoxValue->setCurrentIndex(1);
            break;

        case(1):
            ui->comboBoxValue->setCurrentIndex(2);
            break;

        case(2):
            ui->comboBoxValue->setCurrentIndex(3);
            break;

        default:
            ui->comboBoxValue->setCurrentIndex(0);
    }

    ui->pushButtonApply->setEnabled(true);
}


void DialogELFClass::on_comboBoxValue_currentIndexChanged(int index)
{
    switch(index)
    {
        case(1):
            ui->lineEditValue->setText("00");
            break;

        case(2):
            ui->lineEditValue->setText("01");
            break;

        case(3):
            ui->lineEditValue->setText("02");
            break;
    }
}
