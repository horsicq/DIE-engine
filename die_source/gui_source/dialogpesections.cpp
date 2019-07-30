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
#include "dialogpesections.h"
#include "ui_dialogpesections.h"

DialogPESections::DialogPESections(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPESections)
{
    ui->setupUi(this);

    actEditHeader=new QAction(tr("Edit header"), this);
    actHex=new QAction(tr("Hex"), this);
    actDump=new QAction(tr("Dump"), this);
    actEntropy=new QAction(tr("Entropy"), this);
    actSearch=new QAction(tr("Search"), this);
    actCopyAsString=new QAction("", this);

    connect(actEditHeader,SIGNAL(triggered()),this,SLOT(editHeader()));
    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actDump,SIGNAL(triggered()),this,SLOT(dump()));
    connect(actEntropy,SIGNAL(triggered()),this,SLOT(entropy()));
    connect(actSearch,SIGNAL(triggered()),this,SLOT(search()));
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonAddNewSection->setShortcut(QKeySequence(__KeySequence_ctrlA));
    ui->pushButtonCheckPackedStatus->setShortcut(QKeySequence(__KeySequence_ctrlF));
    ui->pushButtonDeleteLastSection->setShortcut(QKeySequence(__KeySequence_ctrlW));
}

DialogPESections::~DialogPESections()
{
    delete ui;
}

bool DialogPESections::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->pushButtonAddNewSection->setToolTip(tr("Add new section"));
        ui->pushButtonDeleteLastSection->setToolTip(tr("Delete last section"));

        ui->pushButtonCheckPackedStatus->setToolTip(tr("Check packed status"));
    }

    QTableWidgetItem *newItem;
    //    pefile->setFileName(sFileName);

    QString sTemp;

    ui->tableWidgetSections->clear();

    ui->tableWidgetSections->setColumnCount(8);

    QStringList labels;

    labels.append("Name");
    labels.append("V.Address");
    labels.append("V.Size");
    labels.append("Offset");
    labels.append("R.Size");
    labels.append("Flags");
    labels.append("Entropy");
    labels.append("Packed");

    ui->tableWidgetSections->setHorizontalHeaderLabels(labels);
    ui->tableWidgetSections->horizontalHeader()->setVisible(true);

    nNumberOfSections=pefile->getFileHeader_NumberOfSections();

    nNumberOfSections=qMin(nNumberOfSections,256);

    ui->tableWidgetSections->setRowCount(nNumberOfSections);

    for(int i=0; i<nNumberOfSections; i++)
    {
        newItem = new QTableWidgetItem;
        //        newItem->setFlags(newItem->flags()^Qt::ItemIsEditable);

        sTemp=pefile->getSectionNameAsString(i);

        newItem->setText(sTemp);

        ui->tableWidgetSections->setItem(i,0,newItem);
        ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getSection_VirtualAddress(i),8,16,QChar('0')));
        ui->tableWidgetSections->setItem(i,1,newItem);
        ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getSection_VirtualSize(i),8,16,QChar('0')));
        ui->tableWidgetSections->setItem(i,2,newItem);
        ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getSection_PointerToRawData(i),8,16,QChar('0')));
        ui->tableWidgetSections->setItem(i,3,newItem);
        ui->tableWidgetSections->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getSection_SizeOfRawData(i),8,16,QChar('0')));
        ui->tableWidgetSections->setItem(i,4,newItem);
        ui->tableWidgetSections->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(pefile->getSection_Characteristics(i),8,16,QChar('0')));
        ui->tableWidgetSections->setItem(i,5,newItem);
        ui->tableWidgetSections->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText("?");
        ui->tableWidgetSections->setItem(i,6,newItem);
        ui->tableWidgetSections->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText("?");
        ui->tableWidgetSections->setItem(i,7,newItem);
        ui->tableWidgetSections->item(i,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetSections->setRowHeight(i,20);
    }

    if(nNumberOfSections==0)
    {
        ui->pushButtonDeleteLastSection->setEnabled(false);
    }
    else
    {
        if(!ui->checkBoxReadOnly->isChecked())
        {
            ui->pushButtonDeleteLastSection->setEnabled(true);
        }
    }

    ui->tableWidgetSections->setColumnWidth(0,65);
    ui->tableWidgetSections->setColumnWidth(1,65);
    ui->tableWidgetSections->setColumnWidth(2,65);
    ui->tableWidgetSections->setColumnWidth(3,65);
    ui->tableWidgetSections->setColumnWidth(4,65);
    ui->tableWidgetSections->setColumnWidth(5,65);
    ui->tableWidgetSections->setColumnWidth(6,55);
    ui->tableWidgetSections->setColumnWidth(7,55);


    if(pOptions->bSectionsEntropyAfterOpen)
    {
        on_pushButtonCheckPackedStatus_clicked();
    }

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());


    return true;

}

void DialogPESections::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogPESections::on_pushButtonAddNewSection_clicked()
{
    IMAGE_SECTION_HEADER ish= {0};

    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file..."),pOptions->sLastDirectory, tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        pefile->addSection(&ish,sFileName);
    }

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    reload();
    emit reloadSignal();
}

void DialogPESections::on_pushButtonDeleteLastSection_clicked()
{
    pefile->deleteLastSection();

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    reload();
    emit reloadSignal();
}

void DialogPESections::on_tableWidgetSections_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetSections->selectedItems().count())
    {
        return;
    }

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    //     QString sString=((QTableWidgetItem *)(ui->tableWidgetSections->focusWidget()))->text();
    //     qDebug(sString.toAscii().data());
    if(pefile->getSection_SizeOfRawData(nSection))
    {
        actSearch->setEnabled(true);
        actHex->setEnabled(true);
        actDump->setEnabled(true);
        actEntropy->setEnabled(true);
    }
    else
    {
        actSearch->setEnabled(false);
        actHex->setEnabled(false);
        actDump->setEnabled(false);
        actEntropy->setEnabled(false);
    }

    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));

    menu.addSeparator();
    menu.addAction(actEditHeader);
    menu.addAction(actSearch);
    menu.addAction(actEntropy);
    menu.addAction(actHex);
    menu.addAction(actDump);

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetSections->mapToGlobal(pos2));

}

void DialogPESections::on_checkBoxReadOnly_stateChanged(int arg1)
{
    ui->pushButtonAddNewSection->setEnabled(!ui->checkBoxReadOnly->isChecked());
    //actEditHeader->setEnabled(!ui->checkBoxReadOnly->isChecked());

    if(nNumberOfSections)
    {
        ui->pushButtonDeleteLastSection->setEnabled(!ui->checkBoxReadOnly->isChecked());
    }
    else
    {
        ui->pushButtonDeleteLastSection->setEnabled(false);
    }
}

void DialogPESections::editHeader()
{
    //    PE pefile(pbaData->data(),pbaData->size());

    DialogPESectionHeader dialogpesectionheader(this);

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    dialogpesectionheader.setData(pOptions,pefile,nSection);

    dialogpesectionheader.exec();
}

void DialogPESections::search()
{
    DialogSearch dialog(this);

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    //    dialoghde.setData(pefile->getFileName(),pefile->getSection_PointerToRawData(nSection),pefile->getSection_SizeOfRawData(nSection),pefile->getDisasmMode(),pOptions);
    dialog.setData(pOptions,pefile,pefile->getSection_PointerToRawData(nSection),pefile->getSection_SizeOfRawData(nSection));

    dialog.exec();
}

void DialogPESections::hex()
{
    DialogHDE dialoghde(this);

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    //    dialoghde.setData(pefile->getFileName(),pefile->getSection_PointerToRawData(nSection),pefile->getSection_SizeOfRawData(nSection),pefile->getDisasmMode(),pOptions);
    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->getSection_PointerToRawData(nSection),pefile->getSection_SizeOfRawData(nSection),tr("Section(%1)").arg(nSection));

    dialoghde.exec();
}

void DialogPESections::dump()
{
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Section(%1)").arg(nSection)), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        pefile->dumpSection(sFileName,nSection);
    }
}
void DialogPESections::entropy()
{
    DialogEntropy dialogentropy(this);

    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();
    dialogentropy.setData(pOptions,pefile,pefile->getSection_PointerToRawData(nSection),pefile->getSection_SizeOfRawData(nSection));

    dialogentropy.exec();
}
void DialogPESections::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetSections->selectedItems().at(0)->text()));
}

void DialogPESections::on_pushButtonCheckPackedStatus_clicked()
{
    double dEntropy;

    nNumberOfSections;

    for(int i=0; i<nNumberOfSections; i++)
    {
        dEntropy=pefile->calculateEntropy(pefile->getSection_PointerToRawData(i),pefile->getSection_SizeOfRawData(i));

        //        if(fEntropy<0.01)
        //        {
        //           fEntropy=0.0;
        //        }

        //        fEntropy=0.0;
        ui->tableWidgetSections->item(i,6)->setText(QString("%1").arg(dEntropy,0,'f',2));

        if(dEntropy>pOptions->dThreshold)
        {
            ui->tableWidgetSections->item(i,7)->setText(tr("yes"));
        }
        else
        {
            ui->tableWidgetSections->item(i,7)->setText(tr("no"));
        }
    }
}
