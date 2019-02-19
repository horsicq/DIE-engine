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
#include "machfile.h"

MACHFile::MACHFile(QObject *parent) :
    Binary(parent)
{
}

bool MACHFile::isValid()
{

    if(size()>=(int)sizeof(mach_header))
    {
        unsigned int nMagic=readDword(0);

        if((nMagic==MH_MAGIC)||(nMagic==MH_CIGAM)||(nMagic==MH_MAGIC_64)||(nMagic==MH_CIGAM_64))
        {
            return true;
        }

    }

    emit appendError("Invalid MACH file");

    return false;
}

bool MACHFile::isMACH64()
{
    unsigned int nMagic=readDword(0);

    if((nMagic==MH_MAGIC_64)||(nMagic==MH_CIGAM_64))
    {
        return true;
    }


    return false;
}

bool MACHFile::isReverse()
{

    if(nIsReverse==-1)
    {
        unsigned int nMagic=readDword(0);

        if((nMagic==MH_CIGAM)||(nMagic==MH_CIGAM_64))
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
void MACHFile::entryPointLoad()
{
    //    nEntryPointOffset=getAddressOfEntryPoint();

    //    _loadEntryPoint();

    if(isMACH64())
    {
        nEntryPointOffset=RVAToOffset64(getEntryPoint());
    }
    else
    {
        nEntryPointOffset=RVAToOffset32(getEntryPoint());
    }

    _loadEntryPoint();
}

unsigned int MACHFile::getHeader_magic()
{
    return readDword(offsetof(mach_header,magic),isReverse());
}

unsigned int MACHFile::getHeader_cputype()
{
    return readDword(offsetof(mach_header,cputype),isReverse());
}

unsigned int MACHFile::getHeader_cpusubtype()
{
    return readDword(offsetof(mach_header,cpusubtype),isReverse());
}

unsigned int MACHFile::getHeader_filetype()
{
    return readDword(offsetof(mach_header,filetype),isReverse());
}

unsigned int MACHFile::getHeader_ncmds()
{
    return readDword(offsetof(mach_header,ncmds),isReverse());
}

unsigned int MACHFile::getHeader_sizeofcmds()
{
    return readDword(offsetof(mach_header,sizeofcmds),isReverse());
}

unsigned int MACHFile::getHeader_flags()
{
    return readDword(offsetof(mach_header,flags),isReverse());
}

unsigned int MACHFile::getHeader_reserved()
{
    return readDword(offsetof(mach_header_64,reserved),isReverse());
}

void MACHFile::setHeader_magic(unsigned int nValue)
{
    writeDword(offsetof(mach_header,magic),nValue,isReverse());

    if((nValue==MH_CIGAM)||(nValue==MH_CIGAM_64))
    {
        nIsReverse=1;
    }
    else
    {
        nIsReverse=0;
    }
}

void MACHFile::setHeader_cputype(unsigned int nValue)
{
    writeDword(offsetof(mach_header,cputype),nValue,isReverse());
}

void MACHFile::setHeader_cpusubtype(unsigned int nValue)
{
    writeDword(offsetof(mach_header,cpusubtype),nValue,isReverse());
}

void MACHFile::setHeader_filetype(unsigned int nValue)
{
    writeDword(offsetof(mach_header,filetype),nValue,isReverse());
}

void MACHFile::setHeader_ncmds(unsigned int nValue)
{
    writeDword(offsetof(mach_header,ncmds),nValue,isReverse());
}

void MACHFile::setHeader_sizeofcmds(unsigned int nValue)
{
    writeDword(offsetof(mach_header,sizeofcmds),nValue,isReverse());
}

void MACHFile::setHeader_flags(unsigned int nValue)
{
    writeDword(offsetof(mach_header,flags),nValue,isReverse());
}

void MACHFile::setHeader_reserved(unsigned int nValue)
{
    writeDword(offsetof(mach_header_64,reserved),nValue,isReverse());
}

unsigned int MACHFile::getMachHeaderOffset()
{
    return 0;
}

unsigned int MACHFile::getMachHeaderSize()
{
    if(isMACH64())
    {
        return sizeof(mach_header_64);
    }
    else
    {
        return sizeof(mach_header);
    }
}

bool MACHFile::isLoadCommandPresent(unsigned int nLoadCommand)
{
    if(nLoadCommand<getHeader_ncmds())
    {
        return true;
    }

    emit appendError(QString("Invalid LoadCommand number: %1").arg(nLoadCommand));

    return false;
}

bool MACHFile::isSegmentPresent(unsigned int nSegment)
{
    if(nSegment<getNumberOfSegments())
    {
        return true;
    }

    emit appendError(QString("Invalid Segment number: %1").arg(nSegment));

    return false;
}

bool MACHFile::isSectionPresent(unsigned int nSection)
{
    if(nSection<getNumberOfSections())
    {
        return true;
    }

    emit appendError(QString("Invalid Section number: %1").arg(nSection));

    return false;
}

QString MACHFile::loadCommandTypeToString(unsigned int nType)
{
    QString sResult;

    if(nType==LC_REQ_DYLD)
    {
        sResult="LC_REQ_DYLD";
    }
    else if(nType==LC_SEGMENT)
    {
        sResult="LC_SEGMENT";
    }
    else if(nType==LC_SYMTAB)
    {
        sResult="LC_SYMTAB";
    }
    else if(nType==LC_SYMSEG)
    {
        sResult="LC_SYMSEG";
    }
    else if(nType==LC_THREAD)
    {
        sResult="LC_THREAD";
    }
    else if(nType==LC_UNIXTHREAD)
    {
        sResult="LC_UNIXTHREAD";
    }
    else if(nType==LC_LOADFVMLIB)
    {
        sResult="LC_LOADFVMLIB";
    }
    else if(nType==LC_IDFVMLIB)
    {
        sResult="LC_IDFVMLIB";
    }
    else if(nType==LC_IDENT)
    {
        sResult="LC_IDENT";
    }
    else if(nType==LC_FVMFILE)
    {
        sResult="LC_FVMFILE";
    }
    else if(nType==LC_PREPAGE)
    {
        sResult="LC_PREPAGE";
    }
    else if(nType==LC_DYSYMTAB)
    {
        sResult="LC_DYSYMTAB";
    }
    else if(nType==LC_LOAD_DYLIB)
    {
        sResult="LC_LOAD_DYLIB";
    }
    else if(nType==LC_ID_DYLIB)
    {
        sResult="LC_ID_DYLIB";
    }
    else if(nType==LC_LOAD_DYLINKER)
    {
        sResult="LC_LOAD_DYLINKER";
    }
    else if(nType==LC_ID_DYLINKER)
    {
        sResult="LC_ID_DYLINKER";
    }
    else if(nType==LC_PREBOUND_DYLIB)
    {
        sResult="LC_PREBOUND_DYLIB";
    }
    else if(nType==LC_ROUTINES)
    {
        sResult="LC_ROUTINES";
    }
    else if(nType==LC_SUB_FRAMEWORK)
    {
        sResult="LC_SUB_FRAMEWORK";
    }
    else if(nType==LC_SUB_UMBRELLA)
    {
        sResult="LC_SUB_UMBRELLA";
    }
    else if(nType==LC_SUB_CLIENT)
    {
        sResult="LC_SUB_CLIENT";
    }
    else if(nType==LC_SUB_LIBRARY)
    {
        sResult="LC_SUB_LIBRARY";
    }
    else if(nType==LC_TWOLEVEL_HINTS)
    {
        sResult="LC_TWOLEVEL_HINTS";
    }
    else if(nType==LC_PREBIND_CKSUM)
    {
        sResult="LC_PREBIND_CKSUM";
    }
    else if(nType==LC_LOAD_WEAK_DYLIB)
    {
        sResult="LC_LOAD_WEAK_DYLIB";
    }
    else if(nType==LC_SEGMENT_64)
    {
        sResult="LC_SEGMENT_64";
    }
    else if(nType==LC_ROUTINES_64)
    {
        sResult="LC_ROUTINES_64";
    }
    else if(nType==LC_UUID)
    {
        sResult="LC_UUID";
    }
    else if(nType==LC_RPATH)
    {
        sResult="LC_RPATH";
    }
    else if(nType==LC_CODE_SIGNATURE)
    {
        sResult="LC_CODE_SIGNATURE";
    }
    else if(nType==LC_SEGMENT_SPLIT_INFO)
    {
        sResult="LC_SEGMENT_SPLIT_INFO";
    }
    else if(nType==LC_REEXPORT_DYLIB)
    {
        sResult="LC_REEXPORT_DYLIB";
    }
    else if(nType==LC_LAZY_LOAD_DYLIB)
    {
        sResult="LC_LAZY_LOAD_DYLIB";
    }
    else if(nType==LC_ENCRYPTION_INFO)
    {
        sResult="LC_ENCRYPTION_INFO";
    }
    else if(nType==LC_DYLD_INFO)
    {
        sResult="LC_DYLD_INFO";
    }
    else if(nType==LC_DYLD_INFO_ONLY)
    {
        sResult="LC_DYLD_INFO_ONLY";
    }
    else
    {
        sResult=QString::number(nType,16);
    }

    return sResult;
}

unsigned int MACHFile::getNumberOfSegments()
{
    unsigned int nNumberOfCommands=getHeader_ncmds();

    unsigned int nResult=0;

    for(unsigned int i=0; i<nNumberOfCommands; i++)
    {
        if((getLoadCommand_type(i)==LC_SEGMENT)||(getLoadCommand_type(i)==LC_SEGMENT_64))
        {
            nResult++;
            if(nResult>1000)
            {
                break;
            }
        }
    }

    return nResult;
}

unsigned int MACHFile::getSegmentHeaderOffset(unsigned int nSegment)
{
    unsigned int nNumberOfCommands=getHeader_ncmds();
    unsigned int nResult=0;

    for(unsigned int i=0; i<nNumberOfCommands; i++)
    {
        if((getLoadCommand_type(i)==LC_SEGMENT)||(getLoadCommand_type(i)==LC_SEGMENT_64))
        {
            nResult++;
        }

        if(nResult-1==nSegment)
        {
            return getLoadCommand_offset(nResult-1);
        }


    }

    emit appendError(QString("Invalid segment number: %1").arg(nSegment));

    return 0;
}

unsigned int MACHFile::getSegmentHeaderSize()
{
    return isMACH64()?sizeof(segment_command_64):sizeof(segment_command);
}

QString MACHFile::getSegment_segname32(unsigned int nSegment)
{
    return getAnsiString(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,segname),16);
}

unsigned int MACHFile::getSegment_vmaddr32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,vmaddr),isReverse());
}

unsigned int MACHFile::getSegment_vmsize32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,vmsize),isReverse());
}

unsigned int MACHFile::getSegment_fileoff32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,fileoff),isReverse());
}

unsigned int MACHFile::getSegment_filesize32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,filesize),isReverse());
}

unsigned int MACHFile::getSegment_maxprot32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,maxprot),isReverse());
}

unsigned int MACHFile::getSegment_initprot32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,initprot),isReverse());
}

unsigned int MACHFile::getSegment_nsects32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,nsects),isReverse());
}

unsigned int MACHFile::getSegment_flags32(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command,flags),isReverse());
}

QString MACHFile::getSegment_segname64(unsigned int nSegment)
{
    return getAnsiString(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,segname),16);
}

unsigned long long MACHFile::getSegment_vmaddr64(unsigned int nSegment)
{
    return readQword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,vmaddr),isReverse());
}

unsigned long long MACHFile::getSegment_vmsize64(unsigned int nSegment)
{
    return readQword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,vmsize),isReverse());
}

unsigned long long MACHFile::getSegment_fileoff64(unsigned int nSegment)
{
    return readQword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,fileoff),isReverse());
}

unsigned long long MACHFile::getSegment_filesize64(unsigned int nSegment)
{
    return readQword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,filesize),isReverse());
}

unsigned int MACHFile::getSegment_maxprot64(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,maxprot),isReverse());
}

unsigned int MACHFile::getSegment_initprot64(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,initprot),isReverse());
}

unsigned int MACHFile::getSegment_nsects64(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,nsects),isReverse());
}

unsigned int MACHFile::getSegment_flags64(unsigned int nSegment)
{
    return readDword(getSegmentHeaderOffset(nSegment)+offsetof(segment_command_64,flags),isReverse());
}

unsigned int MACHFile::getSegment_nsects(unsigned int nSegment)
{
    return isMACH64()?getSegment_nsects64(nSegment):getSegment_nsects32(nSegment);
}

unsigned int MACHFile::getNumberOfSections()
{
    unsigned int nResult=0;
    unsigned int nNumberOfSegments=getNumberOfSegments();

    for(unsigned int i=0; i<nNumberOfSegments; i++)
    {
        nResult+=getSegment_nsects(i);
    }

    return nResult;
}

unsigned int MACHFile::getSectionHeaderOffset(unsigned int nSection)
{
    unsigned int nNumberOfSegments=getNumberOfSegments();
    unsigned int nResult=0;
    unsigned int nCurrentSection=0;

    for(unsigned int i=0; i<nNumberOfSegments; i++)
    {
        nResult=getSegmentHeaderOffset(i);
        nResult+=getSegmentHeaderSize();

        for(unsigned int j=0; j<getSegment_nsects(i); j++)
        {
            if(nCurrentSection+j==nSection)
            {
                return nResult;
            }

            nResult+=getSectionHeaderSize();
        }

        nCurrentSection+=getSegment_nsects(i);
    }

    emit appendError(QString("Invalid section number: %1").arg(nSection));

    return 0;
}

unsigned int MACHFile::getSectionHeaderSize()
{
    return isMACH64()?sizeof(section_64):sizeof(section);
}

QString MACHFile::getSection_sectname32(unsigned int nSection)
{
    return getAnsiString(getSectionHeaderOffset(nSection)+offsetof(section,sectname),16);
}

QString MACHFile::getSection_segname32(unsigned int nSection)
{
    return getAnsiString(getSectionHeaderOffset(nSection)+offsetof(section,segname),16);
}

unsigned int MACHFile::getSection_addr32(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section,addr),isReverse());
}

unsigned int MACHFile::getSection_size32(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section,size),isReverse());
}

unsigned int MACHFile::getSection_offset32(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section,offset),isReverse());
}

unsigned int MACHFile::getSection_align32(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section,align),isReverse());
}

unsigned int MACHFile::getSection_reloff32(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section,reloff),isReverse());
}

unsigned int MACHFile::getSection_nreloc32(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section,nreloc),isReverse());
}

unsigned int MACHFile::getSection_flags32(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section,flags),isReverse());
}

QString MACHFile::getSection_sectname64(unsigned int nSection)
{
    return getAnsiString(getSectionHeaderOffset(nSection)+offsetof(section_64,sectname),16);
}

QString MACHFile::getSection_segname64(unsigned int nSection)
{
    return getAnsiString(getSectionHeaderOffset(nSection)+offsetof(section_64,segname),16);
}

unsigned long long MACHFile::getSection_addr64(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset(nSection)+offsetof(section_64,addr),isReverse());
}

unsigned long long MACHFile::getSection_size64(unsigned int nSection)
{
    return readQword(getSectionHeaderOffset(nSection)+offsetof(section_64,size),isReverse());
}

unsigned int MACHFile::getSection_offset64(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section_64,offset),isReverse());
}

unsigned int MACHFile::getSection_align64(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section_64,align),isReverse());
}

unsigned int MACHFile::getSection_reloff64(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section_64,reloff),isReverse());
}

unsigned int MACHFile::getSection_nreloc64(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section_64,nreloc),isReverse());
}

unsigned int MACHFile::getSection_flags64(unsigned int nSection)
{
    return readDword(getSectionHeaderOffset(nSection)+offsetof(section_64,flags),isReverse());
}

unsigned int MACHFile::RVAToOffset32(unsigned int nRVA)
{
    unsigned int nNumberOfSegments=getNumberOfSegments();

    for(unsigned int i=0; i<nNumberOfSegments; i++)
    {
        if((getSegment_vmaddr32(i)<=nRVA)&&(nRVA<getSegment_vmaddr32(i)+getSegment_vmsize32(i)))
        {
            return getSegment_fileoff32(i)+nRVA-getSegment_vmaddr32(i);
        }
    }

    emit appendWarning(QString("Invalid RVA: %1").arg(nRVA,8,16,QChar('0')));
    return -1;
}

unsigned int MACHFile::OffsetToRVA32(unsigned int nOffset)
{
    unsigned int nNumberOfSegments=getNumberOfSegments();

    for(unsigned int i=0; i<nNumberOfSegments; i++)
    {
        if((getSegment_fileoff32(i)<=nOffset)&&(nOffset<getSegment_fileoff32(i)+getSegment_filesize32(i)))
        {
            return getSegment_vmaddr32(i)+nOffset-getSegment_fileoff32(i);
        }
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,8,16,QChar('0')));
    return -1;
}

unsigned long long MACHFile::OffsetToRVA64(unsigned long long nOffset)
{
    unsigned int nNumberOfSegments=getNumberOfSegments();

    for(unsigned int i=0; i<nNumberOfSegments; i++)
    {
        if((getSegment_fileoff64(i)<=nOffset)&&(nOffset<getSegment_fileoff64(i)+getSegment_filesize64(i)))
        {
            return getSegment_vmaddr64(i)+nOffset-getSegment_fileoff64(i);
        }
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,8,16,QChar('0')));
    return -1;
}

unsigned long long MACHFile::RVAToOffset64(unsigned long long nRVA)
{
    unsigned int nNumberOfSegments=getNumberOfSegments();

    for(unsigned int i=0; i<nNumberOfSegments; i++)
    {
        if((getSegment_vmaddr64(i)<=nRVA)&&(nRVA<getSegment_vmaddr64(i)+getSegment_vmsize64(i)))
        {
            return getSegment_fileoff64(i)+nRVA-getSegment_vmaddr64(i);
        }
    }

    emit appendWarning(QString("Invalid RVA: %1").arg(nRVA,8,16,QChar('0')));
    return -1;
}

unsigned long long MACHFile::OffsetToRVA(unsigned long long nOffset)
{
    if(isMACH64())
    {
        return OffsetToRVA64(nOffset);
    }
    else
    {
        return OffsetToRVA32(nOffset);
    }
}

unsigned long long MACHFile::RVAToOffset(unsigned long long nRVA)
{
    if(isMACH64())
    {
        return RVAToOffset64(nRVA);
    }
    else
    {
        return RVAToOffset32(nRVA);
    }
}

unsigned int MACHFile::VAToOffset32(unsigned int nVA)
{
    return RVAToOffset32(nVA);
}

unsigned long long MACHFile::VAToOffset64(unsigned long long nVA)
{
    return RVAToOffset64(nVA);
}

unsigned long long MACHFile::VAToOffset(unsigned long long nVA)
{
    unsigned long long nResult=0;

    if(isMACH64())
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

unsigned int MACHFile::OffsetToVA32(unsigned int nOffset)
{
    return OffsetToRVA32(nOffset);
}

unsigned long long MACHFile::OffsetToVA64(unsigned long long nOffset)
{
    return OffsetToRVA64(nOffset);
}

unsigned long long MACHFile::OffsetToVA(unsigned long long nOffset)
{
    unsigned long long nResult=0;

    if(isMACH64())
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

unsigned long long MACHFile::getEntryPoint()
{
    unsigned long long nResult=0;

    unsigned int nNumberOfCommands=getHeader_ncmds();

    if(nNumberOfCommands>0xFF)
    {
        nNumberOfCommands=0xFF;
    }

    for(unsigned int i=0; i<nNumberOfCommands; i++)
    {
        if((getLoadCommand_type(i)==LC_THREAD)||(getLoadCommand_type(i)==LC_UNIXTHREAD))
        {
            unsigned int nOffset=getLoadCommand_offset(i);

            unsigned int nFlavor=readDword(nOffset+8,isReverse());

            if(nFlavor==x86_THREAD_STATE32)
            {
                nResult=readDword(nOffset+16+offsetof(F_STRUCT_X86_THREAD_STATE32,eip),isReverse());
            }
            else if(nFlavor==x86_THREAD_STATE64)
            {
                nResult=readQword(nOffset+16+offsetof(F_STRUCT_X86_THREAD_STATE64,rip),isReverse());
            }
        }
    }

    return nResult;
}

QList<load_command> MACHFile::getLoadCommands()
{
    QList<load_command> listResult;

    load_command record;

    unsigned int nNumberOfCommands=getHeader_ncmds();

    if(nNumberOfCommands>0xFF)
    {
        nNumberOfCommands=0xFF;
    }

    unsigned int nOffset=getMachHeaderSize();

    for(unsigned int i=0; i<nNumberOfCommands; i++)
    {
        record.cmd=readDword(nOffset+offsetof(load_command,cmd),isReverse());
        record.cmdsize=readDword(nOffset+offsetof(load_command,cmdsize),isReverse());

        listResult.append(record);

        nOffset+=record.cmdsize;
    }

    return listResult;
}

QList<load_command_offset> MACHFile::getLoadCommands_offset()
{
    QList<load_command_offset> listResult;

    load_command_offset record;

    unsigned int nNumberOfCommands=getHeader_ncmds();

    if(nNumberOfCommands>0xFF)
    {
        nNumberOfCommands=0xFF;
    }

    unsigned int nOffset=getMachHeaderSize();

    for(unsigned int i=0; i<nNumberOfCommands; i++)
    {
        record.cmd=readDword(nOffset+offsetof(load_command,cmd),isReverse());
        record.cmdsize=readDword(nOffset+offsetof(load_command,cmdsize),isReverse());
        record.offset=nOffset;

        listResult.append(record);

        nOffset+=record.cmdsize;
    }



    return listResult;
}

QList<segment_command> MACHFile::getSegmentsList32()
{
    QList<load_command_offset> list=getLoadCommands_offset();
    unsigned int nOffset=0;

    QList<segment_command> listResult;

    segment_command record;

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).cmd==LC_SEGMENT)
        {
            nOffset=list.at(i).offset;

            //            uint32_t cmd;
            //            uint32_t cmdsize;
            //            char segname[16];
            //            uint32_t vmaddr;
            //            uint32_t vmsize;
            //            uint32_t fileoff;
            //            uint32_t filesize;
            //            vm_prot_t maxprot;
            //            vm_prot_t initprot;
            //            uint32_t nsects;
            //            uint32_t flags;

            record.cmd=readDword(nOffset+offsetof(segment_command,cmd),isReverse());
            record.cmdsize=readDword(nOffset+offsetof(segment_command,cmdsize),isReverse());
            readArray(nOffset+offsetof(segment_command,segname),record.segname,16);
            record.vmaddr=readDword(nOffset+offsetof(segment_command,vmaddr),isReverse());
            record.vmsize=readDword(nOffset+offsetof(segment_command,vmsize),isReverse());
            record.fileoff=readDword(nOffset+offsetof(segment_command,fileoff),isReverse());
            record.filesize=readDword(nOffset+offsetof(segment_command,filesize),isReverse());
            record.maxprot=readDword(nOffset+offsetof(segment_command,maxprot),isReverse());
            record.initprot=readDword(nOffset+offsetof(segment_command,initprot),isReverse());
            record.nsects=readDword(nOffset+offsetof(segment_command,nsects),isReverse());
            record.flags=readDword(nOffset+offsetof(segment_command,flags),isReverse());

            listResult.append(record);
        }
    }

    return listResult;
}

QList<segment_command_64> MACHFile::getSegmentsList64()
{
    QList<load_command_offset> list=getLoadCommands_offset();
    unsigned int nOffset=0;

    QList<segment_command_64> listResult;

    segment_command_64 record;

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).cmd==LC_SEGMENT_64)
        {
            nOffset=list.at(i).offset;

            record.cmd=readDword(nOffset+offsetof(segment_command_64,cmd),isReverse());
            record.cmdsize=readDword(nOffset+offsetof(segment_command_64,cmdsize),isReverse());
            readArray(nOffset+offsetof(segment_command_64,segname),record.segname,16);
            record.vmaddr=readQword(nOffset+offsetof(segment_command_64,vmaddr),isReverse());
            record.vmsize=readQword(nOffset+offsetof(segment_command_64,vmsize),isReverse());
            record.fileoff=readQword(nOffset+offsetof(segment_command_64,fileoff),isReverse());
            record.filesize=readQword(nOffset+offsetof(segment_command_64,filesize),isReverse());
            record.maxprot=readDword(nOffset+offsetof(segment_command_64,maxprot),isReverse());
            record.initprot=readDword(nOffset+offsetof(segment_command_64,initprot),isReverse());
            record.nsects=readDword(nOffset+offsetof(segment_command_64,nsects),isReverse());
            record.flags=readDword(nOffset+offsetof(segment_command_64,flags),isReverse());

            listResult.append(record);
        }
    }

    return listResult;
}

QList<section> MACHFile::getSectionsList32()
{
    QList<section> listResult;

    //    return listResult;
    QList<load_command_offset> list=getLoadCommands_offset();
    unsigned int nOffset=0;

    section record;

    unsigned int nNumberOfSections=0;

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).cmd==LC_SEGMENT)
        {
            nOffset=list.at(i).offset;

            nNumberOfSections=readDword(nOffset+offsetof(segment_command,nsects),isReverse());

            nOffset+=sizeof(segment_command);

            for(unsigned int j=0; j<nNumberOfSections; j++)
            {
                readArray(nOffset+offsetof(section,sectname),record.sectname,16);
                readArray(nOffset+offsetof(section,segname),record.segname,16);
                record.addr=readDword(nOffset+offsetof(section,addr),isReverse());
                record.size=readDword(nOffset+offsetof(section,size),isReverse());
                record.offset=readDword(nOffset+offsetof(section,offset),isReverse());
                record.align=readDword(nOffset+offsetof(section,align),isReverse());
                record.reloff=readDword(nOffset+offsetof(section,reloff),isReverse());
                record.nreloc=readDword(nOffset+offsetof(section,nreloc),isReverse());
                record.flags=readDword(nOffset+offsetof(section,flags),isReverse());
                record.reserved1=readDword(nOffset+offsetof(section,reserved1),isReverse());
                record.reserved2=readDword(nOffset+offsetof(section,reserved2),isReverse());

                listResult.append(record);
                nOffset+=sizeof(section);
            }
        }
    }

    return listResult;
}

QList<section_64> MACHFile::getSectionsList64()
{
    QList<section_64> listResult;


    QList<load_command_offset> list=getLoadCommands_offset();
    unsigned int nOffset=0;

    section_64 record;

    unsigned int nNumberOfSections=0;

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).cmd==LC_SEGMENT_64)
        {
            nOffset=list.at(i).offset;

            nNumberOfSections=readDword(nOffset+offsetof(segment_command_64,nsects),isReverse());

            nOffset+=sizeof(segment_command_64);

            for(unsigned int j=0; j<nNumberOfSections; j++)
            {
                readArray(nOffset+offsetof(section_64,sectname),record.sectname,16);
                readArray(nOffset+offsetof(section_64,segname),record.segname,16);
                record.addr=readQword(nOffset+offsetof(section_64,addr),isReverse());
                record.size=readQword(nOffset+offsetof(section_64,size),isReverse());
                record.offset=readDword(nOffset+offsetof(section_64,offset),isReverse());
                record.align=readDword(nOffset+offsetof(section_64,align),isReverse());
                record.reloff=readDword(nOffset+offsetof(section_64,reloff),isReverse());
                record.nreloc=readDword(nOffset+offsetof(section_64,nreloc),isReverse());
                record.flags=readDword(nOffset+offsetof(section_64,flags),isReverse());
                record.reserved1=readDword(nOffset+offsetof(section_64,reserved1),isReverse());
                record.reserved2=readDword(nOffset+offsetof(section_64,reserved2),isReverse());

                listResult.append(record);
                nOffset+=sizeof(section_64);
            }
        }
    }

    return listResult;
}

bool MACHFile::dumpSegment(QString sFileName, int nSegment)
{
    unsigned int nOffset=0;
    unsigned int nSize=0;

    if(isMACH64())
    {
        nOffset=getSegment_fileoff64(nSegment);
        nSize=getSegment_filesize64(nSegment);
    }
    else
    {
        nOffset=getSegment_fileoff32(nSegment);
        nSize=getSegment_filesize32(nSegment);
    }

    return dump(sFileName,nOffset,nSize);
}

bool MACHFile::dumpSection(QString sFileName, int nSection)
{
    unsigned int nOffset=0;
    unsigned int nSize=0;

    if(isMACH64())
    {
        nOffset=getSection_offset64(nSection);
        nSize=getSection_size64(nSection);
    }
    else
    {
        nOffset=getSection_offset32(nSection);
        nSize=getSection_size32(nSection);
    }

    return dump(sFileName,nOffset,nSize);
}

bool MACHFile::isSectionNamePresent(QString sSectionName)
{
    QString sTemp;

    if(isMACH64())
    {
        QList<section_64> list64=getSectionsList64();

        for(int i=0; i<list64.count(); i++)
        {
            sTemp=list64.at(i).sectname;

            if(sTemp.length()>16)
            {
                sTemp.resize(16);
            }

            if(sTemp==sSectionName)
            {
                return true;
            }
        }
    }
    else
    {
        QList<section> list32=getSectionsList32();

        for(int i=0; i<list32.count(); i++)
        {
            sTemp=list32.at(i).sectname;

            if(sTemp.length()>16)
            {
                sTemp.resize(16);
            }

            if(sTemp==sSectionName)
            {
                return true;
            }
        }
    }

    return false;
}

unsigned int MACHFile::getSectionNumber(QString sSectionName)
{
    QString sTemp;

    if(isMACH64())
    {
        QList<section_64> list64=getSectionsList64();

        for(int i=0; i<list64.count(); i++)
        {
            sTemp=list64.at(i).sectname;

            if(sTemp==sSectionName)
            {
                return i;
            }
        }
    }
    else
    {
        QList<section> list32=getSectionsList32();

        for(int i=0; i<list32.count(); i++)
        {
            sTemp=list32.at(i).sectname;

            if(sTemp==sSectionName)
            {
                return i;
            }
        }
    }

    return -1;
}

bool MACHFile::isUUIDPresent()
{
    QList<load_command> list=getLoadCommands();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).cmd==LC_UUID)
        {
            return true;
        }
    }

    return false;
}

QByteArray MACHFile::getUUID()
{
    QByteArray baResult;
    QList<load_command_offset> list=getLoadCommands_offset();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).cmd==LC_UUID)
        {
            unsigned int nOffset=list.at(i).offset;
            nOffset+=sizeof(load_command);

            baResult=readArray(nOffset,list.at(i).cmdsize-sizeof(load_command));

            break;
        }
    }

    return baResult;
}

QString MACHFile::getUUIDAsString()
{
    QByteArray baUUID=getUUID();

    return baUUID.toHex();
}

bool MACHFile::setUUID(QByteArray baUUID)
{
    QList<load_command_offset> list=getLoadCommands_offset();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).cmd==LC_UUID)
        {
            unsigned int nOffset=list.at(i).offset;
            nOffset+=sizeof(load_command);

            if((int)(list.at(i).cmdsize-sizeof(load_command))==baUUID.size())
            {
                return writeArray(nOffset,baUUID.data(),baUUID.size());
            }
        }
    }

    return false;
}

bool MACHFile::setUUIDFromString(QString sUUID)
{
    QByteArray baUUID(sUUID.toLatin1().data(),sUUID.size());

    baUUID=QByteArray::fromHex(baUUID);

    return setUUID(baUUID);
}

QList<DYLIB_FULL> MACHFile::getLibs()
{
    QList<DYLIB_FULL> listResult;

    QList<load_command_offset> list=getLoadCommands_offset();

    for(int i=0; i<list.count(); i++)
    {
        if((list.at(i).cmd==LC_LOAD_DYLIB)||(list.at(i).cmd==LC_LOAD_WEAK_DYLIB)||(list.at(i).cmd==LC_ID_DYLIB))
        {
            unsigned int nOffset=list.at(i).offset;
            nOffset+=sizeof(load_command);

            DYLIB_FULL record;
            unsigned int nStringOffset=readDword(nOffset,isReverse());
            nOffset+=4;
            record.timestamp=readDword(nOffset,isReverse());
            nOffset+=4;
            record.current_version=readDword(nOffset,isReverse());
            nOffset+=4;
            record.compatibility_version=readDword(nOffset,isReverse());
            record.sVollName=getString(list.at(i).offset+nStringOffset,list.at(i).cmdsize-20);

            unsigned int nCount=record.sVollName.count("/");
            record.sShortName=record.sVollName.section("/",nCount,nCount);

            listResult.append(record);
        }
    }

    return listResult;
}

bool MACHFile::isLibraryPresent(QString sLibrary)
{
    QList<DYLIB_FULL> list=getLibs();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).sShortName==sLibrary)
        {
            return true;
        }
    }

    return false;
}

unsigned int MACHFile::getLibraryCurrentVersion(QString sLibrary)
{
    QList<DYLIB_FULL> list=getLibs();

    for(int i=0; i<list.count(); i++)
    {
        if(list.at(i).sShortName==sLibrary)
        {
            unsigned int nResult=list.at(i).current_version;
            return nResult;
        }
    }

    return 0;
}

unsigned int MACHFile::getSectionFileOffset(int nSection)
{
    if(isMACH64())
    {
        return getSection_offset64(nSection);
    }
    else
    {
        return getSection_offset32(nSection);
    }
}

unsigned int MACHFile::getSectionFileSize(int nSection)
{
    if(isMACH64())
    {
        return getSection_size64(nSection);
    }
    else
    {
        return getSection_size32(nSection);
    }
}

QString MACHFile::getSectionName(int nSection)
{
    if(isMACH64())
    {
        return getSection_sectname64(nSection);
    }
    else
    {
        return getSection_sectname32(nSection);
    }
}

QString MACHFile::getSegmentName(int nSegment)
{
    if(isMACH64())
    {
        return getSegment_segname64(nSegment);
    }
    else
    {
        return getSegment_segname32(nSegment);
    }
}

unsigned int MACHFile::getSegmentFileSize(int nSegment)
{
    if(isMACH64())
    {
        return getSegment_filesize64(nSegment);
    }
    else
    {
        return getSegment_filesize32(nSegment);
    }
}

unsigned int MACHFile::getSegmentFileOffset(int nSegment)
{
    if(isMACH64())
    {
        return getSegment_fileoff64(nSegment);
    }
    else
    {
        return getSegment_fileoff32(nSegment);
    }
}

bool MACHFile::compareEP(QString sSignature, unsigned int nOffset)
{
    return compare(sSignature,getEntryPointOffset()+nOffset);
}

unsigned long long MACHFile::getEntryPointOffset()
{
    return RVAToOffset(getEntryPoint());
}

unsigned int MACHFile::calculateRawSize()
{
    unsigned int nResult=getMachHeaderSize();

    for(unsigned int i=0; i<getNumberOfSegments(); i++)
    {
        nResult=qMax(nResult,getSegmentFileOffset(i)+getSegmentFileSize(i));
    }

    return nResult;
}

unsigned int MACHFile::getOverlaySize()
{
    return MAXIMAL((long long)(this->size())-(long long)calculateRawSize(),0);
}

unsigned int MACHFile::getOverlayOffset()
{
    return calculateRawSize();
}

bool MACHFile::isOverlayPresent()
{
    return getOverlaySize()!=0;
}

bool MACHFile::compareOverlay(QString sSignature, unsigned int nOffset)
{
    return compare(sSignature,getOverlayOffset()+nOffset);
}


unsigned int MACHFile::getLoadCommand_type(unsigned int nLoadCommand)
{
    return readDword(getLoadCommand_offset(nLoadCommand)+offsetof(load_command,cmd),isReverse());
}

unsigned int MACHFile::getLoadCommand_size(unsigned int nLoadCommand)
{
    return readDword(getLoadCommand_offset(nLoadCommand)+offsetof(load_command,cmdsize),isReverse());
}

unsigned int MACHFile::getLoadCommand_offset(unsigned int nLoadCommand)
{
    if(isLoadCommandPresent(nLoadCommand))
    {
        unsigned int nOffset=getMachHeaderSize();

        for(unsigned int i=0; i<nLoadCommand; i++)
        {
            nOffset+=readDword(nOffset+offsetof(load_command,cmdsize),isReverse());
        }

        return nOffset;
    }

    return 0;
}
