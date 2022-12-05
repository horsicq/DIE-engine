/* Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>

#include "../global.h"
#include "dialogabout.h"
#include "dialogoptions.h"
#include "dialogshortcuts.h"
#include "xoptions.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GuiMainWindow;
}
QT_END_NAMESPACE

class GuiMainWindow : public QMainWindow {
    Q_OBJECT

public:
    GuiMainWindow(QWidget *pParent = nullptr);
    ~GuiMainWindow();

private slots:
    void on_pushButtonExit_clicked();
    void on_pushButtonAbout_clicked();
    void on_pushButtonShortcuts_clicked();
    void on_pushButtonOptions_clicked();
    void on_pushButtonOpenFile_clicked();
    void on_pushButtonDemangle_clicked();
    void on_toolButtonRecentFiles_clicked();
    void on_checkBoxAdvanced_toggled(bool bChecked);

    QString getCurrentFileName();
    void adjust();
    void adjustFile();
    void setAdvanced(bool bState);

public slots:
    void processFile(QString sFileName);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Ui::GuiMainWindow *ui;
    XOptions g_xOptions;
    XShortcuts g_xShortcuts;
    QMenu *g_pRecentFilesMenu;
};
#endif  // GUIMAINWINDOW_H
