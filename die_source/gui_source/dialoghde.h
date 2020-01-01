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
#ifndef DIALOGHDE_H
#define DIALOGHDE_H

#include "qxdialogx.h"
#include <QString>
#include <QPainter>
#include <QFileDialog>
#include <QCryptographicHash>
#include <math.h>
//#include "../qhexview.h"
#include "../../qxhexform.h"
#include "threadsearchplugin.h"

namespace Ui
{
class DialogHDE;
}

class DialogHDE : public QXDialogX
{
    Q_OBJECT

public:
    explicit DialogHDE(QWidget *parent=nullptr);
    void setData(__DIE_OPTIONS *pOptions,QString sFileName,unsigned long long nAddress,unsigned long long nSize,QString sType,QString sMode,QString sSyntax,unsigned long long nSelectionStart,unsigned long long nSelectionSize,QString sName);
    ~DialogHDE();

private slots:
    bool reload();
    void closeDialog();
    //    void on_pushButtonSearch_clicked();

signals:
    void saveSignal();
    void appendError(QString sText);
    //    void reloadSignal();

private:
    Ui::DialogHDE *ui;

    QXHexForm *pHexEdit;
    QString sFileName;
    unsigned long long nAddress;
    unsigned long long nSize;
    QString sType;
    QString sMode;
    QString sSyntax;
    QString sName;
    unsigned long long nSelectionStart;
    unsigned long long nSelectionSize;
    //    ThreadSearchPlugin *tsp;
    //    QTimer timerPlugin;
};

#endif // DIALOGHDE_H
