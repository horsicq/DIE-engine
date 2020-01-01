// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#include "qxhexsearchthread.h"

QXHexSearchThread::QXHexSearchThread(QObject *parent) :
    QObject(parent)
{
    bSearch=false;
}

unsigned char QXHexSearchThread::signatureToByte(QString *psSignature)
{
    unsigned char cResult;

    QByteArray baData;
    baData.append(*psSignature);

    baData=QByteArray::fromHex(baData);

    baData.resize(1);

    cResult=*((unsigned char *)baData.data());

    return cResult;
}

unsigned short QXHexSearchThread::signatureToWord(QString *psSignature)
{
    unsigned short sResult;

    QByteArray baData;
    baData.append(*psSignature);

    baData=QByteArray::fromHex(baData);

    baData.resize(2);

    sResult=*((unsigned short *)baData.data());

    return sResult;
}

unsigned int QXHexSearchThread::signatureToDword(QString *psSignature)
{
    unsigned int nResult;

    QByteArray baData;
    baData.append(*psSignature);

    baData=QByteArray::fromHex(baData);

    baData.resize(4);

    nResult=*((unsigned int *)baData.data());

    return nResult;
}

unsigned int QXHexSearchThread::_search(QString *psSignature, QByteArray *pbaData)
{
    unsigned int nResult=-1;

    char *pData=pbaData->data();
    unsigned int nSize=pbaData->size();

    //    QByteArray baHEX;

    //    baHEX=baHEX.fromHex(*psSignature);

    if(psSignature->size()==8) // dword
    {
        unsigned int nDword=signatureToDword(psSignature);

        for(unsigned int i=0; i<nSize-3; i++)
        {
            if(*((unsigned int *)(pData+i))==nDword)
            {
                return i;
            }
        }

    }
    else if(psSignature->size()==4)
    {
        unsigned short sWord=signatureToWord(psSignature);

        for(unsigned int i=0; i<nSize-1; i++)
        {
            if(*((unsigned short *)(pData+i))==sWord)
            {
                return i;
            }
        }

    }
    else if(psSignature->size()==2)
    {
        unsigned char cByte=signatureToByte(psSignature);

        for(unsigned int i=0; i<nSize; i++)
        {
            if(*((unsigned char *)(pData+i))==cByte)
            {
                return i;
            }
        }

    }


    return nResult;
}

void QXHexSearchThread::stopSearch()
{
    bSearch=false;
}

void QXHexSearchThread::startSearch()
{
    bSearch=true;

    unsigned long long nOffset=pDataBlock->nSearchOffset;
    unsigned long long nSize=pDataBlock->nMaxSize-(pDataBlock->nSearchOffset-pDataBlock->nStartAddress);
    unsigned int nTemp;
    bool bTemp;
    QByteArray baBuffer;
    QString sTemp;
    unsigned int nBlockOffset;

    if(pDataBlock->nReadBlockMax)
    {
        emit progressRangeChanged(0,nSize/(pDataBlock->nReadBlockMax*100));
    }

    QString sSignature=pDataBlock->sSearchSignature;
    QString sSignature2;
    bool bIsText=false;
    bool bIsWild=false;

    if(sSignature.contains(".")||sSignature.contains("?"))
    {
        bIsWild=true;
    }

    if(sSignature.contains("#"))
    {
        bIsText=true;
        sSignature.remove("#");

        baBuffer=baBuffer.fromHex(sSignature.toLatin1());

        if((baBuffer.size()>2)&&(baBuffer.at(1)==0))
        {
            // Unicode
            sTemp=QString::fromUtf16((ushort *)baBuffer.data(),baBuffer.size()/2);
            sSignature=sTemp.toLower();
            sSignature2=sTemp.toUpper();

            baBuffer.clear();
            baBuffer.append((char *)(sSignature.utf16()),sSignature.size()*2);
            baBuffer=baBuffer.toHex();
            sSignature.clear();
            sSignature.append(baBuffer);

            baBuffer.clear();
            baBuffer.append((char *)(sSignature2.utf16()),sSignature2.size()*2);
            baBuffer=baBuffer.toHex();
            sSignature2.clear();
            sSignature2.append(baBuffer);
        }
        else
        {
            sSignature=baBuffer.toLower().toHex();
            sSignature2=baBuffer.toUpper().toHex();


        }

    }

    int nSignatureSize=sSignature.size();


    unsigned int nBlockSize;


    //    unsigned char cByte;
    //    cByte=signatureToByte(&(pDataBlock->sSearchSignature));
    //    unsigned short sWord;
    //    unsigned int nDword;
    //    if(nSignatureSize==1)
    //    {
    //        cByte=signatureToByte(&(pDataBlock->sSearchSignature));
    //    }
    //    else if(nSignatureSize==2)
    //    {
    //        sWord=signatureToWord(&(pDataBlock->sSearchSignature));
    //    }
    //    else if(nSignatureSize==4)
    //    {
    //        nDword=signatureToDword(&(pDataBlock->sSearchSignature));
    //    }

    int j=0;

    while((nSize>(nSignatureSize-1)/2)&&bSearch)
    {
        nTemp=qMin((unsigned long long)(pDataBlock->nReadBlockMax),nSize);
        baBuffer=pDataBlock->__ReadMemory(pDataBlock->parameter,nOffset,nTemp,&bTemp);
        sTemp.clear();
        sTemp.append(baBuffer.toHex());
        //        pBlock=baBuffer.data();
        nBlockSize=sTemp.size();

        //        nBlockOffset=_search(&(pDataBlock->sSearchSignature),&baBuffer);
        nBlockOffset=-1;

        for(int i=0; (unsigned int)i<nBlockSize-(nSignatureSize-1); i+=2)
        {
            if(bSearch==0)
            {
                goto next;
            }

            //            if(*((unsigned char *)(pBlock+i))==cByte)
            //            {

            //                nBlockOffset=i;
            //                goto next;
            //            }

            if(bIsWild)
            {
                for(int k=0; k<nSignatureSize; k++)
                {
                    if((sSignature[k]!='.')&&(sSignature[k]!='?'))
                    {
                        //                    QString sTemp2=sTemp[i+k];
                        //                    sTemp2=pDataBlock->sSearchSignature[k];
                        if(sTemp[i+k]!=sSignature[k])
                        {
                            goto next2;
                        }
                    }
                }
            }
            else if(bIsText)
            {
                for(int k=0; k<nSignatureSize; k++)
                {
                    if((sTemp[i+k]!=sSignature[k])&&(sTemp[i+k]!=sSignature2[k]))
                    {
                        goto next2;
                    }
                }
            }
            else
            {
                for(int k=0; k<nSignatureSize; k++)
                {
                    if(sTemp[i+k]!=sSignature[k])
                    {
                        goto next2;
                    }
                }
            }

            nBlockOffset=i/2;
            goto next;
next2:
            ;

        }

        //        if(nSignatureSize==1)
        //        {
        //            for(int i=0;i<nBlockSize;i++)
        //            {
        //                if(bSearch==0)
        //                {
        //                    goto next;
        //                }

        //                if(*((unsigned char *)(pBlock+i))==cByte)
        //                {

        //                    nBlockOffset=i;
        //                    goto next;
        //                }
        //            }
        //        }
        //        else if(nSignatureSize==2)
        //        {
        //            for(int i=0;i<nBlockSize-1;i++)
        //            {
        //                if(bSearch==0)
        //                {
        //                    goto next;
        //                }
        //                if(*((unsigned short *)(pBlock+i))==sWord)
        //                {
        //                    nBlockOffset=i;
        //                    goto next;
        //                }
        //            }
        //        }
        //        else if(nSignatureSize==4)
        //        {
        //            for(int i=0;i<nBlockSize-3;i++)
        //            {
        //                if(bSearch==0)
        //                {
        //                    goto next;
        //                }
        //                if(*((unsigned int *)(pBlock+i))==nDword)
        //                {
        //                    nBlockOffset=i;
        //                    goto next;
        //                }
        //            }
        //        }

next:

        if(nBlockOffset!=-1)
        {
            emit _found(nOffset+nBlockOffset,nSignatureSize/2);
            break;
        }

        nTemp=nTemp-(nSignatureSize-1)/2;
        nSize-=nTemp;
        nOffset+=nTemp;


        if(j%100==0)
        {
            emit progressValueChanged(j/100);
        }

        j++;
    }

    if(nBlockOffset==-1)
    {
        emit _nothingfound();
    }

    bSearch=false;
    emit _searchComplete();
}

void QXHexSearchThread::setData(__DATABLOCK *pDataBlock)
{
    this->pDataBlock=pDataBlock;
}
