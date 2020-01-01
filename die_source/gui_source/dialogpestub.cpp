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
#include "dialogpestub.h"
#include "ui_dialogpestub.h"

DialogPEStub::DialogPEStub(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEStub)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);
    ui->lineEditStatus->setHex(false);

    bStubPresent=false;
    bInvalidHeader=false;

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_hex));
    ui->pushButtonAdd->setShortcut(QKeySequence(__KeySequence_ctrlA));
    ui->pushButtonDump->setShortcut(QKeySequence(__KeySequence_ctrlD));
    ui->pushButtonRemove->setShortcut(QKeySequence(__KeySequence_ctrlW));
    ui->pushButtonReplace->setShortcut(QKeySequence(__KeySequence_ctrlR));
}

DialogPEStub::~DialogPEStub()
{
    delete ui;
}

bool DialogPEStub::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->pushButtonAdd->setToolTip(tr("Add MS DOS Stub"));
        ui->pushButtonDump->setToolTip(tr("Dump MS DOS Stub"));
        ui->pushButtonRemove->setToolTip(tr("Remove MS DOS Stub"));
        ui->pushButtonReplace->setToolTip(tr("Replace MS DOS Stub"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditStatus->setToolTip(tr("MS DOS Stub status"));
        ui->pushButtonRich->setToolTip(tr("Rich signature"));

        ui->pushButtonHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("MS DOS Stub")));
        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg(tr("MS DOS Stub")));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg(tr("MS DOS Stub")));
    }

    bStubPresent=pefile->isDosStubPresent();

    if(bStubPresent)
    {
        nOffset=pefile->getDosStubOffset();
        nSize=pefile->getDosStubSize();

        ui->lineEditOffset->setText(QString("%1").arg(nOffset));
        ui->lineEditSize->setText(QString("%1").arg(nSize));
        ui->lineEditOffset->setEnabled(true);
        ui->lineEditSize->setEnabled(true);
        ui->pushButtonHex->setEnabled(true);
        ui->pushButtonDump->setEnabled(true);

        ui->lineEditStatus->setText(tr("%1 present").arg(tr("Stub")));
    }
    else
    {
        ui->lineEditOffset->setText("");
        ui->lineEditSize->setText("");
        ui->lineEditOffset->setEnabled(false);
        ui->lineEditSize->setEnabled(false);
        ui->pushButtonHex->setEnabled(false);
        ui->pushButtonDump->setEnabled(false);

        ui->lineEditStatus->setText(tr("No %1 present").arg(tr("Stub")));


        if(pefile->getDosHeader_lfanew()<0x40)
        {
            bInvalidHeader=true;
            ui->lineEditStatus->setText(QString("%1 %2").arg(tr("Invalid")).arg(tr("Header")));
        }
    }

    ui->pushButtonRich->setEnabled(pefile->isRichSignaturePresent());

    ui->checkBoxReadOnly->setChecked(!ui->checkBoxReadOnly->isChecked());
    ui->checkBoxReadOnly->setChecked(!ui->checkBoxReadOnly->isChecked());

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());

    return true;
}

void DialogPEStub::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    if(ui->checkBoxReadOnly->checkState())
    {
        ui->pushButtonAdd->setEnabled(false);
        ui->pushButtonReplace->setEnabled(false);
        ui->pushButtonRemove->setEnabled(false);
    }
    else
    {
        if(bStubPresent)
        {
            ui->pushButtonReplace->setEnabled(true);
            ui->pushButtonRemove->setEnabled(true);
        }
        else
        {

            ui->pushButtonAdd->setEnabled(!bInvalidHeader);
        }
    }
}

void DialogPEStub::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogPEStub::on_pushButtonAdd_clicked()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file..."),pOptions->sLastDirectory, tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        if(pefile->addDosStub(sFileName))
        {
            ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

            emit reloadSignal();
        }
    }
}

void DialogPEStub::on_pushButtonHex_clicked()
{
    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"x86-ASM","16","MASM",nOffset,nSize,tr("MS DOS Stub"));

    dialoghde.exec();
}

void DialogPEStub::on_pushButtonDump_clicked()
{
    //    QFile file;
    QString sFileName;
    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Stub")), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        pefile->dumpDosStub(sFileName);
    }
}

void DialogPEStub::on_pushButtonRemove_clicked()
{
    if(bStubPresent)
    {
        pefile->DeleteDosStub();

        ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);
        emit reloadSignal();
    }
}

void DialogPEStub::on_pushButtonReplace_clicked()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file..."),pOptions->sLastDirectory, tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        if(pefile->DeleteDosStub())
        {
            if(pefile->addDosStub(sFileName))
            {
                ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

                emit reloadSignal();
            }
        }
    }
}

void DialogPEStub::on_pushButtonRich_clicked()
{
    QList<PEFile::RICH_RECORD> list=pefile->getRichSignatureRecords();

    QString sResult;

    for(int i=0; i<list.count(); i++)
    {
        sResult+=tr("id=%1 version=%2 count=%3").arg(list.at(i).nId).arg(list.at(i).nVersion).arg(list.at(i).nCount);
        sResult+="\n";
    }

    QMessageBox::information(this,"Rich signature",sResult);
}
