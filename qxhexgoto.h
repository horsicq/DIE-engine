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
#ifndef QXHEXGOTO_H
#define QXHEXGOTO_H

#include <QDialog>
#include "qxhex.h"
#include <QRegExpValidator>

namespace Ui
{
class QXHexGoTo;
}

class QXHexGoTo : public QDialog
{
    Q_OBJECT

public:
    explicit QXHexGoTo(QWidget *parent=nullptr);
    ~QXHexGoTo();
    //    void setData(unsigned long long nStartAddress,unsigned long long nMaxSize,bool bIsHEX);
    void setData(QList<__DATARECORD> *pList,bool bIsHEX);

    void _showToolTips(bool bState);

private:
    Ui::QXHexGoTo *ui;
    QList<__DATARECORD> *pList;
    //    unsigned long long nStartAddress;
    //    unsigned long long nMaxSize;

signals:
    void valueChanged(unsigned long long);

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();


};

#endif // QXHEXGOTO_H
