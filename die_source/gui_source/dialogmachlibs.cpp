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
#include "dialogmachlibs.h"
#include "ui_dialogmachlibs.h"

DialogMACHLibs::DialogMACHLibs(QWidget *parent) :
    DialogMACHGeneric(parent),
    ui(new Ui::DialogMACHLibs)
{
    ui->setupUi(this);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));

    actCopyAsString=new QAction("", this);

    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));
}

DialogMACHLibs::~DialogMACHLibs()
{
    delete ui;
}

void DialogMACHLibs::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogMACHLibs::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    QTableWidgetItem *newItem;
    //    pefile->setFileName(sFileName);

    ui->tableWidgetStrings->clear();

    QList<DYLIB_FULL> listStrings=machfile->getLibs();
    ui->tableWidgetStrings->setRowCount(listStrings.count());
    ui->tableWidgetStrings->setColumnCount(5);

    QStringList labels;

    labels.append("timestamp");
    labels.append("Cur.Ver");
    labels.append("Comp.Ver");
    labels.append("Path");
    labels.append("Name");

    ui->tableWidgetStrings->setHorizontalHeaderLabels(labels);

    for(int i=0; i<listStrings.count(); i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listStrings.at(i).timestamp,8,16,QChar('0')));
        ui->tableWidgetStrings->setItem(i,0,newItem);
        ui->tableWidgetStrings->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listStrings.at(i).current_version,8,16,QChar('0')));
        ui->tableWidgetStrings->setItem(i,1,newItem);
        ui->tableWidgetStrings->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listStrings.at(i).compatibility_version,8,16,QChar('0')));
        ui->tableWidgetStrings->setItem(i,2,newItem);
        ui->tableWidgetStrings->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        newItem = new QTableWidgetItem;
        newItem->setText(listStrings.at(i).sVollName);
        ui->tableWidgetStrings->setItem(i,3,newItem);
        ui->tableWidgetStrings->item(i,3)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        newItem = new QTableWidgetItem;
        newItem->setText(listStrings.at(i).sShortName);
        ui->tableWidgetStrings->setItem(i,4,newItem);
        ui->tableWidgetStrings->item(i,4)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        ui->tableWidgetStrings->setRowHeight(i,20);
    }

    ui->tableWidgetStrings->setColumnWidth(0,80);
    ui->tableWidgetStrings->setColumnWidth(1,80);
    ui->tableWidgetStrings->setColumnWidth(2,80);
    ui->tableWidgetStrings->setColumnWidth(3,300);
    ui->tableWidgetStrings->setColumnWidth(4,100);

    return true;
}

void DialogMACHLibs::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetStrings->selectedItems().at(0)->text()));
}

void DialogMACHLibs::on_tableWidgetStrings_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetStrings->selectedItems().count())
    {
        return;
    }

//    int nDebug=ui->tableWidgetStrings->selectedItems().at(0)->row();

    //     QString sString=((QTableWidgetItem *)(ui->tableWidgetSections->focusWidget()))->text();
    //     qDebug(sString.toAscii().data());

    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetStrings->selectedItems().at(0)->text()));

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetStrings->mapToGlobal(pos2));
}
