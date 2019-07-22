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
#include "dialogpemetadatatables.h"
#include "ui_dialogpemetadatatables.h"

DialogPEMetaDataTables::DialogPEMetaDataTables(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEMetaDataTables)
{
    ui->setupUi(this);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEMetaDataTables::~DialogPEMetaDataTables()
{
    delete ui;
}

void DialogPEMetaDataTables::on_pushButtonOK_clicked()
{
    this->close();
}
bool DialogPEMetaDataTables::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->checkBoxBlob->setToolTip(tr("Size of \"#Blob\" stream >= 2^16"));
        ui->checkBoxGUID->setToolTip(tr("Size of \"#GUID\" stream >= 2^16"));
        ui->checkBoxString->setToolTip(tr("Size of \"#String\" stream >= 2^16"));
        ui->lineEditHeapOffsetSizes->setToolTip(tr("The HeapSizes field is a bit vector that encodes how wide indexes into the various heaps are."));
        ui->lineEditMajorVersion->setToolTip(tr("MajorVersion"));
        ui->lineEditMinorVersion->setToolTip(tr("MinorVersion"));
        ui->lineEditReserved1->setToolTip(tr("Reserved: The first unsigned int is always 0."));
        ui->lineEditReserved2->setToolTip(tr("Reserved: BYTE, always 1"));
        ui->lineEditSorted->setToolTip(tr("Sorted: A bitmask-QWORD. It tells us which tables are sorted."));
        ui->lineEditValid->setToolTip(tr("Valid: It's a bitmask-QWORD that tells us which MetaData Tables are present in the assembly"));
    }

    if(pefile->isNETPresent())
    {
        ui->lineEditHeapOffsetSizes->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_HeapOffsetSizes(),2,16,QChar('0')));
        ui->lineEditMajorVersion->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_MajorVersion(),2,16,QChar('0')));
        ui->lineEditMinorVersion->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_MinorVersion(),2,16,QChar('0')));
        ui->lineEditReserved1->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_Reserved1(),8,16,QChar('0')));
        ui->lineEditReserved2->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_Reserved2(),2,16,QChar('0')));
        ui->lineEditSorted->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_Sorted(),16,16,QChar('0')));
        ui->lineEditValid->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_Valid(),16,16,QChar('0')));

        ui->tableWidgetExtra->setRowCount(pefile->getCLI_MetaData_Tables_Valid_NumberOfRows());
        ui->tableWidgetExtra->setColumnCount(4);

        QStringList labels;

        labels.append("Num");
        labels.append("Name");
        labels.append("Rows");
        labels.append("Is sorted");

        ui->tableWidgetExtra->setHorizontalHeaderLabels(labels);

        QTableWidgetItem *newItem;

        for(int i=0,j=0; i<64; i++)
        {
            if(pefile->isCLI_MetaData_Tables_TablePresent(i))
            {
                newItem = new QTableWidgetItem;
                newItem->setText(QString("%1").arg(i));
                ui->tableWidgetExtra->setItem(j,0,newItem);
                ui->tableWidgetExtra->item(j,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                newItem = new QTableWidgetItem;
                newItem->setText(pefile->CLI_MetaData_Tables_TableToString(i));
                ui->tableWidgetExtra->setItem(j,1,newItem);
                ui->tableWidgetExtra->item(j,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                newItem = new QTableWidgetItem;
                newItem->setText(QString("%1").arg(pefile->getCLI_MetaData_Tables_TablesNumberOfIndexes(i),8,16,QChar('0')));
                ui->tableWidgetExtra->setItem(j,2,newItem);
                ui->tableWidgetExtra->item(j,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                newItem = new QTableWidgetItem;

                if(pefile->isCLI_MetaData_Tables_TableSorted(i))
                {
                    newItem->setText(tr("yes"));
                }
                else
                {
                    newItem->setText(tr("no"));
                }

                ui->tableWidgetExtra->setItem(j,3,newItem);
                ui->tableWidgetExtra->item(j,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                ui->tableWidgetExtra->setRowHeight(j,20);
                j++;
            }
        }

        ui->tableWidgetExtra->resizeColumnsToContents();
    }

    return true;
}

void DialogPEMetaDataTables::on_lineEditHeapOffsetSizes_textChanged(const QString &arg1)
{
    bool bOK;
    unsigned int nValue=arg1.toUInt(&bOK,16);

    ui->checkBoxString->setChecked(nValue&0x01);
    ui->checkBoxGUID->setChecked(nValue&0x02);
    ui->checkBoxBlob->setChecked(nValue&0x04);
}
