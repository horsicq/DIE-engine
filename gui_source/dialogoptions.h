// Copyright (c) 2020 hors<horsicq@gmail.com>
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
#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include "xoptions.h"
#include "xbinary.h"
#include "../global.h"

namespace Ui {
class DialogOptions;
}

class DialogOptions : public QDialog
{
    Q_OBJECT

    enum TAB
    {
        TAB_GENERAL=0,
        TAB_SCAN,
        TAB_APPEARANCE,
        TAB_CONTEXT
    };

public:
    explicit DialogOptions(QWidget *pParent,XOptions *pOptions);
    ~DialogOptions();

private slots:
#ifdef Q_OS_WIN
    void contextState();
#endif
    void on_pushButtonCancel_clicked();
    void on_pushButtonOK_clicked();
    void on_listWidgetOptions_currentRowChanged(int nCurrentRow);
    void on_toolButtonDIEDatabase_clicked();
    void on_pushButtonRegister_clicked();
    void on_pushButtonClear_clicked();
    void on_toolButtonDIEInfo_clicked();

private:
    Ui::DialogOptions *ui;
    XOptions *pOptions;
};

#endif // DIALOGOPTIONS_H
