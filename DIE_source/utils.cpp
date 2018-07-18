// Copyright (c) 2012-2018 hors<horsicq@gmail.com>
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
    sResult="C:\\tmp_build\\qt5\\die\\die_source";
#endif
#ifdef Q_OS_MAC
    sResult="/Volumes/DATA/prepare/qt/DIE_source";
#endif
#ifdef Q_OS_LINUX
    sResult="/home/trix/ownCloud/prepare/qt/DIE_source";
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
