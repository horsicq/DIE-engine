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
#ifndef ELFFILE_H
#define ELFFILE_H

#include "binary.h"

class ELFFile : public Binary
{
public:
    struct STRING_TABLE_RECORD
    {
        unsigned int nIndex;
        QString sString;
    };

    struct MACHINE
    {
        unsigned int nMachine;
        QString sName;
    };

    Q_OBJECT
public:
    explicit ELFFile(QObject *parent=nullptr);

    QByteArray getSectionNamesTable();
    QByteArray getSection(int nSection);

    void entryPointLoad();

    static QList<MACHINE> getMachineList();
private:

signals:
private slots:
    //    bool compareEPfast(char *pszSignature);
public slots:

    bool compareEPfast(QString sSignature);
    bool compareEP(QString sSignature,unsigned int nOffset=0);
    unsigned long long getEntryPointOffset();

    bool isValid();
    bool isELF64();

    bool isReverse();

    unsigned int getElfHeaderOffset();
    unsigned int getElfHeaderSize();

    unsigned int getElfHeader_magic();
    unsigned char getElfHeader_class();
    unsigned char getElfHeader_data();
    unsigned char getElfHeader_iversion();
    unsigned char getElfHeader_osabi();
    unsigned char getElfHeader_abiversion();
    unsigned char getElfHeader_pad(int nIndex);

    void setElfHeader_magic(unsigned int nValue);
    void setElfHeader_class(unsigned char cValue);
    void setElfHeader_data(unsigned char cValue);
    void setElfHeader_iversion(unsigned char cValue);
    void setElfHeader_osabi(unsigned char cValue);
    void setElfHeader_abiversion(unsigned char cValue);
    void setElfHeader_pad(unsigned char cValue,int nIndex);

    unsigned short getElfHeader_type();
    unsigned short getElfHeader_machine();
    QString getMachineAsString();
    unsigned int getElfHeader_version();
    unsigned int getElfHeader_entry();
    unsigned long long getElfHeader_entry64();
    unsigned int getElfHeader_phoff();
    unsigned long long getElfHeader_phoff64();
    unsigned int getElfHeader_shoff();
    unsigned long long getElfHeader_shoff64();
    unsigned int getElfHeader_flags();
    unsigned short getElfHeader_ehsize();
    unsigned short getElfHeader_phentsize();
    unsigned short getElfHeader_phnum();
    unsigned short getElfHeader_shentsize();
    unsigned short getElfHeader_shnum();
    unsigned short getElfHeader_shstrndx();

    void setElfHeader_type(unsigned short sValue);
    void setElfHeader_machine(unsigned short sValue);
    void setElfHeader_version(unsigned int nValue);
    void setElfHeader_entry(unsigned int nValue);
    void setElfHeader_entry64(unsigned long long nValue);
    void setElfHeader_phoff(unsigned int nValue);
    void setElfHeader_phoff64(unsigned long long nValue);
    void setElfHeader_shoff(unsigned int nValue);
    void setElfHeader_shoff64(unsigned long long nValue);
    void setElfHeader_flags(unsigned int nValue);
    void setElfHeader_ehsize(unsigned short sValue);
    void setElfHeader_phentsize(unsigned short sValue);
    void setElfHeader_phnum(unsigned short sValue);
    void setElfHeader_shentsize(unsigned short sValue);
    void setElfHeader_shnum(unsigned short sValue);
    void setElfHeader_shstrndx(unsigned short sValue);

    unsigned int getSectionHeader32_name(unsigned int nSection);
    unsigned int getSectionHeader32_type(unsigned int nSection);
    unsigned int getSectionHeader32_flags(unsigned int nSection);
    unsigned int getSectionHeader32_addr(unsigned int nSection);
    unsigned int getSectionHeader32_offset(unsigned int nSection);
    unsigned int getSectionHeader32_size(unsigned int nSection);
    unsigned int getSectionHeader32_link(unsigned int nSection);
    unsigned int getSectionHeader32_info(unsigned int nSection);
    unsigned int getSectionHeader32_addralign(unsigned int nSection);
    unsigned int getSectionHeader32_entsize(unsigned int nSection);

    void setSectionHeader32_name(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_type(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_flags(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_addr(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_offset(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_size(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_link(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_info(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_addralign(unsigned int nSection,unsigned int nValue);
    void setSectionHeader32_entsize(unsigned int nSection,unsigned int nValue);

    unsigned int getSectionHeader64_name(unsigned int nSection);
    unsigned int getSectionHeader64_type(unsigned int nSection);
    unsigned long long getSectionHeader64_flags(unsigned int nSection);
    unsigned long long getSectionHeader64_addr(unsigned int nSection);
    unsigned long long getSectionHeader64_offset(unsigned int nSection);
    unsigned long long getSectionHeader64_size(unsigned int nSection);
    unsigned int getSectionHeader64_link(unsigned int nSection);
    unsigned int getSectionHeader64_info(unsigned int nSection);
    unsigned long long getSectionHeader64_addralign(unsigned int nSection);
    unsigned long long getSectionHeader64_entsize(unsigned int nSection);

    void setSectionHeader64_name(unsigned int nSection,unsigned int nValue);
    void setSectionHeader64_type(unsigned int nSection,unsigned int nValue);
    void setSectionHeader64_flags(unsigned int nSection,unsigned long long nValue);
    void setSectionHeader64_addr(unsigned int nSection,unsigned long long nValue);
    void setSectionHeader64_offset(unsigned int nSection,unsigned long long nValue);
    void setSectionHeader64_size(unsigned int nSection,unsigned long long nValue);
    void setSectionHeader64_link(unsigned int nSection,unsigned int nValue);
    void setSectionHeader64_info(unsigned int nSection,unsigned int nValue);
    void setSectionHeader64_addralign(unsigned int nSection,unsigned long long nValue);
    void setSectionHeader64_entsize(unsigned int nSection,unsigned long long nValue);

    unsigned int getProgramHeader32_type(unsigned int nProgram);
    unsigned int getProgramHeader32_offset(unsigned int nProgram);
    unsigned int getProgramHeader32_vaddr(unsigned int nProgram);
    unsigned int getProgramHeader32_paddr(unsigned int nProgram);
    unsigned int getProgramHeader32_filesz(unsigned int nProgram);
    unsigned int getProgramHeader32_memsz(unsigned int nProgram);
    unsigned int getProgramHeader32_flags(unsigned int nProgram);
    unsigned int getProgramHeader32_align(unsigned int nProgram);

    void setProgramHeader32_type(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader32_offset(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader32_vaddr(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader32_paddr(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader32_filesz(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader32_memsz(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader32_flags(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader32_align(unsigned int nProgram,unsigned int nValue);

    unsigned int getProgramHeader64_type(unsigned int nProgram);
    unsigned int getProgramHeader64_flags(unsigned int nProgram);
    unsigned long long getProgramHeader64_offset(unsigned int nProgram);
    unsigned long long getProgramHeader64_vaddr(unsigned int nProgram);
    unsigned long long getProgramHeader64_paddr(unsigned int nProgram);
    unsigned long long getProgramHeader64_filesz(unsigned int nProgram);
    unsigned long long getProgramHeader64_memsz(unsigned int nProgram);
    unsigned long long getProgramHeader64_align(unsigned int nProgram);

    void setProgramHeader64_type(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader64_flags(unsigned int nProgram,unsigned int nValue);
    void setProgramHeader64_offset(unsigned int nProgram,unsigned long long nValue);
    void setProgramHeader64_vaddr(unsigned int nProgram,unsigned long long nValue);
    void setProgramHeader64_paddr(unsigned int nProgram,unsigned long long nValue);
    void setProgramHeader64_filesz(unsigned int nProgram,unsigned long long nValue);
    void setProgramHeader64_memsz(unsigned int nProgram,unsigned long long nValue);
    void setProgramHeader64_align(unsigned int nProgram,unsigned long long nValue);

    bool isSectionPresent(int nSection);
    unsigned int getSectionHeaderOffset32(int nSection);
    unsigned int getSectionHeaderOffset64(int nSection);

    bool isProgramPresent(int nProgram);
    unsigned int getProgramHeaderOffset32(int nProgram);
    unsigned int getProgramHeaderOffset64(int nProgram);

    bool dumpSection(QString sFileName,int nSection);
    bool dumpProgram(QString sFileName,int nProgram);
    unsigned int RVAToOffset32(unsigned int nRVA);
    unsigned int OffsetToRVA32(unsigned int nOffset);
    unsigned long long OffsetToRVA64(unsigned long long nOffset);
    unsigned long long RVAToOffset64(unsigned long long nRVA);

    virtual unsigned long long OffsetToRVA(unsigned long long nOffset);
    virtual unsigned long long RVAToOffset(unsigned long long nRVA);

    unsigned int VAToOffset32(unsigned int nVA);
    unsigned long long VAToOffset64(unsigned long long nVA);
    unsigned int OffsetToVA32(unsigned int nOffset);
    unsigned long long OffsetToVA64(unsigned long long nOffset);

    virtual unsigned long long VAToOffset(unsigned long long nVA);
    virtual unsigned long long OffsetToVA(unsigned long long nOffset);

    bool isSectionNamePresent(QString sSectionName);

    static QString programHeaderTypeToString(unsigned int nType);
    static QString sectionHeaderTypeToString(unsigned int nType);

    QList<STRING_TABLE_RECORD> getStringTableAsList();
    QList<STRING_TABLE_RECORD> getStringTableAsList(unsigned int nSection);
    bool isStringInTablePresent(QString sSectionName,QString sString);

    QString getSectionName(int nSection);

    unsigned int getSectionFileOffset(int nSection);
    unsigned int getSectionFileSize(int nSection);
    unsigned int getSectionNumber(QString sSectionName);

    unsigned int getProgramFileOffset(int nSection);
    unsigned int getProgramFileSize(int nSection);

    unsigned int getOverlaySize();
    unsigned int getOverlayOffset();
    bool isOverlayPresent();
    unsigned int calculateRawSize();
    unsigned int getSizeOfHeader();

    unsigned int getNumberOfSections();
    unsigned int getNumberOfPrograms();

    bool compareOverlay(QString sSignature,unsigned int nOffset=0);
};

#endif // ELFFILE_H
