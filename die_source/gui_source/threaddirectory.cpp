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
#include "threaddirectory.h"

ThreadDirectory::ThreadDirectory(QObject *parent) :
    QObject(parent)
{
    bIsRun=false;
    bIsCompleted=true;
    nTotal=0;
    //    filter.append("*.exe");
    //    filter.append("*.dll");
    //    filter.append("*.sys");
    //    filter.append("*.bpl");
}

void ThreadDirectory::setData(QString sDirectory, bool bSubDirectory,QList<QString> *pList)
{
    this->sDirectory=sDirectory;
    this->bSubDirectory=bSubDirectory;
    this->pList=pList;
}

void ThreadDirectory::_stop()
{
    bIsRun=false;
}

bool ThreadDirectory::isCompleted()
{
    return bIsCompleted;
}

void ThreadDirectory::findFiles(QString sDirectoryName)
{
    if(!bIsRun)
    {
        return;
    }

    QDir dir;
    dir.setPath(sDirectoryName);

    QFileInfoList fi=dir.entryInfoList();

    for(int i=0; (i<fi.count())&&(bIsRun); i++)
    {
        if(fi.at(i).fileName()==".")
        {
            continue;
        }

        if(fi.at(i).fileName()=="..")
        {
            continue;
        }
        else if(fi.at(i).isFile())
        {
            pList->append(fi.at(i).absoluteFilePath());
            nTotal++;
        }

        if(fi.at(i).isDir()&&(bSubDirectory))
        {
            findFiles(fi.at(i).absoluteFilePath());
        }
    }

    emit setTotal(nTotal);
}

void ThreadDirectory::process()
{
    if(bIsRun)
    {
        return;
    }

    nTotal=0;

    bIsRun=true;
    bIsCompleted=false;

    findFiles(sDirectory);

    if(bIsRun)
    {
        emit _finished();
    }

    bIsRun=false;
    bIsCompleted=true;
}
