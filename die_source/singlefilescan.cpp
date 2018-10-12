#include "singlefilescan.h"

static QCoreApplication* app=0;
static int argc = 0;

SingleFileScan::SingleFileScan(QObject *parent):
    QObject(parent)
{
    //    sDataBase="###[unknown]###";
}

//void SingleFileScan::setOptions(__DIE_OPTIONS *pOptions)
//{
//    this->pOptions=pOptions;

//    Scan::loadScripts(pOptions);
//}

void SingleFileScan::flagsToOptions(unsigned int nFlags, __DIE_OPTIONS *pOptions)
{
    pOptions->bShowErrors=nFlags&DIE_SHOWERRORS;
    pOptions->bScanShowOptionsDIE=nFlags&DIE_SHOWOPTIONS;
    pOptions->bScanShowVersionDIE=nFlags&DIE_SHOWVERSION;
    pOptions->bScanSubfolders=false;
    pOptions->bShowEntropy=nFlags&DIE_SHOWENTROPY;
    pOptions->bSingleLineOutput=nFlags&DIE_SINGLELINEOUTPUT;
    pOptions->bShowFileFormatOnce=nFlags&DIE_SHOWFILEFORMATONCE;
    pOptions->bFullScan=nFlags&DIE_FULLSCAN;
    pOptions->bShowScanTime=false;
    pOptions->sm=SM_DIE;
    pOptions->pMutexResult=0;
}

QString SingleFileScan::process(QString sFileName)
{
    sResult.clear();

    Scan scan;

    QObject::connect(&scan,SIGNAL(die_appendSignatureSignal(QString)),this,SLOT(appendSignature(QString)),Qt::DirectConnection);

    if(options.bShowErrors)
    {
        QObject::connect(&scan,SIGNAL(appendError(QString)),this,SLOT(appendError(QString)),Qt::DirectConnection);
    }

    scan.setData(&options);
    scan.setRun(true);

    scan.analize(sFileName,options.bFullScan);

    if(options.bShowEntropy)
    {
        float fEntropy=Binary::calculateEntropy(sFileName);
        appendMessage(QString("Entropy: %1").arg(fEntropy));
    }

    scan.setRun(false);

    return sResult;
}

QString SingleFileScan::firstBytes(QString sFileName,unsigned int nSize)
{
    QByteArray baResult;
    QString sType=Binary::getType(sFileName);

    if((sType=="PE+(64)")||(sType=="PE"))
    {
        PEFile file;

        if(file.setFileName(sFileName))
        {
            file.entryPointLoad();
            baResult=file.readArrayFromEntryPoint(nSize);
        }
    }
    else if((sType=="ELF")||(sType=="ELF64"))
    {
        ELFFile file;

        if(file.setFileName(sFileName))
        {
            file.entryPointLoad();
            baResult=file.readArrayFromEntryPoint(nSize);
        }
    }
    else if((sType=="MACH")||(sType=="MACH64"))
    {
        MACHFile file;

        if(file.setFileName(sFileName))
        {
            file.entryPointLoad();
            baResult=file.readArrayFromEntryPoint(nSize);
        }
    }
    else if(sType=="MSDOS")
    {
        MSDOSFile file;

        if(file.setFileName(sFileName))
        {
            file.entryPointLoad();
            baResult=file.readArrayFromEntryPoint(nSize);
        }
    }
    else
    {
        Binary file;

        if(file.setFileName(sFileName))
        {
            file.entryPointLoad();
            baResult=file.readArrayFromEntryPoint(nSize);
        }
    }

    return baResult.toHex().data();
}

void SingleFileScan::setDataBase(QString sDataBase)
{
    if(sDataBase=="")
    {
        sDataBase=__DefaultDataBasePath;
    }

    options.sDataBasePath=sDataBase;
    Scan::die_loadScripts(&options);
#ifdef QT_DEBUG
    qDebug("New database: %s",sDataBase.toLatin1().data());
#endif
    
}

__DIE_OPTIONS SingleFileScan::options={};

void SingleFileScan::appendMessage(QString sMessage)
{
    if((sResult!="")&&(options.bSingleLineOutput))
    {
        sResult+="; ";
    }

    sResult+=sMessage;

    if(options.bSingleLineOutput)
    {

    }
    else
    {
        sResult+="\n";
    }
}

void SingleFileScan::appendSignature(QString sMessage)
{
    QString sType=sMessage.section(": ",0,0);

    if((options.bShowFileFormatOnce)&&(sType==sCurrentType))
    {
        sMessage=sMessage.section(": ",1,-1);
    }
    else
    {
        sCurrentType=sType;
    }

    appendMessage(sMessage);
}

void SingleFileScan::appendError(QString sMessage)
{
    appendMessage(sMessage);
}

#ifdef Q_WS_WIN
bool  WINAPI  _DllMain(_In_  HINSTANCE hinstDLL,_In_  DWORD fdwReason,_In_  LPVOID lpvReserved)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            if(app)
            {
                delete app;
            }

            break;
        }
    }

    return(TRUE);
}
#endif

int __DIE_scan(QString szFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags,QString sDataBase)
{
    SingleFileScan sfs;

    //    __DIE_OPTIONS options;

    //    options.sDataBasePath=sDataBase;
    sfs.flagsToOptions(nFlags,&sfs.options);
    sfs.setDataBase(sDataBase);
    //    sfs.setOptions(&options);

    QString sResult=sfs.process(szFileName);

    int nSize=sResult.size();

    if(nSize<nOutBufferSize)
    {
        pszOutBuffer[nSize-1]=0;
        strcpy(pszOutBuffer,sResult.toLatin1().data());
    }
    else
    {
        pszOutBuffer[nOutBufferSize-1]=0;
        strncpy(pszOutBuffer,sResult.toLatin1().data(),nOutBufferSize-1);
    }

    return nSize;
}

int __DIE_scanExA(char *pszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, char *pszDataBase)
{
    if(QCoreApplication::instance()==0)
    {
        app = new QCoreApplication(argc, NULL);
    }

    return __DIE_scan(pszFileName,pszOutBuffer,nOutBufferSize,nFlags,pszDataBase);
}

int __DIE_scanExW(wchar_t *pwszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, wchar_t *pwszDataBase)
{
    if(QCoreApplication::instance()==0)
    {
        app = new QCoreApplication(argc, NULL);
    }

    QString sFileName;
    sFileName=QString::fromWCharArray(pwszFileName);
    QString sDataBase;
    sDataBase=QString::fromWCharArray(pwszDataBase);

    return __DIE_scan(sFileName,pszOutBuffer,nOutBufferSize,nFlags,sDataBase);
}

int __DIE_scanA(char *pszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags)
{
    return __DIE_scanExA(pszFileName,pszOutBuffer,nOutBufferSize,nFlags,0);
}

int __DIE_scanW(wchar_t *pwszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags)
{
    return __DIE_scanExW(pwszFileName,pszOutBuffer,nOutBufferSize,nFlags,0);
}

char *__DIE_versionA(void)
{
    return (char *)__VERSION;
}

wchar_t *__DIE_versionW(void)
{
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
    return WIDEN(__VERSION);
}
