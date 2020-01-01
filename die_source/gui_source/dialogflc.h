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
#ifndef DIALOGFLC_H
#define DIALOGFLC_H

#include "qxdialogx.h"
#include "dialogoptions.h"
#include "dialoghde.h"

namespace Ui
{
class DialogFLC;
}

class DialogFLC : public QXDialogX
{
    Q_OBJECT

public:
    explicit DialogFLC(QWidget *parent);
    ~DialogFLC();
    void setData(__DIE_OPTIONS *pOptions,Binary *_binary);

private slots:
    void on_pushButtonOK_clicked();
    bool reload();

    void on_pushButtonFileOffset_clicked();

    QString valueToString(unsigned long long nValue);
    unsigned long long stringToValue(QString sValue,int nBits);

    void on_radioButtonFileOffset_toggled(bool checked);
    void on_radioButtonVirtualAddress_toggled(bool checked);
    void on_radioButtonRVA_toggled(bool checked);

    void on_lineEditBaseAddress_textChanged(const QString &arg1);
    void on_lineEditFileOffset_textChanged(const QString &arg1);
    void on_lineEditVirtualAddress_textChanged(const QString &arg1);
    void on_lineEditRVA_textChanged(const QString &arg1);

    void on_checkBoxAddressAsHex_toggled(bool checked);

private:
    Ui::DialogFLC *ui;
    Binary *_binary;

    QString sType;

    PEFile pefile;
    MSDOSFile msdosfile;
    MACHFile machfile;
    ELFFile elffile;
    bool bConvert;
};

#endif // DIALOGFLC_H
