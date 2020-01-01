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
#include "dialogpetimedatestamp.h"
#include "ui_dialogpetimedatestamp.h"

DialogPETimeDateStamp::DialogPETimeDateStamp(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPETimeDateStamp)
{
    ui->setupUi(this);

    ui->lineEditTimeDateStamp->setInputMask("HHHHHHHH");


    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPETimeDateStamp::~DialogPETimeDateStamp()
{
    delete ui;
}

bool DialogPETimeDateStamp::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditTimeDateStamp->setToolTip(tr("TimeDateStamp"));
        ui->dateTimeEditTimeDateStamp->setToolTip(tr("Set TimeDateStamp"));
    }

    ui->lineEditTimeDateStamp->setText(QString("%1").arg(pefile->getFileHeader_TimeDateStamp(),4,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPETimeDateStamp::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    ui->lineEditTimeDateStamp->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->dateTimeEditTimeDateStamp->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogPETimeDateStamp::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogPETimeDateStamp::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPETimeDateStamp::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setFileHeader_TimeDateStamp(ui->lineEditTimeDateStamp->text().toUInt(&bTemp,16));


    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPETimeDateStamp::on_lineEditTimeDateStamp_textChanged(const QString &arg1)
{
    QDateTime dt;
    bool bTemp;

    dt.setTime_t(arg1.toInt(&bTemp,16));
    ui->dateTimeEditTimeDateStamp->setDateTime(dt);

    ui->pushButtonApply->setEnabled(true);
}

void DialogPETimeDateStamp::on_dateTimeEditTimeDateStamp_dateTimeChanged(const QDateTime &date)
{
    ui->lineEditTimeDateStamp->setText(QString("%1").arg(date.toTime_t(),8,16));
}
