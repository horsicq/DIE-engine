#include "scriptmsdos.h"

scriptMSDOS::scriptMSDOS(QObject *parent) :
    scriptGeneric(parent)
{
}

void scriptMSDOS::setData(MSDOSFile *msdosfile)
{
    this->msdosfile=msdosfile;

    //    emit appendError("Test2");
}

//bool scriptMSDOS::compareHeader(QString sSignature)
//{
//    return msdosfile->compareHeader(sSignature);
//}

bool scriptMSDOS::compare(QString sSignature,unsigned int nOffset)
{
    return msdosfile->compare(sSignature,nOffset);
}
bool scriptMSDOS::compareEP(QString sSignature,unsigned int nOffset)
{
    return msdosfile->compareEP(sSignature,nOffset);
}

//bool scriptMSDOS::compareEPn(QString sSignature,unsigned int nOffset)
//{
//    return msdosfile->compareEPn(sSignature,nOffset);
//}

QString scriptMSDOS::getFileDirectory()
{
    return msdosfile->getFileDirectory();
}

QString scriptMSDOS::getFileBaseName()
{
    return msdosfile->getFileBaseName();
}

QString scriptMSDOS::getFileSuffix()
{
    return msdosfile->getFileSuffix();
}

QString scriptMSDOS::getFileCompleteSuffix()
{
    return msdosfile->getFileCompleteSuffix();
}
QString scriptMSDOS::getString(unsigned int nOffset, unsigned int nSize)
{
    return msdosfile->getString(nOffset,nSize);
}

unsigned int scriptMSDOS::readDword(unsigned int nOffset)
{
    return msdosfile->readDword(nOffset);
}

unsigned short scriptMSDOS::readWord(unsigned int nOffset)
{
    return msdosfile->readWord(nOffset);
}

unsigned char scriptMSDOS::readByte(unsigned int nOffset)
{
    return msdosfile->readByte(nOffset);
}

unsigned int scriptMSDOS::getSize()
{
    return msdosfile->size();
}
int scriptMSDOS::findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue)
{
    return msdosfile->findByte(nOffset,nSize,cValue);
}

int scriptMSDOS::findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue)
{
    return msdosfile->findWord(nOffset,nSize,sValue);
}

int scriptMSDOS::findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue)
{
    return msdosfile->findDword(nOffset,nSize,nValue);
}

int scriptMSDOS::findString(unsigned int nOffset, unsigned int nSize, QString sValue)
{
    return msdosfile->findString(nOffset,nSize,sValue);
}

int scriptMSDOS::findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return msdosfile->findSignature(nOffset,nSize,sSignature);
}

bool scriptMSDOS::isNE()
{
    return msdosfile->isNE();
}

bool scriptMSDOS::isLE()
{
    return msdosfile->isLE();
}

bool scriptMSDOS::isLX()
{
    return msdosfile->isLX();
}

bool scriptMSDOS::isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return msdosfile->isSignaturePresent(nOffset,nSize,sSignature);
}

float scriptMSDOS::calculateEntropy(unsigned int nOffset, unsigned int nSize)
{
    return msdosfile->calculateEntropy(nOffset,nSize);
}

QString scriptMSDOS::getSignature(unsigned int nOffset, unsigned int nSize)
{
    return msdosfile->getSignature(nOffset,nSize);
}

QString scriptMSDOS::calculateMD5(unsigned int nOffset, unsigned int nSize)
{
    return msdosfile->calculateMD5(nOffset,nSize);
}



unsigned int scriptMSDOS::getOverlayOffset()
{
    return msdosfile->getOverlayOffset();
}

unsigned int scriptMSDOS::getOverlaySize()
{
    return msdosfile->getOverlaySize();
}

bool scriptMSDOS::isOverlayPresent()
{
    return msdosfile->isOverlayPresent();
}

bool scriptMSDOS::compareOverlay(QString sSignature, unsigned int nOffset)
{
    return msdosfile->compareOverlay(sSignature,nOffset);
}

int scriptMSDOS::getEntryPointOffset()
{
    return msdosfile->getEntryPointOffset();
}
