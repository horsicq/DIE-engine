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
#include "dialogpedllcharacteristics.h"
#include "ui_dialogpedllcharacteristics.h"

DialogPEDllCharacteristics::DialogPEDllCharacteristics(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEDllCharacteristics)
{
    ui->setupUi(this);

    ui->lineEditDllCharacteristics->setInputMask("HHHH");

    connect(ui->lineEditDllCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(lineEditDllCharacteristics_textChanged(QString)));
    connect(this,SIGNAL(checked()),this,SLOT(checkbox_reload()));

    bEdited=true;

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPEDllCharacteristics::~DialogPEDllCharacteristics()
{
    delete ui;
}

void DialogPEDllCharacteristics::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->lineEditDllCharacteristics->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->groupBox->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogPEDllCharacteristics::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPEDllCharacteristics::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditDllCharacteristics->setToolTip(tr("DLL characteristics"));

        ui->checkBoxDYNAMIC_BASE->setToolTip(tr("DLL can move"));
        ui->checkBoxFORCE_INTEGRITY->setToolTip(tr("Code Integrity Image"));
        ui->checkBoxNO_BIND->setToolTip(tr("Do not bind this image"));
        ui->checkBoxNO_ISOLATION->setToolTip(tr("Image understands isolation and doesn't want it"));
        ui->checkBoxNO_SEH->setToolTip(tr("Image does not use SEH.  No SE handler may reside in this image"));
        ui->checkBoxNX_COMPAT->setToolTip(tr("Image is NX compatible"));
        ui->checkBoxTERMINAL_SERVER_AWARE->setToolTip(tr("Terminal server aware"));
        ui->checkBoxWDM_DRIVER->setToolTip(tr("Driver uses WDM model"));
        ui->checkBoxHIGH_ENTROPY_VA->setToolTip(tr("Image can handle a high entropy 64-bit virtual address space."));
        ui->checkBoxAPPCONTAINER->setToolTip(tr("Image should execute in an AppContainer"));
        ui->checkBoxGUARD_CF->setToolTip(tr("Image supports Control Flow Guard."));
    }

    bEdited=false;

    ui->lineEditDllCharacteristics->setText(QString("%1").arg(pefile->getOptionalHeader_DllCharacteristics(),4,16,QChar('0')));

    bEdited=true;

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());

    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPEDllCharacteristics::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEDllCharacteristics::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setOptionalHeader_DllCharacteristics(ui->lineEditDllCharacteristics->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}
void DialogPEDllCharacteristics::lineEditDllCharacteristics_textChanged(const QString &arg1)
{
    bool bTemp;
    unsigned short sDllCharacteristics=0;

    sDllCharacteristics=arg1.toUInt(&bTemp,16);

    ui->checkBoxHIGH_ENTROPY_VA->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA);
    ui->checkBoxDYNAMIC_BASE->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE);
    ui->checkBoxFORCE_INTEGRITY->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY);
    ui->checkBoxNX_COMPAT->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_NX_COMPAT);
    ui->checkBoxNO_ISOLATION->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_NO_ISOLATION);
    ui->checkBoxNO_SEH->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_NO_SEH);
    ui->checkBoxNO_BIND->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_NO_BIND);
    ui->checkBoxAPPCONTAINER->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_APPCONTAINER);
    ui->checkBoxWDM_DRIVER->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_WDM_DRIVER);
    ui->checkBoxGUARD_CF->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_GUARD_CF);
    ui->checkBoxTERMINAL_SERVER_AWARE->setChecked(sDllCharacteristics&IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE);

    ui->pushButtonApply->setEnabled(true);
}

void DialogPEDllCharacteristics::checkbox_reload()
{
    unsigned short sDllCharacteristics=0;

    if(bEdited)
    {
        if(ui->checkBoxDYNAMIC_BASE->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
        }

        if(ui->checkBoxFORCE_INTEGRITY->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY;
        }

        if(ui->checkBoxNX_COMPAT->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_NX_COMPAT;
        }

        if(ui->checkBoxNO_ISOLATION->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_NO_ISOLATION;
        }

        if(ui->checkBoxNO_SEH->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_NO_SEH;
        }

        if(ui->checkBoxNO_BIND->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_NO_BIND;
        }

        if(ui->checkBoxWDM_DRIVER->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_WDM_DRIVER;
        }

        if(ui->checkBoxTERMINAL_SERVER_AWARE->isChecked())
        {
            sDllCharacteristics+=IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE;
        }

        bEdited=false;
        ui->lineEditDllCharacteristics->setText(QString("%1").arg(sDllCharacteristics,4,16,QChar('0')));
        bEdited=true;
    }

    //connect(ui->lineEditDllCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(lineEditDllCharacteristics_textChanged(QString)));
}

void DialogPEDllCharacteristics::on_checkBoxDYNAMIC_BASE_stateChanged(int arg1)
{
    emit checked();
}

void DialogPEDllCharacteristics::on_checkBoxFORCE_INTEGRITY_stateChanged(int arg1)
{
    emit checked();
}

void DialogPEDllCharacteristics::on_checkBoxNX_COMPAT_stateChanged(int arg1)
{
    emit checked();
}

void DialogPEDllCharacteristics::on_checkBoxNO_ISOLATION_stateChanged(int arg1)
{
    emit checked();
}

void DialogPEDllCharacteristics::on_checkBoxNO_SEH_stateChanged(int arg1)
{
    emit checked();
}

void DialogPEDllCharacteristics::on_checkBoxNO_BIND_stateChanged(int arg1)
{
    emit checked();
}

void DialogPEDllCharacteristics::on_checkBoxWDM_DRIVER_stateChanged(int arg1)
{
    emit checked();
}

void DialogPEDllCharacteristics::on_checkBoxTERMINAL_SERVER_AWARE_stateChanged(int arg1)
{
    emit checked();
}
