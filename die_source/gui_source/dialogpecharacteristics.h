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
#ifndef DIALOGPECHARACTERISTICS_H
#define DIALOGPECHARACTERISTICS_H

#include "dialogpegeneric.h"

namespace Ui
{
class DialogPECharacteristics;
}

class DialogPECharacteristics : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPECharacteristics(QWidget *parent);
    ~DialogPECharacteristics();

private:
    Ui::DialogPECharacteristics *ui;
    bool bEdited;

signals:
    void checked();

private slots:
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonOK_clicked();

    bool reload();

    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void lineEditCharacteristics_textChanged(const QString &arg1);
    void on_checkBoxRELOCS_STRIPPED_stateChanged(int arg1);
    void on_checkBoxEXECUTABLE_IMAGE_stateChanged(int arg1);
    void on_checkBoxLINE_NUMS_STRIPPED_stateChanged(int arg1);
    void on_checkBoxLOCAL_SYMS_STRIPPED_stateChanged(int arg1);
    void on_checkBoxAGGRESIVE_WS_TRIM_stateChanged(int arg1);
    void on_checkBoxLARGE_ADDRESS_AWARE_stateChanged(int arg1);
    void on_checkBoxBYTES_REVERSED_LO_stateChanged(int arg1);
    void on_checkBox32BIT_MACHINE_stateChanged(int arg1);
    void on_checkBoxDEBUG_STRIPPED_stateChanged(int arg1);
    void on_checkBoxREMOVABLE_RUN_FROM_SWAP_stateChanged(int arg1);
    void on_checkBoxNET_RUN_FROM_SWAP_stateChanged(int arg1);
    void on_checkBoxSYSTEM_stateChanged(int arg1);
    void on_checkBoxDLL_stateChanged(int arg1);
    void on_checkBoxUP_SYSTEM_ONLY_stateChanged(int arg1);
    void on_checkBoxBYTES_REVERSED_HI_stateChanged(int arg1);
    void checkbox_reload();
};

#endif // DIALOGPECHARACTERISTICS_H
