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
#include "threadentropy.h"

ThreadEntropy::ThreadEntropy(QObject *parent) :
    QObject(parent)
{
    bIsRun=false;
    bIsCompleted=true;
}

void ThreadEntropy::setData(Binary *_binary,unsigned int nOffset,unsigned int nSize,double *pXEntropy,double *pYEntropy,double *pXHistogram,double *pYHistogram,int nPunkts)
{
    this->_binary=_binary;
    this->nOffset=nOffset;
    this->nSize=nSize;
    this->pXEntropy=pXEntropy;
    this->pYEntropy=pYEntropy;
    this->pXHistogram=pXHistogram;
    this->pYHistogram=pYHistogram;
    this->nPunkts=nPunkts;
}

void ThreadEntropy::_stop()
{
    bIsRun=false;
}

bool ThreadEntropy::isCompleted()
{
    return bIsCompleted;
}
double ThreadEntropy::getEntropy(unsigned int nDataOffset,unsigned int nDataSize,bool bProgressBar)
{
    if(nDataSize==0)
    {
        return 0;
    }

    double dEntropy=1.4426950408889634073599246810023;

    QByteArray baTemp;
    baTemp.resize(BUFFER_SIZE);

    unsigned int _nSize=nDataSize;

    double bytes[256]= {0.0};
    double temp;

    unsigned int nTemp=0;

    unsigned int nCount=nDataSize/BUFFER_SIZE;
    unsigned int _nOffset=nDataOffset;

    int k=0;

    for(unsigned int i=0; (i<nCount+1)&&(bIsRun); i++)
    {
        nTemp=MINIMAL(BUFFER_SIZE,_nSize);

        if(!_binary->readArrayFromFile(_nOffset,baTemp.data(),nTemp))
        {
            bIsRun=false;
        }

        for(unsigned int m=0; m<nTemp; m++)
        {
            bytes[(unsigned char)baTemp.data()[m]]+=1;
        }

        _nSize-=nTemp;
        _nOffset+=nTemp;

        if(bProgressBar)
        {
            if(i+1>(nCount/30)*k)
            {
                emit setProgressBar(nCount,i+1);
                k++;
            }
        }
    }

    for(int j=0; (j<256)&&(bIsRun); j++)
    {
        temp=bytes[j]/(double)nDataSize;

        if(temp)
        {
            dEntropy+=(-log(temp)/log((double)2))*bytes[j];
        }
    }

    dEntropy=dEntropy/(double)nDataSize;

    return dEntropy;
}

void ThreadEntropy::getHistogram(unsigned int nDataOffset, unsigned int nDataSize, double *pX,bool bProgressBar)
{
    QByteArray baTemp;
    baTemp.resize(BUFFER_SIZE);

    unsigned int _nSize=nDataSize;

    unsigned int nTemp=0;

    unsigned int nCount=nDataSize/BUFFER_SIZE;
    unsigned int _nOffset=nDataOffset;

    int k=0;

    for(unsigned int i=0; (i<nCount+1)&&(bIsRun); i++)
    {
        nTemp=MINIMAL(BUFFER_SIZE,_nSize);

        if(!_binary->readArrayFromFile(_nOffset,baTemp.data(),nTemp))
        {
            bIsRun=false;
        }

        for(unsigned int m=0; m<nTemp; m++)
        {
            pX[(unsigned char)baTemp.data()[m]]+=1;
        }

        _nSize-=nTemp;
        _nOffset+=nTemp;

        if(bProgressBar)
        {
            if(i+1>(nCount/30)*k)
            {
                emit setProgressBar(nCount,i+1);
                k++;
            }
        }
    }
}

void ThreadEntropy::process()
{
    if(bIsRun)
    {
        return;
    }

    bIsRun=true;
    bIsCompleted=false;

    getHistogram(nOffset,nSize,pYHistogram);

    double dEntropy=getEntropy(nOffset,nSize);

    // new>>
    pYEntropy[nPunkts]=dEntropy;
    //<<

    emit setEntropy(dEntropy);

    unsigned int nProcent=nSize/nPunkts;

    if(nProcent)
    {
        for(int i=0; (i<nPunkts)&&(bIsRun); i++)
        {
            pXEntropy[i]=i*nProcent+nOffset;
            pYEntropy[i]=dEntropy;

            //            qDebug("%d x=%f y=%f",i,pXEntropy[i],pYEntropy[i]);
        }
    }
    else
    {
        for(int i=0; (i<nPunkts)&&(bIsRun); i++)
        {
            pXEntropy[i]=0;
            pYEntropy[i]=dEntropy;
        }

        pXEntropy[0]=nOffset;
        pYEntropy[0]=dEntropy;
        pXEntropy[1]=nOffset+nSize;
        pYEntropy[1]=dEntropy;
    }

    emit reloadGraph();

    if(nProcent)
    {
        int k=0;

        for(int j=0; (j<nPunkts-nPunkts/10)&&(bIsRun); j++)
        {
            dEntropy=getEntropy(nOffset+j*nProcent,nProcent*(nPunkts/10),false);
            pYEntropy[j]=dEntropy;

            if(j+1>(100/30)*k)
            {
                emit setProgressBar(nPunkts,j+1);
                k++;
            }

            emit reloadGraph();
        }
    }

    emit setProgressBar(1,1);

    emit _finished();

    bIsRun=false;

    //    msleep(1000);

    bIsCompleted=true;
}
