#include "handleoptions.h"



HandleOptions::HandleOptions()
{
}

void HandleOptions::save(__DIE_OPTIONS *pOptions)
{
    QString sIniFile=Utils::getApplicationPath()+QDir::separator()+"die.ini";
    QSettings settings(sIniFile,QSettings::IniFormat);

    settings.setValue(__StayOnTop,pOptions->bStayOnTop);
    settings.setValue(__ScanAfterOpen,pOptions->bScanAfterOpen);
    settings.setValue(__ScanShowVersionDIE,pOptions->bScanShowVersionDIE);
    settings.setValue(__ScanShowOptionsDIE,pOptions->bScanShowOptionsDIE);
    settings.setValue(__ScanDeepScanDIE,pOptions->bScanDeepScanDIE);
    settings.setValue(__ScanResizeColumnsToContent,pOptions->bScanResizeToContentsDIE);
    settings.setValue(__ScanMethod,(int)pOptions->sm);
#ifdef USE_NFD
    settings.setValue(__ScanDeepScanNFD,pOptions->bScanDeepScanNFD);
    settings.setValue(__ScanScanRecursiveNFD,pOptions->bScanRecursiveNFD);
#endif
    settings.setValue(__ShowTooltips,pOptions->bShowTooltips);
    settings.setValue(__ShowErrors,pOptions->bShowErrors);
    settings.setValue(__ShowScanTime,pOptions->bShowScanTime);
    settings.setValue(__SectionsEntropyAfterOpen,pOptions->bSectionsEntropyAfterOpen);
    settings.setValue(__SingleApplication,pOptions->bSingleApplication);
    settings.setValue(__SaveLastDirectory,pOptions->bSaveLastDirectory);
    settings.setValue(__LastDirectory,pOptions->sLastDirectory);
    settings.setValue(__ResizeableMainWindow,pOptions->bResizeableMainWindow);
    settings.setValue(__Threshold,pOptions->fThreshold);
    settings.setValue(__EntropyGraph,pOptions->nEntropyGraph);
    settings.setValue(__Style,pOptions->sStyle);
    settings.setValue(__StyleSheet,pOptions->sStyleSheet);
    settings.setValue(__EnableLanguage,pOptions->bEnableLanguage);
    settings.setValue(__Language,pOptions->sLanguage);
    settings.setValue(__Codec,pOptions->sCodec);
    settings.setValue(__EnableFont,pOptions->bEnableFont);
    settings.setValue(__Font,pOptions->sFont);
    settings.setValue(__FontHEX,pOptions->sFontHEX);
    settings.setValue(__AddressWidthHEX,pOptions->nAddressWidth);
    settings.setValue(__Endianness,pOptions->sEndianness);
    settings.setValue(__EnablePlugins,pOptions->bEnablePlugins);

    settings.setValue(__DataBasePath,pOptions->sDataBasePath);
    settings.setValue(__EditorPath,pOptions->sEditorPath);
    settings.setValue(__HelpPath,pOptions->sHelpPath);
    settings.setValue(__InfoPath,pOptions->sInfoPath);
    settings.setValue(__PluginsPath,pOptions->sPluginsPath);
    settings.setValue(__QSSPath,pOptions->sQSSPath);
    settings.setValue(__LangsPath,pOptions->sLangsPath);
    settings.setValue(__ScriptsPath,pOptions->sScriptsPath);
    settings.setValue(__SearchPath,pOptions->sSearchPath);
#ifdef USE_YARA
    settings.setValue(__DataBaseYARAPath,pOptions->sDataBaseYARAPath);
#endif
}

void HandleOptions::load(__DIE_OPTIONS *pOptions)
{
    QString sIniFile=Utils::getApplicationPath()+QDir::separator()+"die.ini";
    QSettings settings(sIniFile,QSettings::IniFormat);

    pOptions->bStayOnTop=settings.value(__StayOnTop,false).toBool();
    pOptions->bScanAfterOpen=settings.value(__ScanAfterOpen,true).toBool();
    pOptions->bScanShowVersionDIE=settings.value(__ScanShowVersionDIE,true).toBool();
    pOptions->bScanShowOptionsDIE=settings.value(__ScanShowOptionsDIE,true).toBool();
    pOptions->bScanDeepScanDIE=settings.value(__ScanDeepScanDIE,false).toBool();
    pOptions->bScanResizeToContentsDIE=settings.value(__ScanResizeColumnsToContent,false).toBool();
    pOptions->sm=(SCAN_METHODS)settings.value(__ScanMethod,(int)SM_DIE).toInt();
#ifdef USE_NFD
    pOptions->bScanDeepScanNFD=settings.value(__ScanDeepScanNFD,true).toBool();
    pOptions->bScanRecursiveNFD=settings.value(__ScanScanRecursiveNFD,true).toBool();
#endif
    pOptions->bShowTooltips=settings.value(__ShowTooltips,true).toBool();
    pOptions->bShowErrors=settings.value(__ShowErrors,false).toBool();
    pOptions->bShowScanTime=settings.value(__ShowScanTime,false).toBool();
    pOptions->bSectionsEntropyAfterOpen=settings.value(__SectionsEntropyAfterOpen,true).toBool();
    pOptions->bSingleApplication=settings.value(__SingleApplication,true).toBool();
    pOptions->bSaveLastDirectory=settings.value(__SaveLastDirectory,true).toBool();
    QString sLastDirectory=settings.value(__LastDirectory,"").toString();

    if(sLastDirectory!="")
    {
        QDir dir;
        dir.setPath(sLastDirectory);

        if(!dir.exists())
        {
            sLastDirectory="";
        }
    }

    pOptions->sLastDirectory=sLastDirectory;

    pOptions->bResizeableMainWindow=settings.value(__ResizeableMainWindow,false).toBool();
    pOptions->fThreshold=settings.value(__Threshold,7).toDouble();
    pOptions->nEntropyGraph=settings.value(__EntropyGraph,100).toUInt();
#ifdef Q_OS_WIN
    pOptions->sStyle=settings.value(__Style,"Fusion").toString();
    pOptions->bEnableFont=settings.value(__EnableFont,true).toBool();
    pOptions->sFont=settings.value(__Font,"MS Shell Dlg 2,8,-1,5,50,0,0,0,0,0").toString();
    pOptions->sFontHEX=settings.value(__FontHEX,"Courier,8,-1,5,50,0,0,0,0,0").toString();
#endif
#ifdef Q_OS_MAC
    pOptions->sStyle=settings.value(__Style,"Fusion").toString();
    pOptions->bEnableFont=settings.value(__EnableFont,false).toBool();
    pOptions->sFont=settings.value(__Font,"").toString();
    pOptions->sFontHEX=settings.value(__FontHEX,"").toString();
#endif
#ifdef Q_OS_LINUX
    pOptions->sStyle=settings.value(__Style,"Fusion").toString();
    pOptions->bEnableFont=settings.value(__EnableFont,false).toBool();
    pOptions->sFont=settings.value(__Font,"").toString();
    pOptions->sFontHEX=settings.value(__FontHEX,"").toString();
#endif
    pOptions->sStyleSheet=settings.value(__StyleSheet,"orange").toString();
    pOptions->bEnableLanguage=settings.value(__EnableLanguage,false).toBool();
    pOptions->sLanguage=settings.value(__Language,"").toString();
    pOptions->sCodec=settings.value(__Codec,"").toString();
    pOptions->nAddressWidth=settings.value(__AddressWidthHEX,8).toUInt();
    pOptions->sEndianness=settings.value(__Endianness,"Big Endian").toString();
    pOptions->bEnablePlugins=settings.value(__EnablePlugins,false).toBool();
    pOptions->sDataBasePath=settings.value(__DataBasePath,__DefaultDataBasePath).toString();
    pOptions->sEditorPath=settings.value(__EditorPath,__DefaultEditorPath).toString();
    pOptions->sHelpPath=settings.value(__HelpPath,__DefaultHelpPath).toString();
    pOptions->sInfoPath=settings.value(__InfoPath,__DefaultInfoPath).toString();
    pOptions->sPluginsPath=settings.value(__PluginsPath,__DefaultPluginsPath).toString();
    pOptions->sQSSPath=settings.value(__QSSPath,__DefaultQSSPath).toString();
    pOptions->sLangsPath=settings.value(__LangsPath,__DefaultLangsPath).toString();
    pOptions->sScriptsPath=settings.value(__ScriptsPath,__DefaultScriptsPath).toString();
    pOptions->sSearchPath=settings.value(__SearchPath,__DefaultSearchPath).toString();
#ifdef USE_YARA
    pOptions->sDataBaseYARAPath=settings.value(__DataBaseYARAPath,__DefaultDataBaseYARAPath).toString();
#endif
}
