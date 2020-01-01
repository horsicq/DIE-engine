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
#include "formelf.h"
#include "ui_formelf.h"

FormELF::FormELF(QWidget *parent) :
    FormGeneric(parent),
    ui(new Ui::FormELF)
{
    ui->setupUi(this);

    ui->pushButtonElfHeader->setShortcut(QKeySequence(__KeySequence_altM));
    ui->pushButtonProgramHeaderTable->setShortcut(QKeySequence(__KeySequence_altB));
    ui->pushButtonSectionHeaderTable->setShortcut(QKeySequence(__KeySequence_altN));
    ui->pushButtonEntryPoint->setShortcut(QKeySequence(__KeySequence_ctrlN));
    ui->pushButtonOverlay->setShortcut(QKeySequence(__KeySequence_altO));
}

FormELF::~FormELF()
{
    delete ui;
}

bool FormELF::reload()
{
    ELFFile elffile;
    connect(&elffile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(elffile.setFileName(sFileName))
    {
        if(elffile.isELF64())
        {
            ui->lineEditEntryPoint->setText(QString("%1").arg(elffile.getElfHeader_entry64(),16,16,QChar('0')));
        }
        else
        {
            ui->lineEditEntryPoint->setText(QString("%1").arg(elffile.getElfHeader_entry(),8,16,QChar('0')));
        }

        ui->pushButtonSectionHeaderTable->setEnabled(elffile.getElfHeader_shnum());
        ui->pushButtonProgramHeaderTable->setEnabled(elffile.getElfHeader_phnum());
        ui->pushButtonStringTable->setEnabled(elffile.getElfHeader_shstrndx());
        ui->pushButtonOverlay->setEnabled(elffile.isOverlayPresent());
    }

    return true;
}
void FormELF::showTooltips(bool bShow)
{
    if(bShow)
    {
        ui->pushButtonElfHeader->setToolTip(tr("ELF Header"));
        ui->pushButtonProgramHeaderTable->setToolTip(tr("Program Header Table"));
        ui->pushButtonSectionHeaderTable->setToolTip(tr("Section Header Table"));
        ui->lineEditEntryPoint->setToolTip(tr("Address of EntryPoint"));
        ui->pushButtonEntryPoint->setToolTip(tr("Address of EntryPoint"));
        ui->pushButtonOverlay->setToolTip(tr("Overlay"));
    }
    else
    {
        ui->pushButtonElfHeader->setToolTip("");
        ui->pushButtonProgramHeaderTable->setToolTip("");
        ui->pushButtonSectionHeaderTable->setToolTip("");
        ui->lineEditEntryPoint->setToolTip("");
        ui->pushButtonEntryPoint->setToolTip("");
        ui->pushButtonOverlay->setToolTip("");
    }
}

void FormELF::on_pushButtonElfHeader_clicked()
{
    ELFFile elffile;
    connect(&elffile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(elffile.setFileName(sFileName))
    {
        DialogELFHeader dialogelfheader(this);

        dialogelfheader.setData(pOptions,&elffile);

        dialogelfheader.exec();
    }
}

void FormELF::on_pushButtonProgramHeaderTable_clicked()
{
    ELFFile elffile;
    connect(&elffile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(elffile.setFileName(sFileName))
    {
        DialogELFPHT dialogelfpht(this);

        dialogelfpht.setData(pOptions,&elffile);

        dialogelfpht.exec();
    }
}

void FormELF::on_pushButtonSectionHeaderTable_clicked()
{
    ELFFile elffile;
    connect(&elffile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(elffile.setFileName(sFileName))
    {
        DialogELFSHT dialogelfsht(this);

        dialogelfsht.setData(pOptions,&elffile);

        dialogelfsht.exec();
    }
}

void FormELF::on_pushButtonEntryPoint_clicked()
{
    ELFFile elffile;
    connect(&elffile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(elffile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        if(elffile.isELF64())
        {
            dialoghde.setData(pOptions,elffile.getFileName(),0,0,"x86-ASM",elffile.getDisasmMode(),"MASM",elffile.RVAToOffset64(elffile.getElfHeader_entry64()),1,tr("EntryPoint"));
        }
        else
        {
            dialoghde.setData(pOptions,elffile.getFileName(),0,0,"x86-ASM",elffile.getDisasmMode(),"MASM",elffile.RVAToOffset(elffile.getElfHeader_entry()),1,tr("EntryPoint"));
        }

        dialoghde.exec();
    }
}

void FormELF::on_pushButtonStringTable_clicked()
{
    ELFFile elffile;
    connect(&elffile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(elffile.setFileName(sFileName))
    {
        DialogELFStrings dialog(this);

        dialog.setData(pOptions,&elffile,elffile.getElfHeader_shstrndx());

        dialog.exec();
    }
}

void FormELF::on_pushButtonOverlay_clicked()
{
    ELFFile elffile;
    connect(&elffile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(elffile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        dialoghde.setData(pOptions,elffile.getFileName(),0,0,"HEX","","",elffile.getOverlayOffset(),elffile.getOverlaySize(),tr("Overlay"));

        dialoghde.exec();
    }
}
