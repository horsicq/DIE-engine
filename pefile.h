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
#ifndef PEFILE_H
#define PEFILE_H

#include "binary.h"
#include "versionresource.h"

class PEFile : public Binary
{
public:
    struct RICH_RECORD
    {
        unsigned int nId;
        unsigned int nVersion;
        unsigned int nCount;
    };
    struct IMPORT_LIST
    {
        QString sName;
        unsigned long long nThunkOffset;
        unsigned long long nThunkRelAddress;
        unsigned long long nThunkAddress;
    };

private:
    struct _____SECTIONRECORD
    {
        unsigned int nOldOffset;
        unsigned int nNewOffset;
        unsigned int nOldSize;
        unsigned int nNewSize;
    };

#define CLI_METADATA_Module                 0x0000000000000001 //0
#define CLI_METADATA_TypeRef                0x0000000000000002 //1
#define CLI_METADATA_TypeDef                0x0000000000000004 //2
#define CLI_METADATA_unknown3               0x0000000000000008 //3
#define CLI_METADATA_Field                  0x0000000000000010 //4
#define CLI_METADATA_unknown5               0x0000000000000020 //5
#define CLI_METADATA_MethodDef              0x0000000000000040 //6
#define CLI_METADATA_unknown7               0x0000000000000080 //7
#define CLI_METADATA_Param                  0x0000000000000100 //8
#define CLI_METADATA_InterfaceImpl          0x0000000000000200 //9
#define CLI_METADATA_MemberRef              0x0000000000000400 //10
#define CLI_METADATA_Constant               0x0000000000000800 //11
#define CLI_METADATA_CustomAttribute        0x0000000000001000 //12
#define CLI_METADATA_FieldMarshal           0x0000000000002000 //13
#define CLI_METADATA_DeclSecurity           0x0000000000004000 //14
#define CLI_METADATA_ClassLayout            0x0000000000008000 //15
#define CLI_METADATA_FieldLayout            0x0000000000010000 //16
#define CLI_METADATA_StandAloneSig          0x0000000000020000 //17
#define CLI_METADATA_EventMap               0x0000000000040000 //18
#define CLI_METADATA_unknown19              0x0000000000080000 //19
#define CLI_METADATA_Event                  0x0000000000100000 //20
#define CLI_METADATA_PropertyMap            0x0000000000200000 //21
#define CLI_METADATA_unknown22              0x0000000000400000 //22
#define CLI_METADATA_Property               0x0000000000800000 //23
#define CLI_METADATA_MethodSemantics        0x0000000001000000 //24
#define CLI_METADATA_MethodImpl             0x0000000002000000 //25
#define CLI_METADATA_ModuleRef              0x0000000004000000 //26
#define CLI_METADATA_TypeSpec               0x0000000008000000 //27
#define CLI_METADATA_ImplMap                0x0000000010000000 //28
#define CLI_METADATA_FieldRVA               0x0000000020000000 //29
#define CLI_METADATA_unknown30              0x0000000040000000 //30
#define CLI_METADATA_unknown31              0x0000000080000000 //31
#define CLI_METADATA_Assembly               0x0000000100000000 //32
#define CLI_METADATA_AssemblyProcessor      0x0000000200000000 //33
#define CLI_METADATA_AssemblyOS             0x0000000400000000 //34
#define CLI_METADATA_AssemblyRef            0x0000000800000000 //35
#define CLI_METADATA_AssemblyRefProcessor   0x0000001000000000 //36
#define CLI_METADATA_AssemblyRefOS          0x0000002000000000 //37
#define CLI_METADATA_File                   0x0000004000000000 //38
#define CLI_METADATA_ExportedType           0x0000008000000000 //39
#define CLI_METADATA_ManifestResource       0x0000010000000000 //40
#define CLI_METADATA_NestedClass            0x0000020000000000 //41
#define CLI_METADATA_GenericParam           0x0000040000000000 //42
#define CLI_METADATA_unknown43              0x0000080000000000 //43
#define CLI_METADATA_GenericParamConstraint 0x0000100000000000 //44
#define CLI_METADATA_unknown45              0x0000200000000000 //45
#define CLI_METADATA_unknown46              0x0000400000000000 //46
#define CLI_METADATA_unknown47              0x0000800000000000 //47
#define CLI_METADATA_unknown48              0x0001000000000000 //48
#define CLI_METADATA_unknown49              0x0002000000000000 //49
#define CLI_METADATA_unknown50              0x0004000000000000 //50
#define CLI_METADATA_unknown51              0x0008000000000000 //51
#define CLI_METADATA_unknown52              0x0000000000000000 //52
#define CLI_METADATA_unknown53              0x0000000000000000 //53
#define CLI_METADATA_unknown54              0x0000000000000000 //54
#define CLI_METADATA_unknown55              0x0000000000000000 //55
#define CLI_METADATA_unknown56              0x0000000000000000 //56
#define CLI_METADATA_unknown57              0x0000000000000000 //57
#define CLI_METADATA_unknown58              0x0000000000000000 //58
#define CLI_METADATA_unknown59              0x0000000000000000 //59
#define CLI_METADATA_unknown60              0x0000000000000000 //60
#define CLI_METADATA_unknown61              0x0000000000000000 //61
#define CLI_METADATA_unknown62              0x0000000000000000 //62
#define CLI_METADATA_unknown63              0x0000000000000000 //63


    Q_OBJECT
public:
    explicit PEFile(QObject *parent=nullptr);
    ~PEFile();

    QByteArray createHeader32();

    bool addSection(IMAGE_SECTION_HEADER *pISH,QByteArray baData);
    bool addSection(IMAGE_SECTION_HEADER *pISH,char *pData,int nDataSize);
    bool addSection(IMAGE_SECTION_HEADER *pISH,QString sFileName);

    bool rebuild();
    unsigned int getSectionRealSize(unsigned int nOffset,unsigned int nSize,unsigned int nAlignment);

    bool addDosStub(QByteArray &baData);
    bool addDosStub(char *pData,int nDataSize);

    int CalculateHeadersSize(int nDosStubSize, int nSizeOfOptionalHeader, int nNumberOfSection, int nFileAlignment);
    void FixRawOffsets(int nDelta);
    void fixSizeOfHeaders();

    unsigned short CheckSum(int nStartValue, int nDataSize);

    bool addOverlay(QByteArray &baData);
    bool addOverlay(char *pData,int nDataSize);

    unsigned int getNewSection_VirtualAddress();
    unsigned int getNewSection_PointerToRawData();

    bool completeCheck();

    QByteArray getHeaders(); // delete
    QByteArray getEntryPointCode(); // delete

    bool compareEP(QString sSignature,unsigned int nOffset=0);
    bool compareOverlay(QString sSignature,unsigned int nOffset=0);

    void entryPointLoad();

    QByteArray getResourceAsXML();
    void resourceToXML(unsigned int nResourceOffset, unsigned int nResourceDirectory, QXmlStreamWriter *pXml, int nLevel);

    QByteArray getImportAsXML();
    bool saveImportAsXMLFile(QString sFileName);

    QByteArray ImportAsXMLToBin(QByteArray baXML, IMAGE_DATA_DIRECTORY *pddImportTable, IMAGE_DATA_DIRECTORY *pddIAT);

    bool addImportSection(IMAGE_SECTION_HEADER *pISH,QByteArray baData,IMAGE_DATA_DIRECTORY *pddImportTable,IMAGE_DATA_DIRECTORY *pddIAT);
    bool addImportSection(IMAGE_SECTION_HEADER *pISH, char *pData, int nDataSize, IMAGE_DATA_DIRECTORY *pddImportTable, IMAGE_DATA_DIRECTORY *pddIAT);
    void FixImportBin(unsigned int nOffset, unsigned int nImportTableOffset, int nDelta);
    bool addImportSectionFromXML(IMAGE_SECTION_HEADER *pISH, QByteArray baXML);
    //    bool addImportSectionFromMap(IMAGE_SECTION_HEADER *pISH, QMap<unsigned long long,QString> &mapIAT);
private:
    bool bIsNETEntryPontLoad;
private slots:
    bool compareEP_NETfast(char *pszSignature);
    //    bool compareEPfast(QString sSignature);
    //    bool compareEP(char *pszSignature,unsigned int nOffset=0);
    //    bool compareOverlay(char *pszSignature,unsigned int nOffset=0);
public slots:

    bool compareEP_NETfast(QString sSignature);
    bool compareEPfast(QString sSignature);

    bool compareEP_NET(QString sSignature,unsigned int nOffset=0);

    bool isValid();
    bool isPEPlus();

    unsigned int getDosHeaderOffset();
    unsigned int getDosHeaderSize();

    unsigned short getDosHeader_magic();
    unsigned short getDosHeader_cblp();
    unsigned short getDosHeader_cp();
    unsigned short getDosHeader_crlc();
    unsigned short getDosHeader_cparhdr();
    unsigned short getDosHeader_minalloc();
    unsigned short getDosHeader_maxalloc();
    unsigned short getDosHeader_ss();
    unsigned short getDosHeader_sp();
    unsigned short getDosHeader_csum();
    unsigned short getDosHeader_ip();
    unsigned short getDosHeader_cs();
    unsigned short getDosHeader_lfarlc();
    unsigned short getDosHeader_ovno();
    unsigned short getDosHeader_res(int nIndex);
    unsigned short getDosHeader_oemid();
    unsigned short getDosHeader_oeminfo();
    unsigned short getDosHeader_res2(int nIndex);

    void setDosHeader_magic(unsigned short sValue);
    void setDosHeader_cblp(unsigned short sValue);
    void setDosHeader_cp(unsigned short sValue);
    void setDosHeader_crlc(unsigned short sValue);
    void setDosHeader_cparhdr(unsigned short sValue);
    void setDosHeader_minalloc(unsigned short sValue);
    void setDosHeader_maxalloc(unsigned short sValue);
    void setDosHeader_ss(unsigned short sValue);
    void setDosHeader_sp(unsigned short sValue);
    void setDosHeader_csum(unsigned short sValue);
    void setDosHeader_ip(unsigned short sValue);
    void setDosHeader_cs(unsigned short sValue);
    void setDosHeader_lfarlc(unsigned short sValue);
    void setDosHeader_ovno(unsigned short sValue);
    void setDosHeader_res(short sValue,int nIndex);
    void setDosHeader_oemid(unsigned short sValue);
    void setDosHeader_oeminfo(unsigned short sValue);
    void setDosHeader_res2(short sValue,int nIndex);

    unsigned int getDosHeader_lfanew();
    void setDosHeader_lfanew(unsigned int nValue);

    unsigned int getNtHeadersOffset();
    unsigned int getNtHeadersSize();

    unsigned int getNTHeaders_Signature();
    void setNTHeaders_Signature(unsigned int nValue);

    unsigned short getFileHeader_Machine();
    unsigned short getFileHeader_NumberOfSections();
    unsigned int getFileHeader_TimeDateStamp();
    unsigned int getFileHeader_PointerToSymbolTable();
    unsigned int getFileHeader_NumberOfSymbols();
    unsigned short getFileHeader_SizeOfOptionalHeader();
    unsigned short getFileHeader_Characteristics();

    void setFileHeader_Machine(unsigned short sValue);
    void setFileHeader_NumberOfSections(unsigned short sValue);
    void setFileHeader_TimeDateStamp(unsigned int nValue);
    void setFileHeader_PointerToSymbolTable(unsigned int nValue);
    void setFileHeader_NumberOfSymbols(unsigned int nValue);
    void setFileHeader_SizeOfOptionalHeader(unsigned short sValue);
    void setFileHeader_Characteristics(unsigned short sValue);

    unsigned short getOptionalHeader_Magic();
    unsigned char getOptionalHeader_MajorLinkerVersion();
    unsigned char getOptionalHeader_MinorLinkerVersion();
    unsigned int getOptionalHeader_SizeOfCode();
    unsigned int getOptionalHeader_SizeOfInitializedData();
    unsigned int getOptionalHeader_SizeOfUninitializedData();
    unsigned int getOptionalHeader_AddressOfEntryPoint();
    unsigned int getOptionalHeader_BaseOfCode();
    unsigned int getOptionalHeader_BaseOfData();

    void setOptionalHeader_Magic(unsigned short sValue);
    void setOptionalHeader_MajorLinkerVersion(unsigned char cValue);
    void setOptionalHeader_MinorLinkerVersion(unsigned char cValue);
    void setOptionalHeader_SizeOfCode(unsigned int nValue);
    void setOptionalHeader_SizeOfInitializedData(unsigned int nValue);
    void setOptionalHeader_SizeOfUninitializedData(unsigned int nValue);
    void setOptionalHeader_AddressOfEntryPoint(unsigned int nValue);
    void setOptionalHeader_BaseOfCode(unsigned int nValue);
    void setOptionalHeader_BaseOfData(unsigned int nValue);

    unsigned int getOptionalHeader_ImageBase();
    unsigned long long getOptionalHeader_ImageBase64();

    unsigned int getOptionalHeader_SectionAlignment();
    unsigned int getOptionalHeader_FileAlignment();
    unsigned short getOptionalHeader_MajorOperatingSystemVersion();
    unsigned short getOptionalHeader_MinorOperatingSystemVersion();
    unsigned short getOptionalHeader_MajorImageVersion();
    unsigned short getOptionalHeader_MinorImageVersion();
    unsigned short getOptionalHeader_MajorSubsystemVersion();
    unsigned short getOptionalHeader_MinorSubsystemVersion();
    unsigned int getOptionalHeader_Win32VersionValue();
    unsigned int getOptionalHeader_SizeOfImage();
    unsigned int getOptionalHeader_SizeOfHeaders();
    unsigned int getOptionalHeader_CheckSum();
    unsigned short getOptionalHeader_Subsystem();
    unsigned short getOptionalHeader_DllCharacteristics();

    unsigned int getOptionalHeader_SizeOfStackReserve();
    unsigned int getOptionalHeader_SizeOfStackCommit();
    unsigned int getOptionalHeader_SizeOfHeapReserve();
    unsigned int getOptionalHeader_SizeOfHeapCommit();
    unsigned long long getOptionalHeader_SizeOfStackReserve64();
    unsigned long long getOptionalHeader_SizeOfStackCommit64();
    unsigned long long getOptionalHeader_SizeOfHeapReserve64();
    unsigned long long getOptionalHeader_SizeOfHeapCommit64();

    unsigned int getOptionalHeader_LoaderFlags();
    unsigned int getOptionalHeader_NumberOfRvaAndSizes();

    void setOptionalHeader_ImageBase(unsigned int nValue);
    void setOptionalHeader_ImageBase64(unsigned long long nValue);

    void setOptionalHeader_SectionAlignment(unsigned int nValue);
    void setOptionalHeader_FileAlignment(unsigned int nValue);
    void setOptionalHeader_MajorOperatingSystemVersion(unsigned short sValue);
    void setOptionalHeader_MinorOperatingSystemVersion(unsigned short sValue);
    void setOptionalHeader_MajorImageVersion(unsigned short sValue);
    void setOptionalHeader_MinorImageVersion(unsigned short sValue);
    void setOptionalHeader_MajorSubsystemVersion(unsigned short sValue);
    void setOptionalHeader_MinorSubsystemVersion(unsigned short sValue);
    void setOptionalHeader_Win32VersionValue(unsigned int nValue);
    void setOptionalHeader_SizeOfImage(unsigned int nValue);
    void setOptionalHeader_SizeOfHeaders(unsigned int nValue);
    void setOptionalHeader_CheckSum(unsigned int nValue);
    void setOptionalHeader_Subsystem(unsigned short sValue);
    void setOptionalHeader_DllCharacteristics(unsigned short sValue);
    void setOptionalHeader_SizeOfStackReserve(unsigned int nValue);
    void setOptionalHeader_SizeOfStackCommit(unsigned int nValue);
    void setOptionalHeader_SizeOfHeapReserve(unsigned int nValue);
    void setOptionalHeader_SizeOfHeapCommit(unsigned int nValue);

    void setOptionalHeader_SizeOfStackReserve64(unsigned long long nValue);
    void setOptionalHeader_SizeOfStackCommit64(unsigned long long nValue);
    void setOptionalHeader_SizeOfHeapReserve64(unsigned long long nValue);
    void setOptionalHeader_SizeOfHeapCommit64(unsigned long long nValue);

    void setOptionalHeader_LoaderFlags(unsigned int nValue);
    void setOptionalHeader_NumberOfRvaAndSizes(unsigned int nValue);

    void getOptionalHeader_DataDirectory(int nDirectory,IMAGE_DATA_DIRECTORY *pDataDirectory);
    void setOptionalHeader_DataDirectory(int nDirectory,IMAGE_DATA_DIRECTORY *pDataDirectory);

    bool isDirectoryPresent(int nDirectory);
    bool isRVAPresentInFile(unsigned int nRVA);
    bool isOffsetValid(unsigned int nOffset);
    bool isRVAValid(unsigned int nRVA);
    bool isVAValid(unsigned int nVA);

    int getNumberOfSections();
    bool isSectionPresent(int nSection);
    unsigned int getSectionHeaderOffset(int nSection);
    unsigned int getSectionsTableOffset();
    unsigned int getSectionsTableSize();

    // char *getSection_Name(int nSection);
    unsigned int getSection_VirtualSize(int nSection);
    unsigned int getSection_VirtualAddress(int nSection);
    unsigned int getSection_SizeOfRawData(int nSection);
    unsigned int getSection_PointerToRawData(int nSection);
    unsigned int getSection_PointerToRelocations(int nSection);
    unsigned int getSection_PointerToLinenumbers(int nSection);
    unsigned short getSection_NumberOfRelocations(int nSection);
    unsigned short getSection_NumberOfLinenumbers(int nSection);
    unsigned int getSection_Characteristics(int nSection);

    //void setSection_Name(int nSection,char *pcValue);
    void setSection_VirtualSize(int nSection,unsigned int nValue);
    void setSection_VirtualAddress(int nSection,unsigned int nValue);
    void setSection_SizeOfRawData(int nSection,unsigned int nValue);
    void setSection_PointerToRawData(int nSection,unsigned int nValue);
    void setSection_PointerToRelocations(int nSection,unsigned int nValue);
    void setSection_PointerToLinenumbers(int nSection,unsigned int nValue);
    void setSection_NumberOfRelocations(int nSection,unsigned short sValue);
    void setSection_NumberOfLinenumbers(int nSection,unsigned short sValue);
    void setSection_Characteristics(int nSection,unsigned int nValue);

    QString getSectionNameAsString(int nSection);
    void setSectionNameAsString(int nSection,QString sValue);

    QList<IMAGE_SECTION_HEADER> getSections();

    virtual unsigned long long OffsetToRVA(unsigned long long nOffset);
    unsigned int OffsetToVA32(unsigned int nOffset);
    unsigned long long OffsetToVA64(unsigned int nOffset);
    virtual unsigned long long RVAToOffset(unsigned long long nRVA);
    unsigned int RVAToSection(unsigned int nRVA);
    unsigned int getEntryPointSection();
    unsigned int calculateRawSize();
    unsigned int VAToOffset32(unsigned int nVA);
    unsigned int VAToOffset64(unsigned long long nVA);
    virtual unsigned long long VAToOffset(unsigned long long nVA);
    virtual unsigned long long OffsetToVA(unsigned long long nOffset);

    unsigned long long VAToRVA(unsigned long long nVA);
    unsigned long long RVAToVA(unsigned long long nRVA);

    unsigned long long getMinimalVA();
    unsigned long long getMaximalVA();

    unsigned int getDataDirectoriesOffset();
    unsigned int getDataDirectoriesSize();

    unsigned int getDosStubSize();
    unsigned int getDosStubOffset();
    bool isDosStubPresent();
    bool isRichSignaturePresent();

    QList<RICH_RECORD> getRichSignatureRecords();
    unsigned int getNumberOfRichIDs();
    bool isRichVersionPresent(unsigned int nID);

    unsigned int CalculateCheckSum();
    unsigned int getOverlaySize();
    unsigned int getOverlayOffset();
    bool isOverlayPresent();

    bool isSignedFile();

    bool isImportPresent();
    bool isRelocsPresent();
    bool isTLSPresent();
    bool isDebugPresent();
    bool isBoundImportPresent();
    bool isExportPresent();
    bool isResourcePresent();
    bool isLoadConfigPresent();
    bool isNETPresent();
    bool isManifestPresent();
    bool isDll();
    bool isDriver();

    unsigned int calculateSizeOfImage();
    unsigned int calculateSizeOfImage2();
    unsigned int calculateSizeOfHeaders();

    unsigned int calculateBaseOfCode();
    unsigned int calculateBaseOfData();
    unsigned int calculateSizeOfInitializedData();
    unsigned int calculateSizeOfInitializedData2();
    unsigned int calculateSizeOfUninitializedData();
    unsigned int calculateSizeOfCode();

    bool isVersionInfoPresent();

    QString getFileVersion();

    bool dumpSection(QString sFileName,int nSection);
    bool deleteLastSection();
    bool addDosStub(QString sFileName);
    bool DeleteDosStub();
    void FixCheckSum();
    bool dumpDosStub(QString sFileName);
    bool addOverlay(QString sFileName);
    bool deleteOverlay();
    bool dumpOverlay(QString sFileName);
    unsigned int getExportTableOffset();
    unsigned int getExport_Characteristics();
    unsigned int getExport_TimeDateStamp();
    unsigned short getExport_MajorVersion();
    unsigned short getExport_MinorVersion();
    unsigned int getExport_Name();
    unsigned int getExport_Base();
    unsigned int getExport_NumberOfFunctions();
    unsigned int getExport_NumberOfNames();
    unsigned int getExport_AddressOfFunctions();
    unsigned int getExport_AddressOfNames();
    unsigned int getExport_AddressOfNameOrdinals();
    void setExport_Characteristics(unsigned int nValue);
    void setExport_TimeDateStamp(unsigned int nValue);
    void setExport_MajorVersion(unsigned short sValue);
    void setExport_MinorVersion(unsigned short sValue);
    void setExport_Name(unsigned int nValue);
    void setExport_Base(unsigned int nValue);
    void setExport_NumberOfFunctions(unsigned int nValue);
    void setExport_NumberOfNames(unsigned int nValue);
    void setExport_AddressOfFunctions(unsigned int nValue);
    void setExport_AddressOfNames(unsigned int nValue);
    void setExport_AddressOfNameOrdinals(unsigned int nValue);

    QString getExport_NameAsString();

    unsigned int getExport_FunctionAddress(int nNumber);
    unsigned short getExport_FunctionNameOrdinal(int nNumber);
    unsigned int getExport_FunctionName(int nNumber);

    void setExport_FunctionAddress(int nNumber,unsigned int nValue);
    void setExport_FunctionNameOrdinal(int nNumber,unsigned short sValue);
    void setExport_FunctionName(int nNumber,unsigned int nValue);

    QString getExport_FunctionNameAsString(int nNumber);

    bool isExportFunctionPresent(QString sFunctionName);
    bool isExportFunctionPresentExp(QString sFunctionName);

    unsigned int getImportSection();

    unsigned int getImportTableOffset();
    unsigned int getNumberOfImports();

    QList<IMAGE_IMPORT_DESCRIPTOR> getImports();
    QList<QString> getImportLibraiesAsNames();
    QList<QString> getImportFunctionsAsNames();


    QList<IMPORT_LIST> getImportFunctionsAsList();

    QList<QString> getImportLibraryFunctionsAsNames(QString sLibraryName);

    unsigned int getImport_OriginalFirstThunk(unsigned int nImport);
    unsigned int getImport_TimeDateStamp(unsigned int nImport);
    unsigned int getImport_ForwarderChain(unsigned int nImport);
    unsigned int getImport_Name(unsigned int nImport);
    unsigned int getImport_FirstThunk(unsigned int nImport);

    void setImport_OriginalFirstThunk(unsigned int nImport,unsigned int nValue);
    void setImport_TimeDateStamp(unsigned int nImport,unsigned int nValue);
    void setImport_ForwarderChain(unsigned int nImport,unsigned int nValue);
    void setImport_Name(unsigned int nImport,unsigned int nValue);
    void setImport_FirstThunk(unsigned int nImport,unsigned int nValue);

    QString getImport_NameAsString(unsigned int nImport);

    unsigned int getNumberOfImportThunks(unsigned int nImport);
    unsigned int getNumberOfImportThunks(QString sLibraryName);

    unsigned int getImportThunk(unsigned int nImport,unsigned int nFunctionNumber);
    unsigned int getImportThunk(QString sLibraryName,unsigned int nFunctionNumber);
    unsigned long long getImportThunk64(unsigned int nImport,unsigned int nFunctionNumber);
    unsigned long long getImportThunk64(QString sLibraryName,unsigned int nFunctionNumber);

    unsigned int getImportThunkAddress(unsigned int nImport,unsigned int nFunctionNumber);
    unsigned long long getImportThunkAddress64(unsigned int nImport,unsigned int nFunctionNumber);

    QString getImportFunctionName(unsigned int nImport,unsigned int nFunctionNumber);
    QString getImportFunctionName(QString sLibraryName,unsigned int nFunctionNumber);

    void setImportThunk(unsigned int nImport,unsigned int nFunctionNumber,unsigned int nValue);
    void setImportThunk64(unsigned int nImport,unsigned int nFunctionNumber,unsigned long long nValue);

    unsigned int getNumberOfImportOriginalThunks(unsigned int nImport);
    unsigned int getNumberOfImportOriginalThunks(QString sLibraryName);
    unsigned int getImportOriginalThunk(unsigned int nImport,unsigned int nFunctionNumber);
    unsigned int getImportOriginalThunk(QString sLibraryName,unsigned int nFunctionNumber);

    unsigned int getImportLibraryNumber(QString sLibraryName);
    QString getImportLibraryName(unsigned int nImport);

    unsigned int getExportSection();
    unsigned short getExport_FunctionNameOrdinal(int nNumber, unsigned int nAddressOfNameOrdinals, unsigned int nExport_Base);
    unsigned int getExport_FunctionAddress(int nNumber, unsigned int nAddressOfFunctions);
    QString getExport_FunctionNameAsString(int nNumber, unsigned int nAddressOfNames);
    unsigned long long getImportThunk64(unsigned int nOriginalFirstThunk, unsigned int nFirstThunk, unsigned int nFunctionNumber);
    unsigned int getImportThunk(unsigned int nOriginalFirstThunk, unsigned int nFirstThunk, unsigned int nFunctionNumber);
    QString getImportFunctionName(unsigned long long nThunk64);
    QString getImportFunctionName(unsigned int nThunk);
    unsigned short getImportHint(unsigned long long nThunk);
    unsigned short getImportHint(unsigned int nThunk);

    unsigned int getImport_FirstThunk(unsigned int nImport, unsigned int nImportTableOffset);
    unsigned int getImport_Name(unsigned int nImport, unsigned int nImportTableOffset);
    unsigned int getImport_ForwarderChain(unsigned int nImport, unsigned int nImportTableOffset);
    unsigned int getImport_TimeDateStamp(unsigned int nImport, unsigned int nImportTableOffset);
    unsigned int getImport_OriginalFirstThunk(unsigned int nImport, unsigned int nImportTableOffset);
    QString getImport_NameAsString(unsigned int nImport, unsigned int nImportTableOffset);
    bool isLibraryPresent(QString sLibraryName);
    bool isLibraryFunctionPresent(QString sLibraryName,QString sFunctionName);
    bool isSectionNamePresent(QString sSectionName);
    QString getSectionNameAsString(int nSection, unsigned int nSectionsTableOffset);

    unsigned int getSectionNumber(QString sSectionName);
    unsigned int getSectionNumberExp(QString sSectionName);
    bool isSectionNamePresentExp(QString sSectionName);
    QString getSectionNameCollision(QString sString1, QString sString2);

    int getResourceSection();
    unsigned int getResourceOffset();

    //bool isResourceIdPresent(unsigned int nID);
    bool isResourceNamePresent(QString sName);
    bool isResourceNamePresentFast(QString sName);
    unsigned int getResourceNameOffset(QString sName);
    unsigned int getResourceNameSize(QString sName);
    unsigned int getResourceNameOffsetFast(QString sName);
    unsigned int getResourceNameSizeFast(QString sName);
    QByteArray getResourceIdData(unsigned int nID1,unsigned int nID2=-1,unsigned int nID3=-1);
    QByteArray getResourceIdDataFast(unsigned int nID1);
    bool isResourceIdPresent(unsigned int nID1,unsigned int nID2=-1,unsigned int nID3=-1);

    static QString getResourceString(unsigned char *pOffset);

    QString resourceIdToString(unsigned int nID);

    unsigned int getNumberOfResources();
    unsigned int getResourceOffsetByNumber(unsigned int nNumber);
    unsigned int getResourceSizeByNumber(unsigned int nNumber);
    unsigned int getResourceIdByNumber(unsigned int nNumber);
    QString getResourceNameByNumber(unsigned int nNumber);
    QString getResourceTypeByNumber(unsigned int nNumber);

    QString getManifest();

    // CLI

    unsigned int getNETOffset();

    bool initCLI();

    unsigned int getCLIHeaderOffset();
    unsigned int getCLI_Size();
    unsigned short getCLI_MajorRuntimeVersion();
    unsigned short getCLI_MinorRuntimeVersion();
    void getCLI_MetaData(IMAGE_DATA_DIRECTORY *pDataDirectory);
    unsigned int getCLI_Flags();
    unsigned int getCLI_EntryPoint();
    void getCLI_Resources(IMAGE_DATA_DIRECTORY *pDataDirectory);
    void getCLI_StrongNameSignature(IMAGE_DATA_DIRECTORY *pDataDirectory);
    void getCLI_CodeManagerTable(IMAGE_DATA_DIRECTORY *pDataDirectory);
    void getCLI_VTableFixups(IMAGE_DATA_DIRECTORY *pDataDirectory);
    void getCLI_ExportAddressTableJumps(IMAGE_DATA_DIRECTORY *pDataDirectory);
    void getCLI_ManagedNativeHeader(IMAGE_DATA_DIRECTORY *pDataDirectory);

    QByteArray getCLI_MetaData();

    unsigned int getCLI_MetaData_Signature();
    unsigned short getCLI_MetaData_MajorVersion();
    unsigned short getCLI_MetaData_MinorVersion();
    unsigned int getCLI_MetaData_Reserved();
    QString getCLI_MetaData_Version();
    unsigned short getCLI_MetaData_Flags();
    unsigned short getCLI_MetaData_Streams();
    unsigned int getCLI_MetaData_Stream_Offset(int nStream);
    unsigned int getCLI_MetaData_Stream_Size(int nStream);
    QString getCLI_MetaData_Stream_Name(int nStream);
    unsigned int getCLI_MetaDataOffset();
    unsigned int getCLI_MetaData_TablesHeaderOffset();
    unsigned int getCLI_MetaDataStreamOffset(QString sStream);
    unsigned int getCLI_MetaData_Tables_Reserved1();
    unsigned char getCLI_MetaData_Tables_MajorVersion();
    unsigned char getCLI_MetaData_Tables_MinorVersion();
    unsigned char getCLI_MetaData_Tables_HeapOffsetSizes();
    unsigned char getCLI_MetaData_Tables_Reserved2();
    unsigned long long getCLI_MetaData_Tables_Valid();
    unsigned long long getCLI_MetaData_Tables_Sorted();
    unsigned int getCLI_MetaData_Tables_Valid_NumberOfRows();
    bool isCLI_MetaData_Tables_TablePresent(int nTable);
    bool isCLI_MetaData_Tables_TableSorted(int nTable);
    static QString CLI_MetaData_Tables_TableToString(int nTable);
    unsigned int getCLI_MetaData_Tables_TablesNumberOfIndexes(int nTable);
    unsigned int getCLI_MetaData_Tables_SizeOfHeader(int nTable);
    unsigned int getCLI_MetaData_Tables_HeadersOffset(int nTable);
    unsigned int getCLI_MetaData_Tables_MethodDef_RVA(int nIndex);
    unsigned int getCLIEntryPoint();
    void entryPointLoad_NET();



    QByteArray getCLI_MetaData_Strings();
    QByteArray getCLI_MetaData_US();
    QByteArray getCLI_MetaData_Blob();
    QByteArray getCLI_MetaData_GUID();

    QList<QString> getCLI_MetaData_StringsAsList();
    QList<QString> getCLI_MetaData_USAsList();

    bool isCLI_MetaData_StringPresent(QString sString);
    bool isCLI_MetaData_UnicodeStringPresent(QString sString);

    QByteArray getRelocsAsXML();
    QList<unsigned int> getRelocsAsList();
    bool addRelocsSectionFromList(QList<unsigned int> *pList,QString sSectionName=".relocs");
    unsigned int getNumberOfRelocs();
    unsigned int getNumberOfRelocsPages();
    unsigned int getRelocsTableOffset();

    int getRelocsSection();

    unsigned int getTLSSection();
    unsigned int getTLSTableOffset();

    unsigned int getTLS_StartAddressOfRawData();
    unsigned long long getTLS_StartAddressOfRawData64();
    unsigned int getTLS_EndAddressOfRawData();
    unsigned long long getTLS_EndAddressOfRawData64();
    unsigned int getTLS_AddressOfIndex();
    unsigned long long getTLS_AddressOfIndex64();
    unsigned int getTLS_AddressOfCallBacks();
    unsigned long long getTLS_AddressOfCallBacks64();

    unsigned int getTLS_SizeOfZeroFill();
    unsigned int getTLS_Characteristics();

    void setTLS_StartAddressOfRawData(unsigned int nValue);
    void setTLS_StartAddressOfRawData64(unsigned long long nValue);
    void setTLS_EndAddressOfRawData(unsigned int nValue);
    void setTLS_EndAddressOfRawData64(unsigned long long nValue);
    void setTLS_AddressOfIndex(unsigned int nValue);
    void setTLS_AddressOfIndex64(unsigned long long nValue);
    void setTLS_AddressOfCallBacks(unsigned int nValue);
    void setTLS_AddressOfCallBacks64(unsigned long long nValue);

    void setTLS_SizeOfZeroFill(unsigned int nValue);
    void setTLS_Characteristics(unsigned int nValue);

    unsigned int getLoadConfigTableOffset();

    unsigned int getLoadConfig_Size();
    unsigned int getLoadConfig_TimeDateStamp();
    unsigned short getLoadConfig_MajorVersion();
    unsigned short getLoadConfig_MinorVersion();
    unsigned int getLoadConfig_GlobalFlagsClear();
    unsigned int getLoadConfig_GlobalFlagsSet();
    unsigned int getLoadConfig_CriticalSectionDefaultTimeout();

    void setLoadConfig_Size(unsigned int nValue);
    void setLoadConfig_TimeDateStamp(unsigned int nValue);
    void setLoadConfig_MajorVersion(unsigned short sValue);
    void setLoadConfig_MinorVersion(unsigned short sValue);
    void setLoadConfig_GlobalFlagsClear(unsigned int nValue);
    void setLoadConfig_GlobalFlagsSet(unsigned int nValue);
    void setLoadConfig_CriticalSectionDefaultTimeout(unsigned int nValue);

    // TODO set PEPlus
    unsigned int getLoadConfig_DeCommitFreeBlockThreshold();
    unsigned long long getLoadConfig_DeCommitFreeBlockThreshold64();
    unsigned int getLoadConfig_DeCommitTotalFreeThreshold();
    unsigned long long getLoadConfig_DeCommitTotalFreeThreshold64();
    unsigned int getLoadConfig_LockPrefixTable();
    unsigned long long getLoadConfig_LockPrefixTable64();
    unsigned int getLoadConfig_MaximumAllocationSize();
    unsigned long long getLoadConfig_MaximumAllocationSize64();
    unsigned int getLoadConfig_VirtualMemoryThreshold();
    unsigned long long getLoadConfig_VirtualMemoryThreshold64();
    unsigned int getLoadConfig_ProcessAffinityMask();
    unsigned long long getLoadConfig_ProcessAffinityMask64();

    void setLoadConfig_DeCommitFreeBlockThreshold(unsigned int nValue);
    void setLoadConfig_DeCommitFreeBlockThreshold64(unsigned long long nValue);
    void setLoadConfig_DeCommitTotalFreeThreshold(unsigned int nValue);
    void setLoadConfig_DeCommitTotalFreeThreshold64(unsigned long long nValue);
    void setLoadConfig_LockPrefixTable(unsigned int nValue);
    void setLoadConfig_LockPrefixTable64(unsigned long long nValue);
    void setLoadConfig_MaximumAllocationSize(unsigned int nValue);
    void setLoadConfig_MaximumAllocationSize64(unsigned long long nValue);
    void setLoadConfig_VirtualMemoryThreshold(unsigned int nValue);
    void setLoadConfig_VirtualMemoryThreshold64(unsigned long long nValue);
    void setLoadConfig_ProcessAffinityMask(unsigned int nValue);
    void setLoadConfig_ProcessAffinityMask64(unsigned long long nValue);

    unsigned int getLoadConfig_ProcessHeapFlags();
    unsigned short getLoadConfig_CSDVersion();
    unsigned short getLoadConfig_Reserved1();

    void setLoadConfig_ProcessHeapFlags(unsigned int nValue);
    void setLoadConfig_CSDVersion(unsigned short sValue);
    void setLoadConfig_Reserved1(unsigned short sValue);

    unsigned int getLoadConfig_EditList();
    unsigned long long getLoadConfig_EditList64();
    unsigned int getLoadConfig_SecurityCookie();
    unsigned long long getLoadConfig_SecurityCookie64();
    unsigned int getLoadConfig_SEHandlerTable();
    unsigned long long getLoadConfig_SEHandlerTable64();
    unsigned int getLoadConfig_SEHandlerCount();
    unsigned long long getLoadConfig_SEHandlerCount64();

    void setLoadConfig_EditList(unsigned int nValue);
    void setLoadConfig_EditList64(unsigned long long nValue);
    void setLoadConfig_SecurityCookie(unsigned int nValue);
    void setLoadConfig_SecurityCookie64(unsigned long long nValue);
    void setLoadConfig_SEHandlerTable(unsigned int nValue);
    void setLoadConfig_SEHandlerTable64(unsigned long long nValue);
    void setLoadConfig_SEHandlerCount(unsigned int nValue);
    void setLoadConfig_SEHandlerCount64(unsigned long long nValue);

    unsigned int getBoundImportTableOffset();
    unsigned int getBoundImport_NumberOfImports();
    unsigned int getBoundImport_TimeDateStamp(unsigned int nImport);
    unsigned short getBoundImport_OffsetModuleName(unsigned int nImport);
    unsigned short getBoundImport_NumberOfModuleForwarderRefs(unsigned int nImport);
    QString getBoundImport_ModuleNameAsString(unsigned int nImport);

    unsigned int getDebugTableOffset();
    unsigned int getDebugTableSize();
    unsigned int getDebug_NumberOfDirectories();
    unsigned int getDebug_Characteristics(unsigned int nDebug);
    unsigned int getDebug_TimeDateStamp(unsigned int nDebug);
    unsigned short getDebug_MajorVersion(unsigned int nDebug);
    unsigned short getDebug_MinorVersion(unsigned int nDebug);
    unsigned int getDebug_Type(unsigned int nDebug);
    unsigned int getDebug_SizeOfData(unsigned int nDebug);
    unsigned int getDebug_AddressOfRawData(unsigned int nDebug);
    unsigned int getDebug_PointerToRawData(unsigned int nDebug);

    unsigned long long getEntryPointOffset();
    QString getVersionStringInfo(QString sKey);
private:
    struct CLI_SECTION
    {
        bool bIsInit;

        unsigned int nCLIHeaderOffset;
        unsigned int nCLI_Size;
        unsigned sCLI_MajorRuntimeVersion;
        unsigned sCLI_MinorRuntimeVersion;
        IMAGE_DATA_DIRECTORY CLI_MetaData;
        unsigned int nCLI_Flags;
        unsigned int nCLI_EntryPoint;
        IMAGE_DATA_DIRECTORY CLI_Resources;
        IMAGE_DATA_DIRECTORY CLI_StrongNameSignature;
        IMAGE_DATA_DIRECTORY CLI_CodeManagerTable;
        IMAGE_DATA_DIRECTORY CLI_VTableFixups;
        IMAGE_DATA_DIRECTORY CLI_ExportAddressTableJumps;
        IMAGE_DATA_DIRECTORY CLI_ManagedNativeHeader;

        unsigned int nCLI_MetaDataOffset;
        unsigned int nCLI_MetaData_Signature;
        unsigned short sCLI_MetaData_MajorVersion;
        unsigned short sCLI_MetaData_MinorVersion;
        unsigned int nCLI_MetaData_Reserved;
        unsigned int nCLI_MetaData_VersionStringLength;
        QString sCLI_MetaData_Version;
        unsigned short sCLI_MetaData_Flags;
        unsigned short sCLI_MetaData_Streams;

        QList <unsigned int> listCLI_MetaData_Stream_Offsets;
        QList <unsigned int> listCLI_MetaData_Stream_Sizes;
        QList <QString> listCLI_MetaData_Stream_Names;

        unsigned int nCLI_MetaData_TablesHeaderOffset;
        unsigned int nCLI_MetaData_TablesSize;

        unsigned int nCLI_MetaData_Tables_Reserved1;
        unsigned char cCLI_MetaData_Tables_MajorVersion;
        unsigned char cCLI_MetaData_Tables_MinorVersion;
        unsigned char cCLI_MetaData_Tables_HeapOffsetSizes;
        unsigned char cCLI_MetaData_Tables_Reserved2;
        unsigned long long nCLI_MetaData_Tables_Valid;
        unsigned long long nCLI_MetaData_Tables_Sorted;
        unsigned int nCLI_MetaData_Tables_Valid_NumberOfRows;
        unsigned int CLI_MetaData_Tables_TablesNumberOfIndexes[64];
        unsigned int CLI_MetaData_Tables_TablesOffsets[64];
        unsigned int CLI_MetaData_Tables_TablesSizes[64];

        unsigned int nCLI_MetaData_StringsOffset;
        unsigned int nCLI_MetaData_StringsSize;
        unsigned int nCLI_MetaData_USOffset;
        unsigned int nCLI_MetaData_USSize;
        unsigned int nCLI_MetaData_BlobOffset;
        unsigned int nCLI_MetaData_BlobSize;
        unsigned int nCLI_MetaData_GUIDOffset;
        unsigned int nCLI_MetaData_GUIDSize;

        unsigned int nEntryPoint;
        char pEntryPoint[EPCODE];
        unsigned int nEntryPointSize;
    } CLI_Section;
};

#endif // PEFILE_H
