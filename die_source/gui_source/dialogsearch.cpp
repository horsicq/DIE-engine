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
#include "dialogsearch.h"
#include "ui_dialogsearch.h"

DialogSearch::DialogSearch(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogSearch)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);

    //    setProgressBar(1,1);

    ui->listWidgetMods->setCurrentRow(0);

    QHeaderView *verticalHeader = ui->tableViewANSI->verticalHeader();
#if QT_VERSION_MAJOR >= 0x05
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
#else
    verticalHeader->setResizeMode(QHeaderView::Fixed);
#endif
    verticalHeader->setDefaultSectionSize(20);

    actHex=new QAction(tr("Hex"), this);
    actCopyAsString=new QAction("", this);

    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));

    ui->comboBoxEndianness->addItem("Little Endian");
    ui->comboBoxEndianness->addItem("Big Endian");

    filter=new QSortFilterProxyModel(this);
}

DialogSearch::~DialogSearch()
{
    emit reset();

    delete ui;
}

void DialogSearch::setData(__DIE_OPTIONS *pOptions,Binary *_binary, unsigned int nOffset, unsigned int nSize)
{
    this->_binary=_binary;
    this->nOffset=nOffset;
    this->nSize=nSize;

    ui->lineEditOffset->setText(QString("%1").arg(nOffset));
    ui->lineEditSize->setText(QString("%1").arg(nSize));

    cryptoSignatures=ThreadSearch::loadCryptoSignatures(Utils::getSearchPath(pOptions)+QDir::separator()+"crypto.db");


    setOptions(pOptions);
}

void DialogSearch::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogSearch::on_pushButtonSearch_clicked()
{
    reload();
    search();
}


bool DialogSearch::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->pushButtonSearch->setToolTip(tr("Search"));
    }

    bool bTemp;
    QString sTemp;
    unsigned nFileSize=_binary->size();

    sTemp=ui->lineEditOffset->text();

    if(sTemp.contains(QRegExp("^0x")))
    {
        nOffset=sTemp.toInt(&bTemp,16);
    }
    else
    {
        nOffset=sTemp.toInt();
    }

    sTemp=ui->lineEditSize->text();

    if(sTemp.contains(QRegExp("^0x")))
    {
        nSize=sTemp.toInt(&bTemp,16);
    }
    else
    {
        nSize=sTemp.toInt();
    }

    if((int)nOffset>=nFileSize)
    {
        nOffset=0;
    }

    if(nSize>=nFileSize-nOffset)
    {
        nSize=0;
    }

    if(nSize==0)
    {
        nSize=nFileSize-nOffset;
    }

    ui->lineEditOffset->setText(QString("%1").arg(nOffset));
    ui->lineEditSize->setText(QString("%1").arg(nSize));

    return true;
}

void DialogSearch::appendRow(QString sName, unsigned long long nOffset, unsigned long long nSize)
{
    searchTable.addRow(sName,nOffset,nSize);
    //    if(!sName.contains(QChar('%')))
    //    {
    //        qDebug(sName.toAscii().data());
    //    }
}

void DialogSearch::search()
{
    ui->lineEditFilter->clear();
    searchTable.clear();
    ui->tableViewANSI->setModel(0);

    scan= new ThreadSearch;

    QFutureWatcher<QString> *watcher=new QFutureWatcher<QString>;

    QThread* thread =new QThread;
    scan->moveToThread(thread);

    QProgressDialog dialog;
    dialog.setMinimumSize(400,100);

    dialog.setWindowTitle(tr("Searching..."));

    //    connect(&dialog, SIGNAL(canceled()), watcher, SLOT(cancel()),Qt::DirectConnection);
    connect(&dialog, SIGNAL(canceled()), scan, SLOT(_stop()),Qt::DirectConnection);

    //    connect(this, SIGNAL(reset()), scan, SLOT(_stop()),Qt::DirectConnection);
    connect(thread, SIGNAL(started()), scan, SLOT(process()));
    connect(scan, SIGNAL(_finished()), thread, SLOT(quit()));
    connect(scan, SIGNAL(_finished()), scan, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(scan, SIGNAL(_finished()), this, SLOT(_on_finished()));
    connect(scan, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));
    connect(scan, SIGNAL(_finished()), &dialog, SLOT(reset()));

    connect(scan, SIGNAL(foundLimit(int)), this, SLOT(foundLimit(int)));

    connect(scan, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    connect(scan, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    connect(watcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    connect(watcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
    connect(watcher, SIGNAL(progressTextChanged(QString)), &dialog, SLOT(setLabelText(QString)));
    connect(watcher, SIGNAL(finished()), watcher, SLOT(deleteLater()));

    //    connect(scan,SIGNAL(setProgressBar(int,int)),this,SLOT(setProgressBar(int,int)));
    connect(scan,SIGNAL(appendRow(QString,unsigned long long,unsigned long long)),this,SLOT(appendRow(QString,unsigned long long,unsigned long long)),Qt::DirectConnection);

    searchdata.binary=_binary;
    searchdata.sType=ui->listWidgetMods->currentItem()->text();
    searchdata.nOffset=nOffset;
    searchdata.nSize=nSize;
    searchdata.nFlags=ui->comboBoxEndianness->currentIndex();
    searchdata.pCryptoSignatures=&cryptoSignatures;
    searchdata.watcher=watcher;
    searchdata.nMinLength=ui->spinBoxMinLength->value();
    //    scan->setData(_binary,ui->listWidgetMods->currentItem()->text(),nOffset,nSize,ui->comboBoxEndianness->currentIndex(),&cryptoSignatures,watcher);
    scan->setData(&searchdata);
    thread->start();
    dialog.exec();

    ui->pushButtonSearch->setEnabled(false);
    timerHeuristic.singleShot(1000,this,SLOT(searchEnable()));
}

void DialogSearch::_on_finished()
{
    //filter=new QSortFilterProxyModel;
    filter->setSourceModel(&searchTable);

    ui->tableViewANSI->setModel(filter);
    ui->tableViewANSI->setColumnWidth(0,350);
    ui->tableViewANSI->setColumnWidth(1,62);
    ui->tableViewANSI->setColumnWidth(2,62);

    ui->tableViewANSI->sortByColumn(1,Qt::AscendingOrder);

    //    ui->pushButtonSearch->setText("Search");

    connect(ui->tableViewANSI->selectionModel(),SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),SLOT(_selectedChanged()));

    if(searchTable.rowCount())
    {
        ui->pushButtonSaveResult->setEnabled(true);
    }
}

void DialogSearch::on_tableViewANSI_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableViewANSI->currentIndex().isValid())
    {
        return;
    }

    QModelIndex index=ui->tableViewANSI->currentIndex();
    //     QString sString=((QTableWidgetItem *)(ui->tableWidgetSections->focusWidget()))->text();
    //     qDebug(sString.toAscii().data());
    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(index.model()->data(index).toString()));

    menu.addSeparator();
    menu.addAction(actHex);

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableViewANSI->mapToGlobal(pos2));
}
void DialogSearch::hex()
{
    DialogHDE dialoghde(this);

    QModelIndexList list=ui->tableViewANSI->selectionModel()->selectedIndexes();

    if(!list.isEmpty())
    {
        unsigned long long nOffset=ui->tableViewANSI->model()->data(list.at(1)).toString().toULongLong(0,16);
        unsigned long long nSize=ui->tableViewANSI->model()->data(list.at(2)).toString().toULongLong(0,16);

        ////    dialoghde.setData(pefile->getFileName(),pefile->getSection_PointerToRawData(nSection),pefile->getSection_SizeOfRawData(nSection),pefile->getDisasmMode(),pOptions);
        dialoghde.setData(pOptions,_binary->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Selected"));

        dialoghde.exec();
    }

    //    qDebug(ui->tableViewANSI->model()->data(list.at(0)).toString().toAscii().data());
    //    qDebug(ui->tableViewANSI->model()->data(list.at(1)).toString().toAscii().data());
    //    qDebug(ui->tableViewANSI->model()->data(list.at(2)).toString().toAscii().data());
}

void DialogSearch::copyAsString()
{
    QModelIndex index=ui->tableViewANSI->currentIndex();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(index.model()->data(index).toString()));
}

void DialogSearch::on_pushButtonHEX_clicked()
{
    hex();
}

void DialogSearch::_selectedChanged()
{
    ui->pushButtonHEX->setEnabled(true);
}

void DialogSearch::on_listWidgetMods_currentRowChanged(int currentRow)
{
    ui->tableViewANSI->setModel(0);
    ui->pushButtonSaveResult->setEnabled(false);

    QString sCurrentText=ui->listWidgetMods->item(currentRow)->text();

    ui->pushButtonDump->setEnabled(false);
    ui->pushButtonDumpAll->setEnabled(false);
    ui->pushButtonHEX->setEnabled(false);

    if(sCurrentText=="ANSI")
    {
        ui->spinBoxMinLength->setEnabled(true);
        ui->comboBoxEndianness->setEnabled(false);
    }
    else if(sCurrentText=="UNICODE")
    {
        ui->spinBoxMinLength->setEnabled(true);
        ui->comboBoxEndianness->setEnabled(true);
    }
    else if(sCurrentText=="Crypto")
    {
        ui->spinBoxMinLength->setEnabled(false);
        ui->comboBoxEndianness->setEnabled(true);
    }
    else if(sCurrentText=="Links")
    {
        ui->spinBoxMinLength->setEnabled(true);
        ui->comboBoxEndianness->setEnabled(true);
    }
    else
    {
        ui->spinBoxMinLength->setEnabled(false);
        ui->comboBoxEndianness->setEnabled(false);
    }
}

void DialogSearch::foundLimit(int nLimit)
{
    QMessageBox::information(this,tr("Limit"),tr("Found %1 records").arg(nLimit));
}

void DialogSearch::on_pushButtonSaveResult_clicked()
{
    QFile file;
    QString sFileName;
    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.txt").arg(tr("Search")), tr("Text files (*.txt);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            file.resize(0);
            QString sResult;

            for(int i=0; i<searchTable.rowCount(); i++)
            {
                sResult+=QString("%1\t%2\t%3\r\n").arg(searchTable.getName(i)).arg(searchTable.getOffset(i),0,16).arg(searchTable.getSize(i),0,16);
            }

            file.write(sResult.toLatin1().data());

            file.close();
        }
        else
        {
            QMessageBox::critical(this,tr("Error"),tr("Cannot save file: %1").arg(sFileName));
        }
    }
}

void DialogSearch::searchEnable()
{
    ui->pushButtonSearch->setEnabled(true);
}

void DialogSearch::_error(QString sString)
{
    QMessageBox::critical(this,tr("Error"),sString);
}

void DialogSearch::on_lineEditFilter_textChanged(const QString &arg1)
{
    filter->setFilterRegExp(arg1);
    filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter->setFilterKeyColumn(0);
}
