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
#ifndef DIALOGPE_H
#define DIALOGPE_H

#include "dialogpegeneric.h"
#include "dialogpedosheader.h"
#include "dialogpestub.h"
#include "dialogpeoverlay.h"
#include "dialogpentheaders.h"
#include "dialogpesections.h"
#include "dialogpedirectories.h"
#include "dialogpeimport.h"
#include "dialogpeexport.h"
#include "dialogperesource.h"
#include "dialogoptions.h"

namespace Ui
{
class DialogPE;
}

class DialogPE : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPE(QWidget *parent);
    ~DialogPE();

private slots:
    bool reload();
    void on_pushButtonOK_clicked();
    void on_pushButtonDosHeader_clicked();
    void on_pushButtonStub_clicked();
    void on_pushButtonOverlay_clicked();
    void on_pushButtonNtHeaders_clicked();
    void on_pushButtonSections_clicked();
    void on_pushButtonDirectories_clicked();
    //void on_pushButtonResource_clicked();
    void applyEnable();
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();

    void pushButtonTimeDateStampExtra_clicked();
    void pushButtonAddressOfEntryPointExtra_clicked();
    void pushButtonCheckSumExtra_clicked();
    void pushButtonSubsystemExtra_clicked();

private:
    Ui::DialogPE *ui;

    QLineEditX *lineEditEntryPoint;
    QLineEditX *lineEditImageBase;
    QLineEditX *lineEditSizeOfImage;
    QLineEditX *lineEditTimeDateStamp;
    QLineEditX *lineEditCheckSum;
    QLineEditX *lineEditSubsystem;

    QPushButtonX *pushButtonAddressOfEntryPointExtra;
    QPushButtonX *pushButtonTimeDateStampExtra;
    QPushButtonX *pushButtonCheckSumExtra;
    QPushButtonX *pushButtonSubsystemExtra;

    //public:
    //    void setData(QString sFileName,__OPTIONS *pOptions);
    //protected:
    //    QString sFileName;
    //    __OPTIONS *pOptions;
    //    PEFile pefile;
    //signals:
    //    void reloadSignal();
    //    void appendError(QString sString);
};

#endif // DIALOGPE_H
