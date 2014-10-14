#include "scriptmach.h"

scriptMACH::scriptMACH(QObject *parent) :
    scriptGeneric(parent)
{
}

void scriptMACH::setData(MACHFile *machfile)
{
    this->machfile=machfile;

//    emit appendError("Test2");
}


//bool scriptMACH::compareHeader(QString sSignature)
//{
//    return machfile->compareHeader(sSignature);
//}

bool scriptMACH::compare(QString sSignature,unsigned int nOffset)
{
    return machfile->_compare(sSignature,nOffset);
}
bool scriptMACH::compareEP(QString sSignature,unsigned int nOffset)
{
//    return machfile->compareEPfast(sSignature);

    return machfile->compareEP(sSignature,nOffset);
}

//bool scriptMACH::compareEPn(QString sSignature,unsigned int nOffset)
//{
//    return machfile->compareEPn(sSignature,nOffset);
//}

QString scriptMACH::getFileDirectory()
{
    return machfile->getFileDirectory();
}

QString scriptMACH::getFileBaseName()
{
    return machfile->getFileBaseName();
}

QString scriptMACH::getFileSuffix()
{
    return machfile->getFileSuffix();
}

QString scriptMACH::getFileCompleteSuffix()
{
    return machfile->getFileCompleteSuffix();
}
QString scriptMACH::getString(unsigned int nOffset, unsigned int nSize)
{
    return machfile->getString(nOffset,nSize);
}

unsigned int scriptMACH::readDword(unsigned int nOffset)
{
    return machfile->readDword(nOffset);
}

unsigned short scriptMACH::readWord(unsigned int nOffset)
{
    return machfile->readWord(nOffset);
}

unsigned char scriptMACH::readByte(unsigned int nOffset)
{
    return machfile->readByte(nOffset);
}

unsigned int scriptMACH::getSize()
{
    return machfile->size();
}
int scriptMACH::findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue)
{
    return machfile->findByte(nOffset,nSize,cValue);
}

int scriptMACH::findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue)
{
    return machfile->findWord(nOffset,nSize,sValue);
}

int scriptMACH::findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue)
{
    return machfile->findDword(nOffset,nSize,nValue);
}

int scriptMACH::findString(unsigned int nOffset, unsigned int nSize, QString sValue)
{
    return machfile->findString(nOffset,nSize,sValue);
}

int scriptMACH::findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return machfile->findSignature(nOffset,nSize,sSignature);
}

bool scriptMACH::isSectionNamePresent(QString sSectionName)
{
    return machfile->isSectionNamePresent(sSectionName);
}

bool scriptMACH::isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return machfile->isSignaturePresent(nOffset,nSize,sSignature);
}

bool scriptMACH::compareReg(QString sSignature,unsigned int nOffset)
{
    return compare(sSignature,nOffset);
}

bool scriptMACH::isLibraryPresent(QString sLibrary)
{
    return machfile->isLibraryPresent(sLibrary);
}

unsigned int scriptMACH::getLibraryCurrentVersion(QString sLibrary)
{
    return machfile->getLibraryCurrentVersion(sLibrary);
}

int scriptMACH::getSectionNumber(QString sSectionName)
{
    return machfile->getSectionNumber(sSectionName);
}

QString scriptMACH::getGeneralOptions()
{
    QString sResult;

    unsigned int nFileType=machfile->getHeader_filetype();

    if(nFileType==MH_OBJECT)
    {
        sResult+="OBJ";
    }
    else if(nFileType==MH_EXECUTE)
    {
        sResult+="EXE";
    }
    else if(nFileType==MH_CORE)
    {
        sResult+="CORE";
    }
    else if(nFileType==MH_DYLIB)
    {
        sResult+="DYLIB";
    }
    else if(nFileType==MH_BUNDLE)
    {
        sResult+="BUNDLE";
    }



    if(machfile->isMACH64())
    {
        sResult+="64";
    }
    else
    {
        sResult+="32";
    }

    return sResult;
}

int scriptMACH::getNumberOfSections()
{
    return machfile->getNumberOfSections();
}

int scriptMACH::getNumberOfSegments()
{
    return machfile->getNumberOfSegments();
}

float scriptMACH::calculateEntropy(unsigned int nOffset, unsigned int nSize)
{
    return machfile->calculateEntropy(nOffset,nSize);
}

QString scriptMACH::getSignature(unsigned int nOffset, unsigned int nSize)
{
    return machfile->getSignature(nOffset,nSize);
}

QString scriptMACH::calculateMD5(unsigned int nOffset, unsigned int nSize)
{
    return machfile->calculateMD5(nOffset,nSize);
}

unsigned int scriptMACH::getSectionFileOffset(int nSection)
{
    return machfile->getSectionFileOffset(nSection);
}

unsigned int scriptMACH::getSectionFileSize(int nSection)
{
    return machfile->getSectionFileSize(nSection);
}

QString scriptMACH::getSectionName(int nSection)
{
    return machfile->getSectionName(nSection);
}

QString scriptMACH::getSegmentName(int nSegment)
{
    return machfile->getSegmentName(nSegment);
}

unsigned int scriptMACH::getSegmentFileSize(int nSegment)
{
    return machfile->getSegmentFileSize(nSegment);
}

unsigned int scriptMACH::getSegmentFileOffset(int nSegment)
{
    return machfile->getSegmentFileOffset(nSegment);
}
long long scriptMACH::RVAToOffset(unsigned long long nRVA)
{
    return machfile->RVAToOffset(nRVA);
}

long long scriptMACH::VAToOffset(unsigned long long nVA)
{
    return machfile->VAToOffset(nVA);
}

long long scriptMACH::OffsetToVA(unsigned long long nOffset)
{
    return machfile->OffsetToVA(nOffset);
}

long long scriptMACH::OffsetToRVA(unsigned long long nOffset)
{
    return machfile->OffsetToRVA(nOffset);
}

unsigned int scriptMACH::getOverlaySize()
{
    return machfile->getOverlaySize();
}

unsigned int scriptMACH::getOverlayOffset()
{
    return machfile->getOverlayOffset();
}

bool scriptMACH::isOverlayPresent()
{
    return machfile->isOverlayPresent();
}

bool scriptMACH::compareOverlay(QString sSignature, unsigned int nOffset)
{
    return machfile->compareOverlay(sSignature,nOffset);
}

long long scriptMACH::getEntryPointOffset()
{
    return machfile->getEntryPointOffset();
}
