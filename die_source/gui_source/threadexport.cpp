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
#include "threadexport.h"

ThreadExport::ThreadExport(QObject *parent) :
    QObject(parent)
{
    bIsRun=false;
    bIsCompleted=true;
}

void ThreadExport::setData(PEFile *pefile)
{
    this->pefile=pefile;
}

void ThreadExport::_stop()
{
    bIsRun=false;
}

bool ThreadExport::isCompleted()
{
    return bIsCompleted;
}

void ThreadExport::process()
{
    if(bIsRun)
    {
        return;
    }

    bIsRun=true;
    bIsCompleted=false;

    QString sOrdinal;
    QString sAddress;
    QString sName;

    int nNumberOfFunctions=pefile->getExport_NumberOfFunctions();
    int nNumberOfNames=pefile->getExport_NumberOfNames();

    if(nNumberOfFunctions>0xFFFF)
    {
        nNumberOfFunctions=0;
    }

    unsigned int nAddressOfNameOrdinals=pefile->RVAToOffset(pefile->getExport_AddressOfNameOrdinals());
    unsigned int nAddressOfFunctions=pefile->RVAToOffset(pefile->getExport_AddressOfFunctions());
    unsigned int nAddressOfNames=pefile->RVAToOffset(pefile->getExport_AddressOfNames());
    unsigned int nExport_Base=pefile->getExport_Base();
    unsigned int nOrdinal=0;

    int k=0;

    for(int i=0; (i<nNumberOfFunctions)&&(bIsRun); i++)
    {
        if(!bIsRun)
        {
            break;
        }

        nOrdinal=pefile->getExport_FunctionNameOrdinal(i,nAddressOfNameOrdinals,nExport_Base);
        sOrdinal=QString("%1").arg(nOrdinal,4,16,QChar('0'));

        if(!bIsRun)
        {
            break;
        }

        sAddress=QString("%1").arg(pefile->getExport_FunctionAddress(nOrdinal-nExport_Base,nAddressOfFunctions),8,16,QChar('0'));

        if(!bIsRun)
        {
            break;
        }

        if(i<nNumberOfNames)
        {
            sName=pefile->getExport_FunctionNameAsString(i,nAddressOfNames);
        }
        else
        {
            sName="";
        }

        if(!bIsRun)
        {
            break;
        }

        //        qDebug("%d: %s %s %s",sOrdinal.toAscii().data(),sAddress.toAscii().data(),sName.toAscii().data());

        emit appendRow(i,sOrdinal,sAddress,sName);

        if(i+1>(nNumberOfFunctions/30)*k)
        {
            emit setProgressBar(nNumberOfFunctions,i+1);
            k++;
        }
    }

    emit setProgressBar(1,1);
    emit _finished();

    bIsRun=false;

    //    msleep(1000);

    bIsCompleted=true;
}
