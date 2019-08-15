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
#include "dialogpebasereloc.h"
#include "ui_dialogpebasereloc.h"

DialogPEBaseReloc::DialogPEBaseReloc(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEBaseReloc)
{
    ui->setupUi(this);

    ui->tableWidgetRelocs->setColumnCount(4);

    QStringList labels;

    labels.append("Address");
    labels.append("Size");
    labels.append("Section");
    labels.append("");

    ui->tableWidgetRelocs->setHorizontalHeaderLabels(labels);

    ui->tableWidgetRelocs->setColumnWidth(0,100);
    ui->tableWidgetRelocs->setColumnWidth(1,125);
    ui->tableWidgetRelocs->setColumnWidth(2,80);
    ui->tableWidgetRelocs->setColumnWidth(3,100);

    ui->tableWidgetRelocPage->setColumnCount(3);
    labels.clear();

    labels.append("Address");
    labels.append("Type");
    labels.append("Value");

    ui->tableWidgetRelocPage->setHorizontalHeaderLabels(labels);

    ui->tableWidgetRelocs->setColumnWidth(0,100);
    ui->tableWidgetRelocs->setColumnWidth(1,125);
    ui->tableWidgetRelocs->setColumnWidth(2,80);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEBaseReloc::~DialogPEBaseReloc()
{
    delete ui;
}

void DialogPEBaseReloc::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogPEBaseReloc::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    QTableWidgetItem *newItem;

    int nNumberOfRelocsPages=pefile->getNumberOfRelocsPages();

    ui->tableWidgetRelocs->setRowCount(nNumberOfRelocsPages);
    unsigned int nRelocsTableOffset=pefile->getRelocsTableOffset();
    unsigned int nAddress;
    unsigned int nSize;

    unsigned int nSection;

    int i=0;

    do
    {
        nAddress=pefile->readDword(nRelocsTableOffset);
        nSize=pefile->readDword(nRelocsTableOffset+4);

        if(nAddress&&nSize)
        {
            listAddresses.append(nAddress);
            listSizes.append(nSize);
            listOffsets.append(nRelocsTableOffset);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(nAddress,8,16,QChar('0')));

            ui->tableWidgetRelocs->setItem(i,0,newItem);
            ui->tableWidgetRelocs->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;
            newItem->setText(QString("%1").arg(nSize,8,16,QChar('0')));

            ui->tableWidgetRelocs->setItem(i,1,newItem);
            ui->tableWidgetRelocs->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;

            nSection=pefile->RVAToSection(nAddress);

            if(nSection!=-1)
            {
                newItem->setText(pefile->getSectionNameAsString(nSection));
            }

            ui->tableWidgetRelocs->setItem(i,2,newItem);
            ui->tableWidgetRelocs->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            newItem = new QTableWidgetItem;

            newItem->setText(QString("%1").arg(nRelocsTableOffset,8,16,QChar('0')));

            ui->tableWidgetRelocs->setItem(i,3,newItem);
            ui->tableWidgetRelocs->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            ui->tableWidgetRelocs->setRowHeight(i,20);
            i++;
            nRelocsTableOffset+=nSize;
        }
    }
    while(nAddress&&nSize);

    return true;
}

void DialogPEBaseReloc::on_tableWidgetRelocs_clicked(const QModelIndex &index)
{
    int nRow=ui->tableWidgetRelocs->selectedItems().at(0)->row();
    //    unsigned int nAddress=ui->tableWidgetRelocs->item(nRow,0)->text().toUInt();
    //    unsigned int nSize=ui->tableWidgetRelocs->item(nRow,1)->text().toUInt();
    //    unsigned int nOffset=ui->tableWidgetRelocs->item(nRow,3)->text().toUInt();

    if((nRow>=0)&&(nRow<listAddresses.count()))
    {
        unsigned int nAddress=listAddresses.at(nRow);
        unsigned int nSize=listSizes.at(nRow);
        unsigned int nOffset=listOffsets.at(nRow);
        QLineEditX *lineEdit;
        unsigned short sTypeOffset;
        QTableWidgetItem *newItem;

        nOffset+=8;

        ui->tableWidgetRelocPage->setRowCount(0);
        ui->tableWidgetRelocPage->setRowCount((nSize-8)/2);

        for(int i=0; (unsigned int)i<(nSize-8)/2; i++)
        {
            sTypeOffset=pefile->readWord(nOffset);

            lineEdit=new QLineEditX;
            lineEdit->setReadOnly(true);
            lineEdit->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            lineEdit->setText(QString("%1").arg(nAddress+(sTypeOffset&0x0FFF),8,16,QChar('0')));
            ui->tableWidgetRelocPage->setIndexWidget(ui->tableWidgetRelocPage->model()->index(i,0),lineEdit);

            newItem = new QTableWidgetItem;

            if((sTypeOffset&0xF000)==0)
            {
                newItem->setText("ABSOLUTE");
            }
            else if((sTypeOffset&0xF000)==0x3000)
            {
                newItem->setText("HIGHLOW");
            }
            else if((sTypeOffset&0xF000)==0xA000)
            {
                newItem->setText("DIR64");
            }

            ui->tableWidgetRelocPage->setItem(i,1,newItem);
            ui->tableWidgetRelocPage->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            lineEdit=new QLineEditX;
            lineEdit->setReadOnly(true);
            lineEdit->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            if((sTypeOffset&0xF000))
            {
                lineEdit->setText(QString("%1").arg(pefile->readDword(pefile->RVAToOffset(nAddress+(sTypeOffset&0x0FFF))),8,16,QChar('0')));
            }

            ui->tableWidgetRelocPage->setIndexWidget(ui->tableWidgetRelocPage->model()->index(i,2),lineEdit);

            ui->tableWidgetRelocPage->setRowHeight(i,20);

            nOffset+=2;
        }
    }
}
