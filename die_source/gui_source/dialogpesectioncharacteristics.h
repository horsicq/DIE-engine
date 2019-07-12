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
#ifndef DIALOGPESECTIONCHARACTERISTICS_H
#define DIALOGPESECTIONCHARACTERISTICS_H

#include "dialogpegeneric.h"

namespace Ui
{
class DialogPESectionCharacteristics;
}

class DialogPESectionCharacteristics : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPESectionCharacteristics(QWidget *parent);
    ~DialogPESectionCharacteristics();
    void setData(__DIE_OPTIONS *pOptions,PEFile *pefile,  int nSection);

private:
    Ui::DialogPESectionCharacteristics *ui;

    int nSection;
    bool bEdited;

signals:
    void checked();

private slots:
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonOK_clicked();

    bool reload();

    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void on_lineEditSectionSectionCharacteristics_textChanged(const QString &arg1);
    void on_comboBoxALIGN_currentIndexChanged(int index);
    void on_checkBoxCNT_CODE_stateChanged(int arg1);
    void on_checkBoxCNT_INITIALIZED_DATA_stateChanged(int arg1);
    void on_checkBoxCNT_UNINITIALIZED_DATA_stateChanged(int arg1);
    void on_checkBoxLNK_INFO_stateChanged(int arg1);
    void on_checkBoxLNK_REMOVE_stateChanged(int arg1);
    void on_checkBoxLNK_COMDAT_stateChanged(int arg1);
    void on_checkBoxNO_DEFER_SPEC_EXC_stateChanged(int arg1);
    void on_checkBoxGPREL_stateChanged(int arg1);
    void on_checkBoxMEM_PURGEABLE_stateChanged(int arg1);
    void on_checkBoxMEM_LOCKED_stateChanged(int arg1);
    void on_checkBoxMEM_PRELOAD_stateChanged(int arg1);
    void on_checkBoxLNK_NRELOC_OVFL_stateChanged(int arg1);
    void on_checkBoxMEM_DISCARDABLE_stateChanged(int arg1);
    void on_checkBoxMEM_NOT_CACHED_stateChanged(int arg1);
    void on_checkBoxMEM_NOT_PAGED_stateChanged(int arg1);
    void on_checkBoxMEM_SHARED_stateChanged(int arg1);
    void on_checkBoxMEM_EXECUTE_stateChanged(int arg1);
    void on_checkBoxMEM_READ_stateChanged(int arg1);
    void on_checkBoxMEM_WRITE_stateChanged(int arg1);
    void checkbox_reload();
};

#endif // DIALOGPESECTIONCHARACTERISTICS_H
