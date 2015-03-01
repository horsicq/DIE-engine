#include "scan.h"

//using namespace QtConcurrent;

Scan::Scan(QObject *parent) :
    QObject(parent)
{

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
        for(int i=0;(i<dirContent->count())&&(bIsRun);i++)
        {
            analize(dirContent->at(i),pOptions->bFullScan);
            emit appendSignature("");

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
    pOptions->nNumberOfResults=0;

    emit appendFileName(sFileName);

    Binary file;

    PluginsScript engine;

    connect(&file,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

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

    for(int i=0;i<nCount;i++)
    {
        QString sType=listTypes.at(i);
        if((sType=="PE")||(sType=="PE+(64)"))
        {
            scanPE(sFileName,sType);
        }
        else if((sType=="ELF")||(sType=="ELF64"))
        {
            scanELF(sFileName,sType);
        }
        else if(sType=="MSDOS")
        {
            scanMSDOS(sFileName,sType);
        }
        else if((sType=="MACH")||(sType=="MACH64"))
        {
            scanMACH(sFileName,sType);
        }
        else if(sType=="Text")
        {
            scanText(sFileName,sType);
        }
        else if(sType=="Binary")
        {
            scanBinary(sFileName,sType);
        }
    }


}

bool Scan::scanPE(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

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

        handleSignatures(&engine,&(pOptions->listPEScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::scanELF(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

    ELFFile _elfile;
    connect(&_elfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptELF scriptelf;
    connect(&scriptelf,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_elfile.setFileName(sFileName))
    {
        _elfile.entryPointLoad();

        scriptelf.setData(&_elfile);

        engine.setData(&scriptelf,"ELF",Utils::getDataBasePath(pOptions));

        handleSignatures(&engine,&(pOptions->listELFScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }

}

bool Scan::scanMACH(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

    MACHFile _machfile;
    connect(&_machfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptMACH scriptmach;
    connect(&scriptmach,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_machfile.setFileName(sFileName))
    {
        _machfile.entryPointLoad();

        scriptmach.setData(&_machfile);

        engine.setData(&scriptmach,"MACH",Utils::getDataBasePath(pOptions));

        handleSignatures(&engine,&(pOptions->listMACHScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::scanMSDOS(QString sFileName, QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

    MSDOSFile _msdosfile;
    connect(&_msdosfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptMSDOS scriptmsdos;
    connect(&scriptmsdos,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_msdosfile.setFileName(sFileName))
    {
        _msdosfile.entryPointLoad();

        scriptmsdos.setData(&_msdosfile);

        engine.setData(&scriptmsdos,"MSDOS",Utils::getDataBasePath(pOptions));

        handleSignatures(&engine,&(pOptions->listMSDOSScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::scanText(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

    TextFile _text;
    connect(&_text,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptText scripttext;
    connect(&scripttext,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_text.setFileName(sFileName))
    {
        scripttext.setData(&_text);

        engine.setData(&scripttext,"Text",Utils::getDataBasePath(pOptions));

        handleSignatures(&engine,&(pOptions->listTextScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

bool Scan::scanBinary(QString sFileName,QString sPrefix)
{
    PluginsScript engine;
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

    Binary _binary;
    connect(&_binary,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptBinary scriptbinary;
    connect(&scriptbinary,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(_binary.setFileName(sFileName))
    {
        scriptbinary.setData(&_binary);

        engine.setData(&scriptbinary,"Binary",Utils::getDataBasePath(pOptions));

        handleSignatures(&engine,&(pOptions->listBinaryScripts),sPrefix);

        return true;
    }
    else
    {
        emit setProgressBar(1,1);
        return false;
    }
}

void Scan::handleSignatures(PluginsScript *pluginScript, QList<__SIGNATURE> *pListSignatures, QString sType)
{
    int _nNumberOfSignatures=pListSignatures->count();

    for(int i=0;i<_nNumberOfSignatures;i++)
    {
//                listSignatures.append(pOptions->listBinaryScripts.at(i));
        if(pListSignatures->at(i).sName=="_init")
        {
            pluginScript->evaluate (pListSignatures->at(i).sText,"_init");
            break;
        }
    }

    int k=0;
    bResult=false;

    for(int i=0;(i<_nNumberOfSignatures)&&(bIsRun);i++)
    {
        if(pListSignatures->at(i).sName!="_init")
        {
            QTime scanTime;
            if(pOptions->bShowScanTime)
            {
                scanTime=QTime::currentTime();
            }

            compareFile(pluginScript,pListSignatures->at(i).sText,pListSignatures->at(i).sName,sType);


            if(pOptions->bShowScanTime)
            {
                emit appendError(QString("%1: %2 ms").arg(pListSignatures->at(i).sName).arg(scanTime.msecsTo(QTime::currentTime())));
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
        emit appendSignature(sType+": Nothing found");
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

    for(int i=0;i<dircontent.count();i++)
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

    for(int i=0;i<listSignatures.count();i++)
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

    for(int i=0;i<listSignatures.count();i++)
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

    for(int j=0;j<values.count();j++)
    {
        for(int i=0;i<listSignatures.count();i++)
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

void Scan::loadScripts(__DIE_OPTIONS *pOptions)
{
    loadTypeScripts(&pOptions->listBinaryScripts,"Binary",pOptions);
    loadTypeScripts(&pOptions->listTextScripts,"Text",pOptions);
    loadTypeScripts(&pOptions->listELFScripts,"ELF",pOptions);
    loadTypeScripts(&pOptions->listMSDOSScripts,"MSDOS",pOptions);
    loadTypeScripts(&pOptions->listPEScripts,"PE",pOptions);
    loadTypeScripts(&pOptions->listMACHScripts,"MACH",pOptions);
}

QString Scan::compareFile(PluginsScript *pScript,QString sScript,QString sScriptName,QString sType)
{
    QScriptValueList args;
    QString sResult="";

    args<<true<<pOptions->bShowVersion<<pOptions->bShowOptions;

    QScriptValue result=pScript->call(sScript,"detect",args,sScriptName);

    if(result.toString()!="")
    {
        pOptions->nNumberOfResults++;
        QString sSignature;
        if(bShowSource)
        {
            sSignature=sScriptName+";";
        }
        sSignature+=sType+": "+result.toString();

        emit appendSignature(sSignature);

        bResult=true;

        sResult=result.toString();
    }

    return sResult;
}

//void Scan::_compareFile(QString sScript)
//{
//    compareFile(pScript,sScript);
//}
