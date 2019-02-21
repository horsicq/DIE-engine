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
#include "elffile.h"

ELFFile::ELFFile(QObject *parent) :
    Binary(parent)
{
}

bool ELFFile::isValid()
{
    if(size()>=(int)sizeof(Elf32_Ehdr))
    {
        if(getElfHeader_magic()==0x464c457f)
        {
            return true;
        }
    }

    emit appendError("Invalid ELF file");

    return false;
}

bool ELFFile::isELF64()
{
    if(isValid())
    {
        if(getElfHeader_class()==2)
        {
            return true;
        }
    }

    return false;
}

bool ELFFile::isReverse()
{
    if(nIsReverse==-1)
    {
        if(readByte(5)==2)
        {
            nIsReverse=1;
        }
        else
        {
            nIsReverse=0;
        }
    }

    return (nIsReverse==1);
}

unsigned int ELFFile::getElfHeaderOffset()
{
    return 0;
}

unsigned int ELFFile::getElfHeaderSize()
{
    if(isELF64())
    {
        return sizeof(Elf64_Ehdr);
    }

    return sizeof(Elf32_Ehdr);
}

unsigned int ELFFile::getElfHeader_magic()
{
    return readDword(0); // no reverse!
}

unsigned char ELFFile::getElfHeader_class()
{
    return readByte(4);
}

unsigned char ELFFile::getElfHeader_data()
{
    return readByte(5);
}

unsigned char ELFFile::getElfHeader_iversion()
{
    return readByte(6);
}

unsigned char ELFFile::getElfHeader_osabi()
{
    return readByte(7);
}

unsigned char ELFFile::getElfHeader_abiversion()
{
    return readByte(8);
}

unsigned char ELFFile::getElfHeader_pad(int nIndex)
{
    if(nIndex<9)
    {
        return readByte(9+nIndex);
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
    return 0;
}

void ELFFile::setElfHeader_magic(unsigned int nValue)
{
    writeDword(0,nValue,isReverse());
}

void ELFFile::setElfHeader_class(unsigned char cValue)
{
    writeByte(4,cValue);
}

void ELFFile::setElfHeader_data(unsigned char cValue)
{
    writeByte(5,cValue);

    if(cValue==2)
    {
        nIsReverse=1;
    }
    else
    {
        nIsReverse=0;
    }
}

void ELFFile::setElfHeader_iversion(unsigned char cValue)
{
    writeByte(6,cValue);
}

void ELFFile::setElfHeader_osabi(unsigned char cValue)
{
    writeByte(7,cValue);
}

void ELFFile::setElfHeader_abiversion(unsigned char cValue)
{
    writeByte(8,cValue);
}

void ELFFile::setElfHeader_pad(unsigned char cValue, int nIndex)
{
    if(nIndex<9)
    {
        writeByte(9+nIndex,cValue);
        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
}

unsigned short ELFFile::getElfHeader_type()
{
    return readWord(offsetof(Elf32_Ehdr,e_type),isReverse());
}

unsigned short ELFFile::getElfHeader_machine()
{
    return readWord(offsetof(Elf32_Ehdr,e_machine),isReverse());
}

QString ELFFile::getMachineAsString()
{
    unsigned short sMachine=getElfHeader_machine();
    QList<MACHINE> list=getMachineList();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).nMachine==sMachine)
        {
            return list.at(i).sName.section("_",1,1);
        }
    }

    return "";
}

unsigned int ELFFile::getElfHeader_version()
{
    return readDword(offsetof(Elf32_Ehdr,e_version),isReverse());
}

unsigned int ELFFile::getElfHeader_entry()
{
    return readDword(offsetof(Elf32_Ehdr,e_entry),isReverse());
}

unsigned long long ELFFile::getElfHeader_entry64()
{
    return readQword(offsetof(Elf64_Ehdr,e_entry),isReverse());
}

unsigned int ELFFile::getElfHeader_phoff()
{
    return readDword(offsetof(Elf32_Ehdr,e_phoff),isReverse());
}

unsigned long long ELFFile::getElfHeader_phoff64()
{
    return readQword(offsetof(Elf64_Ehdr,e_phoff),isReverse());
}

unsigned int ELFFile::getElfHeader_shoff()
{
    return readDword(offsetof(Elf32_Ehdr,e_shoff),isReverse());
}

unsigned long long ELFFile::getElfHeader_shoff64()
{
    return readQword(offsetof(Elf64_Ehdr,e_shoff),isReverse());
}

unsigned int ELFFile::getElfHeader_flags()
{
    if(isELF64())
    {
        return readDword(offsetof(Elf64_Ehdr,e_flags),isReverse());
    }

    return readDword(offsetof(Elf32_Ehdr,e_flags),isReverse());
}

unsigned short ELFFile::getElfHeader_ehsize()
{
    if(isELF64())
    {
        return readWord(offsetof(Elf64_Ehdr,e_ehsize),isReverse());
    }

    return readWord(offsetof(Elf32_Ehdr,e_ehsize),isReverse());
}

unsigned short ELFFile::getElfHeader_phentsize()
{
    if(isELF64())
    {
        return readWord(offsetof(Elf64_Ehdr,e_phentsize),isReverse());
    }

    return readWord(offsetof(Elf32_Ehdr,e_phentsize),isReverse());
}

unsigned short ELFFile::getElfHeader_phnum()
{
    if(isELF64())
    {
        return readWord(offsetof(Elf64_Ehdr,e_phnum),isReverse());
    }

    return readWord(offsetof(Elf32_Ehdr,e_phnum),isReverse());
}

unsigned short ELFFile::getElfHeader_shentsize()
{
    if(isELF64())
    {
        return readWord(offsetof(Elf64_Ehdr,e_shentsize),isReverse());
    }

    return readWord(offsetof(Elf32_Ehdr,e_shentsize),isReverse());
}

unsigned short ELFFile::getElfHeader_shnum()
{
    if(isELF64())
    {
        return readWord(offsetof(Elf64_Ehdr,e_shnum),isReverse());
    }

    return readWord(offsetof(Elf32_Ehdr,e_shnum),isReverse());
}

unsigned short ELFFile::getElfHeader_shstrndx()
{
    if(isELF64())
    {
        return readWord(offsetof(Elf64_Ehdr,e_shstrndx),isReverse());
    }

    return readWord(offsetof(Elf32_Ehdr,e_shstrndx),isReverse());
}

void ELFFile::setElfHeader_type(unsigned short sValue)
{
    writeWord(offsetof(Elf32_Ehdr,e_type),sValue,isReverse());
}

void ELFFile::setElfHeader_machine(unsigned short sValue)
{
    writeWord(offsetof(Elf32_Ehdr,e_machine),sValue,isReverse());
}

void ELFFile::setElfHeader_version(unsigned int nValue)
{
    writeDword(offsetof(Elf32_Ehdr,e_version),nValue,isReverse());
}

void ELFFile::setElfHeader_entry(unsigned int nValue)
{
    writeDword(offsetof(Elf32_Ehdr,e_entry),nValue,isReverse());
}

void ELFFile::setElfHeader_entry64(unsigned long long nValue)
{
    writeQword(offsetof(Elf64_Ehdr,e_entry),nValue,isReverse());
}

void ELFFile::setElfHeader_phoff(unsigned int nValue)
{
    writeDword(offsetof(Elf32_Ehdr,e_phoff),nValue,isReverse());
}

void ELFFile::setElfHeader_phoff64(unsigned long long nValue)
{
    writeQword(offsetof(Elf64_Ehdr,e_phoff),nValue,isReverse());
}

void ELFFile::setElfHeader_shoff(unsigned int nValue)
{
    writeDword(offsetof(Elf32_Ehdr,e_shoff),nValue,isReverse());
}

void ELFFile::setElfHeader_shoff64(unsigned long long nValue)
{
    writeQword(offsetof(Elf64_Ehdr,e_shoff),nValue,isReverse());
}

void ELFFile::setElfHeader_flags(unsigned int nValue)
{
    if(isELF64())
    {
        writeDword(offsetof(Elf64_Ehdr,e_flags),nValue,isReverse());
        return;
    }

    writeDword(offsetof(Elf32_Ehdr,e_flags),nValue,isReverse());
}

void ELFFile::setElfHeader_ehsize(unsigned short sValue)
{
    if(isELF64())
    {
        writeWord(offsetof(Elf64_Ehdr,e_ehsize),sValue,isReverse());
        return;
    }

    writeWord(offsetof(Elf32_Ehdr,e_ehsize),sValue,isReverse());
}

void ELFFile::setElfHeader_phentsize(unsigned short sValue)
{
    if(isELF64())
    {
        writeWord(offsetof(Elf64_Ehdr,e_phentsize),sValue,isReverse());
        return;
    }

    writeWord(offsetof(Elf32_Ehdr,e_phentsize),sValue,isReverse());
}

void ELFFile::setElfHeader_phnum(unsigned short sValue)
{
    if(isELF64())
    {
        writeWord(offsetof(Elf64_Ehdr,e_phnum),sValue,isReverse());
        return;
    }

    writeWord(offsetof(Elf32_Ehdr,e_phnum),sValue,isReverse());
}

void ELFFile::setElfHeader_shentsize(unsigned short sValue)
{
    if(isELF64())
    {
        writeWord(offsetof(Elf64_Ehdr,e_shentsize),sValue,isReverse());
        return;
    }

    writeWord(offsetof(Elf32_Ehdr,e_shentsize),sValue,isReverse());
}

void ELFFile::setElfHeader_shnum(unsigned short sValue)
{
    if(isELF64())
    {
        writeWord(offsetof(Elf64_Ehdr,e_shnum),sValue,isReverse());
        return;
    }

    writeWord(offsetof(Elf32_Ehdr,e_shnum),sValue,isReverse());
}

void ELFFile::setElfHeader_shstrndx(unsigned short sValue)
{
    if(isELF64())
    {
        writeWord(offsetof(Elf64_Ehdr,e_shstrndx),sValue,isReverse());
        return;
    }

    writeWord(offsetof(Elf32_Ehdr,e_shstrndx),sValue,isReverse());
}
bool ELFFile::isSectionPresent(int nSection)
{
    if(nSection<getElfHeader_shnum())
    {
        return true;
    }

    emit appendError(QString("Invalid section number: %1").arg(nSection));

    return false;
}
unsigned int ELFFile::getSectionHeaderOffset32(int nSection)
{
    if(isSectionPresent(nSection))
    {
        return getElfHeader_shoff()+nSection*getElfHeader_shentsize();
    }

    return 0;
}
unsigned int ELFFile::getSectionHeaderOffset64(int nSection)
{
    if(isSectionPresent(nSection))
    {
        return getElfHeader_shoff64()+nSection*getElfHeader_shentsize();
    }

    return 0;
}

bool ELFFile::isProgramPresent(int nProgram)
{
    if(nProgram<getElfHeader_phnum())
    {
        return true;
    }

    emit appendError(QString("Invalid nProgram number: %1").arg(nProgram));

    return false;
}

unsigned int ELFFile::getProgramHeaderOffset32(int nProgram)
{
    if(isProgramPresent(nProgram))
    {
        return getElfHeader_phoff()+nProgram*getElfHeader_phentsize();
    }

    return 0;
}

unsigned int ELFFile::getProgramHeaderOffset64(int nProgram)
{
    if(isProgramPresent(nProgram))
    {
        return getElfHeader_phoff64()+nProgram*getElfHeader_phentsize();
    }

    return 0;
}

bool ELFFile::dumpSection(QString sFileName, int nSection)
{
    unsigned int nOffset=0;
    unsigned int nSize=0;

    if(isELF64())
    {
        nOffset=getSectionHeader64_offset(nSection);
        nSize=getSectionHeader64_size(nSection);
    }
    else
    {
        nOffset=getSectionHeader32_offset(nSection);
        nSize=getSectionHeader32_size(nSection);
    }

    return dump(sFileName,nOffset,nSize);
}

bool ELFFile::dumpProgram(QString sFileName, int nProgram)
{
    unsigned int nOffset=0;
    unsigned int nSize=0;

    if(isELF64())
    {
        nOffset=getProgramHeader64_offset(nProgram);
        nSize=getProgramHeader64_filesz(nProgram);
    }
    else
    {
        nOffset=getProgramHeader32_offset(nProgram);
        nSize=getProgramHeader32_filesz(nProgram);
    }

    return dump(sFileName,nOffset,nSize);
}
unsigned int ELFFile::getSectionHeader32_name(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_name),isReverse());
}

unsigned int ELFFile::getSectionHeader32_type(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_type),isReverse());
}

unsigned int ELFFile::getSectionHeader32_flags(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_flags),isReverse());
}

unsigned int ELFFile::getSectionHeader32_addr(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_addr),isReverse());
}

unsigned int ELFFile::getSectionHeader32_offset(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_offset),isReverse());
}

unsigned int ELFFile::getSectionHeader32_size(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_size),isReverse());
}

unsigned int ELFFile::getSectionHeader32_link(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_link),isReverse());
}

unsigned int ELFFile::getSectionHeader32_info(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_info),isReverse());
}

unsigned int ELFFile::getSectionHeader32_addralign(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_addralign),isReverse());
}

unsigned int ELFFile::getSectionHeader32_entsize(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_entsize),isReverse());
}

void ELFFile::setSectionHeader32_name(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_name),nValue,isReverse());
}

void ELFFile::setSectionHeader32_type(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_type),nValue,isReverse());
}

void ELFFile::setSectionHeader32_flags(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_flags),nValue,isReverse());
}

void ELFFile::setSectionHeader32_addr(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_addr),nValue,isReverse());
}

void ELFFile::setSectionHeader32_offset(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_offset),nValue,isReverse());
}

void ELFFile::setSectionHeader32_size(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_size),nValue,isReverse());
}

void ELFFile::setSectionHeader32_link(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_link),nValue,isReverse());
}

void ELFFile::setSectionHeader32_info(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_info),nValue,isReverse());
}

void ELFFile::setSectionHeader32_addralign(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_addralign),nValue,isReverse());
}

void ELFFile::setSectionHeader32_entsize(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset32(nSection)+offsetof(Elf32_Shdr,sh_entsize),nValue,isReverse());
}

unsigned int ELFFile::getSectionHeader64_name(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_name),isReverse());
}

unsigned int ELFFile::getSectionHeader64_type(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_type),isReverse());
}

unsigned long long ELFFile::getSectionHeader64_flags(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_flags),isReverse());
}

unsigned long long ELFFile::getSectionHeader64_addr(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_addr),isReverse());
}

unsigned long long ELFFile::getSectionHeader64_offset(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_offset),isReverse());
}

unsigned long long ELFFile::getSectionHeader64_size(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_size),isReverse());
}

unsigned int ELFFile::getSectionHeader64_link(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_link),isReverse());
}

unsigned int ELFFile::getSectionHeader64_info(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_info),isReverse());
}

unsigned long long ELFFile::getSectionHeader64_addralign(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_addralign),isReverse());
}

unsigned long long ELFFile::getSectionHeader64_entsize(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_entsize),isReverse());
}

void ELFFile::setSectionHeader64_name(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_name),nValue,isReverse());
}

void ELFFile::setSectionHeader64_type(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_type),nValue,isReverse());
}

void ELFFile::setSectionHeader64_flags(unsigned int nSection, unsigned long long nValue)
{
    writeQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_flags),nValue,isReverse());
}

void ELFFile::setSectionHeader64_addr(unsigned int nSection, unsigned long long nValue)
{
    writeQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_addr),nValue,isReverse());
}

void ELFFile::setSectionHeader64_offset(unsigned int nSection, unsigned long long nValue)
{
    writeQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_offset),nValue,isReverse());
}

void ELFFile::setSectionHeader64_size(unsigned int nSection, unsigned long long nValue)
{
    writeQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_size),nValue,isReverse());
}

void ELFFile::setSectionHeader64_link(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_link),nValue,isReverse());
}

void ELFFile::setSectionHeader64_info(unsigned int nSection, unsigned int nValue)
{
    writeDword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_info),nValue,isReverse());
}

void ELFFile::setSectionHeader64_addralign(unsigned int nSection, unsigned long long nValue)
{
    writeQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_addralign),nValue,isReverse());
}

void ELFFile::setSectionHeader64_entsize(unsigned int nSection, unsigned long long nValue)
{
    writeQword(getSectionHeaderOffset64(nSection)+offsetof(Elf64_Shdr,sh_entsize),nValue,isReverse());
}

unsigned int ELFFile::getProgramHeader32_type(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_type),isReverse());
}

unsigned int ELFFile::getProgramHeader32_offset(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_offset),isReverse());
}

unsigned int ELFFile::getProgramHeader32_vaddr(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_vaddr),isReverse());
}

unsigned int ELFFile::getProgramHeader32_paddr(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_paddr),isReverse());
}

unsigned int ELFFile::getProgramHeader32_filesz(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_filesz),isReverse());
}

unsigned int ELFFile::getProgramHeader32_memsz(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_memsz),isReverse());
}

unsigned int ELFFile::getProgramHeader32_flags(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_flags),isReverse());
}

unsigned int ELFFile::getProgramHeader32_align(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_align),isReverse());
}

void ELFFile::setProgramHeader32_type(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_type),nValue,isReverse());
}

void ELFFile::setProgramHeader32_offset(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_offset),nValue,isReverse());
}

void ELFFile::setProgramHeader32_vaddr(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_vaddr),nValue,isReverse());
}

void ELFFile::setProgramHeader32_paddr(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_paddr),nValue,isReverse());
}

void ELFFile::setProgramHeader32_filesz(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_filesz),nValue,isReverse());
}

void ELFFile::setProgramHeader32_memsz(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_memsz),nValue,isReverse());
}

void ELFFile::setProgramHeader32_flags(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_flags),nValue,isReverse());
}

void ELFFile::setProgramHeader32_align(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset32(nProgram)+offsetof(Elf32_Phdr,p_align),nValue,isReverse());
}

unsigned int ELFFile::getProgramHeader64_type(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_type),isReverse());
}

unsigned int ELFFile::getProgramHeader64_flags(unsigned int nProgram)
{
    return readDword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_flags),isReverse());
}

unsigned long long ELFFile::getProgramHeader64_offset(unsigned int nProgram)
{
    return readQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_offset),isReverse());
}

unsigned long long ELFFile::getProgramHeader64_vaddr(unsigned int nProgram)
{
    return readQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_vaddr),isReverse());
}

unsigned long long ELFFile::getProgramHeader64_paddr(unsigned int nProgram)
{
    return readQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_paddr),isReverse());
}

unsigned long long ELFFile::getProgramHeader64_filesz(unsigned int nProgram)
{
    return readQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_filesz),isReverse());
}

unsigned long long ELFFile::getProgramHeader64_memsz(unsigned int nProgram)
{
    return readQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_memsz),isReverse());
}

unsigned long long ELFFile::getProgramHeader64_align(unsigned int nProgram)
{
    return readQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_align),isReverse());
}

void ELFFile::setProgramHeader64_type(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_type),nValue,isReverse());
}

void ELFFile::setProgramHeader64_flags(unsigned int nProgram, unsigned int nValue)
{
    writeDword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_flags),nValue,isReverse());
}

void ELFFile::setProgramHeader64_offset(unsigned int nProgram, unsigned long long nValue)
{
    writeQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_offset),nValue,isReverse());
}

void ELFFile::setProgramHeader64_vaddr(unsigned int nProgram, unsigned long long nValue)
{
    writeQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_vaddr),nValue,isReverse());
}

void ELFFile::setProgramHeader64_paddr(unsigned int nProgram, unsigned long long nValue)
{
    writeQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_paddr),nValue,isReverse());
}

void ELFFile::setProgramHeader64_filesz(unsigned int nProgram, unsigned long long nValue)
{
    writeQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_filesz),nValue,isReverse());
}

void ELFFile::setProgramHeader64_memsz(unsigned int nProgram, unsigned long long nValue)
{
    writeQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_memsz),nValue,isReverse());
}

void ELFFile::setProgramHeader64_align(unsigned int nProgram, unsigned long long nValue)
{
    writeQword(getProgramHeaderOffset64(nProgram)+offsetof(Elf64_Phdr,p_align),nValue,isReverse());
}

unsigned int ELFFile::OffsetToRVA32(unsigned int nOffset)
{

    for(int i=0; i<getElfHeader_phnum(); i++)
    {
        if((getProgramHeader32_offset(i)<=nOffset)&&(nOffset<getProgramHeader32_offset(i)+getProgramHeader32_filesz(i)))
        {
            return getProgramHeader32_vaddr(i)+nOffset-getProgramHeader32_offset(i);
        }
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,8,16,QChar('0')));
    return -1;
}
unsigned long long ELFFile::OffsetToRVA64(unsigned long long nOffset)
{

    for(int i=0; i<getElfHeader_phnum(); i++)
    {
        if((getProgramHeader64_offset(i)<=nOffset)&&(nOffset<getProgramHeader64_offset(i)+getProgramHeader64_filesz(i)))
        {
            return getProgramHeader64_vaddr(i)+nOffset-getProgramHeader64_offset(i);
        }
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,16,16,QChar('0')));
    return -1;
}

unsigned int ELFFile::RVAToOffset32(unsigned int nRVA)
{
    for(int i=0; i<getElfHeader_phnum(); i++)
    {
        if((getProgramHeader32_vaddr(i)<=nRVA)&&(nRVA<getProgramHeader32_vaddr(i)+getProgramHeader32_memsz(i)))
        {
            return getProgramHeader32_offset(i)+nRVA-getProgramHeader32_vaddr(i);
        }
    }

    emit appendWarning(QString("Invalid RVA: %1").arg(nRVA,8,16,QChar('0')));
    return -1;
}

unsigned long long ELFFile::RVAToOffset64(unsigned long long nRVA)
{
    for(int i=0; i<getElfHeader_phnum(); i++)
    {
        if((getProgramHeader64_vaddr(i)<=nRVA)&&(nRVA<getProgramHeader64_vaddr(i)+getProgramHeader64_memsz(i)))
        {
            return getProgramHeader64_offset(i)+nRVA-getProgramHeader64_vaddr(i);
        }
    }

    emit appendWarning(QString("Invalid RVA: %1").arg(nRVA,16,16,QChar('0')));
    return -1;
}

unsigned long long ELFFile::OffsetToRVA(unsigned long long nOffset)
{
    unsigned long long nResult=0;

    if(isELF64())
    {
        nResult=OffsetToRVA64(nOffset);
    }
    else
    {
        nResult=OffsetToRVA32(nOffset);
    }

    if(nResult==0xFFFFFFFF)
    {
        nResult=-1;
    }

    return nResult;
}

unsigned long long ELFFile::RVAToOffset(unsigned long long nRVA)
{
    unsigned long long nResult=0;

    if(isELF64())
    {
        nResult=RVAToOffset64(nRVA);
    }
    else
    {
        nResult=RVAToOffset32(nRVA);
    }

    if(nResult==0xFFFFFFFF)
    {
        nResult=-1;
    }

    return nResult;
}

unsigned int ELFFile::VAToOffset32(unsigned int nVA)
{
    return RVAToOffset32(nVA);
}

unsigned long long ELFFile::VAToOffset64(unsigned long long nVA)
{
    return RVAToOffset64(nVA);
}

unsigned int ELFFile::OffsetToVA32(unsigned int nOffset)
{
    return OffsetToRVA32(nOffset);
}

unsigned long long ELFFile::OffsetToVA64(unsigned long long nOffset)
{
    return OffsetToRVA64(nOffset);
}

unsigned long long ELFFile::VAToOffset(unsigned long long nVA)
{
    if(isELF64())
    {
        return VAToOffset64(nVA);
    }
    else
    {
        return VAToOffset32(nVA);
    }
}

unsigned long long ELFFile::OffsetToVA(unsigned long long nOffset)
{
    if(isELF64())
    {
        return OffsetToVA64(nOffset);
    }
    else
    {
        return OffsetToVA32(nOffset);
    }
}

QByteArray ELFFile::getSectionNamesTable()
{
    QByteArray baResult;
    unsigned int nShstrndx=getElfHeader_shstrndx();

    if(nShstrndx)
    {
        if(nShstrndx<getElfHeader_shnum())
        {
            return getSection(nShstrndx);
        }
    }

    emit appendError(QString("Invalid shstrndx: %1").arg(nShstrndx,0,16,QChar('0')));
    return baResult;
}

QByteArray ELFFile::getSection(int nSection)
{
    QByteArray baResult;
    unsigned int nSize=0;
    unsigned int nOffset=0;

    if(isSectionPresent(nSection))
    {
        if(isELF64())
        {
            nSize=getSectionHeader64_size(nSection);
            nOffset=getSectionHeader64_offset(nSection);
        }
        else
        {
            nSize=getSectionHeader32_size(nSection);
            nOffset=getSectionHeader32_offset(nSection);
        }

        baResult=readArray(nOffset,nSize);
    }

    return baResult;
}

QString ELFFile::getSectionName(int nSection)
{
    unsigned int nName=0;
    QString sResult;

    if(isELF64())
    {
        nName=getSectionHeader64_name(nSection);
    }
    else
    {
        nName=getSectionHeader32_name(nSection);
    }

    QByteArray baSectionNamesTable=getSectionNamesTable();

    if(((int)nName<baSectionNamesTable.size())&&((int)nName>0))
    {
        sResult.append(baSectionNamesTable.data()+nName);
    }


    return sResult;
}

unsigned int ELFFile::getSectionFileOffset(int nSection)
{
    if(isELF64())
    {
        return getSectionHeader64_offset(nSection);
    }
    else
    {
        return getSectionHeader32_offset(nSection);
    }
}

unsigned int ELFFile::getSectionFileSize(int nSection)
{
    if(isELF64())
    {
        return getSectionHeader64_size(nSection);
    }
    else
    {
        return getSectionHeader32_size(nSection);
    }
}

unsigned int ELFFile::getSectionNumber(QString sSectionName)
{
    int nSections=getElfHeader_shnum();

    for(int i=0; i<nSections; i++)
    {
        if(getSectionName(i)==sSectionName)
        {
            return i;
        }
    }

    return -1;
}

unsigned int ELFFile::getProgramFileOffset(int nProgram)
{
    if(isELF64())
    {
        return getProgramHeader64_offset(nProgram);
    }
    else
    {
        return getProgramHeader32_offset(nProgram);
    }
}

unsigned int ELFFile::getProgramFileSize(int nProgram)
{
    if(isELF64())
    {
        return getProgramHeader64_filesz(nProgram);
    }
    else
    {
        return getProgramHeader32_filesz(nProgram);
    }
}


bool ELFFile::isSectionNamePresent(QString sSectionName)
{
    unsigned int nNumberOfSections=getElfHeader_shnum();

    for(unsigned int i=0; i<nNumberOfSections; i++)
    {
        if(getSectionName(i)==sSectionName)
        {
            return true;
        }
    }

    return false;
}

bool ELFFile::compareEPfast(QString sSignature)
{
    if(!bIsEntryPontLoad)
    {
        entryPointLoad();
    }

    return Binary::compareEPfast(sSignature,0);
}

bool ELFFile::compareEP(QString sSignature, unsigned int nOffset)
{
    return compare(sSignature,getEntryPointOffset()+nOffset);
}

unsigned long long ELFFile::getEntryPointOffset()
{
    return RVAToOffset(getElfHeader_entry());
}

//bool ELFFile::compareEPfast(QString sSignature)
//{
//    return compareEPfast(convertSignature(sSignature).toAscii().data());
//}

QString ELFFile::programHeaderTypeToString(unsigned int nType)
{
    QString sResult;

    if(nType==0)
    {
        sResult="PT_NULL";
    }
    else if(nType==1)
    {
        sResult="PT_LOAD";
    }
    else if(nType==2)
    {
        sResult="PT_DYNAMIC";
    }
    else if(nType==3)
    {
        sResult="PT_INTERP";
    }
    else if(nType==4)
    {
        sResult="PT_NOTE";
    }
    else if(nType==5)
    {
        sResult="PT_SHLIB";
    }
    else if(nType==6)
    {
        sResult="PT_PHDR";
    }
    else if(nType==0x70000000)
    {
        sResult="PT_LOPROC";
    }
    else if(nType==0x7fffffff)
    {
        sResult="PT_HIPROC";
    }

    return sResult;
}

QString ELFFile::sectionHeaderTypeToString(unsigned int nType)
{

    QString sResult;

    if(nType==0)
    {
        sResult="SHT_NULL";
    }
    else if(nType==1)
    {
        sResult="SHT_PROGBITS";
    }
    else if(nType==2)
    {
        sResult="SHT_SYMTAB";
    }
    else if(nType==3)
    {
        sResult="SHT_STRTAB";
    }
    else if(nType==4)
    {
        sResult="SHT_RELA";
    }
    else if(nType==5)
    {
        sResult="SHT_HASH";
    }
    else if(nType==6)
    {
        sResult="SHT_DYNAMIC";
    }
    else if(nType==7)
    {
        sResult="SHT_NOTE";
    }
    else if(nType==8)
    {
        sResult="SHT_NOBITS";
    }
    else if(nType==9)
    {
        sResult="SHT_REL";
    }
    else if(nType==10)
    {
        sResult="SHT_SHLIB";
    }
    else if(nType==11)
    {
        sResult="SHT_DYNSYM";
    }
    else if(nType==12)
    {
        sResult="SHT_NUM";
    }
    else if(nType==0x70000000)
    {
        sResult="SHT_LOPROC";
    }
    else if(nType==0x7fffffff)
    {
        sResult="SHT_HIPROC";
    }
    else if(nType==0x80000000)
    {
        sResult="SHT_LOUSER";
    }
    else if(nType==0xFfffffff)
    {
        sResult="SHT_HIUSER";
    }

    return sResult;
}

QList<ELFFile::STRING_TABLE_RECORD> ELFFile::getStringTableAsList()
{
    //    QList<STRING_TABLE_RECORD> listResult;

    //    if(getElfHeader_shstrndx())
    //    {
    //        QByteArray baStringTable=getSectionNamesTable();

    //        unsigned int nSize=baStringTable.size();
    //        char *pOffset=baStringTable.data();

    //        STRING_TABLE_RECORD record;
    //        unsigned int nStringSize=0;

    //        record.nIndex=0;

    //        while(nSize>0)
    //        {
    //            record.sString=pOffset;
    //            listResult.append(record);

    //            nStringSize=record.sString.size()+1;
    //            pOffset+=nStringSize;
    //            record.nIndex+=nStringSize;

    //            nSize-=nStringSize;
    //        }
    //    }

    return getStringTableAsList(getElfHeader_shstrndx());
}

QList<ELFFile::STRING_TABLE_RECORD> ELFFile::getStringTableAsList(unsigned int nSection)
{
    QList<STRING_TABLE_RECORD> listResult;

    if(nSection)
    {
        QByteArray baStringTable=getSection(nSection);

        unsigned int nSize=baStringTable.size();
        char *pOffset=baStringTable.data();

        STRING_TABLE_RECORD record;
        unsigned int nStringSize=0;

        record.nIndex=0;

        while((int)nSize>0)
        {
            record.sString=pOffset;
            listResult.append(record);

            nStringSize=record.sString.size()+1;
            pOffset+=nStringSize;
            record.nIndex+=nStringSize;

            nSize-=nStringSize;
        }
    }

    return listResult;
}

bool ELFFile::isStringInTablePresent(QString sSectionName, QString sString)
{
    unsigned int nNumberOfSections=getElfHeader_shnum();

    if(nNumberOfSections>0xFF)
    {
        return false;
    }

    for(unsigned int i=0; i<nNumberOfSections; i++)
    {
        if(getSectionName(i)==sSectionName)
        {
            QList<STRING_TABLE_RECORD> listResult=getStringTableAsList(i);

            for(int j=0; j<listResult.count(); j++)
            {
                //                qDebug(listResult.at(j).sString.toAscii().data());
                //                qDebug(sString.toAscii().data());
                if(listResult.at(j).sString==sString)
                {
                    return true;
                }
            }

            return false;
        }
    }

    return false;
}

void ELFFile::entryPointLoad()
{
    if(isELF64())
    {
        nEntryPointOffset=RVAToOffset64(getElfHeader_entry64());
    }
    else
    {
        nEntryPointOffset=RVAToOffset(getElfHeader_entry());
    }

    _loadEntryPoint();
}

QList<ELFFile::MACHINE> ELFFile::getMachineList()
{
    QList<MACHINE> listResult;
    MACHINE machine;

    machine.nMachine=0;
    machine.sName="EM_NONE";
    listResult.append(machine);

    machine.nMachine=1;
    machine.sName="EM_M32";
    listResult.append(machine);
    machine.nMachine=2;
    machine.sName="EM_SPARC";
    listResult.append(machine);
    machine.nMachine=3;
    machine.sName="EM_386";
    listResult.append(machine);
    machine.nMachine=4;
    machine.sName="EM_68K";
    listResult.append(machine);
    machine.nMachine=5;
    machine.sName="EM_88K";
    listResult.append(machine);
    machine.nMachine=6;
    machine.sName="EM_486";
    listResult.append(machine);
    machine.nMachine=7;
    machine.sName="EM_860";
    listResult.append(machine);
    machine.nMachine=8;
    machine.sName="EM_MIPS";
    listResult.append(machine);
    machine.nMachine=10;
    machine.sName="MIPS_RS";
    listResult.append(machine);
    machine.nMachine=15;
    machine.sName="EM_PARISC";
    listResult.append(machine);
    machine.nMachine=18;
    machine.sName="EM_SPARC32PLUS";
    listResult.append(machine);
    machine.nMachine=20;
    machine.sName="EM_PPC";
    listResult.append(machine);
    machine.nMachine=21;
    machine.sName="EM_PPC64";
    listResult.append(machine);
    machine.nMachine=22;
    machine.sName="EM_S390";
    listResult.append(machine);
    machine.nMachine=23;
    machine.sName="EM_SPU";
    listResult.append(machine);
    machine.nMachine=40;
    machine.sName="EM_ARM";
    listResult.append(machine);
    machine.nMachine=42;
    machine.sName="EM_SH";
    listResult.append(machine);
    machine.nMachine=43;
    machine.sName="EM_SPARCV9";
    listResult.append(machine);
    machine.nMachine=50;
    machine.sName="EM_IA64";
    listResult.append(machine);
    machine.nMachine=62;
    machine.sName="EM_AMD64";
    listResult.append(machine);
    machine.nMachine=76;
    machine.sName="EM_CRIS";
    listResult.append(machine);
    machine.nMachine=87;
    machine.sName="EM_V850";
    listResult.append(machine);
    machine.nMachine=88;
    machine.sName="EM_M32R";
    listResult.append(machine);
    machine.nMachine=89;
    machine.sName="EM_MN10300";
    listResult.append(machine);
    machine.nMachine=106;
    machine.sName="EM_BLACKFIN";
    listResult.append(machine);
    machine.nMachine=140;
    machine.sName="EM_TIC6000";
    listResult.append(machine);
    machine.nMachine=183;
    machine.sName="EM_AARCH64";
    listResult.append(machine);
    machine.nMachine=0x5441;
    machine.sName="EM_FRV";
    listResult.append(machine);
    machine.nMachine=0x18ad;
    machine.sName="EM_AVR32";
    listResult.append(machine);
    machine.nMachine=0x9026;
    machine.sName="EM_ALPHA";
    listResult.append(machine);
    machine.nMachine=0x9080;
    machine.sName="EM_CYGNUSV850";
    listResult.append(machine);
    machine.nMachine=0x9041;
    machine.sName="EM_CYGNUSM32R";
    listResult.append(machine);
    machine.nMachine=0xA390;
    machine.sName="EM_S390OLD";
    listResult.append(machine);
    machine.nMachine=0xbeef;
    machine.sName="EM_CYGNUSMN10300";
    listResult.append(machine);

    return listResult;
}
unsigned int ELFFile::getOverlaySize()
{
    return MAXIMAL((long long)(this->size())-(long long)calculateRawSize(),0);
}

unsigned int ELFFile::getOverlayOffset()
{
    return calculateRawSize();
}
bool ELFFile::isOverlayPresent()
{
    return getOverlaySize()!=0;
}

unsigned int ELFFile::calculateRawSize()
{
    unsigned int nResult=getSizeOfHeader();

    for(unsigned int i=0; i<getNumberOfPrograms(); i++)
    {
        nResult=qMax(nResult,getProgramFileOffset(i)+getProgramFileSize(i));
    }

    return nResult;
}

unsigned int ELFFile::getSizeOfHeader()
{
    if(isELF64())
    {
        return sizeof(Elf64_Ehdr);
    }
    else
    {
        return sizeof(Elf32_Ehdr);
    }
}

unsigned int ELFFile::getNumberOfSections()
{
    return getElfHeader_shnum();
}

unsigned int ELFFile::getNumberOfPrograms()
{
    return getElfHeader_phnum();
}

bool ELFFile::compareOverlay(QString sSignature, unsigned int nOffset)
{
    return compare(sSignature,getOverlayOffset()+nOffset);
}











