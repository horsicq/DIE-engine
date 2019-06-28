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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QFileDialog>
#include <QScriptEngineDebugger>
#include <QPluginLoader>
#ifdef Q_OS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif
#include "formpe.h"
#include "formelf.h"
#include "formmsdos.h"
#include "formmach.h"
#include "dialogoptions.h"
#include "dialogabout.h"
#include "dialogpe.h"
#include "../scan.h"
#include "dialogdirectoryheuristicscan.h"
#include "dialogextra.h"
#include "dialoginfo.h"
#include "dialogflc.h"
#include "dialogentropy.h"
#include "dialogsearch.h"
#include "dialogselectplugin.h"
#include "threadplugin.h"
#include "dialogsignatures.h"
#include "../scriptdie.h"

#include "../plugins_source/plugininterface.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    //typedef void (*SetBuffer)(char *,int);
    static const int WINDOW_DELTA=60;
    static const int WINDOW_WIDTH=580+40;
    static const int WINDOW_WIDTH_MAX=16777215;
    static const int WINDOW_HEIGHT_BINARY=230+WINDOW_DELTA;
    static const int WINDOW_HEIGHT_PE=314+WINDOW_DELTA;
    static const int WINDOW_HEIGHT_ELF=290+WINDOW_DELTA;
    static const int WINDOW_HEIGHT_MACH=290+WINDOW_DELTA;
    static const int WINDOW_HEIGHT_MSDOS=230+WINDOW_DELTA;
    static const int WINDOW_HEIGHT_MAX=16777215;

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadPlugins();
    void on_pushButtonExit_clicked();
    void on_pushButtonOptions_clicked();
    void on_pushButtonAbout_clicked();
    void on_pushButtonHex_clicked();
    void on_pushButtonOpenFile_clicked();
    void openFile(QString sFileName);

    void clearResult();

    void controlsEnable(bool bEnable);
    void on_lineEditType_textChanged(const QString &arg1);
    bool reload();
    void _reload();
    void setProgressBarScan(int nMax, int nValue);
    void scanCompleteSlot(int nMs);

    void setTop(bool bState);

    void resizeScanResult(bool bContent);

    void on_pushButtonScan_clicked();
    void scanSlot();
    void setHeuristicEnabled();
    void on_pushButtonDirectory_clicked();

    void on_pushButtonExtra_clicked();
    void _pushButtonInfo();

    void _error(QString sString);
    void on_tableWidgetResult_die_clicked(const QModelIndex &index);
    void updateOptions();
    //    void getLastDirectory();
    void on_pushButtonClearLog_clicked();
    void on_pushButtonSaveLog_clicked();
    void appendLog(QString sString);
    void on_pushButtonEntropy_clicked();

    void _directory(QString sDirectoryName);
    void showTooltips(bool bShow);
    //void on_tableWidgetResult_customContextMenuRequested(const QPoint &pos);
    void copyAsString();
    void showSource(QString sSignature);

    void on_pushButtonSelect_clicked();
    void on_pushButtonRun_clicked();
    void on_pushButtonInfo_clicked();
    void on_pushButtonDebug_clicked();
    void on_pushButtonClear_clicked();
    void on_pushButtonSave_clicked();

    void pluginFinished();
    void setPluginEnabled();

    void appendScriptMessage(QString sString);
    void appendScriptWarningMessage(QString sString);
    void appendScriptErrorMessage(QString sString);
    void appendScriptSuccessMessage(QString sString);

    void on_lineEditScriptName_textChanged(const QString &arg1);

    void setCurrentSize(bool bState);
    void tabScan();
    void tabScripts();
    void tabPlugins();
    void tabLog();
    void on_pushButtonSignatures_clicked();
    void on_listWidgetPlugins_itemSelectionChanged();
    void on_tabWidgetMain_currentChanged(int index);
    void on_lineEditFileName_textChanged(const QString &arg1);
    void on_pushButtonSearch_clicked();
    void on_pushButtonFLC_clicked();

    void on_comboBoxScanMethod_currentIndexChanged(int index);

    void changeMethod(SCAN_METHODS sm,bool bOpenFile=false);
    void on_pushButtonScanMethodInfo_clicked();

signals:
    void reloadSignal();
    void scanSignal();
    void appendError(QString sError);
    void directory(QString sDirectoryName);
    void reset();

private:
    Ui::MainWindow *ui;
    QWidget *staticform;
    //QString sFileName;
    unsigned int nFileSize;
    Scan *scan;
    QTimer timerHeuristic;
    QTimer timerPlugin;
    // QString sExtra;
    //    QTime scanTime;
    QString sCurrentFile;
    __DIE_OPTIONS options;
    QAction *actCopyAsString;
    //QAction *actShowSource;
    PluginsScript pluginscript;
    ThreadPlugin *pThreadPlugin;
    scriptDIE scriptdie;
    //    scriptSql scriptsql;
    PEFile __pefile;
    Binary __binary;
    ELFFile __elffile;
    MSDOSFile __msdosfile;
    QString sCurrentType;
    QShortcut *shortcut1;
    QShortcut *shortcut2;
    QShortcut *shortcut3;
    QShortcut *shortcut4;
    //    QShortcut *shortcut5;
    //    QShortcut *shortcut6;
    //    QShortcut *shortcut7;
    //    QShortcut *shortcut8;
    //    QShortcut *shortcut9;
    //    QShortcut *shortcut10;
    //    QShortcut *shortcut11;
    //    QShortcut *shortcut12;
    //    QShortcut *shortcut13;
    //    QShortcut *shortcut14;
    //    QShortcut *shortcut15;
    //    QShortcut *shortcut16;
    //    QShortcut *shortcut17;
    //    QShortcut *shortcut18;
    //    QShortcut *shortcut19;
    //    QShortcut *shortcut20;

    //    bool bIsDialogsopen;
    QPluginLoader pluginloader;
    DIEPLUGIN dieplugin;

    bool bIsScan;
    bool bIsRun;
    bool bIsLoad;

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);

};

#endif // MAINWINDOW_H




