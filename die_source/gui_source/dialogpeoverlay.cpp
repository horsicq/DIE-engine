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
#include "dialogpeoverlay.h"
#include "ui_dialogpeoverlay.h"

DialogPEOverlay::DialogPEOverlay(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEOverlay)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);
    ui->lineEditStatus->setHex(false);

    bOverlayPresent=false;

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_hex));
    ui->pushButtonAdd->setShortcut(QKeySequence(__KeySequence_ctrlA));
    ui->pushButtonDump->setShortcut(QKeySequence(__KeySequence_ctrlD));
    ui->pushButtonEntropy->setShortcut(QKeySequence(__KeySequence_ctrlE));
    ui->pushButtonRemove->setShortcut(QKeySequence(__KeySequence_ctrlW));
    ui->pushButtonReplace->setShortcut(QKeySequence(__KeySequence_ctrlR));
}

DialogPEOverlay::~DialogPEOverlay()
{
    delete ui;
}

bool DialogPEOverlay::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->pushButtonAdd->setToolTip(tr("Add overlay"));
        ui->pushButtonDump->setToolTip(tr("Dump overlay"));
        ui->pushButtonRemove->setToolTip(tr("Remove overlay"));
        ui->pushButtonReplace->setToolTip(tr("Replace overlay"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditStatus->setToolTip(tr("Overlay status"));

        ui->pushButtonHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Overlay")));
        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg(tr("Overlay")));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg(tr("Overlay")));

    }

    bOverlayPresent=pefile->isOverlayPresent();

    if(bOverlayPresent)
    {
        nOffset=pefile->getOverlayOffset();
        nSize=pefile->getOverlaySize();

        ui->lineEditOffset->setText(QString("%1").arg(nOffset));
        ui->lineEditSize->setText(QString("%1").arg(nSize));
        ui->lineEditOffset->setEnabled(true);
        ui->lineEditSize->setEnabled(true);
        ui->pushButtonHex->setEnabled(true);
        ui->pushButtonDump->setEnabled(true);
        ui->pushButtonEntropy->setEnabled(true);

        ui->lineEditStatus->setText(tr("%1 present").arg(tr("Overlay")));
    }
    else
    {
        ui->lineEditOffset->setText("");
        ui->lineEditSize->setText("");
        ui->lineEditOffset->setEnabled(false);
        ui->lineEditSize->setEnabled(false);
        ui->pushButtonHex->setEnabled(false);
        ui->pushButtonDump->setEnabled(false);
        ui->pushButtonEntropy->setEnabled(false);

        ui->lineEditStatus->setText(tr("No %1 present").arg(tr("Overlay")));
    }

    ui->checkBoxReadOnly->setChecked(!ui->checkBoxReadOnly->isChecked());
    ui->checkBoxReadOnly->setChecked(!ui->checkBoxReadOnly->isChecked());

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());

    return true;
}

void DialogPEOverlay::on_checkBoxReadOnly_stateChanged(int arg1)
{
    if(ui->checkBoxReadOnly->checkState())
    {
        ui->pushButtonAdd->setEnabled(false);
        ui->pushButtonReplace->setEnabled(false);
        ui->pushButtonRemove->setEnabled(false);
    }
    else
    {
        if(bOverlayPresent)
        {
            ui->pushButtonReplace->setEnabled(true);
            ui->pushButtonRemove->setEnabled(true);
        }
        else
        {
            ui->pushButtonAdd->setEnabled(true);
        }
    }
}

void DialogPEOverlay::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogPEOverlay::on_pushButtonAdd_clicked()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file..."),pOptions->sLastDirectory, tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        if(pefile->addOverlay(sFileName))
        {
            ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

            reload();
            emit reloadSignal();
        }
    }
}

void DialogPEOverlay::on_pushButtonHex_clicked()
{
    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Overlay"));

    dialoghde.exec();
}

void DialogPEOverlay::on_pushButtonDump_clicked()
{
    QString sFileName;


    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Overlay")), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        pefile->dumpOverlay(sFileName);
    }
}

void DialogPEOverlay::on_pushButtonRemove_clicked()
{
    if(bOverlayPresent)
    {
        pefile->deleteOverlay();

        ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

        emit reloadSignal();
    }
}

void DialogPEOverlay::on_pushButtonReplace_clicked()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file..."),pOptions->sLastDirectory, tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        if(pefile->deleteOverlay())
        {
            if(pefile->addOverlay(sFileName))
            {
                ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

                emit reloadSignal();
            }
        }
    }
}


void DialogPEOverlay::on_pushButtonEntropy_clicked()
{
    DialogEntropy dialogentropy(this);

    dialogentropy.setData(pOptions,pefile,nOffset,nSize);

    dialogentropy.exec();
}
