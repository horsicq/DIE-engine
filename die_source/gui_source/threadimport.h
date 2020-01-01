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
#ifndef THREADIMPORT_H
#define THREADIMPORT_H

#include <QObject>
#include "../../pefile.h"

class ThreadImport : public QObject
{
    Q_OBJECT
public:
    struct IMPORT_FUNCTION
    {
        QString sName;
        QString sThunk;
        QString sOrdinal;
        QString sHint;
    };

    struct IMPORT_LIBRARY
    {
        QString sName;
        unsigned int nOriginalFirstThunk;
        unsigned int nTimeDateStamp;
        unsigned int nForwarderChain;
        unsigned int nName;
        unsigned int nFirstThunk;
        QList<IMPORT_FUNCTION> listFunctions;
    };

    explicit ThreadImport(QObject *parent=nullptr);
    void setData(PEFile *pefile, QList<IMPORT_LIBRARY> *pListLibraries);

    bool isCompleted();
    bool isRun();

signals:
    void setProgressBar(int nMax,int nValue);
    //    void appendRow(int index,QString sThunk,QString sOrdinal,QString sHint,QString sName);
    void _started();
    void _finished();

public slots:
    void process();
    void _stop();

private:
    PEFile *pefile;
    //    QString sLibraryName;
    QList<IMPORT_LIBRARY> *pListLibraries;
    bool bIsRun;
    bool bIsCompleted;
};

#endif // THREADIMPORT_H
