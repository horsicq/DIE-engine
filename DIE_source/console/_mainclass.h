#ifndef _MAINCLASS_H
#define _MAINCLASS_H

#include <QObject>
#include <stdio.h>
#include "../utils.h"

class _MainClass : public QObject
{
    Q_OBJECT
public:
    explicit _MainClass(QObject *parent = 0);
    void setOptions(__DIE_OPTIONS *pOptions);
signals:

public slots:
    void handleMessage(QString szMessage);
    //    void appendSignature(QString szMessage);
    void appendError(QString szMessage);
    void appendFileName(QString szMessage);

private:
    bool bSingleLineOutput;
    __DIE_OPTIONS *pOptions;
    QString sCurrentType;
};

#endif // _MAINCLASS_H
