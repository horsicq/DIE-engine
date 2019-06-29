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
#ifndef PEFORM_H
#define PEFORM_H

#include "formgeneric.h"
#include "dialoghde.h"
#include "dialogpeversioninfo.h"
#include "dialogpe.h"

namespace Ui
{
class FormPE;
}

class FormPE : public FormGeneric
{
    Q_OBJECT

public:
    explicit FormPE(QWidget *parent = 0);
    ~FormPE();

private:
    Ui::FormPE *ui;
public slots:
    bool reload();
    void on_pushButtonPE_clicked();
    void on_pushButtonResource_clicked();
    void on_pushButtonOverlay_clicked();
    void on_pushButtonImport_clicked();
    void on_pushButtonExport_clicked();
    //    void on_pushButtonStub_clicked();
    void on_pushButtonNumberOfSections_clicked();
    void on_pushButtonEntryPoint_clicked();
    void on_pushButtonNET_clicked();

    void showTooltips(bool bShow);


};

#endif // PEFORM_H
