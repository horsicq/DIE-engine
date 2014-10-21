#ifndef PLUGINSSCRIPT_H
#define PLUGINSSCRIPT_H

#include <QScriptEngine>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include "DIE_source/utils.h"
//#include "../_pe.h"
//#include "scriptpe.h"
//#include "scriptbinary.h"
//#include "scriptelf.h"
//#include "scriptmsdos.h"
////#include "dialogoptions.h"
//#include <QTime>

//struct __SD
//{
//    QByteArray *baData;
//    unsigned int nEntryPointOffset;
//    unsigned int nSize;
//};

class PluginsScript : public QScriptEngine
{
    Q_OBJECT
public:
    explicit PluginsScript(QObject *parent = 0);
    void setData(QObject *pClass,QString sClassName,QString sDefaultPath);
    QScriptValue callFromFile(QString sScriptFile,QString sFunctionName,QScriptValueList valuelist=QScriptValueList());
    QScriptValue evalFromFile(QString sScriptFile);
    QScriptValue call(QString sScript,QString sFunctionName,QScriptValueList valuelist,QString sExtra="");
    bool isFunctionPresentInFile(QString sScriptFile,QString sFunctionName);
    bool isFunctionPresent(QString sScript,QString sFunctionName);
    void addClass(QObject *pClass,QString sClassName,QString sDefaultPath);
    void _appendError(QString sText);
    void _appendMessage(QString sText);
    QString getDefaultPath();
signals:
    void appendError(QString sText);
    void appendMessage(QString sText);
public slots:
private:
    void addFunction(FunctionSignature function,QString sFunctionName);   
    QString sDefaultPath;
};

#endif // PLUGINSSCRIPT_H
