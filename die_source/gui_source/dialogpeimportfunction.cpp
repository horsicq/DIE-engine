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
#include "dialogpeimportfunction.h"
#include "ui_dialogpeimportfunction.h"

DialogPEImportFunction::DialogPEImportFunction(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEImportFunction)
{
    ui->setupUi(this);

    ui->lineEditFunctionName->setHex(false);

    ui->tableWidgetImportFunction->setRowCount(3);
    ui->tableWidgetImportFunction->setColumnCount(2);

    for(int i=0; i<3; i++)
    {
        ui->tableWidgetImportFunction->setRowHeight(i,20);
    }

    QTableWidgetItem *newItem;

    lineEditThunk=new QLineEditX;
    lineEditOrdinal=new QLineEditX;
    lineEditHint=new QLineEditX;

    lineEditThunk->setAlignment(Qt::AlignCenter);
    lineEditOrdinal->setAlignment(Qt::AlignCenter);
    lineEditHint->setAlignment(Qt::AlignCenter);

    lineEditThunk->setInputMask("HHHHHHHH");
    lineEditOrdinal->setInputMask("HHHHHHHH");
    lineEditHint->setInputMask("HHHH");

    newItem = new QTableWidgetItem;
    newItem->setText("Thunk");
    ui->tableWidgetImportFunction->setItem(0,0,newItem);
    ui->tableWidgetImportFunction->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("Ordinal");
    ui->tableWidgetImportFunction->setItem(1,0,newItem);
    ui->tableWidgetImportFunction->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    newItem = new QTableWidgetItem;
    newItem->setText("Hint");
    ui->tableWidgetImportFunction->setItem(2,0,newItem);
    ui->tableWidgetImportFunction->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetImportFunction->setCellWidget(0,1,lineEditThunk);
    ui->tableWidgetImportFunction->setCellWidget(1,1,lineEditOrdinal);
    ui->tableWidgetImportFunction->setCellWidget(2,1,lineEditHint);

    connect(lineEditThunk,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditOrdinal,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditHint,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    ui->tableWidgetImportFunction->setColumnWidth(0,115);
    ui->tableWidgetImportFunction->setColumnWidth(1,115);

    ui->checkBoxReadOnly->setChecked(true);

    lineEditOrdinal->setReadOnly(true);
    lineEditHint->setReadOnly(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPEImportFunction::~DialogPEImportFunction()
{
    delete ui;
}

void DialogPEImportFunction::setData(__DIE_OPTIONS *pOptions,PEFile *pefile,int nNumber,int nFunctionNumber)
{
    this->nNumber=nNumber;
    this->nFunctionNumber=nFunctionNumber;

    DialogPEGeneric::setData(pOptions,pefile);
}


void DialogPEImportFunction::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPEImportFunction::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditFunctionName->setToolTip(tr("Function name"));
    }

    if(pefile->isPEPlus())
    {
        lineEditThunk->setInputMask("HHHHHHHHHHHHHHHH");
        unsigned long long nThunk64=pefile->getImportThunk64(nNumber,nFunctionNumber);

        lineEditThunk->setText(QString("%1").arg(nThunk64,16,16,QChar('0')));

        if(nThunk64&0x8000000000000000)
        {
            lineEditOrdinal->setText(QString("%1").arg(nThunk64&0x7FFFFFFFFFFFFFFF,16,16,QChar('0')));
            lineEditHint->setText("");
            ui->lineEditFunctionName->setText("");
        }
        else
        {
            lineEditHint->setText(QString("%1").arg(pefile->getImportHint(nThunk64),4,16,QChar('0')));
            ui->lineEditFunctionName->setText(pefile->getImportFunctionName(nNumber,nFunctionNumber));
        }
    }
    else
    {
        lineEditThunk->setInputMask("HHHHHHHH");
        unsigned int nThunk=pefile->getImportThunk(nNumber,nFunctionNumber);

        lineEditThunk->setText(QString("%1").arg(nThunk,8,16,QChar('0')));

        if(nThunk&0x80000000)
        {
            lineEditOrdinal->setText(QString("%1").arg(nThunk&0x7FFFFFFF,8,16,QChar('0')));
            lineEditHint->setText("");
            ui->lineEditFunctionName->setText("");
        }
        else
        {
            lineEditHint->setText(QString("%1").arg(pefile->getImportHint(nThunk),4,16,QChar('0')));
            ui->lineEditFunctionName->setText(pefile->getImportFunctionName(nNumber,nFunctionNumber));
        }
    }
    //    ui->tableWidgetFunctions->setColumnWidth(0,115);
    //    ui->tableWidgetFunctions->setColumnWidth(1,115);

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}


void DialogPEImportFunction::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineEditThunk->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogPEImportFunction::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEImportFunction::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    if(pefile->isPEPlus())
    {
        pefile->setImportThunk64(nNumber,nFunctionNumber,lineEditThunk->text().toULongLong(&bTemp,16));
    }
    else
    {
        pefile->setImportThunk(nNumber,nFunctionNumber,lineEditThunk->text().toUInt(&bTemp,16));
    }

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPEImportFunction::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}
