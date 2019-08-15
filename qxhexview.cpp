#include "qxhexview.h"

QXHexView::QXHexView(QWidget *parent) :
    QScrollArea(parent)
{
    QFont newFont("Courier");
    setFont(newFont);

    _init(&datablock);

    QVBoxLayout *layout = new QVBoxLayout(this);
    //    QHBoxLayout *toolBox = new QHBoxLayout(this);

    setLayout(layout);
    //    setWidget(qhexviewx);
    setWidgetResizable(true);

    //    QComboBox *comboBoxType=new QComboBox(this);
    //    QComboBox *comboBoxMode=new QComboBox(this);
    //    QWidget* empty = new QWidget(this);
    //    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    //    toolBox->addWidget(comboBoxType);
    //    toolBox->addWidget(comboBoxMode);
    //    toolBox->addWidget(empty);
    //    layout->addLayout(toolBox);
    qxhexviewx=new QXHexViewX(this);

    layout->addWidget(qxhexviewx);
    //    qxhexviewx->resize(200,1000);

    connect(qxhexviewx,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(qxhexviewx,SIGNAL(setCurrentAddress(unsigned long long)),this,SLOT(goToAddress(unsigned long long)));
    connect(qxhexviewx,SIGNAL(rescroll()),this,SLOT(rescroll()));

    connect(qxhexviewx,SIGNAL(selectionStartChanged(unsigned long long)),this,SIGNAL(selectionStartChanged(unsigned long long)));
    connect(qxhexviewx,SIGNAL(selectionSizeChanged(unsigned long long)),this,SIGNAL(selectionSizeChanged(unsigned long long)));
    connect(qxhexviewx,SIGNAL(cursorChanged(unsigned long long)),this,SIGNAL(cursorChanged(unsigned long long)));
    connect(qxhexviewx,SIGNAL(goToAddress(unsigned long long)),this,SLOT(goToAddress(unsigned long long)));
    connect(qxhexviewx,SIGNAL(setReadOnly(bool)),this,SIGNAL(setReadOnly(bool)));

    connect(qxhexviewx,SIGNAL(_signalStepB()),this,SLOT(_stepB()));
    connect(qxhexviewx,SIGNAL(_signalStepF()),this,SLOT(_stepF()));
    connect(qxhexviewx,SIGNAL(_signalPageDown()),this,SLOT(_pageDown()));
    connect(qxhexviewx,SIGNAL(_signalPageUp()),this,SLOT(_pageUp()));

    connect(qxhexviewx,SIGNAL(_signalStartDocument()),this,SLOT(_startDocument()));
    connect(qxhexviewx,SIGNAL(_signalEndDocument()),this,SLOT(_endDocument()));

    scrollbar = new QScrollBar(Qt::Vertical, this);

    //    scrollbar=this->verticalScrollBar();

    //    layout->addWidget(qhexviewx);
    //    layout->addWidget(scrollbar);

    this->setVerticalScrollBar(scrollbar);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    connect(scrollbar,SIGNAL(valueChanged(int)),this,SLOT(scrollSlot(int)));

    qxhexviewx->setData(&datablock);
}

QList<QString> QXHexView::setData(QByteArray(*__ReadMemory)(QVariant,unsigned long long,unsigned long long,bool *),bool (*__WriteMemory)(QVariant,unsigned long long,QByteArray), QVariant parameter, unsigned long long nStartAddress, unsigned long long nMaxSize)
{
    QList<QString> listImages;

    _init(&datablock);

    if(nMaxSize==0)
    {
        // only File!
        QFile file;
        file.setFileName(parameter.toString());
        datablock.nMaxSize=file.size();
    }
    else
    {
        datablock.nMaxSize=nMaxSize;
    }

    if(datablock.nMaxSize>2147483647)
    {
        datablock.nMaxSize=0;
        emit appendError(QString("File is too large"));
        return listImages;
    }

    _initHEX(&datablock);

    datablock.__ReadMemory=__ReadMemory;
    datablock.__WriteMemory=__WriteMemory;
    datablock.nStartAddress=nStartAddress;
    datablock.nCurrentAddress=nStartAddress;

    datablock.parameter=parameter;
    datablock.nSymbolWidth=qxhexviewx->fontMetrics().width(QLatin1Char('T'));
    datablock.nSymbolHeight=qxhexviewx->fontMetrics().height();

    //    rescroll();
    //    scrollSlot(0);
    qxhexviewx->__update(nStartAddress);

    QString sImage=Binary::getType(parameter.toString());

    listImages.append(sImage);

    if(sImage!="Binary")
    {
        listImages.append("Binary");
    }

    setImage(listImages.at(0));

    reloadMemoryMap();

    return listImages;
}

void QXHexView::setType(QString sType, QString sMode, QString sSyntax)
{
    datablock.sType=sType;
    datablock.sMode=sMode;
    datablock.sSyntax=sSyntax;

    if(datablock.sType=="HEX")
    {
        //        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(sMode.toAscii().data()));
        //        qDebug(QTextCodec::codecForCStrings()->name().data());
        _initHEX(&datablock);
    }
    else if(datablock.sType=="x86-ASM")
    {
        _initX86ASM(&datablock);
    }

    if(datablock.nCursor)
    {
        datablock.nCurrentAddress=datablock.nCursor;
    }

    scrollbar->setMaximum(0);

    //    datablock.nCurrentScrollPosition=datablock._addressToScrollPosition(&datablock,datablock.nCurrentAddress);

    if(datablock.__ReadMemory)
    {
        qxhexviewx->__update(datablock.nCurrentAddress);
    }
}

void QXHexView::setAddressAsHEX(bool bState)
{
    datablock.bAddressAsHEX=bState;
    qxhexviewx->update();
}

void QXHexView::setSelection(unsigned long long nSelectionStart, unsigned long long nSelectionSize)
{
    datablock.nStartSelection=nSelectionStart;
    datablock.nEndSelection=nSelectionStart+nSelectionSize;
}

void QXHexView::goToAddress(unsigned long long nAddress)
{
    if((datablock.nStartAddress<=nAddress)&&(nAddress<datablock.nStartAddress+datablock.nMaxSize))
    {
        datablock.nCurrentAddress=nAddress;

        //        int b=scrollbar->maximum();

        if(scrollbar->maximum()==0)
        {
            scrollbar->setRange(0,(datablock.nMaxSize-datablock._getStep())/datablock._getStep());
        }

        scrollbar->setValue(datablock._addressToScrollPosition(&datablock,datablock.nCurrentAddress));
    }
    else
    {
        emit appendError(QString("%1 %2: %3").arg(tr("Invalid")).arg(tr("Address")).arg(nAddress));
    }
}

QByteArray QXHexView::ReadFromFile(QVariant parameter,unsigned long long nOffset,unsigned long long nSize,bool *pbIsReadOnly)
{
    *pbIsReadOnly=true;
    QString sFileName=parameter.toString();
    QByteArray baResult;

    if(!sFileName.isEmpty())
    {
        QFile file;
        bool bIsOpen=false;
        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            *pbIsReadOnly=false;
            bIsOpen=true;
        }

        if(!bIsOpen)
        {
            if(file.open(QIODevice::ReadOnly))
            {
                bIsOpen=true;
            }
        }

        if(bIsOpen)
        {

            file.seek(nOffset);

            baResult.clear();
            baResult=file.read(nSize);
            file.close();
        }
    }

    return baResult;
}

bool QXHexView::WriteToFile(QVariant parameter, unsigned long long nOffset,QByteArray baData)
{
    QString sFileName=parameter.toString();
    bool bResult=false;

    if(!sFileName.isEmpty())
    {
        QFile file;

        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            file.seek(nOffset);

            if(file.write(baData.data(),baData.size())!=-1)
            {
                bResult=true;
            }

            file.close();
        }
    }

    return bResult;
}

void QXHexView::_paintHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    if(_pDataBlock->__ReadMemory==0)
    {
        return;
    }
    //#ifdef QT_DEBUG
    //    QElapsedTimer timer;
    //    timer.start();
    //#endif
    int nSymbolWidth=_pDataBlock->nSymbolWidth;
    int nSymbolHeight=_pDataBlock->nSymbolHeight;
    int nLineCount=_pDataBlock->nLineCount;

    unsigned long long nAddress=_pDataBlock->nCurrentAddress;

    unsigned long long nStartSelection=_pDataBlock->nStartSelection;
    unsigned long long nEndSelection=_pDataBlock->nEndSelection;
    unsigned long long nNextSelection=_pDataBlock->nNextSelection;
    unsigned long long nCursor=_pDataBlock->nCursor;

    bool bShowCursor=_pDataBlock->bShowCursor;
    bool bCursorH=_pDataBlock->bCursorH;
    bool bCursorL=_pDataBlock->bCursorL;

    _pDataBlock->bShowCursor=!bShowCursor;

    if(nStartSelection>=nEndSelection)
    {
        qSwap(nStartSelection,nEndSelection);
        nEndSelection=nNextSelection;
    }

    ((QXHexViewX *)pWidget)->emitSelectionStartChanged(_pDataBlock->_offsetToAddress(_pDataBlock,nStartSelection));

    ((QXHexViewX *)pWidget)->emitSelectionSizeChanged(nEndSelection-nStartSelection);

    unsigned int nReadBlockSize=nLineCount*(pWidget->rect().height()/nSymbolHeight);

    nReadBlockSize=qMin(nReadBlockSize,(unsigned int)(_pDataBlock->nMaxSize-_pDataBlock->nCurrentAddress+_pDataBlock->nStartAddress));
    _pDataBlock->nReadBlockSize=nReadBlockSize;
    ((QXHexViewX *)pWidget)->read(); // Get Buffer Size

    //    int nSize=_pDataBlock->baBuffer.size();
    int nSize=nReadBlockSize;

    //    qDebug("Readblocksize=%d",nReadBlockSize);
    //    char *pOffset=_pDataBlock->baBuffer.data();
    //    int nSize2=nLineCount*pWidget->rect().height()/nSymbolHeight-1;
    if(nCursor<nAddress)
    {
        nCursor=nAddress;
        _pDataBlock->nCursor=nCursor;
    }
    else if(nCursor>nAddress+nSize-1)
    {
        nCursor=nAddress+nSize-1;
        _pDataBlock->nCursor=nCursor;
    }

    //    QString sHEX=ConvertToHex(&_pDataBlock->baBuffer);
    //    QString sANSI=ConvertToAnsi(&_pDataBlock->baBuffer);

    ((QXHexViewX *)pWidget)->emitCursorChanged(_pDataBlock->_offsetToAddress(_pDataBlock,nCursor));

    int nAddressWidth=_pDataBlock->nAddressWidth;
    int xPosAddress=0;
    int xPosHex=xPosAddress+nSymbolWidth*(nAddressWidth+2);
    int xPosAnsi=xPosHex+nSymbolWidth*(nLineCount*3+1);
    int xPosEnd=xPosAnsi+nSymbolWidth*(nLineCount+2);
    int xIndent=nSymbolWidth;
    int yIndent=nSymbolHeight;

    QPainter painter(pWidget);

    painter.setPen(pWidget->palette().color(QPalette::Text));
    painter.setBackground(pWidget->palette().brush(QPalette::Background));
    painter.setBackgroundMode(Qt::OpaqueMode);
    //    painter.setBackgroundMode(Qt::TransparentMode);
    // Draw lines
    //    painter.drawLine(pWidget->rect().x()+xPosAddress,pWidget->rect().y(),pWidget->rect().x()+xPosAddress,pWidget->rect().height());

    //#ifdef QT_DEBUG
    //    qDebug("Paint :%d ms",timer.elapsed());
    //#endif

    QString sTemp;

    bool bSel=false;
    bool bEndSel=false;
    bool bCursor=false;
    bool bMod=false;

    for(int i=0,j=-1; i<nSize; i++)
    {
        // address
        if(i%nLineCount==0)
        {
            j++;

            if(_pDataBlock->bAddressAsHEX)
            {
                sTemp=(QString("%1").arg(_pDataBlock->_offsetToAddress(_pDataBlock,nAddress+nLineCount*j),nAddressWidth,16,QChar('0')));
            }
            else
            {
                sTemp=(QString("%1").arg(_pDataBlock->_offsetToAddress(_pDataBlock,nAddress+nLineCount*j),nAddressWidth,10,QChar('0')));
            }


            for(int k=0; k<sTemp.length(); k++)
            {
                painter.drawText(xPosAddress+xIndent+nSymbolWidth*k,pWidget->rect().y()+nSymbolHeight*(j)+yIndent,sTemp.at(k));
            }
        }

        bMod=((QXHexViewX *)pWidget)->getModAddresses()->contains(nAddress+i);

        if(((nStartSelection<=(nAddress+i))&&((nEndSelection>(nAddress+i)))))
        {
            bSel=true;
        }

        if(nEndSelection==(nAddress+i+1))
        {
            bEndSel=true;
        }

        if((nCursor==nAddress+i)&&(bShowCursor))
        {
            bCursor=true;
        }

        if(bSel)
        {
            _paintRevert(pWidget,&painter);
        }

        if(bMod)
        {
            _paintFontRevert(pWidget,&painter);
        }

#ifdef QT_DEBUG
        if(_pDataBlock->sHEX.length()<=2*(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i))
        {
            qDebug("HEX lenght :%d index %d",_pDataBlock->sHEX.length(),2*(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i));
        }
#endif
        if(bCursor&&(bCursorH||bCursorL))
        {
            if(bCursorH)
            {
                _paintRevert(pWidget,&painter);
                //                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3),pWidget->rect().y()+nSymbolHeight*(j)+yIndent,ByteToHex((unsigned char)(*(pOffset+i))).at(0));
                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3),pWidget->rect().y()+nSymbolHeight*(j)+yIndent,_pDataBlock->sHEX.at(2*(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i)));
                _paintRevert(pWidget,&painter);
                //                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3)+nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(j)+yIndent,ByteToHex((unsigned char)(*(pOffset+i))).at(1));
                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3)+nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(j)+yIndent,_pDataBlock->sHEX.at(2*(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i)+1));
            }
            else if(bCursorL)
            {
                //                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3),pWidget->rect().y()+nSymbolHeight*(j)+yIndent,ByteToHex((unsigned char)(*(pOffset+i))).at(0));
                //                _paintRevert(pWidget,&painter);
                //                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3)+nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(j)+yIndent,ByteToHex((unsigned char)(*(pOffset+i))).at(1));
                //                _paintRevert(pWidget,&painter);
                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3),pWidget->rect().y()+nSymbolHeight*(j)+yIndent,_pDataBlock->sHEX.at(2*(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i)));
                _paintRevert(pWidget,&painter);
                painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3)+nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(j)+yIndent,_pDataBlock->sHEX.at(2*(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i)+1));
                _paintRevert(pWidget,&painter);
            }
        }
        else
        {
            painter.drawText(xPosHex+xIndent+(i%nLineCount)*nSymbolWidth*3,pWidget->rect().y()+nSymbolHeight*(j)+yIndent,_pDataBlock->sHEX.mid(2*(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i),2));
        }

        if((i%nLineCount!=15)&&(!bEndSel))
        {
            painter.drawText(xPosHex+xIndent+(i%nLineCount)*(nSymbolWidth*3)+2*nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(j)+yIndent," ");
        }

        //        if(bCursor)
        //        {
        //            _paintRevert(pWidget,&painter);
        //        }
        //        if(bCursor)
        //        {
        //            _paintRevert(pWidget,&painter);
        //        }

        if(bCursor&&(!bCursorH)&&(!bCursorL))
        {
            _paintRevert(pWidget,&painter);
        }

#ifdef QT_DEBUG
        if(_pDataBlock->sANSI.length()<=_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i)
        {
            qDebug("ANSI lenght :%d index %d",_pDataBlock->sANSI.length(),(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i));
        }
#endif
        int __nSize=_pDataBlock->sANSI.length();
        int __i=(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress+i);
        QChar _chSymbol=_pDataBlock->sANSI.at(__i);
        //        qDebug("i=%d %s",__i,_pDataBlock->sANSI.mid(__i,1).toLatin1().data());
        painter.drawText(xPosAnsi+xIndent+(i%nLineCount)*nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(j)+yIndent,_chSymbol);

        if(bCursor&&(!bCursorH)&&(!bCursorL))
        {
            _paintRevert(pWidget,&painter);
        }

        if(bMod)
        {
            _paintFontRevert(pWidget,&painter);
        }

        if(bSel)
        {
            _paintRevert(pWidget,&painter);
        }

        bCursor=false;
        bSel=false;
        bEndSel=false;
    }

    painter.drawLine(pWidget->rect().x()+xPosHex,pWidget->rect().y(),pWidget->rect().x()+xPosHex,pWidget->rect().height());
    painter.drawLine(pWidget->rect().x()+xPosAnsi,pWidget->rect().y(),pWidget->rect().x()+xPosAnsi,pWidget->rect().height());
    painter.drawLine(pWidget->rect().x()+xPosEnd,pWidget->rect().y(),pWidget->rect().x()+xPosEnd,pWidget->rect().height());
    //#ifdef QT_DEBUG
    //    qDebug("_Paint :%d ms",timer.elapsed());
    //#endif
}

bool QXHexView::_mouseEventHEX(QWidget *pWidget, QEvent *event)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    //    if(mouseEvent->button()!=Qt::LeftButton)
    //    {
    //        return false;
    //    }
    //    qDebug("%X",_pDataBlock->nReadBlockSize);
    int nSymbolWidth=_pDataBlock->nSymbolWidth;
    int nSymbolHeight=_pDataBlock->nSymbolHeight;
    int nLineCount=_pDataBlock->nLineCount;

    unsigned long long nCurrentAddress=_pDataBlock->nCurrentAddress;
    unsigned long long nAddress;

    int nAddressWidth=_pDataBlock->nAddressWidth;
    int xPosAddress=0;
    int xPosHex=xPosAddress+nSymbolWidth*(nAddressWidth+2);
    int xPosAnsi=xPosHex+nSymbolWidth*(nLineCount*3+1);
    int xPosEnd=xPosAnsi+nSymbolWidth*(nLineCount+2);
    int xIndent=nSymbolWidth;
    int yIndent=nSymbolHeight;

    int nX=mouseEvent->pos().x();
    int nY=mouseEvent->pos().y();

    if(nX<0)
    {
        return false;
    }

    if(nY<0)
    {
        return false;
    }

    if(nX>pWidget->rect().width())
    {
        return false;
    }

    if(nY>pWidget->rect().height())
    {
        return false;
    }

    _pDataBlock->bCursorH=false;
    _pDataBlock->bCursorL=false;

    if((nX>xPosAddress)&&(nX<xPosHex))
    {
        nAddress=((nY)/nSymbolHeight)*(nLineCount);
        _pDataBlock->bCursorH=true;
    }
    else if((nX>xPosHex)&&(nX<xPosAnsi))
    {
        nAddress=((nY)/nSymbolHeight)*(nLineCount)+(nX-xIndent-xPosHex)/(nSymbolWidth*3);

        if(((nX-xIndent-xPosHex)/(nSymbolWidth)%3)==0)
        {
            _pDataBlock->bCursorH=true;
        }
        else
        {
            _pDataBlock->bCursorL=true;
        }

    }
    else if((nX>xPosAnsi)&&(nX<xPosEnd))
    {
        nAddress=((nY)/nSymbolHeight)*(nLineCount)+(nX-xIndent-xPosAnsi)/(nSymbolWidth);
    }
    else
    {
        nAddress=-1;
    }

    if(nAddress!=-1)
    {
        if(nAddress<_pDataBlock->nReadBlockSize)
        {
            nAddress+=nCurrentAddress;

            if(nAddress>=(_pDataBlock->nStartAddress+_pDataBlock->nMaxSize))
            {
                nAddress=-1;
            }
        }
        else
        {
            nAddress=-1;
        }
    }

    if(nAddress!=-1)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            //            _pDataBlock->nStartSelection=nAddress;
            //            _pDataBlock->nEndSelection=nAddress+1;

            _pDataBlock->nCursor=nAddress;
            _pDataBlock->nFirstClick=nAddress;

            ((QXHexViewX *)pWidget)->__update();
        }
        //        else if(event->type() == QEvent::MouseButtonRelease)
        //        {
        //            _pDataBlock->nStartSelection=_pDataBlock->nCursor;
        //            _pDataBlock->nNextSelection=_pDataBlock->nCursor+1;

        //            if(_pDataBlock->nEndSelection>_pDataBlock->nStartSelection)
        //            {
        //                _pDataBlock->nEndSelection=nAddress+1;
        //            }
        //            else
        //            {
        //                _pDataBlock->nEndSelection=nAddress;
        //            }
        //        }
        else if(event->type() == QEvent::MouseMove)
        {
            _pDataBlock->nStartSelection=_pDataBlock->nFirstClick;
            _pDataBlock->nNextSelection=_pDataBlock->nFirstClick+1;


            _pDataBlock->bShowCursor=false;

            //            _pDataBlock->nEndSelection=nAddress+1;
            if(_pDataBlock->nEndSelection>_pDataBlock->nStartSelection)
            {
                _pDataBlock->nEndSelection=nAddress+1;
            }
            else
            {
                _pDataBlock->nEndSelection=nAddress;
            }

            //            qDebug("Start sel %d",_pDataBlock->nStartSelection);
            //            qDebug("End sel %d",_pDataBlock->nEndSelection);
            //            qDebug("Next sel %d",_pDataBlock->nNextSelection);

            ((QXHexViewX *)pWidget)->__update();
        }
    }

    return false;
}

void QXHexView::_keyPressEventHEX(QWidget *pWidget, QKeyEvent *event)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    if(event->key()==Qt::Key_Left)
    {
        _moveLeftHEX(pWidget);
    }
    else if(event->key()==Qt::Key_Right)
    {
        _moveRightHEX(pWidget);
    }
    else if(event->key()==Qt::Key_Up)
    {
        _moveUpHEX(pWidget);
    }
    else if(event->key()==Qt::Key_Down)
    {
        _moveDownHEX(pWidget);
    }
    else if(event->key()==Qt::Key_PageDown)
    {
        _movePageDownHEX(pWidget);
    }
    else if(event->key()==Qt::Key_PageUp)
    {
        _movePageUpHEX(pWidget);
    }
    else if(!(_pDataBlock->bIsReadOnlyMode)&&(!(_pDataBlock->bIsReadOnly)))
    {
        _writeCharHEX(pWidget,event->key(),event->modifiers(),event->text());
    }
}

void QXHexView::_contextMenuHEX(QWidget *pWidget, QPoint pos)
{
    _contextMenu(pWidget,pos);
}

void QXHexView::_contextMenu(QWidget *pWidget, QPoint pos)
{
    QMenu *menu = new QMenu(pWidget);
    QMenu *menuSelect= new QMenu(tr("Select"),pWidget);
    QMenu *menuCopy= new QMenu(tr("Copy"),pWidget);
    QMenu *menuSearch= new QMenu(tr("Search"),pWidget);

    QAction *actGoToAddress=new QAction(tr("Go to address"), pWidget);
    actGoToAddress->setShortcut(QKeySequence("Alt+G"));
    connect(actGoToAddress,SIGNAL(triggered()),pWidget,SLOT(goToAddress()),Qt::QueuedConnection); // !!!!

    QAction *actSearchSignature=new QAction(tr("Signature"), pWidget);
    actSearchSignature->setShortcut(QKeySequence("Ctrl+F"));
    connect(actSearchSignature,SIGNAL(triggered()),pWidget,SLOT(searchData()),Qt::QueuedConnection); // !!!!
    QAction *actSearchText=new QAction(tr("Text"), pWidget);
    actSearchText->setShortcut(QKeySequence("Ctrl+T"));
    connect(actSearchText,SIGNAL(triggered()),pWidget,SLOT(searchText()),Qt::QueuedConnection); // !!!!
    QAction *actSearchValue=new QAction(tr("Value"), pWidget);
    actSearchValue->setShortcut(QKeySequence("Ctrl+H"));
    connect(actSearchValue,SIGNAL(triggered()),pWidget,SLOT(searchValue()),Qt::QueuedConnection); // !!!!

    QAction *actSearchNext=new QAction(tr("Next"), pWidget);

#ifdef Q_OS_MAC
    actSearchNext->setShortcut(QKeySequence("Ctrl+G"));
#else
    actSearchNext->setShortcut(QKeySequence("F3"));
#endif
    connect(actSearchNext,SIGNAL(triggered()),pWidget,SLOT(findNext()));

    QAction *actDump=new QAction(tr("Dump"), pWidget);
    actDump->setShortcut(QKeySequence("Ctrl+D"));
    connect(actDump,SIGNAL(triggered()),pWidget,SLOT(dump()));

    QAction *actSelectAll=new QAction(tr("Select all"), pWidget);
    actSelectAll->setShortcut(QKeySequence("Ctrl+A"));
    connect(actSelectAll,SIGNAL(triggered()),pWidget,SLOT(selectAll()));

    QAction *actSelectReset=new QAction(tr("Selection reset"), pWidget);
    //    actSelectAll->setShortcut(QKeySequence("Ctrl+A"));
    connect(actSelectReset,SIGNAL(triggered()),pWidget,SLOT(selectReset()));
    QAction *actCopyAsHEX=new QAction(tr("as HEX(0x1000 bytes only)"), pWidget);
    actCopyAsHEX->setShortcut(QKeySequence("Ctrl+C"));
    connect(actCopyAsHEX,SIGNAL(triggered()),pWidget,SLOT(copyAsHEX()));
    QAction *actCopyAsHEXWithSpaces=new QAction(tr("as HEX with spaces(0x1000 bytes only)"), pWidget);
    //    actCopyAsHEX->setShortcut(QKeySequence("Ctrl+C"));
    connect(actCopyAsHEXWithSpaces,SIGNAL(triggered()),pWidget,SLOT(copyAsHEXWithSpaces()));
    QAction *actCopyAsANSI=new QAction(tr("as ANSI(0x1000 bytes only)"), pWidget);
    //    actCopyAsHEX->setShortcut(QKeySequence("Ctrl+C"));
    connect(actCopyAsANSI,SIGNAL(triggered()),pWidget,SLOT(copyAsANSI()));

    QAction *actCopyAsSignature=new QAction(tr("as signature(0x80 bytes only)"), pWidget);
    connect(actCopyAsSignature,SIGNAL(triggered()),pWidget,SLOT(copyAsSignature()),Qt::QueuedConnection); // !!!

    menuSelect->addAction(actSelectAll);
    menuSelect->addAction(actSelectReset);
    menu->addMenu(menuSelect);
    menu->addAction(actGoToAddress);
    //    menu->addAction(actSearch);

    if(((QXHexViewX *)pWidget)->isSelectionPresent())
    {
        pWidget->addAction(actDump);
        menu->addAction(actDump);

        //        actDump->setShortcutContext(Qt::ApplicationShortcut);
        menuCopy->addAction(actCopyAsHEX);
        menuCopy->addAction(actCopyAsHEXWithSpaces);
        menuCopy->addAction(actCopyAsANSI);
        menuCopy->addAction(actCopyAsSignature);
        menu->addMenu(menuCopy);
    }

    menuSearch->addAction(actSearchSignature);
    menuSearch->addAction(actSearchText);
    menuSearch->addAction(actSearchValue);
    menuSearch->addAction(actSearchNext);
    menu->addMenu(menuSearch);

    menu->exec(pos);

    delete actSelectAll;
    delete actSelectReset;
    delete actGoToAddress;
    delete actCopyAsHEX;
    delete actCopyAsHEXWithSpaces;
    delete actCopyAsANSI;
    delete actDump;
    delete actSearchSignature;
    delete actSearchText;
    delete actSearchValue;
    delete actSearchNext;
    delete menuSelect;
    delete menuCopy;
    delete menuSearch;
    delete menu;
}

//void QXHexView::_dump(QXHexViewX *pWidget,unsigned long long nAddress,unsigned long long nSize)
//{

////    qDebug("Size; %d",nSize);

//    QByteArray baTemp;

//    QString sDumpFileName=QFileDialog::getSaveFileName(pWidget, tr("Save File..."),"Dump.bin", tr("Dump files (*.bin);;All files (*)"));
//    if (!sDumpFileName.isEmpty())
//    {
//        QFile file;

//        file.setFileName(sDumpFileName);

//        if(file.open(QIODevice::ReadWrite))
//        {
//            file.resize(0);

//            for(unsigned long long i=0;i<nSize;i+=0x1000)
//            {
//                baTemp.append(pWidget->read(nAddress+i,0x1000));
//            }

//            baTemp.resize(nSize);

//            file.write(baTemp);

//            file.close();
//        }
//        else
//        {
//            pWidget->emitError("Cannot save to file: "+sDumpFileName);
//        }
//    }
//}

void QXHexView::_paintRevert(QWidget *pWidget,QPainter *pPainter)
{
    if(pPainter->background()==pWidget->palette().brush(QPalette::Text))
    {
        pPainter->setBackground(pWidget->palette().brush(QPalette::Background));
    }
    else if(pPainter->background()==pWidget->palette().brush(QPalette::Background))
    {
        pPainter->setBackground(pWidget->palette().brush(QPalette::Text));
    }

    if(pPainter->pen()==pWidget->palette().color(QPalette::Text))
    {
        pPainter->setPen(pWidget->palette().color(QPalette::Background));
    }
    else if(pPainter->pen()==pWidget->palette().color(QPalette::Background))
    {
        pPainter->setPen(pWidget->palette().color(QPalette::Text));
    }
}

void QXHexView::_paintFontRevert(QWidget *pWidget, QPainter *pPainter)
{
    QFont font=pPainter->font();

    if(font.weight()==QFont::Normal)
    {
        font.setWeight(QFont::Black);
    }
    else if(font.weight()==QFont::Black)
    {
        font.setWeight(QFont::Normal);
    }

    pPainter->setFont(font);
}

int QXHexView::_getStepHEX()
{
    return 16;
}

void QXHexView::_init(__DATABLOCK *pData)
{
    pData->__ReadMemory=0;
    pData->_paint=0; //!!!!
    pData->_mouseEvent=0;
    pData->_contextMenu=0;
    pData->_keyPressEvent=0;
    pData->_action[0]=0;
    pData->_getStep=0;
    pData->_addressToScrollPosition=0;
    pData->_scrollPositionToAddress=0;
    pData->nCurrentAddress=0;
    pData->nStartSelection=0;
    pData->nEndSelection=0;
    pData->nStartAddress=0;
    pData->nCursor=0;
    pData->nNextSelection=0;
    pData->nMaxSize=0;
    //    pData->nStep=0;
    pData->nAddressDelta=0;
    pData->bAddressAsHEX=true;
    pData->nCurrentScrollPosition=0;
    pData->nFirstClick=0;
    pData->nFirstClickNext=0;
    pData->nAddressWidth=10;

    pData->nBufferAddress=0;
    //    pData->nBufferSize=0;
    pData->bVorBuffer=true;

    pData->bIsMemoryMap=false;
}

void QXHexView::_initHEX(__DATABLOCK *pData)
{

    pData->_paint=&_paintHEX; //!!!!
    pData->_mouseEvent=&_mouseEventHEX;
    pData->_keyPressEvent=&_keyPressEventHEX;
    pData->_contextMenu=&_contextMenuHEX;
    //    pData->_action[0]=&_dump;
    pData->_getStep=&_getStepHEX;
    pData->_addressToScrollPosition=&_addressToScrollPositionHEX;
    pData->_scrollPositionToAddress=&_scrollPositionToAddressHEX;
    pData->_offsetToAddress=&offsetToAddress;
    pData->nReadBlockSize=qMin((unsigned long long)0x4000,pData->nMaxSize);
    pData->nReadBlockMax=qMin((unsigned long long)0x4000,pData->nMaxSize);
    pData->nLineCount=16;
}

void QXHexView::scrollSlot(int nScrollPosition)
{
    //    qDebug("nScrollPosition %d",nScrollPosition);

    if((nScrollPosition==0)&&(scrollbar->maximum()==0))
        //    if(scrollbar->maximum()==0)
    {
        scrollbar->setRange(0,(datablock.nMaxSize-datablock._getStep())/datablock._getStep());
        scrollbar->setValue(datablock._addressToScrollPosition(&datablock,datablock.nCurrentAddress));
    }
    else
    {
        if(datablock.__ReadMemory)
        {
            if(nScrollPosition!=(int)datablock.nCurrentScrollPosition)
            {
                unsigned long long nAddress=datablock._scrollPositionToAddress(&datablock,nScrollPosition);

                qxhexviewx->__update(nAddress);
                scrollbar->setValue((int)datablock.nCurrentScrollPosition);
                //                qDebug("Update1 %X",datablock.nCurrentScrollPosition);
                //                qDebug("nScrollPosition %d",nScrollPosition);

                //                qDebug("Update2 %X",datablock.nCurrentScrollPosition);
            }
        }
    }
}

void QXHexView::rescroll()
{
    int nMax=(datablock.nMaxSize-datablock._getStep())/datablock._getStep();

    if(nMax!=scrollbar->maximum())
    {
        scrollbar->setRange(0,nMax);
    }
}

void QXHexView::__update()
{
    qxhexviewx->__update();
}

unsigned long long QXHexView::offsetToAddress(__DATABLOCK *pData,unsigned long long nOffset)
{
    unsigned long long _nOffset;
    unsigned long long _nAddress;
    unsigned long long _nSize;

    for(int i=0; i<pData->records.count(); i++)
    {
        _nOffset=pData->records.at(i).nOffset;
        _nAddress=pData->records.at(i).nAddress;
        _nSize=pData->records.at(i).nSize;

        if((_nOffset<=nOffset)&&(nOffset<_nOffset+_nSize))
        {
            return nOffset-_nOffset+_nAddress;
        }
    }

    return nOffset;
}

unsigned long long QXHexView::addressToOffset(__DATABLOCK *pData, unsigned long long nAddress)
{
    unsigned long long _nOffset;
    unsigned long long _nAddress;
    unsigned long long _nSize;

    for(int i=0; i<pData->records.count(); i++)
    {
        _nOffset=pData->records.at(i).nOffset;
        _nAddress=pData->records.at(i).nAddress;
        _nSize=pData->records.at(i).nSize;

        if((_nAddress<=nAddress)&&(nAddress<_nAddress+_nSize))
        {
            return nAddress-_nAddress+_nOffset;
        }
    }

    return nAddress;
}

void QXHexView::setReadOnlyMode(bool bState)
{
    datablock.bIsReadOnlyMode=bState;
}

void QXHexView::reload()
{
    qxhexviewx->reload();
}

unsigned long long QXHexView::_addressToScrollPositionHEX(__DATABLOCK *pData,unsigned long long nAddress)
{
    pData->nAddressDelta=(nAddress-(pData->nStartAddress))%(pData->nLineCount);
    pData->nCurrentScrollPosition=(nAddress-(pData->nStartAddress))/(pData->nLineCount);

    return pData->nCurrentScrollPosition;
}

unsigned long long QXHexView::_scrollPositionToAddressHEX(__DATABLOCK *pData,unsigned long long nScrollPosition)
{
    unsigned long long nResult=0;

    if(nScrollPosition==0)
    {
        nResult=0;
        pData->nAddressDelta=0;
    }
    else
    {
        nResult=nScrollPosition*(pData->nLineCount)+pData->nAddressDelta;
    }

    pData->nCurrentScrollPosition=nScrollPosition;

    return nResult+(pData->nStartAddress);
}

int QXHexView::_getRealSizeX86ASM(__DATABLOCK *pDataBlock,char *pData, int nDataSize, int nNumberofLines)
{
    int nSize=0;
    int nLen=0;
    int k=0;

    while(nSize<nDataSize)
    {
        nLen=_getOpcodeSizeX86ASM(pDataBlock,pData+nSize,nDataSize-nSize);

        nSize+=nLen;

        if(k>=nNumberofLines)
        {
            break;
        }

        k++;
    }

    return nSize;
}

void QXHexView::_paintX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    if(_pDataBlock->__ReadMemory==0)
    {
        return;
    }

    int nSymbolWidth=_pDataBlock->nSymbolWidth;
    int nSymbolHeight=_pDataBlock->nSymbolHeight;
    int nLineCount=_pDataBlock->nLineCount;
    bool bSel=false;
    bool bCursor=false;
    bool bCursorH=_pDataBlock->bCursorH;
    bool bCursorL=_pDataBlock->bCursorL;

    if(((!bCursorH)&&(!bCursorL)))
    {
        bCursorH=true;
        _pDataBlock->bCursorH=true;
    }

    unsigned long long nAddress=_pDataBlock->nCurrentAddress;

    //    qDebug("Current Address %d",nAddress);

    unsigned long long nStartSelection=_pDataBlock->nStartSelection;
    unsigned long long nEndSelection=_pDataBlock->nEndSelection;
    unsigned long long nNextSelection=_pDataBlock->nNextSelection;

    unsigned long long nCursor=_pDataBlock->nCursor;

    bool bShowCursor=_pDataBlock->bShowCursor;
    {
        _pDataBlock->bShowCursor=!bShowCursor;
    }

    //    qDebug("__Start selection %X",nStartSelection);
    //    qDebug("__Next selection %X",nNextSelection);
    //    qDebug("__End selection %X",nEndSelection);

    if(nStartSelection>=nEndSelection)
    {
        qSwap(nStartSelection,nEndSelection);
        nEndSelection=nNextSelection;
    }

    //    qDebug("_Start selection %X",nStartSelection);
    //    qDebug("_Next selection %X",nNextSelection);
    //    qDebug("_End selection %X",nEndSelection);

    //    qDebug("nStartSelection Address %d",nStartSelection);
    //    qDebug("nEndSelection Address %d",nEndSelection);

    ((QXHexViewX *)pWidget)->emitSelectionStartChanged(_pDataBlock->_offsetToAddress(_pDataBlock,nStartSelection));

    ((QXHexViewX *)pWidget)->emitSelectionSizeChanged(nEndSelection-nStartSelection);
    //    if(nEndSelection-nStartSelection)
    //    {
    //        pWidget->emitSelectionSizeChanged(nEndSelection-nStartSelection);
    //    }
    //    else
    //    {
    //        pWidget->emitSelectionSizeChanged(1);
    //    }

    unsigned int nReadBlockSize=nLineCount*(pWidget->rect().height()/nSymbolHeight);
    int nScreenLines=pWidget->rect().height()/nSymbolHeight;

    nReadBlockSize=qMin(nReadBlockSize,(unsigned int)(_pDataBlock->nMaxSize-_pDataBlock->nCurrentAddress+_pDataBlock->nStartAddress));
    _pDataBlock->nReadBlockSize=nReadBlockSize;
    //    qDebug("%d",nReadBlockSize);
    ((QXHexViewX *)pWidget)->read(); // Get Buffer Size

    //    int nSize=_pDataBlock->baBuffer.size();
    int nSize=nReadBlockSize;

    char *pOffset=_pDataBlock->baBuffer.data()+(_pDataBlock->nCurrentAddress-_pDataBlock->nBufferAddress);

    if(_pDataBlock->sHEX.at(0)==QChar('?'))
    {
        return; //
    }
    else
    {
        nSize=_getRealSizeX86ASM(_pDataBlock,pOffset,nSize,nScreenLines-1);
    }

#ifdef QT_DEBUG
    qDebug("ReadBlockSize=%d",nSize);
#endif
    _pDataBlock->nReadBlockSize=nSize;
    //    int nSize2=_getRealSizeX86ASM(_pDataBlock,pOffset,nSize,nScreenLines-1);

    if((nCursor<nAddress)||(nCursor>=nAddress+nSize))
    {
        nCursor=nAddress;
        _pDataBlock->nCursor=nCursor;
    }

    ((QXHexViewX *)pWidget)->emitCursorChanged(_pDataBlock->_offsetToAddress(_pDataBlock,nCursor));

    //    int nAddressWidth=(QString("%1").arg(nAddress+nReadBlockSize)).length();
    int nAddressWidth=_pDataBlock->nAddressWidth;
    int xPosAddress=0;
    int xPosHex=xPosAddress+nSymbolWidth*(nAddressWidth+2);
    int xPosDisasm=xPosHex+nSymbolWidth*(nLineCount*2+2);
    int xIndent=nSymbolWidth;
    int yIndent=nSymbolHeight;

    QPainter painter(pWidget);

    painter.setPen(pWidget->palette().color(QPalette::Text));
    painter.setBackground(pWidget->palette().color(QPalette::Background));
    painter.setBackgroundMode(Qt::OpaqueMode);
    // Draw lines
    //    painter.drawLine(pWidget->rect().x()+xPosAddress,pWidget->rect().y(),pWidget->rect().x()+xPosAddress,pWidget->rect().height());
    painter.drawLine(pWidget->rect().x()+xPosHex,pWidget->rect().y(),pWidget->rect().x()+xPosHex,pWidget->rect().height());
    painter.drawLine(pWidget->rect().x()+xPosDisasm,pWidget->rect().y(),pWidget->rect().x()+xPosDisasm,pWidget->rect().height());

    QString sTemp;

    _pDataBlock->listAddresses.clear();
    _pDataBlock->listSizes.clear();

    DISASM MyDisasm;
    memset(&MyDisasm,0,sizeof(MyDisasm));

    if(_pDataBlock->sMode=="16")
    {
        MyDisasm.Archi=0x10;
    }
    else if(_pDataBlock->sMode=="32")
    {
        MyDisasm.Archi=0;
    }
    else if(_pDataBlock->sMode=="64")
    {
        MyDisasm.Archi=0x40;
    }

    if(_pDataBlock->sSyntax=="MASM")
    {
        MyDisasm.Options=0x000;
    }
    else if(_pDataBlock->sSyntax=="GOASM")
    {
        MyDisasm.Options=0x100;
    }
    else if(_pDataBlock->sSyntax=="NASM")
    {
        MyDisasm.Options=0x200;
    }
    else if(_pDataBlock->sSyntax=="AT")
    {
        MyDisasm.Options=0x400;
    }

    //        MyDisasm.Archi=pOptions->nAsmMode;
    //        MyDisasm.Options=pOptions->nAsmSyntax;

    MyDisasm.EIP=(UIntPtr)pOffset;

    int nError=0;
    int nLen=0;
    int k=0;
    QString sOpcode;
    QString sBinary;
    int i=0;

    for(; !nError;)
    {
        MyDisasm.SecurityBlock = (UIntPtr)(pOffset+nSize+1)-(UIntPtr)MyDisasm.EIP;
        MyDisasm.VirtualAddr=_pDataBlock->_offsetToAddress(_pDataBlock,i+nAddress);

        //        qDebug("SB: %d",MyDisasm.SecurityBlock);
        //        qDebug("VA: %d",MyDisasm.VirtualAddr);
        //        qDebug("Offset: %X",pOffset);

        sBinary.clear();
        sOpcode.clear();

        if(MyDisasm.SecurityBlock>0)
        {
            nLen = Disasm(&MyDisasm);

            if(nLen==OUT_OF_BLOCK)
            {
                sOpcode="invalid opcode";
                MyDisasm.EIP++;
                nLen=1;
            }
            else if(nLen==UNKNOWN_OPCODE)
            {
                sOpcode="invalid opcode";
                MyDisasm.EIP++;
                nLen=1;
            }
            else
            {
                sOpcode=MyDisasm.CompleteInstr;
                //                qDebug(sOpcode.toAscii().data());

                MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)nLen;
                //            MyDisasm.VirtualAddr=MyDisasm.VirtualAddr=+(UIntPtr)nLen;

                if(MyDisasm.EIP > (UIntPtr)(pOffset+nSize))
                {
                    nError=1;
                }
            }

            //            sOpcode=MyDisasm.CompleteInstr;
        }
        else
        {
            nError=1;
        }

        if(!nError)
        {
            // address
            {
                _pDataBlock->listAddresses.append(nAddress+i);
                _pDataBlock->listSizes.append(nLen);

                if(_pDataBlock->bAddressAsHEX)
                {
                    sTemp=(QString("%1").arg(MyDisasm.VirtualAddr,nAddressWidth,16,QChar('0')));
                }
                else
                {
                    sTemp=(QString("%1").arg(MyDisasm.VirtualAddr,nAddressWidth,10,QChar('0')));
                }

                painter.drawText(xPosAddress+xIndent,pWidget->rect().y()+nSymbolHeight*(k)+yIndent,sTemp);
            }

            //            if((nStartSelection<=((nAddress+i))&&((nEndSelection>(nAddress+i)))))
            //            {
            //                bSel=true;

            //            }

            if((nStartSelection>=(nAddress+i))&&(nStartSelection<(nAddress+i+nLen)))
            {
                bSel=true;
            }
            else if((nStartSelection<=(nAddress+i))&&(nEndSelection>(nAddress+i+nLen)))
            {
                bSel=true;
            }
            else if((nEndSelection>(nAddress+i))&&(nEndSelection<=(nAddress+i+nLen)))
            {
                bSel=true;
            }

            if(bSel)
            {
                _paintRevert(pWidget,&painter);
            }

            painter.drawText(xPosDisasm+xIndent,pWidget->rect().y()+nSymbolHeight*(k)+yIndent,sOpcode);

            if(bSel)
            {
                _paintRevert(pWidget,&painter);
            }

            bSel=false;

            // Binary
            for(int j=0; j<nLen; j++)
            {
                if((nStartSelection<=((nAddress+i+j))&&((nEndSelection>(nAddress+i+j)))))
                {
                    bSel=true;
                }

                if((nAddress+i+j==nCursor)&&(bShowCursor))
                {
                    bCursor=true;
                }

                if(bSel)
                {
                    _paintRevert(pWidget,&painter);
                }

                if(bCursor&&(bCursorH||bCursorL))
                {
                    if(bCursorH||((!bCursorH)&&(!bCursorL)))
                    {
                        _paintRevert(pWidget,&painter);
                        painter.drawText(xPosHex+xIndent+(nSymbolWidth*2*j),pWidget->rect().y()+nSymbolHeight*(k)+yIndent,ByteToHex((unsigned char)(*(pOffset+i+j))).at(0));
                        _paintRevert(pWidget,&painter);
                        painter.drawText(xPosHex+xIndent+(nSymbolWidth*2*j)+nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(k)+yIndent,ByteToHex((unsigned char)(*(pOffset+i+j))).at(1));

                    }
                    else if(bCursorL)
                    {

                        painter.drawText(xPosHex+xIndent+(nSymbolWidth*2*j),pWidget->rect().y()+nSymbolHeight*(k)+yIndent,ByteToHex((unsigned char)(*(pOffset+i+j))).at(0));
                        _paintRevert(pWidget,&painter);
                        painter.drawText(xPosHex+xIndent+(nSymbolWidth*2*j)+nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(k)+yIndent,ByteToHex((unsigned char)(*(pOffset+i+j))).at(1));
                        _paintRevert(pWidget,&painter);
                    }

                    //                    painter.drawText(xPosHex+xIndent+(nSymbolWidth*2*j)+2*nSymbolWidth,pWidget->rect().y()+nSymbolHeight*(k)+yIndent," ");
                }
                else
                {
                    painter.drawText(xPosHex+xIndent+(nSymbolWidth*2*j),pWidget->rect().y()+nSymbolHeight*(k)+yIndent,ByteToHex((unsigned char)(*(pOffset+j+i))));
                }

                //                if(bCursor)
                //                {
                //                    _paintRevert(pWidget,&painter);
                //                }

                //                painter.drawText(xPosHex+xIndent+(nSymbolWidth*2*j),pWidget->rect().y()+nSymbolHeight*(k)+yIndent,ByteToHex((unsigned char)(*(pOffset+j+i))));

                //                if(bCursor)
                //                {
                //                    _paintRevert(pWidget,&painter);
                //                }

                if(bSel)
                {
                    //                    painter.setBackground(brUnselected);
                    //                    painter.setPen(peUnselected);
                    _paintRevert(pWidget,&painter);
                }

                bCursor=false;
                bSel=false;
            }
        }

        i+=nLen;
        k++;

        if(k>nScreenLines-1)
        {
            nError=1;
        }

    }
    //    _pDataBlock->listAddresses.append(nAddress+i+nLen);
}

bool QXHexView::_mouseEventX86ASM(QWidget *pWidget, QEvent *event)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    unsigned long long nCurrentAddress=_pDataBlock->nCurrentAddress;
    unsigned long long nAddress;
    //    unsigned long long nAddress2;

    int xPosAddress=0;
    int nSymbolWidth=_pDataBlock->nSymbolWidth;
    int nSymbolHeight=_pDataBlock->nSymbolHeight;
    int nLineCount=_pDataBlock->nLineCount;

    int nAddressWidth=_pDataBlock->nAddressWidth;
    int xIndent=nSymbolWidth;
    int yIndent=nSymbolHeight;

    int xPosHex=xPosAddress+nSymbolWidth*(nAddressWidth+2);
    int xPosDisasm=xPosHex+nSymbolWidth*(nLineCount*2+2);

    int nX=mouseEvent->pos().x();
    int nY=mouseEvent->pos().y();

    if(nX<0)
    {
        return false;
    }

    if(nY<0)
    {
        return false;
    }

    if(nX>pWidget->rect().width())
    {
        return false;
    }

    if(nY>pWidget->rect().height())
    {
        return false;
    }

    nAddress=-1;

    int nIndex=(nY)/nSymbolHeight;

    unsigned long long nSize=(nX-xIndent-xPosHex)/(nSymbolWidth*2);

    bool bHex=false;

    _pDataBlock->bCursorH=false;
    _pDataBlock->bCursorL=false;

    if(nIndex<_pDataBlock->listAddresses.count())
    {
        if((nX>xPosHex)&&(nX<xPosDisasm))
        {
            nAddress=_pDataBlock->listAddresses.at(nIndex)+qMin(nSize,_pDataBlock->listSizes.at(nIndex)-1);

            //        nAddress2=nAddress+1;
            if((((nX-xIndent-xPosHex)/(nSymbolWidth))%2)==0)
            {
                _pDataBlock->bCursorH=true;
            }
            else
            {
                _pDataBlock->bCursorL=true;
            }

            bHex=true;
        }
        else
        {
            nAddress=_pDataBlock->listAddresses.at(nIndex);
            _pDataBlock->bCursorH=true;
        }
    }

    if(nAddress!=-1)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            _pDataBlock->nCursor=nAddress;
            _pDataBlock->nFirstClick=nAddress;
            int nInd=_getIndexFromAddress(_pDataBlock,_pDataBlock->nFirstClick);
            //            if(nInd+1==_pDataBlock->listAddresses.count())
            //            {
            //                _pDataBlock->nFirstClickNext=_pDataBlock->nStartAddress+_pDataBlock->nMaxSize;
            //            }
            //            else
            //            {
            //                _pDataBlock->nFirstClickNext=_pDataBlock->listAddresses.at(nInd+1);
            //            }
            _pDataBlock->nFirstClickNext=_pDataBlock->listAddresses.at(nInd)+_pDataBlock->listSizes.at(nInd);
        }
        else if(event->type() == QEvent::MouseMove)
        {
            _pDataBlock->bShowCursor=false;

            _pDataBlock->nStartSelection=_pDataBlock->nFirstClick;

            if(bHex)
            {
                _pDataBlock->nNextSelection=_pDataBlock->nFirstClick+1;
            }
            else
            {
                //                int nInd=_getIndexFromAddress(_pDataBlock,_pDataBlock->nFirstClick);
                //                qDebug("Index %d",nInd);
                //                qDebug("count %d",_pDataBlock->listAddresses.count());
                //                if(nInd+1==_pDataBlock->listAddresses.count())
                //                {
                //                    _pDataBlock->nNextSelection=_pDataBlock->nStartAddress+_pDataBlock->nMaxSize;
                //                }
                //                else
                //                {
                //                    _pDataBlock->nNextSelection=_pDataBlock->listAddresses.at(nInd+1);
                //                }
                _pDataBlock->nNextSelection=_pDataBlock->nFirstClickNext;
            }

            if(bHex)
            {
                if(_pDataBlock->nEndSelection>_pDataBlock->nStartSelection)
                {
                    _pDataBlock->nEndSelection=nAddress+1;
                }
                else
                {
                    _pDataBlock->nEndSelection=nAddress;
                }
            }
            else
            {
                if(_pDataBlock->nEndSelection>_pDataBlock->nStartSelection)
                {
                    //                    if(nIndex+1==_pDataBlock->listAddresses.count())
                    //                    {
                    //                        _pDataBlock->nEndSelection=_pDataBlock->nStartAddress+_pDataBlock->nMaxSize;
                    //                    }
                    //                    else
                    //                    {
                    //                        _pDataBlock->nEndSelection=_pDataBlock->listAddresses.at(nIndex+1);
                    //                    }
                    //                    qDebug("Address %X",_pDataBlock->listAddresses.at(nIndex));
                    //                    qDebug("Size %X",_pDataBlock->listSizes.at(nIndex));
                    _pDataBlock->nEndSelection=_pDataBlock->listAddresses.at(nIndex)+_pDataBlock->listSizes.at(nIndex);
                }
                else
                {
                    _pDataBlock->nEndSelection=nAddress;
                }
            }
        }
    }
    //    qDebug("Start selection %X",_pDataBlock->nStartSelection);
    //    qDebug("Next selection %X",_pDataBlock->nNextSelection);
    //    qDebug("End selection %X",_pDataBlock->nEndSelection);
    ((QXHexViewX *)pWidget)->__update();

    return false;
}

void QXHexView::_keyPressEventX86ASM(QWidget *pWidget, QKeyEvent *event)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    if(event->key()==Qt::Key_Left)
    {
        _moveLeftX86ASM(pWidget);
    }
    else if(event->key()==Qt::Key_Right)
    {
        _moveRightX86ASM(pWidget);
    }
    else if(event->key()==Qt::Key_Up)
    {
        _moveUpX86ASM(pWidget);
    }
    else if(event->key()==Qt::Key_Down)
    {
        _moveDownX86ASM(pWidget);
    }
    else if(event->key()==Qt::Key_PageDown)
    {
        _movePageDownX86ASM(pWidget);
    }
    else if(event->key()==Qt::Key_PageUp)
    {
        _movePageUpX86ASM(pWidget);
    }
    else if(!(_pDataBlock->bIsReadOnlyMode)&&(!(_pDataBlock->bIsReadOnly)))
    {
        _writeCharX86ASM(pWidget,event->key(),event->modifiers(),event->text());
    }
}

void QXHexView::_contextMenuX86ASM(QWidget *pWidget, QPoint pos)
{
    _contextMenu(pWidget,pos);
}

int QXHexView::_getStepX86ASM()
{
    return 1;
}

unsigned long long QXHexView::_getNextOpcodeAddressX86ASM(QWidget *pWidget, unsigned long long nCurrent)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nResult=0;

    unsigned long long nAddress=qMax((long long)0,(long long)nCurrent-100);

    QByteArray baData=((QXHexViewX *)pWidget)->read(nAddress+_pDataBlock->nStartAddress,200);

    char *pOffset=baData.data();
    int nSize=baData.size();
    QList<int> listAddresses;
    int nLen=0;

    for(int i=0; i<nSize;)
    {
        listAddresses.append(i);
        nLen=_getOpcodeSizeX86ASM(_pDataBlock,pOffset+i,nSize-i);

        i+=nLen;
    }

    for(int i=0; i<listAddresses.count()-1; i++)
    {
        if((nCurrent>=listAddresses.at(i)+nAddress)&&(nCurrent<listAddresses.at(i+1)+nAddress))
        {
            nResult=listAddresses.at(i+1)+nAddress;
            break;
        }
    }

    if(nResult==0)
    {
        nResult=nCurrent;
    }

    return nResult;
}

unsigned long long QXHexView::_getPrevOpcodeAddressX86ASM(QWidget *pWidget, unsigned long long nCurrent)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nResult=0;

    unsigned long long nAddress=qMax((long long)0,(long long)nCurrent-100);

    QByteArray baData=((QXHexViewX *)pWidget)->read(nAddress+_pDataBlock->nStartAddress,200);

    char *pOffset=baData.data();
    int nSize=baData.size();
    QList<int> listAddresses;
    int nLen=0;

    for(int i=0; i<nSize;)
    {
        listAddresses.append(i);
        nLen=_getOpcodeSizeX86ASM(_pDataBlock,pOffset+i,nSize-i);

        i+=nLen;
    }

    bool bNotFound=true;

    for(int i=0; i<listAddresses.count()-1; i++)
    {
        if((nCurrent>=listAddresses.at(i)+nAddress)&&(nCurrent<listAddresses.at(i+1)+nAddress))
        {
            if(i==0)
            {
                nResult=nAddress;
            }
            else
            {
                nResult=listAddresses.at(i-1)+nAddress;
            }

            bNotFound=false;
            break;
        }
    }

    if(bNotFound)
    {
        nResult=nCurrent;
    }

    return nResult;
}

void QXHexView::_initX86ASM(__DATABLOCK *pData)
{
    pData->_paint=&_paintX86ASM; //!!!!
    pData->_mouseEvent=&_mouseEventX86ASM;
    pData->_contextMenu=&_contextMenuX86ASM;
    pData->_keyPressEvent=&_keyPressEventX86ASM;
    //    pData->_action[0]=&_dump;
    pData->_getStep=&_getStepX86ASM;
    pData->_addressToScrollPosition=&_addressToScrollPositionX86ASM;
    pData->_scrollPositionToAddress=&_scrollPositionToAddressX86ASM;
    pData->_offsetToAddress=&offsetToAddress;
    pData->nReadBlockSize=qMin((unsigned long long)0x1000,pData->nMaxSize);
    pData->nReadBlockMax=qMin((unsigned long long)0x1000,pData->nMaxSize);
    pData->nLineCount=15;
}

unsigned long long QXHexView::_addressToScrollPositionX86ASM(__DATABLOCK *pData, unsigned long long nAddress)
{
    pData->nCurrentScrollPosition=nAddress-(pData->nStartAddress);
    return pData->nCurrentScrollPosition;
}

unsigned long long QXHexView::_scrollPositionToAddressX86ASM(__DATABLOCK *pData, unsigned long long nScrollPosition)
{
    if(nScrollPosition==pData->nCurrentScrollPosition)
    {
        return nScrollPosition+(pData->nStartAddress);
    }

    unsigned long long nAddress=qMax((long long)0,(long long)nScrollPosition-100);

    bool bTemp;
    QByteArray baData=pData->__ReadMemory(pData->parameter.toString(),nAddress+pData->nStartAddress,200,&bTemp);

    char *pOffset=baData.data();
    int nSize=baData.size();
    QList<int> listAddresses;
    int nLen=0;

    for(int i=0; i<nSize;)
    {
        listAddresses.append(i);
        nLen=_getOpcodeSizeX86ASM(pData,pOffset+i,nSize-i);

        i+=nLen;
    }

    if(pData->nCurrentScrollPosition<nScrollPosition)
    {
        for(int i=0; i<listAddresses.count()-1; i++)
        {
            if((nScrollPosition>listAddresses.at(i)+nAddress)&&(nScrollPosition<=listAddresses.at(i+1)+nAddress))
            {
                nScrollPosition=listAddresses.at(i+1)+nAddress;
                break;
            }
        }
    }
    else if(pData->nCurrentScrollPosition>nScrollPosition)
    {
        for(int i=0; i<listAddresses.count()-1; i++)
        {
            if((nScrollPosition>=listAddresses.at(i)+nAddress)&&(nScrollPosition<listAddresses.at(i+1)+nAddress))
            {
                nScrollPosition=listAddresses.at(i)+nAddress;
                break;
            }
        }
    }

    pData->nCurrentScrollPosition=nScrollPosition;

    return nScrollPosition+(pData->nStartAddress);
}

int QXHexView::_getOpcodeSizeX86ASM(__DATABLOCK *pDataBlock, char *pData, int nDataSize)
{
    if(!nDataSize)
    {
        return 0;
    }

    DISASM MyDisasm;
    memset(&MyDisasm,0,sizeof(MyDisasm));

    if(pDataBlock->sMode=="16")
    {
        MyDisasm.Archi=0x10;
    }
    else if(pDataBlock->sMode=="32")
    {
        MyDisasm.Archi=0;
    }
    else if(pDataBlock->sMode=="64")
    {
        MyDisasm.Archi=0x40;
    }

    MyDisasm.EIP=(UIntPtr)pData;
    MyDisasm.SecurityBlock = nDataSize;

    int nLen=0;

    nLen = Disasm(&MyDisasm);

    if(nLen==OUT_OF_BLOCK)
    {
        nLen=1;
    }
    else if(nLen==UNKNOWN_OPCODE)
    {
        nLen=1;
    }

    return nLen;
}

int QXHexView::_getIndexFromAddress(__DATABLOCK *pDataBlock, unsigned long long nAddress)
{
    int nResult=0;

    for(int i=0; i<pDataBlock->listAddresses.count(); i++)
    {
        if((nAddress>=pDataBlock->listAddresses.at(i))&&(nAddress<pDataBlock->listAddresses.at(i)+pDataBlock->listSizes.at(i)))
        {
            nResult=i;
            break;
        }
    }

    //    qDebug("index %d",nResult);
    return nResult;
}

void QXHexView::_moveRightX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    bool bMax=false;

    if(_pDataBlock->nCursor==_pDataBlock->nStartAddress+_pDataBlock->nMaxSize-1)
    {
        bMax=true;
    }

    if(_pDataBlock->bCursorH)
    {
        _pDataBlock->bCursorH=false;
        _pDataBlock->bCursorL=true;
    }
    else if(_pDataBlock->bCursorL)
    {
        if(!bMax)
        {
            _pDataBlock->bCursorH=true;
            _pDataBlock->bCursorL=false;
            _pDataBlock->nCursor++;
        }
    }

    if(_pDataBlock->nCursor>=_pDataBlock->nCurrentAddress+_pDataBlock->nReadBlockSize)
    {
        _stepFX86ASM(pWidget);
    }
    else
    {
        ((QXHexViewX *)pWidget)->__update();
    }
}

void QXHexView::_moveLeftX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    bool bMin=false;

    if(_pDataBlock->nCursor==_pDataBlock->nStartAddress)
    {
        bMin=true;
    }

    if(_pDataBlock->bCursorH)
    {
        if(!bMin)
        {
            _pDataBlock->bCursorH=false;
            _pDataBlock->bCursorL=true;
            _pDataBlock->nCursor--;
        }
    }
    else if(_pDataBlock->bCursorL)
    {
        _pDataBlock->bCursorH=true;
        _pDataBlock->bCursorL=false;
    }

    if(_pDataBlock->nCursor<_pDataBlock->nCurrentAddress)
    {
        _stepBX86ASM(pWidget);
    }
    else
    {
        ((QXHexViewX *)pWidget)->__update();
    }
}

void QXHexView::_moveUpX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nCursor=_getPrevOpcodeAddressX86ASM(pWidget,_pDataBlock->nCursor);

    if(nCursor<=_pDataBlock->nCursor)
    {
        _pDataBlock->nCursor=nCursor;
    }

    _stepBX86ASM(pWidget);
}

void QXHexView::_moveDownX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nCursor=_getNextOpcodeAddressX86ASM(pWidget,_pDataBlock->nCursor);

    if(nCursor<_pDataBlock->nMaxSize)
    {
        _pDataBlock->nCursor=nCursor;
    }
    else
    {
        _pDataBlock->nCursor=_pDataBlock->nMaxSize-1;
    }

    _stepFX86ASM(pWidget);
}

void QXHexView::_moveRightHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    bool bMax=false;

    if(_pDataBlock->nCursor==_pDataBlock->nStartAddress+_pDataBlock->nMaxSize-1)
    {
        bMax=true;
    }

    if(!(_pDataBlock->bCursorH)&&!(_pDataBlock->bCursorL))
    {
        if(!bMax)
        {
            _pDataBlock->nCursor++;
        }

    }
    else if(_pDataBlock->bCursorH)
    {
        _pDataBlock->bCursorH=false;
        _pDataBlock->bCursorL=true;
    }
    else if(_pDataBlock->bCursorL)
    {
        if(!bMax)
        {
            _pDataBlock->bCursorH=true;
            _pDataBlock->bCursorL=false;
            _pDataBlock->nCursor++;
        }
    }

    if(_pDataBlock->nCursor>=_pDataBlock->nCurrentAddress+_pDataBlock->nReadBlockSize)
    {
        _stepFHEX(pWidget);
    }
    else
    {
        ((QXHexViewX *)pWidget)->__update();
    }
}

void QXHexView::_moveLeftHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    bool bMin=false;

    if(_pDataBlock->nCursor==_pDataBlock->nStartAddress)
    {
        bMin=true;
    }

    if(!(_pDataBlock->bCursorH)&&!(_pDataBlock->bCursorL))
    {
        if(!bMin)
        {
            _pDataBlock->nCursor--;
        }

    }
    else if(_pDataBlock->bCursorH)
    {
        if(!bMin)
        {
            _pDataBlock->bCursorH=false;
            _pDataBlock->bCursorL=true;
            _pDataBlock->nCursor--;
        }

    }
    else if(_pDataBlock->bCursorL)
    {
        _pDataBlock->bCursorH=true;
        _pDataBlock->bCursorL=false;

    }

    if(_pDataBlock->nCursor<_pDataBlock->nCurrentAddress)
    {
        _stepBHEX(pWidget);
    }
    else
    {
        ((QXHexViewX *)pWidget)->__update();
    }
}

void QXHexView::_moveUpHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nCursor=_pDataBlock->nCursor-_getStepHEX();

    if(nCursor<=_pDataBlock->nCursor)
    {
        _pDataBlock->nCursor=nCursor;
    }

    _stepBHEX(pWidget);
}

void QXHexView::_moveDownHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nCursor=_pDataBlock->nCursor+_getStepHEX();

    if(nCursor<_pDataBlock->nMaxSize)
    {
        _pDataBlock->nCursor=nCursor;
    }
    else
    {
        _pDataBlock->nCursor=_pDataBlock->nMaxSize-1;
    }

    //    qDebug("Cursor %d",nCursor);

    _stepFHEX(pWidget);
}

void QXHexView::_movePageUpHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nCursor=_pDataBlock->nCursor-20*_getStepHEX();

    if(nCursor<=_pDataBlock->nCursor)
    {
        _pDataBlock->nCursor=nCursor;
    }

    unsigned long long nAddress=_pDataBlock->nCurrentAddress-20*_getStepHEX();

    if(nAddress>_pDataBlock->nCurrentAddress)
    {
    }
    else
    {
        ((QXHexViewX *)pWidget)->_pageUp();
    }
}

void QXHexView::_movePageDownHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nCursor=_pDataBlock->nCursor+20*_getStepHEX();

    if(nCursor<_pDataBlock->nMaxSize)
    {
        _pDataBlock->nCursor=nCursor;
    }
    else
    {
        _pDataBlock->nCursor=_pDataBlock->nMaxSize-1;
    }

    unsigned long long nAddress=_pDataBlock->nCurrentAddress+20*_getStepHEX();

    if(nAddress<_pDataBlock->nMaxSize)
    {
        ((QXHexViewX *)pWidget)->_pageDown();
    }
}

void QXHexView::_movePageUpX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();
    //    unsigned long long nCursor=_pDataBlock->nCursor-20*_getStepX86ASM();


    //    if(nCursor<=_pDataBlock->nCursor)
    //    {
    //        nCursor=_getPrevOpcodeAddressX86ASM(pWidget,_pDataBlock->nCursor);
    //        _pDataBlock->nCursor=nCursor;
    //    }
    unsigned long long nAddress=_pDataBlock->nCurrentAddress-20*_getStepX86ASM();

    if(nAddress>_pDataBlock->nCurrentAddress)
    {
    }
    else
    {
        _pDataBlock->nCursor=nAddress;
        ((QXHexViewX *)pWidget)->_pageUp();
    }
}

void QXHexView::_movePageDownX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();
    //    unsigned long long nCursor=_pDataBlock->nCursor+20*_getStepX86ASM();
    //    if(nCursor<_pDataBlock->nMaxSize)
    //    {
    //        nCursor=_getNextOpcodeAddressX86ASM(pWidget,nCursor);
    //        _pDataBlock->nCursor=nCursor;
    //    }
    //    else
    //    {
    //        _pDataBlock->nCursor=_pDataBlock->nMaxSize-1;
    //    }
    unsigned long long nAddress=_pDataBlock->nCurrentAddress+20*_getStepX86ASM();

    if(nAddress<_pDataBlock->nMaxSize)
    {
        _pDataBlock->nCursor=nAddress;
        ((QXHexViewX *)pWidget)->_pageDown();
    }
}

void QXHexView::_writeCharHEX(QWidget *pWidget,int nKeyCode,int nMod,QString sText)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();
    QString alar="0123456789ABCDEFabsdef";

    bool bHEX=false;
    bool bANSI=false;
    unsigned char bByte;
    unsigned char cTemp;

    // HEX
    if(_pDataBlock->bCursorH||_pDataBlock->bCursorL)
    {
#ifdef Q_OS_MAC

        if(!(nMod&Qt::META)&&!(nMod&Qt::ALT))
#else
        if(!(nMod&Qt::CTRL)&&!(nMod&Qt::ALT))
#endif

        {
            if((nKeyCode>=Qt::Key_0)&&(nKeyCode<=Qt::Key_9))
            {
                bHEX=true;
                bByte=nKeyCode-Qt::Key_0;
            }
            else if((nKeyCode>=Qt::Key_A)&&(nKeyCode<=Qt::Key_F))
            {
                bHEX=true;
                bByte=nKeyCode-Qt::Key_A+10;
            }
        }
    }
    else
    {
#ifdef Q_OS_MAC

        if(!(nMod&Qt::META)&&!(nMod&Qt::ALT))
#else
        if(!(nMod&Qt::CTRL)&&!(nMod&Qt::ALT))
#endif

        {
            if((nKeyCode>=Qt::Key_Space)&&(nKeyCode<=Qt::Key_AsciiTilde))
            {
                bANSI=true;

                bByte=((unsigned char *)(sText.toLatin1().data()))[0];
            }
        }
    }

    if(bHEX)
    {
        bHEX=alar.contains(sText);
    }

    if(bHEX)
    {
        cTemp=readByte(pWidget,_pDataBlock->nCursor);

        if(!_pDataBlock->bIsReadOnly)
        {
            if(_pDataBlock->bCursorH)
            {
                cTemp=(cTemp&0x0F)+(bByte<<4);
            }
            else if(_pDataBlock->bCursorL)
            {
                cTemp=(cTemp&0xF0)+bByte;
            }

            writeByte(pWidget,_pDataBlock->nCursor,cTemp);

            _moveRightHEX(pWidget);

            //            qDebug(QString(cTemp).toAscii().data());
        }
    }
    else if(bANSI)
    {
        cTemp=readByte(pWidget,_pDataBlock->nCursor);

        if(!_pDataBlock->bIsReadOnly)
        {
            cTemp=bByte;

            writeByte(pWidget,_pDataBlock->nCursor,cTemp);

            _moveRightHEX(pWidget);
        }
    }
    //((QXHexViewX *)pWidget)->_reload();
}

void QXHexView::_writeCharX86ASM(QWidget *pWidget,int nKeyCode,int nMod,QString sText)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    QString alar="0123456789ABCDEFabsdef";

    bool bHEX=false;
    unsigned char bByte;
    unsigned char cTemp;

    // HEX
    if(_pDataBlock->bCursorH||_pDataBlock->bCursorL)
    {
#ifdef Q_OS_MAC

        if(!(nMod&Qt::META)&&!(nMod&Qt::ALT))
#else
        if(!(nMod&Qt::CTRL)&&!(nMod&Qt::ALT))
#endif
        {
            if((nKeyCode>=Qt::Key_0)&&(nKeyCode<=Qt::Key_9))
            {
                bHEX=true;
                bByte=nKeyCode-Qt::Key_0;
            }
            else if((nKeyCode>=Qt::Key_A)&&(nKeyCode<=Qt::Key_F))
            {
                bHEX=true;
                bByte=nKeyCode-Qt::Key_A+10;
            }
        }
    }

    if(bHEX)
    {
        bHEX=alar.contains(sText);
    }

    if(bHEX)
    {
        cTemp=readByte(pWidget,_pDataBlock->nCursor);

        if(!_pDataBlock->bIsReadOnly)
        {
            if(_pDataBlock->bCursorH)
            {
                cTemp=(cTemp&0x0F)+(bByte<<4);
            }
            else if(_pDataBlock->bCursorL)
            {
                cTemp=(cTemp&0xF0)+bByte;
            }

            writeByte(pWidget,_pDataBlock->nCursor,cTemp);

            _moveRightHEX(pWidget);

            //            qDebug(QString(cTemp).toAscii().data());
        }
    }

    //((QXHexViewX *)pWidget)->_reload();
}

unsigned char QXHexView::readByte(QWidget *pWidget, unsigned long long nOffset)
{
    unsigned char cResult;

    QByteArray baData=((QXHexViewX *)pWidget)->read(nOffset,1);

    cResult=baData.data()[0];

    return cResult;
}

bool QXHexView::writeByte(QWidget *pWidget, unsigned long long nOffset, unsigned char cByte)
{
    QByteArray baData;

    baData.resize(1);
    ((unsigned char *)baData.data())[0]=cByte;

    if(((QXHexViewX *)pWidget)->write(nOffset,baData))
    {
        __DATABLOCK *pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

        if((pDataBlock->nBufferAddress<=nOffset)&&(pDataBlock->baBuffer.size()>=1))
        {
            int nIndex=nOffset-pDataBlock->nBufferAddress;
            pDataBlock->baBuffer[nIndex]=cByte;

            // TODO Too slow
            pDataBlock->sHEX=ConvertToHex(&pDataBlock->baBuffer);
            pDataBlock->sANSI=ConvertToAnsi(&pDataBlock->baBuffer);
        }

        return true;
    }

    return false;
}

void QXHexView::_stepFHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nAddress=_pDataBlock->nCurrentAddress+_getStepHEX();

    if(nAddress<_pDataBlock->nMaxSize)
    {
        //        pWidget->emitGoToAddress(nAddress);
        //        scrollbar->setValue(scrollbar->value()+1);
        ((QXHexViewX *)pWidget)->_stepF();
    }
}

void QXHexView::_stepBHEX(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nAddress=_pDataBlock->nCurrentAddress-_getStepHEX();

    if(nAddress>_pDataBlock->nCurrentAddress)
    {
        //        nAddress=0;
    }
    else
    {
        ((QXHexViewX *)pWidget)->_stepB();
    }

    //    pWidget->emitGoToAddress(nAddress);
}

void QXHexView::_stepFX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nAddress=_getNextOpcodeAddressX86ASM(pWidget,_pDataBlock->nCurrentAddress);

    if(nAddress<_pDataBlock->nMaxSize)
    {
        //        pWidget->emitGoToAddress(nAddress);
        ((QXHexViewX *)pWidget)->_stepF();
    }
}

void QXHexView::_stepBX86ASM(QWidget *pWidget)
{
    __DATABLOCK *_pDataBlock=((QXHexViewX *)pWidget)->getDataBlock();

    unsigned long long nAddress=_getPrevOpcodeAddressX86ASM(pWidget,_pDataBlock->nCurrentAddress);

    if(nAddress>_pDataBlock->nCurrentAddress)
    {
        nAddress=0;
    }
    else
    {
        ((QXHexViewX *)pWidget)->_stepB();
    }
}

void QXHexView::_stepB()
{
    //    qDebug("B");
    scrollbar->setValue(scrollbar->value()-1);
}

void QXHexView::_stepF()
{
    //    qDebug("F");
    scrollbar->setValue(scrollbar->value()+1);
}
void QXHexView::_pageDown()
{
    scrollbar->setValue(scrollbar->value()+20);
}

void QXHexView::_startDocument()
{
    scrollbar->setValue(0);
}

void QXHexView::_endDocument()
{
    scrollbar->setValue(scrollbar->maximum());
}

void QXHexView::_pageUp()
{
    scrollbar->setValue(scrollbar->value()-20);
}
bool QXHexView::event(QEvent *event)
{
    if(event->type()==QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);

        //        qDebug("scroll %d",wheelEvent->delta()/120);
        //        qDebug("scroll value %d",scrollbar->value());
        if((scrollbar->value()==0)&&(wheelEvent->delta()/120==1))
        {
            if(datablock._scrollPositionToAddress)
            {
                unsigned long long nAddress=datablock._scrollPositionToAddress(&datablock,0);
                qxhexviewx->__update(nAddress);
            }
        }
        else
        {
            scrollbar->setValue(scrollbar->value()-(wheelEvent->delta()/120));
        }
    }

    return QScrollArea::event(event);
}

void QXHexView::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)
    {
        emit closeDialog();
    }

    if(datablock._keyPressEvent)
    {
        datablock._keyPressEvent(qxhexviewx,event);
    }
}

void QXHexView::setImage(QString sImage)
{
    __DATARECORD dr;
    datablock.records.clear();

    if(sImage=="Binary")
    {
        //        datablock.nAddressWidth=10;
        //        dr.nOffset=0;
        //        dr.nAddress=0;
        //        dr.nSize=Binary::getSize(datablock.parameter.toString());
        dr.nOffset=datablock.nStartAddress;
        dr.nAddress=datablock.nStartAddress;
        dr.nSize=datablock.nMaxSize;

        datablock.records.append(dr);
    }
    else if(sImage=="MSDOS")
    {
        dr.nOffset=datablock.nStartAddress;
        dr.nAddress=datablock.nStartAddress;
        dr.nSize=datablock.nMaxSize;

        datablock.records.append(dr);
    }
    else if(sImage=="PE")
    {
        PEFile pefile;

        if(pefile.setFileName(datablock.parameter.toString()))
        {
            //            datablock.nAddressWidth=10;

            unsigned int nImageBase=pefile.getOptionalHeader_ImageBase();

            for(int i=pefile.getFileHeader_NumberOfSections()-1; i>=0; i--)
            {
                dr.nOffset=pefile.getSection_PointerToRawData(i); // TODO ALIGN_DOWN FileAlignment

                if(dr.nOffset)
                {
                    dr.nSize=pefile.getSection_SizeOfRawData(i);
                    dr.nAddress=pefile.getSection_VirtualAddress(i)+nImageBase;
                    datablock.records.append(dr);
                }
            }

            // header

            dr.nOffset=0;
            dr.nAddress=nImageBase;
            dr.nSize=pefile.calculateSizeOfHeaders();

            datablock.records.append(dr);
        }
    }
    else if(sImage=="PE+(64)")
    {
        PEFile pefile;

        if(pefile.setFileName(datablock.parameter.toString()))
        {
            //            datablock.nAddressWidth=10;
            unsigned long long nImageBase64=pefile.getOptionalHeader_ImageBase64();

            for(int i=pefile.getFileHeader_NumberOfSections()-1; i>=0; i--)
            {
                dr.nOffset=pefile.getSection_PointerToRawData(i);

                if(dr.nOffset)
                {
                    dr.nSize=pefile.getSection_SizeOfRawData(i);
                    dr.nAddress=pefile.getSection_VirtualAddress(i)+nImageBase64;

                    datablock.records.append(dr);
                }
            }

            // header
            dr.nOffset=0;
            dr.nAddress=nImageBase64;
            dr.nSize=pefile.calculateSizeOfHeaders();

            datablock.records.append(dr);
        }
    }
    else if(sImage=="ELF")
    {
        ELFFile elffile;

        if(elffile.setFileName(datablock.parameter.toString()))
        {

            //            datablock.nAddressWidth=10;

            for(int i=0; i<elffile.getElfHeader_phnum(); i++)
            {
                dr.nOffset=elffile.getProgramHeader32_offset(i);
                dr.nAddress=elffile.getProgramHeader32_vaddr(i);
                dr.nSize=elffile.getProgramHeader32_filesz(i);

                datablock.records.append(dr);
            }
        }
    }
    else if(sImage=="ELF64")
    {
        ELFFile elffile;

        if(elffile.setFileName(datablock.parameter.toString()))
        {
            //            datablock.nAddressWidth=10;

            for(int i=0; i<elffile.getElfHeader_phnum(); i++)
            {
                dr.nOffset=elffile.getProgramHeader64_offset(i);
                dr.nAddress=elffile.getProgramHeader64_vaddr(i);
                dr.nSize=elffile.getProgramHeader64_filesz(i);

                datablock.records.append(dr);
            }
        }
    }
    else if(sImage=="MACH")
    {
        MACHFile machfile;

        if(machfile.setFileName(datablock.parameter.toString()))
        {
            //            datablock.nAddressWidth=10;

            for(int i=0; (unsigned int)i<machfile.getNumberOfSegments(); i++)
            {
                dr.nOffset=machfile.getSegment_fileoff32(i);
                dr.nAddress=machfile.getSegment_vmaddr32(i);
                dr.nSize=machfile.getSegment_filesize32(i);

                datablock.records.append(dr);
            }
        }
    }

    else if(sImage=="MACH64")
    {
        MACHFile machfile;

        if(machfile.setFileName(datablock.parameter.toString()))
        {
            //            datablock.nAddressWidth=10;

            for(int i=0; (unsigned int)i<machfile.getNumberOfSegments(); i++)
            {
                dr.nOffset=machfile.getSegment_fileoff64(i);
                dr.nAddress=machfile.getSegment_vmaddr64(i);
                dr.nSize=machfile.getSegment_filesize64(i);

                datablock.records.append(dr);
            }
        }
    }

    qxhexviewx->__update();
}

void QXHexView::_setFont(QFont font)
{
    qxhexviewx->_setFont(font);
}

void QXHexView::setAddressWidth(unsigned int nAddressWidth)
{
    datablock.nAddressWidth=nAddressWidth;
}

void QXHexView::_showToolTips(bool bState)
{
    datablock.bShowToolTips=bState;
}

void QXHexView::setLittleEndian(bool bState)
{
    datablock.bIsLittleEndian=bState;
}

void QXHexView::setVorBuffer(bool bState)
{
    datablock.bVorBuffer=bState;
}

void QXHexView::reloadMemoryMap()
{
    datablock.MemoryMap.clear();

    if(datablock.bIsMemoryMap)
    {

    }
    else
    {
        __MEMORYMAP mm;

        mm.nAddress=datablock.nStartAddress;
        mm.nSize=datablock.nMaxSize;

        datablock.MemoryMap.append(mm);
    }

    emit _reloadMemoryMap(&(datablock.MemoryMap));
}
