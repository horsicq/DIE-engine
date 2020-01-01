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
#ifndef DIALOGPEEXPORT_H
#define DIALOGPEEXPORT_H

#include "dialogpegeneric.h"
#include "dialogpeexportfunction.h"
#include "threadexport.h"

namespace Ui
{
class DialogPEExport;
}

class DialogPEExport : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPEExport(QWidget *parent);
    ~DialogPEExport();

private slots:
    bool reload();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void applyEnable();
    void edit();

    void on_tableWidgetFunctions_customContextMenuRequested(const QPoint &pos);
    void setProgressBar(int nMax, int nValue);
    void appendRow(int index,QString sOrdinal, QString sAddress, QString sName);
    void copyAsString();

signals:
    void reset();

private:
    Ui::DialogPEExport *ui;

    QLineEditX *lineEditCharacteristics;
    QLineEditX *lineEditTimeDateStamp;
    QLineEditX *lineEditMajorVersion;
    QLineEditX *lineEditMinorVersion;
    QLineEditX *lineEditName;
    QLineEditX *lineEditBase;
    QLineEditX *lineEditNumberOfFunctions;
    QLineEditX *lineEditNumberOfNames;
    QLineEditX *lineEditAddressOfFunctions;
    QLineEditX *lineEditAddressOfNames;
    QLineEditX *lineEditAddressOfNameOrdinals;

    QAction *actEdit;
    ThreadExport *scan;

    QAction *actCopyAsString;
};

#endif // DIALOGPEEXPORT_H
