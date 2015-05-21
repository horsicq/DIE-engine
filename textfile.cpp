#include "textfile.h"

TextFile::TextFile(QObject *parent) :
    Binary(parent)
{
}

bool TextFile::isValid()
{
    if(isPlainText())
    {
        return true;
    }

    emit appendError("Invalid Text file");

    return false;
}
