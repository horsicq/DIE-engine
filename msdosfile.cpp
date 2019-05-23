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
#include "msdosfile.h"

MSDOSFile::MSDOSFile(QObject *parent) :
    Binary(parent)
{
}

bool MSDOSFile::isValid()
{
    if(size()>=(int)sizeof(IMAGE_DOS_HEADER))
    {
        if(getDosHeader_magic()==IMAGE_DOS_SIGNATURE)
        {
            return true;
        }
        else if(getDosHeader_magic()==0x4D5A)
        {
            return true;
        }
    }

    emit appendError("Invalid MSDOS file");

    return false;
}

unsigned int MSDOSFile::getDosHeaderOffset()
{
    return 0;
}
unsigned int MSDOSFile::getDosHeaderSize()
{
    return sizeof(IMAGE_DOS_HEADER);
}
unsigned short MSDOSFile::getDosHeader_magic()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_magic));
}
unsigned short MSDOSFile::getDosHeader_cblp()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cblp));
}
unsigned short MSDOSFile::getDosHeader_cp()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cp));
}
unsigned short MSDOSFile::getDosHeader_crlc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_crlc));
}
unsigned short MSDOSFile::getDosHeader_cparhdr()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cparhdr));
}
unsigned short MSDOSFile::getDosHeader_minalloc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_minalloc));
}
unsigned short MSDOSFile::getDosHeader_maxalloc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_maxalloc));
}
unsigned short MSDOSFile::getDosHeader_ss()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_ss));
}
unsigned short MSDOSFile::getDosHeader_sp()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_sp));
}
unsigned short MSDOSFile::getDosHeader_csum()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_csum));
}
unsigned short MSDOSFile::getDosHeader_ip()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_ip));
}
unsigned short MSDOSFile::getDosHeader_cs()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_cs));
}
unsigned short MSDOSFile::getDosHeader_lfarlc()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_lfarlc));
}
unsigned short MSDOSFile::getDosHeader_ovno()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_ovno));
}
unsigned short MSDOSFile::getDosHeader_res(int nIndex)
{
    if(nIndex<4)
    {
        return readWord(offsetof(IMAGE_DOS_HEADER,e_res)+sizeof(unsigned short)*nIndex);
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
    return 0;
}
unsigned short MSDOSFile::getDosHeader_oemid()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_oemid));
}
unsigned short MSDOSFile::getDosHeader_oeminfo()
{
    return readWord(offsetof(IMAGE_DOS_HEADER,e_oeminfo));
}
unsigned short MSDOSFile::getDosHeader_res2(int nIndex)
{
    if(nIndex<10)
    {
        return readWord(offsetof(IMAGE_DOS_HEADER,e_res2)+sizeof(unsigned short)*nIndex);
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
    return 0;
}
void MSDOSFile::setDosHeader_magic(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_magic),sValue);
}
void MSDOSFile::setDosHeader_cblp(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cblp),sValue);
}
void MSDOSFile::setDosHeader_cp(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cp),sValue);
}
void MSDOSFile::setDosHeader_crlc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_crlc),sValue);
}
void MSDOSFile::setDosHeader_cparhdr(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cparhdr),sValue);
}
void MSDOSFile::setDosHeader_minalloc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_minalloc),sValue);
}
void MSDOSFile::setDosHeader_maxalloc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_maxalloc),sValue);
}
void MSDOSFile::setDosHeader_ss(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_ss),sValue);
}
void MSDOSFile::setDosHeader_sp(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_sp),sValue);
}
void MSDOSFile::setDosHeader_csum(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_csum),sValue);
}
void MSDOSFile::setDosHeader_ip(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_ip),sValue);
}
void MSDOSFile::setDosHeader_cs(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_cs),sValue);
}
void MSDOSFile::setDosHeader_lfarlc(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_lfarlc),sValue);
}
void MSDOSFile::setDosHeader_ovno(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_ovno),sValue);
}
void MSDOSFile::setDosHeader_res(short sValue,int nIndex)
{
    if(nIndex<4)
    {
        writeWord(offsetof(IMAGE_DOS_HEADER,e_res)+sizeof(unsigned short)*nIndex,sValue);
        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
}
void MSDOSFile::setDosHeader_oemid(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_oemid),sValue);
}
void MSDOSFile::setDosHeader_oeminfo(unsigned short sValue)
{
    writeWord(offsetof(IMAGE_DOS_HEADER,e_oeminfo),sValue);
}
void MSDOSFile::setDosHeader_res2(short sValue,int nIndex)
{
    if(nIndex<10)
    {
        writeWord(offsetof(IMAGE_DOS_HEADER,e_res2)+sizeof(unsigned short)*nIndex,sValue);
        return;
    }

    emit appendError(QString("Value out of range: n=%1").arg(nIndex));
}

unsigned int MSDOSFile::getDosHeader_lfanew()
{
    return readDword(offsetof(IMAGE_DOS_HEADER,e_lfanew));
}

void MSDOSFile::setDosHeader_lfanew(unsigned int nValue)
{
    writeDword(offsetof(IMAGE_DOS_HEADER,e_lfanew),nValue);
}

unsigned int MSDOSFile::getEntryPointOffset()
{
    //    Var eh9,eha,eh21,eh22,eh23,eh24,cs,ip,hs,ep:longint;
    //    Begin
    //          eh9:=eh[$9-1]; // cparhdr
    //          eha:=eh[$A-1]; // cparhdr
    //          eh21:=eh[21-1]; // ip
    //          eh22:=eh[22-1]; // ip
    //          eh23:=eh[23-1]; // cs
    //          eh24:=eh[24-1]; // cs
    //          cs:=eh24*$100+eh23;
    //          ip:=eh22*$100+eh21;
    //          hs:=16*(eha*$100+eh9);
    //          ep:=cs*16+ip+hs;
    //          if ep>$100000 then ep:=ep-$100000;
    //          CalcMZ:=ep;
    //    End;

    return RVAToOffset(getDosHeader_ip());
}

unsigned int MSDOSFile::getBaseOffset()
{
    return getDosHeader_cparhdr()*16;
}

bool MSDOSFile::isNE()
{
    unsigned int nOffset=getDosHeader_lfanew();
    unsigned short sNew=readWord(nOffset);

    return (sNew==0x454E); // TODO const
}

bool MSDOSFile::isLE()
{
    unsigned int nOffset=getDosHeader_lfanew();
    unsigned short sNew=readWord(nOffset);

    return (sNew==0x454C); // TODO const
}

bool MSDOSFile::isLX()
{
    unsigned int nOffset=getDosHeader_lfanew();
    unsigned short sNew=readWord(nOffset);

    return (sNew==0x584C); // TODO const
}

bool MSDOSFile::compareEP(QString sSignature, unsigned int nOffset)
{
    return compare(sSignature,getEntryPointOffset()+nOffset);
}

bool MSDOSFile::compareEPfast(QString sSignature)
{
    if(!bIsEntryPontLoad)
    {
        entryPointLoad();
    }

    return Binary::compareEPfast(sSignature,0);
}
void MSDOSFile::entryPointLoad()
{
    nEntryPointOffset=getEntryPointOffset();

    _loadEntryPoint();
}

unsigned long long MSDOSFile::OffsetToRVA(unsigned long long nOffset)
{
    unsigned long long nResult=-1;

    if(isOffsetValid(nOffset))
    {
        int nDelta=(getDosHeader_cparhdr()*16)+((short)getDosHeader_cs()*16);
        nResult=nOffset-nDelta;
    }

    return nResult;

    //    int nDelta=(getDosHeader_cparhdr()*16)+((short)getDosHeader_cs()*16);

    ////    if(nDelta>0x100000)
    ////    {
    ////        nDelta-=0x100000;
    ////    }

    ////    if(nOffset>=((getDosHeader_cparhdr()*16)+(getDosHeader_cs()*16)))
    //    if(nOffset>=nDelta)
    ////    if(nOffset>=((getDosHeader_cparhdr()*16)+((short)getDosHeader_cs()*16)))
    //    {
    ////        nResult=nOffset-((getDosHeader_cparhdr()*16)+(getDosHeader_cs()*16));
    //        nResult=nOffset-nDelta;
    ////        nResult=nOffset-((getDosHeader_cparhdr()*16)+((short)getDosHeader_cs()*16));
    //    }
    //    else
    //    {
    //        nResult=-1;
    //    }
}

unsigned long long MSDOSFile::RVAToOffset(unsigned long long nRVA)
{
    nRVA=nRVA&0xFFFF;
    unsigned long long nResult=(getDosHeader_cparhdr()*16)+(getDosHeader_cs()*16)+nRVA;

    if(nResult>0x100000)
    {
        nResult-=0x100000;
    }

    if(!isOffsetValid(nResult))
    {
        nResult=-1;
    }

    return nResult;
}

unsigned long long MSDOSFile::VAToOffset(unsigned long long nVA)
{
    unsigned long long nResult;

    if(nVA>0xFFFF)
    {
        nResult=(getDosHeader_cparhdr()*16)+((nVA>>16)&0xFFFF)*16+(nVA&0xFFFF);
    }
    else
    {
        nResult=RVAToOffset(nVA);
    }

    if(!isOffsetValid(nResult))
    {
        nResult=-1;
    }

    return nResult;
}

unsigned long long MSDOSFile::VAToOffset32(unsigned long long nVA)
{
    unsigned long long nResult;

    nResult=(getDosHeader_cparhdr()*16)+((nVA>>16)&0xFFFF)*16+(nVA&0xFFFF);

    if(!isOffsetValid(nResult))
    {
        nResult=-1;
    }

    return nResult;
}

unsigned long long MSDOSFile::OffsetToVA(unsigned long long nOffset)
{
    return OffsetToRVA(nOffset);
}

unsigned int MSDOSFile::getOverlayOffset()
{
    //	unsigned int nResult=(getDosHeader_cp()-1)*0x200+getDosHeader_cblp();

	// TODO Check
    unsigned int nResult=0;

    if(isNE())
    {
        unsigned int nNE=readDword(0x3C);
        unsigned int nSegments=readWord(nNE+0x1C);
        unsigned int nShift=readWord(nNE+0x32);

        if(nShift==0)
        {
            nShift=9;
        }

        unsigned int nTable=nNE+readWord(nNE+0x22);
        unsigned int nLast=0;
        unsigned int nSize;
        unsigned int bRelocs;

        for(unsigned int i=0; i<nSegments; i++)
        {
            unsigned int nSeg=readWord(nTable+i*8);

            if(nSeg>nLast)
            {
                nLast=nSeg;
                nSize=readWord(nTable+2+i*8);
                bRelocs=readWord(nTable+4+i*8)&0x100;
            }
        }

        if(nSize==0)
        {
            nSize=65536;
        }

        nResult=(nLast<<nShift)+nSize;

        if(bRelocs)
        {
            nResult+=2+readWord(nLast)*8;
        }

        nResult=(nResult+(1<<nShift)-1)&-(1<<nShift);
    }
    else if(isLE())
    {
        unsigned int nLE=readDword(0x3C);
        unsigned int nPages=readDword(nLE+0x14);
        unsigned int nPageSize=readDword(nLE+0x28);
        unsigned int nLastPage=readDword(nLE+0x2C);
        unsigned int nDataPage=readDword(nLE+0x80);
        nResult=(nPages-1)*nPageSize+nLastPage+nDataPage;
    }
    else
    {
        nResult=((readWord(4)-1)<<9)+readWord(2);
    }

    return nResult;
}
unsigned int MSDOSFile::getOverlaySize()
{
    unsigned int nResult=getOverlayOffset();

    if(nResult>size())
    {
        return 0;
    }

    return size()-nResult;
}
bool MSDOSFile::isOverlayPresent()
{
    return getOverlaySize();
}

bool MSDOSFile::compareOverlay(QString sSignature, unsigned int nOffset)
{
    return compare(sSignature,getOverlayOffset()+nOffset);
}
