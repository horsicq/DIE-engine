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
        analize(sScanFileName);
    }
    else if(dirContent)
    {
        emit setProgressBar2(1,0);
        for(int i=0;(i<dirContent->count())&&(bIsRun);i++)
        {
            analize(dirContent->at(i));
            emit appendSignature("");

            emit setProgressBar2(dirContent->count(),i+1);
        }

        emit setProgressBar2(1,1);

    }

    emit _finished();

    bIsCompleted=true;
    bIsRun=false;
}

bool Scan::analize(QString sFileName)
{
    pOptions->nNumberOfResults=0;

    emit appendFileName(sFileName);
    QList<QString> listSignatures;
    QList<QString> listSignaturesNames;
    QString s_Init;

    Binary file;

    PluginsScript engine;
    int k=0;

    connect(&file,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    connect(&engine,SIGNAL(appendMessage(QString)),this,SIGNAL(appendSignature(QString)));

    PEFile _pefile;
    connect(&_pefile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptPE scriptpe;
    connect(&scriptpe,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    ELFFile _elfile;
    connect(&_elfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptELF scriptelf;
    connect(&scriptelf,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    MSDOSFile _msdosfile;
    connect(&_msdosfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptMSDOS scriptmsdos;
    connect(&scriptmsdos,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    MACHFile _machfile;
    connect(&_machfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptMACH scriptmach;
    connect(&scriptmach,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    Binary _binary;
    connect(&_binary,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptBinary scriptbinary;
    connect(&scriptbinary,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    TextFile _text;
    connect(&_text,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
    scriptText scripttext;
    connect(&scripttext,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    sType="";
    if(file.setFileName(sFileName))
    {
        sType=file.getType();
        //qDebug(sType.toAscii().data());
        file.close();
    }
    else
    {
        file.close();

        return false;
    }

    bResult=false;

    QList<__SIGNATURE> *pListSignatures;

    if((sType=="PE")||(sType=="PE+(64)"))
    {
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

//            nNumberOfSignatures=pOptions->listPEScripts.count();
//            for(int i=0;i<nNumberOfSignatures;i++)
//            {
//                listSignaturesNames.append(pOptions->listPEScripts.at(i).sName);
//                listSignatures.append(pOptions->listPEScripts.at(i).sText);
//            }
            pListSignatures=&(pOptions->listPEScripts);
        }
        else
        {
            emit setProgressBar(1,1);
            return false;
        }

    }
    else if((sType=="ELF")||(sType=="ELF64"))
    {
        if(_elfile.setFileName(sFileName))
        {
            _elfile.entryPointLoad();

            scriptelf.setData(&_elfile);

            engine.setData(&scriptelf,"ELF",Utils::getDataBasePath(pOptions));
//            emit appendError("Test");

//            nNumberOfSignatures=pOptions->listELFScripts.count();
//            for(int i=0;i<nNumberOfSignatures;i++)
//            {
////                listSignatures.append(pOptions->listELFScripts.at(i));
//                listSignaturesNames.append(pOptions->listELFScripts.at(i).sName);
//                listSignatures.append(pOptions->listELFScripts.at(i).sText);
//            }
            pListSignatures=&(pOptions->listELFScripts);
        }
        else
        {
            emit setProgressBar(1,1);
            return false;
        }

    }
    else if(sType=="MSDOS")
    {
        if(_msdosfile.setFileName(sFileName))
        {
            _msdosfile.entryPointLoad();

            scriptmsdos.setData(&_msdosfile);

            engine.setData(&scriptmsdos,"MSDOS",Utils::getDataBasePath(pOptions));
//            emit appendError("Test");


//            nNumberOfSignatures=pOptions->listMSDOSScripts.count();
//            for(int i=0;i<nNumberOfSignatures;i++)
//            {
////                listSignatures.append(pOptions->listMSDOSScripts.at(i));
//                listSignaturesNames.append(pOptions->listMSDOSScripts.at(i).sName);
//                listSignatures.append(pOptions->listMSDOSScripts.at(i).sText);
//            }
            pListSignatures=&(pOptions->listMSDOSScripts);
        }
        else
        {
            emit setProgressBar(1,1);
            return false;
        }

    }
    else if((sType=="MACH")||(sType=="MACH64"))
    {
        if(_machfile.setFileName(sFileName))
        {
            _machfile.entryPointLoad();

            scriptmach.setData(&_machfile);

            engine.setData(&scriptmach,"MACH",Utils::getDataBasePath(pOptions));
//            emit appendError("Test");

//            nNumberOfSignatures=pOptions->listMACHScripts.count();
//            for(int i=0;i<nNumberOfSignatures;i++)
//            {
//                listSignaturesNames.append(pOptions->listMACHScripts.at(i).sName);
//                listSignatures.append(pOptions->listMACHScripts.at(i).sText);
//            }
            pListSignatures=&(pOptions->listMACHScripts);
        }
        else
        {
            emit setProgressBar(1,1);
            return false;
        }

    }
    else if(sType=="Text")
    {
        if(_text.setFileName(sFileName))
        {
            scripttext.setData(&_text);

            engine.setData(&scripttext,"Text",Utils::getDataBasePath(pOptions));

//            nNumberOfSignatures=pOptions->listTextScripts.count();
//            for(int i=0;i<nNumberOfSignatures;i++)
//            {
////                listSignatures.append(pOptions->listBinaryScripts.at(i));
//                listSignaturesNames.append(pOptions->listTextScripts.at(i).sName);
//                listSignatures.append(pOptions->listTextScripts.at(i).sText);
//            }
            pListSignatures=&(pOptions->listTextScripts);
        }
        else
        {
            emit setProgressBar(1,1);
            return false;
        }
    }
    else if(sType=="Binary")
    {
        if(_binary.setFileName(sFileName))
        {
            scriptbinary.setData(&_binary);

            engine.setData(&scriptbinary,"Binary",Utils::getDataBasePath(pOptions));

//            nNumberOfSignatures=pOptions->listBinaryScripts.count();
//            for(int i=0;i<nNumberOfSignatures;i++)
//            {
////                listSignatures.append(pOptions->listBinaryScripts.at(i));
//                listSignaturesNames.append(pOptions->listBinaryScripts.at(i).sName);
//                listSignatures.append(pOptions->listBinaryScripts.at(i).sText);
//            }
            pListSignatures=&(pOptions->listBinaryScripts);
        }
        else
        {
            emit setProgressBar(1,1);
            return false;
        }
    }


    nNumberOfSignatures=pListSignatures->count();

    for(int i=0;i<nNumberOfSignatures;i++)
    {
//                listSignatures.append(pOptions->listBinaryScripts.at(i));
        if(pListSignatures->at(i).sName=="_init")
        {
            s_Init=pListSignatures->at(i).sText;
        }
        else
        {
            listSignaturesNames.append(pListSignatures->at(i).sName);
            listSignatures.append(pListSignatures->at(i).sText);
        }

    }



    if(s_Init!="")
    {
//        engine.currentContext()->setActivationObject(engine.currentContext()->parentContext()->activationObject());
        engine.evaluate (s_Init,"_init");
    }

    nNumberOfSignatures=listSignatures.count();

    for(int i=0;(i<nNumberOfSignatures)&&(bIsRun);i++)
    {
        if(listSignaturesNames.at(i)!="_init")
        {
            QTime scanTime;
            if(pOptions->bShowScanTime)
            {
                scanTime=QTime::currentTime();
            }

            compareFile(&engine,listSignatures.at(i),listSignaturesNames.at(i));


            if(pOptions->bShowScanTime)
            {
                emit appendError(QString("%1: %2 ms").arg(listSignaturesNames.at(i)).arg(scanTime.msecsTo(QTime::currentTime())));
            }


            if(i+1>(nNumberOfSignatures/30)*k)
            {
                emit setProgressBar(nNumberOfSignatures,i+1);
                k++;
            }
        }

    }

    emit setProgressBar(1,1);

    if(bResult==false)
    {
        emit appendSignature(sType+": Nothing found");
    }


    return bResult;
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

QString Scan::compareFile(PluginsScript *pScript,QString sScript,QString sScriptName)
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
