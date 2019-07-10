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
#ifndef DIALOGDIRECTORYHEURISTICSCAN_H
#define DIALOGDIRECTORYHEURISTICSCAN_H

#include "qxdialogx.h"
#include <QDialog>
#include <QThread>
#include <QFileDialog>
#include <QDir>
#include <QList>
#include <QTimer>
#include <QSettings>
#include "../scan.h"
//#include "scanheuristic.h"
//#include "scansignatures.h"
#include "dialogoptions.h"
#include "threaddirectory.h"

namespace Ui
{
class DialogDirectoryHeuristicScan;
}

class DialogDirectoryHeuristicScan : public QXDialogX
{
    Q_OBJECT

public:
    explicit DialogDirectoryHeuristicScan(QWidget *parent);
    ~DialogDirectoryHeuristicScan();
    void setData(__DIE_OPTIONS *pOptions);
    void setDirectory(QString sText);

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonDirectory_clicked();
    void setProgressBar1(int nMax, int nValue);
    void setProgressBar2(int nMax, int nValue);
    void on_pushButtonScan_clicked();
    //    void appendText(QString sText); // Obsolete
    void appendError(QString sText);
    void setEnabled();
    void appendFileName(QString sText);
    //bool getDeepSignatureScanState();

    void on_pushButtonSave_clicked();
    void startScan();
    void setTotal(int nTotal);

    void _reset();

    bool reload();
    void singleScanCompleteSlot(int nMs);

signals:
    void pushButtonScan_click();
    void reset();

private:
    Ui::DialogDirectoryHeuristicScan *ui;
    //ScanHeuristic *scanheuristic;
    //ScanSignatures *scansignatures;
    Scan *scan;
    ThreadDirectory *td;
    //QSqlDatabase *Base;
    QList<QString> dirContent;
    QTimer timer;
    int nScanned;
    QDateTime starttime;
    bool bIsScan;
};

#endif // DIALOGDIRECTORYHEURISTICSCAN_H
