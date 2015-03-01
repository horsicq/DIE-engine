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

//void _MainClass::appendSignature(QString szMessage)
//{
//    QString sType=szMessage.section(": ",0,0);
//    if((pOptions->bShowFileFormatOnce)&&(sType!=sCurrentType))
//    {
//        szMessage=szMessage.section(": ",1,-1);
//        sCurrentType=sType;
//    }

//    handleMessage(szMessage);
//}

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
