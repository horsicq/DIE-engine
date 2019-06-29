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
#ifndef SCRIPTDIE_H
#define SCRIPTDIE_H

#include <QObject>
//#include <QFileDialog>

class scriptDIE : public QObject
{
    Q_OBJECT
public:
    explicit scriptDIE(QObject *parent=nullptr);
    void setData(QString sFileName);
    void setData(QWidget *parent);

signals:
    void appendMessage(QString sString);
    void appendWarningMessage(QString sString);
    void appendErrorMessage(QString sString);
    void appendSuccessMessage(QString sString);

public slots:
    void addMessage(QString sString);
    void addWarningMessage(QString sString);
    void addErrorMessage(QString sString);
    void addSuccessMessage(QString sString);
    QString getCurrentFileName();
    //QString getOpenFileName(QString caption,QString dir,QString filter);
    //QString getSaveFileName(QString caption,QString dir,QString filter);
    //QString getExistingDirectoryName(QString caption,QString dir);
    QString byteToHex(unsigned char cValue);
    QString wordToHex(unsigned short sValue);
    QString dwordToHex(unsigned int nValue);
    QString qwordToHex(unsigned long long qValue);
private:
    QString sFileName;
    QWidget *parent;
};

#endif // SCRIPTDIE_H
