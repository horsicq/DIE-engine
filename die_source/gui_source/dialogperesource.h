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
#ifndef DIALOGPERESOURCE_H
#define DIALOGPERESOURCE_H

#include "dialogpegeneric.h"
#include "dialoghde.h"
#include "dialogpeversioninfo.h"
#include "dialogpemanifest.h"

namespace Ui
{
class DialogPEResource;
}

class DialogPEResource : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPEResource(QWidget *parent = 0);
    ~DialogPEResource();

private slots:
    bool reload();
    void on_pushButtonOK_clicked();
    void on_treeWidgetResource_customContextMenuRequested(const QPoint &pos);
    void hex();
    void dump();
    void version();
    void manifest();
    void on_pushButtonManifest_clicked();
    void on_pushButtonVersion_clicked();

private:
    Ui::DialogPEResource *ui;

    QAction *actHex;
    QAction *actDump;
    QAction *actVersion;
    QAction *actManifest;
};

#endif // DIALOGPERESOURCE_H
