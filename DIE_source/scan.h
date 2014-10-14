#ifndef SCAN_H
#define SCAN_H


#include <QCoreApplication>
#include <QtConcurrentMap>
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

class Scan : public QObject
{
    Q_OBJECT
public:
    explicit Scan(QObject *parent = 0);
    void setData(QString sScanFileName,__DIE_OPTIONS *pOptions);
    void setData(QList<QString> *dirContent,__DIE_OPTIONS *pOptions);
    void setData(__DIE_OPTIONS *pOptions);
    void setShowSource(bool bShowSource);

    bool isRun();
    bool isCompleted();
    void setRun(bool bIsRun);

    bool analize(QString sFileName);
    static void loadTypeScripts(QList<__SIGNATURE> *pList,QString sType,__DIE_OPTIONS *pOptions);
    static void loadScripts(__DIE_OPTIONS *pOptions);
//    void _compareFile(QString sScript);
signals:
    void appendSignature(const QString &sString);
    void setProgressBar(int nMax,int nValue);
    void setProgressBar2(int nMax,int nValue);
    void appendError(QString sText);
    void appendFileName(QString sText);
    void appendExtra(QString sText);
    void _finished();

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
    QString sType;
//    PluginsScript *pScript;
    QString sFileName;
    QString compareFile(PluginsScript *pScript,QString sScript,QString sScriptName);
};

#endif // SCAN_H
