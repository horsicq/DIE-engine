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
#include "threadsearch.h"

struct INPUT_STRUCT
{
    unsigned long long nOffset;
    unsigned long long nSize;
    unsigned char *pMemory;
    CRYPTO_RECORD cryptorecord;
};

QString findCrypto(INPUT_STRUCT input)
{
    QString sResult;

    QString sSignatureName=input.cryptorecord.sName;
    QString sSignature;
    QByteArray baSignature;

    QString sPart;
    QString sTemp;
    unsigned int nRawSize=0;
    unsigned int nOffset=0;
    unsigned int nSize=input.nSize;
    unsigned int nTempOffset=0;
    bool bFirst=false;
    unsigned long long nStartOffset=0;
    int nSignatureSize=0;
    char *pSignature=0;
    char *pOffset=(char *)input.pMemory;

    sSignature=input.cryptorecord.sSignature;
    QString _sSignature=Binary::convertSignature(sSignature);

    //    if(_sSignature.contains("+")||_sSignature.contains("."))
    if(_sSignature.contains("+"))
    {
        bFirst=true;
        nOffset=0;

        while(nOffset<nSize)
        {
            if(bFirst)
            {
                sSignature=_sSignature;
            }

            if(sSignature=="")
            {
                sResult+=QString("%1;%2;%3;;").arg(sSignatureName).arg(nStartOffset).arg(nOffset-nStartOffset);
                bFirst=true;
                continue;
            }

            sTemp=sSignature;
            nRawSize=sTemp.remove("+").size();

            sPart=sSignature.section("+",0,0);

            baSignature=QByteArray::fromHex(sPart.toLatin1());
            nSignatureSize=baSignature.size();
            pSignature=(char *)(baSignature.data());

            sSignature=sSignature.section("+",1,-1);

            if((int)(nSize-nOffset)<(int)nRawSize)
            {
                nTempOffset=-1;
                break;
            }
            else
            {
                if(bFirst)
                {
                    nTempOffset=Binary::findDataInMemory(pOffset+nOffset,nSize-nOffset,pSignature,nSignatureSize);
                    nStartOffset=nOffset+nTempOffset;
                }
                else
                {
                    nTempOffset=Binary::findDataInMemory(pOffset+nOffset,qMin((int)(nSize-nOffset),20+nSignatureSize),pSignature,nSignatureSize);
                }
            }

            if(nTempOffset==-1)
            {
                if(bFirst)
                {
                    break;
                }
                else
                {
                    bFirst=true;
                    continue;
                }
            }
            else
            {
                if(bFirst)
                {
                    bFirst=false;
                }

                nOffset+=nTempOffset+nSignatureSize;
            }
        }
    }
    else
    {
        baSignature=QByteArray::fromHex(_sSignature.toLatin1());
        nSignatureSize=baSignature.size();
        pSignature=(char *)(baSignature.data());

        for(int j=0; j<(int)(nSize-nSignatureSize); j++)
        {
            if(Binary::memoryCompare(pOffset+j,pSignature,nSignatureSize))
            {
                //!!!
                sResult+=QString("%1;%2;%3;;").arg(sSignatureName).arg(j).arg(nSignatureSize);
            }
        }
    }

    return sResult;
}

ThreadSearch::ThreadSearch(QObject *parent) :
    QObject(parent)
{
    bIsRun=false;
    bIsCompleted=true;
}

void ThreadSearch::setData(SEARCHDATA *pSearchData)
{
    this->pSearchData=pSearchData;
}

void ThreadSearch::_stop()
{
#ifdef QT_DEBUG
    qDebug("Stop");
#endif
    bIsRun=false;
    pSearchData->watcher->cancel();
    pSearchData->watcher->waitForFinished();
}

void ThreadSearch::slot_progressRangeChanged(int minimum, int maximum)
{
    qDebug("%d %d",minimum,maximum);
}

int ThreadSearch::appendResult(QString sName, unsigned long long nOffset, unsigned long long nSize)
{
    bool bIsValid=false;

    if(pSearchData->sType=="ANSI")
    {
        if(nSize>=pSearchData->nMinLength)
        {
            bIsValid=true;
        }
    }
    else if(pSearchData->sType=="UNICODE")
    {
        if(nSize>=2*pSearchData->nMinLength)
        {
            bIsValid=true;
        }
    }
    else if(pSearchData->sType=="Crypto")
    {
        bIsValid=true;
    }
    else if(pSearchData->sType=="Links")
    {
        if(sName.contains("http://"))
        {
            sName="http://"+sName.section("http://",1,-1);
            bIsValid=true;
        }
        else if(sName.contains("www."))
        {
            sName="www."+sName.section("www.",1,-1);
            bIsValid=true;
        }

        if(bIsValid)
        {
            bIsValid=sName.length()>=pSearchData->nMinLength;
        }
    }
    else
    {
        bIsValid=true;
    }

    if(bIsValid)
    {
        emit appendRow(sName,pSearchData->nOffset+nOffset,nSize);
        return 1;
    }
    else
    {
        return 0;
    }
}

void ThreadSearch::searchANSI(QFile *pFile)
{
    unsigned long long nOffset=pSearchData->nOffset;
    unsigned long long nSize=pSearchData->nSize;

    char *pOffset=0;
    bool bFound=false;
    unsigned int nFoundSize=0;

    unsigned int nFoundLimit=10000;
    unsigned long long nSearchBuferSize=0x1000;
    unsigned long long nIndex=0;
    unsigned long long nFoundIndex=0;

    char szBuffer[103]= {0};

    int nTempSize=0;

    nIndex=0;
    nFoundSize=0;
    emit progressRangeChanged(0,100);

    for(unsigned long long i=0,j=nSearchBuferSize; (i<nSize)&&(bIsRun); i++,j++)
    {
        if(j>=nSearchBuferSize)
        {
            j=0;

            if(pOffset)
            {
                pFile->unmap((unsigned char *)pOffset);
            }

            nTempSize=qMin(nSearchBuferSize,nSize-i);
            pOffset=(char *)pFile->map(nOffset+i,nTempSize);
        }

        if(0x20<=(*(pOffset+j)))
        {
            bFound=true;

            if(nFoundSize<100)
            {
                szBuffer[nFoundSize]=*(pOffset+j);
            }
            else if(nFoundSize==100)
            {
                szBuffer[100]='.';
                szBuffer[101]='.';
            }

            nFoundSize++;
        }
        else
        {
            if(bFound&&(nFoundSize>4))
            {
                if(nFoundSize<100)
                {
                    szBuffer[nFoundSize]=0;
                }

                nFoundIndex+=appendResult(szBuffer,i-nFoundSize,nFoundSize);

                if(nFoundIndex>nFoundLimit)
                {
                    break;
                }
            }

            bFound=false;
            nFoundSize=0;
            szBuffer[0]=0;
        }

        if(i>(nIndex+1)*(pSearchData->nSize/100))
        {
            emit progressValueChanged(nIndex);

            nIndex++;
        }
    }

    if(bFound&&(nFoundSize>4))
    {
        if(nFoundSize<100)
        {
            szBuffer[nFoundSize]=0;
        }

        nFoundIndex+=appendResult(szBuffer,pSearchData->nSize-nFoundSize,nFoundSize);
    }

    if(pOffset)
    {
        pFile->unmap((unsigned char *)pOffset);
    }

    if(nFoundIndex>nFoundLimit)
    {
        emit foundLimit(nFoundLimit);
    }
}

void ThreadSearch::searchUnicode(QFile *pFile)
{
    unsigned long long nOffset=pSearchData->nOffset;
    unsigned long long nSize=pSearchData->nSize;

    char *pOffset=0;
    bool bFound=false;
    unsigned int nFoundSize=0;

    unsigned int nFoundLimit=10000;
    unsigned long long nSearchBuferSize=0x1000;
    unsigned long long nIndex=0;
    unsigned long long nFoundIndex=0;

    short wzBuffer[103]= {0};
    short wChar;

    int nTempSize=0;

    nIndex=0;
    nFoundSize=0;
    emit progressRangeChanged(0,100);

    for(unsigned long long i=0,j=nSearchBuferSize; (i<nSize-1)&&(bIsRun); i++,j++)
    {
        if(j>=nSearchBuferSize-1)
        {
            j=0;

            if(pOffset)
            {
                pFile->unmap((unsigned char *)pOffset);
            }

            nTempSize=qMin(nSearchBuferSize+1,nSize-i-1);
            pOffset=(char *)pFile->map(nOffset+i,nTempSize);
        }

        wChar=*((short *)(pOffset+j));

        if(pSearchData->nFlags) // Big endian
        {
            wChar=qFromBigEndian(wChar);
        }

        if((0x20<=wChar)&&(wChar<0x80))
        {
            bFound=true;

            if(nFoundSize<100)
            {
                wzBuffer[nFoundSize]=(wchar_t)wChar;
            }
            else if(nFoundSize==100)
            {
                wzBuffer[100]='.';
                wzBuffer[101]='.';
            }

            nFoundSize++;
            i++;
            j++;
        }
        else
        {
            if(bFound&&(nFoundSize>4))
            {
                if(nFoundSize<100)
                {
                    wzBuffer[nFoundSize]=0;
                }

                nFoundIndex+=appendResult(QString::fromUtf16((ushort *)wzBuffer),i-nFoundSize*2,nFoundSize*2);

                if(nFoundIndex>nFoundLimit)
                {
                    break;
                }
            }

            bFound=false;
            nFoundSize=0;
            wzBuffer[0]=0;
        }

        if(i>(nIndex+1)*(pSearchData->nSize/100))
        {
            emit progressValueChanged(nIndex);

            nIndex++;
        }
    }

    if(bFound&&(nFoundSize>4))
    {
        if(nFoundSize<100)
        {
            wzBuffer[nFoundSize]=0;
        }

        nFoundIndex+=appendResult(QString::fromUtf16((ushort *)wzBuffer),pSearchData->nSize-nFoundSize*2,nFoundSize*2);
    }

    if(pOffset)
    {
        pFile->unmap((unsigned char *)pOffset);
    }

    if(nFoundIndex>nFoundLimit)
    {
        emit foundLimit(nFoundLimit);
    }
}

void ThreadSearch::searchCrypto(QFile *pFile)
{
    unsigned int nFoundLimit=10000;
    unsigned long long nFoundIndex=0;
    QList<INPUT_STRUCT> args;
    INPUT_STRUCT input_record;
    char *pOffset=(char *)pFile->map(pSearchData->nOffset,pSearchData->nSize);

    if(pOffset)
    {
        for(int i=0; (i<pSearchData->pCryptoSignatures->count())&&bIsRun; i++)
        {
            if(pSearchData->nFlags) // Big endian
            {
                if(pSearchData->pCryptoSignatures->at(i).sType=="LE")
                {
                    continue;
                }
            }
            else // little endian
            {
                if(pSearchData->pCryptoSignatures->at(i).sType=="BE")
                {
                    continue;
                }
            }

            input_record.cryptorecord=pSearchData->pCryptoSignatures->at(i);
            input_record.pMemory=(unsigned char *)pOffset;
            input_record.nOffset=pSearchData->nOffset;
            input_record.nSize=pSearchData->nSize;

            args.append(input_record);
        }

        if(!args.isEmpty())
        {
            pSearchData->watcher->setFuture(QtConcurrent::mapped(args,findCrypto));
            pSearchData->watcher->waitForFinished();

            if(bIsRun)
            {
                QFuture<QString> future=pSearchData->watcher->future();
                QString sResult;
                int nPartCount=0;
                QString sPart;

                for(int i=0; i<(future.resultCount())&&(bIsRun); i++)
                {
                    sResult=future.resultAt(i);

                    if(sResult!="")
                    {
                        nPartCount=sResult.count(";;");

                        for(int j=0; j<nPartCount; j++)
                        {
                            sPart=sResult.section(";;",j,j);

                            if(sPart!="")
                            {
                                nFoundIndex+=appendResult(sPart.section(";",0,0),sPart.section(";",1,1).toULongLong(),sPart.section(";",2,2).toULongLong());

                                if(nFoundIndex>nFoundLimit)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        pFile->unmap((unsigned char *)pOffset);

        if(nFoundIndex>nFoundLimit)
        {
            emit foundLimit(nFoundLimit);
        }
    }
    else
    {
        emit appendError(tr("Cannot map file"));
    }
}

bool ThreadSearch::isCompleted()
{
    return bIsCompleted;
}

QList<CRYPTO_RECORD> ThreadSearch::loadCryptoSignatures(QString sDatabaseName)
{
    QList<CRYPTO_RECORD> listResult;

    CRYPTO_RECORD record;
    QString line;
    QFile file;

    file.setFileName(sDatabaseName);

    if(file.open(QIODevice::ReadOnly))
    {
#ifdef QT_DEBUG
        qDebug("Load crypto db: %s",sDatabaseName.toLatin1().data());
#endif

        QTextStream in(&file);

        while(!in.atEnd())
        {
            line=in.readLine();

            if(line!="")
            {
                if(!line.contains(QRegExp("^#")))
                {
                    record.sName=line.section(";",0,0);
                    record.sType=line.section(";",1,1);
                    record.sSignature=line.section(";",2,2);

                    listResult.append(record);
                }
            }
        }

        file.close();
    }

    return listResult;
}

void ThreadSearch::process()
{
    if(bIsRun)
    {
        return;
    }

#ifdef QT_DEBUG
    qDebug("Start thread");
#endif
    bIsRun=true;
    bIsCompleted=false;

    unsigned int nFoundLimit=10000;
    unsigned long long nFoundIndex=0;

    //    int j=0;
    QFile file;

    file.setFileName(pSearchData->binary->getFileName());

    if(file.open(QIODevice::ReadOnly))
    {
#ifdef QT_DEBUG
        QTime scanTime=QTime::currentTime();
#endif

        if((pSearchData->sType=="ANSI")||(pSearchData->sType=="Links"))
        {
            searchANSI(&file);
        }

        if((pSearchData->sType=="UNICODE")||(pSearchData->sType=="Links"))
        {
            searchUnicode(&file);
        }

        if(pSearchData->sType=="Crypto")
        {
            searchCrypto(&file);
        }

        //            if(file.unmap((unsigned char *)pOffset))
        //            {
        //#ifdef QT_DEBUG
        //                qDebug("Unmap file");
        //#endif
        //            }
        //        }
        //        else
        //        {
        //#ifdef QT_DEBUG
        //            qDebug("Cannot map file");
        //#endif
        //        }
        //        if(pSearchData->sType=="Executables")
        //        {
        //            nIndex=0;
        //            nFoundSize=0;
        //            emit progressRangeChanged(0,100);

        //            for(int i=0;(i<pSearchData->nSize)&&(bIsRun);i++)
        //            {
        //                if(pSearchData->nSize-i>=sizeof(IMAGE_DOS_HEADER))
        //                {
        //                    wChar=*((short *)(pOffset+i));

        //                    if(wChar==0x5A4D)
        //                    {
        //                        nFoundIndex+=appendResult(QString("MS-DOS(%1)").arg(nFoundIndex),i,sizeof(IMAGE_DOS_HEADER)); // TR!

        //                        if(nFoundIndex>nFoundLimit)
        //                        {
        //                            goto label_finish;
        //                        }
        //                    }
        //                }


        //                if(i>(nIndex+1)*(pSearchData->nSize/100))
        //                {
        //                    emit progressValueChanged(nIndex);

        //                    nIndex++;
        //                }
        //            }
        //        }

#ifdef QT_DEBUG
        qDebug("scan time: %d",scanTime.msecsTo(QTime::currentTime()));
#endif
label_finish:
        file.close();
    }

    if(nFoundIndex>nFoundLimit)
    {
        emit foundLimit(nFoundLimit);
    }

#ifdef QT_DEBUG
    qDebug("Stop thread");
#endif

    emit _finished();

    bIsRun=false;

    bIsCompleted=true;
}
