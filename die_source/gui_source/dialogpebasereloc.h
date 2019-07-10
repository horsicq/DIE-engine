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
#ifndef DIALOGPEBASERELOCK_H
#define DIALOGPEBASERELOCK_H

#include "dialogpegeneric.h"
#include "qlineeditx.h"

namespace Ui
{
class DialogPEBaseReloc;
}

class DialogPEBaseReloc : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPEBaseReloc(QWidget *parent);
    ~DialogPEBaseReloc();

private slots:
    void on_pushButtonOK_clicked();
    bool reload();
    //    void setTableEnable();
    void on_tableWidgetRelocs_clicked(const QModelIndex &index);

private:
    Ui::DialogPEBaseReloc *ui;

    //    QByteArray baSelected;
    unsigned int nOffset;
    unsigned int nSize;

    QAction *actCopyAsStringReloc;

    QList<unsigned int> listAddresses;
    QList<unsigned int> listSizes;
    QList<unsigned int> listOffsets;
};

#endif // DIALOGPEBASERELOCK_H
