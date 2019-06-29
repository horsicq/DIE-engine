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
#include "dialogpesubsystem.h"
#include "ui_dialogpesubsystem.h"

DialogPESubsystem::DialogPESubsystem(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPESubsystem)
{
    ui->setupUi(this);



    ui->comboBoxSubsystem->addItem("");
    ui->comboBoxSubsystem->addItem("UNKNOWN");
    ui->comboBoxSubsystem->addItem("NATIVE");
    ui->comboBoxSubsystem->addItem("WINDOWS_GUI");
    ui->comboBoxSubsystem->addItem("WINDOWS_CUI");
    ui->comboBoxSubsystem->addItem("OS2_CUI");
    ui->comboBoxSubsystem->addItem("POSIX_CUI");
    ui->comboBoxSubsystem->addItem("NATIVE_WINDOWS");
    ui->comboBoxSubsystem->addItem("WINDOWS_CE_GUI");
    ui->comboBoxSubsystem->addItem("EFI_APPLICATION");
    ui->comboBoxSubsystem->addItem("EFI_BOOT_SERVICE_DRIVER");
    ui->comboBoxSubsystem->addItem("EFI_RUNTIME_DRIVER");
    ui->comboBoxSubsystem->addItem("EFI_ROM");
    ui->comboBoxSubsystem->addItem("XBOX");

    ui->lineEditSubsystem->setInputMask("HHHH");


    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPESubsystem::~DialogPESubsystem()
{
    delete ui;
}

void DialogPESubsystem::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPESubsystem::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditSubsystem->setToolTip(tr("Subsystem"));
        ui->comboBoxSubsystem->setToolTip(tr("Set Subsystem"));
    }

    unsigned short sSubsystem=pefile->getOptionalHeader_Subsystem();

    ui->lineEditSubsystem->setText(QString("%1").arg(sSubsystem,4,16,QChar('0')));


    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());

    ui->pushButtonApply->setEnabled(false);

    return true;
}


void DialogPESubsystem::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->lineEditSubsystem->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->comboBoxSubsystem->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogPESubsystem::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPESubsystem::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setOptionalHeader_Subsystem(ui->lineEditSubsystem->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPESubsystem::on_lineEditSubsystem_textChanged(const QString &arg1)
{
    bool bTemp;

    switch(arg1.toInt(&bTemp,16))
    {
        case(IMAGE_SUBSYSTEM_UNKNOWN):
            ui->comboBoxSubsystem->setCurrentIndex(1);
            break;

        case(IMAGE_SUBSYSTEM_NATIVE):
            ui->comboBoxSubsystem->setCurrentIndex(2);
            break;

        case(IMAGE_SUBSYSTEM_WINDOWS_GUI):
            ui->comboBoxSubsystem->setCurrentIndex(3);
            break;

        case(IMAGE_SUBSYSTEM_WINDOWS_CUI):
            ui->comboBoxSubsystem->setCurrentIndex(4);
            break;

        case(IMAGE_SUBSYSTEM_OS2_CUI):
            ui->comboBoxSubsystem->setCurrentIndex(5);
            break;

        case(IMAGE_SUBSYSTEM_POSIX_CUI):
            ui->comboBoxSubsystem->setCurrentIndex(6);
            break;

        case(IMAGE_SUBSYSTEM_NATIVE_WINDOWS):
            ui->comboBoxSubsystem->setCurrentIndex(7);
            break;

        case(IMAGE_SUBSYSTEM_WINDOWS_CE_GUI):
            ui->comboBoxSubsystem->setCurrentIndex(8);
            break;

        case(IMAGE_SUBSYSTEM_EFI_APPLICATION):
            ui->comboBoxSubsystem->setCurrentIndex(9);
            break;

        case(IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER):
            ui->comboBoxSubsystem->setCurrentIndex(10);
            break;

        case(IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER):
            ui->comboBoxSubsystem->setCurrentIndex(11);
            break;

        case(IMAGE_SUBSYSTEM_EFI_ROM):
            ui->comboBoxSubsystem->setCurrentIndex(12);
            break;

        case(IMAGE_SUBSYSTEM_XBOX):
            ui->comboBoxSubsystem->setCurrentIndex(13);
            break;

        default:
            ui->comboBoxSubsystem->setCurrentIndex(0);
    }

    ui->pushButtonApply->setEnabled(true);
}


void DialogPESubsystem::on_comboBoxSubsystem_currentIndexChanged(int index)
{

    switch(index)
    {
        case(1):
            ui->lineEditSubsystem->setText("0000");
            break;

        case(2):
            ui->lineEditSubsystem->setText("0001");
            break;

        case(3):
            ui->lineEditSubsystem->setText("0002");
            break;

        case(4):
            ui->lineEditSubsystem->setText("0003");
            break;

        case(5):
            ui->lineEditSubsystem->setText("0005");
            break;

        case(6):
            ui->lineEditSubsystem->setText("0007");
            break;

        case(7):
            ui->lineEditSubsystem->setText("0008");
            break;

        case(8):
            ui->lineEditSubsystem->setText("0009");
            break;

        case(9):
            ui->lineEditSubsystem->setText("000a");
            break;

        case(10):
            ui->lineEditSubsystem->setText("000b");
            break;

        case(11):
            ui->lineEditSubsystem->setText("000c");
            break;

        case(12):
            ui->lineEditSubsystem->setText("000d");
            break;

        case(13):
            ui->lineEditSubsystem->setText("000e");
            break;

    }
}
