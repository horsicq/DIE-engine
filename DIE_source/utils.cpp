#include "utils.h"

Utils::Utils()
{
}

QString Utils::getApplicationPath()
{
    QString sResult;
    sResult=qApp->applicationDirPath();
#ifndef QT_NO_DEBUG
#ifdef Q_WS_WIN
    sResult="F:\\prepare\\qt\\DIE_source";
#endif
#ifdef Q_OS_MAC
    sResult="/Volumes/DATA/prepare/qt/DIE_source";
#endif
#ifdef Q_OS_LINUX
    sResult="/home/trix/prepare/qt/DIE_source";
#endif
#endif

    return sResult;
}

QString Utils::getPath(QString sFileName)
{
    QFileInfo fi;
    fi.setFile(sFileName);
//    fi.path();
    return fi.path();
}

QString Utils::getLastDirecory(__DIE_OPTIONS *pOptions)
{
    if(pOptions->sLastDirectory=="")
    {
        pOptions->sLastDirectory=getApplicationPath();
    }

    return pOptions->sLastDirectory;
}

QString Utils::convertPath(QString sPath)
{
    if(sPath.contains("$app"))
    {
        sPath=sPath.replace("$app",getApplicationPath()).replace("/",QDir::separator());
    }

    return sPath;
}

QString Utils::getDataBasePath(__DIE_OPTIONS *pOptions)
{
    if(pOptions->sDataBasePath=="")
    {
        pOptions->sDataBasePath=__DefaultDataBasePath;
    }

    return convertPath(pOptions->sDataBasePath);
}

QString Utils::getEditorPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sEditorPath);
}

QString Utils::getHelpPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sHelpPath);
}

QString Utils::getInfoPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sInfoPath);
}

QString Utils::getPluginsPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sPluginsPath);
}

QString Utils::getQSSPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sQSSPath);
}

QString Utils::getLangsPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sLangsPath);
}

QString Utils::getScriptsPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sScriptsPath);
}

QString Utils::getSearchPath(__DIE_OPTIONS *pOptions)
{
    return convertPath(pOptions->sSearchPath);
}

QString Utils::getDefaultFont()
{

    return "";
}
