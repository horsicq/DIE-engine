#include "scriptgeneric.h"

scriptGeneric::scriptGeneric(QObject *parent) :
    QObject(parent)
{
}

void scriptGeneric::_log(QString sString)
{
    emit appendError(sString);
}


unsigned int scriptGeneric::swapBytes(unsigned int nValue)
{
    unsigned int nResult;

    ((char *)(&nResult))[3]=((char *)(&nValue))[0];
    ((char *)(&nResult))[2]=((char *)(&nValue))[1];
    ((char *)(&nResult))[1]=((char *)(&nValue))[2];
    ((char *)(&nResult))[0]=((char *)(&nValue))[3];

    return nResult;
}

QString scriptGeneric::upperCase(QString sString)
{
    return sString.toUpper();
}

