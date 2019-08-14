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
#ifndef QXHEXVIEW_H
#define QXHEXVIEW_H

#include <QScrollArea>
#include <QMessageBox>
#include <QVariant>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QEvent>
#include "qxhexviewx.h"
#include <QAction>
#include <QMenu>
#include <QComboBox>
#define BEA_ENGINE_STATIC
//#define BEA_USE_STDCALL
#include "BeaEngine.h"
#include "binary.h"
#include "pefile.h"
#include "elffile.h"
#include "machfile.h"
#include <QShortcut>
#include <QElapsedTimer>
//#include <QTextCodec>


class QXHexView : public QScrollArea
{
    Q_OBJECT
public:
    explicit QXHexView(QWidget *parent=nullptr);
    QList<QString> setData(QByteArray(*__ReadMemory)(QVariant,unsigned long long,unsigned long long,bool *),bool (*__WriteMemory)(QVariant,unsigned long long,QByteArray),QVariant parameter,unsigned long long nStartAddress,unsigned long long nMaxSize);
    void setType(QString sType,QString sMode,QString sSyntax);
    void setAddressAsHEX(bool bState);
    void setSelection(unsigned long long nSelectionStart,unsigned long long nSelectionSize);
    void setImage(QString sImage);
    void _setFont(QFont font);
    void setAddressWidth(unsigned int nAddressWidth);
    void _showToolTips(bool bState);
    void setLittleEndian(bool bState);
    void setVorBuffer(bool bState);
    void reloadMemoryMap();

signals:
    void _reloadMemoryMap(QList<__MEMORYMAP> *pMemoryMap);
    void appendError(QString szText);
    void selectionStartChanged(unsigned long long nValue);
    void selectionSizeChanged(unsigned long long nValue);
    void cursorChanged(unsigned long long nValue);
    void setReadOnly(bool bState);
    void closeDialog();

public slots:
    void goToAddress(unsigned long long nAddress);
    static QByteArray ReadFromFile(QVariant parameter,unsigned long long nOffset,unsigned long long nSize,bool *pbIsReadOnly);
    static bool WriteToFile(QVariant parameter,unsigned long long nOffset,QByteArray baData);
    static void _init(__DATABLOCK *pData);
    //    static void _dump(QXHexViewX *pWidget,unsigned long long nAddress,unsigned long long nSize);

    static void _paintRevert(QWidget *pWidget,QPainter *pPainter);
    static void _paintFontRevert(QWidget *pWidget,QPainter *pPainter);
    static void _paintHEX(QWidget *pWidget);
    static bool _mouseEventHEX(QWidget *pWidget,QEvent *event);
    static void _keyPressEventHEX(QWidget *pWidget,QKeyEvent *event);
    static void _contextMenuHEX(QWidget *pWidget,QPoint pos);
    static void _contextMenu(QWidget *pWidget,QPoint pos);
    static int _getStepHEX();
    static void _initHEX(__DATABLOCK *pData);
    static unsigned long long _addressToScrollPositionHEX(__DATABLOCK *pData,unsigned long long nAddress);
    static unsigned long long _scrollPositionToAddressHEX(__DATABLOCK *pData,unsigned long long nScrollPosition);

    static int _getRealSizeX86ASM(__DATABLOCK *pDataBlock,char *pData,int nDataSize,int nNumberofLines);
    static void _paintX86ASM(QWidget *pWidget);
    static bool _mouseEventX86ASM(QWidget *pWidget,QEvent *event);
    static void _keyPressEventX86ASM(QWidget *pWidget,QKeyEvent *event);
    static void _contextMenuX86ASM(QWidget *pWidget,QPoint pos);
    static int _getStepX86ASM();
    static unsigned long long _getNextOpcodeAddressX86ASM(QWidget *pWidget,unsigned long long nCurrent);
    static unsigned long long _getPrevOpcodeAddressX86ASM(QWidget *pWidget,unsigned long long nCurrent);
    static void _initX86ASM(__DATABLOCK *pData);
    static unsigned long long _addressToScrollPositionX86ASM(__DATABLOCK *pData,unsigned long long nAddress);
    static unsigned long long _scrollPositionToAddressX86ASM(__DATABLOCK *pData,unsigned long long nScrollPosition);
    static int _getOpcodeSizeX86ASM(__DATABLOCK *pDataBlock,char *pData,int nDataSize);
    static int _getIndexFromAddress(__DATABLOCK *pDataBlock,unsigned long long nAddress);
    static void _moveRightX86ASM(QWidget *pWidget);
    static void _moveLeftX86ASM(QWidget *pWidget);
    static void _moveUpX86ASM(QWidget *pWidget);
    static void _moveDownX86ASM(QWidget *pWidget);
    static void _moveRightHEX(QWidget *pWidget);
    static void _moveLeftHEX(QWidget *pWidget);
    static void _moveUpHEX(QWidget *pWidget);
    static void _moveDownHEX(QWidget *pWidget);
    static void _movePageUpHEX(QWidget *pWidget);
    static void _movePageDownHEX(QWidget *pWidget);

    static void _movePageUpX86ASM(QWidget *pWidget);
    static void _movePageDownX86ASM(QWidget *pWidget);

    static void _writeCharHEX(QWidget *pWidget,int nKeyCode,int nMod,QString sText);
    static void _writeCharX86ASM(QWidget *pWidget,int nKeyCode,int nMod,QString sText);

    static unsigned char readByte(QWidget *pWidget,unsigned long long nOffset);
    static bool writeByte(QWidget *pWidget,unsigned long long nOffset,unsigned char cByte);

    static void _stepFHEX(QWidget *pWidget);
    static void _stepBHEX(QWidget *pWidget);
    static void _stepFX86ASM(QWidget *pWidget);
    static void _stepBX86ASM(QWidget *pWidget);

    void _stepB();
    void _stepF();
    void _pageUp();
    void _pageDown();
    void _startDocument();
    void _endDocument();

    void scrollSlot(int nScrollPosition);
    //    void setScrollValue(unsigned long long nValue);
    void rescroll();
    void __update();

    static unsigned long long offsetToAddress(__DATABLOCK *pData,unsigned long long nOffset);
    static unsigned long long addressToOffset(__DATABLOCK *pData,unsigned long long nAddress);

    void setReadOnlyMode(bool bState);
    void reload();

protected:
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent * event);

private:
    //    QByteArray (*__ReadMemory)(QVariant,unsigned long long,unsigned long long);
    //    QVariant parameter;

    //    static unsigned int nDataBlockSize;
    QScrollBar *scrollbar;
    QXHexViewX *qxhexviewx;
    //    bool bReadOnlyMode;

    __DATABLOCK datablock;
};

#endif // QXHEXVIEW_H
