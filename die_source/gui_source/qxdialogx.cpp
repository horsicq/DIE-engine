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
#include "qxdialogx.h"

QXDialogX::QXDialogX(QWidget *parent) :
    QDialog(parent)
{
    connect(this, SIGNAL(reloadSignal()), this, SLOT(reload()));
    connect(this, SIGNAL(reloadSignal()), parent, SIGNAL(reloadSignal()));
    connect(this, SIGNAL(appendError(QString)), parent, SIGNAL(appendError(QString)));

    pOptions=0;
}

QXDialogX::~QXDialogX()
{
    if(pOptions)
    {
        pOptions->nDialogOpen--;
    }
}

void QXDialogX::setOptions(__DIE_OPTIONS *pOptions)
{
    this->pOptions=pOptions;

    reload();
}

int QXDialogX::exec()
{
    if(pOptions)
    {
        pOptions->nDialogOpen++;
    }

    return QDialog::exec();
}
