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
#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include "qxdialogx.h"
#include "dialogoptions.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTime>
#include "qwt_global.h"

namespace Ui
{
class DialogAbout;
}

class DialogAbout : public QXDialogX
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent);
    ~DialogAbout();
    void setData(__DIE_OPTIONS *pOptions);

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCheckUpdate_clicked();
    void replyFinished(QNetworkReply *pReply);
    void tabInfo();
    void tabLibs();
    void tabThanks();
    void on_listWidgetThanks_currentTextChanged(const QString &currentText);
    void setButtonImage(QString sImageName);
    void on_pushButtonAvatar_clicked();
    bool reload();

private:
    Ui::DialogAbout *ui;
    QNetworkAccessManager* m_manager;
    QShortcut *shortcut1;
    QShortcut *shortcut2;
    QShortcut *shortcut3;
};

#endif // DIALOGABOUT_H
