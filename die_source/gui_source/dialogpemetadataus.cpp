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
#include "dialogpemetadataus.h"
#include "ui_dialogpemetadataus.h"

DialogPEMetaDataUS::DialogPEMetaDataUS(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEMetaDataUS)
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

DialogPEMetaDataUS::~DialogPEMetaDataUS()
{
    delete ui;
}

void DialogPEMetaDataUS::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogPEMetaDataUS::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    //    ui->tableWidgetStrings->clear();
    QList<QString> listStrings=pefile->getCLI_MetaData_USAsList();
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

    ui->tableWidgetStrings->setColumnWidth(0,50);
    ui->tableWidgetStrings->setColumnWidth(1,250);

    return true;
}

void DialogPEMetaDataUS::copyAsString()
{
    QModelIndex index=ui->tableWidgetStrings->currentIndex();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(index.model()->data(index).toString()));
}

void DialogPEMetaDataUS::on_tableWidgetStrings_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetStrings->currentIndex().isValid())
    {
        return;
    }

    QModelIndex index=ui->tableWidgetStrings->currentIndex();
    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(index.model()->data(index).toString()));

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetStrings->mapToGlobal(pos2));
}
