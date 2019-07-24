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
#include "dialogpecharacteristics.h"
#include "ui_dialogpecharacteristics.h"

DialogPECharacteristics::DialogPECharacteristics(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPECharacteristics)
{
    ui->setupUi(this);

    ui->lineEditCharacteristics->setInputMask("HHHH");

    connect(ui->lineEditCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(lineEditCharacteristics_textChanged(QString)));
    connect(this,SIGNAL(checked()),this,SLOT(checkbox_reload()));

    bEdited=true;

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPECharacteristics::~DialogPECharacteristics()
{
    delete ui;
}

void DialogPECharacteristics::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->lineEditCharacteristics->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->groupBox->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogPECharacteristics::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPECharacteristics::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditCharacteristics->setToolTip(tr("Characteristics"));

        ui->checkBox32BIT_MACHINE->setToolTip(tr("32 bit word machine"));
        ui->checkBoxAGGRESIVE_WS_TRIM->setToolTip(tr("Aggressively trim working set"));
        ui->checkBoxRELOCS_STRIPPED->setToolTip(tr("Relocation info stripped from file"));
        ui->checkBoxEXECUTABLE_IMAGE->setToolTip(tr("File is executable  (i.e. no unresolved external references)"));
        ui->checkBoxLINE_NUMS_STRIPPED->setToolTip(tr("Line numbers stripped from file"));
        ui->checkBoxLOCAL_SYMS_STRIPPED ->setToolTip(tr("Local symbols stripped from file"));
        ui->checkBoxLARGE_ADDRESS_AWARE->setToolTip(tr("App can handle >2gb addresses"));
        ui->checkBoxBYTES_REVERSED_LO->setToolTip(tr("Bytes of machine word are reversed"));
        ui->checkBoxDEBUG_STRIPPED->setToolTip(tr("Debugging info stripped from file in .DBG file"));
        ui->checkBoxREMOVABLE_RUN_FROM_SWAP->setToolTip(tr("If Image is on removable media, copy and run from the swap file"));
        ui->checkBoxNET_RUN_FROM_SWAP->setToolTip(tr("If Image is on Net, copy and run from the swap file"));
        ui->checkBoxSYSTEM->setToolTip(tr("System File"));
        ui->checkBoxDLL->setToolTip(tr("File is a DLL"));
        ui->checkBoxUP_SYSTEM_ONLY->setToolTip(tr("File should only be run on a UP machine"));
        ui->checkBoxBYTES_REVERSED_HI->setToolTip(tr("Bytes of machine word are reversed"));
    }

    bEdited=false;

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());

    ui->lineEditCharacteristics->setText(QString("%1").arg(pefile->getFileHeader_Characteristics(),4,16,QChar('0')));

    bEdited=true;

    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPECharacteristics::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPECharacteristics::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setFileHeader_Characteristics(ui->lineEditCharacteristics->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPECharacteristics::lineEditCharacteristics_textChanged(const QString &arg1)
{
    bool bTemp;
    unsigned short sCharacteristics=0;

    sCharacteristics=arg1.toUInt(&bTemp,16);

    ui->checkBoxRELOCS_STRIPPED->setChecked(sCharacteristics&IMAGE_FILE_RELOCS_STRIPPED);
    ui->checkBoxEXECUTABLE_IMAGE->setChecked(sCharacteristics&IMAGE_FILE_EXECUTABLE_IMAGE);
    ui->checkBoxLINE_NUMS_STRIPPED->setChecked(sCharacteristics&IMAGE_FILE_LINE_NUMS_STRIPPED);
    ui->checkBoxLOCAL_SYMS_STRIPPED->setChecked(sCharacteristics&IMAGE_FILE_LOCAL_SYMS_STRIPPED);
    ui->checkBoxAGGRESIVE_WS_TRIM->setChecked(sCharacteristics&IMAGE_FILE_AGGRESIVE_WS_TRIM);
    ui->checkBoxLARGE_ADDRESS_AWARE->setChecked(sCharacteristics&IMAGE_FILE_LARGE_ADDRESS_AWARE);
    ui->checkBoxBYTES_REVERSED_LO->setChecked(sCharacteristics&IMAGE_FILE_BYTES_REVERSED_LO);

    ui->checkBox32BIT_MACHINE->setChecked(sCharacteristics&IMAGE_FILE_32BIT_MACHINE);
    ui->checkBoxDEBUG_STRIPPED->setChecked(sCharacteristics&IMAGE_FILE_DEBUG_STRIPPED);
    ui->checkBoxREMOVABLE_RUN_FROM_SWAP->setChecked(sCharacteristics&IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP);
    ui->checkBoxNET_RUN_FROM_SWAP->setChecked(sCharacteristics&IMAGE_FILE_NET_RUN_FROM_SWAP);
    ui->checkBoxSYSTEM->setChecked(sCharacteristics&IMAGE_FILE_SYSTEM);
    ui->checkBoxDLL->setChecked(sCharacteristics&IMAGE_FILE_DLL);
    ui->checkBoxUP_SYSTEM_ONLY->setChecked(sCharacteristics&IMAGE_FILE_UP_SYSTEM_ONLY);
    ui->checkBoxBYTES_REVERSED_HI->setChecked(sCharacteristics&IMAGE_FILE_BYTES_REVERSED_HI);

    ui->pushButtonApply->setEnabled(true);
}

void DialogPECharacteristics::on_checkBoxRELOCS_STRIPPED_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxEXECUTABLE_IMAGE_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxLINE_NUMS_STRIPPED_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxLOCAL_SYMS_STRIPPED_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxAGGRESIVE_WS_TRIM_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxLARGE_ADDRESS_AWARE_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxBYTES_REVERSED_LO_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBox32BIT_MACHINE_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxDEBUG_STRIPPED_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxREMOVABLE_RUN_FROM_SWAP_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxNET_RUN_FROM_SWAP_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxSYSTEM_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxDLL_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxUP_SYSTEM_ONLY_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::on_checkBoxBYTES_REVERSED_HI_stateChanged(int arg1)
{
    emit checked();
}

void DialogPECharacteristics::checkbox_reload()
{
    unsigned short sCharacteristics=0;

    if(bEdited)
    {
        if(ui->checkBoxRELOCS_STRIPPED->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_RELOCS_STRIPPED;
        }

        if(ui->checkBoxEXECUTABLE_IMAGE->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_EXECUTABLE_IMAGE;
        }

        if(ui->checkBoxLINE_NUMS_STRIPPED->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_LINE_NUMS_STRIPPED;
        }

        if(ui->checkBoxLOCAL_SYMS_STRIPPED->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_LOCAL_SYMS_STRIPPED;
        }

        if(ui->checkBoxAGGRESIVE_WS_TRIM->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_AGGRESIVE_WS_TRIM;
        }

        if(ui->checkBoxLARGE_ADDRESS_AWARE->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_LARGE_ADDRESS_AWARE;
        }

        if(ui->checkBoxBYTES_REVERSED_LO->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_BYTES_REVERSED_LO;
        }

        if(ui->checkBox32BIT_MACHINE->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_32BIT_MACHINE;
        }

        if(ui->checkBoxDEBUG_STRIPPED->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_DEBUG_STRIPPED;
        }

        if(ui->checkBoxREMOVABLE_RUN_FROM_SWAP->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP;
        }

        if(ui->checkBoxNET_RUN_FROM_SWAP->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_NET_RUN_FROM_SWAP;
        }

        if(ui->checkBoxSYSTEM->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_SYSTEM;
        }

        if(ui->checkBoxDLL->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_DLL;
        }

        if(ui->checkBoxUP_SYSTEM_ONLY->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_UP_SYSTEM_ONLY;
        }

        if(ui->checkBoxBYTES_REVERSED_HI->isChecked())
        {
            sCharacteristics+=IMAGE_FILE_BYTES_REVERSED_HI;
        }

        bEdited=false;
        ui->lineEditCharacteristics->setText(QString("%1").arg(sCharacteristics,4,16,QChar('0')));
        bEdited=true;
    }
    //connect(ui->lineEditCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(lineEditCharacteristics_textChanged(QString)));
}
