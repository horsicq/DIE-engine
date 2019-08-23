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
#include "dialogpeimportlibrary.h"
#include "ui_dialogpeimportlibrary.h"

DialogPEImportLibrary::DialogPEImportLibrary(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEImportLibrary)
{
    ui->setupUi(this);

    ui->lineEditLibrary->setHex(false);

    ui->tableWidgetImportLibrary->setRowCount(5);
    ui->tableWidgetImportLibrary->setColumnCount(2);

    for(int i=0; i<5; i++)
    {
        ui->tableWidgetImportLibrary->setRowHeight(i,20);
    }

    QTableWidgetItem *newItem;

    lineEditOriginalFirstThunk=new QLineEditX;
    lineEditTimeDateStamp=new QLineEditX;
    lineEditForwarderChain=new QLineEditX;
    lineEditName=new QLineEditX;
    lineEditFirstThunk=new QLineEditX;

    lineEditOriginalFirstThunk->setAlignment(Qt::AlignCenter);
    lineEditTimeDateStamp->setAlignment(Qt::AlignCenter);
    lineEditForwarderChain->setAlignment(Qt::AlignCenter);
    lineEditName->setAlignment(Qt::AlignCenter);
    lineEditFirstThunk->setAlignment(Qt::AlignCenter);

    lineEditOriginalFirstThunk->setInputMask("HHHHHHHH");
    lineEditTimeDateStamp->setInputMask("HHHHHHHH");
    lineEditForwarderChain->setInputMask("HHHHHHHH");
    lineEditName->setInputMask("HHHHHHHH");
    lineEditFirstThunk->setInputMask("HHHHHHHH");

    newItem = new QTableWidgetItem;
    newItem->setText("OriginalFirstThunk");
    ui->tableWidgetImportLibrary->setItem(0,0,newItem);
    ui->tableWidgetImportLibrary->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("TimeDateStamp");
    ui->tableWidgetImportLibrary->setItem(1,0,newItem);
    ui->tableWidgetImportLibrary->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("ForwarderChain");
    ui->tableWidgetImportLibrary->setItem(2,0,newItem);
    ui->tableWidgetImportLibrary->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("Name");
    ui->tableWidgetImportLibrary->setItem(3,0,newItem);
    ui->tableWidgetImportLibrary->item(3,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("FirstThunk");
    ui->tableWidgetImportLibrary->setItem(4,0,newItem);
    ui->tableWidgetImportLibrary->item(4,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetImportLibrary->setCellWidget(0,1,lineEditOriginalFirstThunk);
    ui->tableWidgetImportLibrary->setCellWidget(1,1,lineEditTimeDateStamp);
    ui->tableWidgetImportLibrary->setCellWidget(2,1,lineEditForwarderChain);
    ui->tableWidgetImportLibrary->setCellWidget(3,1,lineEditName);
    ui->tableWidgetImportLibrary->setCellWidget(4,1,lineEditFirstThunk);

    connect(lineEditOriginalFirstThunk,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditTimeDateStamp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditForwarderChain,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditName,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditFirstThunk,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    //ui->tableWidgetImportLibrary->resizeColumnsToContents();

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPEImportLibrary::~DialogPEImportLibrary()
{
    delete ui;
}

void DialogPEImportLibrary::setData(__DIE_OPTIONS *pOptions,PEFile *pefile,int nNumber)
{
    this->nNumber=nNumber;

    DialogPEGeneric::setData(pOptions,pefile);
}

void DialogPEImportLibrary::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPEImportLibrary::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditLibrary->setToolTip(tr("Library name"));
    }

    lineEditOriginalFirstThunk->setText(QString("%1").arg(pefile->getImport_OriginalFirstThunk(nNumber),8,16,QChar('0')));
    lineEditTimeDateStamp->setText(QString("%1").arg(pefile->getImport_TimeDateStamp(nNumber),8,16,QChar('0')));
    lineEditForwarderChain->setText(QString("%1").arg(pefile->getImport_ForwarderChain(nNumber),8,16,QChar('0')));
    lineEditName->setText(QString("%1").arg(pefile->getImport_Name(nNumber),8,16,QChar('0')));
    lineEditFirstThunk->setText(QString("%1").arg(pefile->getImport_FirstThunk(nNumber),8,16,QChar('0')));

    ui->lineEditLibrary->setText(pefile->getImport_NameAsString(nNumber));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPEImportLibrary::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    lineEditOriginalFirstThunk->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditTimeDateStamp->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditForwarderChain->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditName->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditFirstThunk->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogPEImportLibrary::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEImportLibrary::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setImport_OriginalFirstThunk(nNumber,lineEditOriginalFirstThunk->text().toUInt(&bTemp,16));
    pefile->setImport_TimeDateStamp(nNumber,lineEditTimeDateStamp->text().toUInt(&bTemp,16));
    pefile->setImport_ForwarderChain(nNumber,lineEditForwarderChain->text().toUInt(&bTemp,16));
    pefile->setImport_Name(nNumber,lineEditName->text().toUInt(&bTemp,16));
    pefile->setImport_FirstThunk(nNumber,lineEditFirstThunk->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPEImportLibrary::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}
