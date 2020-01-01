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
#include "qxhexviewx.h"

QXHexViewX::QXHexViewX(QScrollArea *parent) :
    QWidget(parent)
{
    //    update();
    //    _paint=0;
    pDataBlock=0;

    //#ifdef Q_OS_MAC
    //    QFont newFont("Courier",12);
    //#else
    //    QFont newFont("Courier");
    //#endif

    //    setFont(newFont);

    connect(&_cursorTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    _cursorTimer.setInterval(500);
    _cursorTimer.start();

    connect(this, SIGNAL(__repaint()), this, SLOT(__repaintSlot()));

    bIsUpdated=true;
    //    nTemp=0;

    shortcutDump = new QShortcut(QKeySequence("Ctrl+D"), this);
    QObject::connect(shortcutDump, SIGNAL(activated()), this, SLOT(dump()));
    shortcutGoToAddress = new QShortcut(QKeySequence("Alt+G"), this);
    QObject::connect(shortcutGoToAddress, SIGNAL(activated()), this, SLOT(goToAddress()));
    shortcutSearchSignature = new QShortcut(QKeySequence("Ctrl+F"), this);
    QObject::connect(shortcutSearchSignature, SIGNAL(activated()), this, SLOT(searchData()));
    shortcutSearchText = new QShortcut(QKeySequence("Ctrl+T"), this);
    QObject::connect(shortcutSearchText, SIGNAL(activated()), this, SLOT(searchText()));
    shortcutSearchValue = new QShortcut(QKeySequence("Ctrl+H"), this);
    QObject::connect(shortcutSearchValue, SIGNAL(activated()), this, SLOT(searchValue()));
    shortcutSelectAll = new QShortcut(QKeySequence("Ctrl+A"), this);
    QObject::connect(shortcutSelectAll, SIGNAL(activated()), this, SLOT(selectAll()));
    shortcutCopy = new QShortcut(QKeySequence("Ctrl+C"), this);
    QObject::connect(shortcutCopy, SIGNAL(activated()), this, SLOT(copyAsHEX()));

    shortcutStartDocument = new QShortcut(QKeySequence::MoveToStartOfDocument, this);
    QObject::connect(shortcutStartDocument, SIGNAL(activated()), this, SIGNAL(_signalStartDocument()));
    shortcutEndDocument = new QShortcut(QKeySequence::MoveToEndOfDocument, this);
    QObject::connect(shortcutEndDocument, SIGNAL(activated()), this, SIGNAL(_signalEndDocument()));
#ifdef Q_OS_MAC
    shortcutSearchNext = new QShortcut(QKeySequence("Ctrl+G"), this);
#else
    shortcutSearchNext = new QShortcut(QKeySequence("F3"), this);
#endif
    QObject::connect(shortcutSearchNext, SIGNAL(activated()), this, SLOT(findNext()));
}

QXHexViewX::~QXHexViewX()
{

}

QWidget *QXHexViewX::getHinstance()
{
    return this;
}

void QXHexViewX::setData(__DATABLOCK *pFunctions)
{
    this->pDataBlock=pFunctions;
    //    update();
}

void QXHexViewX::_setFont(QFont font)
{
    setFont(font);

    pDataBlock->nSymbolWidth=fontMetrics().width(QLatin1Char('T'));
    pDataBlock->nSymbolHeight=fontMetrics().height();
}

void QXHexViewX::__update(unsigned long long nCurrentAddress)
{
    pDataBlock->nCurrentAddress=nCurrentAddress;

    //    qDebug("Update %X",nCurrentAddress);
    repaint();
}

void QXHexViewX::__update()
{
    repaint();
}

void QXHexViewX::timerSlot()
{
    if(bIsUpdated)
    {
        repaint();
    }
}

void QXHexViewX::__repaintSlot()
{

    if(pDataBlock->_paint)
    {
        _cursorTimer.stop();
        pDataBlock->_paint(this);
        emit rescroll();
        _cursorTimer.start();
    }
}

void QXHexViewX::stopSearch()
{
    //    qDebug("Cancel");
    searchThread->stopSearch();
}

void QXHexViewX::stopDump()
{
    dumpThread->stopDump();
}

void QXHexViewX::read()
{
    if(pDataBlock->bVorBuffer)
    {
#ifdef QT_DEBUG
        //        qDebug("VorBuffer true");
#endif

        if(!((pDataBlock->nCurrentAddress>=pDataBlock->nBufferAddress)&&(pDataBlock->nCurrentAddress+pDataBlock->nReadBlockSize<=pDataBlock->nBufferAddress+pDataBlock->nBufferSize)&&(pDataBlock->baBuffer.size())))
        {
#ifdef QT_DEBUG
            qDebug("Read");
#endif

            if(pDataBlock->nCurrentAddress+pDataBlock->nReadBlockSize>pDataBlock->nBufferAddress+pDataBlock->nBufferSize)
            {
                pDataBlock->nBufferAddress=pDataBlock->nCurrentAddress;

#ifdef QT_DEBUG
                qDebug("Current");
#endif
            }
            else
            {
                if(pDataBlock->nCurrentAddress-pDataBlock->nStartAddress>(pDataBlock->nReadBlockMax/4)*3)
                {
                    pDataBlock->nBufferAddress=pDataBlock->nCurrentAddress-(pDataBlock->nReadBlockMax/4)*3;
#ifdef QT_DEBUG
                    qDebug("B");
#endif
                }
                else
                {
                    pDataBlock->nBufferAddress=pDataBlock->nStartAddress;
#ifdef QT_DEBUG
                    qDebug("F");
#endif
                }
            }

            pDataBlock->nBufferSize=qMin((unsigned long long)pDataBlock->nReadBlockMax,pDataBlock->nMaxSize-(pDataBlock->nBufferAddress-pDataBlock->nStartAddress));
            _reload();

        }
    }
    else
    {
#ifdef QT_DEBUG
        qDebug("VorBuffer false");
        qDebug("Address: %x",pDataBlock->nCurrentAddress);
        qDebug("Size: %x",pDataBlock->nReadBlockSize);
#endif
        pDataBlock->nBufferAddress=pDataBlock->nCurrentAddress;
        pDataBlock->nBufferSize=pDataBlock->nReadBlockSize;

        _reload();
    }
}

QByteArray QXHexViewX::read(unsigned long long nAddress, unsigned long long nSize)
{
    QByteArray baResult;
    bool bIsReadOnly;

    if((nAddress>=pDataBlock->nStartAddress)&&(nAddress+nSize<=pDataBlock->nStartAddress+pDataBlock->nMaxSize))
    {
#ifdef QT_DEBUG
        qDebug("Read from file");
#endif
        nSize=qMin(nSize,pDataBlock->nMaxSize-nAddress+pDataBlock->nStartAddress);

        baResult=pDataBlock->__ReadMemory(pDataBlock->parameter.toString(),nAddress,nSize,&bIsReadOnly);
        pDataBlock->bIsReadOnly=bIsReadOnly;
        emit setReadOnly(bIsReadOnly);
    }

    return baResult;
}

bool QXHexViewX::write(unsigned long long nAddress, QByteArray baData)
{
    //    qDebug("Write: %x",nAddress);
    if(pDataBlock->__WriteMemory(pDataBlock->parameter.toString(),nAddress,baData))
    {
        setModAddresses.insert(nAddress);

        return true;
    }

    return false;
}

void QXHexViewX::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //    if(bIsUpdated)
    //    {
    //        return;
    //    }
    emit __repaint();
}
bool QXHexViewX::event(QEvent *event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    if((pDataBlock)&&(pDataBlock->_mouseEvent))
    {
        if(event->type()==QEvent::ContextMenu)
        {
            return false;
        }
        else if(event->type()==QEvent::MouseButtonPress)
        {
            if(mouseEvent->button()==Qt::LeftButton)
            {
                return pDataBlock->_mouseEvent(this,event);
            }
        }
        else if(event->type()==QEvent::MouseMove)
        {
            return pDataBlock->_mouseEvent(this,event);
        }
        else if(event->type()==QEvent::MouseButtonRelease)
        {
            if(mouseEvent->button()==Qt::RightButton)
            {
                if(pDataBlock->_contextMenu)
                {
                    pDataBlock->_contextMenu(this,mouseEvent->globalPos());
                }
            }
        }
    }

    return QWidget::event(event);
}

//void QXHexViewX::keyPressEvent(QKeyEvent *event)
//{
//    qDebug("key");
//}

void QXHexViewX::trigger0()
{
    //    qDebug("trigger0");
    unsigned long long nAddress=qMin(pDataBlock->nStartSelection,pDataBlock->nEndSelection);
    unsigned long long nSize=qMax(pDataBlock->nStartSelection,pDataBlock->nEndSelection)-nAddress;
    pDataBlock->_action[0](this,nAddress,nSize);
}
void QXHexViewX::trigger1()
{
    qDebug("trigger1");
}
void QXHexViewX::trigger2()
{
    qDebug("trigger2");
}
void QXHexViewX::trigger3()
{
    qDebug("trigger3");
}
void QXHexViewX::trigger4()
{
    qDebug("trigger4");
}
void QXHexViewX::trigger5()
{
    qDebug("trigger5");
}
void QXHexViewX::trigger6()
{
    qDebug("trigger6");
}
void QXHexViewX::trigger7()
{
    qDebug("trigger7");
}
void QXHexViewX::trigger8()
{
    qDebug("trigger8");
}
void QXHexViewX::trigger9()
{
    qDebug("trigger9");
}

void QXHexViewX::goToAddress()
{
    bIsUpdated=false;
    //    QString sString=QInputDialog::getText(this,"Go to","Address");

    //    emit setScrollValue(sString.toULongLong());
    //    __update(sString.toULongLong());

    QXHexGoTo dialog(this);

    dialog.setData(&(pDataBlock->records),pDataBlock->bAddressAsHEX);
    dialog._showToolTips(pDataBlock->bShowToolTips);

    connect(&dialog,SIGNAL(valueChanged(unsigned long long)),this,SIGNAL(setCurrentAddress(unsigned long long)));

    dialog.exec();

    bIsUpdated=true;
}

void QXHexViewX::search()
{
    bIsUpdated=false;
    QXHexSearch dialog(this);
    dialog._showToolTips(pDataBlock->bShowToolTips);
    dialog.setLittleEndian(pDataBlock->bIsLittleEndian);

    //    dialog.setData(&(pDataBlock->records),pDataBlock->bAddressAsHEX);

    connect(&dialog,SIGNAL(signature(QString,bool)),this,SLOT(findSignature(QString,bool)),Qt::QueuedConnection);

    dialog.exec();

    bIsUpdated=true;
}

void QXHexViewX::searchData()
{
    bIsUpdated=false;
    QXHexSearch dialog(this);
    dialog._showToolTips(pDataBlock->bShowToolTips);
    dialog.setLittleEndian(pDataBlock->bIsLittleEndian);

    dialog.setTab(0);

    //    dialog.setData(&(pDataBlock->records),pDataBlock->bAddressAsHEX);

    connect(&dialog,SIGNAL(signature(QString,bool)),this,SLOT(findSignature(QString,bool)),Qt::QueuedConnection);

    dialog.exec();

    bIsUpdated=true;
}

void QXHexViewX::searchText()
{
    bIsUpdated=false;
    QXHexSearch dialog(this);
    dialog._showToolTips(pDataBlock->bShowToolTips);
    dialog.setLittleEndian(pDataBlock->bIsLittleEndian);

    dialog.setTab(1);

    //    dialog.setData(&(pDataBlock->records),pDataBlock->bAddressAsHEX);

    connect(&dialog,SIGNAL(signature(QString,bool)),this,SLOT(findSignature(QString,bool)),Qt::QueuedConnection);

    dialog.exec();

    bIsUpdated=true;
}

void QXHexViewX::searchValue()
{
    bIsUpdated=false;
    QXHexSearch dialog(this);
    dialog._showToolTips(pDataBlock->bShowToolTips);
    dialog.setLittleEndian(pDataBlock->bIsLittleEndian);

    dialog.setTab(2);

    //    dialog.setData(&(pDataBlock->records),pDataBlock->bAddressAsHEX);

    connect(&dialog,SIGNAL(signature(QString,bool)),this,SLOT(findSignature(QString,bool)),Qt::QueuedConnection);

    dialog.exec();

    bIsUpdated=true;
}

void QXHexViewX::findSignature(QString sSignature,bool bIsCursor)
{
    if(bIsCursor)
    {
        pDataBlock->nSearchOffset=pDataBlock->nCursor;
    }
    else
    {
        pDataBlock->nSearchOffset=pDataBlock->nStartAddress;
    }

    pDataBlock->sSearchSignature=sSignature;
    //    qDebug(sSignature.toAscii().data());
    _searchThread();
    //    QMessageBox::information(this,"Information","Nothing found");
}

void QXHexViewX::findNext()
{
    if(pDataBlock->sSearchSignature!="")
    {
        if(pDataBlock->nSearchOffset+1<pDataBlock->nMaxSize)
        {
            //            pDataBlock->nSearchOffset++; // Mb TODO Search from current offset
            pDataBlock->nCursor=pDataBlock->nCurrentAddress;
            pDataBlock->nSearchOffset=pDataBlock->nCursor+1;
            _searchThread();
        }
    }
}

void QXHexViewX::_searchThread()
{
    searchThread = new QXHexSearchThread;
    searchThread->setData(pDataBlock);
    QThread* thread = new QThread;
    searchThread->moveToThread(thread);

    QProgressDialog dialog;

    dialog.setWindowTitle(tr("Search"));
    //    connect(&dialog, SIGNAL(canceled()), search, SLOT(stopSearch()));
    connect(&dialog, SIGNAL(canceled()), this, SLOT(stopSearch()));
    connect(thread, SIGNAL(started()), searchThread, SLOT(startSearch()));
    connect(searchThread, SIGNAL(_searchComplete()), thread, SLOT(quit()));
    connect(searchThread, SIGNAL(_searchComplete()), searchThread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(searchThread, SIGNAL(_found(unsigned long long,unsigned long long)), this, SLOT(found(unsigned long long,unsigned long long)));
    connect(searchThread, SIGNAL(_searchComplete()), &dialog, SLOT(reset()));
    connect(searchThread, SIGNAL(_nothingfound()), this, SLOT(nothingFound()));

    connect(searchThread, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    connect(searchThread, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    thread->start();

    dialog.exec();
}

void QXHexViewX::_dumpThread()
{
    dumpThread = new QXHexDumpThread;
    dumpThread->setData(pDataBlock);
    QThread* thread = new QThread;
    dumpThread->moveToThread(thread);

    QProgressDialog dialog;

    dialog.setWindowTitle(tr("Dump"));
    //    connect(&dialog, SIGNAL(canceled()), search, SLOT(stopSearch()));
    connect(&dialog, SIGNAL(canceled()), this, SLOT(stopDump()));
    connect(dumpThread, SIGNAL(_error(QString)), this, SLOT(error(QString)));
    connect(dumpThread, SIGNAL(_information(QString)), this, SLOT(information(QString)));
    connect(thread, SIGNAL(started()), dumpThread, SLOT(startDump()));
    connect(dumpThread, SIGNAL(_dumpComplete()), thread, SLOT(quit()));
    connect(dumpThread, SIGNAL(_dumpComplete()), dumpThread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(dumpThread, SIGNAL(_dumpComplete()), &dialog, SLOT(reset()));

    connect(dumpThread, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    connect(dumpThread, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    thread->start();

    dialog.exec();
}

void QXHexViewX::found(unsigned long long nAddress,unsigned long long nSize)
{
    pDataBlock->nStartSelection=nAddress;
    pDataBlock->nEndSelection=nAddress+nSize;
    pDataBlock->nCursor=nAddress;

    pDataBlock->nSearchOffset=nAddress;

    emit goToAddress(nAddress);
}

void QXHexViewX::nothingFound()
{
    QMessageBox::information(this,tr("Information"),tr("Nothing found"));
}

void QXHexViewX::dumpComplete(QString sFileName)
{
    QMessageBox::information(this,tr("Information"),sFileName);
}

void QXHexViewX::error(QString sString)
{
    QMessageBox::critical(this,tr("Error"),sString);
}

void QXHexViewX::information(QString sInformation)
{
    QMessageBox::information(this,tr("Information"),sInformation);
}

void QXHexViewX::dump()
{
    bIsUpdated=false;

    //    QByteArray baTemp;
    unsigned long long nAddress=getSelectionOffset();
    unsigned long long nSize=getSelectionSize();
    //    unsigned long long _nSize=nSize;

    if(!nSize)
    {
        return;
    }

    //    qDebug("Start: %X",nStartSelection);
    //    qDebug("End: %X",nEndSelection);
    //    qDebug("Address: %X",nAddress);
    //    qDebug("Size: %X",nSize);

    QString sDumpFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),QString("%1.bin").arg(tr("Dump")), tr("Dump files (*.bin);;All files (*)"));

    if(!sDumpFileName.isEmpty())
    {
        pDataBlock->nDumpAddress=nAddress;
        pDataBlock->nDumpSize=nSize;
        pDataBlock->sDumpFileName=sDumpFileName;

        _dumpThread();
        //        QFile file;
        //        int nTemp;

        //        file.setFileName(sDumpFileName);

        //        if(file.open(QIODevice::ReadWrite))
        //        {
        //            file.resize(0);

        //            while(nSize)
        //            {
        //                nTemp=qMin((unsigned long long)pDataBlock->nReadBlockMax,nSize);
        //                baTemp.append(read(nAddress,nTemp));

        //                nAddress+=nTemp;
        //                nSize-=nTemp;
        //            }

        //            baTemp.resize(_nSize);

        //            file.write(baTemp.data(),baTemp.size());

        //            file.close();
        //        }
        //        else
        //        {
        //            emitError("Cannot save to file: "+sDumpFileName);
        //        }
    }

    bIsUpdated=true;
}

void QXHexViewX::selectAll()
{
    pDataBlock->nStartSelection=pDataBlock->nStartAddress;
    pDataBlock->nEndSelection=pDataBlock->nStartAddress+pDataBlock->nMaxSize;

    __update();
}

void QXHexViewX::selectReset()
{
    pDataBlock->nStartSelection=0;
    pDataBlock->nEndSelection=0;
    pDataBlock->nNextSelection=0;

    __update();
}

void QXHexViewX::copyAsHEX()
{
    unsigned long long nAddress=getSelectionOffset();
    unsigned long long nSize=getSelectionSize();

    if(!nSize)
    {
        return;
    }

    QByteArray baTemp;
    baTemp.append(read(nAddress,qMin((unsigned long long)0x1000,nSize)));

    baTemp=baTemp.toHex();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(baTemp.data());

}

void QXHexViewX::copyAsHEXWithSpaces()
{
    unsigned long long nAddress=getSelectionOffset();
    unsigned long long nSize=getSelectionSize();

    if(!nSize)
    {
        return;
    }

    QByteArray baTemp;
    baTemp.append(read(nAddress,qMin((unsigned long long)0x1000,nSize)));

    baTemp=baTemp.toHex();
    QString sTemp2;

    for(int i=0; i<baTemp.size(); i+=2)
    {
        sTemp2.append(baTemp.data()[i]);
        sTemp2.append(baTemp.data()[i+1]);
        sTemp2.append(" ");
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(sTemp2);
}

void QXHexViewX::copyAsANSI()
{
    unsigned long long nAddress=getSelectionOffset();
    unsigned long long nSize=getSelectionSize();

    if(!nSize)
    {
        return;
    }

    QByteArray baTemp;
    baTemp.append(read(nAddress,qMin((unsigned long long)0x1000,nSize)));

    QString sTemp2;

    sTemp2=ConvertToAnsi(&baTemp);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(sTemp2);
}

void QXHexViewX::copyAsSignature()
{
    unsigned long long nAddress=getSelectionOffset();
    unsigned long long nSize=getSelectionSize();

    if(!nSize)
    {
        return;
    }

    bIsUpdated=false;
    QXHexSignature dialog(this);
    dialog._showToolTips(pDataBlock->bShowToolTips);

    QByteArray baTemp;
    baTemp.append(read(nAddress,qMin((unsigned long long)0x80,nSize)));

    dialog.setData(baTemp);

    //    connect(&dialog,SIGNAL(signature(QString,bool)),this,SLOT(findSignature(QString,bool)),Qt::QueuedConnection);

    dialog.exec();

    bIsUpdated=true;
}

unsigned long long QXHexViewX::getSelectionOffset()
{
    unsigned long long nStartSelection=pDataBlock->nStartSelection;
    unsigned long long nEndSelection=pDataBlock->nEndSelection;

    if(nStartSelection>=nEndSelection)
    {
        qSwap(nStartSelection,nEndSelection);
    }

    return nStartSelection;
}

unsigned long long QXHexViewX::getSelectionSize()
{
    unsigned long long nStartSelection=pDataBlock->nStartSelection;
    unsigned long long nEndSelection=pDataBlock->nEndSelection;

    if(nStartSelection>=nEndSelection)
    {
        qSwap(nStartSelection,nEndSelection);
        nEndSelection=pDataBlock->nNextSelection;
    }

    unsigned long long nSize=nEndSelection-nStartSelection;

    return nSize;
}

bool QXHexViewX::isSelectionPresent()
{
    return getSelectionSize()!=0;
}

void QXHexViewX::reload()
{
    _reload();
    //    pDataBlock->_paint(this);
}

void QXHexViewX::_reload()
{
    pDataBlock->baBuffer=read(pDataBlock->nBufferAddress,pDataBlock->nBufferSize);

    if(pDataBlock->baBuffer.size()==0)
    {
        pDataBlock->baBuffer.fill(0,pDataBlock->nBufferSize);
        pDataBlock->sHEX.fill('?',2*(pDataBlock->nBufferSize));
        pDataBlock->sANSI.fill('?',pDataBlock->nBufferSize);
    }
    else
    {
        pDataBlock->sHEX=ConvertToHex(&pDataBlock->baBuffer);
        pDataBlock->sANSI=ConvertToAnsi(&pDataBlock->baBuffer);
    }

    //            pDataBlock->sHEX.fill('?',2*(pDataBlock->nReadBlockMax));
    //            pDataBlock->sANSI.fill('?',pDataBlock->nReadBlockMax);
    //        }
    //        else
    //        {
    //            pDataBlock->sHEX=ConvertToHex(&pDataBlock->baBuffer);
    //            pDataBlock->sANSI=ConvertToAnsi(&pDataBlock->baBuffer);
    //        }

}

QSet<unsigned long long> *QXHexViewX::getModAddresses()
{
    return &setModAddresses;
}

void QXHexViewX::emitError(QString sString)
{
    emit appendError(sString);
}

void QXHexViewX::emitRescroll()
{
    emit rescroll();
}

__DATABLOCK *QXHexViewX::getDataBlock()
{
    return pDataBlock;
}

void QXHexViewX::emitSelectionStartChanged(unsigned long long nValue)
{
    emit selectionStartChanged(nValue);
}

void QXHexViewX::emitSelectionSizeChanged(unsigned long long nValue)
{
    emit selectionSizeChanged(nValue);
}

void QXHexViewX::emitCursorChanged(unsigned long long nValue)
{
    emit cursorChanged(nValue);
    //    qDebug("%d",nValue);
}

void QXHexViewX::emitGoToAddress(unsigned long long nValue)
{
    emit goToAddress(nValue);
}

void QXHexViewX::_stepF()
{
    emit _signalStepF();
}

void QXHexViewX::_stepB()
{
    emit _signalStepB();
}

void QXHexViewX::_pageUp()
{
    emit _signalPageUp();
}

void QXHexViewX::_pageDown()
{
    emit _signalPageDown();
}
