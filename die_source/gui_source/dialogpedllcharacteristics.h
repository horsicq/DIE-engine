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
#ifndef DIALOGDLLCHARACTERISTICS_H
#define DIALOGDLLCHARACTERISTICS_H

#include "dialogpegeneric.h"

namespace Ui
{
class DialogPEDllCharacteristics;
}

class DialogPEDllCharacteristics : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPEDllCharacteristics(QWidget *parent);
    ~DialogPEDllCharacteristics();

private:
    Ui::DialogPEDllCharacteristics *ui;
    bool bEdited;

signals:
    void checked();

private slots:
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonOK_clicked();

    bool reload();

    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void lineEditDllCharacteristics_textChanged(const QString &arg1);

    void checkbox_reload();
    void on_checkBoxDYNAMIC_BASE_stateChanged(int arg1);
    void on_checkBoxFORCE_INTEGRITY_stateChanged(int arg1);
    void on_checkBoxNX_COMPAT_stateChanged(int arg1);
    void on_checkBoxNO_ISOLATION_stateChanged(int arg1);
    void on_checkBoxNO_SEH_stateChanged(int arg1);
    void on_checkBoxNO_BIND_stateChanged(int arg1);
    void on_checkBoxWDM_DRIVER_stateChanged(int arg1);
    void on_checkBoxTERMINAL_SERVER_AWARE_stateChanged(int arg1);
};

#endif // DIALOGDLLCHARACTERISTICS_H
