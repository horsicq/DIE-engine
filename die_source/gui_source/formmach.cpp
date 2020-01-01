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
#include "formmach.h"
#include "ui_formmach.h"

FormMACH::FormMACH(QWidget *parent) :
    FormGeneric(parent),
    ui(new Ui::FormMACH)
{
    ui->setupUi(this);

    connect(this, SIGNAL(reloadSignal()), this, SLOT(reload()));

    connect(this, SIGNAL(appendError(QString)), parent, SLOT(_error(QString)));
    connect(this, SIGNAL(reloadSignal()), parent, SLOT(_reload()));

    ui->pushButtonMachHeader->setShortcut(QKeySequence(__KeySequence_altM));
    ui->pushButtonLoadCommands->setShortcut(QKeySequence(__KeySequence_altB));
    ui->pushButtonSegments->setShortcut(QKeySequence(__KeySequence_altN));
    ui->pushButtonEntryPoint->setShortcut(QKeySequence(__KeySequence_ctrlN));
    ui->pushButtonOverlay->setShortcut(QKeySequence(__KeySequence_altO));
}

FormMACH::~FormMACH()
{
    delete ui;
}

bool FormMACH::reload()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        if(machfile.isMACH64())
        {
            ui->lineEditEntryPoint->setText(QString("%1").arg(machfile.getEntryPoint(),16,16,QChar('0')));
        }
        else
        {
            ui->lineEditEntryPoint->setText(QString("%1").arg(machfile.getEntryPoint(),8,16,QChar('0')));
        }
    }

    ui->pushButtonUUID->setEnabled(machfile.isUUIDPresent());
    ui->pushButtonOverlay->setEnabled(machfile.isOverlayPresent());

    return true;
}
void FormMACH::showTooltips(bool bShow)
{
    if(bShow)
    {
        ui->pushButtonMachHeader->setToolTip(tr("MACH Header"));
        ui->pushButtonLoadCommands->setToolTip(tr("Load Command Data Structures table"));
        ui->pushButtonSegments->setToolTip(tr("Segments header table"));
        ui->lineEditEntryPoint->setToolTip(tr("Address of EntryPoint"));
        ui->pushButtonEntryPoint->setToolTip(tr("Address of EntryPoint"));
        ui->pushButtonOverlay->setToolTip(tr("Overlay"));
    }
    else
    {
        ui->pushButtonMachHeader->setToolTip("");
        ui->pushButtonLoadCommands->setToolTip("");
        ui->pushButtonSegments->setToolTip("");
        ui->lineEditEntryPoint->setToolTip("");
        ui->pushButtonEntryPoint->setToolTip("");
        ui->pushButtonOverlay->setToolTip("");
    }
}

void FormMACH::on_pushButtonEntryPoint_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        if(machfile.isMACH64())
        {
            dialoghde.setData(pOptions,machfile.getFileName(),0,0,"x86-ASM",machfile.getDisasmMode(),"MASM",machfile.RVAToOffset64(machfile.getEntryPoint()),1,tr("EntryPoint"));
        }
        else
        {
            dialoghde.setData(pOptions,machfile.getFileName(),0,0,"x86-ASM",machfile.getDisasmMode(),"MASM",machfile.RVAToOffset32(machfile.getEntryPoint()),1,tr("EntryPoint"));
        }

        dialoghde.exec();
    }
}

void FormMACH::on_pushButtonMachHeader_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogMACHHeader dialog(this);

        dialog.setData(&machfile,pOptions);

        dialog.exec();
    }
}

void FormMACH::on_pushButtonLoadCommands_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogMACHLC dialog(this);

        dialog.setData(pOptions,&machfile);

        dialog.exec();
    }
}

void FormMACH::on_pushButtonSegments_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogMACHSegments dialog(this);

        dialog.setData(pOptions,&machfile);

        dialog.exec();
    }
}

void FormMACH::on_pushButtonSections_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogMACHSections dialog(this);

        dialog.setData(pOptions,&machfile);

        dialog.exec();
    }
}

void FormMACH::on_pushButtonUUID_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogMACHUUID dialog(this);

        dialog.setData(pOptions,&machfile);

        dialog.exec();
    }
}

void FormMACH::on_pushButtonLibs_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogMACHLibs dialog(this);

        dialog.setData(pOptions,&machfile);

        dialog.exec();
    }
}

void FormMACH::on_pushButtonOverlay_clicked()
{
    MACHFile machfile;
    connect(&machfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(machfile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        dialoghde.setData(pOptions,machfile.getFileName(),0,0,"HEX","","",machfile.getOverlayOffset(),machfile.getOverlaySize(),tr("Overlay"));

        dialoghde.exec();
    }
}
