#include "binary.h"

Binary::Binary(QObject *parent) :
    QObject(parent)
{
    bIsReadOnly=false;
    //    nEntryPointSize=0;
    bIsEntryPontLoad=false;
    nEntryPointOffset=0;

    ___nSize=0;

    ___pMemory=0;

    nIsReverse=-1; // unknown
    //    _clearMemory();
}

Binary::~Binary()
{
    file.unmap(___pMemory);
    file.close();
}
bool Binary::reopen()
{
    if(___pMemory)
    {
        file.unmap(___pMemory);
        file.close();
    }

    file.setFileName(sFileName);

    if(!file.exists())
    {
        emit appendError(QString("Cannot open file: %1").arg(sFileName));
        return false;
    }

    bIsReadOnly=true;

    //    memorylist.clear();
    //    baHeader.clear();

    //    _clearMemory();
    if(file.open(QIODevice::ReadWrite))
    {
        bIsReadOnly=false;
        ___nSize=file.size();
        ___pMemory=file.map(0,___nSize);

        if(bReadHeader)
        {
            unsigned int nTemp=file.size();

            if(nTemp>BUFFER_SIZE)
            {
                nTemp=BUFFER_SIZE;
            }

            //            _memorylist[0].nOffset=0;
            //            _memorylist[0].baData=file.read(nTemp);
        }

        emit isReadOnly(bIsReadOnly);
        return true;
    }
    else
    {
        if(file.open(QIODevice::ReadOnly))
        {

            ___nSize=file.size();
            ___pMemory=file.map(0,___nSize);

            if(bReadHeader)
            {
                unsigned int nTemp=file.size();

                if(nTemp>0x2000)
                {
                    nTemp=0x2000;
                }

                //                _memorylist[0].nOffset=0;
                //                _memorylist[0].baData=file.read(nTemp);
            }

            emit isReadOnly(bIsReadOnly);

            return true;
        }
    }

    emit isReadOnly(bIsReadOnly);
    emit appendError(QString("Cannot open file: %1").arg(sFileName));

    return false;
}

bool Binary::isReadOnly()
{
    return bIsReadOnly;
}

bool Binary::resize(unsigned long long nNewSize)
{
    file.unmap(___pMemory);

    if(file.resize(nNewSize))
    {

        ___nSize=nNewSize;
        ___pMemory=file.map(0,___nSize);

        return true;
    }

    ___pMemory=file.map(0,___nSize);
    return false;
}

bool Binary::setFileName(QString sFileName,bool bReadHeader)
{
    this->sFileName=sFileName;
    this->bReadHeader=bReadHeader;

    return reopen();
}

QString Binary::getFileName()
{
    return sFileName;
}


unsigned long long Binary::size()
{
    //    return file.size();
    return ___nSize;
}

QString Binary::getFileBaseName()
{
    QFileInfo fi;

    fi.setFile(getFileName());

    return fi.baseName();
}

QString Binary::getFileSuffix()
{
    QFileInfo fi;

    fi.setFile(getFileName());

    return fi.suffix();
}

QString Binary::getFileCompleteSuffix()
{
    QFileInfo fi;

    fi.setFile(getFileName());

    return fi.completeSuffix();
}

unsigned char Binary::readByte(unsigned int nOffset)
{
    //    if(isArrayInMemory(nOffset,1))
    //    {
    //        return readByteFromMemory(nOffset);
    //    }
    //    else
    //    {
    //        return readByteFromFile(nOffset);
    //    }
    unsigned char cValue=0;

    readArray(nOffset,(char *)&cValue,1);

    return cValue;
}

unsigned short Binary::readWord(unsigned int nOffset,bool bReverse)
{
    unsigned short sValue=0;

    readArray(nOffset,(char *)&sValue,2);
    //    if(isArrayInMemory(nOffset,2))
    //    {
    //        sValue=readWordFromMemory(nOffset);
    //    }
    //    else
    //    {
    //        sValue=readWordFromFile(nOffset);
    //    }

    if(bReverse)
    {
        sValue=swapWord(sValue);
    }

    return sValue;
}

unsigned int Binary::readDword(unsigned int nOffset,bool bReverse)
{
    unsigned int nValue=0;

    readArray(nOffset,(char *)&nValue,4);

    //    if(isArrayInMemory(nOffset,4))
    //    {
    //        nValue=readDwordFromMemory(nOffset);
    //    }
    //    else
    //    {
    //        readArray(nOffset,(char *)&nValue,4);
    ////        nValue=readDwordFromFile(nOffset);
    //    }

    if(bReverse)
    {
        nValue=swapDword(nValue);
    }

    return nValue;
}

unsigned long long Binary::readQword(unsigned int nOffset,bool bReverse)
{
    unsigned long long qValue=0;

    readArray(nOffset,(char *)&qValue,8);
    //    if(isArrayInMemory(nOffset,8))
    //    {
    //        qResult=readQwordFromMemory(nOffset);
    //    }
    //    else
    //    {
    //        qResult=readQwordFromFile(nOffset);
    //    }

    if(bReverse)
    {
        qValue=swapQword(qValue);
    }

    return qValue;
}

unsigned char Binary::readByteFromFile(unsigned int nOffset)
{
    unsigned char cResult;

    if(readArrayFromFile(nOffset,(char *)&cResult,1))
    {
        return cResult;
    }

    return 0;
}

unsigned short Binary::readWordFromFile(unsigned int nOffset)
{
    unsigned short sResult;

    if(readArrayFromFile(nOffset,(char *)&sResult,2))
    {
        return sResult;
    }

    return 0;
}

unsigned int Binary::readDwordFromFile(unsigned int nOffset)
{
    unsigned int nResult;

    if(readArrayFromFile(nOffset,(char *)&nResult,4))
    {
        return nResult;
    }

    return 0;
}

unsigned long long Binary::readQwordFromFile(unsigned int nOffset)
{
    unsigned long long qResult;

    if(readArrayFromFile(nOffset,(char *)&qResult,8))
    {
        return qResult;
    }

    return 0;
}

//unsigned char Binary::readByteFromMemory(unsigned int nOffset)
//{
//    unsigned char cValue;

//    readArrayFromMemory(nOffset,(char *)&cValue,1);

//    return cValue;
//}

//unsigned short Binary::readWordFromMemory(unsigned int nOffset)
//{
//    unsigned short sValue;

//    readArrayFromMemory(nOffset,(char *)&sValue,2);

//    return sValue;
//}

//unsigned int Binary::readDwordFromMemory(unsigned int nOffset)
//{
//    unsigned int nValue;

//    readArrayFromMemory(nOffset,(char *)&nValue,4);

//    return nValue;
//}

//unsigned long long Binary::readQwordFromMemory(unsigned int nOffset)
//{
//    unsigned long long qValue;

//    readArrayFromMemory(nOffset,(char *)&qValue,8);

//    return qValue;
//}
//bool Binary::isArrayInMemory(unsigned int nOffset,unsigned int nSize)
//{
////    if((unsigned int)baHeader.size()>=nOffset+nSize)
////    {
////        return true;
////    }
////    else
////    {
////        for(int i=0;i<memorylist.count();i++)
////        {
////            if(memorylist.at(i).nOffset<=nOffset)
////            {
////                if(nOffset+nSize<=memorylist.at(i).nOffset+memorylist.at(i).nSize)
////                {
////                    return true;
////                }
////            }
////        }
////    }

//    for(int i=0;i<MEMORYCOUNT;i++)
//    {
//        if((nOffset>=_memorylist[i].nOffset)&&(nOffset+nSize<=_memorylist[i].nOffset+_memorylist[i].baData.size()))
//        {
//            return true;
//        }
//    }

//    return false;
//}

bool Binary::readArrayFromFile(unsigned int nOffset, char *pBuffer, unsigned int nSize)
{
    //    emit appendError(QString("offset=%1").arg(nOffset,0,16));

    if(nOffset+nSize>size())
    {
        emit appendError(QString("Read from file failed: (offset=0x%1 size=0x%2)").arg(nOffset,8,16,QChar('0')).arg(nSize,0,16,QChar('0')));
        return false;
    }


    file.seek(nOffset);

    QByteArray baTemp;

    baTemp=file.read(nSize);

    if(baTemp.size()==nSize)
    {
        memcpy(pBuffer,baTemp.data(),nSize);
        return true;
    }

    emit appendError(QString("Read from file failed: (offset=0x%1 size=0x%2)").arg(nOffset,8,16,QChar('0')).arg(nSize,0,16,QChar('0')));

    return false;
}

bool Binary::writeArrayToFile(unsigned int nOffset, char *pBuffer, unsigned int nSize)
{
    if(bIsReadOnly)
    {
        emit appendError("Cannot write(read-only mode)");
        return false;
    }

    if(!isOffsetAndSizeValid(nOffset,nSize))
    {
        return false;
    }

    //    if(nOffset+nSize>size())
    //    {
    //        emit appendError(QString("Write to file failed: (offset=0x%1 size=0x%2)").arg(nOffset,8,16,QChar('0')).arg(nSize,0,16,QChar('0')));
    //        return false;
    //    }

    file.seek(nOffset);

    if(file.write(pBuffer,nSize)==nSize)
    {
        remap();
        return true;
    }

    emit appendError(QString("Write to file failed: (offset=0x%1 size=0x%2)").arg(nOffset,8,16,QChar('0')).arg(nSize,0,16,QChar('0')));

    return false;
}

bool Binary::readArray(unsigned int nOffset, char *pBuffer, unsigned int nSize)
{
    //    bool  bResult;


    if(!isOffsetAndSizeValid(nOffset,nSize))
    {
        return false;
    }

    if(___pMemory)
    {
        memcpy(pBuffer,___pMemory+nOffset,nSize);
    }
    else
    {
        readArrayFromFile(nOffset,pBuffer,nSize);
    }



    //    if(isArrayInMemory(nOffset,nSize))
    //    {
    //        bResult=readArrayFromMemory(nOffset,pBuffer,nSize);
    //    }
    ////    else
    ////    {
    ////        bResult=readArrayFromFile(nOffset,pBuffer,nSize);
    ////    }
    //    else if(nSize>BUFFER_SIZE)
    //    {
    //        bResult=readArrayFromFile(nOffset,pBuffer,nSize);
    //    }
    //    else
    //    {
    //        // add memory
    //        unsigned int _nOffset=ALIGN_DOWN(nOffset,BUFFER_SIZE);
    //        unsigned int _nSize=ALIGN_UP(nOffset+nSize,BUFFER_SIZE)-_nOffset;

    //        if(_nOffset+_nSize>size())
    //        {
    //            _nSize=size()-_nOffset;
    //        }

    //        addMemory(_nOffset,_nSize);

    //        bResult=readArrayFromMemory(nOffset,pBuffer,nSize);
    //    }

    return true;
}

QByteArray Binary::readArray(unsigned int nOffset, unsigned int nSize)
{
    QByteArray baResult;
    unsigned int nFileSize=size();

    if(nOffset<nFileSize)
    {
        if((unsigned long long)nOffset+(unsigned long long)nSize>(unsigned long long)nFileSize)
        {
            nSize=nFileSize-nOffset;
        }

        baResult.resize(nSize);

        readArray(nOffset,baResult.data(),nSize);
    }

    return baResult;

}

bool Binary::writeArray(unsigned int nOffset, char *pBuffer, unsigned int nSize)
{
    return writeArrayToFile(nOffset,pBuffer,nSize);

    //    if(isArrayInMemory(nOffset,nSize))
    //    {
    //        return writeArrayToMemory(nOffset,pBuffer,nSize);
    //    }
    //    else if(nSize>BUFFER_SIZE)
    //    {
    //        return writeArrayToFile(nOffset,pBuffer,nSize);
    //    }
    //    else
    //    {
    //        // add memory
    //        unsigned int _nOffset=ALIGN_DOWN(nOffset,BUFFER_SIZE);
    //        unsigned int _nSize=ALIGN_UP(nOffset+nSize,BUFFER_SIZE)-_nOffset;

    //        addMemory(_nOffset,_nSize);

    ////        return writeArrayToFile(nOffset,pBuffer,nSize);
    //        return writeArrayToMemory(nOffset,pBuffer,nSize);
    //    }
}

//bool Binary::readArrayFromMemory(unsigned int nOffset, char *pBuffer, unsigned int nSize)
//{
////    if((unsigned int)baHeader.size()>=nOffset+nSize)
////    {
////        memcpy(pBuffer,baHeader.data()+nOffset,nSize);

////        return true;
////    }
////    else
////    {
////        for(int i=0;i<memorylist.count();i++)
////        {
////            if(memorylist.at(i).nOffset<=nOffset)
////            {
////                if(nOffset+nSize<=memorylist.at(i).nOffset+memorylist.at(i).nSize)
////                {
////                    memcpy(pBuffer,memorylist.at(i).baData.data()+nOffset-memorylist.at(i).nOffset,nSize);
////                    return true;
////                }
////            }
////        }
////    }

////    return false;
//    for(int i=0;i<MEMORYCOUNT;i++)
//    {
//        if((nOffset>=_memorylist[i].nOffset)&&(nOffset+nSize<=_memorylist[i].nOffset+_memorylist[i].baData.size()))
//        {
//            memcpy(pBuffer,_memorylist[i].baData.data()+nOffset-_memorylist[i].nOffset,nSize);
//            return true;
//        }
//    }

//    return false;
//}

//bool Binary::writeArrayToMemory(unsigned int nOffset, char *pBuffer, unsigned int nSize)
//{

//    for(int i=0;i<MEMORYCOUNT;i++)
//    {
//        if((nOffset>=_memorylist[i].nOffset)&&(nOffset+nSize<=_memorylist[i].nOffset+_memorylist[i].baData.size()))
//        {
//            memcpy(_memorylist[i].baData.data()+nOffset-_memorylist[i].nOffset,pBuffer,nSize);

//            _memorylist[i].bIsChanged=true;

//            updateFile();
//            return true;
//        }
//    }

//    return false;
////    if((unsigned int)baHeader.size()>=nOffset+nSize)
////    {
////        memcpy(baHeader.data()+nOffset,pBuffer,nSize);
////        updateFile();

////        return true;
////    }
////    else
////    {
////        for(int i=0;i<memorylist.count();i++)
////        {
////            if(memorylist.at(i).nOffset<=nOffset)
////            {
////                if(nOffset+nSize<=memorylist.at(i).nOffset+memorylist.at(i).nSize)
////                {
////                    memcpy((char *)memorylist.at(i).baData.data()+nOffset-memorylist.at(i).nOffset,pBuffer,nSize);
////                    updateFile();

////                    return true;
////                }
////            }
////        }
////    }

////    return false;



////    updateFile();

//    //    return true;
//}

QByteArray Binary::readArrayFromEntryPoint(unsigned int nSize)
{
    return readArray(nEntryPointOffset,nSize);
}

//void Binary::updateFile()
//{
//    for(int i=0;i<MEMORYCOUNT;i++)
//    {
//        if(_memorylist[i].bIsChanged)
//        {
//            if(_memorylist[i].baData.size())
//            {
//                writeArrayToFile(_memorylist[i].nOffset,_memorylist[i].baData.data(),_memorylist[i].baData.size());
//            }

//            _memorylist[i].bIsChanged=false;
//        }

//    }


////    writeArrayToFile(0,baHeader.data(),baHeader.size());

////    for(int i=0;i<memorylist.count();i++)
////    {
////        writeArrayToFile(memorylist.at(i).nOffset,(char *)memorylist.at(i).baData.data(),memorylist.at(i).nSize);
//    //    }
//}

void Binary::remap()
{
    reopen();
    //    file.unmap(___pMemory);
    //    ___pMemory=file.map(0,___nSize);
}
void Binary::writeByte(unsigned int nOffset, unsigned char cValue)
{
    //    if(!bIsReadOnly)
    //    {
    //        if(isArrayInMemory(nOffset,1))
    //        {
    //            writeByteToMemory(nOffset,cValue);
    //        }
    //        else
    //        {
    //            writeByteToFile(nOffset,cValue);
    //        }

    //        return;
    //    }
    //    emit appendError("Cannot write(read-only mode)");

    writeArray(nOffset,(char *)&cValue,1);

}

void Binary::writeWord(unsigned int nOffset, unsigned short sValue,bool bReverse)
{

    if(bReverse)
    {
        sValue=swapWord(sValue);
    }

    //    QString s=QString("offset=%1 size=").arg(nOffset);

    //    qDebug(s.toAscii().data());

    //    if(!bIsReadOnly)
    //    {
    //        if(isArrayInMemory(nOffset,2))
    //        {
    //            writeWordToMemory(nOffset,sValue);
    //        }
    //        else
    //        {
    //            writeWordToFile(nOffset,sValue);
    //        }

    //        return;
    //    }
    //    emit appendError("Cannot write(read-only mode)");

    writeArray(nOffset,(char *)&sValue,2);

}

void Binary::writeDword(unsigned int nOffset, unsigned int nValue,bool bReverse)
{

    if(bReverse)
    {
        nValue=swapDword(nValue);
    }

    //    if(!bIsReadOnly)
    //    {
    //        if(isArrayInMemory(nOffset,4))
    //        {
    //            //emit appendError("In header");
    //            writeDwordToMemory(nOffset,nValue);
    //        }
    //        else
    //        {
    //            //emit appendError("In file");
    //            writeDwordToFile(nOffset,nValue);
    //        }

    //        return;
    //    }
    //    emit appendError("Cannot write(read-only mode)");

    writeArray(nOffset,(char *)&nValue,4);

}

void Binary::writeQword(unsigned int nOffset, unsigned long long qValue,bool bReverse)
{
    if(bReverse)
    {
        qValue=swapQword(qValue);
    }

    //    if(!bIsReadOnly)
    //    {
    //        if(isArrayInMemory(nOffset,8))
    //        {
    //            writeQwordToMemory(nOffset,qValue);
    //        }
    //        else
    //        {
    //            writeQwordToFile(nOffset,qValue);
    //        }

    //        return;
    //    }
    //    emit appendError("Cannot write(read-only mode)");
    writeArray(nOffset,(char *)&qValue,8);
}

unsigned short Binary::swapWord(unsigned short sValue)
{
    unsigned short sResult;

    ((char *)(&sResult))[0]=((char *)(&sValue))[1];
    ((char *)(&sResult))[1]=((char *)(&sValue))[0];

    return sResult;
}

unsigned int Binary::swapDword(unsigned int nValue)
{
    unsigned int nResult;

    ((char *)(&nResult))[3]=((char *)(&nValue))[0];
    ((char *)(&nResult))[2]=((char *)(&nValue))[1];
    ((char *)(&nResult))[1]=((char *)(&nValue))[2];
    ((char *)(&nResult))[0]=((char *)(&nValue))[3];

    return nResult;
}

unsigned int Binary::swapQword(unsigned long long qValue)
{
    unsigned long long qResult;

    ((char *)(&qResult))[7]=((char *)(&qValue))[0];
    ((char *)(&qResult))[6]=((char *)(&qValue))[1];
    ((char *)(&qResult))[5]=((char *)(&qValue))[2];
    ((char *)(&qResult))[4]=((char *)(&qValue))[3];
    ((char *)(&qResult))[3]=((char *)(&qValue))[4];
    ((char *)(&qResult))[2]=((char *)(&qValue))[5];
    ((char *)(&qResult))[1]=((char *)(&qValue))[6];
    ((char *)(&qResult))[0]=((char *)(&qValue))[7];

    return qResult;
}

void Binary::writeByteToFile(unsigned int nOffset, unsigned char cValue)
{
    writeArrayToFile(nOffset,(char *)&cValue,1);
}

void Binary::writeWordToFile(unsigned int nOffset, unsigned short sValue)
{
    writeArrayToFile(nOffset,(char *)&sValue,2);
}

void Binary::writeDwordToFile(unsigned int nOffset, unsigned int nValue)
{
    writeArrayToFile(nOffset,(char *)&nValue,4);
}

void Binary::writeQwordToFile(unsigned int nOffset, unsigned long long qValue)
{
    writeArrayToFile(nOffset,(char *)&qValue,8);
}

//void Binary::writeByteToMemory(unsigned int nOffset, unsigned char cValue)
//{
//    writeArrayToMemory(nOffset,(char *)&cValue,1);
//}

//void Binary::writeWordToMemory(unsigned int nOffset, unsigned short sValue)
//{
//    writeArrayToMemory(nOffset,(char *)&sValue,2);
//}

//void Binary::writeDwordToMemory(unsigned int nOffset, unsigned int nValue)
//{
//    writeArrayToMemory(nOffset,(char *)&nValue,4);
//}

//void Binary::writeQwordToMemory(unsigned int nOffset, unsigned long long qValue)
//{
//    writeArrayToMemory(nOffset,(char *)&qValue,8);
//}
bool Binary::createFile(QString sFileName,unsigned int nSize)
{
    QFile tempfile;
    tempfile.setFileName(sFileName);

    if(tempfile.open(QIODevice::ReadWrite))
    {
        if(tempfile.resize(nSize))
        {
            tempfile.close();

            if(setFileName(sFileName,false))
            {
                return true;
            }

            return false;
        }

        emit appendError(QString("Cannot resize file: %1 (size=%2)").arg(sFileName).arg(nSize));
        return false;

    }

    emit appendError(QString("Cannot open file: %1").arg(sFileName));
    return false;
}

bool Binary::deleteFile()
{
    QString sTemp=file.fileName();
    file.unmap(___pMemory);
    file.close();
    return QFile::remove(sTemp);
}

bool Binary::copy(Binary *pFileDest, unsigned int nOffsetDest, Binary *pFileSource, unsigned int nOffsetSource,unsigned int nSize)
{
    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE];

    while(nSize>0)
    {
        nTemp=MINIMAL(BUFFER_SIZE,nSize);

        if(!pFileSource->readArrayFromFile(nOffsetSource,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit pFileSource->appendError("Copy error");

            return false;
        }

        if(!pFileDest->writeArrayToFile(nOffsetDest,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit pFileDest->appendError("Copy error");

            return false;
        }

        nSize-=nTemp;
        nOffsetSource+=nTemp;
        nOffsetDest+=nTemp;
    }

    delete[] pBuffer;

    //    pFileDest->memoryReload(); // ??? was only memoryReload()

    return true;
}

bool Binary::copy(Binary *pFileSource)
{
    if(resize(pFileSource->size()))
    {
        if(!copy(this,0,pFileSource,0,pFileSource->size()))
        {
            return true;
        }

        //        memoryReload();

        return true;
    }

    appendError("Cannot resize file");

    return false;
}
bool Binary::dump(QString sFileName, unsigned int nOffset, unsigned int nSize)
{
    Binary tempfile;

    if(!tempfile.createFile(sFileName,nSize))
    {
        return false;
    }


    unsigned int nOffsetDest=0;
    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE];

    while(nSize>0)
    {
        nTemp=MINIMAL(BUFFER_SIZE,nSize);

        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Copy error");

            return false;
        }

        if(!tempfile.writeArrayToFile(nOffsetDest,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Copy error");

            return false;
        }

        nSize-=nTemp;
        nOffset+=nTemp;
        nOffsetDest+=nTemp;
    }

    delete[] pBuffer;

    return true;
}

QString Binary::getType(QString sFileName)
{
    Binary bin;
    bin.setFileName(sFileName);

    return bin.getType();
}

unsigned long long Binary::getSize(QString sFileName)
{
    Binary bin;
    bin.setFileName(sFileName);

    return bin.size(); // (unsigned int)
}

QString Binary::getFileDirectory()
{
    QFileInfo fi;

    fi.setFile(getFileName());

    return fi.dir().path();
}

void Binary::_loadEntryPoint()
{
    baEntryPoint.clear();

    file.seek(nEntryPointOffset);

    baEntryPoint=file.read(EPCODE);

    bIsEntryPontLoad=true;
}

void Binary::entryPointLoad()
{
    nEntryPointOffset=0;

    _loadEntryPoint();
}

QString Binary::getSignature(unsigned int nOffset, unsigned int nSize)
{
    return readArray(nOffset,nSize).toHex().toUpper();
}

//unsigned int Binary::intelDisasm(DISASM *pDisasm,unsigned int nOffset, unsigned int nSize)
//{
//    if(!isOffsetValid(nOffset))
//    {
//        return 0;
//    }

//    if(nOffset+nSize>=___nSize)
//    {
//        nSize=0;
//    }

//    if(nSize==0)
//    {
//        nSize=___nSize-nOffset;
//    }

//    if(nSize==0)
//    {
//        return 0;
//    }

////    DISASM MyDisasm;
////    memset(&MyDisasm,0,sizeof(MyDisasm));

////    if(nMode==16)
////    {
////        MyDisasm.Archi=0x10;
////    }
////    else if(nMode==64)
////    {
////        MyDisasm.Archi=0x40;
////    }
////    else // 32
////    {
////        MyDisasm.Archi=0;
////    }

//    nSize=qMin(nSize,(unsigned int)15);

//    QByteArray baTemp=readArray(nOffset,nSize);

//    pDisasm->EIP=(UIntPtr)baTemp.data();
//    pDisasm->SecurityBlock =nSize+1;

//    int nLen = Disasm(pDisasm);

//    return nLen;
//}

//void Binary::_clearMemory()
//{
//    nMemoryPosition=1;
//    for(int i=0;i<MEMORYCOUNT;i++)
//    {
//        _memorylist[i].bIsChanged=false;
//        _memorylist[i].nOffset=0;
//        _memorylist[i].baData.clear();
//    }
//}

//void Binary::addMemory(unsigned int nOffset, unsigned int nSize)
//{
//    nMemoryPosition++;
//    if(nMemoryPosition>MEMORYCOUNT-1)
//    {
//        nMemoryPosition=1;
//    }

//    _memorylist[nMemoryPosition].bIsChanged=false;
//    _memorylist[nMemoryPosition].nOffset=nOffset;
//    _memorylist[nMemoryPosition].baData.clear();

//    file.seek(nOffset);
//    _memorylist[nMemoryPosition].baData=file.read(nSize);
//}
//void Binary::memoryReload()
//{
////    int nSize=baHeader.size();
////    baHeader.clear();
////    file.seek(0);
////    baHeader=file.read(nSize);

////    __MEMSTRUCT data;

////    for(int i=0;i<memorylist.count();i++)
////    {
////        data=memorylist.at(i);
////        file.seek(data.nOffset);
////        data.baData=file.read(data.nSize);
////    }

//    for(int i=0;i<MEMORYCOUNT;i++)
//    {
//        if(_memorylist[i].baData.size())
//        {
//            int nTemp=_memorylist[i].baData.size();
//            _memorylist[i].baData.clear();
//            file.seek(_memorylist[i].nOffset);
//            _memorylist[i].baData=file.read(nTemp);
//        }

////        if((nOffset>=_memorylist[i].nOffset)&&(nOffset+nSize<=_memorylist[i].nOffset+_memorylist[i].baData.size()))
////        {
////            return true;
////        }
//    }

//}

//bool Binary::addMemory(unsigned int nOffset, unsigned int nSize)
//{
//    if(nOffset>=size())
//    {
//        emit appendError(QString("Invalid offset: %1").arg(nOffset,8,16,QChar('0')));

//        return false;
//    }

//    if(nOffset+nSize>size())
//    {
//        nSize=size()-nOffset;
//    }


//    __MEMSTRUCT data;
//    data.baData.resize(nSize);

//    file.seek(nOffset);

//    if(nSize==file.read(data.baData.data(),nSize))
//    {
//        data.nOffset=nOffset;
//        data.nSize=nSize;
//        memorylist.append(data);

//        return true;
//    }
//    emit appendError("Cannot add memory");

//    return false;
//}

//void Binary::clearMemory()
//{
//    memorylist.clear();
//}
QString Binary::getString(unsigned int nOffset,unsigned int nMaxSize)
{
    QString sResult;

    if(isOffsetValid(nOffset))
    {
        nMaxSize=qMin((unsigned long long)nMaxSize,size()-nOffset);

        unsigned char *pBuffer=new unsigned char[nMaxSize+1];
        memset(pBuffer,0,nMaxSize+1);

        unsigned char *pOffset=0;
        QByteArray baData;

        if(___pMemory)
        {
            pOffset=___pMemory+nOffset;
        }
        else
        {
            baData=readArray(nOffset,nMaxSize);

            pOffset=(unsigned char *)baData.data();
        }

        for(int i=0; i<nMaxSize; i++)
        {
            //            pBuffer[i]=readByte(nOffset+i);
            if(pOffset[i]==0)
            {
                break;
            }
            else
            {
                pBuffer[i]=pOffset[i];
            }
        }

        //    readArray(nOffset,pBuffer,nMaxSize);
        //        strcpy_s(pBuffer,nMaxSize+1,___pMemory+nOffset);

        sResult.append((char *)pBuffer);

        delete[] pBuffer;
    }



    return sResult;
}

QString Binary::getAnsiString(unsigned int nOffset, unsigned int nMaxSize)
{
    if(!isOffsetValid(nOffset))
    {
        return "";
    }

    nMaxSize=qMin((unsigned long long)nMaxSize,size()-nOffset);

    QString sResult;

    char *pBuffer=new char[nMaxSize+1];
    memset(pBuffer,0,nMaxSize+1);

    //    for(int i=0;i<nMaxSize;i++)
    //    {
    //        pBuffer[i]=readByte(nOffset+i);
    //        if((pBuffer[i]==0)||(pBuffer[i]>127))
    //        {
    //            break;
    //        }
    //    }
    //    readArray(nOffset,pBuffer,nMaxSize);

    unsigned char *pOffset=0;
    QByteArray baData;

    if(___pMemory)
    {
        pOffset=___pMemory+nOffset;
    }
    else
    {
        baData=readArray(nOffset,nMaxSize);

        pOffset=(unsigned char *)baData.data();
    }


    for(int i=0; i<nMaxSize; i++)
    {
        if((pOffset[i]==0)||((unsigned char)pOffset[i]>127))
        {
            pBuffer[i]=0;
            break;
        }
        else
        {
            pBuffer[i]=pOffset[i];
        }
    }

    sResult.append((char *)pBuffer);

    delete[] pBuffer;

    return sResult;
}

void Binary::setString(unsigned int nOffset, QString sValue)
{
#if QT_VERSION >= 0x050000
    writeArray(nOffset,sValue.toLatin1().data(),sValue.length());
#else
    writeArray(nOffset,sValue.toAscii().data(),sValue.length());
#endif
}
unsigned long long Binary::OffsetToRVA(unsigned long long nOffset)
{
    //#ifdef QT_DEBUG
    //    qDebug("Binary::OffsetToRVA");
    //#endif

    if(isOffsetValid(nOffset))
    {
        return nOffset;
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,0,16,QChar('0')));
    return -1;
}
unsigned long long Binary::RVAToOffset(unsigned long long nRVA)
{
    nRVA=nRVA&0xFFFF;

    //#ifdef QT_DEBUG
    //    qDebug("Binary::RVAToOffset");
    //#endif
    if(isOffsetValid(nRVA))
    {
        return nRVA;
    }

    emit appendWarning(QString("Invalid RVA: %1").arg(nRVA,0,16,QChar('0')));
    return -1;
}
unsigned long long Binary::VAToOffset(unsigned long long nVA)
{
    //#ifdef QT_DEBUG
    //    qDebug("Binary::VAToOffset");
    //#endif
    if(isOffsetValid(nVA))
    {
        return nVA;
    }

    emit appendWarning(QString("Invalid VA: %1").arg(nVA,0,16,QChar('0')));
    return -1;
}

unsigned long long Binary::VAToOffset32(unsigned long long nVA)
{
    return VAToOffset(nVA);
}

unsigned long long Binary::OffsetToVA(unsigned long long nOffset)
{
    if(isOffsetValid(nOffset))
    {
        return nOffset;
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,0,16,QChar('0')));
    return -1;
}
bool Binary::compare(QString sSignature, unsigned int nOffset)
{
    sSignature=convertSignature(sSignature);

    if((sSignature.contains("$"))||(sSignature.contains("#")))
    {
        unsigned int nDword=0;
        unsigned short sWord=0;
        unsigned char cByte=0;
        unsigned int nRVA=0;
        unsigned long long nBase=0;
        QString sSeq;

        while(true)
        {
            if(sSignature.left(8)=="$$$$$$$$")
            {
                nDword=readDword(nOffset);

                nRVA=OffsetToRVA(nOffset);

                if(nRVA==-1)
                {
                    return false;
                }

                int nTest=nDword;
                nTest++;

                nRVA+=(int)nDword+4;

                nOffset=RVAToOffset(nRVA);

                if(nOffset==-1)
                {
                    return false;
                }

                sSignature=sSignature.mid(8);
            }
            else if(sSignature.left(4)=="$$$$")
            {
                sWord=readWord(nOffset);

                nRVA=OffsetToRVA(nOffset);

                if(nRVA==-1)
                {
                    return false;
                }

                nRVA+=(short)sWord+2;

                nOffset=RVAToOffset(nRVA);

                if(nOffset==-1)
                {
                    return false;
                }

                sSignature=sSignature.mid(4);
            }
            else if(sSignature.left(2)=="$$")
            {
                cByte=readByte(nOffset);

                nRVA=OffsetToRVA(nOffset);

                if(nRVA==-1)
                {
                    return false;
                }

                nRVA+=(char)cByte+1;

                nOffset=RVAToOffset(nRVA);

                if(nOffset==-1)
                {
                    return false;
                }

                sSignature=sSignature.mid(2);
            }
            else if(sSignature.left(8)=="########")
            {
                sSignature=sSignature.mid(8);

                if(sSignature.left(1)=="[")
                {
                    nBase=sSignature.section("[",1,1).section("]",0,0).toLongLong(0,16);
                    sSignature=sSignature.section("]",1,-1);
                }

                nDword=readDword(nOffset)-nBase;

                nOffset=VAToOffset32(nDword);

                if(nOffset==-1)
                {
                    return false;
                }


            }
            else if(sSignature.left(4)=="####")
            {
                sSignature=sSignature.mid(4);

                if(sSignature.left(1)=="[")
                {
                    nBase=sSignature.section("[",1,1).section("]",0,0).toLongLong(0,16);
                    sSignature=sSignature.section("]",1,-1);
                }

                sWord=readWord(nOffset)-nBase;

                nOffset=VAToOffset(sWord);

                if(nOffset==-1)
                {
                    return false;
                }


            }
            else if(sSignature.left(2)=="##")
            {
                sSignature=sSignature.mid(2);

                if(sSignature.left(1)=="[")
                {
                    nBase=sSignature.section("[",1,1).section("]",0,0).toLongLong(0,16);
                    sSignature=sSignature.section("]",1,-1);
                }

                cByte=readByte(nOffset)-nBase;

                nOffset=VAToOffset(cByte);

                if(nOffset==-1)
                {
                    return false;
                }


            }

            if(sSignature=="")
            {
                return true;
            }


            sSeq=sSignature.section("$",0,0);

            if(sSeq.contains("#"))
            {
                sSeq=sSignature.section("#",0,0);
            }

            if(!_compare(sSeq,nOffset))
            {
                return false;
            }


            nOffset+=sSeq.size()/2;
            sSignature=sSignature.mid(sSeq.size());
        }
    }
    else
    {
        int nEPDelta=nOffset-nEntryPointOffset;

        if((nEPDelta>=0)&&(2*nEPDelta+sSignature.size()<baEntryPoint.size()*2))
        {
            return compareEPfast(sSignature,nEPDelta);
        }
        else
        {
            return _compare(sSignature,nOffset);
        }

    }
}

//bool Binary::compare(char *pszSignature,unsigned int nOffset)
//{
//    bool bResult=false;
//    int nSize=strlen(pszSignature)/2;

//    char *pBuffer=new char[nSize];
//    char *pBuffer2=new char[nSize*2+1];
////    char *pHex;

//    if(readArray(nOffset,pBuffer,nSize))
//    {
//        dataToHex(pBuffer,nSize,pBuffer2);
//        bResult=signatureCompare(pBuffer2,pszSignature,nSize*2);
//    }
//    else
//    {
//        bResult=false;
//    }

//    delete[] pBuffer;
//    delete[] pBuffer2;

//    return bResult;
//}
void Binary::close()
{
    //    _clearMemory();
    file.close();
}
//bool Binary::compareHeader(char *pszSignature)
//{
//    return compare(pszSignature,0);
//}

bool Binary::compareEPfast(QString sSignature,int nOffset)
{
    //#ifdef QT_DEBUG
    //    qDebug("Binary::compareEPfast");
    //#endif
    bool bResult=false;
    int nSize=sSignature.size()/2;

    char *pBuffer=new char[nSize*2+1];

    if(nSize+nOffset<=baEntryPoint.size())
    {

        dataToHex(baEntryPoint.data()+nOffset,nSize,pBuffer);
        bResult=signatureCompare(pBuffer,sSignature.toAscii().data(),nSize*2);

    }
    else
    {
        QByteArray baBuffer=readArray(nEntryPointOffset+nOffset,nSize);
        dataToHex(baBuffer.data(),nSize,pBuffer);
        bResult=signatureCompare(pBuffer,sSignature.toAscii().data(),nSize*2);
    }

    delete[] pBuffer;

    return bResult;
}

//bool Binary::compareEPfast(QString sSignature)
//{
//    return compareEPfast(convertSignature(sSignature).toAscii().data());
//}

bool Binary::compareEP(QString sSignature,unsigned int nOffset)
{
    //    if(nOffset==0)
    //    {
    //        return compareEPfast(convertSignature(sSignature).toAscii().data());
    //    }
    //    else
    //    {
    //        return _compare(sSignature,nEntryPointOffset+nOffset);
    //    }
    //    return compareEPfast(convertSignature(sSignature).toAscii().data(),nOffset);
    //    return compare(sSignature,getEntryPointOffset()+nOffset);
    return compare(sSignature,nOffset);
}

bool Binary::isPlainText()
{
    QByteArray baData=readArray(0,0x1000);

    unsigned char *pDataOffset=(unsigned char *)baData.data();
    int nDataSize=baData.size();

    if(nDataSize==0)
    {
        return false;
    }

    for(int i=0; i<nDataSize; i++)
    {
        if(pDataOffset[i]<0x9)
        {
            return false;
        }
    }

    return true;
}

bool Binary::_compare(QString sSignature,unsigned int nOffset)
{
    //#ifdef QT_DEBUG
    //    qDebug("Binary::_compare");
    //#endif
    //    return compare(convertSignature(sSignature).toAscii().data(),nOffset);
    bool bResult=false;
    int nSize=sSignature.size()/2;

    char *pBuffer=new char[nSize];
    char *pBuffer2=new char[nSize*2+1];
    //    char *pHex;

    if(readArray(nOffset,pBuffer,nSize))
    {
        dataToHex(pBuffer,nSize,pBuffer2);
        bResult=signatureCompare(pBuffer2,sSignature.toAscii().data(),nSize*2);
    }
    else
    {
        bResult=false;
    }

    delete[] pBuffer;
    delete[] pBuffer2;

    return bResult;
}

//bool Binary::compareFast(QString sSignature,unsigned int nOffset)
//{
//    return _compare(sSignature,nOffset);
//}

//bool Binary::compareHeader(QString sSignature)
//{
//    return _compare(sSignature,0);
//}

QString Binary::convertSignature(QString sSignature)
{
    if(sSignature.contains(QChar(39)))
    {
        QByteArray baBuffer;
        QByteArray baSignature=sSignature.toAscii();

        baBuffer.resize(sSignature.size()*2);
        baBuffer.fill(0);

        char *pBuffer=baBuffer.data();
        char *pSignature=baSignature.data();
        int nSignatureSize=baSignature.size();
        bool bIsANSI=false;
        char cANSI[2];

        for(int i=0,j=0; i<nSignatureSize; i++)
        {
            if(pSignature[i]==39)
            {
                bIsANSI=!bIsANSI;
            }
            else if(bIsANSI)
            {
                dataToHex(&pSignature[i],1,cANSI);
                pBuffer[j]=cANSI[0];
                j++;
                pBuffer[j]=cANSI[1];
                j++;
            }
            else
            {
                pBuffer[j]=pSignature[i];

                j++;
            }
        }

        sSignature=pBuffer;
    }

    sSignature=sSignature.toUpper();

    if(sSignature.contains(QChar(' ')))
    {
        sSignature=sSignature.remove(QChar(' '));
    }

    if(sSignature.contains(QChar('?')))
    {
        sSignature=sSignature.replace(QChar('?'),QChar('.'));
    }

    return sSignature;
}
bool Binary::dataToHex(char *pData, unsigned int nDataSize,char *pOut)
{
    unsigned char cTemp;


    for(int y=0, x=0; y<nDataSize; ++y, ++x)
    {

        cTemp=((unsigned char)pData[y])>>4;

        pOut[x]=(char)(cTemp>9 ? cTemp+0x37 : cTemp+0x30);

        cTemp=((unsigned char)(pData[y]&0xF));

        pOut[++x]=(char)(cTemp>9 ? cTemp+0x37 : cTemp+0x30);
    }


    pOut[nDataSize*2]=0;


    return true;
}
bool Binary::signatureCompare(char *pData,char *pSignature,int nSize)
{
    for(int i=0,j=0; i<nSize; i++,j++)
    {
        //        if(pSignature[j]==' ') // remove spaces
        //        {
        //            j++;
        //        }
        //        if((pSignature[j]!='.')&&(pSignature[j]!='?')) // wildcards
        if(pSignature[j]!='.')
        {
            if(pData[i]!=pSignature[j])
            {
                return false;
            }

        }
    }

    return true;
}

bool Binary::memoryCompare(char *pMemory1, char *pMemory2, int nSize)
{
    //    for(int i=0,j=0;i<nSize;i++,j++)
    //    {
    //        if(pMemory1[i]!=pMemory2[j])
    //        {
    //            return false;
    //        }
    //    }
    //    return true;

    while(nSize>0)
    {
        if(nSize>=4)
        {
            if(*((unsigned int *)pMemory1)!=*((unsigned int *)pMemory2))
            {
                return false;
            }

            pMemory1+=4;
            pMemory2+=4;
            nSize-=4;
        }
        else if(nSize>=2)
        {
            if(*((unsigned short *)pMemory1)!=*((unsigned short *)pMemory2))
            {
                return false;
            }

            pMemory1+=2;
            pMemory2+=2;
            nSize-=2;
        }
        else
        {
            if(*(pMemory1)!=*(pMemory2))
            {
                return false;
            }

            pMemory1++;
            pMemory2++;
            nSize--;
        }
    }

    return true;
}

unsigned int Binary::findDataInMemory(char *pMemory, unsigned int nMemorySize, char *pData, unsigned int nDataSize)
{
    for(int i=0; i<(int)(nMemorySize-nDataSize+1); i++)
    {
        if(memoryCompare(pMemory+i,pData,nDataSize))
        {
            return i;
        }
    }

    return -1;
}

unsigned int Binary::findSignatureInMemory(char *pMemory, unsigned int nMemorySize, QString sSignature)
{
    QByteArray baSig;
    QString sPart;
    unsigned int nOffset=0;
    unsigned int nNext=0;
    bool bIsFirst=false;
    int nSignatureSize=0;
    unsigned int nResult=0;

    if((!sSignature.contains(".")))
    {
        baSig.append(sSignature);

        baSig=QByteArray::fromHex(baSig);

        return findDataInMemory(pMemory,nMemorySize,baSig.data(),baSig.size());
    }
    else
    {
        bIsFirst=true;

        while(true)
        {
            if(bIsFirst)
            {

                nNext=0;
            }

            nSignatureSize=sSignature.size();

            for(int i=0; i<nSignatureSize; i++)
            {
                if(sSignature.at(i)!=QChar('.'))
                {
                    sSignature=sSignature.mid(i);
                    nOffset+=i/2;
                    break;
                }
            }


            sPart=sSignature.section(".",0,0);
            sSignature="."+sSignature.section(".",1,-1);

            if(sPart=="")
            {
                //Success
                return nResult;
            }


            baSig.clear();
            baSig.append(sPart);


            baSig=QByteArray::fromHex(baSig);

            if(bIsFirst)
            {
                nNext=findDataInMemory(pMemory+nOffset,nMemorySize-nOffset,baSig.data(),baSig.size());
                nResult=nOffset+nNext;
            }
            else
            {
                if(memoryCompare(pMemory+nOffset,baSig.data(),baSig.size()))
                {
                    nNext=0;
                }
                else
                {
                    nNext=-1;
                }

                //                memoryCompare(char *pMemory1, char *pMemory2, int nSize)
            }

            if(nNext==-1)
            {
                if(bIsFirst)
                {
                    break;
                }
                else
                {
                    bIsFirst=true;
                    continue;
                }

            }

            if(bIsFirst)
            {
                bIsFirst=false;
            }

            nOffset+=nNext+baSig.size();
        }

    }

    return -1;
}
//QByteArray Binary::getHeaderData()
//{
//    return baHeader;
//}

unsigned int Binary::findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue)
{
    if(nSize==0)
    {
        return -1;
    }

    if(nSize==-1)
    {
        nSize=size()-nOffset;
    }

    if(!isOffsetValid(nOffset))
    {
        return -1;
    }

    if(!isSizeValid(nSize))
    {
        return -1;
    }

    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE];

    while(nSize>0)
    {
        nTemp=MINIMAL(BUFFER_SIZE,nSize);

        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Read error");

            return -1;
        }

        for(int i=0; i<nTemp; i++)
        {
            if(*((unsigned char *)(pBuffer+i))==cValue)
            {
                delete[] pBuffer;

                return nOffset+i;
            }
        }

        nSize-=nTemp;
        nOffset+=nTemp;
    }

    delete[] pBuffer;

    return -1;
}

unsigned int Binary::findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue)
{
    if(nSize==0)
    {
        return -1;
    }

    if(nSize==-1)
    {
        nSize=size()-nOffset;
    }

    if(!isOffsetValid(nOffset))
    {
        return -1;
    }

    if(!isSizeValid(nSize))
    {
        return -1;
    }

    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE+1];


    while(nSize>1)
    {
        nTemp=MINIMAL(BUFFER_SIZE+1,nSize);

        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Read error");

            return -1;
        }

        for(int i=0; i<nTemp-1; i++)
        {
            if(*((unsigned short *)(pBuffer+i))==sValue)
            {
                delete[] pBuffer;

                return nOffset+i;
            }
        }

        nSize-=nTemp-1;
        nOffset+=nTemp-1;
    }

    delete[] pBuffer;

    return -1;
}

unsigned int Binary::findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue)
{
    if(nSize==0)
    {
        return -1;
    }

    if(nSize==-1)
    {
        nSize=size()-nOffset;
    }

    if(!isOffsetValid(nOffset))
    {
        return -1;
    }

    if(!isSizeValid(nSize))
    {
        return -1;
    }

    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE+3];


    while(nSize>3)
    {
        nTemp=MINIMAL(BUFFER_SIZE+3,nSize);

        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Read error");

            return -1;
        }

        for(int i=0; i<nTemp-3; i++)
        {
            if(*((unsigned int *)(pBuffer+i))==nValue)
            {
                delete[] pBuffer;

                return nOffset+i;
            }
        }

        nSize-=nTemp-3;
        nOffset+=nTemp-3;
    }

    delete[] pBuffer;

    return -1;
}

unsigned int Binary::findArray(unsigned int nOffset, unsigned int nSize, char *pArray, unsigned int nArraySize)
{
    if(nSize==0)
    {
        return -1;
    }

    if(nArraySize==0)
    {
        return -1;
    }

    if(nSize==-1)
    {
        nSize=size()-nOffset;
    }

    if(nArraySize>nSize)
    {
        return -1;
    }

    if(!isOffsetValid(nOffset))
    {
        return -1;
    }

    if(!isSizeValid(nSize))
    {
        return -1;
    }

    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE+(nArraySize-1)];


    while(nSize>nArraySize-1)
    {
        nTemp=MINIMAL(BUFFER_SIZE+(nArraySize-1),nSize);

        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Read error");

            return -1;
        }

        for(int i=0; i<nTemp-(nArraySize-1); i++)
        {
            if(pBuffer[i]==pArray[0])
            {
                if(memoryCompare(pBuffer+i+1,pArray+1,nArraySize-1))
                {
                    delete[] pBuffer;

                    return nOffset+i;
                }
            }
        }

        nSize-=nTemp-(nArraySize-1);
        nOffset+=nTemp-(nArraySize-1);
    }

    delete[] pBuffer;

    return -1;
}

unsigned int Binary::findString(unsigned int nOffset, unsigned int nSize, QString sString)
{
    return findArray(nOffset,nSize,sString.toAscii().data(),sString.toAscii().size());
}

unsigned int Binary::findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    if(nSize==0)
    {
        return -1;
    }

    if(sSignature=="")
    {
        return -1;
    }

    if(nSize==-1)
    {
        nSize=size()-nOffset;
    }

    if(sSignature.size()>nSize)
    {
        return -1;
    }

    if(!isOffsetValid(nOffset))
    {
        return -1;
    }

    if(!isSizeValid(nSize))
    {
        return -1;
    }

    sSignature=convertSignature(sSignature);

    QByteArray baSig;

    if((!sSignature.contains("."))&&(!sSignature.contains("?")))
    {
        baSig.append(sSignature);

        baSig=QByteArray::fromHex(baSig);

        return findArray(nOffset,nSize,baSig.data(),baSig.size());
    }
    else
    {
        QString sPattern=sSignature.section(".",0,0);

        baSig.clear();
        baSig.append(sPattern);
        baSig=QByteArray::fromHex(baSig);

        unsigned int nResult=0;

        while(true)
        {
            nResult=findArray(nOffset,nSize,baSig.data(),baSig.size());

            if(nResult==-1)
            {
                return -1;
            }

            if(compare(sSignature,nResult))
            {
                return nResult;
            }

            nResult++;

            nSize-=(nResult-nOffset);
            nOffset=nResult;

        }

    }

    // TODO!!!

    return -1;


    //    unsigned int nArraySize=sSignature.size();
    //    unsigned int nTemp=0;
    //    char *pBuffer=new char[BUFFER_SIZE+(nArraySize-1)];
    //    char *pBuffer2=new char[(BUFFER_SIZE+(nArraySize-1))*2];


    //    char *pszSignature=sSignature.toAscii().data();
    //    int nSignatureSize=sSignature.size();

    //    while(nSize>nArraySize-1)
    //    {
    //        nTemp=MINIMAL(BUFFER_SIZE+(nArraySize-1),nSize);

    //        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
    //        {
    //            delete[] pBuffer;
    //            delete[] pBuffer2;
    //            emit appendError("Read error");

    //            return -1;
    //        }

    //        dataToHex(pBuffer,nTemp,pBuffer2);

    //        for(int i=0;i<nTemp-(nArraySize-1);i++)
    //        {
    //            if(signatureCompare(pBuffer2+2*i,pszSignature,nSignatureSize))
    //            {
    //                delete[] pBuffer;
    //                delete[] pBuffer2;

    //                return nOffset+i;
    //            }
    //        }

    //        nSize-=nTemp-(nArraySize-1);
    //        nOffset+=nTemp-(nArraySize-1);
    //    }

    //    delete[] pBuffer;
    //    delete[] pBuffer2;

    //    return -1;
}

bool Binary::isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature)
{
    return (findSignature(nOffset,nSize,sSignature)!=-1);
}

float Binary::calculateEntropy(unsigned int nOffset,unsigned int nDataSize)
{
    if(nDataSize==0)
    {
        return 0.0;
    }

    if(nDataSize==-1)
    {
        nDataSize=size()-nOffset;

        if(nDataSize==0)
        {
            return 0.0;
        }
    }

    if(!isOffsetValid(nOffset))
    {
        return 0.0;
    }

    if(!isSizeValid(nDataSize))
    {
        return 0.0;
    }

    unsigned int nSize=nDataSize;

    float fEntropy=1.4426950408889634073599246810023;
    float bytes[256]= {0.0};
    float temp;

    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE];

    while(nSize>0)
    {
        nTemp=MINIMAL(BUFFER_SIZE,nSize);

        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Read error");

            return 0;
        }

        for(int i=0; i<nTemp; i++)
        {
            bytes[(unsigned char)pBuffer[i]]+=1;
        }

        nSize-=nTemp;
        nOffset+=nTemp;
    }

    delete[] pBuffer;

    for(int j=0; j<256; j++)
    {
        temp=bytes[j]/(float)nDataSize;

        if(temp)
        {
            fEntropy+=(-log(temp)/log((float)2))*bytes[j];
        }
    }

    fEntropy=fEntropy/(float)nDataSize;

    return fEntropy;
}

float Binary::calculateEntropy(QString sFileName, unsigned int nOffset, unsigned int nDataSize)
{
    Binary bin;

    if(bin.setFileName(sFileName,false))
    {
        return bin.calculateEntropy(nOffset,nDataSize);
    }
    else
    {
        return -1;
    }
}

QString Binary::calculateMD5(unsigned int nOffset, unsigned int nDataSize)
{
    if(nDataSize==0)
    {
        return 0;
    }

    if(nDataSize==-1)
    {
        nDataSize=size()-nOffset;
    }

    if(!isOffsetValid(nOffset))
    {
        return 0;
    }

    if(!isSizeValid(nDataSize))
    {
        return 0;
    }

    unsigned int nSize=nDataSize;


    unsigned int nTemp=0;
    char *pBuffer=new char[BUFFER_SIZE];

    QCryptographicHash crypto(QCryptographicHash::Md5);

    while(nSize>0)
    {
        nTemp=MINIMAL(BUFFER_SIZE,nSize);

        if(!readArrayFromFile(nOffset,pBuffer,nTemp))
        {
            delete[] pBuffer;
            emit appendError("Read error");

            return 0;
        }

        crypto.addData(pBuffer,nTemp);

        nSize-=nTemp;
        nOffset+=nTemp;
    }

    delete[] pBuffer;

    QString sResult=crypto.result().toHex();

    return sResult;
}

QString Binary::calculateMD5(QString sFileName, unsigned int nOffset, unsigned int nDataSize)
{
    Binary bin;
    bin.setFileName(sFileName,false);

    return bin.calculateMD5(nOffset,nDataSize);
}

bool Binary::isOffsetValid(unsigned int nOffset)
{
    if(nOffset<=___nSize)
    {
        return true;
    }

    emit appendWarning(QString("Invalid offset: %1").arg(nOffset,8,16,QChar('0')));

    return false;
}

bool Binary::isSizeValid(unsigned int nSize)
{
    if(nSize<=size())
    {
        return true;
    }

    emit appendWarning(QString("Invalid size: %1").arg(nSize));

    return false;
}

bool Binary::isOffsetAndSizeValid(unsigned int nOffset, unsigned int nSize)
{
    if(nOffset>___nSize)
    {
        emit appendWarning(QString("Invalid offset: %1").arg(nOffset,8,16,QChar('0')));
        return false;
    }

    if(nOffset+nSize>___nSize)
    {
        // SEGFAULT on Linux
        //        emit appendWarning(QString("Invalid size: %1").arg(nSize,8,16,QChar('0')));
        return false;
    }

    return true;
}

QList<QString> Binary::getTypes()
{
    QList<QString> listResult;

    QByteArray baHeader;
    baHeader=readArray(0,BUFFER_SIZE);
    char *pOffset=baHeader.data();
    unsigned int nSize=size();

    if(nSize>=(int)sizeof(IMAGE_DOS_HEADER))
    {
        if((((IMAGE_DOS_HEADER *)pOffset)->e_magic==IMAGE_DOS_SIGNATURE)||(((IMAGE_DOS_HEADER *)pOffset)->e_magic==0x4D5A))
        {
            unsigned int nLfanew=0;
            nLfanew=((IMAGE_DOS_HEADER *)pOffset)->e_lfanew;
            unsigned int nHeaderSize=baHeader.size()-sizeof(IMAGE_NT_HEADERS32);
            QByteArray baNTHeaders;

            bool isHeaderValid=false;

            if((nLfanew<nHeaderSize)&&(baHeader.size()>sizeof(IMAGE_NT_HEADERS32)))
            {
                pOffset+=nLfanew;
                isHeaderValid=true;
            }
            else
            {
                baNTHeaders=readArray(nLfanew,sizeof(IMAGE_NT_HEADERS32));

                if(baNTHeaders.size()==sizeof(IMAGE_NT_HEADERS32))
                {
                    pOffset=baNTHeaders.data();
                    isHeaderValid=true;
                }
            }

            if(isHeaderValid)
            {
                if((((IMAGE_NT_HEADERS *)pOffset))->Signature==IMAGE_NT_SIGNATURE)
                {
                    if((((IMAGE_NT_HEADERS *)pOffset)->FileHeader.Machine)==IMAGE_FILE_MACHINE_AMD64)
                    {
                        listResult.append(QString("PE+(64)"));
                    }
                    else if((((IMAGE_NT_HEADERS *)pOffset)->FileHeader.Machine)==IMAGE_FILE_MACHINE_IA64)
                    {
                        listResult.append(QString("PE+(64)"));
                    }

                    listResult.append(QString("PE"));
                }
            }

            listResult.append(QString("MSDOS"));
        }
    }

    if(nSize>=(int)sizeof(Elf32_Ehdr))
    {
        if((((Elf32_Ehdr *)pOffset)->e_ident[0] == 0x7f) &&
                (((Elf32_Ehdr *)pOffset)->e_ident[1] == 'E') &&
                (((Elf32_Ehdr *)pOffset)->e_ident[2] == 'L') &&
                (((Elf32_Ehdr *)pOffset)->e_ident[3] == 'F'))
        {
            if(((Elf32_Ehdr *)pOffset)->e_ident[4] == 1)
            {
                listResult.append(QString("ELF"));
            }
            else if(((Elf32_Ehdr *)pOffset)->e_ident[4] == 2)
            {
                listResult.append(QString("ELF64"));
            }
        }
    }

    if(nSize>=(int)sizeof(mach_header))
    {
        if(((((mach_header *)pOffset)->magic == MH_MAGIC)||(((mach_header *)pOffset)->magic == MH_CIGAM))&&(((mach_header *)pOffset)->filetype<0xFFFF))
        {
            listResult.append(QString("MACH"));
        }
        else if(((((mach_header *)pOffset)->magic == MH_MAGIC_64)||(((mach_header *)pOffset)->magic == MH_CIGAM_64))&&(((mach_header *)pOffset)->filetype<0xFFFF))
        {
            listResult.append(QString("MACH64"));
        }
    }

    if(isPlainText())
    {
        listResult.append(QString("Text"));
    }

    listResult.append(QString("Binary"));

    return listResult;
}

QString Binary::getType()
{
    return getTypes().at(0);
}
QString Binary::getDisasmMode()
{
    QByteArray baHeader;
    baHeader=readArray(0,BUFFER_SIZE);
    char *pOffset=baHeader.data();
    unsigned int nSize=size();

    if(nSize>=(int)sizeof(IMAGE_DOS_HEADER))
    {
        if(((IMAGE_DOS_HEADER *)pOffset)->e_magic==IMAGE_DOS_SIGNATURE)
        {
            if((((IMAGE_DOS_HEADER *)pOffset)->e_lfanew<baHeader.size()-sizeof(IMAGE_NT_HEADERS))&&(baHeader.size()>sizeof(IMAGE_NT_HEADERS)))
            {
                pOffset+=((IMAGE_DOS_HEADER *)pOffset)->e_lfanew;

                if(((IMAGE_NT_HEADERS *)pOffset)->Signature==IMAGE_NT_SIGNATURE)
                {
                    if((((IMAGE_NT_HEADERS *)pOffset)->FileHeader.Machine)==IMAGE_FILE_MACHINE_AMD64)
                    {
                        return QString("64");
                    }
                    else if((((IMAGE_NT_HEADERS *)pOffset)->FileHeader.Machine)==IMAGE_FILE_MACHINE_IA64)
                    {
                        return QString("64");
                    }

                    return QString("32");
                }
            }
        }
    }

    if(nSize>=(int)sizeof(Elf32_Ehdr))
    {
        if((((Elf32_Ehdr *)pOffset)->e_ident[0] == 0x7f) &&
                (((Elf32_Ehdr *)pOffset)->e_ident[1] == 'E') &&
                (((Elf32_Ehdr *)pOffset)->e_ident[2] == 'L') &&
                (((Elf32_Ehdr *)pOffset)->e_ident[3] == 'F'))
        {
            if(((Elf32_Ehdr *)pOffset)->e_ident[4] == 1)
            {
                return QString("32");
            }
            else if(((Elf32_Ehdr *)pOffset)->e_ident[4] == 2)
            {
                return QString("64");
            }
        }
    }

    if(nSize>=(int)sizeof(mach_header))
    {
        if(((((mach_header *)pOffset)->magic == MH_MAGIC)||(((mach_header *)pOffset)->magic == MH_CIGAM))&&(((mach_header *)pOffset)->filetype<0xFFFF))
        {
            return QString("32");
        }
        else if(((((mach_header *)pOffset)->magic == MH_MAGIC_64)||(((mach_header *)pOffset)->magic == MH_CIGAM_64))&&(((mach_header *)pOffset)->filetype<0xFFFF))
        {
            return QString("64");
        }
    }

    return QString("16");
}
