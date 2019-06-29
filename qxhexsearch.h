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
#ifndef QXHEXSEARCH_H
#define QXHEXSEARCH_H

#include <QDialog>
#include <QMessageBox>

namespace Ui
{
class QXHexSearch;
}

class QXHexSearch : public QDialog
{
    Q_OBJECT

public:
    explicit QXHexSearch(QWidget *parent = 0);
    ~QXHexSearch();
    void _showToolTips(bool bState);
    void setLittleEndian(bool bState);
    void setTab(int nTab);

private:
    Ui::QXHexSearch *ui;
signals:
    void signature(QString sSignature,bool bIsCursor);

private slots:
    void toHEX();
    void on_lineEditValue_textChanged(const QString &arg1);
    void on_radioButtonBigEndian_toggled(bool checked);
    void on_radioButtonLittleEndian_toggled(bool checked);
    void on_radioButtonInt64_toggled(bool checked);
    void on_radioButtonChar_toggled(bool checked);
    void on_radioButtonUChar_toggled(bool checked);
    void on_radioButtonShort_toggled(bool checked);
    void on_radioButtonUShort_toggled(bool checked);
    void on_radioButtonInt_toggled(bool checked);
    void on_radioButtonUInt_toggled(bool checked);
    void on_radioButtonUInt64_toggled(bool checked);
    void on_radioButtonDouble_toggled(bool checked);
    void on_radioButtonFloat_toggled(bool checked);
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_tabWidget_currentChanged(int index);
};

#endif // QXHEXSEARCH_H
