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
#ifndef SCAN_H
#define SCAN_H

#include <QCoreApplication>
//#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QThread>
#include <QDir>
#include <QScriptEngine>
#include <QFileInfoList>
#include <QSettings>
#include <QTime>

#include "../pluginsscript.h"
#include "scriptpe.h"
#include "scriptbinary.h"
#include "scriptelf.h"
#include "scriptmsdos.h"
#include "scriptmach.h"
#include "scripttext.h"
#include "_options.h"
#include "utils.h"

#define DIE_SHOWERRORS              0x00000001
#define DIE_SHOWOPTIONS             0x00000002
#define DIE_SHOWVERSION             0x00000004
#define DIE_SHOWENTROPY             0x00000008
#define DIE_SINGLELINEOUTPUT        0x00000010
#define DIE_SHOWFILEFORMATONCE      0x00000020
#define DIE_FULLSCAN                0x00000040
#define DIE_DEEPSCAN                0x00000080
#define DIE_SHOWJSON                0x00000100

class Scan : public QObject
{
    Q_OBJECT
public:
    explicit Scan(QObject *parent=nullptr);
    void setData(QString sScanFileName,__DIE_OPTIONS *pOptions);
    void setData(QList<QString> *dirContent,__DIE_OPTIONS *pOptions);
    void setData(__DIE_OPTIONS *pOptions);
    void setShowSource(bool bShowSource);
    bool isRun();
    bool isCompleted();
    void setRun(bool bIsRun);
    bool analize(QString sFileName,bool bFullScan);
    bool die_scanPE(QString sFileName,QString sPrefix);
    bool die_scanELF(QString sFileName,QString sPrefix);
    bool die_scanMACH(QString sFileName,QString sPrefix);
    bool die_scanMSDOS(QString sFileName,QString sPrefix);
    bool die_scanText(QString sFileName,QString sPrefix);
    bool die_scanBinary(QString sFileName,QString sPrefix);
    void die_handleSignatures(PluginsScript *pluginScript,QList<__SIGNATURE> *pListSignatures,QString sType);
    static void loadTypeScripts(QList<__SIGNATURE> *pList,QString sType,__DIE_OPTIONS *pOptions);
    static void die_loadScripts(__DIE_OPTIONS *pOptions);
    //    void _compareFile(QString sScript);
    QString toJSON(__DIE_OPTIONS *pOptions);
#ifdef USE_YARA
    static void yara_loadBase(__DIE_OPTIONS *pOptions);
#endif
signals:
    void die_appendSignatureSignal(const QString &sString);
    void setProgressBar(int nMax,int nValue);
    void setProgressBar2(int nMax,int nValue);
    void appendError(QString sText);
    void appendFileName(QString sText);
    void appendExtra(QString sText);
    void singleScanComplete(int nTime);
    void _finished();

private slots:
    void die_appendSignatureSlot(QString sString);

public slots:
    void process();
    void _stop();

private:
    bool bShowSource;
    QString sScanFileName;
    __DIE_OPTIONS *pOptions;
    QList<QString> *dirContent;
    int nNumberOfSignatures;
    bool bIsRun;
    bool bIsCompleted;
    bool bResult;

    //    PluginsScript *pScript;
    QString sFileName;
    QString die_compareFile(PluginsScript *pScript,QString sScript,QString sScriptName,QString sType);
};

#endif // SCAN_H
