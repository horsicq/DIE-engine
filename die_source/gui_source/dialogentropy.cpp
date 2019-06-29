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
#include "dialogentropy.h"
#include "ui_dialogentropy.h"

DialogEntropy::DialogEntropy(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogEntropy)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);

    ui->lineEditEntropy->setHex(false);
    ui->lineEditStatus->setHex(false);

    ui->lineEditPartOffset->setHex(false);
    ui->lineEditPartOffset->setDec(true);
    ui->lineEditPartSize->setHex(false);
    ui->lineEditPartSize->setDec(true);

    plot = new QwtPlot;

    QPen pen(Qt::red);
    curve = new QwtPlotCurve;
    curve->setPen(pen);
    curve->attach(plot);
    plot->setAutoReplot();
    ui->verticalLayoutImageCurve->addWidget(plot);

    plot2=new QwtPlot;

    ui->verticalLayoutImageHistogram->addWidget(plot2);

    QPen pen2(Qt::blue);
    histogram = new QwtPlotHistogram;
    histogram->setPen(pen2);

    histogram->attach(plot2);
    plot2->setAutoReplot();

    plot2->setAxisScale(2,0,256,32);
    //    plot2->setAxisScale(0,0,10000);
    plot2->updateAxes();

    ui->progressBarStatus->setValue(0);
    setProgressBar(1,1);

    pOptions=0;

    setEntropy(0);

    //this->setWindowFlags(this->windowFlags()|Qt::WindowMinMaxButtonsHint);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonReload->setShortcut(QKeySequence(__KeySequence_ctrlR));
    ui->pushButtonSaveDiagram->setShortcut(QKeySequence(__KeySequence_save));

    shortcut1 = new QShortcut(QKeySequence(__KeySequence_tab1), this);
    QObject::connect(shortcut1, SIGNAL(activated()), this, SLOT(tabCurve()));
    shortcut2 = new QShortcut(QKeySequence(__KeySequence_tab2), this);
    QObject::connect(shortcut2, SIGNAL(activated()), this, SLOT(tabHistogram()));
    shortcut3 = new QShortcut(QKeySequence(__KeySequence_tab3), this);
    QObject::connect(shortcut3, SIGNAL(activated()), this, SLOT(tabBytes()));

    bIsScan=false;
}

DialogEntropy::~DialogEntropy()
{
    emit reset();

    delete ui;
}

void DialogEntropy::setData(__DIE_OPTIONS *pOptions,Binary *_binary, unsigned int nOffset, unsigned int nSize)
{
    this->_binary=_binary;
    this->nOffset=nOffset;
    this->nSize=nSize;
    this->_nOffset=nOffset;
    this->_nSize=nSize;

    ui->lineEditOffset->setText(QString("%1").arg(nOffset));
    ui->lineEditSize->setText(QString("%1").arg(nSize));

    setOptions(pOptions);

    ui->pushButtonReload->setFocus();
}

void DialogEntropy::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogEntropy::on_pushButtonReload_clicked()
{
    if(bIsScan)
    {
        scan->_stop();

        ui->pushButtonReload->setText(tr("Reload"));

        bIsScan=false;
    }
    else
    {
        reload();
        bIsScan=true;
    }
}

void DialogEntropy::on_pushButtonSaveDiagram_clicked()
{
    QStringList filter;
    QString sFileName;
    QwtPlot *pPlot;

    if(ui->tabWidget->currentIndex()==0)
    {
        sFileName = pOptions->sLastDirectory+QDir::separator()+QString("%1.png").arg(tr("Curve"));
        pPlot=plot;
    }
    else if(ui->tabWidget->currentIndex()==1)
    {
        sFileName = pOptions->sLastDirectory+QDir::separator()+QString("%1.png").arg(tr("Histogram"));
        pPlot=plot2;
    }

    else
    {
        return;
    }

    const QList<QByteArray> imageFormats=QImageWriter::supportedImageFormats();

    if(imageFormats.size() > 0)
    {
        QString imageFilter("Images (");

        for(int i = 0; i < imageFormats.size(); i++)
        {
            if(i > 0)
            {
                imageFilter += " ";
            }

            imageFilter += "*.";
            imageFilter += imageFormats[i];
        }

        imageFilter += ")";

        filter += imageFilter;
    }


    filter += "PDF Documents (*.pdf)";
    filter += "Postscript Documents (*.ps)";

    sFileName=QFileDialog::getSaveFileName(this,tr("Save diagram"),sFileName,filter.join(";;"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        QwtPlotRenderer renderer;
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground,false);
        //        renderer.setLayoutFlag(QwtPlotRenderer::KeepFrames,true);
        renderer.renderDocument(pPlot,sFileName, QSizeF(300, 200), 85);
    }
}

void DialogEntropy::tabCurve()
{
    ui->tabWidget->setCurrentIndex(0);
}

void DialogEntropy::tabHistogram()
{
    ui->tabWidget->setCurrentIndex(1);
}

void DialogEntropy::tabBytes()
{
    ui->tabWidget->setCurrentIndex(2);
}

void DialogEntropy::_on_finished()
{
    ui->pushButtonReload->setText(tr("Reload"));
    bIsScan=false;
}

bool DialogEntropy::reload()
{
    if((unsigned long long)nOffset+(unsigned long long)nSize>=0x80000000)
    {
        QMessageBox::critical(this,tr("Error"),tr("This file is too large!"));
        this->close();
    }

    emit appendError(tr("This file is too large!"));

    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->pushButtonReload->setToolTip(tr("Reload"));
        ui->pushButtonExtra->setToolTip(tr("Restore"));
        ui->pushButtonSaveDiagram->setToolTip(tr("Save entropy diagram"));
        ui->progressBar->setToolTip(tr("Calculate entropy progress"));
        ui->progressBarStatus->setToolTip(tr("Entropy percentage"));
        ui->lineEditStatus->setToolTip(tr("Status"));

        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg("region"));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg("region"));
        ui->lineEditEntropy->setToolTip(tr("Entropy bits/byte(0-8)"));

        ui->listWidget->setToolTip(tr("Regions of file"));
        ui->lineEditPartOffset->setToolTip(tr("File offset of %1").arg("part"));
        ui->lineEditPartSize->setToolTip(tr("Size of %1 in bytes").arg("part"));
        ui->tableWidget->setToolTip(tr("Bytes in file"));

        ui->tabWidget->setTabToolTip(0,QString("%1(Alt+1)").arg(tr("Curve")));
        ui->tabWidget->setTabToolTip(1,QString("%1(Alt+2)").arg(tr("Histogram")));
        ui->tabWidget->setTabToolTip(2,QString("%1(Alt+3)").arg(tr("Bytes")));
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

    setEntropy(0);

    for(int i=0; i<101; i++)
    {
        xCurve[i]=nSize;
        yCurve[i]=0;
    }


    for(int i=0; i<256; i++)
    {
        xHistogram[i]=i;
        yHistogram[i]=0;
    }

    reloadGraph();


    scan= new ThreadEntropy;

    QThread* thread =new QThread;
    scan->moveToThread(thread);

    connect(this, SIGNAL(reset()), scan, SLOT(_stop()),Qt::DirectConnection);
    connect(thread, SIGNAL(started()), scan, SLOT(process()));
    connect(scan, SIGNAL(_finished()), thread, SLOT(quit()));
    connect(scan, SIGNAL(_finished()), scan, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(scan, SIGNAL(_finished()), this, SLOT(_on_finished()));

    connect(scan,SIGNAL(setProgressBar(int,int)),this,SLOT(setProgressBar(int,int)));
    connect(scan,SIGNAL(setEntropy(float)),this,SLOT(setEntropy(float)));
    connect(scan,SIGNAL(reloadGraph()),this,SLOT(reloadGraph()));


    scan->setData(_binary,nOffset,nSize,xCurve,yCurve,xHistogram,yHistogram,pOptions->nEntropyGraph);
    thread->start();

    bIsScan=true;

    ui->pushButtonReload->setText(tr("Stop"));

    ///////////////////////////////////////////////////////
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///

    ui->listWidget->clear();
    listZones.clear();
    listRegions.clear();

    if(_binary->size()==nSize)
    {
        QListWidgetItem *pItem;
        QString sRegion;

        if((_binary->getType()=="PE")||(_binary->getType()=="PE+(64)"))
        {
            PEFile pefile;
            unsigned int __nOffset;
            unsigned int __nSize;
            float __entropy;
            QColor c;
            QwtPlotZoneItem *zone;

            if(pefile.setFileName(_binary->getFileName()))
            {
                __nOffset=pefile.getDosHeaderOffset();
                __nSize=pefile.calculateSizeOfHeaders();
                __entropy=pefile.calculateEntropy(__nOffset,__nSize);
                pItem=new QListWidgetItem;
                sRegion=QString("PE Header(\"%1\")").arg(__entropy);
                pItem->setText(sRegion);
                listRegions.append(sRegion);
                pItem->setData(Qt::UserRole,QString("%1:%2").arg(__nOffset).arg(__nSize));

                ui->listWidget->addItem(pItem);



                zone = new QwtPlotZoneItem;
                zone->setInterval(__nOffset,__nOffset+__nSize);
                zone->setVisible(false);
                c = Qt::darkBlue;
                c.setAlpha(100);
                zone->setPen(c);
                c.setAlpha(20);
                zone->setBrush(c);
                zone->attach(plot);
                listZones.append(zone);

                for(int i=0; i<pefile.getFileHeader_NumberOfSections(); i++)
                {
                    __nOffset=pefile.getSection_PointerToRawData(i);
                    __nSize=pefile.getSection_SizeOfRawData(i);
                    __entropy=pefile.calculateEntropy(__nOffset,__nSize);
                    pItem=new QListWidgetItem;
                    sRegion=QString("Section%1(\"%2\")(\"%3\")").arg(i).arg(pefile.getSectionNameAsString(i)).arg(__entropy);
                    pItem->setText(sRegion);
                    listRegions.append(sRegion);
                    pItem->setData(Qt::UserRole,QString("%1:%2").arg(__nOffset).arg(__nSize));
                    ui->listWidget->addItem(pItem);

                    zone = new QwtPlotZoneItem;
                    zone->setInterval(__nOffset,__nOffset+__nSize);
                    zone->setVisible(false);
                    c = Qt::darkYellow;
                    c.setAlpha(100);
                    zone->setPen(c);
                    c.setAlpha(20);
                    zone->setBrush(c);
                    zone->attach(plot);
                    listZones.append(zone);
                }

                if(pefile.isOverlayPresent())
                {
                    __nOffset=pefile.getOverlayOffset();
                    __nSize=pefile.getOverlaySize();
                    __entropy=pefile.calculateEntropy(__nOffset,__nSize);
                    pItem=new QListWidgetItem;
                    sRegion=QString("Overlay(\"%1\")").arg(__entropy);
                    pItem->setText(sRegion);
                    listRegions.append(sRegion);
                    pItem->setData(Qt::UserRole,QString("%1:%2").arg(__nOffset).arg(__nSize));
                    ui->listWidget->addItem(pItem);


                    zone = new QwtPlotZoneItem;
                    zone->setInterval(__nOffset,__nOffset+__nSize);
                    zone->setVisible(false);
                    c = Qt::darkRed;
                    c.setAlpha(100);
                    zone->setPen(c);
                    c.setAlpha(20);
                    zone->setBrush(c);
                    zone->attach(plot);
                    listZones.append(zone);
                }
            }
        }
        else if((_binary->getType()=="ELF")||(_binary->getType()=="ELF64"))
        {
            ELFFile elffile;
            unsigned int __nOffset;
            unsigned int __nSize;
            float __entropy;
            QColor c;
            QwtPlotZoneItem *zone;

            if(elffile.setFileName(_binary->getFileName()))
            {
                for(int i=0; i<elffile.getElfHeader_phnum(); i++)
                {

                    if(_binary->getType()=="ELF")
                    {
                        __nOffset=elffile.getProgramHeader32_offset(i);
                        __nSize=elffile.getProgramHeader32_filesz(i);
                    }
                    else
                    {
                        __nOffset=elffile.getProgramHeader64_offset(i);
                        __nSize=elffile.getProgramHeader64_filesz(i);
                    }



                    __entropy=elffile.calculateEntropy(__nOffset,__nSize);
                    pItem=new QListWidgetItem;
                    sRegion=QString("Program%1(\"%2\")").arg(i).arg(__entropy);
                    pItem->setText(sRegion);
                    listRegions.append(sRegion);
                    pItem->setData(Qt::UserRole,QString("%1:%2").arg(__nOffset).arg(__nSize));
                    ui->listWidget->addItem(pItem);

                    zone = new QwtPlotZoneItem;
                    zone->setInterval(__nOffset,__nOffset+__nSize);
                    zone->setVisible(false);
                    c = Qt::darkYellow;
                    c.setAlpha(100);
                    zone->setPen(c);
                    c.setAlpha(20);
                    zone->setBrush(c);
                    zone->attach(plot);
                    listZones.append(zone);
                }

                if(elffile.isOverlayPresent())
                {
                    __nOffset=elffile.getOverlayOffset();
                    __nSize=elffile.getOverlaySize();
                    __entropy=elffile.calculateEntropy(__nOffset,__nSize);
                    pItem=new QListWidgetItem;
                    sRegion=QString("Overlay(\"%1\")").arg(__entropy);
                    pItem->setText(sRegion);
                    listRegions.append(sRegion);
                    pItem->setData(Qt::UserRole,QString("%1:%2").arg(__nOffset).arg(__nSize));
                    ui->listWidget->addItem(pItem);

                    zone = new QwtPlotZoneItem;
                    zone->setInterval(__nOffset,__nOffset+__nSize);
                    zone->setVisible(false);
                    c = Qt::darkRed;
                    c.setAlpha(100);
                    zone->setPen(c);
                    c.setAlpha(20);
                    zone->setBrush(c);
                    zone->attach(plot);
                    listZones.append(zone);
                }
            }
        }
        else if((_binary->getType()=="MACH")||(_binary->getType()=="MACH64"))
        {
            MACHFile machfile;
            unsigned int __nOffset;
            unsigned int __nSize;
            float __entropy;
            QColor c;
            QwtPlotZoneItem *zone;

            if(machfile.setFileName(_binary->getFileName()))
            {
                for(int i=0; i<machfile.getNumberOfSegments(); i++)
                {

                    if(_binary->getType()=="MACH")
                    {
                        __nOffset=machfile.getSegment_fileoff32(i);
                        __nSize=machfile.getSegment_filesize32(i);
                    }
                    else
                    {
                        __nOffset=machfile.getSegment_fileoff64(i);
                        __nSize=machfile.getSegment_filesize64(i);
                    }



                    __entropy=machfile.calculateEntropy(__nOffset,__nSize);
                    pItem=new QListWidgetItem;
                    sRegion=QString("Segment%1(\"%2\")").arg(i).arg(__entropy);
                    pItem->setText(sRegion);
                    listRegions.append(sRegion);
                    pItem->setData(Qt::UserRole,QString("%1:%2").arg(__nOffset).arg(__nSize));
                    ui->listWidget->addItem(pItem);

                    zone = new QwtPlotZoneItem;
                    zone->setInterval(__nOffset,__nOffset+__nSize);
                    zone->setVisible(false);
                    c = Qt::darkYellow;
                    c.setAlpha(100);
                    zone->setPen(c);
                    c.setAlpha(20);
                    zone->setBrush(c);
                    zone->attach(plot);
                    listZones.append(zone);
                }

                if(machfile.isOverlayPresent())
                {
                    __nOffset=machfile.getOverlayOffset();
                    __nSize=machfile.getOverlaySize();
                    __entropy=machfile.calculateEntropy(__nOffset,__nSize);
                    pItem=new QListWidgetItem;
                    sRegion=QString("Overlay(\"%1\")").arg(__entropy);
                    pItem->setText(sRegion);
                    listRegions.append(sRegion);
                    pItem->setData(Qt::UserRole,QString("%1:%2").arg(__nOffset).arg(__nSize));
                    ui->listWidget->addItem(pItem);


                    zone = new QwtPlotZoneItem;
                    zone->setInterval(__nOffset,__nOffset+__nSize);
                    zone->setVisible(false);
                    c = Qt::darkRed;
                    c.setAlpha(100);
                    zone->setPen(c);
                    c.setAlpha(20);
                    zone->setBrush(c);
                    zone->attach(plot);
                    listZones.append(zone);
                }
            }
        }

    }

    return true;
}
void DialogEntropy::setProgressBar(int nMax, int nValue)
{
    ui->progressBar->setMaximum(nMax);
    ui->progressBar->setValue(nValue);
}

void DialogEntropy::setEntropy(float fEntropy)
{
    ui->lineEditEntropy->setText(QString("%1").arg(fEntropy));

    int nTemp=(int)((fEntropy/8)*1000);

    ui->progressBarStatus->setMaximum(1000);
    ui->progressBarStatus->setValue(nTemp);

    if(pOptions)
    {
        if(fEntropy>pOptions->fThreshold)
        {
            ui->lineEditStatus->setText(tr("packed"));
        }
        else
        {
            ui->lineEditStatus->setText(tr("not packed"));
        }
    }


    //    ui->pushButtonReload->setText("Reload");
}

void DialogEntropy::reloadGraph()
{
    curve->setSamples(xCurve,yCurve,pOptions->nEntropyGraph);
    //    unsigned int nMax=0;

    QVector<QwtIntervalSample> samples(256);

    for(uint i = 0; i < 256; i++)
    {
        QwtInterval interval(double(i), i + 1.0);
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);

        //        nMax=qMax((unsigned int)(yHistogram[i]),nMax);

        samples[i] = QwtIntervalSample(yHistogram[i], interval);
    }



    //    histogram->setData( new QwtIntervalSeriesData( samples ) );
    histogram->setSamples(samples);

    //    plot2->setAxisMaxMajor(0,10000);
    //    plot2->setAxisMaxMajor(1,256);
    //    plot2->replot();

    QTableWidgetItem *item;

    for(int i=0; i<256; i++)
    {
        item=new QTableWidgetItem(QString("0x%1").arg(i,2,16,QChar('0')));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(i,0,item);
        item=new QTableWidgetItem;
        item->setData(Qt::DisplayRole, (unsigned int)yHistogram[i]);
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(i,1,item);
        item=new QTableWidgetItem;
        item->setData(Qt::DisplayRole,yHistogram[i]/nSize*100);
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(i,2,item);
        ui->tableWidget->setRowHeight(i,20);
    }
}



void DialogEntropy::on_listWidget_itemSelectionChanged()
{
    for(int j=0; j<listZones.count(); j++)
    {
        listZones.at(j)->setVisible(false);
    }


    unsigned int nPartOffset=nSize;
    unsigned int nPartSize=0;
    QString sTemp;

    //    qDebug("Changed");
    for(int i=0; i<ui->listWidget->selectedItems().count(); i++)
    {
        sTemp=ui->listWidget->selectedItems().at(i)->text();

        for(int j=0; j<listZones.count(); j++)
        {
            if(listRegions.at(j)==sTemp)
            {
                listZones.at(j)->setVisible(true);
            }
        }

        sTemp=ui->listWidget->selectedItems().at(i)->data(Qt::UserRole).toString();

        nPartOffset=qMin(sTemp.section(":",0,0).toUInt(),nPartOffset);

        nPartSize=qMax(sTemp.section(":",0,0).toUInt()+sTemp.section(":",1,1).toUInt(),nPartSize);
        //        qDebug(ui->listWidget->selectedItems().at(i)->text().toAscii().data(Qt::UserRole));
    }

    if(nPartOffset==nSize)
    {
        nPartOffset=0;
    }

    nPartSize=nPartSize-nPartOffset;

    ui->lineEditPartOffset->setText(QString("%1").arg(nPartOffset));
    ui->lineEditPartSize->setText(QString("%1").arg(nPartSize));
}

void DialogEntropy::on_tabWidget_currentChanged(int index)
{
    ui->pushButtonSaveDiagram->setEnabled(index!=2);
}

void DialogEntropy::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QString sTemp=ui->listWidget->selectedItems().at(0)->data(Qt::UserRole).toString();
    unsigned int nPartOffset=sTemp.section(":",0,0).toUInt();
    unsigned int nPartSize=sTemp.section(":",1,1).toUInt();

    if(nPartSize)
    {
        ui->lineEditOffset->setText(QString("%1").arg(nPartOffset));
        ui->lineEditSize->setText(QString("%1").arg(nPartSize));

        reload();
    }


}

void DialogEntropy::on_pushButtonExtra_clicked()
{
    ui->lineEditOffset->setText(QString("%1").arg(_nOffset));
    ui->lineEditSize->setText(QString("%1").arg(_nSize));
    reload();
}
