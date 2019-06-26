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
#ifndef LITEWINDOW_H
#define LITEWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include "../_options.h"
#include "../scan.h"
#include <QDragEnterEvent>
#include <QUrl>
#include <QMessageBox>
#include <QMimeData>

namespace Ui
{
class LiteWindow;
}

class LiteWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LiteWindow(QWidget *parent=nullptr);
    ~LiteWindow();
    void openFile(QString sFileName);

private slots:
    void on_pushButtonOpen_clicked();
    void on_pushButtonExit_clicked();
    void appendSignature(QString sString);
    void appendError(QString sString);
    void on_checkBoxShowErrors_stateChanged(int arg1);
    void on_checkBoxAddToContextMenu_stateChanged(int arg1);
    bool getShellState();

private:
    Ui::LiteWindow *ui;
    Scan *scan;
    __DIE_OPTIONS options;

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);
};

#endif // LITEWINDOW_H
