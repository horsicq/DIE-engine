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
#include "dialogmachlc.h"
#include "ui_dialogmachlc.h"

DialogMACHLC::DialogMACHLC(QWidget *parent) :
    DialogMACHGeneric(parent),
    ui(new Ui::DialogMACHLC)
{
    ui->setupUi(this);

    actEditHeader=new QAction(tr("Edit header"), this);
    actHex=new QAction(tr("Hex"), this);

    connect(actEditHeader,SIGNAL(triggered()),this,SLOT(editHeader()));
    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogMACHLC::~DialogMACHLC()
{
    delete ui;
}

bool DialogMACHLC::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    ui->tableWidgetSections->clear();

    QTableWidgetItem *newItem;

    ui->tableWidgetSections->setColumnCount(3);

    QStringList labels;

    labels.append("Type");
    labels.append("Offset");
    labels.append("Size");

    ui->tableWidgetSections->setHorizontalHeaderLabels(labels);

    QList<load_command> listLC=machfile->getLoadCommands();

    ui->tableWidgetSections->setRowCount(listLC.count());

    for(int i=0; i<listLC.count(); i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(machfile->loadCommandTypeToString(listLC.at(i).cmd));
        ui->tableWidgetSections->setItem(i,0,newItem);
        ui->tableWidgetSections->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(machfile->getLoadCommand_offset(i),8,16,QChar('0')));
        ui->tableWidgetSections->setItem(i,1,newItem);
        ui->tableWidgetSections->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listLC.at(i).cmdsize,8,16,QChar('0')));
        ui->tableWidgetSections->setItem(i,2,newItem);
        ui->tableWidgetSections->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetSections->setRowHeight(i,20);
    }

    ui->tableWidgetSections->resizeColumnsToContents();

    return false;
}

void DialogMACHLC::on_pushButtonOK_clicked()
{
    this->close();
}


void DialogMACHLC::on_tableWidgetSections_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetSections->rowCount())
    {
        return;
    }

    QMenu menu;

    //    menu.addAction(actEditHeader);
    menu.addAction(actHex);

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetSections->mapToGlobal(pos2));
}
void DialogMACHLC::editHeader()
{
    //    DialogElfProgramHeader dialogprogramheader(this);

    //    int nProgram=ui->tableWidgetSections->selectedItems().at(0)->row();

    //    dialogprogramheader.setData(elffile,pOptions,nProgram);

    //    dialogprogramheader.exec();
}

void DialogMACHLC::hex()
{
    unsigned int nOffset=0;
    unsigned int nSize=0;
    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    nOffset=machfile->getLoadCommand_offset(nSection);
    nSize=machfile->getLoadCommand_size(nSection);

    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,machfile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Load command(%1)").arg(nSection));

    dialoghde.exec();
}

