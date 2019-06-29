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
#ifndef THREADDIRECTORY_H
#define THREADDIRECTORY_H

#include <QObject>
#include <QList>
#include <QDir>

class ThreadDirectory : public QObject
{
    Q_OBJECT
public:
    explicit ThreadDirectory(QObject *parent = 0);
    void setData(QString sDirectory,bool bSubDirectory,QList<QString> *pList);

    bool isCompleted();

signals:
    void setTotal(int nTotal);
    void _finished();

private:
    QString sDirectory;
    bool bSubDirectory;
    QList<QString> *pList;
    bool bIsRun;
    bool bIsCompleted;
    unsigned int nTotal;
    QList<QString> filter;

    void findFiles(QString sDirectoryName);

public slots:
    void process();
    void _stop();
};

#endif // THREADDIRECTORY_H
