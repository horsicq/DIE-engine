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
#ifndef QXHEXSEARCHTHREAD_H
#define QXHEXSEARCHTHREAD_H

#include <QObject>
#include "qxhex.h"

class QXHexSearchThread : public QObject
{
    Q_OBJECT

public:
    explicit QXHexSearchThread(QObject *parent = 0);

signals:
    void _searchComplete();
    void _found(unsigned long long nAddress,unsigned long long nSize);
    void _nothingfound();
    void progressRangeChanged(int nMin,int nMax);
    void progressValueChanged(int nValue);

public slots:
    unsigned char signatureToByte(QString *psSignature);
    unsigned short signatureToWord(QString *psSignature);
    unsigned int signatureToDword(QString *psSignature);
    unsigned int _search(QString *psSignature,QByteArray *pbaData);
    void stopSearch();
    void startSearch();

public:
    void setData(__DATABLOCK *pDataBlock);

private:
    __DATABLOCK *pDataBlock;

    bool bSearch;
};

#endif // QXHEXSEARCHTHREAD_H
