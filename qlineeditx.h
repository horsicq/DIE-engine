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
#ifndef QLINEEDITX_H
#define QLINEEDITX_H

#include <QLineEdit>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QHelpEvent>
#include <QToolTip>
#include <QTimer>
#include "qcontrolx.h"

class QLineEditX : public QLineEdit
{
    Q_OBJECT
public:
    explicit QLineEditX(QWidget *parent=nullptr);

public slots:
    void customContextMenuRequested(const QPoint &pos);
    void copyAsString();
    void copyAsHex0x();
    void copyAsHex();
    void copyAsDec();

    void setHex(bool bValue);
    void setDec(bool bValue);
    void setString(bool bValue);

protected:
    bool event(QEvent *event);
    void leaveEvent(QEvent *myLeave);
    void setCopyMode(bool bIsString,bool bIsDec,bool bIsHex);

private slots:
    void _showTooltip();

private:
    int x;
    int y;
    bool bIsShow;

    QAction *actCopyAsString;
    QAction *actCopyAsHex;
    QAction *actCopyAsHex0x;
    QAction *actCopyAsDec;

    bool bIsHex;
    bool bIsDec;
    bool bIsString;
};

#endif // QLINEEDITX_H
