#include "dialogelfgeneric.h"

DialogELFGeneric::DialogELFGeneric(QWidget *parent) :
    QXDialogX(parent)
{
}

void DialogELFGeneric::setData(__DIE_OPTIONS *pOptions, ELFFile *elffile)
{
    this->elffile=elffile;
    setOptions(pOptions);
}
