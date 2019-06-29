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
#ifndef QDIALOGX_H
#define QDIALOGX_H

#include <QDialog>
#include "dialogoptions.h"
#include "../_options.h"

class QDialogX : public QDialog
{
    Q_OBJECT
public:
    explicit QDialogX(QWidget *parent = 0);


    //    void _error(QString sString);
    //    void _reload();

public:
    void setData(Binary *binary,__DIE_OPTIONS *pOptions);
    void setData(PEFile *pefile,__DIE_OPTIONS *pOptions);
    void setData(MSDOSFile *msdosfile,__DIE_OPTIONS *pOptions);
    void setData(ELFFile *elffile,__DIE_OPTIONS *pOptions);
    void setData(MACHFile *machfile,__DIE_OPTIONS *pOptions);
protected:
    Binary *binary;
    PEFile *pefile;
    MSDOSFile *msdosfile;
    ELFFile *elffile;
    MACHFile *machfile;
    __DIE_OPTIONS *pOptions;
signals:
    void reloadSignal();
    void appendError(QString sString);

public slots:
    virtual bool reload()=0;

};

#endif // QDIALOGX_H
