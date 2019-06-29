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
#include "threadimport.h"

ThreadImport::ThreadImport(QObject *parent) :
    QObject(parent)
{
    bIsRun=false;
    bIsCompleted=true;
}

void ThreadImport::setData(PEFile *pefile, QList<IMPORT_LIBRARY> *pListLibraries)
{
    this->pefile=pefile;
    this->pListLibraries=pListLibraries;
}
void ThreadImport::_stop()
{
    bIsRun=false;
}

bool ThreadImport::isCompleted()
{
    return bIsCompleted;
}

bool ThreadImport::isRun()
{
    return bIsRun;
}

void ThreadImport::process()
{
    if(bIsRun)
    {
        return;
    }

    bIsRun=true;
    bIsCompleted=false;

    emit _started();

    pListLibraries->clear();

    int nImports=pefile->getNumberOfImports();
    unsigned int nImportTableOffset=pefile->getImportTableOffset();

    for(int j=0; j<nImports; j++)
    {
        unsigned int nIndex=j;

        IMPORT_LIBRARY impLibrary;

        impLibrary.sName=pefile->getImport_NameAsString(j,nImportTableOffset);
        impLibrary.nOriginalFirstThunk=pefile->getImport_OriginalFirstThunk(j,nImportTableOffset);
        impLibrary.nTimeDateStamp=pefile->getImport_TimeDateStamp(j,nImportTableOffset);
        impLibrary.nForwarderChain=pefile->getImport_ForwarderChain(j,nImportTableOffset);
        impLibrary.nName=pefile->getImport_Name(j,nImportTableOffset);
        impLibrary.nFirstThunk=pefile->getImport_FirstThunk(j,nImportTableOffset);

        int nNumberOfImportThunks=pefile->getNumberOfImportThunks(nIndex);

        if(nNumberOfImportThunks>0xFFFF)
        {
            nNumberOfImportThunks=0;
        }

        unsigned int nOriginalFirstThunk=pefile->RVAToOffset(pefile->getImport_OriginalFirstThunk(nIndex));
        unsigned int nFirstThunk=pefile->RVAToOffset(pefile->getImport_FirstThunk(nIndex));

        QString sThunk;
        QString sOrdinal;
        QString sHint;
        QString sName;

        for(int i=0; (i<nNumberOfImportThunks)&&(bIsRun); i++)
        {
            IMPORT_FUNCTION impFunction;

            if(pefile->isPEPlus())
            {
                unsigned long long nThunk64;

                nThunk64=pefile->getImportThunk64(nOriginalFirstThunk,nFirstThunk,i);

                sThunk=QString("%1").arg(nThunk64,16,16,QChar('0'));

                if(nThunk64&0x8000000000000000)
                {
                    sOrdinal=QString("%1").arg(nThunk64&0x7FFFFFFFFFFFFFFF,16,16,QChar('0'));
                    sHint="";
                    sName="";
                }
                else
                {
                    sOrdinal="";
                    sHint=QString("%1").arg(pefile->getImportHint(nThunk64),4,16,QChar('0'));
                    sName=pefile->getImportFunctionName(nThunk64);
                }
            }
            else
            {
                unsigned int nThunk;

                nThunk=pefile->getImportThunk(nOriginalFirstThunk,nFirstThunk,i);;
                sThunk=QString("%1").arg(nThunk,8,16,QChar('0'));

                if(nThunk&0x80000000)
                {

                    sOrdinal=QString("%1").arg(nThunk&0x7FFFFFFF,8,16,QChar('0'));
                    sHint="";
                    sName="";
                }
                else
                {
                    sOrdinal="";
                    sHint=QString("%1").arg(pefile->getImportHint(nThunk),4,16,QChar('0'));
                    sName=pefile->getImportFunctionName(nThunk);
                }
            }

            impFunction.sName=sName;
            impFunction.sHint=sHint;
            impFunction.sOrdinal=sOrdinal;
            impFunction.sThunk=sThunk;

            impLibrary.listFunctions.append(impFunction);
        }

        emit setProgressBar(nNumberOfImportThunks,j);
        pListLibraries->append(impLibrary);
    }


    emit setProgressBar(1,1);

    emit _finished();

    bIsRun=false;

    //    msleep(1000);

    bIsCompleted=true;
}

