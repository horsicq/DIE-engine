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
#ifndef QXHEXFORM_H
#define QXHEXFORM_H

#include <QWidget>
//#include <QTextCodec>
#include "qxhexview.h"
#include "qcomboboxx.h"

namespace Ui
{
class QXHexForm;
}

class QXHexForm : public QWidget
{
    Q_OBJECT

public:
    explicit QXHexForm(QWidget *parent = 0);
    ~QXHexForm();
    void setData(QByteArray(*__ReadMemory)(QVariant,unsigned long long,unsigned long long,bool *),bool (*__WriteMemory)(QVariant,unsigned long long,QByteArray),QVariant parameter,unsigned long long nStartAddress,unsigned long long nMaxSize);
    void setType(QString sType,QString sMode,QString sSyntax);
    void setAddress(unsigned long long nAddress);
    void setSelection(unsigned long long nSelectionStart,unsigned long long nSelectionSize);
    void goToAddress(unsigned long long nAddress);
    void setName(QString sName);
    void setImage(QString sImage);
    void _setFont(QFont font);
    void setAddressWidth(unsigned int nAddressWidth);
    void _showToolTips(bool bState);
    void setLittleEndian(bool bState);
    void setVorBuffer(bool bState);

private slots:
    void on_checkBoxAddressAsHex_stateChanged(int arg1);
    void setSelectionStart(unsigned long long nValue);
    void setSelectionSize(unsigned long long nValue);
    void setCursor(unsigned long long nValue);
    void on_comboBoxType_currentIndexChanged(const QString &arg1);
    void on_comboBoxMode_currentIndexChanged(const QString &arg1);
    void on_comboBoxSyntax_currentIndexChanged(const QString &arg1);
    void setComboBoxType(QString sType);
    void setComboBoxMode(QString sMode);
    void setComboBoxSyntax(QString sSyntax);

    void on_pushButtonExtra_clicked();

    void on_comboBoxImage_currentIndexChanged(const QString &arg1);
    void setReadOnly(bool bState);

    void on_checkBoxReadOnly_stateChanged(int arg1);

    void setHEXfocus();

    void on_pushButtonReload_clicked();

    //    void reloadMemoryMap(QList<__MEMORYMAP> *pMemoryMap);
    //    void handleMemoryMap(bool bIsHEX);

signals:
    void appendError(QString sString);
    void closeDialog();

private:
    Ui::QXHexForm *ui;
    QString sType;
    QString sMode;
    QString sSyntax;

    unsigned long long nSelectionStart;
    unsigned long long nSelectionSize;

    //    QTextCodec *sysCodec;
    //    QList<QByteArray> listCodecs;
};

#endif // QXHEXFORM_H
