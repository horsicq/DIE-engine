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
#include "scriptbinary.h"

scriptBinary::scriptBinary(QObject *parent) :
    scriptGeneric(parent)
{
}

void scriptBinary::setData(Binary *binary)
{
    this->binary=binary;
}
//bool scriptBinary::isPlainText()
//{
//    return binary->isPlainText();
//}

bool scriptBinary::compare(QString sSignature,unsigned int nOffset)
{
    return binary->compare(sSignature,nOffset);
}

//bool scriptBinary::compareHeader(QString sSignature)
//{
//    return binary->compareHeader(sSignature);
//}

bool scriptBinary::compareEP(QString sSignature,unsigned int nOffset)
{
    return binary->compareEP(sSignature,nOffset);
}

//bool scriptBinary::compareEPn(QString sSignature,unsigned int nOffset)
//{
//    return compare(sSignature,nOffset);
//}
//void scriptBinary::_log(QString sString)
//{
//    emit appendError(sString);
//}

QString scriptBinary::getString(unsigned int nOffset, unsigned int nSize)
{
    return binary->getString(nOffset,nSize);
}

unsigned int scriptBinary::readDword(unsigned int nOffset)
{
    return binary->readDword(nOffset);
}

unsigned short scriptBinary::readWord(unsigned int nOffset)
{
    return binary->readWord(nOffset);
}

unsigned char scriptBinary::readByte(unsigned int nOffset)
{
    return binary->readByte(nOffset);
}

unsigned int scriptBinary::getSize()
{
    return binary->size();
}

QString scriptBinary::getFileDirectory()
{
    return binary->getFileDirectory();
}

QString scriptBinary::getFileBaseName()
{
    return binary->getFileBaseName();
}

QString scriptBinary::getFileSuffix()
{
    return binary->getFileSuffix();
}

QString scriptBinary::getFileCompleteSuffix()
{
    return binary->getFileCompleteSuffix();
}

int scriptBinary::findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue)
{
    return binary->findByte(nOffset,nSize,cValue);
}

int scriptBinary::findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue)
{
    return binary->findWord(nOffset,nSize,sValue);
}

int scriptBinary::findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue)
{
    return binary->findDword(nOffset,nSize,nValue);
}

int scriptBinary::findString(unsigned int nOffset, unsigned int nSize, QString sValue)
{
    return binary->findString(nOffset,nSize,sValue);
}

int scriptBinary::findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return binary->findSignature(nOffset,nSize,sSignature);
}

bool scriptBinary::isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return binary->isSignaturePresent(nOffset,nSize,sSignature);
}

double scriptBinary::calculateEntropy(unsigned int nOffset, unsigned int nSize)
{
    return binary->calculateEntropy(nOffset,nSize);
}

QString scriptBinary::getSignature(unsigned int nOffset, unsigned int nSize)
{
    return binary->getSignature(nOffset,nSize);
}

QString scriptBinary::calculateMD5(unsigned int nOffset, unsigned int nSize)
{
    return binary->calculateMD5(nOffset,nSize);
}
