/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef LITEMAINWINDOW_H
#define LITEMAINWINDOW_H

#include <QMainWindow>
#include <QMimeData>

#include "../global.h"
#include "die_script.h"
#include "xoptions.h"
#include "scanitemmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LiteMainWindow;
}
QT_END_NAMESPACE

class LiteMainWindow : public QMainWindow {
    Q_OBJECT

public:
    LiteMainWindow(QWidget *pParent = nullptr);
    ~LiteMainWindow();

public slots:
    void processFile(const QString &sFileName);

private slots:
    void on_pushButtonScan_clicked();
    void on_pushButtonExit_clicked();
    void on_pushButtonOpenFile_clicked();
    void process();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void keyPressEvent(QKeyEvent *pEvent) override;

private:
    Ui::LiteMainWindow *ui;
    DiE_Script *g_pDieScript;
    XOptions g_xOptions;
    bool g_bInit;
};
#endif  // LITEMAINWINDOW_H
