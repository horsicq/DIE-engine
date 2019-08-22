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
#include "dialogpemachine.h"
#include "ui_dialogpemachine.h"

DialogPEMachine::DialogPEMachine(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEMachine)
{
    ui->setupUi(this);

    ui->comboBoxMachine->addItem("");
    ui->comboBoxMachine->addItem("UNKNOWN");
    ui->comboBoxMachine->addItem("I386");
    ui->comboBoxMachine->addItem("R3000");
    ui->comboBoxMachine->addItem("R4000");
    ui->comboBoxMachine->addItem("R10000");
    ui->comboBoxMachine->addItem("WCEMIPSV2");
    ui->comboBoxMachine->addItem("ALPHA");
    ui->comboBoxMachine->addItem("SH3");
    ui->comboBoxMachine->addItem("SH3DSP");
    ui->comboBoxMachine->addItem("SH3E");
    ui->comboBoxMachine->addItem("SH4");
    ui->comboBoxMachine->addItem("SH5");
    ui->comboBoxMachine->addItem("ARM");
    ui->comboBoxMachine->addItem("THUMB");
    ui->comboBoxMachine->addItem("AM33");
    ui->comboBoxMachine->addItem("POWERPC");
    ui->comboBoxMachine->addItem("POWERPCFP");
    ui->comboBoxMachine->addItem("IA64");
    ui->comboBoxMachine->addItem("MIPS16");
    ui->comboBoxMachine->addItem("ALPHA64");
    ui->comboBoxMachine->addItem("MIPSFPU");
    ui->comboBoxMachine->addItem("MIPSFPU16");
    ui->comboBoxMachine->addItem("TRICORE");
    ui->comboBoxMachine->addItem("CEF");
    ui->comboBoxMachine->addItem("EBC");
    ui->comboBoxMachine->addItem("AMD64");
    ui->comboBoxMachine->addItem("M32R");
    ui->comboBoxMachine->addItem("CEE");
    ui->comboBoxMachine->addItem("ARM64");

    ui->lineEditMachine->setInputMask("HHHH");

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPEMachine::~DialogPEMachine()
{
    delete ui;
}

void DialogPEMachine::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPEMachine::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditMachine->setToolTip(tr("Machine"));
        ui->comboBoxMachine->setToolTip(tr("Set machine"));
    }

    unsigned short sMACHine=pefile->getFileHeader_Machine();

    ui->lineEditMachine->setText(QString("%1").arg(sMACHine,4,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPEMachine::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    ui->lineEditMachine->setReadOnly(ui->checkBoxReadOnly->checkState());
    ui->comboBoxMachine->setEnabled(!ui->checkBoxReadOnly->checkState());
}

void DialogPEMachine::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEMachine::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setFileHeader_Machine(ui->lineEditMachine->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPEMachine::on_lineEditMachine_textChanged(const QString &arg1)
{
    bool bTemp;

    switch(arg1.toInt(&bTemp,16))
    {
        case(IMAGE_FILE_MACHINE_UNKNOWN):
            ui->comboBoxMachine->setCurrentIndex(1);
            break;

        case(IMAGE_FILE_MACHINE_I386):
            ui->comboBoxMachine->setCurrentIndex(2);
            break;

        case(IMAGE_FILE_MACHINE_R3000):
            ui->comboBoxMachine->setCurrentIndex(3);
            break;

        case(IMAGE_FILE_MACHINE_R4000):
            ui->comboBoxMachine->setCurrentIndex(4);
            break;

        case(IMAGE_FILE_MACHINE_R10000):
            ui->comboBoxMachine->setCurrentIndex(5);
            break;

        case(IMAGE_FILE_MACHINE_WCEMIPSV2):
            ui->comboBoxMachine->setCurrentIndex(6);
            break;

        case(IMAGE_FILE_MACHINE_ALPHA):
            ui->comboBoxMachine->setCurrentIndex(7);
            break;

        case(IMAGE_FILE_MACHINE_SH3):
            ui->comboBoxMachine->setCurrentIndex(8);
            break;

        case(IMAGE_FILE_MACHINE_SH3DSP):
            ui->comboBoxMachine->setCurrentIndex(9);
            break;

        case(IMAGE_FILE_MACHINE_SH3E):
            ui->comboBoxMachine->setCurrentIndex(10);
            break;

        case(IMAGE_FILE_MACHINE_SH4):
            ui->comboBoxMachine->setCurrentIndex(11);
            break;

        case(IMAGE_FILE_MACHINE_SH5):
            ui->comboBoxMachine->setCurrentIndex(12);
            break;

        case(IMAGE_FILE_MACHINE_ARM):
            ui->comboBoxMachine->setCurrentIndex(13);
            break;

        case(IMAGE_FILE_MACHINE_THUMB):
            ui->comboBoxMachine->setCurrentIndex(14);
            break;

        case(IMAGE_FILE_MACHINE_AM33):
            ui->comboBoxMachine->setCurrentIndex(15);
            break;

        case(IMAGE_FILE_MACHINE_POWERPC):
            ui->comboBoxMachine->setCurrentIndex(16);
            break;

        case(IMAGE_FILE_MACHINE_POWERPCFP):
            ui->comboBoxMachine->setCurrentIndex(17);
            break;

        case(IMAGE_FILE_MACHINE_IA64):
            ui->comboBoxMachine->setCurrentIndex(18);
            break;

        case(IMAGE_FILE_MACHINE_MIPS16):
            ui->comboBoxMachine->setCurrentIndex(19);
            break;

        case(IMAGE_FILE_MACHINE_ALPHA64):
            ui->comboBoxMachine->setCurrentIndex(20);
            break;

        case(IMAGE_FILE_MACHINE_MIPSFPU):
            ui->comboBoxMachine->setCurrentIndex(21);
            break;

        case(IMAGE_FILE_MACHINE_MIPSFPU16):
            ui->comboBoxMachine->setCurrentIndex(22);
            break;

        case(IMAGE_FILE_MACHINE_TRICORE):
            ui->comboBoxMachine->setCurrentIndex(23);
            break;

        case(IMAGE_FILE_MACHINE_CEF):
            ui->comboBoxMachine->setCurrentIndex(24);
            break;

        case(IMAGE_FILE_MACHINE_EBC):
            ui->comboBoxMachine->setCurrentIndex(25);
            break;

        case(IMAGE_FILE_MACHINE_AMD64):
            ui->comboBoxMachine->setCurrentIndex(26);
            break;

        case(IMAGE_FILE_MACHINE_M32R):
            ui->comboBoxMachine->setCurrentIndex(27);
            break;

        case(IMAGE_FILE_MACHINE_CEE):
            ui->comboBoxMachine->setCurrentIndex(28);
            break;

        case(0xAA64):
            ui->comboBoxMachine->setCurrentIndex(29);
            break;

        default:
            ui->comboBoxMachine->setCurrentIndex(0);
    }

    ui->pushButtonApply->setEnabled(true);
}

void DialogPEMachine::on_comboBoxMachine_currentIndexChanged(int index)
{
    switch(index)
    {
        case(1):
            ui->lineEditMachine->setText("0000");
            break;

        case(2):
            ui->lineEditMachine->setText("014c");
            break;

        case(3):
            ui->lineEditMachine->setText("0162");
            break;

        case(4):
            ui->lineEditMachine->setText("0166");
            break;

        case(5):
            ui->lineEditMachine->setText("0168");
            break;

        case(6):
            ui->lineEditMachine->setText("0169");
            break;

        case(7):
            ui->lineEditMachine->setText("0184");
            break;

        case(8):
            ui->lineEditMachine->setText("01a2");
            break;

        case(9):
            ui->lineEditMachine->setText("01a3");
            break;

        case(10):
            ui->lineEditMachine->setText("01a4");
            break;

        case(11):
            ui->lineEditMachine->setText("01a6");
            break;

        case(12):
            ui->lineEditMachine->setText("01a8");
            break;

        case(13):
            ui->lineEditMachine->setText("01c0");
            break;

        case(14):
            ui->lineEditMachine->setText("01c2");
            break;

        case(15):
            ui->lineEditMachine->setText("01d3");
            break;

        case(16):
            ui->lineEditMachine->setText("01F0");
            break;

        case(17):
            ui->lineEditMachine->setText("01f1");
            break;

        case(18):
            ui->lineEditMachine->setText("0200");
            break;

        case(19):
            ui->lineEditMachine->setText("0266");
            break;

        case(20):
            ui->lineEditMachine->setText("0284");
            break;

        case(21):
            ui->lineEditMachine->setText("0366");
            break;

        case(22):
            ui->lineEditMachine->setText("0466");
            break;

        case(23):
            ui->lineEditMachine->setText("0520");
            break;

        case(24):
            ui->lineEditMachine->setText("0CEF");
            break;

        case(25):
            ui->lineEditMachine->setText("0EBC");
            break;

        case(26):
            ui->lineEditMachine->setText("8664");
            break;

        case(27):
            ui->lineEditMachine->setText("9041");
            break;

        case(28):
            ui->lineEditMachine->setText("C0EE");
            break;

        case(29):
            ui->lineEditMachine->setText("AA64");
            break;
    }
}
