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
#include "dialogpemetadata.h"
#include "ui_dialogpemetadata.h"

DialogPEMetaData::DialogPEMetaData(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEMetaData)
{
    ui->setupUi(this);


    ui->lineEditVersion->setString(true);
    ui->lineEditVersion->setHex(false);
    ui->lineEditVersion->setDec(false);

    actHex=new QAction(tr("Hex"), this);
    actDump=new QAction(tr("Dump"), this);
    actMetaDataTables=new QAction(tr("MetaData Tables"), this);
    actMetaDataStrings=new QAction(tr("MetaData Strings"), this);
    actMetaDataUS=new QAction(tr("MetaData US"), this);

    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actDump,SIGNAL(triggered()),this,SLOT(dump()));
    connect(actMetaDataTables,SIGNAL(triggered()),this,SLOT(metadatatables()));
    connect(actMetaDataStrings,SIGNAL(triggered()),this,SLOT(metadatastrings()));
    connect(actMetaDataUS,SIGNAL(triggered()),this,SLOT(metadataus()));


    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEMetaData::~DialogPEMetaData()
{
    delete ui;
}

void DialogPEMetaData::on_pushButtonOK_clicked()
{
    this->close();
}
bool DialogPEMetaData::reload()
{

    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->lineEditFlags->setToolTip(tr("Flags"));
        ui->lineEditMajorRuntimeVersion->setToolTip(tr("MajorRuntimeVersion"));
        ui->lineEditMinorRuntimeVersion->setToolTip(tr("MinorRuntimeVersion"));
        ui->lineEditSignature->setToolTip(tr("Signature"));
        ui->lineEditReserved->setToolTip(tr("Reserved"));
        ui->lineEditVersion->setToolTip(tr("Version"));
        ui->lineEditStreams->setToolTip(tr("Number of streams"));
    }


    if(pefile->isNETPresent())
    {

        ui->lineEditSignature->setText(QString("%1").arg(pefile->getCLI_MetaData_Signature(),8,16,QChar('0')));
        ui->lineEditMajorRuntimeVersion->setText(QString("%1").arg(pefile->getCLI_MetaData_MajorVersion(),4,16,QChar('0')));
        ui->lineEditMinorRuntimeVersion->setText(QString("%1").arg(pefile->getCLI_MetaData_MinorVersion(),4,16,QChar('0')));
        ui->lineEditReserved->setText(QString("%1").arg(pefile->getCLI_MetaData_Reserved(),4,16,QChar('0')));
        ui->lineEditVersion->setText(pefile->getCLI_MetaData_Version());
        ui->lineEditFlags->setText(QString("%1").arg(pefile->getCLI_MetaData_Flags(),4,16,QChar('0')));
        ui->lineEditStreams->setText(QString("%1").arg(pefile->getCLI_MetaData_Streams(),4,16,QChar('0')));

        QTreeWidgetItem *item0;

        for(int i=0; i<pefile->getCLI_MetaData_Streams(); i++)
        {
            item0=new QTreeWidgetItem;
            item0->setText(0,pefile->getCLI_MetaData_Stream_Name(i));
            item0->setData(0,Qt::UserRole,QString("%1:%2").arg(pefile->getCLI_MetaData_Stream_Offset(i)+pefile->getCLI_MetaDataOffset()).arg(pefile->getCLI_MetaData_Stream_Size(i)));

            ui->treeWidgetExtra->insertTopLevelItem(i,item0);
        }

        //        int g=pefile->getCLI_MetaData_TablesHeaderOffset();
        //        g++;

    }


    return true;
}

void DialogPEMetaData::hex()
{
    unsigned int nOffset=ui->treeWidgetExtra->currentItem()->data(0,Qt::UserRole).toString().section(":",0,0).toInt();
    unsigned int nSize=ui->treeWidgetExtra->currentItem()->data(0,Qt::UserRole).toString().section(":",1,1).toInt();


    DialogHDE dialoghde(this);

    //    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("MetaData"));

    dialoghde.exec();
}

void DialogPEMetaData::dump()
{
    unsigned int nOffset=ui->treeWidgetExtra->currentItem()->data(0,Qt::UserRole).toString().section(":",0,0).toInt();
    unsigned int nSize=ui->treeWidgetExtra->currentItem()->data(0,Qt::UserRole).toString().section(":",1,1).toInt();
    //    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("MetaData")), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        pefile->dump(sFileName,nOffset,nSize);
    }
}

void DialogPEMetaData::metadatatables()
{
    DialogPEMetaDataTables dialogpemetadatatables(this);
    dialogpemetadatatables.setData(pOptions,pefile);

    dialogpemetadatatables.exec();
}

void DialogPEMetaData::metadatastrings()
{
    DialogPEMetaDataStrings dialogpemetadatastrings(this);
    dialogpemetadatastrings.setData(pOptions,pefile);

    dialogpemetadatastrings.exec();
}

void DialogPEMetaData::metadataus()
{
    DialogPEMetaDataUS dialogpemetadataus(this);
    dialogpemetadataus.setData(pOptions,pefile);

    dialogpemetadataus.exec();
}


void DialogPEMetaData::on_treeWidgetExtra_customContextMenuRequested(const QPoint &pos)
{
    if(ui->treeWidgetExtra->currentItem()->data(0,Qt::UserRole).toString()!="")
    {
        QMenu menu;


        if(ui->treeWidgetExtra->currentItem()->text(0)=="#~")
        {
            menu.addAction(actMetaDataTables);
        }
        else if(ui->treeWidgetExtra->currentItem()->text(0)=="#Strings")
        {
            menu.addAction(actMetaDataStrings);
        }
        else if(ui->treeWidgetExtra->currentItem()->text(0)=="#US")
        {
            menu.addAction(actMetaDataUS);
        }

        menu.addAction(actHex);
        menu.addAction(actDump);

        menu.exec(ui->treeWidgetExtra->mapToGlobal(pos));
    }
}
