// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#include "dialogelfstrings.h"
#include "ui_dialogelfstrings.h"

DialogELFStrings::DialogELFStrings(QWidget *parent) :
    DialogELFGeneric(parent),
    ui(new Ui::DialogELFStrings)
{
    ui->setupUi(this);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));

    actCopyAsString=new QAction("", this);

    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));
}

void DialogELFStrings::setData(__DIE_OPTIONS *pOptions,ELFFile *elffile,unsigned int nSection)
{
    this->nSection=nSection;

    DialogELFGeneric::setData(pOptions,elffile);
}

DialogELFStrings::~DialogELFStrings()
{
    delete ui;
}

void DialogELFStrings::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogELFStrings::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    QTableWidgetItem *newItem;
    //    pefile->setFileName(sFileName);

    ui->tableWidgetStrings->clear();

    QList<ELFFile::STRING_TABLE_RECORD> listStrings=elffile->getStringTableAsList(nSection);
    ui->tableWidgetStrings->setRowCount(listStrings.count());

    QStringList labels;

    labels.append("Index");
    labels.append("String");

    ui->tableWidgetStrings->setHorizontalHeaderLabels(labels);

    ui->tableWidgetStrings->setColumnCount(2);
    ui->tableWidgetStrings->setSortingEnabled(true);

    for(int i=0; i<listStrings.count(); i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setData(Qt::DisplayRole,listStrings.at(i).nIndex);
        ui->tableWidgetStrings->setItem(i,0,newItem);
        ui->tableWidgetStrings->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(listStrings.at(i).sString);
        ui->tableWidgetStrings->setItem(i,1,newItem);
        ui->tableWidgetStrings->item(i,1)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        ui->tableWidgetStrings->setRowHeight(i,20);
    }

    ui->tableWidgetStrings->setColumnWidth(0,50);
    ui->tableWidgetStrings->setColumnWidth(1,250);

    return true;
}

void DialogELFStrings::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetStrings->selectedItems().at(0)->text()));
}

void DialogELFStrings::on_tableWidgetStrings_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetStrings->selectedItems().count())
    {
        return;
    }

    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetStrings->selectedItems().at(0)->text()));

    menu.exec(ui->tableWidgetStrings->viewport()->mapToGlobal(pos));
}
