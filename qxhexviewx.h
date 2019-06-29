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
#ifndef QXHEXVIEWX_H
#define QXHEXVIEWX_H

#include <QScrollArea>
#include <QPainter>
#include <QVariant>
#include <QEvent>
#include <QMouseEvent>
#include <QInputDialog>
#include <QTimer>
#include <QTime>
#include "qxhexgoto.h"
#include "qxhexsearch.h"
#include "qxhexsignature.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <QObject>
#include <QThread>
#include "qxhexsearchthread.h"
#include "qxhexdumpthread.h"
#include <QShortcut>
#include <QApplication>
#include <QClipboard>

#include "qxhex.h"

//QString ByteToAnsi(unsigned char cByte);
//QString ConvertToHex(QByteArray *pbaData);
//QString ByteToHex(unsigned char cByte);
//QString ConvertToAnsi(QByteArray *pbaData);

class QXHexViewX : public QWidget
{
public:
    Q_OBJECT
public:
    explicit QXHexViewX(QScrollArea *parent = 0);
    ~QXHexViewX();
    QWidget *getHinstance();
    void setData(__DATABLOCK *pDataBlock);

    void _setFont(QFont font);

    void read();
    QByteArray read(unsigned long long nAddress,unsigned long long nSize);
    bool write(unsigned long long nAddress,QByteArray baData);

    void emitError(QString sString);
    void emitRescroll();

    __DATABLOCK *getDataBlock();

    void emitSelectionStartChanged(unsigned long long nValue);
    void emitSelectionSizeChanged(unsigned long long nValue);
    void emitCursorChanged(unsigned long long nValue);
    void emitGoToAddress(unsigned long long nValue);
    void _stepF();
    void _stepB();
    void _pageUp();
    void _pageDown();

signals:

    void appendError(QString sString);
    void setCurrentAddress(unsigned long long nValue);
    void rescroll();
    void selectionStartChanged(unsigned long long nValue);
    void selectionSizeChanged(unsigned long long nValue);
    void cursorChanged(unsigned long long nValue);
    void goToAddress(unsigned long long nValue);
    void setReadOnly(bool bState);
    void __repaint();
    void _signalStepB();
    void _signalStepF();
    void _signalPageUp();
    void _signalPageDown();

    void _signalStartDocument();
    void _signalEndDocument();

public slots:
    void trigger0();
    void trigger1();
    void trigger2();
    void trigger3();
    void trigger4();
    void trigger5();
    void trigger6();
    void trigger7();
    void trigger8();
    void trigger9();
    void goToAddress();
    void search();
    void searchData();
    void searchText();
    void searchValue();
    void findSignature(QString sSignature,bool bIsCursor);
    void findNext();
    void _searchThread();
    void _dumpThread();
    void found(unsigned long long nAddress,unsigned long long nSize);
    void nothingFound();
    void dumpComplete(QString sFileName);
    void error(QString sString);
    void information(QString sInformation);

    void __update(unsigned long long nCurrentAddress);
    void __update();
    void timerSlot();
    void __repaintSlot();

    void stopSearch();
    void stopDump();

    void dump();
    void selectAll();
    void selectReset();
    void copyAsHEX();
    void copyAsHEXWithSpaces();
    void copyAsANSI();
    void copyAsSignature();

    unsigned long long getSelectionOffset();
    unsigned long long getSelectionSize();
    bool isSelectionPresent();

    void reload();
    void _reload();

    QSet<unsigned long long> *getModAddresses();
protected:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);
    //    void keyPressEvent(QKeyEvent * event);

private:
    __DATABLOCK *pDataBlock;
    QTimer _cursorTimer;
    bool bIsUpdated;
    QXHexSearchThread *searchThread;
    QXHexDumpThread *dumpThread;
    //    int nTemp;
    QShortcut *shortcutDump;
    QShortcut *shortcutSelectAll;
    QShortcut *shortcutSearchSignature;
    QShortcut *shortcutSearchText;
    QShortcut *shortcutSearchValue;
    QShortcut *shortcutGoToAddress;
    QShortcut *shortcutSearchNext;
    QShortcut *shortcutCopy;

    QShortcut *shortcutStartDocument;
    QShortcut *shortcutEndDocument;

    QSet<unsigned long long> setModAddresses;
};

#endif // QXHEXVIEWX_H
