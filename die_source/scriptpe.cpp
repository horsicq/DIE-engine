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
#include "scriptpe.h"

scriptPE::scriptPE(QObject *parent) :
    scriptGeneric(parent)
{
}

void scriptPE::setData(PEFile *pefile)
{
    this->pefile=pefile;
}

bool scriptPE::isRichSignaturePresent()
{
    return pefile->isRichSignaturePresent();
}
QString scriptPE::getCompilerVersion()
{
    QString sResult;
    int nMajor=pefile->getOptionalHeader_MajorLinkerVersion();
    int nMinor=pefile->getOptionalHeader_MinorLinkerVersion();

    sResult=QString("%1.%2").arg(nMajor).arg(nMinor);

    return sResult;
}
QString scriptPE::getGeneralOptions()
{
    QString sResult;

    bool bIs64=pefile->isPEPlus();
    bool bIsDll=pefile->isDll();
    bool bIsDriver=pefile->isDriver();

    QString sType;
    QString sBits;

    if(bIs64)
    {
        sBits="64";
    }
    else
    {
        sBits="32";
    }

    if(bIsDll)
    {
        sType="DLL";
    }
    else if(bIsDriver)
    {
        sType="Driver";
    }
    else
    {
        sType="EXE";
    }

    sResult=QString("%1%2").arg(sType).arg(sBits);

    return sResult;
}
//bool scriptPE::compareHeader(QString sSignature)
//{
//    return pefile->compareHeader(sSignature);
//}

bool scriptPE::compare(QString sSignature,unsigned int nOffset)
{
    return pefile->compare(sSignature,nOffset);
}

//bool scriptPE::compareReg(QString sSignature,unsigned int nOffset)
//{
////    sSignature=sSignature.toUpper().remove(QChar(' '));
//    sSignature=pefile->convertSignature(sSignature);

//    if((sSignature.contains("$"))||(sSignature.contains("#")))
//    {
////        qDebug(sSignature.toAscii().data());
////        unsigned int nEP=pefile->getEntryPointOffset();
////        unsigned int nOffset=nEP;
//        unsigned int nDword=0;
//        unsigned short sWord=0;
//        unsigned char cByte=0;
//        unsigned int nRVA=0;
//        QString sSeq;
//        while(true)
//        {
//            if(sSignature.left(8)=="$$$$$$$$")
//            {
//                nDword=pefile->readDword(nOffset);
//                if(nDword==-1) return false;

//                nRVA=pefile->OffsetToRVA(nOffset);
//                if(nRVA==-1) return false;

//                nRVA+=(int)nDword+4;

//                nOffset=pefile->RVAToOffset(nRVA);
//                if(nRVA==-1) return false;

//                sSignature=sSignature.mid(8);
//            }
//            else if(sSignature.left(4)=="$$$$")
//            {
//                sWord=pefile->readWord(nOffset);
//                if(sWord==-1) return false;

//                nRVA=pefile->OffsetToRVA(nOffset);
//                if(nRVA==-1) return false;

//                nRVA+=(int)sWord+2;

//                nOffset=pefile->RVAToOffset(nRVA);
//                if(nRVA==-1) return false;

//                sSignature=sSignature.mid(4);
//            }
//            else if(sSignature.left(2)=="$$")
//            {
//                cByte=pefile->readByte(nOffset);
//                if(cByte==-1) return false;

//                nRVA=pefile->OffsetToRVA(nOffset);
//                if(nRVA==-1) return false;

//                nRVA+=(int)cByte+1;

//                nOffset=pefile->RVAToOffset(nRVA);
//                if(nRVA==-1) return false;

//                sSignature=sSignature.mid(2);
//            }
//            else if(sSignature.left(8)=="########")
//            {
//                nDword=pefile->readDword(nOffset);
//                if(nDword==-1) return false;

//                nOffset=pefile->VAToOffset(nDword);
//                if(nRVA==-1) return false;

//                sSignature=sSignature.mid(8);
//            }

//            if(sSignature=="")
//            {
//                return true;
//            }


//            sSeq=sSignature.section("$",0,0);

//            if(sSeq.contains("#"))
//            {
//                sSeq=sSignature.section("#",0,0);
//            }

////            if(nOffset==nEP)
////            {
////                if(!pefile->compareEPfast(sSeq.toAscii().data()))
////                {
////                    return false;
////                }
////            }
////            else
////            {
////                if(!pefile->compare(nOffset,sSeq))
////                {
////                    return false;
////                }
////            }
//            if(!pefile->_compare(sSeq,nOffset))
//            {
//                return false;
//            }


//            nOffset+=sSeq.size()/2;
//            sSignature=sSignature.mid(sSeq.size());
//        }
//    }
//    else
//    {
//        return pefile->_compare(sSignature,nOffset);
//    }
//}
bool scriptPE::compareEP(QString sSignature,unsigned int nOffset)
{
    return pefile->compareEP(sSignature,nOffset);
    //    sSignature=pefile->convertSignature(sSignature);

    //    if((sSignature.contains("$"))||(sSignature.contains("#")))
    //    {
    //        unsigned int nEP=pefile->getEntryPointOffset();
    //        return compareReg(sSignature,nEP+nOffset);
    //    }
    //    else
    //    {
    //        if(nOffset)
    //        {
    //            return compareReg(sSignature,pefile->getEntryPointOffset()+nOffset);
    //        }
    //        else
    //        {
    //            return pefile->compareEPfast(sSignature);
    //        }

    //    }
}

//bool scriptPE::compareEPn(QString sSignature,unsigned int nOffset)
//{
//    return pefile->compareEPn(sSignature,nOffset);
//}

bool scriptPE::compareOverlay(QString sSignature,unsigned int nOffset)
{
    return pefile->compareOverlay(sSignature,nOffset);
}
bool scriptPE::isSectionNamePresent(QString sSectionName)
{
    return pefile->isSectionNamePresent(sSectionName);
}
bool scriptPE::isSectionNamePresentExp(QString sSectionName)
{
    return pefile->isSectionNamePresentExp(sSectionName);
}

bool scriptPE::isLibraryPresent(QString sLibraryName)
{
    return pefile->isLibraryPresent(sLibraryName);
}

bool scriptPE::isLibraryFunctionPresent(QString sLibraryName, QString sFunctionName)
{
    return pefile->isLibraryFunctionPresent(sLibraryName,sFunctionName);
}

QString scriptPE::getImportLibraryName(unsigned int nImport)
{
    return pefile->getImportLibraryName(nImport);
}

unsigned int scriptPE::getMajorLinkerVersion()
{
    return pefile->getOptionalHeader_MajorLinkerVersion();
}

unsigned int scriptPE::getMinorLinkerVersion()
{
    return pefile->getOptionalHeader_MinorLinkerVersion();
}

QString scriptPE::getImportFunctionName(unsigned int nImport, unsigned int nFunctionNumber)
{
    //#ifndef QT_NO_DEBUG
    //    QTime scanTime=QTime::currentTime();
    //#endif

    QString sResult=pefile->getImportFunctionName(nImport,nFunctionNumber);

    //#ifndef QT_NO_DEBUG
    //   qDebug("getImportFunctionName: %d msec",scanTime.msecsTo(QTime::currentTime()));
    //#endif

    return sResult;
}
unsigned int scriptPE::getNumberOfImportThunks(unsigned int nImport)
{
    //#ifndef QT_NO_DEBUG
    //    QTime scanTime=QTime::currentTime();
    //#endif

    //#ifndef QT_NO_DEBUG
    //   qDebug("getNumberOfImportThunks: %d msec",scanTime.msecsTo(QTime::currentTime()));
    //#endif

    unsigned nResult=pefile->getNumberOfImportThunks(nImport);

    return nResult;
}
unsigned int scriptPE::getSizeOfCode()
{
    return pefile->getOptionalHeader_SizeOfCode();
}

unsigned int scriptPE::getSizeOfUninitializedData()
{
    return pefile->getOptionalHeader_SizeOfUninitializedData();
}

int scriptPE::getNumberOfSections()
{
    return pefile->getFileHeader_NumberOfSections();
}

unsigned int scriptPE::getImageBase()
{
    return pefile->getOptionalHeader_ImageBase();
}

bool scriptPE::isDll()
{
    return pefile->isDll();
}

bool scriptPE::isDriver()
{
    return pefile->isDriver();
}

unsigned int scriptPE::getSectionFileSize(int nSectionNumber)
{
    return pefile->getSection_SizeOfRawData(nSectionNumber);
}

unsigned int scriptPE::getSectionFileOffset(int nSectionNumber)
{
    return pefile->getSection_PointerToRawData(nSectionNumber);
}

QString scriptPE::getSectionName(int nSectionNumber)
{
    return pefile->getSectionNameAsString(nSectionNumber);
}

unsigned int scriptPE::getSectionCharacteristics(int nSectionNumber)
{
    return pefile->getSection_Characteristics(nSectionNumber);
}

unsigned int scriptPE::getSectionVirtualSize(int nSectionNumber)
{
    return pefile->getSection_VirtualSize(nSectionNumber);
}

unsigned int scriptPE::getSectionVirtualAddress(int nSectionNumber)
{
    return pefile->getSection_VirtualAddress(nSectionNumber);
}

QString scriptPE::getString(unsigned int nOffset,unsigned int nSize)
{
    return pefile->getString(nOffset,nSize);
}

int scriptPE::getNumberOfImports()
{
    return pefile->getNumberOfImports();
}

int scriptPE::getResourceNameOffset(QString sName)
{
    return pefile->getResourceNameOffsetFast(sName);
}

unsigned int scriptPE::getResourceNameSize(QString sName)
{
    return pefile->getResourceNameSizeFast(sName);
}

bool scriptPE::isConsole()
{
    return pefile->getOptionalHeader_Subsystem()==IMAGE_SUBSYSTEM_WINDOWS_CUI;
}

int scriptPE::findSignature(unsigned int nOffset,unsigned int nSize,QString sSignature)
{
    return pefile->findSignature(nOffset,nSize,sSignature);
}

bool scriptPE::isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return pefile->isSignaturePresent(nOffset,nSize,sSignature);
}

bool scriptPE::isSignatureInSectionPresent(unsigned int nSection, QString sSignature)
{
    if(pefile->getFileHeader_NumberOfSections()>=1)
    {
        unsigned int nOffset=pefile->getSection_PointerToRawData(nSection);
        unsigned int nSize=pefile->getSection_SizeOfRawData(nSection);

        return isSignaturePresent(nOffset,nSize,sSignature);
    }

    return false;
}

unsigned short scriptPE::getMachineType()
{
    return pefile->getFileHeader_Machine();
}

unsigned int scriptPE::getAddressOfEntryPoint()
{
    return pefile->getOptionalHeader_AddressOfEntryPoint();
}

int scriptPE::getEntryPointSection()
{
    return pefile->getEntryPointSection();
}

int scriptPE::getImportSection()
{
    return pefile->getImportSection();
}

int scriptPE::getRelocsSection()
{
    return pefile->getRelocsSection();
}

int scriptPE::getResourceSection()
{
    return pefile->getResourceSection();
}

int scriptPE::getExportSection()
{
    return pefile->getExportSection();
}

int scriptPE::getTLSSection()
{
    return pefile->getTLSSection();
}

int scriptPE::getSectionNumber(QString sSectionName)
{
    return pefile->getSectionNumber(sSectionName);
}
int scriptPE::getSectionNumberExp(QString sSectionName)
{
    return pefile->getSectionNumberExp(sSectionName);
}
QString scriptPE::getSectionNameCollision(QString sString1,QString sString2)
{
    return pefile->getSectionNameCollision(sString1,sString2);
}

bool scriptPE::isOverlayPresent()
{
    return pefile->isOverlayPresent();
}

unsigned int scriptPE::calculateSizeOfHeaders()
{
    return pefile->calculateSizeOfHeaders();
}

QString scriptPE::getFileVersion()
{
    return pefile->getFileVersion();
}

QString scriptPE::getVersionStringInfo(QString sKey)
{
    return pefile->getVersionStringInfo(sKey);
}

QString scriptPE::getManifest()
{
    return pefile->getManifest();
}

float scriptPE::calculateEntropy(unsigned int nOffset, unsigned int nSize)
{
    return pefile->calculateEntropy(nOffset,nSize);
}

QString scriptPE::getSignature(unsigned int nOffset, unsigned int nSize)
{
    return pefile->getSignature(nOffset,nSize);
}

QString scriptPE::calculateMD5(unsigned int nOffset, unsigned int nSize)
{
    return pefile->calculateMD5(nOffset,nSize);
}

bool scriptPE::compareHeader(QString sSignature)
{
    return pefile->compare(sSignature,0);
}

bool scriptPE::isDosStubPresent()
{
    return pefile->isDosStubPresent();
}

unsigned int scriptPE::getDosStubOffset()
{
    return pefile->getDosStubOffset();
}

unsigned int scriptPE::getDosStubSize()
{
    return pefile->getDosStubSize();
}

unsigned int scriptPE::getNumberOfResources()
{
    return pefile->getNumberOfResources();
}

int scriptPE::getResourceOffsetByNumber(unsigned int nNumber)
{
    return pefile->getResourceOffsetByNumber(nNumber);
}

unsigned int scriptPE::getResourceSizeByNumber(unsigned int nNumber)
{
    return pefile->getResourceSizeByNumber(nNumber);
}

unsigned int scriptPE::getResourceIdByNumber(unsigned int nNumber)
{
    return pefile->getResourceIdByNumber(nNumber);
}

QString scriptPE::getResourceNameByNumber(unsigned int nNumber)
{
    return pefile->getResourceNameByNumber(nNumber);
}

QString scriptPE::getResourceTypeByNumber(unsigned int nNumber)
{
    return pefile->getResourceTypeByNumber(nNumber);
}

unsigned int scriptPE::getNumberOfRichIDs()
{
    return pefile->getNumberOfRichIDs();
}

bool scriptPE::isRichVersionPresent(unsigned int nVersion)
{
    return pefile->isRichVersionPresent(nVersion);
}

bool scriptPE::isSignedFile()
{
    return pefile->isSignedFile();
}

bool scriptPE::isExportPresent()
{
    return pefile->isExportPresent();
}

bool scriptPE::isTLSPresent()
{
    return pefile->isTLSPresent();
}

bool scriptPE::isImportPresent()
{
    return pefile->isImportPresent();
}

bool scriptPE::isResourcePresent()
{
    return pefile->isResourcePresent();
}

bool scriptPE::isExportFunctionPresent(QString sFunctionName)
{
    return pefile->isExportFunctionPresent(sFunctionName);
}

bool scriptPE::isExportFunctionPresentExp(QString sFunctionName)
{
    return pefile->isExportFunctionPresentExp(sFunctionName);
}

QString scriptPE::_getImportHash()
{
    QString sResult;

    QString sString;

    QList<QString> list=pefile->getImportFunctionsAsNames();

    for(int i=0;i<list.count();i++)
    {
        sString+=list.at(i);
    }

    sResult=QString(QCryptographicHash::hash(QByteArray(sString.toLatin1().data(),sString.size()),QCryptographicHash::Md5).toHex());

    return sResult;
}

QString scriptPE::getPEFileVersion(QString sFileName)
{
    QString sResult;
    PEFile __pe;

    if(__pe.setFileName(sFileName))
    {
        if(__pe.isValid())
        {
            sResult=__pe.getFileVersion();
        }
    }

    return sResult;
}

QString scriptPE::getFileDirectory()
{
    return pefile->getFileDirectory();
}

QString scriptPE::getFileBaseName()
{
    return pefile->getFileBaseName();
}

QString scriptPE::getFileSuffix()
{
    return pefile->getFileSuffix();
}

QString scriptPE::getFileCompleteSuffix()
{
    return pefile->getFileCompleteSuffix();
}

bool scriptPE::isNET()
{
    return pefile->isNETPresent();
}

QString scriptPE::getNETVersion()
{
    return pefile->getCLI_MetaData_Version();
}

bool scriptPE::compareEP_NET(QString sSignature,unsigned int nOffset)
{
    return pefile->compareEP_NET(sSignature,nOffset);
}

bool scriptPE::isNETStringPresent(QString sString)
{
    return pefile->isCLI_MetaData_StringPresent(sString);
}

bool scriptPE::isNETUnicodeStringPresent(QString sString)
{
    return pefile->isCLI_MetaData_UnicodeStringPresent(sString);
}

unsigned int scriptPE::getSize()
{
    return pefile->size();
}

unsigned int scriptPE::getOverlayOffset()
{
    return pefile->getOverlayOffset();
}

unsigned int scriptPE::getOverlaySize()
{
    return pefile->getOverlaySize();
}

bool scriptPE::isResourceNamePresent(QString sName)
{
    return pefile->isResourceNamePresentFast(sName);
}

unsigned int scriptPE::readDword(unsigned int nOffset)
{
    return pefile->readDword(nOffset);
}

unsigned short scriptPE::readWord(unsigned int nOffset)
{
    return pefile->readWord(nOffset);
}

unsigned char scriptPE::readByte(unsigned int nOffset)
{
    return pefile->readByte(nOffset);
}

long long scriptPE::RVAToOffset(unsigned long long nRVA)
{
    return pefile->RVAToOffset(nRVA);
}

long long scriptPE::VAToOffset(unsigned long long nVA)
{
    return pefile->VAToOffset(nVA);
}

long long scriptPE::OffsetToVA(unsigned long long nOffset)
{
    return pefile->OffsetToVA(nOffset);
}

long long scriptPE::OffsetToRVA(unsigned long long nOffset)
{
    return pefile->OffsetToRVA(nOffset);
}

long long scriptPE::getEntryPointOffset()
{
    return pefile->getEntryPointOffset();
}

int scriptPE::findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue)
{
    return pefile->findByte(nOffset,nSize,cValue);
}

int scriptPE::findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue)
{
    return pefile->findWord(nOffset,nSize,sValue);
}

int scriptPE::findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue)
{
    return pefile->findDword(nOffset,nSize,nValue);
}

int scriptPE::findString(unsigned int nOffset, unsigned int nSize, QString sValue)
{
    return pefile->findString(nOffset,nSize,sValue);
}

bool scriptPE::isPEPlus()
{
    return pefile->isPEPlus();
}
