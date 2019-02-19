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
#ifndef UTILS_H
#define UTILS_H

#include "_options.h"
#include <QDir>
#include <QCoreApplication>

class Utils
{
public:
    Utils();

    static QString getApplicationPath();
    static QString getPath(QString sFileName);
    static QString getLastDirecory(__DIE_OPTIONS *pOptions);

    static QString convertPath(QString sPath);

    static QString getDataBasePath(__DIE_OPTIONS *pOptions);
    static QString getEditorPath(__DIE_OPTIONS *pOptions);
    static QString getHelpPath(__DIE_OPTIONS *pOptions);
    static QString getInfoPath(__DIE_OPTIONS *pOptions);
    static QString getPluginsPath(__DIE_OPTIONS *pOptions);
    static QString getQSSPath(__DIE_OPTIONS *pOptions);
    static QString getLangsPath(__DIE_OPTIONS *pOptions);
    static QString getScriptsPath(__DIE_OPTIONS *pOptions);
    static QString getSearchPath(__DIE_OPTIONS *pOptions);
#ifdef USE_YARA
    static QString getDataBaseYARAPath(__DIE_OPTIONS *pOptions);
#endif
    static QString getDefaultFont();
};

#endif // UTILS_H
