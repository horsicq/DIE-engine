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
#ifndef DIALOGELFSECTIONHEADER_H
#define DIALOGELFSECTIONHEADER_H

#include "dialogelfgeneric.h"

namespace Ui
{
class DialogELFSectionHeader;
}

class DialogELFSectionHeader : public DialogELFGeneric
{
    Q_OBJECT

public:
    explicit DialogELFSectionHeader(QWidget *parent=nullptr);
    ~DialogELFSectionHeader();

    void setData(__DIE_OPTIONS *pOptions,ELFFile *pelffile,int nSection);

private slots:
    bool reload();
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void applyEnable();

signals:
    void reloadSignal();
    void appendError(QString sString);

private:
    Ui::DialogELFSectionHeader *ui;
    //    ELFFile *pelffile;
    //    __OPTIONS *pOptions;
    int nSection;

    QLineEditX *lineEditsh_name;
    QLineEditX *lineEditsh_type;
    QLineEditX *lineEditsh_flags;
    QLineEditX *lineEditsh_addr;
    QLineEditX *lineEditsh_offset;
    QLineEditX *lineEditsh_size;
    QLineEditX *lineEditsh_link;
    QLineEditX *lineEditsh_info;
    QLineEditX *lineEditsh_addralign;
    QLineEditX *lineEditsh_entsize;
};

#endif // DIALOGELFSECTIONHEADER_H
