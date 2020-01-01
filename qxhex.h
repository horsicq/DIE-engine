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
#ifndef QXHEX_H
#define QXHEX_H

#include <QWidget>
#include <QVariant>

QString ByteToAnsi(unsigned char cByte);
QString ConvertToHex(QByteArray *pbaData);
QString ByteToHex(unsigned char cByte);
QString ConvertToAnsi(QByteArray *pbaData);

struct __DATARECORD
{
    unsigned long long nOffset;
    unsigned long long nAddress;
    unsigned long long nSize;
};

struct __MEMORYMAP
{
    unsigned long long nAddress;
    unsigned long long nSize;
};


struct __DATABLOCK
{
    void (* _paint)(QWidget *);
    bool (* _mouseEvent)(QWidget *,QEvent *);
    void (* _keyPressEvent)(QWidget *,QKeyEvent *);
    void (* _contextMenu)(QWidget *,QPoint);
    QVariant parameter;
    QByteArray(*__ReadMemory)(QVariant,unsigned long long,unsigned long long,bool *);
    bool (*__WriteMemory)(QVariant,unsigned long long,QByteArray);
    void (* _action[10])(QWidget *,unsigned long long,unsigned long long);
    int (* _getStep)();
    unsigned long long nStartAddress;
    unsigned long long nMaxSize;
    int nSymbolWidth;
    int nSymbolHeight;
    int nLineCount;
    unsigned long long nCurrentAddress;
    int nReadBlockSize;
    int nReadBlockMax;
    int nAddressWidth;
    unsigned long long nStartSelection;
    unsigned long long nEndSelection;
    unsigned long long nNextSelection;
    unsigned long long nEditAddress;
    unsigned long long nCursor;
    unsigned long long nFirstClick;
    unsigned long long nFirstClickNext;
    bool bCursorH;
    bool bCursorL;

    bool bShowCursor;
    //        int nStep;
    unsigned long long nEditorLineCount;
    unsigned long long(* _addressToScrollPosition)(__DATABLOCK *pData,unsigned long long);
    unsigned long long(* _scrollPositionToAddress)(__DATABLOCK *pData,unsigned long long);
    unsigned long long(* _offsetToAddress)(__DATABLOCK *pData,unsigned long long);
    int nAddressDelta;
    bool bAddressAsHEX;
    QString sType;
    QString sMode;
    QString sSyntax;
    QList<unsigned long long> listAddresses;
    QList<unsigned long long> listSizes;

    unsigned long long nCurrentScrollPosition;
    //        unsigned long long nBlockAddress;
    //        unsigned long long nBlockSize;

    QList<__DATARECORD> records;
    bool bIsReadOnly;
    bool bIsReadOnlyMode;

    QString sSearchSignature;
    unsigned long long nSearchOffset;

    bool bShowToolTips;
    bool bIsLittleEndian;
    bool bVorBuffer;
    unsigned long long nBufferAddress;
    unsigned long long nBufferSize;
    QByteArray baBuffer;
    QString sHEX;
    QString sANSI;

    bool bIsMemoryMap;

    QList<__MEMORYMAP> MemoryMap;

    QString sDumpFileName;
    unsigned long long nDumpAddress;
    unsigned long long nDumpSize;
};

#endif // QXHEX_H
