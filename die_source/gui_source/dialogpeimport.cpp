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
#include "dialogpeimport.h"
#include "ui_dialogpeimport.h"

DialogPEImport::DialogPEImport(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEImport)
{
    ui->setupUi(this);

    ui->tableWidgetLibraries->setColumnCount(6);

    QStringList labels;

    labels.append("Dll Name");
    labels.append("OriginalFirstThunk");
    labels.append("TimeDateStamp");
    labels.append("ForwarderChain");
    labels.append("Name");
    labels.append("FirstThunk");

    ui->tableWidgetLibraries->setHorizontalHeaderLabels(labels);

    ui->tableWidgetLibraries->setColumnWidth(0,100);
    ui->tableWidgetLibraries->setColumnWidth(1,125);
    ui->tableWidgetLibraries->setColumnWidth(2,125);
    ui->tableWidgetLibraries->setColumnWidth(3,125);
    ui->tableWidgetLibraries->setColumnWidth(4,100);
    ui->tableWidgetLibraries->setColumnWidth(5,100);

    ui->tableWidgetFunctions->setColumnCount(4);

    //ui->tableWidgetFunctions->resizeColumnsToContents();

    labels.clear();

    labels.append("Thunk");
    labels.append("Ordinal");
    labels.append("Hint");
    labels.append("Name");

    ui->tableWidgetFunctions->setHorizontalHeaderLabels(labels);

    ui->tableWidgetFunctions->setColumnWidth(0,115);
    ui->tableWidgetFunctions->setColumnWidth(1,115);
    ui->tableWidgetFunctions->setColumnWidth(2,45);
    ui->tableWidgetFunctions->setColumnWidth(3,390);


    actEditFunction=new QAction(tr("Edit"), this);
    actEditLibrary=new QAction(tr("Edit"), this);

    actCopyAsStringFunction=new QAction("", this);
    actCopyAsStringLibrary=new QAction("", this);

    connect(actCopyAsStringFunction,SIGNAL(triggered()),this,SLOT(copyAsStringFunction()));
    connect(actCopyAsStringLibrary,SIGNAL(triggered()),this,SLOT(copyAsStringLibrary()));

    connect(actEditFunction,SIGNAL(triggered()),this,SLOT(editFunction()));
    connect(actEditLibrary,SIGNAL(triggered()),this,SLOT(editLibrary()));



    //    timer.setInterval(1000);
    //    connect(&timer,SIGNAL(timeout()),this,SLOT(setTableEnable()));

    bFirstTime=true;
    nRawLast=0;

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));

    bSync=false;

}

DialogPEImport::~DialogPEImport()
{
    emit reset();

    delete ui;
}


void DialogPEImport::on_pushButtonOK_clicked()
{
    this->close();
}

bool DialogPEImport::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->progressBar->setToolTip(tr("Import progress"));
    }

    bSync=true;

    scan= new ThreadImport;

    QThread* thread =new QThread;
    scan->moveToThread(thread);

    scan->setData(pefile,&listLibraries);

    connect(this, SIGNAL(reset()), scan, SLOT(_stop()),Qt::DirectConnection);
    connect(thread, SIGNAL(started()), scan, SLOT(process()));
    connect(scan, SIGNAL(_finished()), thread, SLOT(quit()));
    connect(scan, SIGNAL(_finished()), this, SLOT(createTable()));
    connect(scan, SIGNAL(_finished()), scan, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(scan,SIGNAL(setProgressBar(int,int)),this,SLOT(setProgressBar(int,int)));

    connect(scan, SIGNAL(_started()), this, SLOT(disableControls()));
    connect(scan, SIGNAL(_finished()), this, SLOT(enableControls()));

    thread->start(QThread::LowestPriority);



    //    if(nNumberOfImports)
    //    {
    //        ui->tableWidgetLibraries->setCurrentCell(0,0);
    //        QModelIndex index;
    //        _on_tableWidgetLibraries_clicked(index);
    //    }

    return true;
}

void DialogPEImport::editLibrary()
{
    DialogPEImportLibrary dialogpeimportlibrary(this);

    int nNumber=ui->tableWidgetLibraries->selectedItems().at(0)->row();

    dialogpeimportlibrary.setData(pOptions,pefile,nNumber);

    dialogpeimportlibrary.exec();
}

void DialogPEImport::editFunction()
{
    DialogPEImportFunction dialogpeimportfunction(this);

    int nNumber=ui->tableWidgetLibraries->selectedItems().at(0)->row();
    int nFunctionNumber=ui->tableWidgetFunctions->selectedItems().at(0)->row();

    dialogpeimportfunction.setData(pOptions,pefile,nNumber,nFunctionNumber);

    dialogpeimportfunction.exec();
}

void DialogPEImport::on_tableWidgetLibraries_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetLibraries->selectedItems().count())
    {
        return;
    }

    QMenu menu;

    menu.addAction(actCopyAsStringLibrary);
    actCopyAsStringLibrary->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetLibraries->selectedItems().at(0)->text()));

    menu.addSeparator();
    menu.addAction(actEditLibrary);

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetLibraries->mapToGlobal(pos2));
}

void DialogPEImport::on_tableWidgetFunctions_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetFunctions->selectedItems().count())
    {
        return;
    }

    QMenu menu;

    menu.addAction(actCopyAsStringFunction);
    actCopyAsStringFunction->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetFunctions->selectedItems().at(0)->text()));

    menu.addSeparator();

    menu.addAction(actEditFunction);

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetFunctions->mapToGlobal(pos2));
}
void DialogPEImport::setProgressBar(int nMax, int nValue)
{
    ui->progressBar->setMaximum(nMax);
    ui->progressBar->setValue(nValue);
}
void DialogPEImport::copyAsStringLibrary()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetLibraries->selectedItems().at(0)->text()));
}
void DialogPEImport::copyAsStringFunction()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetFunctions->selectedItems().at(0)->text()));
}

void DialogPEImport::enableControls()
{
    ui->tableWidgetLibraries->setEnabled(true);
    ui->tableWidgetFunctions->setEnabled(true);
}

void DialogPEImport::disableControls()
{
    ui->tableWidgetLibraries->setEnabled(false);
    ui->tableWidgetFunctions->setEnabled(false);
}
//void DialogPEImport::setTableEnable()
//{
//    ui->tableWidgetLibraries->setEnabled(true);
////    timer.stop();
//}

void DialogPEImport::createTable()
{
    QTableWidgetItem *newItem;
    int nNumberOfImports=listLibraries.count();

    ui->tableWidgetLibraries->setRowCount(nNumberOfImports);

    for(int i=0; i<nNumberOfImports; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(listLibraries.at(i).sName);
        ui->tableWidgetLibraries->setItem(i,0,newItem);
        ui->tableWidgetLibraries->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listLibraries.at(i).nOriginalFirstThunk,8,16,QChar('0')));
        ui->tableWidgetLibraries->setItem(i,1,newItem);
        ui->tableWidgetLibraries->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listLibraries.at(i).nTimeDateStamp,8,16,QChar('0')));
        ui->tableWidgetLibraries->setItem(i,2,newItem);
        ui->tableWidgetLibraries->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listLibraries.at(i).nForwarderChain,8,16,QChar('0')));
        ui->tableWidgetLibraries->setItem(i,3,newItem);
        ui->tableWidgetLibraries->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listLibraries.at(i).nName,8,16,QChar('0')));
        ui->tableWidgetLibraries->setItem(i,4,newItem);
        ui->tableWidgetLibraries->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(listLibraries.at(i).nFirstThunk,8,16,QChar('0')));
        ui->tableWidgetLibraries->setItem(i,5,newItem);
        ui->tableWidgetLibraries->item(i,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetLibraries->setRowHeight(i,20);
    }

    bSync=false;

    if(nNumberOfImports)
    {
        ui->tableWidgetLibraries->setCurrentCell(0,0);
    }
}




void DialogPEImport::on_tableWidgetLibraries_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(!bSync)
    {
        int row=currentRow;

        QTableWidgetItem *newItem;
        int nNumberOfFunctions=listLibraries.at(row).listFunctions.count();

        ui->tableWidgetFunctions->setRowCount(nNumberOfFunctions);

        for(int i=0; i<nNumberOfFunctions; i++)
        {
            newItem = new QTableWidgetItem;
            newItem->setText(listLibraries.at(row).listFunctions.at(i).sThunk);
            ui->tableWidgetFunctions->setItem(i,0,newItem);
            ui->tableWidgetFunctions->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(listLibraries.at(row).listFunctions.at(i).sOrdinal);
            ui->tableWidgetFunctions->setItem(i,1,newItem);
            ui->tableWidgetFunctions->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(listLibraries.at(row).listFunctions.at(i).sHint);
            ui->tableWidgetFunctions->setItem(i,2,newItem);
            ui->tableWidgetFunctions->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newItem = new QTableWidgetItem;
            newItem->setText(listLibraries.at(row).listFunctions.at(i).sName);
            ui->tableWidgetFunctions->setItem(i,3,newItem);
            ui->tableWidgetFunctions->item(i,3)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

            ui->tableWidgetFunctions->setRowHeight(i,20);
        }
    }
}
