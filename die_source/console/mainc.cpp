#include <QCoreApplication>
#include <QtCore>
#include "../scan.h"
#include "../_options.h"
#include "_mainclass.h"
#include "../singlefilescan.h"

void FindFiles(QString sDirectoryName,QStringList *pList,bool bSubDirs)
{
    QDir dir;
    dir.setPath(sDirectoryName);

    QFileInfoList fi=dir.entryInfoList();

    for(int i=0; i<fi.count(); i++)
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
        }

        if(fi.at(i).isDir()&&(bSubDirs))
        {
            FindFiles(fi.at(i).absoluteFilePath(),pList,bSubDirs);
        }
    }
}

bool ParseOptions(__DIE_OPTIONS *pOptions,QStringList *pListArguments,QStringList *pListFiles)
{
    QString sParam;
    QString sTemp;

    QStringList listTemp1;

    for(int i=1; i<pListArguments->count(); i++)
    {
        sParam=pListArguments->at(i);

        if(sParam.toLower().contains("-showerrors:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bShowErrors=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bShowErrors=false;
            }
            else
            {
                printf("Invalid parameter: %s",sParam.toAscii().data());
                return false;
            }

        }
        else if(sParam.toLower().contains("-showoptions:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bShowOptions=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bShowOptions=false;
            }
            else
            {
                printf("Invalid parameter: %s",sParam.toAscii().data());
                return false;
            }
        }
        else if(sParam.toLower().contains("-showversion:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bShowVersion=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bShowVersion=false;
            }
            else
            {
                printf("Invalid parameter: %s",sParam.toAscii().data());
                return false;
            }
        }
        else if(sParam.toLower().contains("-subfolders:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bScanSubfolders=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bScanSubfolders=false;
            }
            else
            {
                printf("Invalid parameter: %s",sParam.toAscii().data());
                return false;
            }
        }
        else if(sParam.toLower().contains("-showentropy:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bShowEntropy=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bShowEntropy=false;
            }
            else
            {
                printf("Invalid parameter: %s",sParam.toAscii().data());
                return false;
            }
        }
        else if(sParam.toLower().contains("-singlelineoutput:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bSingleLineOutput=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bSingleLineOutput=false;
            }
            else
            {
                printf("Invalid parameter %s",sParam.toAscii().data());
                return false;
            }
        }
        else if(sParam.toLower().contains("-showfileformatonce:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bShowFileFormatOnce=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bShowFileFormatOnce=false;
            }
            else
            {
                printf("Invalid parameter: %s",sParam.toAscii().data());
                return false;
            }
        }
        else if(sParam.toLower().contains("-fullscan:"))
        {
            sTemp=sParam.section(":",1,1).toLower();

            if(sTemp=="yes")
            {
                pOptions->bFullScan=true;
            }
            else if(sTemp=="no")
            {
                pOptions->bFullScan=false;
            }
            else
            {
                printf("Invalid parameter: %s",sParam.toAscii().data());
                return false;
            }
        }
        else if(sParam.toLower().contains("-database:"))
        {
            sTemp=sParam.section(":",1,-1).toLower();

            QDir _dir;

            if(_dir.exists(Utils::convertPath(sTemp)))
            {
                pOptions->sDataBasePath=sTemp;
            }
            else
            {
                printf("Invalid database name: %s",sTemp.toAscii().data());
                return false;
            }
        }
        else
        {
            listTemp1.append(sParam);
        }
    }

    if(listTemp1.count()==0)
    {
        printf("No file specified");

        return false;
    }

    QFile file;
    QDir dir;

    //    QDir dir2;
    //    int nNumberOfSlash=0;
    //    QStringList listTemp2;
    //    QFileInfoList fil;
    //    QString sBaseName;
    for(int i=0; i<listTemp1.count(); i++)
    {
        file.setFileName(listTemp1.at(i));
        dir.setPath(listTemp1.at(i));

        if(listTemp1.at(i)==".")
        {
            continue;
        }
        else if(listTemp1.at(i)=="..")
        {
            continue;
        }
        else if(dir.exists())
        {
            FindFiles(listTemp1.at(i),pListFiles,pOptions->bScanSubfolders);
        }
        else if(file.exists())
        {
            pListFiles->append(listTemp1.at(i));
        }
        else if(listTemp1.at(i).contains("*")||listTemp1.at(i).contains("?"))
        {
            QStringList nameFilters;
            nameFilters.append(listTemp1.at(i));
            QDir _dir;
            QFileInfoList listFI=_dir.entryInfoList(nameFilters);


            for(int j=0; j<listFI.count(); j++)
            {
                if(listFI.at(j).isDir())
                {
                    FindFiles(listFI.at(j).absoluteFilePath(),pListFiles,pOptions->bScanSubfolders);
                }
                else if(listFI.at(j).isFile())
                {
                    pListFiles->append(listFI.at(j).absoluteFilePath());
                }
            }
        }
        else
        {
            printf("Invalid parameter or file name: %s",listTemp1.at(i).toAscii().data());
            return false;
        }

        //        if(listTemp1.at(i).contains("*")||listTemp1.at(i).contains("?"))
        //        {
        //            sTemp=listTemp1.at(i);
        //            sTemp.replace("/",QDir::separator());
        //            sTemp.replace("\\",QDir::separator());

        //            nNumberOfSlash=sTemp.count(QDir::separator());

        //            if(nNumberOfSlash)
        //            {
        //                dir2.setPath(sTemp.section(QDir::separator(),0,nNumberOfSlash-1)+QDir::separator());
        //                sTemp=sTemp.section(QDir::separator(),nNumberOfSlash);
        //            }
        //            fil=dir2.entryInfoList(QStringList()<<sTemp);

        //            for(int j=0;j<fil.count();j++)
        //            {
        //                sBaseName=fil.at(j).baseName();
        //                if((sBaseName!=".")&&(sBaseName!="..")&&(sBaseName!=""))
        //                {
        //                    listTemp2.append(fil.at(j).absoluteFilePath());
        //                }
        //            }
        //        }
        //        else
        //        {
        //            listTemp2.append(listTemp1.at(i));
        //        }
    }

    //    QDir _dir;
    //    QFileInfoList listFI=_dir.entryInfoList(nameFilters);


    ////    listAnother2.append(dir2.entryList(listFilters));
    //    for(int i=0;i<listTemp2.count();i++)
    //    {
    //        file.setFileName(listTemp2.at(i));
    //        dir.setPath(listTemp2.at(i));

    //        if(listTemp2.at(i)==".")
    //        {
    //            continue;
    //        }
    //        else if(listTemp2.at(i)=="..")
    //        {
    //            continue;
    //        }
    //        else if(dir.exists())
    //        {
    //            FindFiles(listTemp2.at(i),pListFiles,pOptions->bScanSubfolders);
    //        }
    //        else if(file.exists())
    //        {
    //            pListFiles->append(listTemp2.at(i));
    //        }
    //        else
    //        {
    //            printf("Invalid parameter or file name: %s",listTemp2.at(i).toAscii().data());
    //            return false;
    //        }
    //    }

    return true;
}

void loadOptions(__DIE_OPTIONS *pOptions)
{
    QSettings settings(Utils::getApplicationPath()+QDir::separator()+"diec.ini",QSettings::IniFormat);

    pOptions->bShowErrors=settings.value("ShowErrors",false).toBool();
    pOptions->bShowOptions=settings.value("ShowOptions",true).toBool();
    pOptions->bShowVersion=settings.value("ShowVersion",true).toBool();
    pOptions->bScanSubfolders=settings.value("ScanSubfolders",true).toBool();
    pOptions->bShowEntropy=settings.value("ShowEntropy",false).toBool();
    pOptions->bSingleLineOutput=settings.value("SingleLineOutput",false).toBool();
    pOptions->bShowFileFormatOnce=settings.value("ShowFileFormatOnce",false).toBool();
    pOptions->bFullScan=settings.value("FullScan",false).toBool();
    pOptions->sDataBasePath=settings.value("DataBase","$app/db").toString();
    pOptions->bShowScanTime=false;
}

int getNumberOfScripts(QList<__SIGNATURE> *pList)
{
    int nResult=0;

    for(int i=0; i<pList->count(); i++)
    {
        if(pList->at(i).sName!="_init")
        {
            nResult++;
        }
    }

    return nResult;
}

QMap<QString,int> getSubTypes(QList<__SIGNATURE> *pList)
{
    QMap<QString,int> mapResult;

    for(int i=0; i<pList->count(); i++)
    {
        if(pList->at(i).sName!="_init")
        {
            QString sName=pList->at(i).sName;

            if(sName.count(".")>=3)
            {
                QString sType=sName.section(".",0,0);

                if((sType!="")&&(!sType.contains(' ')))
                {
                    mapResult.insert(sType,mapResult.value(sType)+1);
                }
            }
        }
    }

    return mapResult;
}

int printSignatures(QString sType,QList<__SIGNATURE> *pList)
{
    int nResult=getNumberOfScripts(pList);
    printf("Number of %s signatures: %d\n",sType.toAscii().data(),getNumberOfScripts(pList));

    QMap<QString,int> mapSubtypes=getSubTypes(pList);

    QMapIterator<QString, int> i(mapSubtypes);

    while(i.hasNext())
    {
        i.next();
        printf("\tNumber of %s signatures: %d\n",i.key().toAscii().data(),i.value());
    }

    return nResult;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //    __DIE_OPTIONS options;
    QStringList arguments=QCoreApplication::arguments();
    // Default options
    //    options.bShowErrors=true;
    //    options.bShowOptions=true;
    //    options.bShowVersion=true;
    //    options.bScanSubfolders=true;
    //    options.bShowEntropy=false;
    //    options.bShowScanTime=false;
#ifdef QT_DEBUG
    //    arguments.append("/home/trix/progs/xntsv32/xntsv32.exe");
    ////    arguments.append("-database:$app/help");
    //////    arguments.append("-singlelineoutput:yes");
    //   // arguments.append("-showfileformatonce:no");
    //    arguments.append("-showentropy:yes");
    //   // arguments.append("-fullscan:yes");
#endif

    loadOptions(&SingleFileScan::options);
    //    // fill scripts
    Scan::loadScripts(&SingleFileScan::options);

    bool bInvalidBase=(SingleFileScan::options.listBinaryScripts.count()==0)
                      &&(SingleFileScan::options.listELFScripts.count()==0)
                      &&(SingleFileScan::options.listMACHScripts.count()==0)
                      &&(SingleFileScan::options.listMSDOSScripts.count()==0)
                      &&(SingleFileScan::options.listPEScripts.count()==0)
                      &&(SingleFileScan::options.listTextScripts.count()==0);


    if(arguments.count()==1)
    {
        printf(__DIE);
        printf(" ");
        printf(__VERSION);
        printf(" console\n");
        printf("\n");
        printf("Copyright(C) 2006-2008 Hellsp@wn 2012-2016 Hors\n");
        printf("Website: http://ntinfo.biz\n");
        printf("Bugreports: horsicq@gmail.com\n");
        printf("\n");
        int nTotal=0;
        nTotal+=printSignatures("Binary",&SingleFileScan::options.listBinaryScripts);
        nTotal+=printSignatures("Text",&SingleFileScan::options.listTextScripts);
        nTotal+=printSignatures("MSDOS",&SingleFileScan::options.listMSDOSScripts);
        nTotal+=printSignatures("PE/PE+",&SingleFileScan::options.listPEScripts);
        nTotal+=printSignatures("ELF/ELF64",&SingleFileScan::options.listELFScripts);
        nTotal+=printSignatures("MACH/MACH64",&SingleFileScan::options.listMACHScripts);
        printf("Total signatures: %d\n",nTotal);
        //        printf("Number of Binary signatures: %d\n",getNumberOfScripts(&options.listBinaryScripts));
        //        printf("Number of Text signatures: %d\n",getNumberOfScripts(&options.listTextScripts));
        //        printf("Number of MSDOS signatures: %d\n",getNumberOfScripts(&options.listMSDOSScripts));
        //        printf("Number of PE/PE+ signatures: %d\n",getNumberOfScripts(&options.listPEScripts));
        //        printf("Number of ELF/ELF64 signatures: %d\n",getNumberOfScripts(&options.listELFScripts));
        //        printf("Number of MACH/MACH64 signatures: %d\n",getNumberOfScripts(&options.listMACHScripts));
        printf("\n");
        printf("Usage: DIEC file/directory [options]\n");
        printf("\n");
        printf("Options:\n");
        printf(QString("-showerrors [default: -showerrors:%1] Show all errors\n").arg(SingleFileScan::options.bShowErrors?"yes":"no").toAscii().data());
        printf(QString("-showoptions [default: -showoptions:%1] Show compiler/protector/packer/etc options\n").arg(SingleFileScan::options.bShowOptions?"yes":"no").toAscii().data());
        printf(QString("-showversion [default: -showversion:%1] Show compiler/protector/packer/etc version\n").arg(SingleFileScan::options.bShowVersion?"yes":"no").toAscii().data());
        printf(QString("-subfolders [default: -subfolders:%1] Scan subfolders. Only for directory scan\n").arg(SingleFileScan::options.bScanSubfolders?"yes":"no").toAscii().data());
        printf(QString("-showentropy [default: -showentropy:%1] Show entropy\n").arg(SingleFileScan::options.bShowEntropy?"yes":"no").toAscii().data());
        printf(QString("-singlelineoutput [default: -singlelineoutput:%1] Output as single line\n").arg(SingleFileScan::options.bSingleLineOutput?"yes":"no").toAscii().data());
        printf(QString("-showfileformatonce [default: -showfileformatonce:%1] Show file format once\n").arg(SingleFileScan::options.bShowFileFormatOnce?"yes":"no").toAscii().data());
        printf(QString("-fullscan [default: -fullscan:%1] Full scan for all valid types\n").arg(SingleFileScan::options.bFullScan?"yes":"no").toAscii().data());
        printf(QString("-database [default: -database:\"%1\"] Set scan database\n").arg(SingleFileScan::options.sDataBasePath).toAscii().data());
    }
    else if(arguments.count()>1)
    {
        QStringList listFiles;

        if(!ParseOptions(&SingleFileScan::options,&arguments,&listFiles))
        {
            return 0;
        }

        _MainClass mc(&a);

        mc.setOptions(&SingleFileScan::options);

        if(listFiles.count()!=1)
        {
            mc.handleMessage(QString("Number of files: %1").arg(listFiles.count()));
            printf("\n");
        }

        SingleFileScan sfs;

        //        sfs.setOptions(&options);

        for(int i=0; i<listFiles.count(); i++)
        {
            if(listFiles.count()!=1)
            {
                printf("%s",listFiles.at(i).toAscii().data());

                if(SingleFileScan::options.bSingleLineOutput)
                {
                    printf(": ");
                }
                else
                {
                    printf("\n");
                }
            }

            QString sResult=sfs.process(listFiles.at(i));

            printf("%s",sResult.toAscii().data());

            if(i!=listFiles.count()-1)
            {
                printf("\n");
            }
        }
    }

    if(bInvalidBase)
    {
        printf("Invalid signatures database! Please download signatures from https://github.com/horsicq/Detect-It-Easy/tree/master/db and put it to the application path.\n");
    }

    return 0;
    //    return a.exec();
}
