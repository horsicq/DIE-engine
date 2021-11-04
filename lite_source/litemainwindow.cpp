// Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#include "litemainwindow.h"
#include "ui_litemainwindow.h"

LiteMainWindow::LiteMainWindow(QWidget *pParent)
    : QMainWindow(pParent)
    , ui(new Ui::LiteMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(XOptions::getTitle(X_APPLICATIONDISPLAYNAME,X_APPLICATIONVERSION));

    setAcceptDrops(true);

    if(QCoreApplication::arguments().count()>1)
    {
        QString sFileName=QCoreApplication::arguments().at(1);

        processFile(sFileName);
    }
}

LiteMainWindow::~LiteMainWindow()
{
    delete ui;
}

void LiteMainWindow::processFile(QString sFileName)
{
    ui->lineEditFileName->setText(sFileName);

    if(sFileName!="")
    {
        // TODO
    }
}


void LiteMainWindow::on_pushButtonScan_clicked()
{
    processFile(ui->lineEditFileName->text());
}

void LiteMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void LiteMainWindow::on_pushButtonOpenFile_clicked()
{
    QString sDirectory=""; // mb TODO

    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file")+QString("..."),sDirectory,tr("All files")+QString(" (*)"));

    if(!sFileName.isEmpty())
    {
        processFile(sFileName);
    }
}
