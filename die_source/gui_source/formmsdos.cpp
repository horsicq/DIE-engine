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
#include "formmsdos.h"
#include "ui_formmsdos.h"

FormMSDOS::FormMSDOS(QWidget *parent) :
    FormGeneric(parent),
    ui(new Ui::FormMSDOS)
{
    ui->setupUi(this);

    connect(this, SIGNAL(reloadSignal()), this, SLOT(reload()));

    connect(this, SIGNAL(appendError(QString)), parent, SLOT(_error(QString)));
    connect(this, SIGNAL(reloadSignal()), parent, SLOT(_reload()));

    ui->pushButtonEntryPoint->setShortcut(QKeySequence(__KeySequence_ctrlN));
    ui->pushButtonDosHeader->setShortcut(QKeySequence(__KeySequence_altM));
}

FormMSDOS::~FormMSDOS()
{
    delete ui;
}

bool FormMSDOS::reload()
{
    MSDOSFile msdosfile;
    connect(&msdosfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(msdosfile.setFileName(sFileName))
    {
        ui->lineEditEntryPoint->setText(QString("%1").arg(msdosfile.getDosHeader_ip(),4,16,QChar('0')));
        ui->pushButtonOverlay->setEnabled(msdosfile.isOverlayPresent());
    }

    return true;
}

void FormMSDOS::showTooltips(bool bShow)
{
    if(bShow)
    {
        ui->pushButtonDosHeader->setToolTip(tr("MS DOS Header"));
        ui->lineEditEntryPoint->setToolTip(tr("Address of EntryPoint"));
        ui->pushButtonEntryPoint->setToolTip(tr("Address of EntryPoint"));
    }
    else
    {
        ui->pushButtonDosHeader->setToolTip("");
        ui->lineEditEntryPoint->setToolTip("");
        ui->pushButtonEntryPoint->setToolTip("");
    }
}

void FormMSDOS::on_pushButtonEntryPoint_clicked()
{
    MSDOSFile msdosfile;
    connect(&msdosfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(msdosfile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        //        dialoghde.setData(msdosfile.getFileName(),msdosfile.getAddressOfEntryPoint(),0,msdosfile.getDisasmMode(),pOptions);
        dialoghde.setData(pOptions,msdosfile.getFileName(),0,0,"x86-ASM",msdosfile.getDisasmMode(),"MASM",msdosfile.getEntryPointOffset(),1,tr("EntryPoint"));

        dialoghde.exec();
    }
}

void FormMSDOS::on_pushButtonDosHeader_clicked()
{
    MSDOSFile msdosfile;
    connect(&msdosfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(msdosfile.setFileName(sFileName))
    {
        DialogMSDOSHeader dialogmsdosheader(this);

        dialogmsdosheader.setData(pOptions,&msdosfile);

        dialogmsdosheader.exec();
    }
}

void FormMSDOS::on_pushButtonOverlay_clicked()
{
    MSDOSFile msdosfile;
    connect(&msdosfile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(msdosfile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        dialoghde.setData(pOptions,msdosfile.getFileName(),0,0,"HEX","","",msdosfile.getOverlayOffset(),msdosfile.getOverlaySize(),tr("Overlay"));

        dialoghde.exec();
    }
}
