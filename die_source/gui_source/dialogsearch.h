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
#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include "qxdialogx.h"
#include "threadsearch.h"
#include "searchtablemodel.h"
#include <QSortFilterProxyModel>
#include <QClipboard>
#include "dialoghde.h"
#include <QModelIndex>
#include "search.h"

namespace Ui
{
class DialogSearch;
}

class DialogSearch : public QXDialogX
{
    Q_OBJECT

public:
    explicit DialogSearch(QWidget *parent = 0);
    ~DialogSearch();
    void setData(__DIE_OPTIONS *pOptions,Binary *_binary,unsigned int nOffset,unsigned int nSize);

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonSearch_clicked();
    //    void setProgressBar(int nMax, int nValue);
    bool reload();
    void appendRow(QString sName,unsigned long long nOffset,unsigned long long nSize);
    void _on_finished();
    void search();
    void on_tableViewANSI_customContextMenuRequested(const QPoint &pos);

    void hex();
    void copyAsString();
    void on_pushButtonHEX_clicked();

    void _selectedChanged();

    void on_listWidgetMods_currentRowChanged(int currentRow);
    void foundLimit(int nLimit);
    void on_pushButtonSaveResult_clicked();
    void searchEnable();
    void _error(QString sString);
    void on_lineEditFilter_textChanged(const QString &arg1);

signals:
    void reset();

private:
    Ui::DialogSearch *ui;
    SEARCHDATA searchdata;

    Binary *_binary;
    unsigned int nOffset;
    unsigned int nSize;

    ThreadSearch *scan;

    QSortFilterProxyModel *filter;
    SearchTableModel searchTable;

    QAction *actCopyAsString;
    QAction *actHex;

    QList<CRYPTO_RECORD> cryptoSignatures;

    QTimer timerHeuristic;
};

#endif // DIALOGSEARCH_H
