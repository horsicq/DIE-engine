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
#include "scriptelf.h"

scriptELF::scriptELF(QObject *parent) :
    scriptGeneric(parent)
{
}

void scriptELF::setData(ELFFile *elffile)
{
    this->elffile=elffile;

    //    emit appendError("Test2");
}
QString scriptELF::getGeneralOptions()
{
    QString sResult;

    bool bIs64=elffile->isELF64();

    QString sType;
    QString sBits;
    unsigned int nType=elffile->getElfHeader_type();

    if(nType==0)
    {
        sType="unknown";
    }
    else if(nType==1)
    {
        sType="relocatable";
    }
    else if(nType==2)
    {
        sType="executable";
    }
    else if(nType==3)
    {
        sType="shared object";
    }
    else if(nType==4)
    {
        sType="core file";
    }

    if(bIs64)
    {
        sBits="64";
    }
    else
    {
        sBits="32";
    }

    sResult=QString("%1 %2-%3").arg(sType).arg(elffile->getMachineAsString()).arg(sBits);

    return sResult;
}

unsigned short scriptELF::getElfHeader_type()
{
    return elffile->getElfHeader_type();
}

unsigned short scriptELF::getElfHeader_machine()
{
    return elffile->getElfHeader_machine();
}

unsigned int scriptELF::getElfHeader_version()
{
    return elffile->getElfHeader_version();
}

unsigned int scriptELF::getElfHeader_entry()
{
    return elffile->getElfHeader_entry();
}

unsigned long long scriptELF::getElfHeader_entry64()
{
    return elffile->getElfHeader_entry64();
}

unsigned int scriptELF::getElfHeader_phoff()
{
    return elffile->getElfHeader_phoff();
}

unsigned long long scriptELF::getElfHeader_phoff64()
{
    return elffile->getElfHeader_phoff64();
}

unsigned int scriptELF::getElfHeader_shoff()
{
    return elffile->getElfHeader_shoff();
}
unsigned long long scriptELF::getElfHeader_shoff64()
{
    return elffile->getElfHeader_shoff64();
}

unsigned int scriptELF::getElfHeader_flags()
{
    return elffile->getElfHeader_flags();
}

unsigned short scriptELF::getElfHeader_ehsize()
{
    return elffile->getElfHeader_ehsize();
}

unsigned short scriptELF::getElfHeader_phentsize()
{
    return elffile->getElfHeader_phentsize();
}

unsigned short scriptELF::getElfHeader_phnum()
{
    return elffile->getElfHeader_phnum();
}

unsigned short scriptELF::getElfHeader_shentsize()
{
    return elffile->getElfHeader_shentsize();
}

unsigned short scriptELF::getElfHeader_shnum()
{
    return elffile->getElfHeader_shnum();
}

unsigned short scriptELF::getElfHeader_shstrndx()
{
    return elffile->getElfHeader_shstrndx();

}
//bool scriptELF::compareHeader(QString sSignature)
//{
//    return elffile->compareHeader(sSignature);
//}

bool scriptELF::compare(QString sSignature,unsigned int nOffset)
{
    return elffile->compare(sSignature.toUpper(),nOffset);
}
bool scriptELF::compareEP(QString sSignature,unsigned int nOffset)
{
    //    return elffile->compareEPfast(sSignature);
    return elffile->compareEP(sSignature,nOffset);
}

//bool scriptELF::compareEPn(QString sSignature,unsigned int nOffset)
//{
//    return elffile->compareEPn(sSignature,nOffset);
//}

QString scriptELF::getFileDirectory()
{
    return elffile->getFileDirectory();
}

QString scriptELF::getFileBaseName()
{
    return elffile->getFileBaseName();
}

QString scriptELF::getFileSuffix()
{
    return elffile->getFileSuffix();
}

QString scriptELF::getFileCompleteSuffix()
{
    return elffile->getFileCompleteSuffix();
}

unsigned int scriptELF::getSize()
{
    return elffile->size();
}

QString scriptELF::getString(unsigned int nOffset, unsigned int nSize)
{
    return elffile->getString(nOffset,nSize);
}

unsigned int scriptELF::readDword(unsigned int nOffset)
{
    return elffile->readDword(nOffset);
}

unsigned short scriptELF::readWord(unsigned int nOffset)
{
    return elffile->readWord(nOffset);
}

unsigned char scriptELF::readByte(unsigned int nOffset)
{
    return elffile->readByte(nOffset);
}
int scriptELF::findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue)
{
    return elffile->findByte(nOffset,nSize,cValue);
}

int scriptELF::findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue)
{
    return elffile->findWord(nOffset,nSize,sValue);
}

int scriptELF::findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue)
{
    return elffile->findDword(nOffset,nSize,nValue);
}

int scriptELF::findString(unsigned int nOffset, unsigned int nSize, QString sValue)
{
    return elffile->findString(nOffset,nSize,sValue);
}

int scriptELF::findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return elffile->findSignature(nOffset,nSize,sSignature);
}

bool scriptELF::isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return elffile->isSignaturePresent(nOffset,nSize,sSignature);
}

//bool scriptELF::compareReg(QString sSignature,unsigned int nOffset)
//{
//    return elffile->_compare(sSignature,nOffset);
//}

bool scriptELF::isSectionNamePresent(QString sSectionName)
{
    return elffile->isSectionNamePresent(sSectionName);
}

bool scriptELF::isStringInTablePresent(QString sSectionName, QString sString)
{
    return elffile->isStringInTablePresent(sSectionName,sString);
}

float scriptELF::calculateEntropy(unsigned int nOffset, unsigned int nSize)
{
    return elffile->calculateEntropy(nOffset,nSize);
}

QString scriptELF::getSignature(unsigned int nOffset, unsigned int nSize)
{
    return elffile->getSignature(nOffset,nSize);
}

QString scriptELF::calculateMD5(unsigned int nOffset, unsigned int nSize)
{
    return elffile->calculateMD5(nOffset,nSize);
}

unsigned int scriptELF::getSectionFileOffset(int nSection)
{
    return elffile->getSectionFileOffset(nSection);
}

unsigned int scriptELF::getSectionFileSize(int nSection)
{
    return elffile->getSectionFileSize(nSection);
}

int scriptELF::getSectionNumber(QString sSectionName)
{
    return elffile->getSectionNumber(sSectionName);
}

long long scriptELF::RVAToOffset(unsigned long long nRVA)
{
    return elffile->RVAToOffset(nRVA);
}

long long scriptELF::VAToOffset(unsigned long long nVA)
{
    return elffile->VAToOffset(nVA);
}

long long scriptELF::OffsetToVA(unsigned long long nOffset)
{
    return elffile->OffsetToVA(nOffset);
}

long long scriptELF::OffsetToRVA(unsigned long long nOffset)
{
    return elffile->OffsetToRVA(nOffset);
}

unsigned int scriptELF::getProgramFileOffset(int nProgram)
{
    return elffile->getProgramFileOffset(nProgram);
}

unsigned int scriptELF::getProgramFileSize(int nProgram)
{
    return elffile->getProgramFileSize(nProgram);
}

unsigned int scriptELF::getOverlaySize()
{
    return elffile->getOverlaySize();
}

unsigned int scriptELF::getOverlayOffset()
{
    return elffile->getOverlayOffset();
}

bool scriptELF::isOverlayPresent()
{
    return elffile->isOverlayPresent();
}

int scriptELF::getNumberOfSections()
{
    return elffile->getNumberOfSections();
}

int scriptELF::getNumberOfPrograms()
{
    return elffile->getNumberOfPrograms();
}

QString scriptELF::getSectionName(int nSection)
{
    return elffile->getSectionName(nSection);
}

bool scriptELF::compareOverlay(QString sSignature, unsigned int nOffset)
{
    return compareOverlay(sSignature,nOffset);
}

long long scriptELF::getEntryPointOffset()
{
    return elffile->getEntryPointOffset();
}
