#include "pluginsscript.h"

//QByteArray *pbaDataGlobal;
//__SD *sdGlobal;


QScriptValue includeScript(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue ret(false);

    if(engine == 0)
    {
        return ret;
    }

    QString fileName = context->argument(0).toString();

    fileName=((PluginsScript *)engine)->getDefaultPath()+QDir::separator()+fileName;

    QFile file(fileName);

    if(file.open(QFile::ReadOnly))
    {
        QByteArray line = file.readAll();
        file.close();
        QString contents = QString::fromUtf8(line);
        engine->currentContext()->setActivationObject(engine->currentContext()->parentContext()->activationObject());
        ret = engine->evaluate(contents, fileName);
    }
    else
    {
        ((PluginsScript *)engine)->_appendError(QString("Cannot open file: %1").arg(fileName));
    }

    return ret;
}

QScriptValue _log(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue ret(true);

    ((PluginsScript *)engine)->_appendError(context->argument(0).toString());

    return ret;
}

QScriptValue _message(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue ret(true);

    ((PluginsScript *)engine)->_appendMessage(context->argument(0).toString());

    return ret;
}

//QString _getString(QByteArray *pbaData,int nOffset,int nSize)
//{
//    //PE _pe(pbaData->data(),pbaData->size());
//    PE _pe(pbaData->data(),pbaData->size());
//    QByteArray baTemp=_pe.getArray(nOffset,nSize);
//    baTemp.append('\0');

//    return QString(baTemp.data());
//}



//QString _getCompilerVersion(QByteArray *baData)
//{
//    //PE _pe(pbaData->data(),pbaData->size());
//    PE _pe(baData->data(),baData->size());
//    QString sResult;
//    int nMajor=_pe.getOptionalHeader_MajorLinkerVersion();
//    int nMinor=_pe.getOptionalHeader_MinorLinkerVersion();

//    sResult=QString("%1.%2").arg(nMajor).arg(nMinor);

//    return sResult;
//}
//QString _getGeneralOptions(QByteArray *baData)
//{
//    //PE _pe(pbaData->data(),pbaData->size());
//    PE _pe(baData->data(),baData->size());
//    QString sResult;

//    bool bIs64=_pe.isPEPlus();
//    bool bIsDll=_pe.isDll();
//    bool bIsDriver=_pe.isDriver();

//    QString sType;
//    QString sBits;

//    if(bIs64)
//    {
//        sBits="64";
//    }
//    else
//    {
//        sBits="32";
//    }

//    if(bIsDll)
//    {
//        sType="DLL";
//    }
//    else if(bIsDriver)
//    {
//        sType="Driver";
//    }
//    else
//    {
//        sType="EXE";
//    }

//    sResult=QString("%1%2").arg(sType).arg(sBits);

//    return sResult;
//}

//bool _compare(char *pOffset,char *pSignature,int nSize)
//{
//    for(int i=0;i<nSize;i++)
//    {
//        if(pSignature[i]!='.')
//        {
//            if(pOffset[i]!=pSignature[i])
//            {
//                return false;
//            }

//        }
//    }
//    return true;
//}
//bool __compareEP(QByteArray *pbaData,QString sSignature)
//{

//    QByteArray baSignature;
//    QByteArray baTemp;
//    //PE _pe(pbaData->data(),pbaData->size());
//    PE _pe(pbaData->data(),pbaData->size());
//    int nSize=0;
//    char *pOffset=0;
//    char *pSignature=0;

//    int nSignatureSize=sSignature.length()/2;
//    baSignature=_pe.getArray(_pe.RVAToOffset(_pe.getOptionalHeader_AddressOfEntryPoint()),nSignatureSize).toHex().toUpper();
//    nSize=baSignature.size();
//    pOffset=baSignature.data();
//    baTemp=sSignature.toAscii();
//    pSignature=baTemp.data();

//    if(nSize==sSignature.length())
//    {
//        return _compare(pOffset,pSignature,nSize);
//    }

//    return false;

//}
//bool _compareEP(QByteArray *pbaData,QString sSignature)
//{
//    QByteArray baSignature;
//    QByteArray baTemp;
//    QByteArray baTemp2;

//    char *pOffset=0;
//    char *pSignature=0;

//    int nSignatureSize=sSignature.length()/2;


//    if(sdGlobal->nEntryPointOffset+nSignatureSize<=sdGlobal->nSize)
//    {
//        //baSignature=_pe.getArray(_pe.RVAToOffset(_pe.getOptionalHeader_AddressOfEntryPoint()),nSignatureSize).toHex().toUpper();

//        pOffset=sdGlobal->baData->data();
//        pOffset+=sdGlobal->nEntryPointOffset;
//        baSignature.append(pOffset,nSignatureSize*2);
//        baTemp2=baSignature.toHex().toUpper();
//        baTemp=sSignature.toAscii();
//        pSignature=baTemp.data();
//        pOffset=baTemp2.data();

//        return _compare(pOffset,pSignature,nSignatureSize*2);
//    }

//    return false;

//}
//bool _compareHeader(QByteArray *pbaData,QString sSignature)
//{
//    QByteArray baSignature;
//    QByteArray baTemp;
//    //PE _pe(pbaData->data(),pbaData->size());
//    PE _pe(pbaData->data(),pbaData->size());
//    int nSize=0;
//    char *pOffset=0;
//    char *pSignature=0;

//    int nSignatureSize=sSignature.length()/2;
//    baSignature=_pe.getHeaders();
//    baSignature.resize(nSignatureSize);
//    baSignature=baSignature.toHex().toUpper();
//    nSize=baSignature.size();
//    pOffset=baSignature.data();
//    baTemp=sSignature.toAscii();
//    pSignature=baTemp.data();

//    if(nSize==sSignature.length())
//    {
//        return _compare(pOffset,pSignature,nSize);
//    }

//    return false;

//}
//bool _compareCodeSection(QByteArray *pbaData,QString sSignature)
//{
//    QByteArray baTemp;
//    QByteArray baEntryPointSection;

//    //PE _pe(pbaData->data(),pbaData->size());
//    PE _pe(pbaData->data(),pbaData->size());
//    char *pOffset=0;
//    char *pSignature=0;

//    baTemp=sSignature.toAscii();
//    pSignature=baTemp.data();

//    int nSignatureSize=sSignature.length()/2;
//    baEntryPointSection=_pe.getEntryPointSection();
//    pOffset=baEntryPointSection.data();

//    for(int i=0;i<baEntryPointSection.size()-nSignatureSize;i++)
//    {

//        if(_compare(pOffset+i,pSignature,nSignatureSize))
//        {
//            return true;
//        }
//    }


//    return false;
//}

//QScriptValue compareEP(QScriptContext *context, QScriptEngine *engine)
//{
//    QString sSignature = context->argument(0).toString();
//    return QScriptValue(engine, _compareEP(pbaDataGlobal,sSignature));
//}

//QScriptValue compareCodeSection(QScriptContext *context, QScriptEngine *engine)
//{
//    QString sSignature = context->argument(0).toString();
//    return QScriptValue(engine, _compareCodeSection(pbaDataGlobal,sSignature));
//}

//QScriptValue getGeneralOptions(QScriptContext *context, QScriptEngine *engine)
//{

//    return QScriptValue(engine, _getGeneralOptions(pbaDataGlobal));
//}
//QScriptValue getCompilerVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    return QScriptValue(engine, _getCompilerVersion(pbaDataGlobal));
//}
//QScriptValue getImportFunctionName(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    int nImport= context->argument(0).toInt32();
//    int nFunctionNumber= context->argument(1).toInt32();
//    return QScriptValue(engine, _pe.getImportFunctionName(nImport,nFunctionNumber));
//}
//QScriptValue getNumberOfImportThunks(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    int nImport= context->argument(0).toInt32();
//    return QScriptValue(engine, _pe.getNumberOfImportThunks(nImport));
//}
//QScriptValue isDll(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.isDll());
//}
//QScriptValue getSizeOfUninitializedData(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.getOptionalHeader_SizeOfUninitializedData());
//}
//QScriptValue getSizeOfInitializedData(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.getOptionalHeader_SizeOfInitializedData());
//}
//QScriptValue getSizeOfCode(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.getOptionalHeader_SizeOfCode());
//}
//QScriptValue getNumberOfSections(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.getFileHeader_NumberOfSections());
//}
//QScriptValue getFileAlignment(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.getOptionalHeader_FileAlignment());
//}
//QScriptValue getBaseOfCode(QScriptContext *context, QScriptEngine *engine)
//{
//    //PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.getOptionalHeader_BaseOfCode());
//}
//QScriptValue isRichSignaturePresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    return QScriptValue(engine, _pe.isRichSignaturePresent());
//}
//QScriptValue getString(QScriptContext *context, QScriptEngine *engine)
//{
//    int nOffset= context->argument(0).toInt32();
//    int nSize= context->argument(1).toInt32();
//    return QScriptValue(engine,_getString(pbaDataGlobal,nOffset,nSize));
//}
//QScriptValue getSectionName(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSectionNameAsString(nSectionNumber));
//}
//QScriptValue getSectionVirtualAddress(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_VirtualAddress(nSectionNumber));
//}
//QScriptValue getSectionVirtualSize(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_VirtualSize(nSectionNumber));
//}
//QScriptValue getSectionFileOffset(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_PointerToRawData(nSectionNumber));
//}
//QScriptValue getSectionFileSize(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_SizeOfRawData(nSectionNumber));
//}
//QScriptValue getSectionPointerToRelocations(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_PointerToRelocations(nSectionNumber));
//}
//QScriptValue getSectionPointerToLinenumbers(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_PointerToLinenumbers(nSectionNumber));
//}
//QScriptValue getSectionNumberOfLinenumbers(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_NumberOfLinenumbers(nSectionNumber));
//}
//QScriptValue getSectionNumberOfRelocations(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_NumberOfRelocations(nSectionNumber));
//}
//QScriptValue getSectionCharacteristics(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    int nSectionNumber= context->argument(0).toInt32();
//    return QScriptValue(engine,_pe.getSection_Characteristics(nSectionNumber));
//}
//QScriptValue compareImportFunctionsArray(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    QString sSignature = context->argument(0).toString();

//    QByteArray baData=_pe.getArray(_pe.getImportTableOffset()+_pe.getImportTableSize(),sSignature.size()).toHex().toUpper();
//    QByteArray baTemp=sSignature.toAscii();

//    return QScriptValue(engine,_compare(baData.data(),baTemp.data(),sSignature.size()));
//}
//QScriptValue compareHeader(QScriptContext *context, QScriptEngine *engine)
//{
//    QString sSignature = context->argument(0).toString();
//    return QScriptValue(engine, _compareHeader(pbaDataGlobal,sSignature));
//}

//QScriptValue isSectionNamePresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    QString sSectionName = context->argument(0).toString();

//    for(int i=0;i<_pe.getFileHeader_NumberOfSections();i++)
//    {
//        if(_pe.getSectionNameAsString(i)==sSectionName)
//        {
//            return QScriptValue(engine,true);
//        }
//    }

//    return QScriptValue(engine,false);
//}

//QScriptValue isResourceNamePresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    QString sResourceName = context->argument(0).toString();


//    return QScriptValue(engine,_pe.isResourceNamePresent(sResourceName));
//}

//QScriptValue isDosStubPresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.isDosStubPresent());
//}

//QScriptValue getMachine(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getFileHeader_Machine());
//}

//QScriptValue getTimeDateStamp(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getFileHeader_TimeDateStamp());
//}
//QScriptValue getNumberOfSymbols(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getFileHeader_NumberOfSymbols());
//}
//QScriptValue getPointerToSymbolTable(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getFileHeader_PointerToSymbolTable());
//}
//QScriptValue getSizeOfOptionalHeader(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getFileHeader_SizeOfOptionalHeader());
//}
//QScriptValue getCharacteristics(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getFileHeader_Characteristics());
//}
//QScriptValue getOptionalHeaderMagic(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_Magic());
//}

//QScriptValue getMajorLinkerVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,(unsigned int)_pe.getOptionalHeader_MajorLinkerVersion());
//}
//QScriptValue getMinorLinkerVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,(unsigned int)_pe.getOptionalHeader_MinorLinkerVersion());
//}

//QScriptValue getAddressOfEntryPoint(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_AddressOfEntryPoint());
//}

//QScriptValue getBaseOfData(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_BaseOfData());
//}
//QScriptValue isPEPlus(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.isPEPlus());
//}
//QScriptValue isDriver(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.isDriver());
//}
//QScriptValue getImageBase(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_ImageBase());
//}
//QScriptValue getSectionAlignment(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_SectionAlignment());
//}
//QScriptValue getMajorOperatingSystemVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_MajorOperatingSystemVersion());
//}
//QScriptValue getMinorOperatingSystemVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_MinorOperatingSystemVersion());
//}
//QScriptValue getMajorImageVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_MajorImageVersion());
//}
//QScriptValue getMinorImageVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_MinorImageVersion());
//}
//QScriptValue getMajorSubsystemVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_MajorSubsystemVersion());
//}
//QScriptValue getMinorSubsystemVersion(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_MinorSubsystemVersion());
//}
//QScriptValue getWin32VersionValue(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_Win32VersionValue());
//}
//QScriptValue getSizeOfImage(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_SizeOfImage());
//}
//QScriptValue getSizeOfHeaders(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_SizeOfHeaders());
//}
//QScriptValue getCheckSum(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_CheckSum());
//}
//QScriptValue getSubsystem(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_Subsystem());
//}
//QScriptValue getDllCharacteristics(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_DllCharacteristics());
//}
//QScriptValue getSizeOfStackReserve(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_SizeOfStackReserve());
//}
//QScriptValue getSizeOfStackCommit(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_SizeOfStackCommit());
//}
//QScriptValue getSizeOfHeapReserve(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_SizeOfHeapReserve());
//}
//QScriptValue getSizeOfHeapCommit(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_SizeOfHeapCommit());
//}
//QScriptValue getLoaderFlags(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_LoaderFlags());
//}
//QScriptValue getNumberOfRvaAndSizes(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.getOptionalHeader_NumberOfRvaAndSizes());
//}

//QScriptValue isOverlayPresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.isOverlayPresent());
//}

//QScriptValue isDirectoryPresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());
//    int nDirectory = context->argument(0).toInt32();

//    return QScriptValue(engine,_pe.isDirectoryPresent(nDirectory));
//}
//QScriptValue isImportPresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.isImportPresent());
//}
//QScriptValue isExportPresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.isExportPresent());
//}
//QScriptValue isResourcePresent(QScriptContext *context, QScriptEngine *engine)
//{
//    PE _pe(pbaDataGlobal->data(),pbaDataGlobal->size());

//    return QScriptValue(engine,_pe.isResourcePresent());
//}

//QScriptValue test(QScriptContext *context, QScriptEngine *engine)
//{
//    return QScriptValue(engine,false);
//}

PluginsScript::PluginsScript(QObject *parent) :
    QScriptEngine(parent)
{
    addFunction(includeScript,"includeScript");
    addFunction(_log,"_log");
    addFunction(_message,"_message");
    //    binary=0;

    //    addClass(&scriptpe,"PE");
    //    connect(&scriptpe,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));
}

void PluginsScript::setData(QObject *pClass,QString sClassName,QString sDefaultPath)
{
    //    this->sDefaultPath=sDefaultPath;
    addClass(pClass,sClassName,sDefaultPath);
    //    emit appendError(sClassName);
}

QScriptValue PluginsScript::callFromFile(QString sScriptFile, QString sFunctionName, QScriptValueList valuelist)
{
    QFile file;
    QString sScriptData;

    file.setFileName(sScriptFile);

    if(file.open(QIODevice::ReadOnly))
    {
        sScriptData.append(file.readAll());
        file.close();

        return call(sScriptData,sFunctionName,valuelist);
    }
    else
    {
        emit appendError(QString("Cannot open file: %1").arg(sScriptFile));
    }

    return 0;
}

QScriptValue PluginsScript::evalFromFile(QString sScriptFile)
{
    QFile file(sScriptFile);
    QScriptValue ret;

    if(file.open(QFile::ReadOnly))
    {
        QByteArray line = file.readAll();
        file.close();
        QString contents = QString::fromUtf8(line);
        ret = evaluate(contents, sScriptFile);
    }
    else
    {
        _appendError(QString("Cannot open file: %1").arg(sScriptFile));
    }

    return ret;
}

QScriptValue PluginsScript::call(QString sScript, QString sFunctionName, QScriptValueList valuelist,QString sExtra)
{
    // TODO check if function
    QScriptValue script = evaluate(sScript);

    if(!script.isError())
    {
        QScriptValue detect=globalObject().property(sFunctionName);

        if(!detect.isError())
        {

            QScriptValue result=detect.call(script,valuelist);

            if(!result.isError())
            {
                return result;
            }
            else
            {
                if(sExtra!="")
                {
                    emit appendError(QString("%1: %2").arg(sExtra).arg(result.toString()));
                }
                else
                {
                    emit appendError(QString("%1").arg(result.toString()));
                }
            }
        }
        else
        {
            if(sExtra!="")
            {
                emit appendError(QString("%1: %2").arg(sExtra).arg(detect.toString()));
            }
            else
            {
                emit appendError(QString("%1").arg(detect.toString()));
            }
        }
    }
    else
    {
        if(sExtra!="")
        {
            emit appendError(QString("%1: %2").arg(sExtra).arg(script.toString()));
        }
        else
        {
            emit appendError(QString("%1").arg(script.toString()));
        }
    }

    return 0;
}

bool PluginsScript::isFunctionPresentInFile(QString sScriptFile, QString sFunctionName)
{
    QFile file;
    QString sScriptData;

    file.setFileName(sScriptFile);

    if(file.open(QIODevice::ReadOnly))
    {
        sScriptData.append(file.readAll());
        file.close();

        return isFunctionPresent(sScriptData,sFunctionName);
    }
    else
    {
        emit appendError(QString("Cannot open file: %1").arg(sScriptFile));
    }

    return false;
}

bool PluginsScript::isFunctionPresent(QString sScript, QString sFunctionName)
{
    QScriptValue script = evaluate(sScript);

    if(!script.isError())
    {
        QScriptValue detect=globalObject().property(sFunctionName);

        if(!detect.isError())
        {
            return detect.isFunction();
        }
    }
    else
    {
        emit appendError(QString("%1").arg(script.toString()));
    }

    return false;
}


void PluginsScript::addFunction(QScriptEngine::FunctionSignature function, QString sFunctionName)
{
    QScriptValue func = this->newFunction(function);
    this->globalObject().setProperty(sFunctionName, func);
}

void PluginsScript::addClass(QObject *pClass, QString sClassName,QString sDefaultPath)
{
    this->sDefaultPath=sDefaultPath;
    QScriptValue objectWnd = this->newQObject(pClass);
    this->globalObject().setProperty(sClassName, objectWnd);
}

void PluginsScript::_appendError(QString sText)
{
    emit appendError(sText);
}

void PluginsScript::_appendMessage(QString sText)
{
    emit appendMessage(sText);
}

QString PluginsScript::getDefaultPath()
{
    return sDefaultPath;
}

