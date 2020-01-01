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
#ifndef DIALOGPEDOSHEADER_H
#define DIALOGPEDOSHEADER_H

#include "dialogpegeneric.h"
#include "dialoghde.h"

namespace Ui
{
class DialogPEDosHeader;
}

class DialogPEDosHeader : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPEDosHeader(QWidget *parent);
    ~DialogPEDosHeader();

private slots:
    bool reload();
    void on_pushButtonHex_clicked();
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonApply_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void applyEnable();

private:
    Ui::DialogPEDosHeader *ui;
    unsigned int nOffset;
    unsigned int nSize;

    QLineEdit *lineEdite_magic;
    QLineEdit *lineEdite_cblp;
    QLineEdit *lineEdite_cp;
    QLineEdit *lineEdite_crlc;
    QLineEdit *lineEdite_cparhdr;
    QLineEdit *lineEdite_minalloc;
    QLineEdit *lineEdite_maxalloc;
    QLineEdit *lineEdite_ss;
    QLineEdit *lineEdite_sp;
    QLineEdit *lineEdite_csum;
    QLineEdit *lineEdite_ip;
    QLineEdit *lineEdite_cs;
    QLineEdit *lineEdite_lfarlc;
    QLineEdit *lineEdite_ovno;
    QLineEdit *lineEdite_oemid;
    QLineEdit *lineEdite_oeminfo;
    QLineEdit *lineEdite_lfanew;
    QLineEdit *lineEdite_res0;
    QLineEdit *lineEdite_res1;
    QLineEdit *lineEdite_res2;
    QLineEdit *lineEdite_res3;
    QLineEdit *lineEdite_res20;
    QLineEdit *lineEdite_res21;
    QLineEdit *lineEdite_res22;
    QLineEdit *lineEdite_res23;
    QLineEdit *lineEdite_res24;
    QLineEdit *lineEdite_res25;
    QLineEdit *lineEdite_res26;
    QLineEdit *lineEdite_res27;
    QLineEdit *lineEdite_res28;
    QLineEdit *lineEdite_res29;
};

#endif // DIALOGPEDOSHEADER_H
