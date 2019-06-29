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
#ifndef DIALOGPEDIRECTORIES_H
#define DIALOGPEDIRECTORIES_H

#include "dialogpegeneric.h"
#include "dialoghde.h"
#include "dialogpeimport.h"
#include "dialogpeexport.h"
#include "dialogpebasereloc.h"
#include "dialogperesource.h"
#include "dialogpecom.h"
#include "dialogpetls.h"
#include "dialogpeloadconfig.h"
#include "dialogpedebug.h"
#include "dialogpeboundimport.h"

namespace Ui
{
class DialogPEDirectories;
}

class DialogPEDirectories : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPEDirectories(QWidget *parent);
    ~DialogPEDirectories();

private slots:
    bool reload();

    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonHex_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void applyEnable();

    void pushButtonEXPORTExtra_clicked();
    void pushButtonIMPORTExtra_clicked();
    void pushButtonRESOURCEExtra_clicked();
    void pushButtonEXCEPTIONExtra_clicked();
    void pushButtonSECURITYExtra_clicked();
    void pushButtonBASERELOCExtra_clicked();
    void pushButtonDEBUGExtra_clicked();
    void pushButtonCOPYRIGHTExtra_clicked();
    void pushButtonGLOBALPTRExtra_clicked();
    void pushButtonTLSExtra_clicked();
    void pushButtonLOAD_CONFIGExtra_clicked();
    void pushButtonBOUND_IMPORTExtra_clicked();
    void pushButtonIATExtra_clicked();
    void pushButtonDELAY_IMPORTExtra_clicked();
    void pushButtonCOM_DESCRIPTORExtra_clicked();
    void pushButtonReservedExtra_clicked();

    void pushButtonEXPORTHex_clicked();
    void pushButtonIMPORTHex_clicked();
    void pushButtonRESOURCEHex_clicked();
    void pushButtonEXCEPTIONHex_clicked();
    void pushButtonSECURITYHex_clicked();
    void pushButtonBASERELOCHex_clicked();
    void pushButtonDEBUGHex_clicked();
    void pushButtonCOPYRIGHTHex_clicked();
    void pushButtonGLOBALPTRHex_clicked();
    void pushButtonTLSHex_clicked();
    void pushButtonLOAD_CONFIGHex_clicked();
    void pushButtonBOUND_IMPORTHex_clicked();
    void pushButtonIATHex_clicked();
    void pushButtonDELAY_IMPORTHex_clicked();
    void pushButtonCOM_DESCRIPTORHex_clicked();
    void pushButtonReservedHex_clicked();

private:
    Ui::DialogPEDirectories *ui;
    unsigned int nOffset;
    unsigned int nSize;

    QLineEditX *lineeditEXPORTAddress;
    QLineEditX *lineeditIMPORTAddress;
    QLineEditX *lineeditRESOURCEAddress;
    QLineEditX *lineeditEXCEPTIONAddress;
    QLineEditX *lineeditSECURITYAddress;
    QLineEditX *lineeditBASERELOCAddress;
    QLineEditX *lineeditDEBUGAddress;
    QLineEditX *lineeditCOPYRIGHTAddress;
    QLineEditX *lineeditGLOBALPTRAddress;
    QLineEditX *lineeditTLSAddress;
    QLineEditX *lineeditLOAD_CONFIGAddress;
    QLineEditX *lineeditBOUND_IMPORTAddress;
    QLineEditX *lineeditIATAddress;
    QLineEditX *lineeditDELAY_IMPORTAddress;
    QLineEditX *lineeditCOM_DESCRIPTORAddress;
    QLineEditX *lineeditReservedAddress;

    QLineEditX *lineeditEXPORTSize;
    QLineEditX *lineeditIMPORTSize;
    QLineEditX *lineeditRESOURCESize;
    QLineEditX *lineeditEXCEPTIONSize;
    QLineEditX *lineeditSECURITYSize;
    QLineEditX *lineeditBASERELOCSize;
    QLineEditX *lineeditDEBUGSize;
    QLineEditX *lineeditCOPYRIGHTSize;
    QLineEditX *lineeditGLOBALPTRSize;
    QLineEditX *lineeditTLSSize;
    QLineEditX *lineeditLOAD_CONFIGSize;
    QLineEditX *lineeditBOUND_IMPORTSize;
    QLineEditX *lineeditIATSize;
    QLineEditX *lineeditDELAY_IMPORTSize;
    QLineEditX *lineeditCOM_DESCRIPTORSize;
    QLineEditX *lineeditReservedSize;

    QPushButtonX *pushButtonEXPORTExtra;
    QPushButtonX *pushButtonIMPORTExtra;
    QPushButtonX *pushButtonRESOURCEExtra;
    QPushButtonX *pushButtonEXCEPTIONExtra;
    QPushButtonX *pushButtonSECURITYExtra;
    QPushButtonX *pushButtonBASERELOCExtra;
    QPushButtonX *pushButtonDEBUGExtra;
    QPushButtonX *pushButtonCOPYRIGHTExtra;
    QPushButtonX *pushButtonGLOBALPTRExtra;
    QPushButtonX *pushButtonTLSExtra;
    QPushButtonX *pushButtonLOAD_CONFIGExtra;
    QPushButtonX *pushButtonBOUND_IMPORTExtra;
    QPushButtonX *pushButtonIATExtra;
    QPushButtonX *pushButtonDELAY_IMPORTExtra;
    QPushButtonX *pushButtonCOM_DESCRIPTORExtra;
    QPushButtonX *pushButtonReservedExtra;

    QPushButtonX *pushButtonEXPORTHex;
    QPushButtonX *pushButtonIMPORTHex;
    QPushButtonX *pushButtonRESOURCEHex;
    QPushButtonX *pushButtonEXCEPTIONHex;
    QPushButtonX *pushButtonSECURITYHex;
    QPushButtonX *pushButtonBASERELOCHex;
    QPushButtonX *pushButtonDEBUGHex;
    QPushButtonX *pushButtonCOPYRIGHTHex;
    QPushButtonX *pushButtonGLOBALPTRHex;
    QPushButtonX *pushButtonTLSHex;
    QPushButtonX *pushButtonLOAD_CONFIGHex;
    QPushButtonX *pushButtonBOUND_IMPORTHex;
    QPushButtonX *pushButtonIATHex;
    QPushButtonX *pushButtonDELAY_IMPORTHex;
    QPushButtonX *pushButtonCOM_DESCRIPTORHex;
    QPushButtonX *pushButtonReservedHex;

};

#endif // DIALOGPEDIRECTORIES_H
