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
#ifndef SCRIPTPE_H
#define SCRIPTPE_H

#include "scriptgeneric.h"
#include "scriptbinary.h"
#include "../pefile.h"

class scriptPE : public scriptGeneric
{
    Q_OBJECT
public:
    explicit scriptPE(QObject *parent=nullptr);
    void setData(PEFile *pefile);

public slots:
    unsigned short getMachineType();

    bool isRichSignaturePresent();
    QString getCompilerVersion();
    QString getGeneralOptions();

    bool compareEP(QString sSignature,unsigned int nOffset=0);
    //    bool compareEPn(QString sSignature,unsigned int nOffset);
    bool compareOverlay(QString sSignature,unsigned int nOffset=0);
    bool isSectionNamePresent(QString sSectionName);
    bool isSectionNamePresentExp(QString sSectionName);
    bool isLibraryPresent(QString sLibraryName);
    bool isLibraryFunctionPresent(QString sLibraryName, QString sFunctionName);
    QString getImportLibraryName(unsigned int nImport);
    QString getImportFunctionName(unsigned int nImport, unsigned int nFunctionNumber);
    unsigned int getNumberOfImportThunks(unsigned int nImport);
    unsigned int getSizeOfCode();
    unsigned int getSizeOfUninitializedData();
    int getNumberOfSections();
    unsigned int getImageBase();
    bool isDll();
    bool isDriver();
    unsigned int getSectionFileSize(int nSectionNumber);
    unsigned int getSectionFileOffset(int nSectionNumber);
    QString getSectionName(int nSectionNumber);
    unsigned int getSectionCharacteristics(int nSectionNumber);
    unsigned int getSectionVirtualSize(int nSectionNumber);
    unsigned int getSectionVirtualAddress(int nSectionNumber);

    unsigned int getAddressOfEntryPoint();
    unsigned int getMajorLinkerVersion();
    unsigned int getMinorLinkerVersion();

    int getEntryPointSection();
    int getImportSection();
    int getRelocsSection();
    int getResourceSection();
    int getExportSection();
    int getTLSSection();

    int getSectionNumber(QString sSectionName);
    int getSectionNumberExp(QString sSectionName);
    QString getSectionNameCollision(QString sString1, QString sString2);
    bool isOverlayPresent();
    unsigned int calculateSizeOfHeaders();
    QString getFileVersion();
    QString getPEFileVersion(QString sFileName);
    bool isPEPlus();

    bool isNET();
    QString getNETVersion();
    bool compareEP_NET(QString sSignature,unsigned int nOffset=0);

    bool isNETStringPresent(QString sString);
    bool isNETUnicodeStringPresent(QString sString);

    unsigned int getOverlayOffset();
    unsigned int getOverlaySize();
    bool isResourceNamePresent(QString sName);

    long long RVAToOffset(unsigned long long nRVA);
    long long VAToOffset(unsigned long long nVA);
    long long OffsetToVA(unsigned long long nOffset);
    long long OffsetToRVA(unsigned long long nOffset);

    long long getEntryPointOffset();

    //    bool compareHeader(QString sSignature);
    bool compare(QString sSignature,unsigned int nOffset=0);
    //    bool compareReg(QString sSignature,unsigned int nOffset);
    QString getFileDirectory();
    QString getFileBaseName();
    QString getFileSuffix();
    QString getFileCompleteSuffix();
    unsigned int getSize();
    unsigned int readDword(unsigned int nOffset);
    unsigned short readWord(unsigned int nOffset);
    unsigned char readByte(unsigned int nOffset);
    int findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue);
    int findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue);
    int findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue);
    int findString(unsigned int nOffset, unsigned int nSize, QString sValue);
    int findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature);
    QString getString(unsigned int nOffset,unsigned int nSize=50);

    int getNumberOfImports();

    int getResourceNameOffset(QString sName);
    unsigned int getResourceNameSize(QString sName);
    bool isConsole();

    bool isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature);
    bool isSignatureInSectionPresent(unsigned int nSection,QString sSignature);

    QString getVersionStringInfo(QString sKey);
    QString getManifest();

    float calculateEntropy(unsigned int nOffset, unsigned int nSize);
    QString getSignature(unsigned int nOffset,unsigned int nSize);
    QString calculateMD5(unsigned int nOffset, unsigned int nSize);

    bool compareHeader(QString sSignature);

    bool isDosStubPresent();
    unsigned int getDosStubOffset();
    unsigned int getDosStubSize();

    unsigned int getNumberOfResources();
    int getResourceOffsetByNumber(unsigned int nNumber);
    unsigned int getResourceSizeByNumber(unsigned int nNumber);
    unsigned int getResourceIdByNumber(unsigned int nNumber);
    QString getResourceNameByNumber(unsigned int nNumber);
    QString getResourceTypeByNumber(unsigned int nNumber);

    unsigned int getNumberOfRichIDs();
    bool isRichVersionPresent(unsigned int nID);
    bool isSignedFile();

    bool isExportPresent();
    bool isTLSPresent();
    bool isImportPresent();
    bool isResourcePresent();
    bool isExportFunctionPresent(QString sFunctionName);
    bool isExportFunctionPresentExp(QString sFunctionName);

    QString _getImportHash();

private:
    PEFile *pefile;
    //    int nEPOffset;
    //    int nFileSize;
};

#endif // SCRIPTPE_H
