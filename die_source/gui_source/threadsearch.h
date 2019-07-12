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
#ifndef THREADSEARCH_H
#define THREADSEARCH_H

#include <QObject>
#include <QVariant>
#include "../../binary.h"
#include <qendian.h>
#include <QTextStream>
#include <QThread>
#include <qtconcurrentmap.h>
#include <QFutureWatcher>

struct CRYPTO_RECORD
{
    QString sName;
    QString sType;
    QString sSignature;
};

struct ROW_STRUCT
{
    QString sName;
    unsigned long long nOffset;
    unsigned long long nSize;
};

struct SEARCHDATA
{
    Binary *binary;
    QString sType;
    unsigned long long nOffset;
    unsigned long long nSize;
    unsigned int nFlags;
    QList<CRYPTO_RECORD> *pCryptoSignatures;
    QFutureWatcher<QString> *watcher;
    int nMinLength;
};

class ThreadSearch : public QObject
{
    Q_OBJECT
public:
    explicit ThreadSearch(QObject *parent = 0);
    //    void setData(Binary *binary,QString sType,unsigned long long nOffset,unsigned long long nSize,unsigned int nFlags,QList<CRYPTO_RECORD> *pCryptoSignatures,QFutureWatcher<QString> *watcher);
    void setData(SEARCHDATA *pSearchData);
    bool isCompleted();

    static QList<CRYPTO_RECORD> loadCryptoSignatures(QString sDatabaseName);

signals:
    //    void setProgressBar(int nMax,int nValue);
    void appendRow(QString sName,unsigned long long nOffset,unsigned long long nSize);
    void _finished();
    void progressRangeChanged(int nMin,int nMax);
    void progressValueChanged(int nValue);
    void foundLimit(int nLimit);
    void appendError(QString sString);

public slots:
    void process();
    void _stop();
    void slot_progressRangeChanged(int minimum,int maximum);
    //    void cryptoResultReady(int index);

private:
    bool bIsRun;
    bool bIsCompleted;
    //    Binary *binary;
    //    QString sType;
    //    unsigned long long nOffset;
    //    unsigned long long nSize;
    //    unsigned int nFlags;
    //    QList<CRYPTO_RECORD> *pCryptoSignatures;
    //    QFutureWatcher<QString> *watcher;

    //    QFutureWatcher<QString> *futureWatcher;
    SEARCHDATA *pSearchData;

private slots:
    int appendResult(QString sName,unsigned long long nOffset,unsigned long long nSize);
    void searchANSI(QFile *pFile);
    void searchUnicode(QFile *pFile);
    void searchCrypto(QFile *pFile);
};

#endif // THREADSEARCH_H
