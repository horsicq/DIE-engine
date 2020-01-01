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
#ifndef ELFFORM_H
#define ELFFORM_H

#include "formgeneric.h"
#include "dialogelfheader.h"
#include "dialogelfpht.h"
#include "dialogelfsht.h"
#include "dialogelfstrings.h"

namespace Ui
{
class FormELF;
}

class FormELF : public FormGeneric
{
    Q_OBJECT

public:
    explicit FormELF(QWidget *parent=nullptr);
    ~FormELF();

private slots:
    bool reload();
    void on_pushButtonElfHeader_clicked();
    void on_pushButtonProgramHeaderTable_clicked();
    void on_pushButtonSectionHeaderTable_clicked();
    void on_pushButtonEntryPoint_clicked();
    void on_pushButtonStringTable_clicked();
    void on_pushButtonOverlay_clicked();

public slots:
    void showTooltips(bool bShow);

private:
    Ui::FormELF *ui;
};

#endif // ELFFORM_H
