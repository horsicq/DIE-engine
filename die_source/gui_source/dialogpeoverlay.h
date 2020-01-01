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
#ifndef DIALOGPEOVERLAY_H
#define DIALOGPEOVERLAY_H

#include "dialogpegeneric.h"
#include "dialoghde.h"
#include "dialogentropy.h"

namespace Ui
{
class DialogPEOverlay;
}

class DialogPEOverlay : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPEOverlay(QWidget *parent);
    ~DialogPEOverlay();

private slots:
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonOK_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonHex_clicked();
    void on_pushButtonDump_clicked();
    void on_pushButtonRemove_clicked();
    bool reload();
    void on_pushButtonReplace_clicked();
    void on_pushButtonEntropy_clicked();

private:
    Ui::DialogPEOverlay *ui;
    unsigned int nOffset;
    unsigned int nSize;
    bool bOverlayPresent;
};

#endif // DIALOGPEOVERLAY_H
