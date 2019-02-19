// Copyright (c) 2012-2018 hors<horsicq@gmail.com>
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
#include "_mainclass.h"

_MainClass::_MainClass(QObject *parent) :
    QObject(parent)
{
}

void _MainClass::setOptions(__DIE_OPTIONS *pOptions)
{
    this->pOptions=pOptions;
}
void _MainClass::handleMessage(QString szMessage)
{
    printf("%s",szMessage.toLatin1().data());

    if(pOptions->bSingleLineOutput)
    {
        printf("; ");
    }
    else
    {
        printf("\n");
    }
}

//void _MainClass::appendSignature(QString szMessage)
//{
//    QString sType=szMessage.section(": ",0,0);
//    if((pOptions->bShowFileFormatOnce)&&(sType!=sCurrentType))
//    {
//        szMessage=szMessage.section(": ",1,-1);
//        sCurrentType=sType;
//    }

//    handleMessage(szMessage);
//}

void _MainClass::appendError(QString szMessage)
{
    handleMessage(szMessage);
}

void _MainClass::appendFileName(QString szMessage)
{
    printf("%s",szMessage.toLatin1().data());

    if(pOptions->bSingleLineOutput)
    {
        printf(": ");
    }
    else
    {
        printf("\n");
    }
}
