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
#include "qxhexdumpthread.h"

QXHexDumpThread::QXHexDumpThread(QObject *parent) :
    QObject(parent)
{
}

void QXHexDumpThread::stopDump()
{
    bDump=false;
}

void QXHexDumpThread::startDump()
{
    bDump=true;

    unsigned long long nOffset=pDataBlock->nDumpAddress;
    unsigned long long nSize=pDataBlock->nDumpSize;
    QString sDumpFileName=pDataBlock->sDumpFileName;
    QByteArray baTemp;
    bool bTemp;

    emit progressRangeChanged(0,nSize/(pDataBlock->nReadBlockMax*100));

    QFile file;
    int nTemp;

    file.setFileName(sDumpFileName);

    if(file.open(QIODevice::ReadWrite))
    {
        file.resize(0);

        int j=0;

        while(nSize&&bDump)
        {
            nTemp=qMin((unsigned long long)pDataBlock->nReadBlockMax,nSize);
            baTemp=pDataBlock->__ReadMemory(pDataBlock->parameter,nOffset,nTemp,&bTemp);

            if(bDump)
            {
                file.write(baTemp.data(),baTemp.size());
            }


            nOffset+=nTemp;
            nSize-=nTemp;

            if(j%100==0)
            {
                emit progressValueChanged(j/100);
            }

            j++;
        }


        file.close();

        if(bDump)
        {
            emit _information("Dump saved to file: "+sDumpFileName);
        }


    }
    else
    {
        emit _error("Cannot save to file: "+sDumpFileName);
    }


    bDump=false;
    emit _dumpComplete();
}

void QXHexDumpThread::setData(__DATABLOCK *pDataBlock)
{
    this->pDataBlock=pDataBlock;
}
