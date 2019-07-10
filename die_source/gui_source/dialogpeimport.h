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
#ifndef DIALOGPEIMPORT_H
#define DIALOGPEIMPORT_H

#include "dialogpegeneric.h"
#include "dialogpeimportlibrary.h"
#include "dialogpeimportfunction.h"
#include "threadimport.h"
#include <QTableWidgetItem>

namespace Ui
{
class DialogPEImport;
}

class DialogPEImport : public DialogPEGeneric
{
    Q_OBJECT
public:
    explicit DialogPEImport(QWidget *parent);
    ~DialogPEImport();

private slots:
    void on_pushButtonOK_clicked();
    bool reload();
    void setProgressBar(int nMax, int nValue);
    void editLibrary();
    void editFunction();
    void on_tableWidgetLibraries_customContextMenuRequested(const QPoint &pos);
    void on_tableWidgetFunctions_customContextMenuRequested(const QPoint &pos);
    void copyAsStringLibrary();
    void copyAsStringFunction();
    //    void setTableEnable();
    void enableControls();
    void disableControls();
    //    void on_tableWidgetLibraries_cellChanged(int row, int column);
    void createTable();

    void on_tableWidgetLibraries_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

signals:
    void reset();

private:
    Ui::DialogPEImport *ui;

    //    QByteArray baSelected;
    unsigned int nOffset;
    unsigned int nSize;
    QAction *actEditLibrary;
    QAction *actEditFunction;

    QAction *actCopyAsStringLibrary;
    QAction *actCopyAsStringFunction;

    //QTimer timer;
    ThreadImport *scan;
    QModelIndex indexLast;
    int nRawLast;
    bool bFirstTime;

    bool bSync;

    QList<ThreadImport::IMPORT_LIBRARY> listLibraries;
};

#endif // DIALOGPEIMPORT_H
