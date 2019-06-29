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
#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include "qxdialogx.h"
#include <QStyleFactory>
#include "../scan.h"
#include "handleoptions.h"
#include <QFontDialog>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>

namespace Ui
{
class DialogOptions;
}

class DialogOptions : public QXDialogX
{
    Q_OBJECT
public:
    explicit DialogOptions(QWidget *parent);
    ~DialogOptions();
    //    void setData(QMainWindow *mainWindow);
private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonApply_clicked();
    void on_checkBoxStayOnTop_stateChanged(int arg1);
    void on_checkBoxVersionDIE_stateChanged(int arg1);
    void on_checkBoxOptionsDIE_stateChanged(int arg1);
    void on_checkBoxScanAfterOpen_stateChanged(int arg1);
    void on_checkBoxSaveLastDirectory_stateChanged(int arg1);
    void on_checkBoxShowTooltips_stateChanged(int arg1);
    void on_comboBoxStyle_currentIndexChanged(int index);
    void on_comboBoxStyleSheet_currentIndexChanged(int index);
    void on_pushButtonExeSet_clicked();
    void on_pushButtonExeClear_clicked();
    void on_pushButtonDllSet_clicked();
    void on_pushButtonDllClear_clicked();
    void on_pushButtonSysSet_clicked();
    void on_pushButtonSysClear_clicked();

    void reloadShellState();
    bool getShellState(QString sEntry);
    void clearEntry(QString sEntry);
    void setExtension(QString sExtension);
    void clearExtension(QString sExtension);

    void on_checkBoxSingleApplication_stateChanged(int arg1);
    void on_checkBoxShowErrors_stateChanged(int arg1);
    void on_checkBoxResizeableMainWindow_stateChanged(int arg1);
    void on_checkBoxSectionsEntropyAfterOpen_stateChanged(int arg1);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_spinBoxEntropy_valueChanged(int arg1);

    void tab1();
    void tab2();
    void tab3();
    void tab4();
    void tab5();
    void tab6();
    void tab7();
    void tab8();

    void setTooltips(bool bIsEnable);

    void on_listWidgetTabs_currentRowChanged(int currentRow);
    void on_pushButtonFont_clicked();
    void on_lineEditFont_textChanged(const QString &arg1);
    void on_lineEditFontHEX_textChanged(const QString &arg1);
    void on_pushButtonFontHEX_clicked();

    void on_spinBoxAddressWidth_valueChanged(int arg1);
    void on_comboBoxEndianness_currentIndexChanged(int index);

    void on_checkBoxScanTime_stateChanged(int arg1);

    bool reload();

    void on_pushButtonAllSet_clicked();
    void on_pushButtonAllClear_clicked();
    void on_checkBoxEnablePlugins_stateChanged(int arg1);

    void on_comboBoxCodec_currentIndexChanged(int index);

    void on_pushButtonSetPathDefault_clicked();
    void on_lineEditDataBase_textChanged(const QString &arg1);
    void on_lineEditEditor_textChanged(const QString &arg1);
    void on_lineEditHelp_textChanged(const QString &arg1);
    void on_lineEditInfo_textChanged(const QString &arg1);
    void on_lineEditPlugins_textChanged(const QString &arg1);
    void on_lineEditQSS_textChanged(const QString &arg1);
    void on_lineEditScripts_textChanged(const QString &arg1);
    void on_lineEditSearch_textChanged(const QString &arg1);

    void on_pushButtonDataBase_clicked();
    void on_pushButtonEditor_clicked();
    void on_pushButtonHelp_clicked();
    void on_pushButtonInfo_clicked();
    void on_pushButtonPlugins_clicked();
    void on_pushButtonQSS_clicked();
    void on_pushButtonScripts_clicked();
    void on_pushButtonSearch_clicked();
    void on_pushButtonLangs_clicked();
    void on_comboBoxLanguage_currentIndexChanged(int index);

    void on_lineEditLangs_textChanged(const QString &arg1);
    void on_checkBoxLanguage_stateChanged(int arg1);

    void on_groupBoxFont_toggled(bool arg1);
    void on_checkBoxLanguage_toggled(bool checked);
    void on_comboBoxMethod_currentIndexChanged(int index);
    void on_checkBoxDeepScanNFD_stateChanged(int arg1);
    void on_checkBoxScanOverlayNFD_stateChanged(int arg1);
    void on_checkBoxDeepScanDIE_stateChanged(int arg1);
    void on_checkBoxResizeColumnsToContent_stateChanged(int arg1);
#ifdef USE_YARA
    void on_pushButtonYARADataBase_clicked();
#endif
private:
    Ui::DialogOptions *ui;
    //    QMainWindow *mainWindow;

    QShortcut *shortcut1;
    QShortcut *shortcut2;
    QShortcut *shortcut3;
    QShortcut *shortcut4;
    QShortcut *shortcut5;
    QShortcut *shortcut6;
    QShortcut *shortcut7;
    QShortcut *shortcut8;
};

#endif // DIALOGOPTIONS_H
