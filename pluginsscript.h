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
#ifndef PLUGINSSCRIPT_H
#define PLUGINSSCRIPT_H

#include <QScriptEngine>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include "die_source/utils.h"

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
