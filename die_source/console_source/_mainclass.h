// Copyright (c) 2012-2018 hors<horsicq@gmail.com>
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
#ifndef _MAINCLASS_H
#define _MAINCLASS_H

#include <QObject>
#include <stdio.h>
#include "../utils.h"

class _MainClass : public QObject
{
    Q_OBJECT
public:
    explicit _MainClass(QObject *parent = 0);
    void setOptions(__DIE_OPTIONS *pOptions);
signals:

public slots:
    void handleMessage(QString szMessage);
    //    void appendSignature(QString szMessage);
    void appendError(QString szMessage);
    void appendFileName(QString szMessage);

private:
    bool bSingleLineOutput;
    __DIE_OPTIONS *pOptions;
    QString sCurrentType;
};

#endif // _MAINCLASS_H
