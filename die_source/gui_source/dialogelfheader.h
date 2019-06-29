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
#ifndef DIALOGELFHEADER_H
#define DIALOGELFHEADER_H

#include "dialogelfgeneric.h"
#include "dialoghde.h"
#include "dialogelfclass.h"
#include "dialogelfdata.h"
#include "dialogelftype.h"
#include "dialogelfmachine.h"
namespace Ui
{
class DialogELFHeader;
}

class DialogELFHeader : public DialogELFGeneric
{
    Q_OBJECT

public:
    explicit DialogELFHeader(QWidget *parent = 0);
    ~DialogELFHeader();

    //    void setData(ELFFile *pelffile, __OPTIONS *pOptions);
private:
    Ui::DialogELFHeader *ui;

    QLineEdit *lineEdite_magic;
    QLineEdit *lineEdite_class;
    QLineEdit *lineEdite_data;
    QLineEdit *lineEdite_iversion;
    QLineEdit *lineEdite_osabi;
    QLineEdit *lineEdite_abiversion;
    QLineEdit *lineEdite_pad0;
    QLineEdit *lineEdite_pad1;
    QLineEdit *lineEdite_pad2;
    QLineEdit *lineEdite_pad3;
    QLineEdit *lineEdite_pad4;
    QLineEdit *lineEdite_pad5;
    QLineEdit *lineEdite_pad6;
    QLineEdit *lineEdite_pad7;
    QLineEdit *lineEdite_pad8;

    QLineEdit *lineEdite_type;
    QLineEdit *lineEdite_machine;
    QLineEdit *lineEdite_version;
    QLineEdit *lineEdite_entry;
    QLineEdit *lineEdite_phoff;
    QLineEdit *lineEdite_shoff;
    QLineEdit *lineEdite_flags;
    QLineEdit *lineEdite_ehsize;
    QLineEdit *lineEdite_phentsize;
    QLineEdit *lineEdite_phnum;
    QLineEdit *lineEdite_shentsize;
    QLineEdit *lineEdite_shnum;
    QLineEdit *lineEdite_shstrndx;

    QPushButtonX *pushButtonClassExtra;
    QPushButtonX *pushButtonDataExtra;
    QPushButtonX *pushButtonTypeExtra;
    QPushButtonX *pushButtonMACHineExtra;
    QPushButtonX *pushButtonEntryExtra;


    //    ELFFile *elffile;
    //    __OPTIONS *pOptions;

    //signals:
    //    void reloadSignal();
    //    void appendError(QString sString);

private slots:
    void applyEnable();
    bool reload();

    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void on_pushButtonHex_clicked();

    void pushButtonClassExtra_clicked();
    void pushButtonDataExtra_clicked();
    void pushButtonTypeExtra_clicked();
    void pushButtonMACHineExtra_clicked();
    void pushButtonEntryExtra_clicked();
};

#endif // DIALOGELFHEADER_H
