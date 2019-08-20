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
#ifndef DIALOGMACHHEADER_H
#define DIALOGMACHHEADER_H

#include "dialogmachgeneric.h"
#include "dialoghde.h"
namespace Ui
{
class DialogMACHHeader;
}

class DialogMACHHeader : public DialogMACHGeneric
{
    Q_OBJECT

public:
    explicit DialogMACHHeader(QWidget *parent=nullptr);
    ~DialogMACHHeader();

    void setData(MACHFile *pmachfile, __DIE_OPTIONS *pOptions);

private slots:
    void applyEnable();
    bool reload();

    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void on_pushButtonHex_clicked();

private:
    Ui::DialogMACHHeader *ui;

    QLineEditX *lineEdite_magic;
    QLineEditX *lineEdite_cputype;
    QLineEditX *lineEdite_cpusubtype;
    QLineEditX *lineEdite_filetype;
    QLineEditX *lineEdite_ncmds;
    QLineEditX *lineEdite_sizeofcmds;
    QLineEditX *lineEdite_flags;
    QLineEditX *lineEdite_reserved;
};

#endif // DIALOGMACHHEADER_H
