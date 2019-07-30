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
#include "dialogpemetadatastrings.h"
#include "ui_dialogpemetadatastrings.h"

DialogPEMetaDataStrings::DialogPEMetaDataStrings(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEMetaDataStrings)
{
    ui->setupUi(this);

    actCopyAsString=new QAction("", this);

    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));

    QHeaderView *verticalHeader = ui->tableWidgetStrings->verticalHeader();
#if QT_VERSION_MAJOR >= 0x05
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
#else
    verticalHeader->setResizeMode(QHeaderView::Fixed);
#endif

    verticalHeader->setDefaultSectionSize(20);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEMetaDataStrings::~DialogPEMetaDataStrings()
{
    delete ui;
}

void DialogPEMetaDataStrings::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogPEMetaDataStrings::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    //    QTableWidgetItem *newItem;
    //    pefile->setFileName(sFileName);
    //    ui->tableWidgetStrings->clear();
    //    ui->tableWidgetStrings->setRowCount(listStrings.count());
    //    ui->tableWidgetStrings->setColumnCount(2);
    //    ui->tableWidgetStrings->setHorizontalHeaderLabels(labels);
    //    ui->tableWidgetStrings->setSortingEnabled(true);

    QList<QString> listStrings=pefile->getCLI_MetaData_StringsAsList();

    QStandardItem *newItem;
    model=new QStandardItemModel(listStrings.count(), 2);
    model->setHeaderData(0,Qt::Horizontal,"Nr");
    model->setHeaderData(1,Qt::Horizontal,"String");

    for(int i = 0; i<listStrings.count(); i++)
    {
        newItem = new QStandardItem;
        newItem->setData(i,Qt::DisplayRole);
        //        newItem->setText(QString("%1").arg(i));
        model->setItem(i,0,newItem);
        newItem = new QStandardItem;
        newItem->setText(listStrings.at(i));
        model->setItem(i,1,newItem);
    }

    ui->tableWidgetStrings->setModel(model);

    //    for(int i=0;i<listStrings.count();i++)
    //    {
    //        newItem = new QTableWidgetItem;
    //        newItem->setData(Qt::DisplayRole,i);
    //        ui->tableWidgetStrings->setItem(i,0,newItem);
    //        ui->tableWidgetStrings->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    //        newItem = new QTableWidgetItem;
    //        newItem->setText(listStrings.at(i));
    //        ui->tableWidgetStrings->setItem(i,1,newItem);
    //        ui->tableWidgetStrings->item(i,1)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    //        ui->tableWidgetStrings->setRowHeight(i,20);
    //    }
    ui->tableWidgetStrings->setColumnWidth(0,50);
    ui->tableWidgetStrings->setColumnWidth(1,250);

    return true;
}

void DialogPEMetaDataStrings::copyAsString()
{
    //    QClipboard *clipboard = QApplication::clipboard();
    //    clipboard->setText(QString("%1").arg(ui->tableWidgetStrings->selectedItems().at(0)->text()));

    QModelIndex index=ui->tableWidgetStrings->currentIndex();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(index.model()->data(index).toString()));
}

void DialogPEMetaDataStrings::on_tableWidgetStrings_customContextMenuRequested(const QPoint &pos)
{
    //    if(!ui->tableWidgetStrings->selectedItems().count())
    //    {
    //        return;
    //    }

    //    int nDebug=ui->tableWidgetStrings->selectedItems().at(0)->row();


    //    QMenu menu;

    //    menu.addAction(actCopyAsString);
    //    actCopyAsString->setText(QString("Copy as \"%1\"").arg(ui->tableWidgetStrings->selectedItems().at(0)->text()));

    //    QPoint pos2=pos;
    //    pos2.setY(pos2.ry()+20);
    //    menu.exec(ui->tableWidgetStrings->mapToGlobal(pos2));

    if(!ui->tableWidgetStrings->currentIndex().isValid())
    {
        return;
    }

    QModelIndex index=ui->tableWidgetStrings->currentIndex();
    //     QString sString=((QTableWidgetItem *)(ui->tableWidgetSections->focusWidget()))->text();
    //     qDebug(sString.toAscii().data());
    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(index.model()->data(index).toString()));

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetStrings->mapToGlobal(pos2));
}
