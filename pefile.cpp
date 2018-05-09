#include "pefile.h"

PEFile::PEFile(QObject *parent) :
    Binary(parent)
{
    CLI_Section.bIsInit=false;
    bIsNETEntryPontLoad=false;
}

PEFile::~PEFile()
{

}

QByteArray PEFile::createHeader32()
{
    QByteArray baResult;

    baResult.resize(0x200);
    baResult.fill(0);

    char *pOffset=baResult.data();

    // MS DOS HEADER
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_magic))     =0x5A4D;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_cblp))      =0x0090;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_cp))        =0x0003;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_crlc))      =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_cparhdr))   =0x0004;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_minalloc))  =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_maxalloc))  =0xFFFF;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_ss))        =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_sp))        =0x00B8;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_csum))      =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_ip))        =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_cs))        =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_lfarlc))    =0x0040;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_DOS_HEADER,e_lfanew))    =sizeof(IMAGE_DOS_HEADER);

    // No MS Dos Stub!

    // NT Headers
    pOffset+=sizeof(IMAGE_DOS_HEADER);
    *(unsigned int *)pOffset=IMAGE_NT_SIGNATURE;

    // File Header
    pOffset+=4;
    *(unsigned short *)(pOffset+offsetof(IMAGE_FILE_HEADER,Machine))                =0x014C;
    *(unsigned short *)(pOffset+offsetof(IMAGE_FILE_HEADER,NumberOfSections))       =0x0000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_FILE_HEADER,TimeDateStamp))          =QDateTime::currentDateTime().toTime_t();
    *(unsigned int   *)(pOffset+offsetof(IMAGE_FILE_HEADER,PointerToSymbolTable))   =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_FILE_HEADER,NumberOfSymbols))        =0x00000000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_FILE_HEADER,SizeOfOptionalHeader))   =0x00E0;
    *(unsigned short *)(pOffset+offsetof(IMAGE_FILE_HEADER,Characteristics))        =0x10F;

    // Optional Header
    pOffset+=sizeof(IMAGE_FILE_HEADER);
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,Magic))                        =0x010B;
    *(unsigned char  *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MajorLinkerVersion))           =0x00;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MinorLinkerVersion))           =0x00;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfCode))                   =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfInitializedData))        =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfUninitializedData))      =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,AddressOfEntryPoint))          =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,BaseOfCode))                   =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,BaseOfData))                   =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,ImageBase))                    =0x00400000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SectionAlignment))             =0x00001000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,FileAlignment))                =0x00000200;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MajorOperatingSystemVersion))  =0x0004;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MinorOperatingSystemVersion))  =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MajorImageVersion))            =0x0004;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MinorImageVersion))            =0x0000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MajorSubsystemVersion))        =0x0004;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,MinorSubsystemVersion))        =0x0000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,Win32VersionValue))            =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfImage))                  =0x00001000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfHeaders))                =0x00000200;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,CheckSum))                     =0x00000000;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,Subsystem))                    =0x0002;
    *(unsigned short *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,DllCharacteristics))           =0x0000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfStackReserve))           =0x00100000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfStackCommit))            =0x00001000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfHeapReserve))            =0x00100000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,SizeOfHeapCommit))             =0x00001000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,LoaderFlags))                  =0x00000000;
    *(unsigned int   *)(pOffset+offsetof(IMAGE_OPTIONAL_HEADER32,NumberOfRvaAndSizes))          =0x00000010;

    return baResult;
}
unsigned int PEFile::getDosHeaderOffset()
{
    return 0;
}
unsigned int PEFile::getDosHeaderSize()
{
    return sizeof(IMAGE_DOS_HEADER);
}
unsigned short PEFile::getDosHeader_magic()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_magic));
}
unsigned short PEFile::getDosHeader_cblp()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cblp));
}
unsigned short PEFile::getDosHeader_cp()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cp));
}
unsigned short PEFile::getDosHeader_crlc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_crlc));
}
unsigned short PEFile::getDosHeader_cparhdr()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cparhdr));
}
unsigned short PEFile::getDosHeader_minalloc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_minalloc));
}
unsigned short PEFile::getDosHeader_maxalloc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_maxalloc));
}
unsigned short PEFile::getDosHeader_ss()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_ss));
}
unsigned short PEFile::getDosHeader_sp()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_sp));
}
unsigned short PEFile::getDosHeader_csum()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_csum));
}
unsigned short PEFile::getDosHeader_ip()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_ip));
}
unsigned short PEFile::getDosHeader_cs()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cs));
}
unsigned short PEFile::getDosHeader_lfarlc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_lfarlc));
}
unsigned short PEFile::getDosHeader_ovno()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_ovno));
}
unsigned short PEFile::getDosHeader_res(int nIndex)
{
    if(nIndex<4)
    {
        return readWord(offsetof(IMAGE_DOS_HEADER,e_res)+sizeof(unsigned short)*nIndex);
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
    return 0;
}
unsigned short PEFile::getDosHeader_oemid()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_oemid));
}
unsigned short PEFile::getDosHeader_oeminfo()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_oeminfo));
}
unsigned short PEFile::getDosHeader_res2(int nIndex)
{
    if(nIndex<10)
    {
        return readWord(offsetof(IMAGE_DOS_HEADER,e_res2)+sizeof(unsigned short)*nIndex);
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
    return 0;
}
void PEFile::setDosHeader_magic(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_magic),sValue);
}
void PEFile::setDosHeader_cblp(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cblp),sValue);
}
void PEFile::setDosHeader_cp(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cp),sValue);
}
void PEFile::setDosHeader_crlc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_crlc),sValue);
}
void PEFile::setDosHeader_cparhdr(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cparhdr),sValue);
}
void PEFile::setDosHeader_minalloc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_minalloc),sValue);
}
void PEFile::setDosHeader_maxalloc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_maxalloc),sValue);
}
void PEFile::setDosHeader_ss(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_ss),sValue);
}
void PEFile::setDosHeader_sp(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_sp),sValue);
}
void PEFile::setDosHeader_csum(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_csum),sValue);
}
void PEFile::setDosHeader_ip(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_ip),sValue);
}
void PEFile::setDosHeader_cs(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cs),sValue);
}
void PEFile::setDosHeader_lfarlc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_lfarlc),sValue);
}
void PEFile::setDosHeader_ovno(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_ovno),sValue);
}
void PEFile::setDosHeader_res(short sValue,int nIndex)
{
    if(nIndex<4)
    {
        writeWord(offsetof(IMAGE_DOS_HEADER,e_res)+sizeof(unsigned short)*nIndex,sValue);
        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
}
void PEFile::setDosHeader_oemid(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_oemid),sValue);
}
void PEFile::setDosHeader_oeminfo(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_oeminfo),sValue);
}
void PEFile::setDosHeader_res2(short sValue,int nIndex)
{
    if(nIndex<10)
    {
        writeWord(offsetof(IMAGE_DOS_HEADER,e_res2)+sizeof(unsigned short)*nIndex,sValue);
        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
}

unsigned int PEFile::getDosHeader_lfanew()
{
    return readDword(offsetof(IMAGE_DOS_HEADER,e_lfanew));
}

void PEFile::setDosHeader_lfanew(unsigned int nValue)
{
    writeDword(offsetof(IMAGE_DOS_HEADER,e_lfanew),nValue);
}

unsigned int PEFile::getNtHeadersOffset()
{
    return getDosHeader_lfanew();
}

unsigned int PEFile::getNTHeaders_Signature()
{
    unsigned int nResult=readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,Signature));
    return nResult;
}

void PEFile::setNTHeaders_Signature(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,Signature),nValue);
}

unsigned short PEFile::getFileHeader_Machine()
{
    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.Machine));
}

unsigned short PEFile::getFileHeader_NumberOfSections()
{
    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.NumberOfSections));
}

unsigned int PEFile::getFileHeader_TimeDateStamp()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.TimeDateStamp));
}

unsigned int PEFile::getFileHeader_PointerToSymbolTable()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.PointerToSymbolTable));
}

unsigned int PEFile::getFileHeader_NumberOfSymbols()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.NumberOfSymbols));
}

unsigned short PEFile::getFileHeader_SizeOfOptionalHeader()
{
    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.SizeOfOptionalHeader));
}

unsigned short PEFile::getFileHeader_Characteristics()
{
    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.Characteristics));
}

void PEFile::setFileHeader_Machine(unsigned short sValue)
{
    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.Machine),sValue);
}

void PEFile::setFileHeader_NumberOfSections(unsigned short sValue)
{
    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.NumberOfSections),sValue);
}

void PEFile::setFileHeader_TimeDateStamp(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.TimeDateStamp),nValue);
}

void PEFile::setFileHeader_PointerToSymbolTable(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.PointerToSymbolTable),nValue);
}

void PEFile::setFileHeader_NumberOfSymbols(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.NumberOfSymbols),nValue);
}

void PEFile::setFileHeader_SizeOfOptionalHeader(unsigned short sValue)
{
    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.SizeOfOptionalHeader),sValue);
}

void PEFile::setFileHeader_Characteristics(unsigned short sValue)
{
    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,FileHeader.Characteristics),sValue);
}

unsigned short PEFile::getOptionalHeader_Magic()
{
    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.Magic));
}

unsigned char PEFile::getOptionalHeader_MajorLinkerVersion()
{
    return readByte(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorLinkerVersion));
}

unsigned char PEFile::getOptionalHeader_MinorLinkerVersion()
{
    return readByte(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorLinkerVersion));
}

unsigned int PEFile::getOptionalHeader_SizeOfCode()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfCode));
}

unsigned int PEFile::getOptionalHeader_SizeOfInitializedData()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfInitializedData));
}

unsigned int PEFile::getOptionalHeader_SizeOfUninitializedData()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfUninitializedData));
}

unsigned int PEFile::getOptionalHeader_AddressOfEntryPoint()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.AddressOfEntryPoint));
}

unsigned int PEFile::getOptionalHeader_BaseOfCode()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.BaseOfCode));
}

unsigned int PEFile::getOptionalHeader_BaseOfData()
{
    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.BaseOfData));
}

void PEFile::setOptionalHeader_Magic(unsigned short sValue)
{
    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.Magic),sValue);
}

void PEFile::setOptionalHeader_MajorLinkerVersion(unsigned char cValue)
{
    writeByte(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorLinkerVersion),cValue);
}

void PEFile::setOptionalHeader_MinorLinkerVersion(unsigned char cValue)
{
    writeByte(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorLinkerVersion),cValue);
}

void PEFile::setOptionalHeader_SizeOfCode(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfCode),nValue);
}

void PEFile::setOptionalHeader_SizeOfInitializedData(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfInitializedData),nValue);
}

void PEFile::setOptionalHeader_SizeOfUninitializedData(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfUninitializedData),nValue);
}

void PEFile::setOptionalHeader_AddressOfEntryPoint(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.AddressOfEntryPoint),nValue);
}

void PEFile::setOptionalHeader_BaseOfCode(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.BaseOfCode),nValue);
}

void PEFile::setOptionalHeader_BaseOfData(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.BaseOfData),nValue);
}

unsigned int PEFile::getOptionalHeader_ImageBase()
{
    if(isPEPlus())
    {
        return (unsigned int)readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.ImageBase));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.ImageBase));
}

unsigned long long PEFile::getOptionalHeader_ImageBase64()
{
    return readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.ImageBase));
}

unsigned int PEFile::getOptionalHeader_SectionAlignment()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SectionAlignment));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SectionAlignment));
}

unsigned int PEFile::getOptionalHeader_FileAlignment()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.FileAlignment));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.FileAlignment));
}

unsigned short PEFile::getOptionalHeader_MajorOperatingSystemVersion()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MajorOperatingSystemVersion));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorOperatingSystemVersion));
}

unsigned short PEFile::getOptionalHeader_MinorOperatingSystemVersion()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MinorOperatingSystemVersion));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorOperatingSystemVersion));
}

unsigned short PEFile::getOptionalHeader_MajorImageVersion()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MajorImageVersion));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorImageVersion));
}

unsigned short PEFile::getOptionalHeader_MinorImageVersion()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MinorImageVersion));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorImageVersion));
}

unsigned short PEFile::getOptionalHeader_MajorSubsystemVersion()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MajorSubsystemVersion));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorSubsystemVersion));
}

unsigned short PEFile::getOptionalHeader_MinorSubsystemVersion()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MinorSubsystemVersion));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorSubsystemVersion));
}

unsigned int PEFile::getOptionalHeader_Win32VersionValue()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.Win32VersionValue));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.Win32VersionValue));
}

unsigned int PEFile::getOptionalHeader_SizeOfImage()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfImage));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfImage));
}

unsigned int PEFile::getOptionalHeader_SizeOfHeaders()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeaders));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfHeaders));
}

unsigned int PEFile::getOptionalHeader_CheckSum()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.CheckSum));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.CheckSum));
}

unsigned short PEFile::getOptionalHeader_Subsystem()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.Subsystem));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.Subsystem));
}

unsigned short PEFile::getOptionalHeader_DllCharacteristics()
{
    if(isPEPlus())
    {
        return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.DllCharacteristics));
    }

    return readWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.DllCharacteristics));
}

unsigned int PEFile::getOptionalHeader_SizeOfStackReserve()
{
    if(isPEPlus())
    {
        return (unsigned int)readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfStackReserve));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfStackReserve));
}

unsigned int PEFile::getOptionalHeader_SizeOfStackCommit()
{
    if(isPEPlus())
    {
        return (unsigned int)readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfStackCommit));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfStackCommit));
}

unsigned int PEFile::getOptionalHeader_SizeOfHeapReserve()
{
    if(isPEPlus())
    {
        return (unsigned int)readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeapReserve));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfHeapReserve));
}

unsigned int PEFile::getOptionalHeader_SizeOfHeapCommit()
{
    if(isPEPlus())
    {
        return (unsigned int)readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeapCommit));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfHeapCommit));
}

unsigned long long PEFile::getOptionalHeader_SizeOfStackReserve64()
{
    return readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfStackReserve));
}

unsigned long long PEFile::getOptionalHeader_SizeOfStackCommit64()
{
    return readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfStackCommit));
}

unsigned long long PEFile::getOptionalHeader_SizeOfHeapReserve64()
{
    return readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeapReserve));
}

unsigned long long PEFile::getOptionalHeader_SizeOfHeapCommit64()
{
    return readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeapCommit));
}

unsigned int PEFile::getOptionalHeader_LoaderFlags()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.LoaderFlags));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.LoaderFlags));
}

unsigned int PEFile::getOptionalHeader_NumberOfRvaAndSizes()
{
    if(isPEPlus())
    {
        return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.NumberOfRvaAndSizes));
    }

    return readDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.NumberOfRvaAndSizes));
}

void PEFile::setOptionalHeader_ImageBase(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.ImageBase),nValue);
}

void PEFile::setOptionalHeader_ImageBase64(unsigned long long nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.ImageBase),nValue);
}

void PEFile::setOptionalHeader_SectionAlignment(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SectionAlignment),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SectionAlignment),nValue);
}

void PEFile::setOptionalHeader_FileAlignment(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.FileAlignment),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.FileAlignment),nValue);
}

void PEFile::setOptionalHeader_MajorOperatingSystemVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MajorOperatingSystemVersion),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorOperatingSystemVersion),sValue);
}

void PEFile::setOptionalHeader_MinorOperatingSystemVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MinorOperatingSystemVersion),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorOperatingSystemVersion),sValue);
}

void PEFile::setOptionalHeader_MajorImageVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MajorImageVersion),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorImageVersion),sValue);
}

void PEFile::setOptionalHeader_MinorImageVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MinorImageVersion),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorImageVersion),sValue);
}

void PEFile::setOptionalHeader_MajorSubsystemVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MajorSubsystemVersion),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MajorSubsystemVersion),sValue);
}

void PEFile::setOptionalHeader_MinorSubsystemVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.MinorSubsystemVersion),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.MinorSubsystemVersion),sValue);
}

void PEFile::setOptionalHeader_Win32VersionValue(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.Win32VersionValue),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.Win32VersionValue),nValue);
}

void PEFile::setOptionalHeader_SizeOfImage(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfImage),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfImage),nValue);
}

void PEFile::setOptionalHeader_SizeOfHeaders(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeaders),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfHeaders),nValue);
}

void PEFile::setOptionalHeader_CheckSum(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.CheckSum),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.CheckSum),nValue);
}

void PEFile::setOptionalHeader_Subsystem(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.Subsystem),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.Subsystem),sValue);
}

void PEFile::setOptionalHeader_DllCharacteristics(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.DllCharacteristics),sValue);
        return;
    }

    writeWord(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.DllCharacteristics),sValue);
}

void PEFile::setOptionalHeader_SizeOfStackReserve(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfStackReserve),nValue);
}

void PEFile::setOptionalHeader_SizeOfStackCommit(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfStackCommit),nValue);
}

void PEFile::setOptionalHeader_SizeOfHeapReserve(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfHeapReserve),nValue);
}

void PEFile::setOptionalHeader_SizeOfHeapCommit(unsigned int nValue)
{
    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.SizeOfHeapCommit),nValue);
}

void PEFile::setOptionalHeader_SizeOfStackReserve64(unsigned long long nValue)
{
    writeQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfStackReserve),nValue);
}

void PEFile::setOptionalHeader_SizeOfStackCommit64(unsigned long long nValue)
{
    writeQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfStackCommit),nValue);
}

void PEFile::setOptionalHeader_SizeOfHeapReserve64(unsigned long long nValue)
{
    writeQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeapReserve),nValue);
}

void PEFile::setOptionalHeader_SizeOfHeapCommit64(unsigned long long nValue)
{
    writeQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.SizeOfHeapCommit),nValue);
}

void PEFile::setOptionalHeader_LoaderFlags(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.LoaderFlags),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.LoaderFlags),nValue);
}

void PEFile::setOptionalHeader_NumberOfRvaAndSizes(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.NumberOfRvaAndSizes),nValue);
        return;
    }

    writeDword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.NumberOfRvaAndSizes),nValue);
}

void PEFile::getOptionalHeader_DataDirectory(int nDirectory, IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(nDirectory>=16)
    {
        emit appendError(QString("Value out of range: n=%1").arg(nDirectory));

        return;
    }


    if(isPEPlus())
    {
        readArray(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.DataDirectory)+sizeof(IMAGE_DATA_DIRECTORY)*nDirectory,(char *)pDataDirectory,sizeof(IMAGE_DATA_DIRECTORY));
        return;
    }

    readArray(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.DataDirectory)+sizeof(IMAGE_DATA_DIRECTORY)*nDirectory,(char *)pDataDirectory,sizeof(IMAGE_DATA_DIRECTORY));
}

void PEFile::setOptionalHeader_DataDirectory(int nDirectory, IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(nDirectory>=16)
    {
        emit appendError(QString("Value out of range: n=%1").arg(nDirectory));

        return;
    }


    if(isPEPlus())
    {
        writeArray(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.DataDirectory)+sizeof(IMAGE_DATA_DIRECTORY)*nDirectory,(char *)pDataDirectory,sizeof(IMAGE_DATA_DIRECTORY));
        return;
    }

    writeArray(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS32,OptionalHeader.DataDirectory)+sizeof(IMAGE_DATA_DIRECTORY)*nDirectory,(char *)pDataDirectory,sizeof(IMAGE_DATA_DIRECTORY));
}

bool PEFile::isDirectoryPresent(int nDirectory)
{
    if(nDirectory>=16)
    {
        emit appendError(QString("Value out of range: n=%1").arg(nDirectory));

        return false;
    }

    IMAGE_DATA_DIRECTORY idd;
    getOptionalHeader_DataDirectory(nDirectory,&idd);

    //    return (isRVAValid(idd.VirtualAddress)&&idd.Size);

    return (idd.VirtualAddress&&isRVAPresentInFile(idd.VirtualAddress));
}

bool PEFile::isRVAPresentInFile(unsigned int nRVA)
{
    return (nRVA<=getOptionalHeader_SizeOfImage())&&(RVAToOffset(nRVA)<=calculateRawSize());
}

bool PEFile::isOffsetValid(unsigned int nOffset)
{
    return (nOffset<=calculateRawSize());
}

bool PEFile::isRVAValid(unsigned int nRVA)
{
    return (nRVA<=getOptionalHeader_SizeOfImage());
}

bool PEFile::isVAValid(unsigned int nVA)
{
    return isRVAValid(VAToRVA(nVA));
}

int PEFile::getNumberOfSections()
{
    return getFileHeader_NumberOfSections();
}



bool PEFile::isSectionPresent(int nSection)
{
    if(nSection<getFileHeader_NumberOfSections())
    {
        return true;
    }

    emit appendError(QString("Invalid section number: %1").arg(nSection));

    return false;
}

unsigned int PEFile::getSectionHeaderOffset(int nSection)
{
    if(isSectionPresent(nSection))
    {
        return (int)getSectionsTableOffset()+nSection*sizeof(IMAGE_SECTION_HEADER);
    }

    return 0;
}

unsigned int PEFile::getSectionsTableOffset()
{
    return (unsigned int)getNtHeadersOffset()+4+sizeof(IMAGE_FILE_HEADER)+getFileHeader_SizeOfOptionalHeader();
}

unsigned int PEFile::getSectionsTableSize()
{
    return getFileHeader_NumberOfSections()*sizeof(IMAGE_SECTION_HEADER);
}


unsigned int PEFile::getSection_VirtualSize(int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,Misc.VirtualSize));
}

unsigned int PEFile::getSection_VirtualAddress(int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,VirtualAddress));
}

unsigned int PEFile::getSection_SizeOfRawData(int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,SizeOfRawData));
}

unsigned int PEFile::getSection_PointerToRawData(int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,PointerToRawData));
}

unsigned int PEFile::getSection_PointerToRelocations(int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,PointerToRelocations));
}

unsigned int PEFile::getSection_PointerToLinenumbers(int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,PointerToLinenumbers));
}

unsigned short PEFile::getSection_NumberOfRelocations(int nSection)
{
    return readWord(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,NumberOfRelocations));
}

unsigned short PEFile::getSection_NumberOfLinenumbers(int nSection)
{
    return readWord(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,NumberOfLinenumbers));
}

unsigned int PEFile::getSection_Characteristics(int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,Characteristics));
}




bool PEFile::isValid()
{
    if(size()>=(int)sizeof(IMAGE_DOS_HEADER))
    {
        if(getDosHeader_magic()==IMAGE_DOS_SIGNATURE)
        {
            if(getDosHeader_lfanew()<size())
            {
                if(getNTHeaders_Signature()==IMAGE_NT_SIGNATURE)
                {
                    return true;
                }
            }
        }
    }

    emit appendError("Invalid PE file");

    return false;
}

bool PEFile::isPEPlus()
{
    if(isValid())
    {
        return (getFileHeader_Machine()==IMAGE_FILE_MACHINE_AMD64)||(getFileHeader_Machine()==IMAGE_FILE_MACHINE_IA64);
    }

    return false;
}





unsigned int PEFile::getNtHeadersSize()
{
    return 4+sizeof(IMAGE_FILE_HEADER)+getFileHeader_SizeOfOptionalHeader();
}

void PEFile::setSection_VirtualSize(int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,Misc.VirtualSize),nValue);
}

void PEFile::setSection_VirtualAddress(int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,VirtualAddress),nValue);
}

void PEFile::setSection_SizeOfRawData(int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,SizeOfRawData),nValue);
}

void PEFile::setSection_PointerToRawData(int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,PointerToRawData),nValue);
}

void PEFile::setSection_PointerToRelocations(int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,PointerToRelocations),nValue);
}

void PEFile::setSection_PointerToLinenumbers(int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,PointerToLinenumbers),nValue);
}

void PEFile::setSection_NumberOfRelocations(int nSection, unsigned short sValue)
{
    writeWord(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,NumberOfRelocations),sValue);
}

void PEFile::setSection_NumberOfLinenumbers(int nSection, unsigned short sValue)
{
    writeWord(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,NumberOfLinenumbers),sValue);
}

void PEFile::setSection_Characteristics(int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,Characteristics),nValue);
}

QString PEFile::getSectionNameAsString(int nSection)
{
    return getAnsiString(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,Name),8);
}

QString PEFile::getSectionNameAsString(int nSection,unsigned int nSectionsTableOffset)
{
    return getAnsiString(nSectionsTableOffset+nSection*sizeof(IMAGE_SECTION_HEADER)+offsetof(IMAGE_SECTION_HEADER,Name),8);
}

unsigned int PEFile::getSectionNumber(QString sSectionName)
{
    unsigned int nSectionsTableOffset=getSectionsTableOffset();
    unsigned int nSections=getFileHeader_NumberOfSections();

    for(int i=0; i<nSections; i++)
    {
        if(getSectionNameAsString(i,nSectionsTableOffset)==sSectionName)
        {
            return i;
        }
    }

    return -1;
}

void PEFile::setSectionNameAsString(int nSection, QString sValue)
{
    sValue.resize(8);
    setString(getSectionHeaderOffset(nSection)+offsetof(IMAGE_SECTION_HEADER,Name),sValue);
}

QList<IMAGE_SECTION_HEADER> PEFile::getSections()
{
    QList<IMAGE_SECTION_HEADER> listResult;

    IMAGE_SECTION_HEADER section;

    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nOffset=getSectionsTableOffset();

    for(int i=0; i<nNumberOfSections; i++)
    {
        if(!readArray(nOffset,(char *)&section,sizeof(section)))
        {
            break;
        }

        listResult.append(section);

        nOffset+=sizeof(IMAGE_SECTION_HEADER);
    }



    return listResult;
}

bool PEFile::dumpSection(QString sFileName, int nSection)
{
    return dump(sFileName,getSection_PointerToRawData(nSection),getSection_SizeOfRawData(nSection));
}

bool PEFile::addSection(IMAGE_SECTION_HEADER *pISH, QByteArray baData)
{
    return addSection(pISH,baData.data(),baData.size());
}

bool PEFile::addSection(IMAGE_SECTION_HEADER *pISH, char *pData, int nDataSize)
{
    int nDelta;
    PEFile tempfile;

    pISH->VirtualAddress=getNewSection_VirtualAddress();
    pISH->PointerToRawData=getNewSection_PointerToRawData();

    if(!pISH->SizeOfRawData)
    {
        pISH->SizeOfRawData=ALIGN_UP(nDataSize,getOptionalHeader_FileAlignment());
    }

    if(!pISH->Misc.VirtualSize)
    {
        pISH->Misc.VirtualSize=nDataSize;
    }

    fixSizeOfHeaders();
    unsigned int nHeadersSize=CalculateHeadersSize(getDosStubSize(),getFileHeader_SizeOfOptionalHeader(),getFileHeader_NumberOfSections()+1,getOptionalHeader_FileAlignment());

    nDelta=nHeadersSize-getOptionalHeader_SizeOfHeaders();

    if(nDelta<0)
    {
        nDelta=0;
    }

    if(!tempfile.createFile(file.fileName()+".tmp",size()+nDelta+ALIGN_UP(nDataSize,getOptionalHeader_FileAlignment())))
    {
        return false;
    }

    // Copy Dos Header

    if(!copy(&tempfile,0,this,0,getSectionsTableOffset()+getSectionsTableSize()))
    {
        return false;
    }

    if(!tempfile.writeArrayToFile(getSectionsTableOffset()+getSectionsTableSize(),(char *)pISH,sizeof(IMAGE_SECTION_HEADER)))
    {
        return false;
    }

    if(!copy(&tempfile,nHeadersSize,this,this->getOptionalHeader_SizeOfHeaders(),this->calculateRawSize()-this->getOptionalHeader_SizeOfHeaders()))
    {
        return false;
    }

    if(!tempfile.writeArrayToFile(nHeadersSize+this->calculateRawSize()-this->getOptionalHeader_SizeOfHeaders(),pData,nDataSize))
    {
        return false;
    }

    if(!copy(&tempfile,nHeadersSize+this->calculateRawSize()-this->getOptionalHeader_SizeOfHeaders()+ALIGN_UP(nDataSize,this->getOptionalHeader_FileAlignment()),this,this->getOverlayOffset(),this->getOverlaySize()))
    {
        return false;
    }


    tempfile.FixRawOffsets(nDelta);
    tempfile.setFileHeader_NumberOfSections(getFileHeader_NumberOfSections()+1);
    tempfile.setOptionalHeader_SizeOfImage(pISH->VirtualAddress+ALIGN_UP(pISH->Misc.VirtualSize,getOptionalHeader_SectionAlignment()));
    pISH->PointerToRawData+=nDelta;
    tempfile.FixCheckSum();


    if(!copy(&tempfile))
    {
        return false;
    }

    if(!tempfile.deleteFile())
    {
        return false;
    }

    return reopen();
}

bool PEFile::addSection(IMAGE_SECTION_HEADER *pISH, QString sFileName)
{
    int nDelta;
    PEFile tempfile;
    PEFile sectionfile;

    if(!sectionfile.setFileName(sFileName,false))
    {
        return false;
    }

    pISH->VirtualAddress=getNewSection_VirtualAddress();
    pISH->PointerToRawData=getNewSection_PointerToRawData();

    if(!pISH->SizeOfRawData)
    {
        pISH->SizeOfRawData=ALIGN_UP(sectionfile.size(),getOptionalHeader_FileAlignment());
    }

    if(!pISH->Misc.VirtualSize)
    {
        pISH->Misc.VirtualSize=sectionfile.size();
    }

    fixSizeOfHeaders();
    unsigned int nHeadersSize=CalculateHeadersSize(getDosStubSize(),getFileHeader_SizeOfOptionalHeader(),getFileHeader_NumberOfSections()+1,getOptionalHeader_FileAlignment());

    //    int nTest=getOptionalHeader_SizeOfHeaders();
    nDelta=nHeadersSize-getOptionalHeader_SizeOfHeaders();

    if(nDelta<0)
    {
        nDelta=0;
    }

    if(!tempfile.createFile(file.fileName()+".tmp",size()+nDelta+ALIGN_UP(sectionfile.size(),getOptionalHeader_FileAlignment())))
    {
        return false;
    }

    // Copy Dos Header

    if(!copy(&tempfile,0,this,0,getSectionsTableOffset()+getSectionsTableSize()))
    {
        return false;
    }

    if(!tempfile.writeArrayToFile(getSectionsTableOffset()+getSectionsTableSize(),(char *)pISH,sizeof(IMAGE_SECTION_HEADER)))
    {
        return false;
    }

    int nTempSize=this->calculateRawSize();
    nTempSize=nTempSize-this->getOptionalHeader_SizeOfHeaders();

    if(!copy(&tempfile,nHeadersSize,this,this->getOptionalHeader_SizeOfHeaders(),nTempSize))
    {
        return false;
    }

    if(!copy(&tempfile,nHeadersSize+this->calculateRawSize()-this->getOptionalHeader_SizeOfHeaders(),&sectionfile,0,sectionfile.size()))
    {
        return false;
    }

    if(!copy(&tempfile,nHeadersSize+this->calculateRawSize()-this->getOptionalHeader_SizeOfHeaders()+ALIGN_UP(sectionfile.size(),this->getOptionalHeader_FileAlignment()),this,this->getOverlayOffset(),this->getOverlaySize()))
    {
        return false;
    }


    tempfile.FixRawOffsets(nDelta);
    tempfile.setFileHeader_NumberOfSections(getFileHeader_NumberOfSections()+1);
    tempfile.setOptionalHeader_SizeOfImage(pISH->VirtualAddress+ALIGN_UP(pISH->Misc.VirtualSize,getOptionalHeader_SectionAlignment()));
    pISH->PointerToRawData+=nDelta;
    tempfile.FixCheckSum();


    if(!copy(&tempfile))
    {
        return false;
    }

    if(!tempfile.deleteFile())
    {
        return false;
    }

    return reopen();
}

bool PEFile::rebuild()
{

    //    _____SECTIONRECORD *pListSections;
    //    pListSections=new _____SECTIONRECORD[getFileHeader_NumberOfSections()];

    _____SECTIONRECORD pListSections[50];

    //    QList<_____SECTIONRECORD> listSections;
    //    _____SECTIONRECORD record;



    for(int i=0; i<getFileHeader_NumberOfSections(); i++)
    {
        pListSections[i].nOldOffset=getSection_PointerToRawData(i);
        pListSections[i].nOldSize=getSection_SizeOfRawData(i);
        pListSections[i].nNewOffset=pListSections[i].nOldOffset;
        pListSections[i].nNewSize=pListSections[i].nOldSize;
    }

    unsigned int nFileAlignment=getOptionalHeader_FileAlignment();
    unsigned int nSizeOfHeaders=getSectionRealSize(0,qMin(getOptionalHeader_SizeOfHeaders(),pListSections[0].nOldOffset),nFileAlignment);
    int nDelta=0;
    unsigned int nMaxSize=nSizeOfHeaders;

    setOptionalHeader_SizeOfHeaders(nMaxSize);

    for(int i=0; i<getFileHeader_NumberOfSections(); i++)
    {
        //        if(i==0)
        //        {
        //           pListSections[i].nNewOffset=nSizeOfHeaders;
        //        }
        //        else
        //        {
        //            pListSections[i].nNewOffset+=nDelta;
        //        }

        pListSections[i].nNewOffset=nMaxSize;

        pListSections[i].nNewSize=getSectionRealSize(pListSections[i].nOldOffset,pListSections[i].nOldSize,nFileAlignment);

        //        nDelta=(int)ALIGN_UP(pListSections[i].nNewSize,nFileAlignment)-(int)ALIGN_UP(pListSections[i].nOldSize,nFileAlignment);

        nMaxSize=qMax(nMaxSize,pListSections[i].nNewOffset+pListSections[i].nNewSize);
    }

    PEFile tempfile;

    if(!tempfile.createFile(file.fileName()+".tmp",nMaxSize+getOverlaySize()))
    {
        //        delete [] pListSections;
        return false;
    }

    if(!copy(&tempfile,0,this,0,qMin(nSizeOfHeaders,pListSections[0].nOldOffset)))
    {
        //        delete [] pListSections;
        return false;
    }

    for(int i=0; i<getFileHeader_NumberOfSections(); i++)
    {
        if(!copy(&tempfile,pListSections[i].nNewOffset,this,pListSections[i].nOldOffset,qMin(pListSections[i].nOldSize,pListSections[i].nNewSize)))
        {
            return false;
        }
    }

    for(int i=0; i<getFileHeader_NumberOfSections(); i++)
    {
        tempfile.setSection_PointerToRawData(i,pListSections[i].nNewOffset);
        tempfile.setSection_SizeOfRawData(i,pListSections[i].nNewSize);
    }

    if(isOverlayPresent())
    {
        if(!copy(&tempfile,nMaxSize,this,getOverlayOffset(),getOverlaySize()))
        {
            return false;
        }
    }

    // fix checksum
    tempfile.FixCheckSum();


    if(!copy(&tempfile))
    {
        //        delete [] pListSections;
        return false;
    }

    if(!tempfile.deleteFile())
    {
        //        delete [] pListSections;
        return false;
    }

    //    delete [] pListSections;

    return reopen();

}

unsigned int PEFile::getSectionRealSize(unsigned int nOffset, unsigned int nSize, unsigned int nAlignment)
{
    // GetRealSize;

    if(nAlignment>=nSize)
    {
        return nAlignment;
    }

    unsigned int nAlignedSize=ALIGN_UP(nSize,nAlignment);
    QByteArray baTemp;
    unsigned int nTemp;

    for(int i=nAlignedSize-nAlignment; i>=0; i-=nAlignment)
    {
        if(i<nSize)
        {
            nTemp=qMax(nAlignment,nSize-i);

            baTemp=readArray(nOffset+i,nTemp);

            for(int j=0; j<nTemp; j++)
            {
                if(*(baTemp.data()+j))
                {
                    return i+nAlignment;
                }
            }
        }

    }

    return 0;
}

bool PEFile::deleteLastSection()
{
    if(getFileHeader_NumberOfSections()==0)
    {
        emit appendError("No sections present");
        return false;
    }

    int nDelta;
    PEFile tempfile;

    fixSizeOfHeaders();
    unsigned int nHeadersSize=CalculateHeadersSize(getDosStubSize(),getFileHeader_SizeOfOptionalHeader(),getFileHeader_NumberOfSections()-1,getOptionalHeader_FileAlignment());

    nDelta=nHeadersSize-getOptionalHeader_SizeOfHeaders();


    int nTemp=ALIGN_UP(getSection_SizeOfRawData(getFileHeader_NumberOfSections()-1),getOptionalHeader_FileAlignment());

    if(!tempfile.createFile(file.fileName()+".tmp",size()+nDelta-nTemp))
    {
        return false;
    }

    // Copy Dos Header

    if(!copy(&tempfile,0,this,0,getSectionsTableOffset()+getSectionsTableSize()-sizeof(IMAGE_SECTION_HEADER)))
    {
        return false;
    }

    if(!copy(&tempfile,nHeadersSize,this,this->getOptionalHeader_SizeOfHeaders(),this->getSection_PointerToRawData(getFileHeader_NumberOfSections()-2)-this->getOptionalHeader_SizeOfHeaders()))
    {
        return false;
    }

    if(!copy(&tempfile,this->calculateRawSize()+nDelta-nTemp,this,getOverlayOffset(),getOverlaySize()))
    {
        return false;
    }


    tempfile.FixRawOffsets(nDelta);
    tempfile.setFileHeader_NumberOfSections(getFileHeader_NumberOfSections()-1);
    tempfile.setOptionalHeader_SizeOfImage(getSection_VirtualSize(getFileHeader_NumberOfSections()-2)+getSection_VirtualAddress(getFileHeader_NumberOfSections()-2));
    tempfile.FixCheckSum();


    if(!copy(&tempfile))
    {
        return false;
    }

    if(!tempfile.deleteFile())
    {
        return false;
    }

    return reopen();
}

unsigned long long PEFile::OffsetToRVA(unsigned long long nOffset)
{
    //#ifdef QT_DEBUG
    //    qDebug("PEFile::OffsetToRVA");
    //#endif
    unsigned long long nResult=-1;

    if(nOffset<=calculateRawSize())
    {
        if(nOffset<getOptionalHeader_SizeOfHeaders())
        {
            return nOffset;
        }
        else
        {
            for(int i=0; i<getFileHeader_NumberOfSections(); i++)
            {
                int nPointerToRawData=getSection_PointerToRawData(i);

                if(nPointerToRawData)
                {
                    if((nPointerToRawData<=nOffset)&&(nOffset<nPointerToRawData+getSection_SizeOfRawData(i)))
                    {
                        nResult=getSection_VirtualAddress(i)+nOffset-nPointerToRawData;
                    }
                }
            }

            if(nResult!=-1) // new
            {
                return nResult;
            }
        }
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,8,16,QChar('0')));
    return -1;
}

unsigned int PEFile::OffsetToVA32(unsigned int nOffset)
{
    unsigned int nResult=OffsetToRVA(nOffset);

    if(nResult!=-1)
    {
        nResult+=getOptionalHeader_ImageBase();
    }

    return nResult;
}

unsigned long long PEFile::OffsetToVA64(unsigned int nOffset)
{
    unsigned long long nResult=OffsetToRVA(nOffset);

    if(nResult!=-1)
    {
        nResult+=getOptionalHeader_ImageBase64();
    }

    return nResult;
}

unsigned long long PEFile::RVAToOffset(unsigned long long nRVA)
{
    //#ifdef QT_DEBUG
    //    qDebug("PEFile::RVAToOffset");
    //#endif
    unsigned int nFileAlignment=0;

    if(getOptionalHeader_FileAlignment()==getOptionalHeader_SectionAlignment())
    {
        nFileAlignment=1;
    }
    else
    {
        nFileAlignment=getOptionalHeader_FileAlignment();
    }

    if(nRVA<=getOptionalHeader_SizeOfImage())
    {
        if(nRVA<getOptionalHeader_SizeOfHeaders())
        {
            return nRVA;
        }
        else
        {
            for(int i=0; i<getFileHeader_NumberOfSections(); i++)
            {
                int nPointerToRawData=getSection_PointerToRawData(i);

                if(nPointerToRawData)
                {
                    if((getSection_VirtualAddress(i)<=nRVA)&&(nRVA<getSection_VirtualAddress(i)+getSection_SizeOfRawData(i)))
                    {
                        //                    int nTest=nRVA-getSection_VirtualAddress(i);
                        //                    int nTest2=getSection_PointerToRawData(i);
                        //                    int nTest3=ALIGN_DOWN(getSection_PointerToRawData(i),nFileAlignment)+nRVA-getSection_VirtualAddress(i);
                        return ALIGN_DOWN(nPointerToRawData,nFileAlignment)+nRVA-getSection_VirtualAddress(i);
                    }
                }

            }
        }
    }

    emit appendWarning(QString("Invalid RVA: %1").arg(nRVA,8,16,QChar('0')));
    return -1;
}

unsigned int PEFile::VAToOffset32(unsigned int nVA)
{
    return RVAToOffset(nVA-getOptionalHeader_ImageBase());
}

unsigned int PEFile::VAToOffset64(unsigned long long nVA)
{
    return RVAToOffset(nVA-getOptionalHeader_ImageBase64());
}

unsigned long long PEFile::VAToOffset(unsigned long long nVA)
{
    unsigned long long nResult=0;

    if(isPEPlus())
    {
        nResult=VAToOffset64(nVA);
    }
    else
    {
        nResult=VAToOffset32(nVA);
    }

    if(nResult==0xFFFFFFFF)
    {
        nResult=-1;
    }

    return nResult;
}

unsigned long long PEFile::OffsetToVA(unsigned long long nOffset)
{
    unsigned long long nResult=0;

    if(isPEPlus())
    {
        nResult=OffsetToVA64(nOffset);
    }
    else
    {
        nResult=OffsetToVA32(nOffset);
    }

    if(nResult==0xFFFFFFFF)
    {
        nResult=-1;
    }

    return nResult;
}

unsigned long long PEFile::VAToRVA(unsigned long long nVA)
{
    unsigned long long nRVA=0;

    if(isPEPlus())
    {
        nRVA=nVA-getOptionalHeader_ImageBase64();
    }
    else
    {
        nRVA=nVA-getOptionalHeader_ImageBase();
    }

    if(!isRVAValid(nRVA))
    {
        return -1;
    }

    return nRVA;
}

unsigned long long PEFile::RVAToVA(unsigned long long nRVA)
{
    if(isRVAValid(nRVA))
    {
        if(isPEPlus())
        {
            return nRVA+getOptionalHeader_ImageBase64();
        }
        else
        {
            return nRVA+getOptionalHeader_ImageBase();
        }
    }

    return -1;
}

unsigned long long PEFile::getMinimalVA()
{
    if(isPEPlus())
    {
        return getOptionalHeader_ImageBase64();
    }
    else
    {
        return getOptionalHeader_ImageBase();
    }
}

unsigned long long PEFile::getMaximalVA()
{
    if(isPEPlus())
    {
        return getOptionalHeader_ImageBase64()+ALIGN_UP(getOptionalHeader_SizeOfImage(),getOptionalHeader_SectionAlignment());
    }
    else
    {
        return getOptionalHeader_ImageBase()+ALIGN_UP(getOptionalHeader_SizeOfImage(),getOptionalHeader_SectionAlignment());
    }
}

unsigned int PEFile::RVAToSection(unsigned int nRVA)
{

    for(int i=0; i<getFileHeader_NumberOfSections(); i++)
    {
        if((getSection_VirtualAddress(i)<=nRVA)&&(nRVA<getSection_VirtualAddress(i)+(getSection_VirtualSize(i)?getSection_VirtualSize(i):getSection_SizeOfRawData(i))))
        {
            return i;
        }
    }

    emit appendWarning(QString("Invalid RVA: %1").arg(nRVA,8,16,QChar('0')));
    return -1;
}

unsigned int PEFile::getEntryPointSection()
{
    return RVAToSection(getOptionalHeader_AddressOfEntryPoint());
}

unsigned int PEFile::calculateRawSize()
{
    //    unsigned int nSize=getOptionalHeader_SizeOfHeaders();
    unsigned int nSize=calculateSizeOfHeaders();

    if((int)nSize>0)
    {
        unsigned int nSectionOffset=0;
        unsigned int nSectionSize=0;

        for(int i=0; i<getFileHeader_NumberOfSections(); i++)
        {
            nSectionOffset=getSection_PointerToRawData(i);
            nSectionSize=getSection_SizeOfRawData(i);

            //        nSize=MAXIMAL(nSize,getSection_PointerToRawData(i)+ALIGN_UP(getSection_SizeOfRawData(i),getOptionalHeader_FileAlignment()));
            if(nSectionSize&&Binary::isOffsetValid(nSectionOffset))
            {
                nSize=MAXIMAL(nSize,nSectionOffset+nSectionSize);
            }
        }
    }
    else
    {
        nSize=___nSize;
    }

    return nSize;
}



unsigned int PEFile::getDataDirectoriesOffset()
{
    if(isPEPlus())
    {
        return (getNtHeadersOffset()+sizeof(IMAGE_NT_HEADERS64)-16*sizeof(IMAGE_DATA_DIRECTORY));
    }

    return (getNtHeadersOffset()+sizeof(IMAGE_NT_HEADERS)-16*sizeof(IMAGE_DATA_DIRECTORY));
}

unsigned int PEFile::getDataDirectoriesSize()
{
    return getOptionalHeader_NumberOfRvaAndSizes()*sizeof(IMAGE_DATA_DIRECTORY);
}

unsigned int PEFile::getDosStubSize()
{
    int nSize=(int)getNtHeadersOffset()-(int)getDosStubOffset();

    return MAXIMAL(0,nSize);
}

unsigned int PEFile::getDosStubOffset()
{
    return sizeof(IMAGE_DOS_HEADER);
}

bool PEFile::isDosStubPresent()
{
    return getDosStubSize()!=0;
}

bool PEFile::isRichSignaturePresent()
{
    bool bResult=false;
    int nSize=getDosStubSize();
    char *pBuffer=new char[nSize+1];
    QByteArray baStub;

    readArray(getDosStubOffset(),pBuffer,nSize);

    baStub.setRawData(pBuffer,nSize);

    bResult=baStub.contains("Rich");

    delete[] pBuffer;

    return bResult;
}

QList<PEFile::RICH_RECORD> PEFile::getRichSignatureRecords()
{
    QList<PEFile::RICH_RECORD> listResult;

    if(isRichSignaturePresent())
    {
        //       QByteArray baRich=readArray(getDosStubOffset(),getDosStubSize());

        unsigned int nOffset=findString(getDosStubOffset(),getDosStubSize(),"Rich");

        if(nOffset!=-1)
        {
            unsigned int nXORkey=readDword(nOffset+4);

            unsigned int nCurrentOffset=nOffset-4;

            while(nCurrentOffset>getDosStubOffset())
            {
                unsigned int nTemp=readDword(nCurrentOffset)^nXORkey;

                if(nTemp==0x536e6144)
                {
                    nCurrentOffset+=16;

                    for(; nCurrentOffset<nOffset; nCurrentOffset+=8)
                    {
                        PEFile::RICH_RECORD record;

                        nTemp=readDword(nCurrentOffset)^nXORkey;
                        record.nId=nTemp>>16;
                        record.nVersion=nTemp&0xFFFF;
                        nTemp=readDword(nCurrentOffset+4)^nXORkey;
                        record.nCount=nTemp;

                        listResult.append(record);
                    }

                    break;
                }

                nCurrentOffset-=4;
            }
        }
    }

    return listResult;
}

unsigned int PEFile::getNumberOfRichIDs()
{
    unsigned int nResult=0;
    QList<PEFile::RICH_RECORD> list=getRichSignatureRecords();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).nId)
        {
            nResult++;
        }
    }

    return nResult;
}

bool PEFile::isRichVersionPresent(unsigned int nVersion)
{
    QList<PEFile::RICH_RECORD> list=getRichSignatureRecords();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).nVersion==nVersion)
        {
            return true;
        }
    }

    return false;
}

bool PEFile::addDosStub(QByteArray &baData)
{
    return addDosStub(baData.data(),baData.size());
}

bool PEFile::addDosStub(char *pData, int nDataSize)
{
    int nDelta;
    unsigned int nLfanew;
    PEFile tempfile;

    unsigned int nHeadersSize=CalculateHeadersSize(nDataSize,getFileHeader_SizeOfOptionalHeader(),getFileHeader_NumberOfSections(),getOptionalHeader_FileAlignment());

    nDelta=nHeadersSize-getOptionalHeader_SizeOfHeaders();

    if(!tempfile.createFile(file.fileName()+".tmp",size()+nDelta))
    {
        return false;
    }

    // Copy Dos Header

    if(!copy(&tempfile,tempfile.getDosHeaderOffset(),this,this->getDosHeaderOffset(),this->getDosHeaderSize()))
    {
        return false;
    }

    if(!tempfile.writeArrayToFile(tempfile.getDosStubOffset(),pData,nDataSize))
    {
        return false;
    }

    if(!copy(&tempfile,ALIGN_UP((tempfile.getDosHeaderSize()+nDataSize),4),this,this->getNtHeadersOffset(),this->getSectionsTableOffset()+this->getSectionsTableSize()-this->getNtHeadersOffset()))
    {
        return false;
    }

    if(!copy(&tempfile,nHeadersSize,this,this->getOptionalHeader_SizeOfHeaders(),size()-this->getOptionalHeader_SizeOfHeaders()))
    {
        return false;
    }

    nLfanew=ALIGN_UP((tempfile.getDosHeaderSize()+nDataSize),4);
    //emit appendError(QString("nLfanew=%1").arg(nLfanew));
    tempfile.setDosHeader_lfanew(nLfanew);
    tempfile.FixRawOffsets(nDelta);
    tempfile.FixCheckSum();

    if(!copy(&tempfile))
    {
        return false;
    }

    if(!tempfile.deleteFile())
    {
        return false;
    }

    return true;
}

bool PEFile::addDosStub(QString sFileName)
{
    int nDelta;
    unsigned int nLfanew;
    PEFile tempfile;
    PEFile stubfile;

    if(!stubfile.setFileName(sFileName,false))
    {
        return false;
    }

    unsigned int nHeadersSize=CalculateHeadersSize(stubfile.size(),getFileHeader_SizeOfOptionalHeader(),getFileHeader_NumberOfSections(),getOptionalHeader_FileAlignment());

    nDelta=nHeadersSize-getOptionalHeader_SizeOfHeaders();

    if(!tempfile.createFile(file.fileName()+".tmp",size()+nDelta))
    {
        return false;
    }

    // Copy Dos Header

    if(!copy(&tempfile,tempfile.getDosHeaderOffset(),this,this->getDosHeaderOffset(),this->getDosHeaderSize()))
    {
        return false;
    }

    //    if(!tempfile.writeArrayToFile(tempfile.getDosStubOffset(),pData,nDataSize))
    //    {
    //        return false;
    //    }
    if(!copy(&tempfile,tempfile.getDosStubOffset(),&stubfile,0,stubfile.size()))
    {
        return false;
    }

    if(!copy(&tempfile,ALIGN_UP((tempfile.getDosHeaderSize()+stubfile.size()),4),this,this->getNtHeadersOffset(),this->getSectionsTableOffset()+this->getSectionsTableSize()-this->getNtHeadersOffset()))
    {
        return false;
    }

    if(!copy(&tempfile,nHeadersSize,this,this->getOptionalHeader_SizeOfHeaders(),size()-this->getOptionalHeader_SizeOfHeaders()))
    {
        return false;
    }

    nLfanew=ALIGN_UP((tempfile.getDosHeaderSize()+stubfile.size()),4);
    //emit appendError(QString("nLfanew=%1").arg(nLfanew));
    tempfile.setDosHeader_lfanew(nLfanew);
    tempfile.FixRawOffsets(nDelta);
    tempfile.FixCheckSum();

    if(!copy(&tempfile))
    {
        return false;
    }

    if(!tempfile.deleteFile())
    {
        return false;
    }

    return true;
}

bool PEFile::DeleteDosStub()
{
    if(isDosStubPresent())
    {
        addDosStub(0,0);
    }

    return false;
}
int PEFile::CalculateHeadersSize(int nDosStubSize, int nSizeOfOptionalHeader, int nNumberOfSection, int nFileAlignment)
{
    return ALIGN_UP((sizeof(IMAGE_DOS_HEADER)+ALIGN_UP(nDosStubSize,4)+4+sizeof(IMAGE_FILE_HEADER)+nSizeOfOptionalHeader+nNumberOfSection*sizeof(IMAGE_SECTION_HEADER)),nFileAlignment);
}

unsigned int PEFile::CalculateCheckSum()
{
    unsigned int nCalcSum=CheckSum(0,size());
    unsigned int nHdrSum=getOptionalHeader_CheckSum();

    if(LOWORD(nCalcSum)>=LOWORD(nHdrSum))
    {
        nCalcSum-=LOWORD(nHdrSum);
    }
    else
    {
        nCalcSum=((LOWORD(nCalcSum)-LOWORD(nHdrSum))&0xFFFF)-1;
    }

    if(LOWORD(nCalcSum)>=HIWORD(nHdrSum)) //!!!!!
    {
        nCalcSum-=HIWORD(nHdrSum);
    }
    else
    {
        nCalcSum=((LOWORD(nCalcSum)-HIWORD(nHdrSum))&0xFFFF)-1;
    }

    nCalcSum+=this->size();

    return nCalcSum;
}

unsigned short PEFile::CheckSum(int nStartValue,int nDataSize)
{
    int nSum=nStartValue;
    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE];
    char *pOffset;

    while(nDataSize>0)
    {
        nTemp=MINIMAL(BUFFER_SIZE,nDataSize);

        if(!readArrayFromFile(nStartValue,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Copy error");

            return false;
        }

        pOffset=pBuffer;

        for(int i=0; i<(nTemp+1)/2; i++)
        {
            nSum+=*((unsigned short *)pOffset);

            if(HIWORD(nSum)!=0)
            {
                nSum=LOWORD(nSum)+HIWORD(nSum);
            }

            pOffset+=2;
        }

        nDataSize-=nTemp;
        nStartValue+=nTemp;
    }

    delete[] pBuffer;

    return (unsigned short)(LOWORD(nSum)+HIWORD(nSum));
}

bool PEFile::dumpDosStub(QString sFileName)
{
    return dump(sFileName,getDosStubOffset(),getDosStubSize());
}

unsigned int PEFile::getOverlaySize()
{
    return MAXIMAL((long long)(this->size())-(long long)calculateRawSize(),0);
}

unsigned int PEFile::getOverlayOffset()
{
    return calculateRawSize();
}
bool PEFile::isOverlayPresent()
{
    return getOverlaySize()!=0;
}

bool PEFile::isSignedFile()
{
    IMAGE_DATA_DIRECTORY idd;
    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_SECURITY, &idd);

    if(isOffsetAndSizeValid(idd.VirtualAddress,idd.Size))
    {
        unsigned int nMagic=readDword(idd.VirtualAddress+4);

        if(nMagic==0x00020200)
        {
            return true;
        }
    }

    return false;
}

bool PEFile::addOverlay(QByteArray &baData)
{
    return addOverlay(baData.data(),baData.size());
}

bool PEFile::addOverlay(char *pData, int nDataSize)
{
    if(resize(calculateRawSize()+nDataSize))
    {
        if(!writeArrayToFile(calculateRawSize(),pData,nDataSize))
        {
            return false;
        }

        FixCheckSum();

        return reopen();
    }

    emit appendError("Cannot resize file");
    return false;
}

bool PEFile::addOverlay(QString sFileName)
{
    PEFile overlayfile;

    if(!overlayfile.setFileName(sFileName,false))
    {
        return false;
    }

    resize(calculateRawSize()+overlayfile.size());

    if(!copy(this,calculateRawSize(),&overlayfile,0,overlayfile.size()))
    {
        return false;
    }

    FixCheckSum();

    return reopen();
}

bool PEFile::deleteOverlay()
{
    return addOverlay(0,0);
}

bool PEFile::dumpOverlay(QString sFileName)
{
    return dump(sFileName,getOverlayOffset(),getOverlaySize());
}

unsigned int PEFile::getExportTableOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError("Export is not present");
    return 0;
}

bool PEFile::isImportPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_IMPORT);
}

bool PEFile::isRelocsPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_BASERELOC);
}

bool PEFile::isTLSPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_TLS);
}

bool PEFile::isDebugPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_DEBUG);
}

bool PEFile::isBoundImportPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);
}

bool PEFile::isExportPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_EXPORT);
}

bool PEFile::isResourcePresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_RESOURCE);
}

bool PEFile::isLoadConfigPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
}

bool PEFile::isNETPresent()
{
    return isDirectoryPresent(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR);
}

bool PEFile::isManifestPresent()
{
    return isResourceIdPresent(24);
}



unsigned int PEFile::getImportSection()
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    IMAGE_DATA_DIRECTORY idd;
    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT,&idd);

    return RVAToSection(idd.VirtualAddress);
}

unsigned int PEFile::getExportSection()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    IMAGE_DATA_DIRECTORY idd;
    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT,&idd);

    return RVAToSection(idd.VirtualAddress);
}

unsigned int PEFile::getImportTableOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendWarning("Import is not present");
    return 0;
}
unsigned int PEFile::getResourceOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_RESOURCE,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError("Resource is not present");
    return 0;
}
unsigned int PEFile::getNumberOfImports()
{
    unsigned int nOffset;
    unsigned int nResult=0;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    unsigned int nThunk;
    unsigned int nName;

    nOffset=getImportTableOffset();

    if((nOffset!=-1)&&(nOffset!=0))
    {
        nThunk=readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk));

        //        if(readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk)))
        if((nThunk)&&(isRVAPresentInFile(nThunk)))
        {
            //            while(readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk))&&readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,Name)))
            //            {
            //                nResult++;
            //                nOffset+=sizeof(IMAGE_IMPORT_DESCRIPTOR);
            //            }

            while(true)
            {
                if(!isOffsetValid(nOffset))
                {
                    break;
                }

                nThunk=readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk));
                nName=readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,Name));

                if(!nThunk)
                {
                    break;
                }

                if(!nName)
                {
                    break;
                }

                if(!isRVAPresentInFile(nThunk))
                {
                    break;
                }

                if(!isRVAPresentInFile(nName))
                {
                    break;
                }

                nResult++;

                if(nResult>50)
                {
                    break;
                }

                nOffset+=sizeof(IMAGE_IMPORT_DESCRIPTOR);
            }
        }
        else
        {
            while(true)
            {
                if(!isOffsetValid(nOffset))
                {
                    break;
                }

                nThunk=readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,FirstThunk));
                nName=readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,Name));

                if(!nThunk)
                {
                    break;
                }

                if(!nName)
                {
                    break;
                }

                if(!isRVAPresentInFile(nThunk))
                {
                    break;
                }

                if(!isRVAPresentInFile(nName))
                {
                    break;
                }

                nResult++;

                if(nResult>50)
                {
                    break;
                }

                nOffset+=sizeof(IMAGE_IMPORT_DESCRIPTOR);
            }

            //            while(readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,FirstThunk))&&readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,Name)))
            //            {
            //                nResult++;
            //                nOffset+=sizeof(IMAGE_IMPORT_DESCRIPTOR);
            //            }
        }

        return nResult;
    }

    emit appendWarning("Import is not present");
    return 0;
}

QList<IMAGE_IMPORT_DESCRIPTOR> PEFile::getImports()
{
    QList<IMAGE_IMPORT_DESCRIPTOR> listResult;

    IMAGE_IMPORT_DESCRIPTOR import;

    unsigned int nNumberOfImports=getNumberOfImports();
    unsigned int nOffset=getImportTableOffset();

    for(int i=0; i<nNumberOfImports; i++)
    {
        if(!readArray(nOffset,(char *)&import,sizeof(import)))
        {
            break;
        }

        listResult.append(import);

        nOffset+=sizeof(import);
    }



    return listResult;
}

QList<QString> PEFile::getImportLibraiesAsNames()
{
    QList<QString> listResult;
    QString sRecord;

    unsigned int nNumberOfImports=getNumberOfImports();

    for(int i=0; i<nNumberOfImports; i++)
    {
        sRecord=getImport_NameAsString(i).toLower();

        if(listResult.indexOf(sRecord)==-1)
        {
            listResult.append(sRecord);
        }


    }

    return listResult;
}

QList<QString> PEFile::getImportFunctionsAsNames()
{
    QList<QString> listResult;
    QString sRecord;
    QString sFunction;

    unsigned int nNumberOfImports=getNumberOfImports();
    unsigned int nNumberOfFunctions=0;

    for(int i=0; i<nNumberOfImports; i++)
    {
        sRecord=QString("[%1]").arg(getImport_NameAsString(i).toLower());

        nNumberOfFunctions=getNumberOfImportThunks(i);

        for(int j=0; j<nNumberOfFunctions; j++)
        {
            sFunction=getImportFunctionName(i,j);

            if(sFunction=="")
            {
                if(isPEPlus())
                {

                    sFunction=QString("%1").arg(getImportThunk64(i,j),0,16,QChar('0'));
                }
                else
                {
                    sFunction=QString("%1").arg(getImportThunk(i,j),0,16,QChar('0'));
                }
            }

            if(listResult.indexOf(sRecord+sFunction)==-1)
            {
                listResult.append(sRecord+sFunction);
            }
        }
    }

    return listResult;
}

QList<PEFile::IMPORT_LIST> PEFile::getImportFunctionsAsList()
{
    QList<IMPORT_LIST> listResult;
    QString sRecord;
    QString sFunction;

    unsigned int nNumberOfImports=getNumberOfImports();
    unsigned int nNumberOfFunctions=0;

    IMPORT_LIST record;

    for(int i=0; i<nNumberOfImports; i++)
    {
        sRecord=QString("[%1]").arg(getImport_NameAsString(i).toLower());

        nNumberOfFunctions=getNumberOfImportThunks(i);

        for(int j=0; j<nNumberOfFunctions; j++)
        {
            sFunction=getImportFunctionName(i,j);

            if(sFunction=="")
            {
                if(isPEPlus())
                {
                    sFunction=QString("%1").arg(getImportThunk64(i,j),0,16,QChar('0'));
                }
                else
                {
                    sFunction=QString("%1").arg(getImportThunk(i,j),0,16,QChar('0'));
                }
            }

            record.sName=sRecord+sFunction;

            if(isPEPlus())
            {
                record.nThunkRelAddress=getImportThunkAddress64(i,j);
                record.nThunkAddress=record.nThunkRelAddress+getOptionalHeader_ImageBase64();
                record.nThunkOffset=RVAToOffset(record.nThunkRelAddress);
            }
            else
            {
                record.nThunkRelAddress=getImportThunkAddress(i,j);
                record.nThunkAddress=record.nThunkRelAddress+getOptionalHeader_ImageBase();
                record.nThunkOffset=RVAToOffset(record.nThunkRelAddress);
            }

            listResult.append(record);
        }
    }

    return listResult;
}

QList<QString> PEFile::getImportLibraryFunctionsAsNames(QString sLibraryName)
{
    QList<QString> listResult;
    QString sFunction;

    unsigned int nNumberOfImports=getNumberOfImports();
    unsigned int nNumberOfFunctions=0;

    for(int i=0; i<nNumberOfImports; i++)
    {
        if(getImport_NameAsString(i).toLower()==sLibraryName)
        {
            nNumberOfFunctions=getNumberOfImportThunks(i);

            for(int j=0; j<nNumberOfFunctions; j++)
            {
                sFunction=getImportFunctionName(i,j);

                if(sFunction=="")
                {
                    if(isPEPlus())
                    {

                        sFunction=QString("%1").arg(getImportThunk64(i,j),0,16,QChar('0'));
                    }
                    else
                    {
                        sFunction=QString("%1").arg(getImportThunk(i,j),0,16,QChar('0'));
                    }
                }


                if(listResult.indexOf(sFunction)==-1)
                {
                    listResult.append(sFunction);
                }

            }

            break;
        }
    }

    return listResult;
}

unsigned int PEFile::getImport_OriginalFirstThunk(unsigned int nImport)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        return readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}
unsigned int PEFile::getImport_OriginalFirstThunk(unsigned int nImport,unsigned int nImportTableOffset)
{
    return readDword(nImportTableOffset+nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR)+offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk));
}

unsigned int PEFile::getImport_TimeDateStamp(unsigned int nImport)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        return readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,TimeDateStamp));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}
unsigned int PEFile::getImport_TimeDateStamp(unsigned int nImport,unsigned int nImportTableOffset)
{
    return readDword(nImportTableOffset+nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR)+offsetof(IMAGE_IMPORT_DESCRIPTOR,TimeDateStamp));
}

unsigned int PEFile::getImport_ForwarderChain(unsigned int nImport)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        return readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,ForwarderChain));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}
unsigned int PEFile::getImport_ForwarderChain(unsigned int nImport,unsigned int nImportTableOffset)
{
    return readDword(nImportTableOffset+nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR)+offsetof(IMAGE_IMPORT_DESCRIPTOR,ForwarderChain));
}

unsigned int PEFile::getImport_Name(unsigned int nImport)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        return readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,Name));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}
unsigned int PEFile::getImport_Name(unsigned int nImport,unsigned int nImportTableOffset)
{
    return readDword(nImportTableOffset+nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR)+offsetof(IMAGE_IMPORT_DESCRIPTOR,Name));
}

unsigned int PEFile::getImport_FirstThunk(unsigned int nImport)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        return readDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,FirstThunk));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}
unsigned int PEFile::getImport_FirstThunk(unsigned int nImport,unsigned int nImportTableOffset)
{
    return readDword(nImportTableOffset+nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR)+offsetof(IMAGE_IMPORT_DESCRIPTOR,FirstThunk));
}

void PEFile::setImport_OriginalFirstThunk(unsigned int nImport, unsigned int nValue)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        writeDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk),nValue);

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
}

void PEFile::setImport_TimeDateStamp(unsigned int nImport, unsigned int nValue)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        writeDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,TimeDateStamp),nValue);

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
}

void PEFile::setImport_ForwarderChain(unsigned int nImport, unsigned int nValue)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        writeDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,ForwarderChain),nValue);

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
}

void PEFile::setImport_Name(unsigned int nImport, unsigned int nValue)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        writeDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,Name),nValue);

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
}

void PEFile::setImport_FirstThunk(unsigned int nImport, unsigned int nValue)
{
    unsigned int nOffset;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=getImportTableOffset();
        nOffset+=nImport*sizeof(IMAGE_IMPORT_DESCRIPTOR);

        writeDword(nOffset+offsetof(IMAGE_IMPORT_DESCRIPTOR,FirstThunk),nValue);

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
}

QString PEFile::getImport_NameAsString(unsigned int nImport)
{
    return getAnsiString(RVAToOffset(getImport_Name(nImport)));
}
QString PEFile::getImport_NameAsString(unsigned int nImport,unsigned int nImportTableOffset)
{
    return getAnsiString(RVAToOffset(getImport_Name(nImport,nImportTableOffset)));
}

bool PEFile::isLibraryPresent(QString sLibraryName)
{
    QString sTemp;
    unsigned int nNumberOfImports;
    unsigned int nImportTableOffset;

    nNumberOfImports=getNumberOfImports();
    nImportTableOffset=getImportTableOffset();
    sTemp=sLibraryName.toLower();

    for(int i=0; i<nNumberOfImports; i++)
    {
        if(getImport_NameAsString(i,nImportTableOffset).toLower()==sTemp)
        {
            return true;
        }
    }

    return false;
}

bool PEFile::isLibraryFunctionPresent(QString sLibraryName, QString sFunctionName)
{
    QList<QString> listFunctions=getImportLibraryFunctionsAsNames(sLibraryName);


    return (listFunctions.indexOf(sFunctionName)!=-1);
}

bool PEFile::isSectionNamePresent(QString sSectionName)
{
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nSectionsTableOffset=getSectionsTableOffset();
    QString sTemp=sSectionName.toLower();

    for(int i=0; i<nNumberOfSections; i++)
    {
        if(getSectionNameAsString(i,nSectionsTableOffset).toLower()==sTemp)
        {
            return true;
        }
    }

    return false;
}
unsigned int PEFile::getNumberOfImportThunks(unsigned int nImport)
{
    //#ifndef QT_NO_DEBUG
    //    QTime scanTime=QTime::currentTime();
    //#endif



    unsigned int nOffset=0;
    unsigned int nResult=0;
    unsigned int nRVA=0;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        nRVA=getImport_OriginalFirstThunk(nImport);

        //#ifndef QT_NO_DEBUG
        //   qDebug("getNumberOfImportThunks1: %d msec",scanTime.msecsTo(QTime::currentTime()));
        //#endif


        if(!nRVA)
        {
            nRVA=getImport_FirstThunk(nImport);
        }

        nOffset=RVAToOffset(nRVA);

        if(isPEPlus())
        {
            while(readQword(nOffset))
            {
                nResult++;
                nOffset+=8;
            }
        }
        else
        {
            while(readDword(nOffset))
            {
                nResult++;
                nOffset+=4;
            }
        }

        //#ifndef QT_NO_DEBUG
        //   qDebug("getNumberOfImportThunks2: %d msec",scanTime.msecsTo(QTime::currentTime()));
        //#endif

        return nResult;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}

unsigned int PEFile::getNumberOfImportThunks(QString sLibraryName)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    int nNumberOfImports=getNumberOfImports();

    for(int i=0; i<nNumberOfImports; i++)
    {
        if(getImport_NameAsString(i)==sLibraryName)
        {
            return getNumberOfImportThunks(i);
        }
    }

    emit appendError(QString("Invalid string: %1").arg(sLibraryName));

    return 0;
}

unsigned int PEFile::getImportThunk(unsigned int nImport,unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        if(nFunctionNumber<getNumberOfImportThunks(nImport))
        {
            if(getImport_OriginalFirstThunk(nImport))
            {
                return readDword(RVAToOffset(getImport_OriginalFirstThunk(nImport))+4*nFunctionNumber);
            }
            else
            {
                return readDword(RVAToOffset(getImport_FirstThunk(nImport))+4*nFunctionNumber);
            }
        }

        emit appendError(QString("Value out of range: n=%1").arg(nFunctionNumber));
        return 0;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
    return 0;
}

unsigned int PEFile::getImportThunk(QString sLibraryName,unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    int nImport=getImportLibraryNumber(sLibraryName);

    if(nImport!=-1)
    {
        return getImportThunk(nImport,nFunctionNumber);
    }

    return 0;
}

unsigned long long PEFile::getImportThunk64(unsigned int nImport,unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        if(nFunctionNumber<getNumberOfImportThunks(nImport))
        {
            if(getImport_OriginalFirstThunk(nImport))
            {
                return readQword(RVAToOffset(getImport_OriginalFirstThunk(nImport))+8*nFunctionNumber);
            }
            else
            {
                return readQword(RVAToOffset(getImport_FirstThunk(nImport))+8*nFunctionNumber);
            }
        }

        emit appendError(QString("Value out of range: n=%1").arg(nFunctionNumber));
        return 0;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
    return 0;
}
unsigned long long PEFile::getImportThunk64(unsigned int nOriginalFirstThunk,unsigned int nFirstThunk,unsigned int nFunctionNumber)
{
    if(nOriginalFirstThunk)
    {
        return readQword(nOriginalFirstThunk+8*nFunctionNumber);
    }
    else
    {
        return readQword(nFirstThunk+8*nFunctionNumber);
    }

}
unsigned int PEFile::getImportThunk(unsigned int nOriginalFirstThunk,unsigned int nFirstThunk,unsigned int nFunctionNumber)
{
    if(nOriginalFirstThunk)
    {
        return readDword(nOriginalFirstThunk+4*nFunctionNumber);
    }
    else
    {
        return readDword(nFirstThunk+4*nFunctionNumber);
    }

}

unsigned long long PEFile::getImportThunk64(QString sLibraryName,unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    int nImport=getImportLibraryNumber(sLibraryName);

    if(nImport!=-1)
    {
        return getImportThunk64(nImport,nFunctionNumber);
    }

    return 0;
}

unsigned int PEFile::getImportThunkAddress(unsigned int nImport, unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        if(nFunctionNumber<getNumberOfImportThunks(nImport))
        {
            return getImport_FirstThunk(nImport)+4*nFunctionNumber;
        }

        emit appendError(QString("Value out of range: n=%1").arg(nFunctionNumber));
        return 0;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
    return 0;
}

unsigned long long PEFile::getImportThunkAddress64(unsigned int nImport, unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        if(nFunctionNumber<getNumberOfImportThunks(nImport))
        {
            return getImport_FirstThunk(nImport)+8*nFunctionNumber;
        }

        emit appendError(QString("Value out of range: n=%1").arg(nFunctionNumber));
        return 0;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
    return 0;
}

QString PEFile::getImportFunctionName(unsigned int nImport,unsigned int nFunctionNumber)
{
    unsigned long long nThunk64;
    unsigned int nThunk;
    unsigned int nOffset=0;

    if(isPEPlus())
    {
        nThunk64=getImportThunk64(nImport,nFunctionNumber);

        if(nThunk64)
        {
            if(!(nThunk64&0x8000000000000000))
            {
                nOffset=RVAToOffset(nThunk64);

                return getAnsiString(nOffset+2);
            }
        }

    }
    else
    {
        nThunk=getImportThunk(nImport,nFunctionNumber);

        if(nThunk)
        {
            if(!(nThunk&0x80000000))
            {
                nOffset=RVAToOffset(nThunk);

                return getAnsiString(nOffset+2);
            }
        }
    }


    return "";
}
QString PEFile::getImportFunctionName(unsigned long long nThunk)
{
    return getAnsiString(RVAToOffset(nThunk)+2);
}

QString PEFile::getImportFunctionName(unsigned int nThunk)
{
    return getAnsiString(RVAToOffset(nThunk)+2);
}

unsigned short PEFile::getImportHint(unsigned long long nThunk)
{
    return readWord(RVAToOffset(nThunk));
}

unsigned short PEFile::getImportHint(unsigned int nThunk)
{
    return readWord(RVAToOffset(nThunk));
}



bool PEFile::completeCheck()
{
    if(isValid())
    {
        unsigned int nRowSize=calculateRawSize();
        unsigned int nFileSize=this->size();

        if(nRowSize<=nFileSize)
        {
            return true;
        }
    }

    return false;
}

bool PEFile::isDll()
{
    if(isValid())
    {
        if(getOptionalHeader_Subsystem()!=IMAGE_SUBSYSTEM_NATIVE)
        {
            return (getFileHeader_Characteristics()&IMAGE_FILE_DLL);
        }
    }

    return false;
}

bool PEFile::isDriver()
{
    if(isValid())
    {
        return (getOptionalHeader_Subsystem()==IMAGE_SUBSYSTEM_NATIVE);
    }

    return false;
}

unsigned int PEFile::calculateSizeOfImage()
{
    unsigned int nLastSection=getFileHeader_NumberOfSections()-1;
    return (getSection_VirtualAddress(nLastSection)+ALIGN_UP(getSection_VirtualSize(nLastSection),getOptionalHeader_SectionAlignment()));
}

unsigned int PEFile::calculateSizeOfImage2()
{
    unsigned int nLastSection=getFileHeader_NumberOfSections()-1;
    return (getSection_VirtualAddress(nLastSection)+getSection_VirtualSize(nLastSection));
}

unsigned int PEFile::calculateSizeOfHeaders()
{
    return CalculateHeadersSize(getDosStubSize(),getFileHeader_SizeOfOptionalHeader(),getFileHeader_NumberOfSections(),getOptionalHeader_FileAlignment());
}

unsigned int PEFile::calculateBaseOfCode()
{
    unsigned int nCharacteristics;
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();

    for(int i=0; i<nNumberOfSections; i++)
    {
        nCharacteristics=getSection_Characteristics(i);

        if(nCharacteristics&IMAGE_SCN_MEM_EXECUTE)
        {
            //            int nTemp=nCharacteristics&IMAGE_SCN_CNT_UNINITIALIZED_DATA;
            //            nTemp=0;
            if((nCharacteristics&IMAGE_SCN_CNT_UNINITIALIZED_DATA)==0)
            {
                return getSection_VirtualAddress(i);
            }
        }
    }

    return 0;
}

unsigned int PEFile::calculateBaseOfData()
{
    unsigned int nCharacteristics;
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();

    for(int i=0; i<nNumberOfSections; i++)
    {
        nCharacteristics=getSection_Characteristics(i);

        if(nCharacteristics&IMAGE_SCN_CNT_INITIALIZED_DATA)
        {
            if((nCharacteristics&IMAGE_SCN_CNT_UNINITIALIZED_DATA)==0)
            {
                return getSection_VirtualAddress(i);
            }
        }
    }

    return 0;
}

unsigned int PEFile::calculateSizeOfInitializedData()
{
    unsigned int nCharacteristics;
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nResult=0;

    for(int i=0; i<nNumberOfSections; i++)
    {
        nCharacteristics=getSection_Characteristics(i);

        if(nCharacteristics&IMAGE_SCN_CNT_INITIALIZED_DATA)
        {
            nResult+=ALIGN_UP(qMax(getSection_SizeOfRawData(i),getSection_VirtualSize(i)),getOptionalHeader_FileAlignment());
        }
    }

    return nResult;
}

unsigned int PEFile::calculateSizeOfInitializedData2()
{
    unsigned int nCharacteristics;
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nResult=0;

    for(int i=0; i<nNumberOfSections; i++)
    {
        nCharacteristics=getSection_Characteristics(i);

        if(nCharacteristics&IMAGE_SCN_CNT_INITIALIZED_DATA)
        {
            nResult+=ALIGN_UP(getSection_SizeOfRawData(i),getOptionalHeader_FileAlignment());
        }
    }

    return nResult;
}

unsigned int PEFile::calculateSizeOfUninitializedData()
{
    unsigned int nCharacteristics;
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nResult=0;

    for(int i=0; i<nNumberOfSections; i++)
    {
        nCharacteristics=getSection_Characteristics(i);

        if(nCharacteristics&IMAGE_SCN_CNT_UNINITIALIZED_DATA)
        {
            nResult+=ALIGN_UP(qMax(getSection_SizeOfRawData(i),getSection_VirtualSize(i)),getOptionalHeader_FileAlignment());
        }
    }

    return nResult;
}

unsigned int PEFile::calculateSizeOfCode()
{
    unsigned int nCharacteristics;
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nResult=0;

    for(int i=0; i<nNumberOfSections; i++)
    {
        nCharacteristics=getSection_Characteristics(i);

        if(nCharacteristics&IMAGE_SCN_CNT_CODE)
        {
            nResult+=ALIGN_UP(getSection_SizeOfRawData(i),getOptionalHeader_FileAlignment());
        }
    }

    return nResult;
}

QByteArray PEFile::getHeaders()
{
    QByteArray baResult;

    unsigned int nHeadersSize=calculateSizeOfHeaders();
    baResult.resize(nHeadersSize);

    readArray(0,baResult.data(),baResult.size());

    return baResult;
}

QByteArray PEFile::getEntryPointCode()
{
    QByteArray baResult;

    file.seek(RVAToOffset(getOptionalHeader_AddressOfEntryPoint()));

    baResult=file.read(0x1000);

    return baResult;
}

//bool PEFile::compareEP(char *pszSignature,unsigned int nOffset)
//{
////    bool bResult=false;
////    int nSize=strlen(pszSignature)/2;
////    unsigned int nEP=RVAToOffset(getOptionalHeader_AddressOfEntryPoint());

////    char *pBuffer=new char[nSize];
////    char *pBuffer2=new char[nSize*2+1];
//////    char *pHex;

////    file.seek(nEP+nOffset);
////    if(nSize==file.read(pBuffer,nSize))
////    {
////        dataToHex(pBuffer,nSize,pBuffer2);
////        bResult=signatureCompare(pBuffer2,pszSignature,nSize*2);
////    }
////    else
////    {
////        bResult=false;
////    }

////    delete[] pBuffer;
////    delete[] pBuffer2;

////    return bResult;

//    return compare(pszSignature,getEntryPointOffset()+nOffset);
//}

bool PEFile::compareEP(QString sSignature,unsigned int nOffset)
{
    //    return compareEP(convertSignature(sSignature).toAscii().data(),nOffset);
    return compare(sSignature,getEntryPointOffset()+nOffset);
}

//bool PEFile::compareOverlay(char *pszSignature,unsigned int nOffset)
//{
////    bool bResult=false;
////    int nSize=strlen(pszSignature)/2;
////    unsigned int nOverlay=getOverlayOffset();

////    char *pBuffer=new char[nSize];
////    char *pBuffer2=new char[nSize*2+1];
//////    char *pHex;

////    file.seek(nOverlay);
////    if(nSize==file.read(pBuffer,nSize))
////    {
////        dataToHex(pBuffer,nSize,pBuffer2);
////        bResult=signatureCompare(pBuffer2,pszSignature,nSize*2);
////    }
////    else
////    {
////        bResult=false;
////    }

////    delete[] pBuffer;
////    delete[] pBuffer2;

////    return bResult;
//    return compare(pszSignature,getOverlayOffset()+nOffset);
//}

bool PEFile::compareOverlay(QString sSignature,unsigned int nOffset)
{
    //    return compareOverlay(convertSignature(sSignature).toAscii().data(),nOffset);
    return compare(sSignature,getOverlayOffset()+nOffset);
}

void PEFile::entryPointLoad()
{
    nEntryPointOffset=RVAToOffset(getOptionalHeader_AddressOfEntryPoint());

    _loadEntryPoint();
}


QString PEFile::getImportFunctionName(QString sLibraryName,unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    int nImport=getImportLibraryNumber(sLibraryName);

    if(nImport!=-1)
    {
        return getImportFunctionName(nImport,nFunctionNumber);
    }

    emit appendError(QString("Invalid string: %1").arg(sLibraryName));
    return "";

}

void PEFile::setImportThunk(unsigned int nImport,unsigned int nFunctionNumber, unsigned int nValue)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return;
    }

    if(nImport<getNumberOfImports())
    {
        if(nFunctionNumber<getNumberOfImportThunks(nImport))
        {
            if(getImport_OriginalFirstThunk(nImport))
            {
                return writeDword(RVAToOffset(getImport_OriginalFirstThunk(nImport))+4*nFunctionNumber,nValue);
            }
            else
            {
                return writeDword(RVAToOffset(getImport_FirstThunk(nImport))+4*nFunctionNumber,nValue);
            }

            return;
        }

        emit appendError(QString("Value out of range: n=%1").arg(nFunctionNumber));
        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
    return;
}
///!!!!!!!!!!!!
void PEFile::setImportThunk64(unsigned int nImport,unsigned int nFunctionNumber, unsigned long long nValue)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return;
    }

    if(nImport<getNumberOfImports())
    {
        if(nFunctionNumber<getNumberOfImportThunks(nImport))
        {
            if(getImport_OriginalFirstThunk(nImport))
            {
                return writeQword(RVAToOffset(getImport_OriginalFirstThunk(nImport))+8*nFunctionNumber,nValue);
            }
            else
            {
                return writeQword(RVAToOffset(getImport_FirstThunk(nImport))+8*nFunctionNumber,nValue);
            }

            return;
        }

        emit appendError(QString("Value out of range: n=%1").arg(nFunctionNumber));
        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
    return;
}

unsigned int PEFile::getNumberOfImportOriginalThunks(unsigned int nImport)
{
    unsigned int nOffset=0;
    unsigned int nResult=0;

    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        nOffset=RVAToOffset(getImport_OriginalFirstThunk(nImport));

        if(isPEPlus())
        {
            while(readQword(nOffset))
            {
                nResult++;
                nOffset+=8;
            }
        }
        else
        {
            while(readDword(nOffset))
            {
                nResult++;
                nOffset+=4;
            }
        }

        return nResult;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}

unsigned int PEFile::getNumberOfImportOriginalThunks(QString sLibraryName)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    int nNumberOfImports=getNumberOfImports();

    for(int i=0; i<nNumberOfImports; i++)
    {
        if(getImport_NameAsString(i)==sLibraryName)
        {
            return getNumberOfImportOriginalThunks(i);
        }
    }

    emit appendError(QString("Invalid string: %1").arg(sLibraryName));

    return 0;
}

unsigned int PEFile::getImportOriginalThunk(unsigned int nImport,unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    if(nImport<getNumberOfImports())
    {
        if(nFunctionNumber<getNumberOfImportOriginalThunks(nImport))
        {
            return readDword(RVAToOffset(getImport_FirstThunk(nImport))+4*nFunctionNumber);
        }

        emit appendError(QString("Value out of range: n=%1").arg(nFunctionNumber));
        return 0;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));
    return 0;
}

unsigned int PEFile::getImportOriginalThunk(QString sLibraryName,unsigned int nFunctionNumber)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    int nImport=getImportLibraryNumber(sLibraryName);

    if(nImport!=-1)
    {
        return getImportOriginalThunk(nImport,nFunctionNumber);
    }

    return 0;
}

unsigned int PEFile::getImportLibraryNumber(QString sLibraryName)
{
    if(!isImportPresent())
    {
        emit appendWarning("Import is not present");
        return 0;
    }

    int nNumberOfImports=getNumberOfImports();

    for(int i=0; i<nNumberOfImports; i++)
    {
        if(getImport_NameAsString(i)==sLibraryName)
        {
            return i;
        }
    }

    emit appendError(QString("Invalid string: %1").arg(sLibraryName));

    return -1;
}

QString PEFile::getImportLibraryName(unsigned int nImport)
{
    return getImport_NameAsString(nImport);
}

unsigned int PEFile::getNewSection_VirtualAddress()
{
    int nSections=getFileHeader_NumberOfSections();

    while(nSections)
    {
        if(getSection_VirtualAddress(nSections-1)+ALIGN_UP(getSection_VirtualSize(nSections-1),getOptionalHeader_SectionAlignment()))
        {
            return getSection_VirtualAddress(nSections-1)+ALIGN_UP(getSection_VirtualSize(nSections-1),getOptionalHeader_SectionAlignment());
        }

        nSections--;
    }

    return ALIGN_UP(getOptionalHeader_SizeOfHeaders(),getOptionalHeader_SectionAlignment());
}

unsigned int PEFile::getNewSection_PointerToRawData()
{
    int nSections=getFileHeader_NumberOfSections();

    while(nSections)
    {
        if(getSection_PointerToRawData(nSections-1)+ALIGN_UP(getSection_SizeOfRawData(nSections-1),getOptionalHeader_FileAlignment()))
        {
            return getSection_PointerToRawData(nSections-1)+ALIGN_UP(getSection_SizeOfRawData(nSections-1),getOptionalHeader_FileAlignment());
        }

        nSections--;
    }

    return ALIGN_UP(getOptionalHeader_SizeOfHeaders(),getOptionalHeader_FileAlignment());
}



unsigned int PEFile::getExport_Characteristics()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    unsigned int nTemp=getExportTableOffset();
    nTemp=readDword(nTemp);
    nTemp++;

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,Characteristics));
}

unsigned int PEFile::getExport_TimeDateStamp()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,TimeDateStamp));
}

unsigned short PEFile::getExport_MajorVersion()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readWord(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,MajorVersion));
}

unsigned short PEFile::getExport_MinorVersion()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readWord(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,MinorVersion));
}

unsigned int PEFile::getExport_Name()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,Name));
}

unsigned int PEFile::getExport_Base()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,Base));
}

unsigned int PEFile::getExport_NumberOfFunctions()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,NumberOfFunctions));
}

unsigned int PEFile::getExport_NumberOfNames()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,NumberOfNames));
}

unsigned int PEFile::getExport_AddressOfFunctions()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,AddressOfFunctions));
}

unsigned int PEFile::getExport_AddressOfNames()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,AddressOfNames));
}

unsigned int PEFile::getExport_AddressOfNameOrdinals()
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return 0;
    }

    return readDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,AddressOfNameOrdinals));
}

void PEFile::setExport_Characteristics(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,Characteristics),nValue);
}

void PEFile::setExport_TimeDateStamp(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,TimeDateStamp),nValue);
}

void PEFile::setExport_MajorVersion(unsigned short sValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeWord(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,MajorVersion),sValue);
}

void PEFile::setExport_MinorVersion(unsigned short sValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeWord(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,MinorVersion),sValue);
}

void PEFile::setExport_Name(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,Name),nValue);
}

void PEFile::setExport_Base(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,Base),nValue);
}

void PEFile::setExport_NumberOfFunctions(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,NumberOfFunctions),nValue);
}

void PEFile::setExport_NumberOfNames(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,NumberOfNames),nValue);
}

void PEFile::setExport_AddressOfFunctions(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,AddressOfFunctions),nValue);
}

void PEFile::setExport_AddressOfNames(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,AddressOfNames),nValue);
}

void PEFile::setExport_AddressOfNameOrdinals(unsigned int nValue)
{
    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return;
    }

    writeDword(getExportTableOffset()+offsetof(IMAGE_EXPORT_DIRECTORY,AddressOfNameOrdinals),nValue);
}

QString PEFile::getExport_NameAsString()
{
    QString sResult;

    if(!isExportPresent())
    {
        emit appendError("Export is not present");
        return sResult;
    }

    return getAnsiString(RVAToOffset(getExport_Name()));
}

unsigned int PEFile::getExport_FunctionAddress(int nNumber)
{
    unsigned int nOffset;

    if(nNumber<(int)getExport_NumberOfFunctions())
    {
        nOffset=RVAToOffset(getExport_AddressOfFunctions());
        nOffset+=nNumber*4;

        return readDword(nOffset);
    }

    emit appendError(QString("Value out of range: n=%1").arg(nNumber));

    return 0;
}
unsigned int PEFile::getExport_FunctionAddress(int nNumber,unsigned int nAddressOfFunctions)
{
    return readDword(nAddressOfFunctions+nNumber*4);
}

unsigned short PEFile::getExport_FunctionNameOrdinal(int nNumber)
{
    unsigned int nOffset;

    if(nNumber<(int)getExport_NumberOfFunctions())
    {
        nOffset=RVAToOffset(getExport_AddressOfNameOrdinals());
        nOffset+=nNumber*2;

        return readWord(nOffset)+getExport_Base();
    }

    emit appendError(QString("Value out of range: n=%1").arg(nNumber));

    return 0;
}
unsigned short PEFile::getExport_FunctionNameOrdinal(int nNumber,unsigned int nAddressOfNameOrdinals,unsigned int nExport_Base)
{
    return readWord(nAddressOfNameOrdinals+nNumber*2)+nExport_Base;
}

unsigned int PEFile::getExport_FunctionName(int nNumber)
{
    unsigned int nOffset;

    if(nNumber<(int)getExport_NumberOfNames())
    {
        nOffset=RVAToOffset(getExport_AddressOfNames());
        nOffset+=nNumber*4;

        return readDword(nOffset);
    }

    emit appendError(QString("Value out of range: n=%1").arg(nNumber));

    return 0;
}

void PEFile::setExport_FunctionAddress(int nNumber, unsigned int nValue)
{
    unsigned int nOffset;

    if(nNumber<(int)getExport_NumberOfFunctions())
    {
        nOffset=RVAToOffset(getExport_AddressOfFunctions());
        nOffset+=nNumber*4;

        writeDword(nOffset,nValue);

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nNumber));
}

void PEFile::setExport_FunctionNameOrdinal(int nNumber, unsigned short sValue)
{
    unsigned int nOffset;

    if(nNumber<(int)getExport_NumberOfFunctions())
    {
        nOffset=RVAToOffset(getExport_AddressOfNameOrdinals());
        nOffset+=nNumber*2;

        writeWord(nOffset,sValue-getExport_Base());

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nNumber));

}

void PEFile::setExport_FunctionName(int nNumber, unsigned int nValue)
{
    unsigned int nOffset;

    if(nNumber<(int)getExport_NumberOfNames())
    {
        nOffset=RVAToOffset(getExport_AddressOfNames());
        nOffset+=nNumber*4;

        writeDword(nOffset,nValue);

        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nNumber));

}

QString PEFile::getExport_FunctionNameAsString(int nNumber)
{
    if(nNumber<getExport_NumberOfNames())
    {
        unsigned int nRVA=getExport_FunctionName(nNumber);
        unsigned int nOffset=RVAToOffset(nRVA);
        //        qDebug("RVA=%X Offset=%X",nRVA,nOffset);
        return getAnsiString(nOffset,140);
    }

    return "";
}

QString PEFile::getExport_FunctionNameAsString(int nNumber,unsigned int nAddressOfNames)
{
    return getAnsiString(RVAToOffset(readDword(nAddressOfNames+4*nNumber)),120);
}
void PEFile::FixCheckSum()
{
    setOptionalHeader_CheckSum(CalculateCheckSum());
}
void PEFile::FixRawOffsets(int nDelta)
{
    setOptionalHeader_SizeOfHeaders(getOptionalHeader_SizeOfHeaders()+nDelta);

    for(int i=0; i<getFileHeader_NumberOfSections(); i++)
    {
        //        int nh=(int)getSection_PointerToRawData(i)+nDelta;
        setSection_PointerToRawData(i,(int)getSection_PointerToRawData(i)+nDelta);
    }
}

void PEFile::fixSizeOfHeaders()
{
    unsigned int nSizeOfHeaders=calculateSizeOfHeaders();
    setOptionalHeader_SizeOfHeaders(nSizeOfHeaders);
}
bool PEFile::isSectionNamePresentExp(QString sSectionName)
{
    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nSectionsTableOffset=getSectionsTableOffset();
    QRegExp exp;
    exp.setPattern(sSectionName);

    for(int i=0; i<nNumberOfSections; i++)
    {
        if(getSectionNameAsString(i,nSectionsTableOffset).toLower().contains(exp))
        {
            return true;
        }
    }

    return false;
}
unsigned int PEFile::getSectionNumberExp(QString sSectionName)
{
    unsigned int nSectionsTableOffset=getSectionsTableOffset();
    unsigned int nSections=getFileHeader_NumberOfSections();
    QRegExp exp;
    exp.setPattern(sSectionName);

    for(int i=0; i<nSections; i++)
    {
        if(getSectionNameAsString(i,nSectionsTableOffset).contains(exp))
        {
            return i;
        }
    }

    return -1;
}

QString PEFile::getSectionNameCollision(QString sString1,QString sString2)
{
    QList<QString> list;
    QString sResult="";
    bool bFlag1=false;
    bool bFlag2=false;
    unsigned int nSecondSection=0;
    QString sSecondSection;


    unsigned int nNumberOfSections=getFileHeader_NumberOfSections();
    unsigned int nSectionsTableOffset=getSectionsTableOffset();

    for(int i=0; i<nNumberOfSections; i++)
    {
        list.append(getSectionNameAsString(i,nSectionsTableOffset));

        if(list.at(i).contains(sString1))
        {
            bFlag1=true;
        }
        else if(bFlag1&&list.at(i).contains(sString2))
        {
            bFlag2=true;
            nSecondSection=i;
            sSecondSection=list.at(i);
        }
    }

    if(bFlag1&&bFlag2)
    {
        sResult=sSecondSection.section(sString2,0,0);

        for(int i=0; i<list.count(); i++)
        {
            if(sResult+sString1==list.at(i))
            {
                return sResult;
            }
        }

        sResult="";
    }


    return sResult;
}

int PEFile::getResourceSection()
{
    if(!isResourcePresent())
    {
        emit appendWarning("Resource is not present");
        return 0;
    }

    IMAGE_DATA_DIRECTORY idd;
    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_RESOURCE,&idd);

    return RVAToSection(idd.VirtualAddress);
}

QByteArray PEFile::getResourceAsXML()
{
    if(!isResourcePresent())
    {
        return 0;
    }

    QByteArray baResult;

    QXmlStreamWriter xml(&baResult);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("Resource");


    unsigned int nResourceOffset;

    nResourceOffset=getResourceOffset();

    if(nResourceOffset)
    {
        resourceToXML(nResourceOffset,nResourceOffset,&xml,0);
    }

    xml.writeEndElement();
    xml.writeEndDocument();

    return baResult;
}
void PEFile::resourceToXML(unsigned int nResourceOffset,unsigned int nResourceDirectory,QXmlStreamWriter *pXml,int nLevel)
{
    int nNumberOfNamedEntries;
    int nNumberOfIdEntries;
    IMAGE_RESOURCE_DIRECTORY_ENTRY ImageResDirEntry;
    IMAGE_RESOURCE_DATA_ENTRY ImageResDataEntry;
    int nStringLength;
    QString sName;
    unsigned int nOffset;
    unsigned int nTemp;

    IMAGE_RESOURCE_DIRECTORY ird;

    QByteArray baTemp;

    if(!readArray(nResourceDirectory,(char *)&ird,sizeof(ird)))
    {
        return;
    }

    pXml->writeStartElement("Directory");
    pXml->writeAttribute("Characteristics",QString::number(ird.Characteristics));
    pXml->writeAttribute("MajorVersion",QString::number(ird.MajorVersion));
    pXml->writeAttribute("MinorVersion",QString::number(ird.MinorVersion));
    pXml->writeAttribute("TimeDateStamp",QString::number(ird.TimeDateStamp));
    pXml->writeAttribute("NumberOfNamedEntries",QString::number(ird.NumberOfNamedEntries));
    pXml->writeAttribute("NumberOfIdEntries",QString::number(ird.NumberOfIdEntries));

    pXml->writeAttribute("Level",QString::number(nLevel));


    nNumberOfNamedEntries=ird.NumberOfNamedEntries;
    nNumberOfIdEntries=ird.NumberOfIdEntries;

    nOffset=nResourceDirectory+sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<(nNumberOfNamedEntries+nNumberOfIdEntries); i++)
    {
        if(!readArray(nOffset,(char *)&ImageResDirEntry,sizeof(ImageResDirEntry)))
        {
            pXml->writeEndElement();
            return;
        }

        pXml->writeStartElement("Entry");

        if((ImageResDirEntry.Name)&0x80000000)
        {
            nStringLength=readWord(nResourceOffset+((ImageResDirEntry.Name)&0x7FFFFFFF));
            baTemp.resize(nStringLength*sizeof(short));

            if(!readArray(nResourceOffset+((ImageResDirEntry.Name)&0x7FFFFFFF)+sizeof(short),baTemp.data(),baTemp.size()))
            {
                pXml->writeEndElement();
                return;
            }

            sName.setUnicode((QChar *)baTemp.data(),nStringLength);

            pXml->writeAttribute("Name",sName);
        }
        else
        {
            pXml->writeAttribute("Id",QString::number(ImageResDirEntry.Id));
        }

        pXml->writeAttribute("Level",QString::number(nLevel));

        if((ImageResDirEntry.OffsetToData)&0x80000000)
        {
            nTemp=nResourceOffset+((ImageResDirEntry.OffsetToData)&0x7FFFFFFF);
            resourceToXML(nResourceOffset,nTemp,pXml,nLevel+1);
        }
        else
        {
            nTemp=nResourceOffset+ImageResDirEntry.OffsetToData;

            if(!readArray(nTemp,(char *)&ImageResDataEntry,sizeof(ImageResDataEntry)))
            {
                pXml->writeEndElement();
                pXml->writeEndElement();
                return;
            }

            pXml->writeStartElement("DataEntry");
            pXml->writeAttribute("Reserved",QString::number(ImageResDataEntry.Reserved));
            pXml->writeAttribute("CodePage",QString::number(ImageResDataEntry.CodePage));

            pXml->writeAttribute("Level",QString::number(nLevel+1));

            nTemp=RVAToOffset(ImageResDataEntry.OffsetToData);
            pXml->writeAttribute("Offset",QString::number(nTemp));

            if(nTemp==-1)
            {
                emit appendWarning(QString("Invalid resource RVA: %1 (may be packed)").arg(ImageResDataEntry.OffsetToData,8,16,QChar('0')));
            }

            pXml->writeAttribute("Size",QString::number(ImageResDataEntry.Size));

            pXml->writeEndElement();
        }

        pXml->writeEndElement();
        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }


    pXml->writeEndElement();
}

QByteArray PEFile::getImportAsXML()
{
    if(!isImportPresent())
    {
        return 0;
    }

    QByteArray baResult;

    QXmlStreamWriter xml(&baResult);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("Import");


    unsigned int nImportTableOffset;

    nImportTableOffset=getImportTableOffset();

    if(nImportTableOffset)
    {
        int nNumberOfImports=getNumberOfImports();

        for(int i=0; i<nNumberOfImports; i++)
        {
            xml.writeStartElement("Library");

            xml.writeAttribute("Name",getImport_NameAsString(i,nImportTableOffset));
            //            xml.writeAttribute("OriginalFirstThunk",QString::number(getImport_OriginalFirstThunk(i,nImportTableOffset)));
            xml.writeAttribute("TimeDateStamp",QString::number(getImport_TimeDateStamp(i,nImportTableOffset)));
            xml.writeAttribute("ForwarderChain",QString::number(getImport_ForwarderChain(i,nImportTableOffset)));
            //            xml.writeAttribute("FirstThunk",QString::number(getImport_FirstThunk(i,nImportTableOffset)));

            int nNumberOfImportThunks=getNumberOfImportThunks(i);

            if(nNumberOfImportThunks>0xFFFF)
            {
                nNumberOfImportThunks=0;
            }

            unsigned int nOriginalFirstThunk=RVAToOffset(getImport_OriginalFirstThunk(i));
            unsigned int nFirstThunk=RVAToOffset(getImport_FirstThunk(i));


            for(int j=0; j<nNumberOfImportThunks; j++)
            {
                xml.writeStartElement("Function");

                if(isPEPlus())
                {
                    unsigned long long nThunk64;

                    nThunk64=getImportThunk64(nOriginalFirstThunk,nFirstThunk,j);

                    if(nThunk64&0x8000000000000000)
                    {
                        xml.writeAttribute("Ordinal",QString::number(nThunk64&0x7FFFFFFFFFFFFFFF));
                    }
                    else
                    {
                        xml.writeAttribute("Hint",QString::number(getImportHint(nThunk64)));
                        xml.writeAttribute("Name",getImportFunctionName(nThunk64));
                    }
                }
                else
                {
                    unsigned int nThunk;

                    nThunk=getImportThunk(nOriginalFirstThunk,nFirstThunk,j);

                    if(nThunk&0x80000000)
                    {
                        xml.writeAttribute("Ordinal",QString::number(nThunk&0x7FFFFFFF));
                    }
                    else
                    {
                        xml.writeAttribute("Hint",QString::number(getImportHint(nThunk)));
                        xml.writeAttribute("Name",getImportFunctionName(nThunk));
                    }
                }

                xml.writeEndElement();
            }

            xml.writeEndElement();
        }
    }

    xml.writeEndElement();
    xml.writeEndDocument();

    return baResult;
}

QByteArray PEFile::ImportAsXMLToBin(QByteArray baXML,IMAGE_DATA_DIRECTORY *pddImportTable,IMAGE_DATA_DIRECTORY *pddIAT)
{
    QByteArray baResult;
    QXmlStreamReader xml(baXML);

    //int PE::ImportListToBin(char *pImportList,char *pImportBin,IMAGE_DATA_DIRECTORY *pddImportTable,IMAGE_DATA_DIRECTORY *pddIAT)
    //{
    int nDll=0;
    int nFuncs=0;
    char *pNames;
    int *pnFirstThunk,*pnOriginalFirstThunk;
    IMAGE_IMPORT_DESCRIPTOR *pIID;




    while(!xml.atEnd())
    {
        xml.readNextStartElement();

        // do processing
        if(xml.name()=="Library")
        {
            nDll++;
        }

        if(xml.name()=="Function")
        {
            nFuncs++;
        }
    }

    xml.clear();
    xml.addData(baXML);

    baResult.resize((nDll+nFuncs)*2*sizeof(unsigned int)+sizeof(IMAGE_IMPORT_DESCRIPTOR)+nFuncs*20);
    baResult.fill(0);

    QString sDllName,sFunctionName;
    int nOrdinal;

    pnFirstThunk=(int *)baResult.data();
    pIID=(IMAGE_IMPORT_DESCRIPTOR *)(pnFirstThunk+nDll+nFuncs);
    pnOriginalFirstThunk=(int *)(pIID+nDll+1);
    pNames=(char *)(pnOriginalFirstThunk+nDll+nFuncs);

    pddImportTable->VirtualAddress=(long long)pIID-(long long)pnFirstThunk;
    pddImportTable->Size=(long long)pnOriginalFirstThunk-(long long)pIID;

    pddIAT->VirtualAddress=0;
    pddIAT->Size=(long long)pIID-(long long)pnFirstThunk;


    while(!xml.atEnd())
    {
        xml.readNextStartElement();

        // do processing
        if(xml.name()=="Library")
        {
            sDllName=xml.attributes().value("Name").toString();

            if(!sDllName.isEmpty())
            {
                pIID->FirstThunk=(int)((char *)pnFirstThunk-baResult.data());
                pIID->OriginalFirstThunk=(int)((char *)pnOriginalFirstThunk-baResult.data());
                pIID->ForwarderChain=xml.attributes().value("ForwarderChain").toString().toInt();
                pIID->TimeDateStamp=xml.attributes().value("TimeDateStamp").toString().toInt();

                while(true)
                {
                    xml.readNextStartElement();

                    if(xml.name()=="Function")
                    {
                        sFunctionName=xml.attributes().value("Name").toString();

                        if(!sFunctionName.isEmpty())
                        {
                            *pnFirstThunk=(int)(pNames-baResult.data());
                            pnFirstThunk++;
                            *pnOriginalFirstThunk=(int)(pNames-baResult.data());
                            pnOriginalFirstThunk++;

                            *((short *)pNames)=(short)xml.attributes().value("Hint").toString().toInt();
                            pNames+=sizeof(short);

                            strcpy(pNames,sFunctionName.toAscii());
                            pNames+=sFunctionName.size()+1;
                        }

                        nOrdinal=xml.attributes().value("Ordinal").toString().toLong();

                        if(nOrdinal)
                        {
                            *pnFirstThunk=0x80000000+nOrdinal;
                            pnFirstThunk++;
                            *pnOriginalFirstThunk=0x80000000+nOrdinal;
                            pnOriginalFirstThunk++;
                        }
                    }
                    else if(xml.name()=="Library")
                    {
                        break;
                    }
                }

                pIID->Name=(int)(pNames-baResult.data());

                *pnFirstThunk=0;
                pnFirstThunk++;
                *pnOriginalFirstThunk=0;
                pnOriginalFirstThunk++;

                strcpy(pNames,sDllName.toAscii());
                pNames+=sDllName.size()+1;

                pIID++;
            }

        }

        //        xml.readNextStartElement();

    }

    pIID->FirstThunk=0;
    pIID->OriginalFirstThunk=0;
    pIID->ForwarderChain=0;
    pIID->TimeDateStamp=0;
    pIID->Name=0;

    baResult.resize(pNames-baResult.data());

    return baResult;
}

bool PEFile::addImportSectionFromXML(IMAGE_SECTION_HEADER *pISH,QByteArray baXML)
{
    IMAGE_DATA_DIRECTORY iddImportTable,iddIAT;

    return addImportSection(pISH,ImportAsXMLToBin(baXML,&iddImportTable,&iddIAT),&iddImportTable,&iddIAT);
}

//bool PEFile::addImportSectionFromMap(IMAGE_SECTION_HEADER *pISH, QMap<unsigned long long, QString> &mapIAT)
//{
//    IMAGE_DATA_DIRECTORY iddImportTable,iddIAT;
//    ZeroMemory(&iddIAT,sizeof(iddIAT));

//    QByteArray binImport;

//    return addImportSection(pISH,binImport,&iddImportTable,&iddIAT);
//}

bool PEFile::addImportSection(IMAGE_SECTION_HEADER *pISH, QByteArray baData, IMAGE_DATA_DIRECTORY *pddImportTable, IMAGE_DATA_DIRECTORY *pddIAT)
{
    return addImportSection(pISH,baData.data(),baData.size(),pddImportTable,pddIAT);
}

bool PEFile::addImportSection(IMAGE_SECTION_HEADER *pISH, char *pData,int nDataSize, IMAGE_DATA_DIRECTORY *pddImportTable, IMAGE_DATA_DIRECTORY *pddIAT)
{
    if(!(*(pISH->Name)))
    {
        strcpy((char *)(pISH->Name),".rdata");
    }

    if(!pISH->Characteristics)
    {
        pISH->Characteristics=0x40000020;
    }

    if(addSection(pISH,pData,nDataSize))
    {
        pddImportTable->VirtualAddress+=pISH->VirtualAddress;
        pddIAT->VirtualAddress+=pISH->VirtualAddress;

        setOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT, pddImportTable);
        setOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_IAT, pddIAT);

        if(!getOptionalHeader_BaseOfData())
        {
            setOptionalHeader_BaseOfData(pISH->VirtualAddress);
        }

        setOptionalHeader_SizeOfInitializedData(getOptionalHeader_SizeOfInitializedData()+pISH->SizeOfRawData);

        FixImportBin(pISH->PointerToRawData,getImportTableOffset(),pISH->VirtualAddress);

        return true;
    }

    return false;
}

void PEFile::FixImportBin(unsigned int nOffset,unsigned int nImportTableOffset, int nDelta)
{
    unsigned int nTemp=0;
    unsigned nTempOffset=0;

    int nNumberOfImports=getNumberOfImports();

    for(int i=0; i<nNumberOfImports; i++)
    {
        nTempOffset=nOffset+getImport_FirstThunk(i,nImportTableOffset);

        while(true)
        {
            nTemp=readDword(nTempOffset);

            if(!nTemp)
            {
                break;
            }

            if(!(nTemp&0x80000000))
            {
                writeDword(nTempOffset,nTemp+=nDelta);
            }

            nTempOffset+=4;
        }

        nTempOffset=nOffset+getImport_OriginalFirstThunk(i,nImportTableOffset);

        while(true)
        {
            nTemp=readDword(nTempOffset);

            if(!nTemp)
            {
                break;
            }

            if(!(nTemp&0x80000000))
            {
                writeDword(nTempOffset,nTemp+=nDelta);
            }

            nTempOffset+=4;
        }

        setImport_FirstThunk(i,getImport_FirstThunk(i,nImportTableOffset)+nDelta);
        setImport_OriginalFirstThunk(i,getImport_OriginalFirstThunk(i,nImportTableOffset)+nDelta);
        setImport_Name(i,getImport_Name(i,nImportTableOffset)+nDelta);
    }
}

bool PEFile::saveImportAsXMLFile(QString sFileName)
{
    QByteArray baImport=getImportAsXML();

    Binary tempfile;

    if(!tempfile.createFile(sFileName,baImport.size()))
    {
        return false;
    }

    if(!tempfile.writeArrayToFile(0,baImport.data(),baImport.size()))
    {
        return false;
    }

    return true;
}
bool PEFile::isVersionInfoPresent()
{
    return isResourceIdPresent(16,1,-1);
    //return isResourceNamePresent("BINARY");
}


QString PEFile::getFileVersion()
{
    QString sResult;

    if(isVersionInfoPresent())
    {
        QByteArray baTemp=getResourceIdData(16);

        VersionResource vr(&baTemp);

        sResult=vr.getFileVersion();
    }

    return sResult;
}

QString PEFile::getVersionStringInfo(QString sKey)
{
    QString sResult;

    QByteArray baTemp=getResourceIdDataFast(16);

    if(!baTemp.isEmpty())
    {
        VersionResource vr(&baTemp);

        sResult=vr.getStringFileInfo(sKey);
    }



    return sResult;
}



//bool PEFile::isResourceIdPresent(unsigned int nID)
//{
//    if(!isResourcePresent())
//    {
//        return false;
//    }
//    QByteArray baData=getResourceAsXML();
//    QXmlStreamReader xml(baData);

//    while(!xml.atEnd())
//    {
//        if(xml.readNextStartElement())
//        {
//            if(xml.name()=="Entry")
//            {
//                if(xml.attributes().value("Level").toString()=="0")
//                {
//                    if(xml.attributes().value("Id").toString().toInt()==nID)
//                    {
//                        return true;
//                    }
//                }
//            }
//        }
//    }

//    return false;
//}

bool PEFile::isResourceNamePresent(QString sName)
{
    if(!isResourcePresent())
    {
        return false;
    }

    QByteArray baData=getResourceAsXML();
    QXmlStreamReader xml(baData);




    while(!xml.atEnd())
    {
        if(xml.readNextStartElement())
        {
            if(xml.name()=="Entry")
            {
                //                qDebug(xml.attributes().value("Name").toString().toAscii().data());
                //                if(xml.attributes().value("Level").toString()=="0")
                //                {
                ////                    qDebug(xml.attributes().value("Name").toString().toAscii().data());
                //                    if(xml.attributes().value("Name").toString()==sName)
                //                    {
                //                        return true;
                //                    }
                //                }
                if(xml.attributes().value("Name").toString()==sName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool PEFile::isResourceNamePresentFast(QString sName)
{
    if(!isResourcePresent())
    {
        return false;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        //&0x7FFFFFFF
        //        nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nOffset))->OffsetToData);
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return false;
        }

        //        unsigned int nNameEntries2=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nDirectoryEntryOffset))->NumberOfNamedEntries;
        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));

        if(nNameEntries2)
        {
            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

            for(int j=0; j<nNameEntries2; j++)
            {
                unsigned int nTemp=readDword(nDirectoryEntryOffset)&0x7FFFFFFF;
                nTemp+=nResourceOffset;
                //                nTemp=RVAToOffset(nTemp);
                //                qDebug(getResourceString(___pMemory+nTemp).toAscii().data());
                unsigned int nStringLength=readWord(nTemp);

                if(!isOffsetAndSizeValid(nTemp+2,nStringLength*2))
                {
                    return false;
                }

                QByteArray baName=readArray(nTemp+2,nStringLength*2);

                if(QString::fromUtf16((ushort *)(baName.data()),nStringLength)==sName)
                    //                if(getResourceString(___pMemory+nTemp)==sName)
                {
                    return true;
                }

                nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
            }
        }

        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return false;
}

unsigned int PEFile::getResourceNameOffset(QString sName)
{
    if(!isResourcePresent())
    {
        return false;
    }

    QByteArray baData=getResourceAsXML();
    //    qDebug(baData.data());
    QXmlStreamReader xml(baData);


    while(!xml.atEnd())
    {
        if(xml.readNextStartElement())
        {
            if(xml.name()=="Entry")
            {
                if(xml.attributes().value("Name").toString()==sName)
                {
                    xml.readNextStartElement();
                    xml.readNextStartElement();
                    xml.readNextStartElement();
                    return xml.attributes().value("Offset").toString().toUInt();
                }
            }
        }
    }

    return -1;
}

unsigned int PEFile::getResourceNameSize(QString sName)
{
    if(!isResourcePresent())
    {
        return false;
    }

    QByteArray baData=getResourceAsXML();
    QXmlStreamReader xml(baData);


    while(!xml.atEnd())
    {
        if(xml.readNextStartElement())
        {
            if(xml.name()=="Entry")
            {
                if(xml.attributes().value("Name").toString()==sName)
                {
                    xml.readNextStartElement();
                    xml.readNextStartElement();
                    xml.readNextStartElement();
                    return xml.attributes().value("Size").toString().toUInt();
                }
            }
        }
    }

    return -1;
}

unsigned int PEFile::getResourceNameOffsetFast(QString sName)
{
    if(!isResourcePresent())
    {
        return -1;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    //    unsigned int nNameEntries=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nOffset))->NumberOfNamedEntries;
    //    unsigned int nIDEntries=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nOffset))->NumberOfIdEntries;
    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        //&0x7FFFFFFF
        //        nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nOffset))->OffsetToDirectory);
        //        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return -1;
        }

        //        unsigned int nNameEntries2=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nDirectoryEntryOffset))->NumberOfNamedEntries;
        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));

        if(nNameEntries2)
        {
            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

            for(int j=0; j<nNameEntries2; j++)
            {
                unsigned int nTemp=readDword(nDirectoryEntryOffset)&0x7FFFFFFF;
                nTemp+=nResourceOffset;
                //                nTemp=RVAToOffset(nTemp);
                //                qDebug(getResourceString(___pMemory+nTemp).toAscii().data());
                unsigned int nStringLength=readWord(nTemp);

                if(!isOffsetAndSizeValid(nTemp+2,nStringLength*2))
                {
                    return -1;
                }

                QByteArray baName=readArray(nTemp+2,nStringLength*2);

                if(QString::fromUtf16((ushort *)(baName.data()),nStringLength)==sName)
                {

                    //                    nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nDirectoryEntryOffset))->OffsetToDirectory);
                    nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                    if(!isOffsetValid(nDirectoryEntryOffset))
                    {
                        return -1;
                    }

                    nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

                    if(!isOffsetValid(nDirectoryEntryOffset))
                    {
                        return -1;
                    }

                    //                    nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nDirectoryEntryOffset))->OffsetToData);
                    nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                    if(!isOffsetValid(nDirectoryEntryOffset))
                    {
                        return -1;
                    }

                    //                    unsigned int nRVA=(((IMAGE_RESOURCE_DATA_ENTRY*)(___pMemory+nDirectoryEntryOffset))->OffsetToData);
                    unsigned int nRVA=readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DATA_ENTRY,OffsetToData));

                    return RVAToOffset(nRVA);


                }

                nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
            }
        }

        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return -1;
}

unsigned int PEFile::getResourceNameSizeFast(QString sName)
{
    if(!isResourcePresent())
    {
        return 0;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    //    unsigned int nNameEntries=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nOffset))->NumberOfNamedEntries;
    //    unsigned int nIDEntries=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nOffset))->NumberOfIdEntries;
    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        //&0x7FFFFFFF
        //        nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nOffset))->OffsetToDirectory);
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return 0;
        }

        //        unsigned int nNameEntries2=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nDirectoryEntryOffset))->NumberOfNamedEntries;
        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));

        if(nNameEntries2)
        {
            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

            for(int j=0; j<nNameEntries2; j++)
            {
                unsigned int nTemp=readDword(nDirectoryEntryOffset)&0x7FFFFFFF;
                nTemp+=nResourceOffset;
                //                nTemp=RVAToOffset(nTemp);
                //                qDebug(getResourceString(___pMemory+nTemp).toAscii().data());
                unsigned int nStringLength=readWord(nTemp);

                if(!isOffsetAndSizeValid(nTemp+2,nStringLength*2))
                {
                    return 0;
                }

                QByteArray baName=readArray(nTemp+2,nStringLength*2);

                if(QString::fromUtf16((ushort *)(baName.data()),nStringLength)==sName)
                {

                    //                    nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nDirectoryEntryOffset))->OffsetToDirectory);
                    nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                    if(!isOffsetValid(nDirectoryEntryOffset))
                    {
                        return 0;
                    }

                    nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

                    if(!isOffsetValid(nDirectoryEntryOffset))
                    {
                        return 0;
                    }

                    //                    nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nDirectoryEntryOffset))->OffsetToData);
                    nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                    if(!isOffsetValid(nDirectoryEntryOffset))
                    {
                        return 0;
                    }

                    //                    unsigned int nSize=(((IMAGE_RESOURCE_DATA_ENTRY*)(___pMemory+nDirectoryEntryOffset))->Size);
                    unsigned int nSize=readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DATA_ENTRY,Size));

                    return nSize;


                }

                nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
            }
        }

        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return 0;
}

QByteArray PEFile::getResourceIdData(unsigned int nID1,unsigned int nID2,unsigned int nID3)
{
    bool bFlag1=false;
    bool bFlag2=false;

    if(!isResourcePresent())
    {
        return 0;
    }

    QByteArray baResult;
    QByteArray baData=getResourceAsXML();
    QXmlStreamReader xml(baData);
    unsigned int nOffset;
    unsigned int nSize;

    while(!xml.atEnd())
    {
        if(xml.readNextStartElement())
        {
            if(xml.name()=="Entry")
            {
                if(xml.attributes().value("Level").toString()=="0")
                {
                    if(xml.attributes().value("Id").toString().toInt()==nID1)
                    {
                        bFlag1=true;
                    }
                    else
                    {
                        bFlag1=false;
                    }
                }
                else if(xml.attributes().value("Level").toString()=="1")
                {
                    if((xml.attributes().value("Id").toString().toInt()==nID2)||(nID2==-1))
                    {
                        bFlag2=true;
                    }
                    else
                    {
                        bFlag2=false;
                    }
                }
                else if(xml.attributes().value("Level").toString()=="2")
                {
                    //                    QString sTemp=xml.attributes().value("Id").toString();
                    if((xml.attributes().value("Id").toString().toInt()==nID3)||(nID3==-1))
                    {
                        if(bFlag1&&bFlag2)
                        {
                            xml.readNextStartElement();

                            if(xml.name()=="DataEntry")
                            {
                                nOffset=xml.attributes().value("Offset").toString().toInt();
                                nSize=xml.attributes().value("Size").toString().toInt();

                                baResult.resize(nSize);

                                if(readArray(nOffset,baResult.data(),baResult.size()))
                                {
                                    return baResult;
                                }

                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }

    return baResult;
}

QByteArray PEFile::getResourceIdDataFast(unsigned int nID1)
{
    QByteArray baResult;

    if(!isResourcePresent())
    {
        return baResult;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    //    unsigned int nNameEntries4=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nOffset))->NumberOfNamedEntries;
    //    unsigned int nIDEntries4=((IMAGE_RESOURCE_DIRECTORY *)(___pMemory+nOffset))->NumberOfIdEntries;
    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);
    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)*nNameEntries;

    for(int i=0; i<nIDEntries; i++)
    {
        //&0x7FFFFFFF

        //        unsigned int _nID=(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nOffset))->Id);
        unsigned int _nID=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,Id));

        if(nID1==_nID)
        {
            //            nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nOffset))->OffsetToDirectory);
            nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;
            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

            if(!isOffsetValid(nDirectoryEntryOffset))
            {
                return QByteArray();
            }

            //            nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nDirectoryEntryOffset))->OffsetToDirectory);
            nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;
            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

            if(!isOffsetValid(nDirectoryEntryOffset))
            {
                return QByteArray();
            }

            //            nDirectoryEntryOffset=nResourceOffset+(((IMAGE_RESOURCE_DIRECTORY_ENTRY *)(___pMemory+nDirectoryEntryOffset))->OffsetToData);
            nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData));

            if(!isOffsetValid(nDirectoryEntryOffset))
            {
                return QByteArray();
            }

            //            unsigned int nRVA=(((IMAGE_RESOURCE_DATA_ENTRY*)(___pMemory+nDirectoryEntryOffset))->OffsetToData);
            //            unsigned int nSize=(((IMAGE_RESOURCE_DATA_ENTRY*)(___pMemory+nDirectoryEntryOffset))->Size);
            unsigned int nRVA=readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DATA_ENTRY,OffsetToData));
            unsigned int nSize=readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DATA_ENTRY,Size));

            baResult=readArray(RVAToOffset(nRVA),nSize);

            break;
        }



        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return baResult;
}

bool PEFile::isResourceIdPresent(unsigned int nID1, unsigned int nID2, unsigned int nID3)
{
    bool bFlag1=false;
    bool bFlag2=false;

    if(!isResourcePresent())
    {
        return 0;
    }

    QByteArray baData=getResourceAsXML();
    QXmlStreamReader xml(baData);

    while(!xml.atEnd())
    {
        if(xml.readNextStartElement())
        {
            if(xml.name()=="Entry")
            {
                if(xml.attributes().value("Level").toString()=="0")
                {
                    //                    qDebug(xml.attributes().value("Id").toString().toAscii().data());
                    if(xml.attributes().value("Id").toString().toInt()==nID1)
                    {
                        bFlag1=true;
                    }
                    else
                    {
                        bFlag1=false;
                    }
                }
                else if(xml.attributes().value("Level").toString()=="1")
                {
                    //                    qDebug(xml.attributes().value("Id").toString().toAscii().data());
                    if((xml.attributes().value("Id").toString().toInt()==nID2)||(nID2==-1))
                    {
                        bFlag2=true;
                    }
                    else
                    {
                        bFlag2=false;
                    }
                }
                else if(xml.attributes().value("Level").toString()=="2")
                {
                    //                    qDebug(xml.attributes().value("Id").toString().toAscii().data());
                    if((xml.attributes().value("Id").toString().toInt()==nID3)||(nID3==-1))
                    {
                        if(bFlag1&&bFlag2)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

QString PEFile::getResourceString(unsigned char *pOffset)
{
    int nSize=*(unsigned short *)pOffset;

    return QString::fromUtf16((ushort *)(pOffset+2),nSize);
}

QString PEFile::resourceIdToString(unsigned int nID)
{
    if(nID==0x2000)
    {
        return "RT_NEWRESOURCE";
    }
    else if(nID==0x7fff)
    {
        return "RT_ERROR";
    }
    else if(nID==(unsigned int)RT_CURSOR)
    {
        return "RT_CURSOR";
    }
    else if(nID==(unsigned int)RT_BITMAP)
    {
        return "RT_BITMAP";
    }
    else if(nID==(unsigned int)RT_ICON)
    {
        return "RT_ICON";
    }
    else if(nID==(unsigned int)RT_MENU)
    {
        return "RT_MENU";
    }
    else if(nID==(unsigned int)RT_DIALOG)
    {
        return "RT_DIALOG";
    }
    else if(nID==(unsigned int)RT_STRING)
    {
        return "RT_STRING";
    }
    else if(nID==(unsigned int)RT_FONTDIR)
    {
        return "RT_FONTDIR";
    }
    else if(nID==(unsigned int)RT_FONT)
    {
        return "RT_FONT";
    }
    else if(nID==9)
    {
        return "RT_ACCELERATORS";
    }
    else if(nID==(unsigned int)RT_RCDATA)
    {
        return "RT_RCDATA";
    }
    else if(nID==(unsigned int)RT_MESSAGETABLE)
    {
        return "RT_MESSAGETABLE";
    }
    else if(nID==(unsigned int)RT_GROUP_CURSOR)
    {
        return "RT_GROUP_CURSOR";
    }
    else if(nID==(unsigned int)RT_GROUP_ICON)
    {
        return "RT_GROUP_ICON";
    }
    else if(nID==(unsigned int)RT_VERSION)
    {
        return "RT_VERSION";
    }
    else if(nID==0x2002)
    {
        return "RT_NEWBITMAP";
    }
    else if(nID==0x2004)
    {
        return "RT_NEWMENU";
    }
    else if(nID==0x2005)
    {
        return "RT_NEWDIALOG";
    }
    //else if(nID==(unsigned int)RT_MANIFEST)
    else if(nID==(unsigned int)24)
    {
        return "RT_MANIFEST";
    }


    return QString("%1").arg(nID);
}

unsigned int PEFile::getNumberOfResources()
{
    unsigned int nResult=0;

    if(!isResourcePresent())
    {
        return 0;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    if((int)(nNameEntries+nIDEntries)>100)
    {
        // corrupted resource
        return 0;
    }

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        if(!isOffsetValid(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData)))
        {
            return 0;
        }

        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return 0;
        }

        unsigned int nCharacteristics=readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,Characteristics));

        if(nCharacteristics!=0)
        {
            // Corrupted resource
            break;
        }

        nResult+=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
        nResult+=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return nResult;
}

unsigned int PEFile::getResourceOffsetByNumber(unsigned int nNumber)
{
    unsigned int nCurrentNumber=0;

    if(!isResourcePresent())
    {
        return -1;
    }

    if(nNumber>=getNumberOfResources())
    {
        return -1;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return -1;
        }

        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
        unsigned int nIDEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

        nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

        for(int j=0; j<nNameEntries2+nIDEntries2; j++)
        {
            if(nCurrentNumber==nNumber)
            {
                //                unsigned int nTemp=readDword(nDirectoryEntryOffset)&0x7FFFFFFF;
                //                nTemp+=nResourceOffset;
                nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                if(!isOffsetValid(nDirectoryEntryOffset))
                {
                    return -1;
                }

                nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

                if(!isOffsetValid(nDirectoryEntryOffset))
                {
                    return -1;
                }

                nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                if(!isOffsetValid(nDirectoryEntryOffset))
                {
                    return -1;
                }

                unsigned int nRVA=readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DATA_ENTRY,OffsetToData));

                return RVAToOffset(nRVA);

            }

            nCurrentNumber++;

            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
        }


        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return -1;
}

unsigned int PEFile::getResourceSizeByNumber(unsigned int nNumber)
{
    unsigned int nCurrentNumber=0;

    if(!isResourcePresent())
    {
        return 0;
    }

    if(nNumber>=getNumberOfResources())
    {
        return 0;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return 0;
        }

        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
        unsigned int nIDEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

        nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

        for(int j=0; j<nNameEntries2+nIDEntries2; j++)
        {
            if(nCurrentNumber==nNumber)
            {
                //                unsigned int nTemp=readDword(nDirectoryEntryOffset)&0x7FFFFFFF;
                //                nTemp+=nResourceOffset;
                nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                if(!isOffsetValid(nDirectoryEntryOffset))
                {
                    return 0;
                }

                nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

                if(!isOffsetValid(nDirectoryEntryOffset))
                {
                    return 0;
                }

                nDirectoryEntryOffset=nResourceOffset+readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

                if(!isOffsetValid(nDirectoryEntryOffset))
                {
                    return 0;
                }

                unsigned int nSize=readDword(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DATA_ENTRY,Size));

                return nSize;

            }

            nCurrentNumber++;

            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
        }


        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return 0;
}

unsigned int PEFile::getResourceIdByNumber(unsigned int nNumber)
{
    unsigned int nCurrentNumber=0;

    if(!isResourcePresent())
    {
        return 0;
    }

    if(nNumber>=getNumberOfResources())
    {
        return 0;
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return 0;
        }

        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
        unsigned int nIDEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

        nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

        for(int j=0; j<nNameEntries2+nIDEntries2; j++)
        {
            if(nCurrentNumber==nNumber)
            {
                unsigned int nTemp=readDword(nDirectoryEntryOffset);

                if(!(nTemp&0x80000000))
                {
                    return nTemp&0xFFFF;
                }

                return 0;

            }

            nCurrentNumber++;

            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
        }


        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return 0;
}

QString PEFile::getResourceNameByNumber(unsigned int nNumber)
{
    unsigned int nCurrentNumber=0;

    if(!isResourcePresent())
    {
        return "";
    }

    if(nNumber>=getNumberOfResources())
    {
        return "";
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return "";
        }

        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
        unsigned int nIDEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

        nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

        for(int j=0; j<nNameEntries2+nIDEntries2; j++)
        {
            if(nCurrentNumber==nNumber)
            {
                unsigned int nTemp=readDword(nDirectoryEntryOffset);

                if(nTemp&0x80000000)
                {
                    nTemp&=0x7FFFFFFF;
                    nTemp+=nResourceOffset;

                    unsigned int nStringLength=readWord(nTemp);

                    if(!isOffsetAndSizeValid(nTemp+2,nStringLength*2))
                    {
                        return "";
                    }

                    QByteArray baName=readArray(nTemp+2,nStringLength*2);

                    return QString::fromUtf16((ushort *)(baName.data()),nStringLength);

                }

                return "";

            }

            nCurrentNumber++;

            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
        }


        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return "";
}

QString PEFile::getResourceTypeByNumber(unsigned int nNumber)
{
    QString sType="";
    unsigned int nCurrentNumber=0;

    if(!isResourcePresent())
    {
        return "";
    }

    if(nNumber>=getNumberOfResources())
    {
        return "";
    }

    unsigned int nResourceOffset=getResourceOffset();
    unsigned int nOffset=nResourceOffset;
    unsigned int nDirectoryEntryOffset=0;

    unsigned int nNameEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
    unsigned int nIDEntries=readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

    nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

    for(int i=0; i<nNameEntries+nIDEntries; i++)
    {
        nDirectoryEntryOffset=nResourceOffset+readDword(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,OffsetToData))&0x7FFFFFFF;
        sType=resourceIdToString(readWord(nOffset+offsetof(IMAGE_RESOURCE_DIRECTORY_ENTRY,Id)));

        if(!isOffsetValid(nDirectoryEntryOffset))
        {
            return "";
        }

        unsigned int nNameEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfNamedEntries));
        unsigned int nIDEntries2=readWord(nDirectoryEntryOffset+offsetof(IMAGE_RESOURCE_DIRECTORY,NumberOfIdEntries));

        nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY);

        for(int j=0; j<nNameEntries2+nIDEntries2; j++)
        {
            if(nCurrentNumber==nNumber)
            {
                return sType;
            }

            nCurrentNumber++;

            nDirectoryEntryOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
        }


        nOffset+=sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);
    }

    return "";
}

QString PEFile::getManifest()
{
    return getResourceIdDataFast(24);
}

unsigned int PEFile::getNETOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError(".NET is not present");
    return 0;
}

bool PEFile::initCLI()
{
//    #ifdef QT_DEBUG
//        qDebug("bool PEFile::initCLI()");
//    #endif

    IMAGE_DATA_DIRECTORY idd;
    unsigned int nOffset;

    if(isNETPresent())
    {
        if(!CLI_Section.bIsInit)
        {
//            #ifdef QT_DEBUG
//                qDebug("CLI_Section.bIsInit=0");
//            #endif

            CLI_Section.nEntryPointSize=0;

            getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR,&idd);
            CLI_Section.nCLIHeaderOffset=RVAToOffset(idd.VirtualAddress);
            CLI_Section.nCLI_Size=readDword(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,cb));
            CLI_Section.sCLI_MajorRuntimeVersion=readWord(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,MajorRuntimeVersion));
            CLI_Section.sCLI_MinorRuntimeVersion=readWord(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,MinorRuntimeVersion));;
            CLI_Section.nCLI_Flags=readDword(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,Flags));
            CLI_Section.nCLI_EntryPoint=readDword(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,EntryPointRVA));

            readArray(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,MetaData),(char *)&idd,sizeof(IMAGE_DATA_DIRECTORY));
            CLI_Section.CLI_MetaData.VirtualAddress=idd.VirtualAddress;
            CLI_Section.CLI_MetaData.Size=idd.Size;
            readArray(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,Resources),(char *)&idd,sizeof(IMAGE_DATA_DIRECTORY));
            CLI_Section.CLI_Resources.VirtualAddress=idd.VirtualAddress;
            CLI_Section.CLI_Resources.Size=idd.Size;
            readArray(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,StrongNameSignature),(char *)&idd,sizeof(IMAGE_DATA_DIRECTORY));
            CLI_Section.CLI_StrongNameSignature.VirtualAddress=idd.VirtualAddress;
            CLI_Section.CLI_StrongNameSignature.Size=idd.Size;
            readArray(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,CodeManagerTable),(char *)&idd,sizeof(IMAGE_DATA_DIRECTORY));
            CLI_Section.CLI_CodeManagerTable.VirtualAddress=idd.VirtualAddress;
            CLI_Section.CLI_CodeManagerTable.Size=idd.Size;
            readArray(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,VTableFixups),(char *)&idd,sizeof(IMAGE_DATA_DIRECTORY));
            CLI_Section.CLI_VTableFixups.VirtualAddress=idd.VirtualAddress;
            CLI_Section.CLI_VTableFixups.Size=idd.Size;
            readArray(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,ExportAddressTableJumps),(char *)&idd,sizeof(IMAGE_DATA_DIRECTORY));
            CLI_Section.CLI_ExportAddressTableJumps.VirtualAddress=idd.VirtualAddress;
            CLI_Section.CLI_ExportAddressTableJumps.Size=idd.Size;
            readArray(CLI_Section.nCLIHeaderOffset+offsetof(IMAGE_COR20_HEADER,ManagedNativeHeader),(char *)&idd,sizeof(IMAGE_DATA_DIRECTORY));
            CLI_Section.CLI_ManagedNativeHeader.VirtualAddress=idd.VirtualAddress;
            CLI_Section.CLI_ManagedNativeHeader.Size=idd.Size;

            CLI_Section.nEntryPoint=CLI_Section.nCLI_EntryPoint;

            CLI_Section.nCLI_MetaDataOffset=0;
            CLI_Section.nCLI_MetaData_Signature=0;
            CLI_Section.sCLI_MetaData_MajorVersion=0;
            CLI_Section.sCLI_MetaData_MinorVersion=0;
            CLI_Section.nCLI_MetaData_Reserved=0;
            CLI_Section.sCLI_MetaData_Version="";
            CLI_Section.sCLI_MetaData_Flags=0;
            CLI_Section.sCLI_MetaData_Streams=0;

            CLI_Section.listCLI_MetaData_Stream_Names.clear();
            CLI_Section.listCLI_MetaData_Stream_Offsets.clear();
            CLI_Section.listCLI_MetaData_Stream_Sizes.clear();

            CLI_Section.nCLI_MetaData_TablesHeaderOffset=0;
            CLI_Section.nCLI_MetaData_TablesSize=0;

            CLI_Section.nCLI_MetaData_StringsOffset=0;
            CLI_Section.nCLI_MetaData_StringsSize=0;
            CLI_Section.nCLI_MetaData_StringsOffset=0;
            CLI_Section.nCLI_MetaData_StringsSize=0;
            CLI_Section.nCLI_MetaData_BlobOffset=0;
            CLI_Section.nCLI_MetaData_BlobSize=0;
            CLI_Section.nCLI_MetaData_GUIDOffset=0;
            CLI_Section.nCLI_MetaData_GUIDSize=0;

            CLI_Section.nCLI_MetaData_Tables_Reserved1=0;
            CLI_Section.cCLI_MetaData_Tables_MajorVersion=0;
            CLI_Section.cCLI_MetaData_Tables_MinorVersion=0;
            CLI_Section.cCLI_MetaData_Tables_HeapOffsetSizes=0;
            CLI_Section.cCLI_MetaData_Tables_Reserved2=0;
            CLI_Section.nCLI_MetaData_Tables_Valid=0;
            CLI_Section.nCLI_MetaData_Tables_Sorted=0;
            CLI_Section.nCLI_MetaData_Tables_Valid_NumberOfRows=0;

            for(int i=0; i<64; i++)
            {
                CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[i]=0;
                CLI_Section.CLI_MetaData_Tables_TablesOffsets[i]=0;
                CLI_Section.CLI_MetaData_Tables_TablesSizes[i]=0;
            }


            if(CLI_Section.CLI_MetaData.VirtualAddress&&CLI_Section.CLI_MetaData.Size)
            {
                CLI_Section.nCLI_MetaDataOffset=RVAToOffset(CLI_Section.CLI_MetaData.VirtualAddress);
                CLI_Section.nCLI_MetaData_Signature=readDword(CLI_Section.nCLI_MetaDataOffset);

                if(CLI_Section.nCLI_MetaData_Signature==0x424a5342)
                {
                    CLI_Section.sCLI_MetaData_MajorVersion=readWord(CLI_Section.nCLI_MetaDataOffset+4);
                    CLI_Section.sCLI_MetaData_MinorVersion=readWord(CLI_Section.nCLI_MetaDataOffset+6);
                    CLI_Section.nCLI_MetaData_Reserved=readDword(CLI_Section.nCLI_MetaDataOffset+8);
                    CLI_Section.nCLI_MetaData_VersionStringLength=readDword(CLI_Section.nCLI_MetaDataOffset+12);

                    CLI_Section.sCLI_MetaData_Version=getString(CLI_Section.nCLI_MetaDataOffset+16,CLI_Section.nCLI_MetaData_VersionStringLength);
                    CLI_Section.sCLI_MetaData_Flags=readWord(CLI_Section.nCLI_MetaDataOffset+16+CLI_Section.nCLI_MetaData_VersionStringLength);
                    CLI_Section.sCLI_MetaData_Streams=readWord(CLI_Section.nCLI_MetaDataOffset+16+CLI_Section.nCLI_MetaData_VersionStringLength+2);


                    nOffset=CLI_Section.nCLI_MetaDataOffset+20+CLI_Section.nCLI_MetaData_VersionStringLength;

                    for(int i=0; i<CLI_Section.sCLI_MetaData_Streams; i++)
                    {
                        CLI_Section.listCLI_MetaData_Stream_Offsets.append(readDword(nOffset));
                        CLI_Section.listCLI_MetaData_Stream_Sizes.append(readDword(nOffset+4));
                        CLI_Section.listCLI_MetaData_Stream_Names.append(getString(nOffset+8));

                        if(CLI_Section.listCLI_MetaData_Stream_Names.at(i)=="#~")
                        {
                            CLI_Section.nCLI_MetaData_TablesHeaderOffset=CLI_Section.listCLI_MetaData_Stream_Offsets.at(i)+CLI_Section.nCLI_MetaDataOffset;
                            CLI_Section.nCLI_MetaData_TablesSize=CLI_Section.listCLI_MetaData_Stream_Sizes.at(i);
                        }
                        else if(CLI_Section.listCLI_MetaData_Stream_Names.at(i)=="#Strings")
                        {
                            CLI_Section.nCLI_MetaData_StringsOffset=CLI_Section.listCLI_MetaData_Stream_Offsets.at(i)+CLI_Section.nCLI_MetaDataOffset;
                            CLI_Section.nCLI_MetaData_StringsSize=CLI_Section.listCLI_MetaData_Stream_Sizes.at(i);
                        }
                        else if(CLI_Section.listCLI_MetaData_Stream_Names.at(i)=="#US")
                        {
                            CLI_Section.nCLI_MetaData_USOffset=CLI_Section.listCLI_MetaData_Stream_Offsets.at(i)+CLI_Section.nCLI_MetaDataOffset;
                            CLI_Section.nCLI_MetaData_USSize=CLI_Section.listCLI_MetaData_Stream_Sizes.at(i);
                        }
                        else if(CLI_Section.listCLI_MetaData_Stream_Names.at(i)=="#Blob")
                        {
                            CLI_Section.nCLI_MetaData_BlobOffset=CLI_Section.listCLI_MetaData_Stream_Offsets.at(i)+CLI_Section.nCLI_MetaDataOffset;
                            CLI_Section.nCLI_MetaData_BlobSize=CLI_Section.listCLI_MetaData_Stream_Sizes.at(i);
                        }
                        else if(CLI_Section.listCLI_MetaData_Stream_Names.at(i)=="#GUID")
                        {
                            CLI_Section.nCLI_MetaData_GUIDOffset=CLI_Section.listCLI_MetaData_Stream_Offsets.at(i)+CLI_Section.nCLI_MetaDataOffset;
                            CLI_Section.nCLI_MetaData_GUIDSize=CLI_Section.listCLI_MetaData_Stream_Sizes.at(i);
                        }

                        nOffset+=8;
                        nOffset+=ALIGN_UP(CLI_Section.listCLI_MetaData_Stream_Names.at(i).length()+1,4);
                    }

                    if(CLI_Section.nCLI_MetaData_TablesHeaderOffset)
                    {
                        CLI_Section.nCLI_MetaData_Tables_Reserved1=readDword(CLI_Section.nCLI_MetaData_TablesHeaderOffset);
                        CLI_Section.cCLI_MetaData_Tables_MajorVersion=readByte(CLI_Section.nCLI_MetaData_TablesHeaderOffset+4);
                        CLI_Section.cCLI_MetaData_Tables_MinorVersion=readByte(CLI_Section.nCLI_MetaData_TablesHeaderOffset+5);
                        CLI_Section.cCLI_MetaData_Tables_HeapOffsetSizes=readByte(CLI_Section.nCLI_MetaData_TablesHeaderOffset+6);
                        CLI_Section.cCLI_MetaData_Tables_Reserved2=readByte(CLI_Section.nCLI_MetaData_TablesHeaderOffset+7);
                        CLI_Section.nCLI_MetaData_Tables_Valid=readQword(CLI_Section.nCLI_MetaData_TablesHeaderOffset+8);
                        CLI_Section.nCLI_MetaData_Tables_Sorted=readQword(CLI_Section.nCLI_MetaData_TablesHeaderOffset+16);


                        unsigned long long nValid=CLI_Section.nCLI_MetaData_Tables_Valid;


                        unsigned int nTemp=0;

                        for(nTemp = 0; nValid; nTemp++)
                        {
                            nValid &= nValid - 1;
                        }


                        CLI_Section.nCLI_MetaData_Tables_Valid_NumberOfRows=nTemp;

                        nOffset=CLI_Section.nCLI_MetaData_TablesHeaderOffset+24;

                        for(int i=0; i<64; i++)
                        {
                            if(CLI_Section.nCLI_MetaData_Tables_Valid&((unsigned long long)1<<i))
                            {
                                CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[i]=readDword(nOffset);
                                nOffset+=4;
                            }
                        }

                        unsigned int nSize=0;
                        int nStringIndexSize=2;
                        int nGUIDIndexSize=2;
                        int nBLOBIndexSize=2;
                        int nResolutionScope=2;
                        int nTypeDefOrRef=2;
                        int nField=2;
                        int nMethodDef=2;
                        int nParamList=2;

                        unsigned char cHeapOffsetSizes=0;

                        cHeapOffsetSizes=CLI_Section.cCLI_MetaData_Tables_HeapOffsetSizes;

                        if(cHeapOffsetSizes&0x01)
                        {
                            nStringIndexSize=4;
                        }

                        if(cHeapOffsetSizes&0x02)
                        {
                            nGUIDIndexSize=4;
                        }

                        if(cHeapOffsetSizes&0x04)
                        {
                            nBLOBIndexSize=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[0]>0x3FFF)
                        {
                            nResolutionScope=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[26]>0x3FFF)
                        {
                            nResolutionScope=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[35]>0x3FFF)
                        {
                            nResolutionScope=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[1]>0x3FFF)
                        {
                            nResolutionScope=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[1]>0x3FFF)
                        {
                            nTypeDefOrRef=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[2]>0x3FFF)
                        {
                            nTypeDefOrRef=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[27]>0x3FFF)
                        {
                            nTypeDefOrRef=4;
                        }


                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[4]>0xFFFF)
                        {
                            nField=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[6]>0xFFFF)
                        {
                            nMethodDef=4;
                        }

                        if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[8]>0xFFFF)
                        {
                            nParamList=4;
                        }

                        nSize=0;
                        nSize+=2;
                        nSize+=nStringIndexSize;
                        nSize+=nGUIDIndexSize;
                        nSize+=nGUIDIndexSize;
                        nSize+=nGUIDIndexSize;
                        CLI_Section.CLI_MetaData_Tables_TablesSizes[0]=nSize;
                        nSize=0;
                        nSize+=nResolutionScope;
                        nSize+=nStringIndexSize;
                        nSize+=nStringIndexSize;
                        CLI_Section.CLI_MetaData_Tables_TablesSizes[1]=nSize;
                        nSize=0;
                        nSize+=4;
                        nSize+=nStringIndexSize;
                        nSize+=nStringIndexSize;
                        nSize+=nTypeDefOrRef;
                        nSize+=nField;
                        nSize+=nMethodDef;
                        CLI_Section.CLI_MetaData_Tables_TablesSizes[2]=nSize;
                        nSize=0;
                        CLI_Section.CLI_MetaData_Tables_TablesSizes[3]=nSize;
                        nSize=0;
                        nSize+=2;
                        nSize+=nStringIndexSize;
                        nSize+=nBLOBIndexSize;
                        CLI_Section.CLI_MetaData_Tables_TablesSizes[4]=nSize;
                        nSize=0;
                        CLI_Section.CLI_MetaData_Tables_TablesSizes[5]=nSize;
                        nSize=0;
                        nSize+=4;
                        nSize+=2;
                        nSize+=2;
                        nSize+=nStringIndexSize;
                        nSize+=nBLOBIndexSize;
                        nSize+=nParamList;
                        CLI_Section.CLI_MetaData_Tables_TablesSizes[6]=nSize;


                        for(int i=0; i<64; i++)
                        {
                            if(CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[i])
                            {
                                CLI_Section.CLI_MetaData_Tables_TablesOffsets[i]=nOffset;
                                nOffset+=CLI_Section.CLI_MetaData_Tables_TablesSizes[i]*CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[i];
                            }
                        }

                        if(!(CLI_Section.nCLI_Flags&COMIMAGE_FLAGS_NATIVE_ENTRYPOINT))
                        {
                            if(((CLI_Section.nEntryPoint&0xFF000000)>>24)==6)
                            {
                                unsigned int nIndex=CLI_Section.nEntryPoint&0xFFFFFF;

                                if(nIndex<=CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[6])
                                {
                                    nOffset=CLI_Section.CLI_MetaData_Tables_TablesOffsets[6];
                                    nOffset+=CLI_Section.CLI_MetaData_Tables_TablesSizes[6]*(nIndex-1);

                                    CLI_Section.nEntryPoint=readDword(nOffset);
                                }
                                else
                                {
                                    CLI_Section.nEntryPoint=0;
                                    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
                                }


                            }
                            else
                            {
                                CLI_Section.nEntryPoint=0;
                            }
                        }
                    }
                    else
                    {
                        emit appendError(".NET. MetaData tables are not present");
                    }
                }
                else
                {
                    emit appendError(".NET invalid MetaData Signature");
                }


            }
            else
            {
                emit appendError(".NET MetaData is not present");
            }

            CLI_Section.bIsInit=true;

        }

        return true;
    }

    //    emit appendError(".NET is not present");

    return false;
}

unsigned int PEFile::getCLIHeaderOffset()
{
    if(initCLI())
    {
        return CLI_Section.nCLIHeaderOffset;
    }

    return 0;
}

unsigned int PEFile::getCLI_Size()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_Size;
    }

    return 0;
}

unsigned short PEFile::getCLI_MajorRuntimeVersion()
{
    if(initCLI())
    {
        return CLI_Section.sCLI_MajorRuntimeVersion;
    }

    return 0;
}

unsigned short PEFile::getCLI_MinorRuntimeVersion()
{
    if(initCLI())
    {
        return CLI_Section.sCLI_MinorRuntimeVersion;
    }

    return 0;
}

void PEFile::getCLI_MetaData(IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(initCLI())
    {
        pDataDirectory->VirtualAddress=CLI_Section.CLI_MetaData.VirtualAddress;
        pDataDirectory->Size=CLI_Section.CLI_MetaData.Size;
    }
    else
    {
        pDataDirectory->VirtualAddress=0;
        pDataDirectory->Size=0;
    }
}

unsigned int PEFile::getCLI_Flags()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_Flags;
    }

    return 0;
}

unsigned int PEFile::getCLI_EntryPoint()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_EntryPoint;
    }

    return 0;
}

void PEFile::getCLI_Resources(IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(initCLI())
    {
        pDataDirectory->VirtualAddress=CLI_Section.CLI_Resources.VirtualAddress;
        pDataDirectory->Size=CLI_Section.CLI_Resources.Size;
    }
    else
    {
        pDataDirectory->VirtualAddress=0;
        pDataDirectory->Size=0;
    }
}

void PEFile::getCLI_StrongNameSignature(IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(initCLI())
    {
        pDataDirectory->VirtualAddress=CLI_Section.CLI_StrongNameSignature.VirtualAddress;
        pDataDirectory->Size=CLI_Section.CLI_StrongNameSignature.Size;
    }
    else
    {
        pDataDirectory->VirtualAddress=0;
        pDataDirectory->Size=0;
    }
}

void PEFile::getCLI_CodeManagerTable(IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(initCLI())
    {
        pDataDirectory->VirtualAddress=CLI_Section.CLI_CodeManagerTable.VirtualAddress;
        pDataDirectory->Size=CLI_Section.CLI_CodeManagerTable.Size;
    }
    else
    {
        pDataDirectory->VirtualAddress=0;
        pDataDirectory->Size=0;
    }
}

void PEFile::getCLI_VTableFixups(IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(initCLI())
    {
        pDataDirectory->VirtualAddress=CLI_Section.CLI_VTableFixups.VirtualAddress;
        pDataDirectory->Size=CLI_Section.CLI_VTableFixups.Size;
    }
    else
    {
        pDataDirectory->VirtualAddress=0;
        pDataDirectory->Size=0;
    }
}

void PEFile::getCLI_ExportAddressTableJumps(IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(initCLI())
    {
        pDataDirectory->VirtualAddress=CLI_Section.CLI_ExportAddressTableJumps.VirtualAddress;
        pDataDirectory->Size=CLI_Section.CLI_ExportAddressTableJumps.Size;
    }
    else
    {
        pDataDirectory->VirtualAddress=0;
        pDataDirectory->Size=0;
    }
}

void PEFile::getCLI_ManagedNativeHeader(IMAGE_DATA_DIRECTORY *pDataDirectory)
{
    if(initCLI())
    {
        pDataDirectory->VirtualAddress=CLI_Section.CLI_ManagedNativeHeader.VirtualAddress;
        pDataDirectory->Size=CLI_Section.CLI_ManagedNativeHeader.Size;
    }
    else
    {
        pDataDirectory->VirtualAddress=0;
        pDataDirectory->Size=0;
    }
}

QByteArray PEFile::getCLI_MetaData()
{
    QByteArray baResult;
    IMAGE_DATA_DIRECTORY idd;

    if(initCLI())
    {
        getCLI_MetaData(&idd);
        baResult=readArray(RVAToOffset(idd.VirtualAddress),idd.Size);
    }

    return baResult;
}

unsigned int PEFile::getCLI_MetaData_Signature()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_MetaData_Signature;
    }

    return 0;
}

unsigned short PEFile::getCLI_MetaData_MajorVersion()
{
    if(initCLI())
    {
        return CLI_Section.sCLI_MetaData_MajorVersion;
    }

    return 0;
}

unsigned short PEFile::getCLI_MetaData_MinorVersion()
{
    if(initCLI())
    {
        return CLI_Section.sCLI_MetaData_MinorVersion;
    }

    return 0;
}

unsigned int PEFile::getCLI_MetaData_Reserved()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_MetaData_Reserved;
    }

    return 0;
}

QString PEFile::getCLI_MetaData_Version()
{
    QString sResult;

    if(initCLI())
    {
        return CLI_Section.sCLI_MetaData_Version;
    }

    return sResult;
}

unsigned short PEFile::getCLI_MetaData_Flags()
{
    if(initCLI())
    {
        return CLI_Section.sCLI_MetaData_Flags;
    }

    return 0;
}

unsigned short PEFile::getCLI_MetaData_Streams()
{
    if(initCLI())
    {
        return CLI_Section.sCLI_MetaData_Streams;
    }

    return 0;
}

unsigned int PEFile::getCLI_MetaData_Stream_Offset(int nStream)
{
    if(initCLI())
    {
        if(nStream<CLI_Section.sCLI_MetaData_Streams)
        {
            return CLI_Section.listCLI_MetaData_Stream_Offsets.at(nStream);
        }

        emit appendError(QString("Value out of range: n=%1").arg(nStream));
    }

    return 0;

}

unsigned int PEFile::getCLI_MetaData_Stream_Size(int nStream)
{
    if(initCLI())
    {
        if(nStream<CLI_Section.sCLI_MetaData_Streams)
        {
            return CLI_Section.listCLI_MetaData_Stream_Sizes.at(nStream);
        }

        emit appendError(QString("Value out of range: n=%1").arg(nStream));
    }

    return 0;
}

QString PEFile::getCLI_MetaData_Stream_Name(int nStream)
{
    if(initCLI())
    {
        if(nStream<CLI_Section.sCLI_MetaData_Streams)
        {
            return CLI_Section.listCLI_MetaData_Stream_Names.at(nStream);
        }

        emit appendError(QString("Value out of range: n=%1").arg(nStream));
    }

    return "";
}

unsigned int PEFile::getCLI_MetaDataOffset()
{

    //    IMAGE_DATA_DIRECTORY idd;
    //    getCLI_MetaData(&idd);

    //    return RVAToOffset(idd.VirtualAddress);

    if(initCLI())
    {
        return CLI_Section.nCLI_MetaDataOffset;
    }

    return 0;
}

unsigned int PEFile::getCLI_MetaData_TablesHeaderOffset()
{
    // TODO
    //    return getCLI_MetaDataStreamOffset("#~");
    if(initCLI())
    {
        return CLI_Section.nCLI_MetaData_TablesHeaderOffset;
    }

    return 0;
}

unsigned int PEFile::getCLI_MetaDataStreamOffset(QString sStream)
{
    unsigned int nResult=0;

    unsigned int nNumberOfStreams=getCLI_MetaData_Streams();

    for(int i=0; i<nNumberOfStreams; i++)
    {
        if(getCLI_MetaData_Stream_Name(i)==sStream)
        {
            nResult=getCLI_MetaData_Stream_Offset(i)+getCLI_MetaDataOffset();
            break;
        }
    }

    return nResult;
}

unsigned int PEFile::getCLI_MetaData_Tables_Reserved1()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_MetaData_Tables_Reserved1;
    }

    return 0;
}

unsigned char PEFile::getCLI_MetaData_Tables_MajorVersion()
{
    if(initCLI())
    {
        return CLI_Section.cCLI_MetaData_Tables_MajorVersion;
    }

    return 0;
}

unsigned char PEFile::getCLI_MetaData_Tables_MinorVersion()
{
    if(initCLI())
    {
        return CLI_Section.cCLI_MetaData_Tables_MinorVersion;
    }

    return 0;
}

unsigned char PEFile::getCLI_MetaData_Tables_HeapOffsetSizes()
{
    if(initCLI())
    {
        return CLI_Section.cCLI_MetaData_Tables_HeapOffsetSizes;
    }

    return 0;
}

unsigned char PEFile::getCLI_MetaData_Tables_Reserved2()
{
    if(initCLI())
    {
        return CLI_Section.cCLI_MetaData_Tables_Reserved2;
    }

    return 0;
}

unsigned long long PEFile::getCLI_MetaData_Tables_Valid()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_MetaData_Tables_Valid;
    }

    return 0;
}

unsigned long long PEFile::getCLI_MetaData_Tables_Sorted()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_MetaData_Tables_Sorted;
    }

    return 0;
}

unsigned int PEFile::getCLI_MetaData_Tables_Valid_NumberOfRows()
{
    if(initCLI())
    {
        return CLI_Section.nCLI_MetaData_Tables_Valid_NumberOfRows;
    }

    return 0;
}

bool PEFile::isCLI_MetaData_Tables_TablePresent(int nTable)
{
    //    unsigned long long nTest=(unsigned long long)1<<43;
    //    nTest=getCLI_MetaData_Tables_Valid()&((unsigned long long)1<<43);
    return ((getCLI_MetaData_Tables_Valid()&((unsigned long long)1<<nTable))!=0);
}

bool PEFile::isCLI_MetaData_Tables_TableSorted(int nTable)
{
    return ((getCLI_MetaData_Tables_Sorted()&((unsigned long long)1<<nTable))!=0);
}

QString PEFile::CLI_MetaData_Tables_TableToString(int nTable)
{

    switch(nTable)
    {
        case 0:
            return "Module";
            break;

        case 1:
            return "TypeRef";
            break;

        case 2:
            return "TypeDef";
            break;

        case 3:
            return "unknown3";
            break;

        case 4:
            return "Field";
            break;

        case 5:
            return "unknown5";
            break;

        case 6:
            return "MethodDef";
            break;

        case 7:
            return "unknown7";
            break;

        case 8:
            return "Param";
            break;

        case 9:
            return "InterfaceImpl";
            break;

        case 10:
            return "MemberRef";
            break;

        case 11:
            return "Constant";
            break;

        case 12:
            return "CustomAttribute";
            break;

        case 13:
            return "FieldMarshal";
            break;

        case 14:
            return "DeclSecurity";
            break;

        case 15:
            return "ClassLayout";
            break;

        case 16:
            return "FieldLayout";
            break;

        case 17:
            return "StandAloneSig";
            break;

        case 18:
            return "EventMap";
            break;

        case 19:
            return "unknown19";
            break;

        case 20:
            return "Event";
            break;

        case 21:
            return "PropertyMap";
            break;

        case 22:
            return "unknown22";
            break;

        case 23:
            return "Property";
            break;

        case 24:
            return "MethodSemantics";
            break;

        case 25:
            return "MethodImpl";
            break;

        case 26:
            return "ModuleRef";
            break;

        case 27:
            return "TypeSpec";
            break;

        case 28:
            return "ImplMap";
            break;

        case 29:
            return "FieldRVA";
            break;

        case 30:
            return "unknown30";
            break;

        case 31:
            return "unknown31";
            break;

        case 32:
            return "Assembly";
            break;

        case 33:
            return "AssemblyProcessor";
            break;

        case 34:
            return "AssemblyOS";
            break;

        case 35:
            return "AssemblyRef";
            break;

        case 36:
            return "AssemblyRefProcessor";
            break;

        case 37:
            return "AssemblyRefOS";
            break;

        case 38:
            return "File";
            break;

        case 39:
            return "ExportedType";
            break;

        case 40:
            return "ManifestResource";
            break;

        case 41:
            return "NestedClass";
            break;

        case 42:
            return "GenericParam";
            break;

        case 43:
            return "unknown43";
            break;

        case 44:
            return "GenericParamConstraint";
            break;

        case 45:
            return "unknown45";
            break;

        case 46:
            return "unknown46";
            break;

        case 47:
            return "unknown47";
            break;

        case 48:
            return "unknown48";
            break;

        case 49:
            return "unknown49";
            break;

        case 50:
            return "unknown50";
            break;

        case 51:
            return "unknown51";
            break;

        case 52:
            return "unknown52";
            break;

        case 53:
            return "unknown53";
            break;

        case 54:
            return "unknown54";
            break;

        case 55:
            return "unknown55";
            break;

        case 56:
            return "unknown56";
            break;

        case 57:
            return "unknown57";
            break;

        case 58:
            return "unknown58";
            break;

        case 59:
            return "unknown59";
            break;

        case 60:
            return "unknown60";
            break;

        case 61:
            return "unknown61";
            break;

        case 62:
            return "unknown62 ";
            break;

        case 63:
            return "unknown63";
            break;

        default:
            return "unknownXXX";
    }
}

unsigned int PEFile::getCLI_MetaData_Tables_TablesNumberOfIndexes(int nTable)
{
    //    unsigned int nOffset=0;
    //    unsigned int nOffset2=0;
    //    unsigned int nResult;
    //    unsigned long long nValid;

    //    nOffset=getCLI_MetaData_TablesHeaderOffset();

    //    if((nOffset!=-1)&&(nOffset!=0))
    //    {
    //        nValid=getCLI_MetaData_Tables_Valid();


    //        if((nValid&((unsigned long long)1<<nTable))!=0)
    //        {
    //            for(int i=0;i<nTable;i++)
    //            {
    //                if((nValid&((unsigned long long)1<<i))!=0)
    //                {
    //                    nOffset2+=4;
    //                }
    //            }
    //            nResult=readDword(nOffset+nOffset2+24);;
    //            return nResult;
    //        }

    //    }


    //    return 0;

    if(initCLI())
    {
        if(nTable<64)
        {
            return CLI_Section.CLI_MetaData_Tables_TablesNumberOfIndexes[nTable];
        }

        emit appendError(QString("Value out of range: n=%1").arg(nTable));
    }

    return 0;
}

unsigned int PEFile::getCLI_MetaData_Tables_SizeOfHeader(int nTable)
{
    if(initCLI())
    {
        if(nTable<64)
        {
            return CLI_Section.CLI_MetaData_Tables_TablesSizes[nTable];
        }

        emit appendError(QString("Value out of range: n=%1").arg(nTable));
    }

    return 0;
    //    unsigned int nResult=0;
    //    int nStringIndexSize=2;
    //    int nGUIDIndexSize=2;
    //    int nBLOBIndexSize=2;
    //    int nResolutionScope=2;
    //    int nTypeDefOrRef=2;
    //    int nField=2;
    //    int nMethodDef=2;
    //    int nParamList=2;

    //    unsigned char cHeapOffsetSizes=0;

    //    cHeapOffsetSizes=getCLI_MetaData_Tables_HeapOffsetSizes();

    //    if(cHeapOffsetSizes&0x01)
    //    {
    //        nStringIndexSize=4;
    //    }

    //    if(cHeapOffsetSizes&0x02)
    //    {
    //        nGUIDIndexSize=4;
    //    }

    //    if(cHeapOffsetSizes&0x04)
    //    {
    //        nBLOBIndexSize=4;
    //    }

    //    if(getCLI_MetaData_Tables_NumberOfRows(0)>0x3FFF)
    //    {
    //        nResolutionScope=4;
    //    }
    //    if(getCLI_MetaData_Tables_NumberOfRows(26)>0x3FFF)
    //    {
    //        nResolutionScope=4;
    //    }
    //    if(getCLI_MetaData_Tables_NumberOfRows(35)>0x3FFF)
    //    {
    //        nResolutionScope=4;
    //    }
    //    if(getCLI_MetaData_Tables_NumberOfRows(1)>0x3FFF)
    //    {
    //        nResolutionScope=4;
    //    }

    //    if(getCLI_MetaData_Tables_NumberOfRows(1)>0x3FFF)
    //    {
    //        nTypeDefOrRef=4;
    //    }
    //    if(getCLI_MetaData_Tables_NumberOfRows(2)>0x3FFF)
    //    {
    //        nTypeDefOrRef=4;
    //    }
    //    if(getCLI_MetaData_Tables_NumberOfRows(27)>0x3FFF)
    //    {
    //        nTypeDefOrRef=4;
    //    }


    //    if(getCLI_MetaData_Tables_NumberOfRows(4)>0xFFFF)
    //    {
    //        nField=4;
    //    }
    //    if(getCLI_MetaData_Tables_NumberOfRows(6)>0xFFFF)
    //    {
    //        nMethodDef=4;
    //    }
    //    if(getCLI_MetaData_Tables_NumberOfRows(8)>0xFFFF)
    //    {
    //        nParamList=4;
    //    }

    //    switch(nTable)
    //    {
    //    case 0:
    //        nResult+=2;
    //        nResult+=nStringIndexSize;
    //        nResult+=nGUIDIndexSize;
    //        nResult+=nGUIDIndexSize;
    //        nResult+=nGUIDIndexSize;
    //        break;
    //    case 1:
    //        nResult+=nResolutionScope;
    //        nResult+=nStringIndexSize;
    //        nResult+=nStringIndexSize;
    //        break;
    //    case 2:
    //        nResult+=4;
    //        nResult+=nStringIndexSize;
    //        nResult+=nStringIndexSize;
    //        nResult+=nTypeDefOrRef;
    //        nResult+=nField;
    //        nResult+=nMethodDef;
    //        break;
    //    case 3:
    //        nResult=0;
    //    case 4:
    //        nResult+=2;
    //        nResult+=nStringIndexSize;
    //        nResult+=nBLOBIndexSize;
    //        break;
    //    case 5:
    //        nResult=0;
    //        break;
    //    case 6:
    //        nResult+=4;
    //        nResult+=2;
    //        nResult+=2;
    //        nResult+=nStringIndexSize;
    //        nResult+=nBLOBIndexSize;
    //        nResult+=nParamList;
    //        break;

    //    default:
    //        nResult=0;
    //    }

    //    return nResult;
}

unsigned int PEFile::getCLI_MetaData_Tables_HeadersOffset(int nTable)
{
    //    unsigned int nOffset=getCLI_MetaData_TablesHeaderOffset()+24+4*getCLI_MetaData_Tables_Valid_NumberOfRows();

    //    for(int i=0;i<nTable;i++)
    //    {
    //        nOffset+=getCLI_MetaData_Tables_TablesNumberOfIndexes(i)*getCLI_MetaData_Tables_SizeOfHeader(i);
    //    }

    //    return nOffset;

    if(initCLI())
    {
        if(nTable<64)
        {
            return CLI_Section.CLI_MetaData_Tables_TablesOffsets[nTable];
        }

        emit appendError(QString("Value out of range: n=%1").arg(nTable));
    }

    return 0;
}

unsigned int PEFile::getCLI_MetaData_Tables_MethodDef_RVA(int nIndex)
{
    if(initCLI())
    {
        if(nIndex<getCLI_MetaData_Tables_TablesNumberOfIndexes(6))
        {
            unsigned int nOffset=getCLI_MetaData_Tables_HeadersOffset(6);
            nOffset+=getCLI_MetaData_Tables_SizeOfHeader(6)*(nIndex-1);

            return readDword(nOffset);
        }

        emit appendError(QString("Value out of range: n=%1").arg(nIndex));
    }



    return 0;
}

unsigned int PEFile::getCLIEntryPoint()
{
    if(initCLI())
    {
        return CLI_Section.nEntryPoint;
    }

    return 0;
    //    unsigned int nOffset=0;
    //    unsigned int nEntryPoint=0;
    //    unsigned int nFlags=0;

    //    nOffset=getCLIHeaderOffset();

    //    if((nOffset!=-1)&&(nOffset!=0))
    //    {
    //        nEntryPoint=readDword(nOffset+offsetof(IMAGE_COR20_HEADER,EntryPointRVA));
    //        nFlags=readDword(nOffset+offsetof(IMAGE_COR20_HEADER,Flags));

    //        if(!(nFlags&COMIMAGE_FLAGS_NATIVE_ENTRYPOINT))
    //        {
    ////            unsigned int nNumberOfSreams=0;
    ////            unsigned int nMetaDataOffset=0;
    ////            IMAGE_DATA_DIRECTORY idd;
    ////            getCLI_MetaData(&idd);
    ////            nMetaDataOffset=RVAToOffset(idd.VirtualAddress);
    ////            nNumberOfSreams=readWord(nMetaDataOffset+18+readDword(nMetaDataOffset+12));


    //            if(((nEntryPoint&0xFF000000)>>24)==6)
    //            {
    //                nEntryPoint=getCLI_MetaData_Tables_MethodDef_RVA(nEntryPoint&0xFFFFFF);
    //            }
    //            else
    //            {
    //                nEntryPoint=0;
    //            }

    //        }
    //    }

    //    return nEntryPoint;
}

void PEFile::entryPointLoad_NET()
{
    if(initCLI())
    {
        unsigned int nEP=RVAToOffset(CLI_Section.nEntryPoint);
        file.seek(nEP);

        CLI_Section.nEntryPointSize=file.read(CLI_Section.pEntryPoint,EPCODE);

        bIsNETEntryPontLoad=true;
    }

}

bool PEFile::compareEP_NETfast(char *pszSignature)
{
    if(!bIsNETEntryPontLoad)
    {
        entryPointLoad_NET();
    }

    bool bResult=false;
    int nSize=strlen(pszSignature)/2;
    char *pBuffer=new char[nSize*2+1];

    if(nSize<=CLI_Section.nEntryPointSize)
    {
        dataToHex(CLI_Section.pEntryPoint,nSize,pBuffer);
        bResult=signatureCompare(pBuffer,pszSignature,nSize*2);
    }

    delete[] pBuffer;

    return bResult;
}

bool PEFile::compareEP_NETfast(QString sSignature)
{
    return compareEP_NETfast(convertSignature(sSignature).toAscii().data());
}

bool PEFile::compareEPfast(QString sSignature)
{
    if(!bIsEntryPontLoad)
    {
        entryPointLoad();
    }

    return Binary::compareEPfast(sSignature,0);
}

bool PEFile::compareEP_NET(QString sSignature, unsigned int nOffset)
{
    return _compare(convertSignature(sSignature),RVAToOffset(CLI_Section.nEntryPoint)+nOffset);
}

QByteArray PEFile::getCLI_MetaData_Strings()
{
    QByteArray baResult;

    if(initCLI())
    {
        baResult=readArray(CLI_Section.nCLI_MetaData_StringsOffset,CLI_Section.nCLI_MetaData_StringsSize);
    }

    return baResult;
}

QByteArray PEFile::getCLI_MetaData_US()
{
//#ifdef QT_DEBUG
//    qDebug("QByteArray PEFile::getCLI_MetaData_US()");
//#endif

    QByteArray baResult;

    if(initCLI())
    {
        baResult=readArray(CLI_Section.nCLI_MetaData_USOffset,CLI_Section.nCLI_MetaData_USSize);
    }

    return baResult;
}

QByteArray PEFile::getCLI_MetaData_Blob()
{
    QByteArray baResult;

    if(initCLI())
    {
        baResult=readArray(CLI_Section.nCLI_MetaData_BlobOffset,CLI_Section.nCLI_MetaData_BlobSize);
    }

    return baResult;
}

QByteArray PEFile::getCLI_MetaData_GUID()
{
    QByteArray baResult;

    if(initCLI())
    {
        baResult=readArray(CLI_Section.nCLI_MetaData_GUIDOffset,CLI_Section.nCLI_MetaData_GUIDSize);
    }

    return baResult;
}

QList<QString> PEFile::getCLI_MetaData_StringsAsList()
{
    QList<QString> listResult;
    QByteArray baStrings=getCLI_MetaData_Strings();
    QString sTemp;

    char *pOffset=baStrings.data();
    int nSize=baStrings.size();

    for(int i=1; i<nSize; i++)
    {
        pOffset++;
        sTemp=pOffset;
        listResult.append(sTemp);

        pOffset+=sTemp.size();
        i+=sTemp.size();

    }

    return listResult;
}

QList<QString> PEFile::getCLI_MetaData_USAsList()
{
    QList<QString> listResult;
    QByteArray baStrings=getCLI_MetaData_US();
    QString sTemp;
    int nStringSize;

    char *_pOffset=baStrings.data();
    char *pOffset=_pOffset;
    int nSize=baStrings.size();

    pOffset++;

    for(int i=1; i<nSize; i++)
    {

        nStringSize=(*((unsigned char *)pOffset));

        if(nStringSize==0x80)
        {
            nStringSize=0;
        }

        if(nStringSize>nSize-i)
        {
            break;
        }

        pOffset++;
        if(pOffset>_pOffset+nSize)
        {
            break;
        }
        sTemp=QString::fromUtf16((ushort *)pOffset,nStringSize/2);

        listResult.append(sTemp);

        pOffset+=nStringSize;
        i+=nStringSize;

    }

    return listResult;
}

bool PEFile::isCLI_MetaData_StringPresent(QString sString)
{
    QList<QString> list=getCLI_MetaData_StringsAsList();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i)==sString)
        {
            return true;
        }
    }

    return false;
}

bool PEFile::isCLI_MetaData_UnicodeStringPresent(QString sString)
{
    QList<QString> list=getCLI_MetaData_USAsList();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i)==sString)
        {
            return true;
        }
    }

    return false;
}

QByteArray PEFile::getRelocsAsXML()
{
    if(!isRelocsPresent())
    {
        return 0;
    }

    QByteArray baResult;

    QXmlStreamWriter xml(&baResult);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("Relocs");


    unsigned int nRelocsOffset;

    nRelocsOffset=getRelocsTableOffset();

    unsigned int nRVA;
    unsigned int nSize;
    unsigned short sTypeOffset;

    if(nRelocsOffset)
    {
        //        resourceToXML(nResourceOffset,nResourceOffset,&xml,0);
        while(true)
        {
            nRVA=readDword(nRelocsOffset);
            nSize=readDword(nRelocsOffset+4);

            if((nRVA==0)&&(nSize==0))
            {
                break;
            }

            xml.writeStartElement("Page");
            xml.writeAttribute("Address",QString::number(nRVA));
            xml.writeAttribute("NumberOfRelocs",QString::number((nSize-8)/2));

            for(int i=0; i<(nSize-8)/2; i++)
            {
                sTypeOffset=readWord(nRelocsOffset+8+i*2);

                xml.writeStartElement("Reloc");
                xml.writeAttribute("Type",QString::number(sTypeOffset>>12));
                xml.writeAttribute("Offset",QString::number(sTypeOffset&0xFFF));
                xml.writeEndElement();
            }

            nRelocsOffset+=nSize;

            xml.writeEndElement();
        }


        //        xml.writeAttribute("NumberOfRelocs",QString::number(ird.Characteristics));
        //        pXml->writeAttribute("MajorVersion",QString::number(ird.MajorVersion));
        //        pXml->writeAttribute("MinorVersion",QString::number(ird.MinorVersion));



    }

    xml.writeEndElement();
    xml.writeEndDocument();

    return baResult;
}

QList<unsigned int> PEFile::getRelocsAsList()
{
    QList<unsigned int> listResult;

    if(isRelocsPresent())
    {
        unsigned int nRelocsOffset;

        nRelocsOffset=getRelocsTableOffset();

        unsigned int nRVA;
        unsigned int nSize;
        unsigned short sTypeOffset;

        if(nRelocsOffset)
        {
            while(true)
            {


                nRVA=readDword(nRelocsOffset);
                nSize=readDword(nRelocsOffset+4);

                if((nRVA==0)&&(nSize==0))
                {
                    break;
                }


                for(int i=0; i<(nSize-8)/2; i++)
                {
                    sTypeOffset=readWord(nRelocsOffset+8+i*2);

                    listResult.append((sTypeOffset&0xFFF)+nRVA);
                }

                nRelocsOffset+=nSize;

            }
        }
    }

    return listResult;
}

bool PEFile::addRelocsSectionFromList(QList<unsigned int> *pList, QString sSectionName)
{
    IMAGE_SECTION_HEADER ish= {0};

    QByteArray baSection;

    IMAGE_BASE_RELOCATION *pIBR=(IMAGE_BASE_RELOCATION *)baSection.data();
    IMAGE_BASE_RELOCATION IBR= {0};



    //    QList<unsigned int> listPageAddresses;
    //    QList<unsigned int> listPages;
    unsigned int nPageAddress=0;
    unsigned int nCount=0;

    for(int i=0; i<pList->count(); i++)
    {
        if(0!=ALIGN_UP(pList->at(i),0x1000))
        {
            nPageAddress=ALIGN_UP(pList->at(i),0x1000);
        }
        else
        {

        }
    }

    return addSection(&ish,baSection);
}

unsigned int PEFile::getNumberOfRelocs()
{
    unsigned int nOffset;
    unsigned int nResult=0;
    unsigned int nAddress;
    unsigned int nSize;

    if(!isRelocsPresent())
    {
        emit appendError("Relocs is not present");
        return 0;
    }

    nOffset=getRelocsTableOffset();

    if((nOffset!=-1)&&(nOffset!=0))
    {
        do
        {
            nAddress=readDword(nOffset+offsetof(IMAGE_BASE_RELOCATION, VirtualAddress));
            nSize=readDword(nOffset+offsetof(IMAGE_BASE_RELOCATION, SizeOfBlock));

            if(nAddress&&nSize)
            {
                nResult+=(nSize-sizeof(IMAGE_BASE_RELOCATION))/2;
                nOffset+=nSize;
            }

        }
        while(nAddress&&nSize);

        return nResult;
    }

    emit appendError("Relocs is not present");
    return 0;
}

unsigned int PEFile::getNumberOfRelocsPages()
{
    unsigned int nOffset;
    unsigned int nResult=0;
    unsigned int nTemp;
    unsigned int nAddress;
    unsigned int nSize;

    if(!isRelocsPresent())
    {
        emit appendError("Relocs is not present");
        return 0;
    }

    nOffset=getRelocsTableOffset();

    if((nOffset!=-1)&&(nOffset!=0))
    {
        do
        {
            nAddress=readDword(nOffset+offsetof(IMAGE_BASE_RELOCATION, VirtualAddress));
            nSize=readDword(nOffset+offsetof(IMAGE_BASE_RELOCATION, SizeOfBlock));

            nResult++;

            nOffset+=nSize;

        }
        while(nAddress&&nSize);

        return nResult-1;
    }

    emit appendError("Relocs is not present");
    return 0;
}

unsigned int PEFile::getRelocsTableOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError("Relocs is not present");
    return 0;
}

int PEFile::getRelocsSection()
{
    if(!isRelocsPresent())
    {
        emit appendWarning("Relocs is not present");
        return 0;
    }

    IMAGE_DATA_DIRECTORY idd;
    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC,&idd);

    return RVAToSection(idd.VirtualAddress);
}

unsigned int PEFile::getTLSSection()
{
    if(!isRelocsPresent())
    {
        emit appendWarning("TLS is not present");
        return 0;
    }

    IMAGE_DATA_DIRECTORY idd;
    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_TLS,&idd);

    return RVAToSection(idd.VirtualAddress);
}

unsigned int PEFile::getTLSTableOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_TLS,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError("TLS is not present");
    return 0;
}

unsigned int PEFile::getTLS_StartAddressOfRawData()
{
    //    if(isPEPlus())
    //    {
    //        return (unsigned int)readQword(getNtHeadersOffset()+offsetof(IMAGE_NT_HEADERS64,OptionalHeader.ImageBase));
    //    }
    return readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,StartAddressOfRawData));
}

unsigned long long PEFile::getTLS_StartAddressOfRawData64()
{
    return readQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,StartAddressOfRawData));
}

unsigned int PEFile::getTLS_EndAddressOfRawData()
{
    return readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,EndAddressOfRawData));
}

unsigned long long PEFile::getTLS_EndAddressOfRawData64()
{
    return readQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,EndAddressOfRawData));
}

unsigned int PEFile::getTLS_AddressOfIndex()
{
    return readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,AddressOfIndex));
}

unsigned long long PEFile::getTLS_AddressOfIndex64()
{
    return readQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,AddressOfIndex));
}

unsigned int PEFile::getTLS_AddressOfCallBacks()
{
    return readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,AddressOfCallBacks));
}

unsigned long long PEFile::getTLS_AddressOfCallBacks64()
{
    return readQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,AddressOfCallBacks));
}

unsigned int PEFile::getTLS_SizeOfZeroFill()
{
    if(isPEPlus())
    {
        return (unsigned int)readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,SizeOfZeroFill));
    }

    return readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,SizeOfZeroFill));
}

unsigned int PEFile::getTLS_Characteristics()
{
    if(isPEPlus())
    {
        return (unsigned int)readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,Characteristics));
    }

    return readDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,Characteristics));
}

void PEFile::setTLS_StartAddressOfRawData(unsigned int nValue)
{
    writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,StartAddressOfRawData),nValue);
}

void PEFile::setTLS_StartAddressOfRawData64(unsigned long long nValue)
{
    writeQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,StartAddressOfRawData),nValue);
}

void PEFile::setTLS_EndAddressOfRawData(unsigned int nValue)
{
    writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,EndAddressOfRawData),nValue);
}

void PEFile::setTLS_EndAddressOfRawData64(unsigned long long nValue)
{
    writeQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,EndAddressOfRawData),nValue);
}

void PEFile::setTLS_AddressOfIndex(unsigned int nValue)
{
    writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,AddressOfIndex),nValue);
}

void PEFile::setTLS_AddressOfIndex64(unsigned long long nValue)
{
    writeQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,AddressOfIndex),nValue);
}

void PEFile::setTLS_AddressOfCallBacks(unsigned int nValue)
{
    writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,AddressOfCallBacks),nValue);
}

void PEFile::setTLS_AddressOfCallBacks64(unsigned long long nValue)
{
    writeQword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,AddressOfCallBacks),nValue);
}

void PEFile::setTLS_SizeOfZeroFill(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,SizeOfZeroFill),nValue);
        return;
    }

    writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,SizeOfZeroFill),nValue);
}

void PEFile::setTLS_Characteristics(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY64,Characteristics),nValue);
        return;
    }

    writeDword(getTLSTableOffset()+offsetof(IMAGE_TLS_DIRECTORY32,Characteristics),nValue);
}

unsigned int PEFile::getLoadConfigTableOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError("Load Config is not present");
    return 0;
}

unsigned int PEFile::getLoadConfig_Size()
{
    if(isPEPlus())
    {
        return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,Size));
    }

    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,Size));
}

unsigned int PEFile::getLoadConfig_TimeDateStamp()
{
    if(isPEPlus())
    {
        return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,TimeDateStamp));
    }

    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,TimeDateStamp));
}

unsigned short PEFile::getLoadConfig_MajorVersion()
{
    if(isPEPlus())
    {
        return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,MajorVersion));
    }

    return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,MajorVersion));
}

unsigned short PEFile::getLoadConfig_MinorVersion()
{
    if(isPEPlus())
    {
        return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,MinorVersion));
    }

    return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,MinorVersion));
}

unsigned int PEFile::getLoadConfig_GlobalFlagsClear()
{
    if(isPEPlus())
    {
        return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GlobalFlagsClear));
    }

    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,GlobalFlagsClear));
}

unsigned int PEFile::getLoadConfig_GlobalFlagsSet()
{
    if(isPEPlus())
    {
        return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GlobalFlagsSet));
    }

    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,GlobalFlagsSet));
}

unsigned int PEFile::getLoadConfig_CriticalSectionDefaultTimeout()
{
    if(isPEPlus())
    {
        return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,CriticalSectionDefaultTimeout));
    }

    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,CriticalSectionDefaultTimeout));
}

void PEFile::setLoadConfig_Size(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,Size),nValue);
        return;
    }

    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,Size),nValue);
}

void PEFile::setLoadConfig_TimeDateStamp(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,TimeDateStamp),nValue);
        return;
    }

    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,TimeDateStamp),nValue);
}

void PEFile::setLoadConfig_MajorVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,MajorVersion),sValue);
        return;
    }

    writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,MajorVersion),sValue);
}

void PEFile::setLoadConfig_MinorVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,MinorVersion),sValue);
        return;
    }

    writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,MinorVersion),sValue);
}

void PEFile::setLoadConfig_GlobalFlagsClear(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GlobalFlagsClear),nValue);
        return;
    }

    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,GlobalFlagsClear),nValue);
}

void PEFile::setLoadConfig_GlobalFlagsSet(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GlobalFlagsSet),nValue);
        return;
    }

    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,GlobalFlagsSet),nValue);
}

void PEFile::setLoadConfig_CriticalSectionDefaultTimeout(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,CriticalSectionDefaultTimeout),nValue);
        return;
    }

    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,CriticalSectionDefaultTimeout),nValue);
}

unsigned int PEFile::getLoadConfig_DeCommitFreeBlockThreshold()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,DeCommitFreeBlockThreshold));
}

unsigned long long PEFile::getLoadConfig_DeCommitFreeBlockThreshold64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,DeCommitFreeBlockThreshold));
}

unsigned int PEFile::getLoadConfig_DeCommitTotalFreeThreshold()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,DeCommitTotalFreeThreshold));
}

unsigned long long PEFile::getLoadConfig_DeCommitTotalFreeThreshold64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,DeCommitTotalFreeThreshold));
}

unsigned int PEFile::getLoadConfig_LockPrefixTable()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,LockPrefixTable));
}

unsigned long long PEFile::getLoadConfig_LockPrefixTable64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,LockPrefixTable));
}

unsigned int PEFile::getLoadConfig_MaximumAllocationSize()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,MaximumAllocationSize));
}

unsigned long long PEFile::getLoadConfig_MaximumAllocationSize64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,MaximumAllocationSize));
}

unsigned int PEFile::getLoadConfig_VirtualMemoryThreshold()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,VirtualMemoryThreshold));
}

unsigned long long PEFile::getLoadConfig_VirtualMemoryThreshold64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,VirtualMemoryThreshold));
}

unsigned int PEFile::getLoadConfig_ProcessAffinityMask()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,ProcessAffinityMask));
}

unsigned long long PEFile::getLoadConfig_ProcessAffinityMask64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,ProcessAffinityMask));
}

void PEFile::setLoadConfig_DeCommitFreeBlockThreshold(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,DeCommitFreeBlockThreshold),nValue);
}

void PEFile::setLoadConfig_DeCommitFreeBlockThreshold64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,DeCommitFreeBlockThreshold),nValue);
}

void PEFile::setLoadConfig_DeCommitTotalFreeThreshold(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,DeCommitTotalFreeThreshold),nValue);
}

void PEFile::setLoadConfig_DeCommitTotalFreeThreshold64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,DeCommitTotalFreeThreshold),nValue);
}

void PEFile::setLoadConfig_LockPrefixTable(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,LockPrefixTable),nValue);
}

void PEFile::setLoadConfig_LockPrefixTable64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,LockPrefixTable),nValue);
}

void PEFile::setLoadConfig_MaximumAllocationSize(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,MaximumAllocationSize),nValue);
}

void PEFile::setLoadConfig_MaximumAllocationSize64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,MaximumAllocationSize),nValue);
}

void PEFile::setLoadConfig_VirtualMemoryThreshold(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,VirtualMemoryThreshold),nValue);
}

void PEFile::setLoadConfig_VirtualMemoryThreshold64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,VirtualMemoryThreshold),nValue);
}

void PEFile::setLoadConfig_ProcessAffinityMask(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,ProcessAffinityMask),nValue);
}

void PEFile::setLoadConfig_ProcessAffinityMask64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,ProcessAffinityMask),nValue);
}

unsigned int PEFile::getLoadConfig_ProcessHeapFlags()
{
    if(isPEPlus())
    {
        return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,ProcessHeapFlags));
    }

    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,ProcessHeapFlags));
}

unsigned short PEFile::getLoadConfig_CSDVersion()
{
    if(isPEPlus())
    {
        return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,CSDVersion));
    }

    return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,CSDVersion));
}

unsigned short PEFile::getLoadConfig_Reserved1()
{
    if(isPEPlus())
    {
        return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,Reserved1));
    }

    return readWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,Reserved1));
}

void PEFile::setLoadConfig_ProcessHeapFlags(unsigned int nValue)
{
    if(isPEPlus())
    {
        writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,ProcessHeapFlags),nValue);
        return;
    }

    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,ProcessHeapFlags),nValue);
}

void PEFile::setLoadConfig_CSDVersion(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,CSDVersion),sValue);
        return;
    }

    writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,CSDVersion),sValue);
}

void PEFile::setLoadConfig_Reserved1(unsigned short sValue)
{
    if(isPEPlus())
    {
        writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,Reserved1),sValue);
        return;
    }

    writeWord(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,Reserved1),sValue);
}

unsigned int PEFile::getLoadConfig_EditList()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,EditList));
}

unsigned long long PEFile::getLoadConfig_EditList64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,EditList));
}

unsigned int PEFile::getLoadConfig_SecurityCookie()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,SecurityCookie));
}

unsigned long long PEFile::getLoadConfig_SecurityCookie64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,SecurityCookie));
}

unsigned int PEFile::getLoadConfig_SEHandlerTable()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,SEHandlerTable));
}

unsigned long long PEFile::getLoadConfig_SEHandlerTable64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,SEHandlerTable));
}

unsigned int PEFile::getLoadConfig_SEHandlerCount()
{
    return readDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,SEHandlerCount));
}

unsigned long long PEFile::getLoadConfig_SEHandlerCount64()
{
    return readQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,SEHandlerCount));
}

void PEFile::setLoadConfig_EditList(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,EditList),nValue);
}

void PEFile::setLoadConfig_EditList64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,EditList),nValue);
}

void PEFile::setLoadConfig_SecurityCookie(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,SecurityCookie),nValue);
}

void PEFile::setLoadConfig_SecurityCookie64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,SecurityCookie),nValue);
}

void PEFile::setLoadConfig_SEHandlerTable(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,SEHandlerTable),nValue);
}

void PEFile::setLoadConfig_SEHandlerTable64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,SEHandlerTable),nValue);
}

void PEFile::setLoadConfig_SEHandlerCount(unsigned int nValue)
{
    writeDword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32,SEHandlerCount),nValue);
}

void PEFile::setLoadConfig_SEHandlerCount64(unsigned long long nValue)
{
    writeQword(getLoadConfigTableOffset()+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,SEHandlerCount),nValue);
}

unsigned int PEFile::getBoundImportTableOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError("Bound Import is not present");
    return 0;
}

unsigned int PEFile::getBoundImport_NumberOfImports()
{
    unsigned int nResult=0;
    unsigned short sNameOffset;

    unsigned int nOffset=getBoundImportTableOffset();

    if((nOffset!=-1)&&(nOffset!=0))
    {
        while(1)
        {
            sNameOffset=readWord(nOffset+offsetof(IMAGE_BOUND_IMPORT_DESCRIPTOR,OffsetModuleName));

            if(!sNameOffset)
            {
                break;
            }

            nResult++;
            nOffset+=sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR);
        }
    }


    return nResult;
}

unsigned int PEFile::getBoundImport_TimeDateStamp(unsigned int nImport)
{
    unsigned int nOffset;

    if(nImport<(int)getBoundImport_NumberOfImports())
    {
        nOffset=getBoundImportTableOffset();
        nOffset+=sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR)*nImport;

        return readDword(nOffset+offsetof(IMAGE_BOUND_IMPORT_DESCRIPTOR,TimeDateStamp));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}

unsigned short PEFile::getBoundImport_OffsetModuleName(unsigned int nImport)
{
    unsigned int nOffset;

    if(nImport<(int)getBoundImport_NumberOfImports())
    {
        nOffset=getBoundImportTableOffset();
        nOffset+=sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR)*nImport;

        return readWord(nOffset+offsetof(IMAGE_BOUND_IMPORT_DESCRIPTOR,OffsetModuleName));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}

unsigned short PEFile::getBoundImport_NumberOfModuleForwarderRefs(unsigned int nImport)
{
    unsigned int nOffset;

    if(nImport<(int)getBoundImport_NumberOfImports())
    {
        nOffset=getBoundImportTableOffset();
        nOffset+=sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR)*nImport;

        return readWord(nOffset+offsetof(IMAGE_BOUND_IMPORT_DESCRIPTOR,NumberOfModuleForwarderRefs));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}

QString PEFile::getBoundImport_ModuleNameAsString(unsigned int nImport)
{
    unsigned int nOffset;

    if(nImport<(int)getBoundImport_NumberOfImports())
    {
        nOffset=getBoundImportTableOffset();
        nOffset+=sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR)*nImport;

        return getString(getBoundImportTableOffset()+readWord(nOffset+offsetof(IMAGE_BOUND_IMPORT_DESCRIPTOR,OffsetModuleName)));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nImport));

    return 0;
}

unsigned int PEFile::getDebugTableOffset()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_DEBUG,&idd);

    if(idd.VirtualAddress)
    {
        return RVAToOffset(idd.VirtualAddress);
    }

    emit appendError("Debug directory is not present");
    return 0;
}

unsigned int PEFile::getDebugTableSize()
{
    IMAGE_DATA_DIRECTORY idd;

    getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_DEBUG,&idd);

    if(idd.VirtualAddress)
    {
        return idd.Size;
    }

    emit appendError("Debug directory is not present");
    return 0;
}

unsigned int PEFile::getDebug_NumberOfDirectories()
{
    unsigned int nResult=0;


    if(isDebugPresent())
    {
        nResult=getDebugTableSize()/sizeof(IMAGE_DEBUG_DIRECTORY);
    }


    return nResult;
}

unsigned int PEFile::getDebug_Characteristics(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readDword(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,Characteristics));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned int PEFile::getDebug_TimeDateStamp(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readDword(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,TimeDateStamp));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned short PEFile::getDebug_MajorVersion(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readWord(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,MajorVersion));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned short PEFile::getDebug_MinorVersion(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readWord(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,MinorVersion));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned int PEFile::getDebug_Type(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readDword(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,Type));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned int PEFile::getDebug_SizeOfData(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readDword(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,SizeOfData));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned int PEFile::getDebug_AddressOfRawData(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readDword(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,AddressOfRawData));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned int PEFile::getDebug_PointerToRawData(unsigned int nDebug)
{
    unsigned int nOffset;

    if(nDebug<(int)getDebug_NumberOfDirectories())
    {
        nOffset=getDebugTableOffset();
        nOffset+=sizeof(IMAGE_DEBUG_DIRECTORY)*nDebug;

        return readDword(nOffset+offsetof(IMAGE_DEBUG_DIRECTORY,PointerToRawData));
    }

    emit appendError(QString("Value out of range: n=%1").arg(nDebug));

    return 0;
}

unsigned long long PEFile::getEntryPointOffset()
{
    unsigned int nAddress=getOptionalHeader_AddressOfEntryPoint();
    return RVAToOffset(nAddress);
}
