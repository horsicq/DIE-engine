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
#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>

#include "../global.h"
#include "dialogabout.h"
#include "dialogoptions.h"
#include "dialogshortcuts.h"
#include "dialogselectstyle.h"
#include "xoptions.h"
#ifdef USE_YARA
#include "xyara.h"
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class GuiMainWindow;
}
QT_END_NAMESPACE

class GuiMainWindow : public QMainWindow {
    Q_OBJECT

    enum SC {
        SC_OPENFILE = 0,
        SC_FULLSCREEN,
        SC_EXIT,
        __SC_SIZE
    };

public:
    GuiMainWindow(QWidget *pParent = nullptr);
    ~GuiMainWindow();

private slots:
    void on_toolButtonExit_clicked();
    void on_toolButtonAbout_clicked();
    void on_toolButtonShortcuts_clicked();
    void on_toolButtonOptions_clicked();
    void on_toolButtonOpenFile_clicked();
    void on_toolButtonDemangle_clicked();
    void on_toolButtonRecentFiles_clicked();
    void on_checkBoxAdvanced_toggled(bool bChecked);
    void on_lineEditFileName_returnPressed();

    void exitSlot();
    void openFileSlot();
    void fullScreenSlot();

    QString getCurrentFileName();
    void adjustView();
    void updateShortcuts();
    void adjustFile();
    void setAdvanced(bool bState);
    void errorMessageSlot(const QString &sText);

public slots:
    void _process(const QString &sName);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Ui::GuiMainWindow *ui;
    XOptions g_xOptions;
    XShortcuts g_xShortcuts;
    QMenu *g_pRecentFilesMenu;
    QShortcut *shortCuts[__SC_SIZE];
    bool g_bFullScreen;
};
#endif  // GUIMAINWINDOW_H
