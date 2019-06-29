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
#ifndef DIALOGMSDOSHEADER_H
#define DIALOGMSDOSHEADER_H

#include "dialogmsdosgeneric.h"
#include "dialoghde.h"

namespace Ui
{
class DialogMSDOSHeader;
}

class DialogMSDOSHeader : public DialogMSDOSGeneric
{
    Q_OBJECT

public:
    explicit DialogMSDOSHeader(QWidget *parent);
    ~DialogMSDOSHeader();
    //    void setData(MSDOSFile *msdosfile, __OPTIONS *pOptions);

private slots:

    bool reload();
    void on_pushButtonHex_clicked();
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonApply_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void applyEnable();

private:
    Ui::DialogMSDOSHeader *ui;
    unsigned int nOffset;
    unsigned int nSize;

    QLineEditX *lineEdite_magic;
    QLineEditX *lineEdite_cblp;
    QLineEditX *lineEdite_cp;
    QLineEditX *lineEdite_crlc;
    QLineEditX *lineEdite_cparhdr;
    QLineEditX *lineEdite_minalloc;
    QLineEditX *lineEdite_maxalloc;
    QLineEditX *lineEdite_ss;
    QLineEditX *lineEdite_sp;
    QLineEditX *lineEdite_csum;
    QLineEditX *lineEdite_ip;
    QLineEditX *lineEdite_cs;
    QLineEditX *lineEdite_lfarlc;
    QLineEditX *lineEdite_ovno;
    QLineEditX *lineEdite_oemid;
    QLineEditX *lineEdite_oeminfo;
    QLineEditX *lineEdite_lfanew;
    QLineEditX *lineEdite_res0;
    QLineEditX *lineEdite_res1;
    QLineEditX *lineEdite_res2;
    QLineEditX *lineEdite_res3;
    QLineEditX *lineEdite_res20;
    QLineEditX *lineEdite_res21;
    QLineEditX *lineEdite_res22;
    QLineEditX *lineEdite_res23;
    QLineEditX *lineEdite_res24;
    QLineEditX *lineEdite_res25;
    QLineEditX *lineEdite_res26;
    QLineEditX *lineEdite_res27;
    QLineEditX *lineEdite_res28;
    QLineEditX *lineEdite_res29;
};

#endif // DIALOGMSDOSHEADER_H
