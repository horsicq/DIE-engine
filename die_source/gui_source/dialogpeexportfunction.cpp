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
#include "dialogpeexportfunction.h"
#include "ui_dialogpeexportfunction.h"

DialogPEExportFunction::DialogPEExportFunction(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEExportFunction)
{
    ui->setupUi(this);

    ui->lineEditFunctionName->setHex(false);

    QTableWidgetItem *newItem;

    ui->tableWidgetExportFunction->setRowCount(3);
    ui->tableWidgetExportFunction->setColumnCount(2);

    for(int i=0; i<3; i++)
    {
        ui->tableWidgetExportFunction->setRowHeight(i,20);
    }


    lineEditOrdinal=new QLineEditX;
    lineEditRVA=new QLineEditX;
    lineEditName=new QLineEditX;

    lineEditOrdinal->setAlignment(Qt::AlignCenter);
    lineEditRVA->setAlignment(Qt::AlignCenter);
    lineEditName->setAlignment(Qt::AlignCenter);

    lineEditOrdinal->setInputMask("HHHH");
    lineEditRVA->setInputMask("HHHHHHHH");
    lineEditName->setInputMask("HHHHHHHH");

    newItem = new QTableWidgetItem;
    newItem->setText("Ordinal");
    ui->tableWidgetExportFunction->setItem(0,0,newItem);
    ui->tableWidgetExportFunction->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("RVA");
    ui->tableWidgetExportFunction->setItem(1,0,newItem);
    ui->tableWidgetExportFunction->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("Name");
    ui->tableWidgetExportFunction->setItem(2,0,newItem);
    ui->tableWidgetExportFunction->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    ui->tableWidgetExportFunction->setIndexWidget(ui->tableWidgetExportFunction->model()->index(0,1),lineEditOrdinal);
    ui->tableWidgetExportFunction->setIndexWidget(ui->tableWidgetExportFunction->model()->index(1,1),lineEditRVA);
    ui->tableWidgetExportFunction->setIndexWidget(ui->tableWidgetExportFunction->model()->index(2,1),lineEditName);

    connect(lineEditOrdinal,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditRVA,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditName,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPEExportFunction::~DialogPEExportFunction()
{
    delete ui;
}

void DialogPEExportFunction::setData(__DIE_OPTIONS *pOptions,PEFile *pefile,int nNumber)
{
    this->nNumber=nNumber;
    //pefile.setFileName(sFileName);
    DialogPEGeneric::setData(pOptions,pefile);
}


void DialogPEExportFunction::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPEExportFunction::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditFunctionName->setToolTip(tr("Function name"));

    }

    //    unsigned int nOrdinal=pefile->getExport_FunctionNameOrdinal(nNumber);
    unsigned int nOrdinal=pefile->getExport_FunctionNameOrdinal(nNumber);
    unsigned int nBase=pefile->getExport_Base();

    lineEditOrdinal->setText(QString("%1").arg(nOrdinal,4,16,QChar('0')));
    lineEditRVA->setText(QString("%1").arg(pefile->getExport_FunctionAddress(nOrdinal-nBase),8,16,QChar('0')));
    lineEditName->setText(QString("%1").arg(pefile->getExport_FunctionName(nNumber),8,16,QChar('0')));

    ui->lineEditFunctionName->setText(pefile->getExport_FunctionNameAsString(nNumber));

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}


void DialogPEExportFunction::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineEditOrdinal->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditRVA->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditName->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogPEExportFunction::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEExportFunction::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    // TODO!!!

    pefile->setExport_FunctionNameOrdinal(nNumber,lineEditOrdinal->text().toUInt(&bTemp,16));
    pefile->setExport_FunctionAddress(nNumber,lineEditRVA->text().toUInt(&bTemp,16));
    pefile->setExport_FunctionName(nNumber,lineEditName->text().toUInt(&bTemp,16));



    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPEExportFunction::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}
