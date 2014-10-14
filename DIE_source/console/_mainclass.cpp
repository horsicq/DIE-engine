#include "_mainclass.h"

_MainClass::_MainClass(QObject *parent) :
    QObject(parent)
{
}

void _MainClass::setOptions(__DIE_OPTIONS *pOptions)
{
    this->pOptions=pOptions;
}
void _MainClass::handleMessage(QString szMessage)
{
    printf("%s",szMessage.toAscii().data());

    if(pOptions->bSingleLineOutput)
    {
        printf("; ");
    }
    else
    {
        printf("\n");
    }
}

void _MainClass::appendSignature(QString szMessage)
{
    if((pOptions->bShowFileFormatOnce)&&(pOptions->nNumberOfResults>1))
    {
        szMessage=szMessage.section(": ",1,-1);
    }

    handleMessage(szMessage);
}

void _MainClass::appendError(QString szMessage)
{
    handleMessage(szMessage);
}

void _MainClass::appendFileName(QString szMessage)
{
    printf("%s",szMessage.toAscii().data());

    if(pOptions->bSingleLineOutput)
    {
        printf(": ");
    }
    else
    {
        printf("\n");
    }
}
