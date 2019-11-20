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
#include "scan.h"

//using namespace QtConcurrent;

Scan::Scan(QObject *parent) :
    QObject(parent)
{
    connect(this,SIGNAL(die_appendSignatureSignal(QString)),this,SLOT(die_appendSignatureSlot(QString)));

    dirContent=0;
    nNumberOfSignatures=0;
    bIsRun=false;
    bIsCompleted=true;

    bShowSource=false;
}

void Scan::setData(QString sScanFileName,__DIE_OPTIONS *pOptions)
{
    this->sScanFileName=sScanFileName;
    this->pOptions=pOptions;
}

void Scan::setData(QList<QString> *dirContent,__DIE_OPTIONS *pOptions)
{
    this->dirContent=dirContent;
    this->pOptions=pOptions;
}

void Scan::setData(__DIE_OPTIONS *pOptions)
{
    this->pOptions=pOptions;
}

void Scan::setShowSource(bool bShowSource)
{
    this->bShowSource=bShowSource;
}

void Scan::_stop()
{
    bIsRun=false;
}

bool Scan::isRun()
{
    return bIsRun;
}

bool Scan::isCompleted()
{
    return bIsCompleted;
}

void Scan::setRun(bool bIsRun)
{
    this->bIsRun=bIsRun;
}

void Scan::process()
{
    if(bIsRun)
    {
        return;
    }

    bIsRun=true;
    bIsCompleted=false;

    if(sScanFileName!="")
    {
        analize(sScanFileName,pOptions->bFullScan);
    }
    else if(dirContent)
    {
        emit setProgressBar2(1,0);

        for(int i=0; (i<dirContent->count())&&(bIsRun); i++)
        {
            analize(dirContent->at(i),pOptions->bFullScan);
            emit die_appendSignatureSignal("");

            emit setProgressBar2(dirContent->count(),i+1);
        }

        emit setProgressBar2(1,1);
    }

    emit _finished();

    bIsCompleted=true;
    bIsRun=false;
}

bool Scan::analize(QString sFileName,bool bFullScan)
{
    if(pOptions->pMutexResult)
    {
        pOptions->pMutexResult->lock();
    }

    QTime scanTime=QTime::currentTime();

    //    pOptions->nNumberOfResults=0;

    emit appendFileName(sFileName);

    pOptions->sCurrentFileName=sFileName;

    if(pOptions->sm==SM_DIE)
    {
        pOptions->die_listResult.clear();

        Binary file;

        PluginsScript engine;

        connect(&file,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
        connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
        connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(die_appendSignatureSignal(QString)));

        QList<QString> listTypes;

        if(file.setFileName(sFileName))
        {
            listTypes=file.getTypes();
            //qDebug(sType.toAscii().data());
            file.close();
        }
        else
        {
            file.close();

            return false;
        }

        int nCount=listTypes.count();

        if(!bFullScan)
        {
            nCount=1;
        }

        for(int i=0; i<nCount; i++)
        {
            QString sType=listTypes.at(i);

            if((sType=="PE")||(sType=="PE+(64)"))
            {
                die_scanPE(sFileName,sType);
            }
            else if((sType=="ELF")||(sType=="ELF64"))
            {
                die_scanELF(sFileName,sType);
            }
            else if(sType=="MSDOS")
            {
                die_scanMSDOS(sFileName,sType);
            }
            else if((sType=="MACH")||(sType=="MACH64"))
            {
                die_scanMACH(sFileName,sType);
            }
            else if(sType=="Text")
            {
                die_scanText(sFileName,sType);
            }
            else if(sType=="Binary")
            {
                die_scanBinary(sFileName,sType);
            }
        }
    }
#ifdef USE_NFD
    else if(pOptions->sm==SM_NFD)
    {
        emit setProgressBar(1,0);

        SpecAbstract::SCAN_OPTIONS nfd_options=SpecAbstract::SCAN_OPTIONS();
        nfd_options.bRecursive=pOptions->bScanRecursiveNFD;
        nfd_options.bDeepScan=pOptions->bScanDeepScanNFD;

        pOptions->nfd_result=StaticScan::processFile(sFileName,&nfd_options);

        emit setProgressBar(1,1);
    }
#endif
#ifdef USE_YARA
    else if(pOptions->sm==SM_YARA)
    {
        emit setProgressBar(1,0);

        pOptions->yara_result=QYara::scanFile(sFileName,pOptions->sDataBaseYARA);

        emit setProgressBar(1,1);
    }
#endif

    int nMs=scanTime.msecsTo(QTime::currentTime());

    emit singleScanComplete(nMs);

    return true;
}

bool Scan::die_scanPE(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(die_appendSignatureSignal(QString)));

    PEFile _pefile;
    connect(&_pefile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptPE scriptpe;
    connect(&scriptpe,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_pefile.setFileName(sFileName))
    {
        //            if(!_pefile.completeCheck())
        //            {
        //                emit setProgressBar(1,1);
        //                return false;
        //            }

        //            emit appendError(QString("Number of signatures: %1").arg(nNumberOfSignatures));

        _pefile.entryPointLoad();

        if(_pefile.isNETPresent())
        {
            _pefile.entryPointLoad_NET();
        }

        scriptpe.setData(&_pefile);

        engine.setData(&scriptpe,"PE",Utils::getDataBasePath(pOptions));

        die_handleSignatures(&engine,&(pOptions->listPEScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::die_scanELF(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(die_appendSignatureSignal(QString)));

    ELFFile _elfile;
    connect(&_elfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptELF scriptelf;
    connect(&scriptelf,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_elfile.setFileName(sFileName))
    {
        _elfile.entryPointLoad();

        scriptelf.setData(&_elfile);

        engine.setData(&scriptelf,"ELF",Utils::getDataBasePath(pOptions));

        die_handleSignatures(&engine,&(pOptions->listELFScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::die_scanMACH(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(die_appendSignatureSignal(QString)));

    MACHFile _machfile;
    connect(&_machfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptMACH scriptmach;
    connect(&scriptmach,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_machfile.setFileName(sFileName))
    {
        _machfile.entryPointLoad();

        scriptmach.setData(&_machfile);

        engine.setData(&scriptmach,"MACH",Utils::getDataBasePath(pOptions));

        die_handleSignatures(&engine,&(pOptions->listMACHScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::die_scanMSDOS(QString sFileName, QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(die_appendSignatureSignal(QString)));

    MSDOSFile _msdosfile;
    connect(&_msdosfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptMSDOS scriptmsdos;
    connect(&scriptmsdos,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_msdosfile.setFileName(sFileName))
    {
        _msdosfile.entryPointLoad();

        scriptmsdos.setData(&_msdosfile);

        engine.setData(&scriptmsdos,"MSDOS",Utils::getDataBasePath(pOptions));

        die_handleSignatures(&engine,&(pOptions->listMSDOSScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::die_scanText(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(die_appendSignatureSignal(QString)));

    TextFile _text;
    connect(&_text,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptText scripttext;
    connect(&scripttext,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_text.setFileName(sFileName))
    {
        scripttext.setData(&_text);

        engine.setData(&scripttext,"Text",Utils::getDataBasePath(pOptions));

        die_handleSignatures(&engine,&(pOptions->listTextScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::die_scanBinary(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(die_appendSignatureSignal(QString)));

    Binary _binary;
    connect(&_binary,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptBinary scriptbinary;
    connect(&scriptbinary,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_binary.setFileName(sFileName))
    {
        scriptbinary.setData(&_binary);

        engine.setData(&scriptbinary,"Binary",Utils::getDataBasePath(pOptions));

        die_handleSignatures(&engine,&(pOptions->listBinaryScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

void Scan::die_handleSignatures(PluginsScript *pluginScript, QList<__SIGNATURE> *pListSignatures, QString sType)
{
    int _nNumberOfSignatures=pListSignatures->count();

    for(int i=0; i<_nNumberOfSignatures; i++)
    {
        //                listSignatures.append(pOptions->listBinaryScripts.at(i));
        if(pListSignatures->at(i).sName=="_init")
        {
            pluginScript->evaluate(pListSignatures->at(i).sText,"_init");
            break;
        }
    }

    int k=0;
    bResult=false;

    bool bDeepScan=pOptions->bScanDeepScanDIE;

    for(int i=0; (i<_nNumberOfSignatures)&&(bIsRun); i++)
    {
        QString _sName=pListSignatures->at(i).sName;
        QString sPrefix=_sName.section(".",0,0);

        bool bSuccess=true;

        if(_sName=="_init")
        {
            bSuccess=false;
        }

        if((sPrefix=="EP")||(sPrefix=="DS"))
        {
            bSuccess=bDeepScan;
        }

        if(bSuccess)
        {
            QTime scanTime;

            if(pOptions->bShowScanTime)
            {
                scanTime=QTime::currentTime();
            }

            die_compareFile(pluginScript,pListSignatures->at(i).sText,pListSignatures->at(i).sName,sType);

            if(pOptions->bShowScanTime)
            {
                emit appendError(QString("%1: %2 ms").arg(_sName).arg(scanTime.msecsTo(QTime::currentTime())));
            }

            if(i+1>(_nNumberOfSignatures/30)*k)
            {
                emit setProgressBar(_nNumberOfSignatures,i+1);
                k++;
            }
        }
    }

    emit setProgressBar(1,1);

    if(bResult==false)
    {
        emit die_appendSignatureSignal(sType+": Nothing found");
    }
}

void Scan::loadTypeScripts(QList<__SIGNATURE> *pList, QString sType,__DIE_OPTIONS *pOptions)
{
    __SIGNATURE _signature;

    QList<__SIGNATURE> listSignatures;

    pList->clear();

    //    QString sPath=Utils::getApplicationPath();

    QDir dir(Utils::getDataBasePath(pOptions)+QDir::separator()+sType);
    QFileInfoList dircontent=dir.entryInfoList(QStringList()<<"*.sg"<<"_init");
    QFile file;
    QString sScript;

    for(int i=0; i<dircontent.count(); i++)
    {
        file.setFileName(dircontent.at(i).absoluteFilePath());

        sScript.clear();

        if(file.open(QIODevice::ReadOnly))
        {
            sScript.append(file.readAll());
            file.close();

            _signature.sName=dircontent.at(i).fileName();
            _signature.sText=sScript;

            listSignatures.append(_signature);
        }

        file.close();
    }

    for(int i=0; i<listSignatures.count(); i++)
    {
        if(listSignatures.at(i).sName=="_init")
        {
            pList->append(listSignatures.at(i));
            listSignatures.removeAt(i);
            break;
        }
    }

    QString sPriority;
    QString sName;
    int nCount=0;

    QSet<QString> set;

    for(int i=0; i<listSignatures.count(); i++)
    {
        sName=listSignatures.at(i).sName;
        nCount=sName.count(".");

        if(nCount>1)
        {
            sPriority=sName.section(".",nCount-1,nCount-1);
            set.insert(sPriority);
        }
    }

    QList<QString> values = set.values();
    qSort(values);

    for(int j=0; j<values.count(); j++)
    {
        for(int i=0; i<listSignatures.count(); i++)
        {
            sName=listSignatures.at(i).sName;
            nCount=sName.count(".");

            if(nCount>1)
            {
                sPriority=sName.section(".",nCount-1,nCount-1);

                if(sPriority==values.at(j))
                {
                    pList->append(listSignatures.at(i));
                    listSignatures.removeAt(i);
                    i--;
                }
            }
        }
    }

    pList->append(listSignatures);
}

void Scan::die_loadScripts(__DIE_OPTIONS *pOptions)
{
    loadTypeScripts(&pOptions->listBinaryScripts,"Binary",pOptions);
    loadTypeScripts(&pOptions->listTextScripts,"Text",pOptions);
    loadTypeScripts(&pOptions->listELFScripts,"ELF",pOptions);
    loadTypeScripts(&pOptions->listMSDOSScripts,"MSDOS",pOptions);
    loadTypeScripts(&pOptions->listPEScripts,"PE",pOptions);
    loadTypeScripts(&pOptions->listMACHScripts,"MACH",pOptions);
}

QString Scan::toJSON(__DIE_OPTIONS *pOptions)
{
    QString sResult;

    QJsonObject jsonResult;

    jsonResult.insert("filename",pOptions->sCurrentFileName);

    if(pOptions->bShowEntropy)
    {
        jsonResult.insert("entropy",QString::number(pOptions->dEntropy));
    }

    QJsonArray jsArray;

    for(int i=0;i<pOptions->die_listResult.count();i++)
    {
        QJsonObject jsRecord;

        jsRecord.insert("name",pOptions->die_listResult.at(i).sName);
        jsRecord.insert("filetype",pOptions->die_listResult.at(i).sFileType);
        jsRecord.insert("type",pOptions->die_listResult.at(i).sType);

        jsArray.append(jsRecord);
    }

    jsonResult.insert("detects",jsArray);

    QJsonDocument saveFormat(jsonResult);

    sResult=saveFormat.toJson(QJsonDocument::Indented).data();

    return sResult;
}
#ifdef USE_YARA
void Scan::yara_loadBase(__DIE_OPTIONS *pOptions)
{
    QFile file;
    file.setFileName(Utils::getDataBaseYARAPath(pOptions));

    if(file.open(QIODevice::ReadOnly))
    {
        pOptions->sDataBaseYARA=file.readAll();

        file.close();
    }
}
#endif
void Scan::die_appendSignatureSlot(QString sString)
{
    //qDebug(sString.toLatin1().data());

    if((sString!="")&&(!sString.contains("Nothing found")))
    {
        __DIE_RESULT record;
        QString sTemp;

        if(sString.contains(";"))
        {
            record.sSignature=sString.section(";",0,0);
            sTemp=sString.section(";",1,-1);
        }
        else
        {
            sTemp=sString;
        }

        record.sFileType=sTemp.section(": ",0,0);
        record.sType=sTemp.section(": ",1,1);
        record.sName=sTemp.section(": ",2,-1);

        pOptions->die_listResult.append(record);
    }
}

QString Scan::die_compareFile(PluginsScript *pScript,QString sScript,QString sScriptName,QString sType)
{
    QScriptValueList args;
    QString sResult="";

    args<<true<<pOptions->bScanShowVersionDIE<<pOptions->bScanShowOptionsDIE;

    QScriptValue result=pScript->call(sScript,"detect",args,sScriptName);

    if(result.toString()!="")
    {
        //        pOptions->nNumberOfResults++;
        QString sSignature;

        if(bShowSource)
        {
            sSignature=sScriptName+";";
        }

        sSignature+=sType+": "+result.toString();

        emit die_appendSignatureSignal(sSignature);

        bResult=true;

        sResult=result.toString();
    }

    return sResult;
}

//void Scan::_compareFile(QString sScript)
//{
//    compareFile(pScript,sScript);
//}
