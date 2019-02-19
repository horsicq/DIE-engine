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
#include "scriptgeneric.h"

scriptGeneric::scriptGeneric(QObject *parent) :
    QObject(parent)
{
}

void scriptGeneric::_log(QString sString)
{
    emit appendError(sString);
}


unsigned int scriptGeneric::swapBytes(unsigned int nValue)
{
    unsigned int nResult;

    ((char *)(&nResult))[3]=((char *)(&nValue))[0];
    ((char *)(&nResult))[2]=((char *)(&nValue))[1];
    ((char *)(&nResult))[1]=((char *)(&nValue))[2];
    ((char *)(&nResult))[0]=((char *)(&nValue))[3];

    return nResult;
}

QString scriptGeneric::upperCase(QString sString)
{
    return sString.toUpper();
}

