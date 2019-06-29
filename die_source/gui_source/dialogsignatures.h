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
#ifndef DIALOGSIGNATURES_H
#define DIALOGSIGNATURES_H

#include "qxdialogx.h"
#include "../../binary.h"
#include "../scan.h"
#include <QListWidgetItem>
#include <QScriptEngineDebugger>
#include <QMainWindow>
#include "dialognewsignature.h"
#include <QShortcut>
#include <QCompleter>
#include <QStringListModel>
#include <QInputDialog>
#include "qscriptenginedebuggerx.h"

namespace Ui
{
class DialogSignatures;
}

class DialogSignatures : public QXDialogX
{
    Q_OBJECT

public:
    explicit DialogSignatures(QWidget *parent = 0);
    ~DialogSignatures();
    void setData(__DIE_OPTIONS *pOptions,QString sFileName,QString sCurrentSignature="");

private:
    Ui::DialogSignatures *ui;
    QString sFileName;
    QString sCurrentSignature;

    QString sFolder;
    QString sCurrent;
    QList<QString> listTooltips;

    PluginsScript pluginscript;
    PEFile pefile;
    Binary binary;
    TextFile text;
    ELFFile elffile;
    MSDOSFile msdosfile;
    MACHFile machfile;

    scriptPE scriptpe;
    scriptELF scriptelf;
    scriptMSDOS scriptmsdos;
    scriptMACH scriptmach;
    scriptBinary scriptbinary;
    scriptText scripttext;

    QAction *actDelete;
    QAction *actRename;

    QShortcut *shortcutDelete;
    QShortcut *shortcutRename;

    QCompleter *completer;

    QAbstractItemModel *modelFromFile(const QString &fileName);
    QAbstractItemModel *pModel;

    bool bHighlightChanged;
private slots:
    bool reload();
    bool reload(QString sNewCurrent);
    void on_listWidgetSignatures_currentRowChanged(int currentRow);
    void on_pushButtonNew_clicked();
    void on_pushButtonSave_clicked();
    void on_textEditSignature_textChanged();
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonDebug_clicked();
    void on_pushButtonRun_clicked();
    void _run(bool bIsDebug);
    void appendLog(QString sString);
    void on_pushButtonClear_clicked();

    void save();
    void deleteSignature();
    void renameSignature();

    void on_listWidgetSignatures_customContextMenuRequested(const QPoint &pos);

    void on_checkBoxHighlight_stateChanged(int arg1);

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // DIALOGSIGNATURES_H
