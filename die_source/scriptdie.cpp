// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#include "scriptdie.h"

scriptDIE::scriptDIE(QObject *parent) :
    QObject(parent)
{
    this->parent=0;
}

void scriptDIE::setData(QString sFileName)
{
    this->sFileName=sFileName;
}

void scriptDIE::setData(QWidget *parent)
{
    this->parent=parent;
}

void scriptDIE::addMessage(QString sString)
{
    emit appendMessage(sString);
}

void scriptDIE::addWarningMessage(QString sString)
{
    emit appendWarningMessage(sString);
}

void scriptDIE::addErrorMessage(QString sString)
{
    emit appendErrorMessage(sString);
}

void scriptDIE::addSuccessMessage(QString sString)
{
    emit appendSuccessMessage(sString);
}

QString scriptDIE::getCurrentFileName()
{
    return sFileName;
}

/*QString scriptPlugin::getOpenFileName(QString caption, QString dir, QString filter)
{
    return QFileDialog::getOpenFileName(0,caption,dir,filter);
}

QString scriptPlugin::getSaveFileName(QString caption, QString dir, QString filter)
{
    return QFileDialog::getSaveFileName(0,caption,dir,filter);
}

QString scriptPlugin::getExistingDirectoryName(QString caption, QString dir)
{
    return QFileDialog::getExistingDirectory(0,caption,dir,QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
} */

QString scriptDIE::byteToHex(unsigned char cValue)
{
    return QString("%1").arg(cValue,2,16,QChar('0'));
}

QString scriptDIE::wordToHex(unsigned short sValue)
{
    return QString("%1").arg(sValue,4,16,QChar('0'));
}

QString scriptDIE::dwordToHex(unsigned int nValue)
{
    return QString("%1").arg(nValue,8,16,QChar('0'));
}

QString scriptDIE::qwordToHex(unsigned long long qValue)
{
    return QString("%1").arg(qValue,16,16,QChar('0'));
}
