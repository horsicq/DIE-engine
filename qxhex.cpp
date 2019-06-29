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
#include "qxhex.h"

QString ConvertToAnsi(QByteArray *pbaData)
{
    QString sTemp;
    int nDataSize=pbaData->size();

    char *pBuffer=new char[nDataSize+1];

    //    sTemp.resize(pbaData->size());
    for(int i=0; i<nDataSize; i++)
    {
        if((32<pbaData->at(i))&&(pbaData->at(i)<127))
            //        if(32<(unsigned char)(pbaData->at(i)))
        {
            pBuffer[i]=(pbaData->at(i));
        }
        else
        {
            pBuffer[i]=('.');
        }
    }

    pBuffer[nDataSize]=0;

    sTemp.append(pBuffer);

    delete[] pBuffer;

    return sTemp;
}
QString ConvertToHex(QByteArray *pbaData)
{
    QString sTemp;
    unsigned char cTemp;
    char *pData=pbaData->data();
    int nDataSize=pbaData->size();

    char *pBuffer=new char[nDataSize*2+1];

    int x=0;

    for(int y=0; y<nDataSize; ++y, ++x)
    {

        cTemp=((unsigned char)pData[y])>>4;

        pBuffer[x]=(char)(cTemp>9 ? cTemp+0x37 : cTemp+0x30);

        cTemp=((unsigned char)(pData[y]&0xF));

        pBuffer[++x]=(char)(cTemp>9 ? cTemp+0x37 : cTemp+0x30);
        //        pBuffer[++x]=' ';
    }


    pBuffer[x]=0;

    sTemp.append(pBuffer);

    delete[] pBuffer;



    return sTemp;
}

QString ByteToHex(unsigned char cByte)
{
    QString sTemp;


    char *pBuffer=new char[2+1];
    unsigned char cTemp;

    cTemp=cByte>>4;
    pBuffer[0]=(char)(cTemp>9 ? cTemp+0x37 : cTemp+0x30);

    cTemp=((unsigned char)(cByte&0xF));
    pBuffer[1]=(char)(cTemp>9 ? cTemp+0x37 : cTemp+0x30);

    pBuffer[2]=0;

    sTemp.append(pBuffer);

    delete[] pBuffer;

    return sTemp;
}

QString ByteToAnsi(unsigned char cByte)
{
    QString sTemp;

    if((32<cByte)&&(cByte<127))
    {
        sTemp.append(cByte);
    }
    else
    {
        sTemp.append('.');
    }

    return sTemp;
}
