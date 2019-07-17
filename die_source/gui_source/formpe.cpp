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
#include "formpe.h"
#include "ui_formpe.h"

FormPE::FormPE(QWidget *parent) :
    FormGeneric(parent),
    ui(new Ui::FormPE)
{
    ui->setupUi(this);
    connect(this, SIGNAL(reloadSignal()), this, SLOT(reload()));

    connect(this, SIGNAL(appendError(QString)), parent, SLOT(_error(QString)));
    connect(this, SIGNAL(reloadSignal()), parent, SLOT(_reload()));

    ui->pushButtonExport->setShortcut(QKeySequence(__KeySequence_altE));
    ui->pushButtonImport->setShortcut(QKeySequence(__KeySequence_altI));
    ui->pushButtonEntryPoint->setShortcut(QKeySequence(__KeySequence_ctrlN));


    ui->pushButtonNET->setShortcut(QKeySequence(__KeySequence_altN));
    ui->pushButtonNumberOfSections->setShortcut(QKeySequence(__KeySequence_altS));
    ui->pushButtonOverlay->setShortcut(QKeySequence(__KeySequence_altO));
    ui->pushButtonPE->setShortcut(QKeySequence(__KeySequence_altP));
    ui->pushButtonResource->setShortcut(QKeySequence(__KeySequence_altR));
}

FormPE::~FormPE()
{
    delete ui;
}

bool FormPE::reload()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(pefile.setFileName(sFileName))
    {
        QString sType=pefile.getType();

        if((sType=="PE")||(sType=="PE+(64)"))
        {
            ui->lineEditNumberOfSections->setText(QString("%1").arg(pefile.getFileHeader_NumberOfSections(),4,16,QChar('0')));
            ui->lineEditEntryPoint->setText(QString("%1").arg(pefile.getOptionalHeader_AddressOfEntryPoint(),8,16,QChar('0')));

            if(pefile.isPEPlus())
            {
                ui->lineEditImageBase->setText(QString("%1").arg(pefile.getOptionalHeader_ImageBase64(),16,16,QChar('0')));
            }
            else
            {
                ui->lineEditImageBase->setText(QString("%1").arg(pefile.getOptionalHeader_ImageBase(),8,16,QChar('0')));
            }

            ui->lineEditSizeOfImage->setText(QString("%1").arg(pefile.getOptionalHeader_SizeOfImage(),8,16,QChar('0')));

            ui->pushButtonExport->setEnabled(pefile.isExportPresent());
            ui->pushButtonImport->setEnabled(pefile.isImportPresent());
            ui->pushButtonResource->setEnabled(pefile.isResourcePresent());
            ui->pushButtonOverlay->setEnabled(pefile.isOverlayPresent());

            ui->pushButtonNET->setEnabled(pefile.isNETPresent());
        }
    }

    return true;
}
void FormPE::on_pushButtonPE_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(pefile.setFileName(sFileName))
    {
        DialogPE dialogpe(this);

        dialogpe.setData(pOptions,&pefile);

        dialogpe.exec();
    }
}
void FormPE::on_pushButtonEntryPoint_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(pefile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        unsigned int nEP=pefile.getOptionalHeader_AddressOfEntryPoint();
        dialoghde.setData(pOptions,pefile.getFileName(),0,0,"x86-ASM",pefile.getDisasmMode(),"MASM",pefile.RVAToOffset(nEP),1,tr("EntryPoint"));

        dialoghde.exec();
    }
}

void FormPE::showTooltips(bool bShow)
{
    if(bShow)
    {
        ui->pushButtonEntryPoint->setToolTip(tr("Address of EntryPoint"));
        ui->pushButtonExport->setToolTip(tr("Export directory"));
        ui->pushButtonImport->setToolTip(tr("Import directory"));
        ui->pushButtonNumberOfSections->setToolTip(tr("Sections table"));
        ui->pushButtonOverlay->setToolTip(tr("Overlay"));
        ui->pushButtonPE->setToolTip(tr("PE viewer/editor"));
        //        ui->pushButtonStub->setToolTip("MS DOS Stub");

        ui->pushButtonResource->setToolTip(tr("Resource directory"));

        ui->pushButtonNET->setToolTip(tr(".NET"));

        ui->lineEditEntryPoint->setToolTip(tr("Address of EntryPoint"));
        ui->lineEditImageBase->setToolTip(tr("Virtual address of image base"));
        ui->lineEditNumberOfSections->setToolTip(tr("Number of sections"));
        ui->lineEditSizeOfImage->setToolTip(tr("Size of image"));
    }
    else
    {
        ui->pushButtonEntryPoint->setToolTip("");
        ui->pushButtonExport->setToolTip("");
        ui->pushButtonImport->setToolTip("");
        ui->pushButtonNumberOfSections->setToolTip("");
        ui->pushButtonOverlay->setToolTip("");
        ui->pushButtonPE->setToolTip("");
        //        ui->pushButtonStub->setToolTip("");
        //        ui->pushButtonVersion->setToolTip("");
        ui->pushButtonResource->setToolTip("");
        ui->lineEditEntryPoint->setToolTip("");
        ui->lineEditImageBase->setToolTip("");
        ui->lineEditNumberOfSections->setToolTip("");
        ui->lineEditSizeOfImage->setToolTip("");
        //        ui->pushButtonManifest->setToolTip("");
        ui->pushButtonNET->setToolTip("");
    }
}

void FormPE::on_pushButtonNumberOfSections_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(pefile.setFileName(sFileName))
    {
        DialogPESections dialogsections(this);

        dialogsections.setData(pOptions,&pefile);

        dialogsections.exec();
    }
}

//void PEForm::on_pushButtonStub_clicked()
//{
//    PEFile pefile;
//    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));
//    if(pefile.setFileName(sFileName))
//    {
//        DialogHDE dialoghde(this);

//        dialoghde.setData(pefile.getFileName(),pOptions,0,0,"x86-ASM","16","MASM",pefile.getDosStubOffset(),pefile.getDosStubSize());

//        dialoghde.exec();
//    }
//}

void FormPE::on_pushButtonExport_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(pefile.setFileName(sFileName))
    {
        DialogPEExport dialogexport(this);

        dialogexport.setData(pOptions,&pefile);

        dialogexport.exec();
    }
}

void FormPE::on_pushButtonImport_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(pefile.setFileName(sFileName))
    {
        DialogPEImport dialogimport(this);
        dialogimport.setData(pOptions,&pefile);

        dialogimport.exec();
    }
}

void FormPE::on_pushButtonOverlay_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));

    if(pefile.setFileName(sFileName))
    {
        DialogHDE dialoghde(this);

        dialoghde.setData(pOptions,pefile.getFileName(),0,0,"HEX","","",pefile.getOverlayOffset(),pefile.getOverlaySize(),"Overlay");

        dialoghde.exec();
    }
}

void FormPE::on_pushButtonResource_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));
    pefile.setFileName(sFileName);

    DialogPEResource dialogresource(this);

    dialogresource.setData(pOptions,&pefile);

    dialogresource.exec();

    pefile.close();
}

void FormPE::on_pushButtonNET_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SIGNAL(appendError(QString)));
    pefile.setFileName(sFileName);

    DialogPECOM dialogcom(this);

    dialogcom.setData(pOptions,&pefile);

    dialogcom.exec();

    pefile.close();
}
