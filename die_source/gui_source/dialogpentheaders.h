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
#ifndef DIALOGPENTHEADERS_H
#define DIALOGPENTHEADERS_H

#include "dialogpegeneric.h"
#include "dialoghde.h"
#include "dialogpemachine.h"
#include "dialogpesections.h"
#include "dialogpetimedatestamp.h"
#include "dialogpecharacteristics.h"
#include "dialogpemagic.h"
#include "dialogpechecksum.h"
#include "dialogpesubsystem.h"
#include "dialogpedllcharacteristics.h"

namespace Ui
{
class DialogPENtHeaders;
}

class DialogPENtHeaders : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPENtHeaders(QWidget *parent);
    ~DialogPENtHeaders();

private slots:
    bool reload();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();
    void on_checkBoxReadOnly_stateChanged(int arg1);
    void on_pushButtonHex_clicked();
    void applyEnable();
    void on_lineEditSignature_textChanged(const QString &arg1);
    void pushButtonMachineExtra_clicked();
    void pushButtonNumberOfSectionsExtra_clicked();
    void pushButtonTimeDateStampExtra_clicked();
    void pushButtonCharacteristicsExtra_clicked();
    void pushButtonMagicExtra_clicked();
    void pushButtonAddressOfEntryPointExtra_clicked();
    void pushButtonSizeOfHeadersExtra_clicked();
    void pushButtonCheckSumExtra_clicked();
    void pushButtonSubsystemExtra_clicked();
    void pushButtonDllCharacteristicsExtra_clicked();

private:
    Ui::DialogPENtHeaders *ui;

    QLineEditX *lineEditMachine;
    QLineEditX *lineEditNumberOfSections;
    QLineEditX *lineEditTimeDateStamp;
    QLineEditX *lineEditPointerToSymbolTable;
    QLineEditX *lineEditNumberOfSymbols;
    QLineEditX *lineEditSizeOfOptionalHeader;
    QLineEditX *lineEditCharacteristics;

    QLineEditX *lineEditMagic;
    QLineEditX *lineEditMajorLinkerVersion;
    QLineEditX *lineEditMinorLinkerVersion;
    QLineEditX *lineEditSizeOfCode;
    QLineEditX *lineEditSizeOfInitializedData;
    QLineEditX *lineEditSizeOfUninitializedData;
    QLineEditX *lineEditAddressOfEntryPoint;
    QLineEditX *lineEditBaseOfCode;
    QLineEditX *lineEditBaseOfData;
    QLineEditX *lineEditImageBase;
    QLineEditX *lineEditSectionAlignment;
    QLineEditX *lineEditFileAlignment;
    QLineEditX *lineEditMajorOperatingSystemVersion;
    QLineEditX *lineEditMinorOperatingSystemVersion;
    QLineEditX *lineEditMajorImageVersion;
    QLineEditX *lineEditMinorImageVersion;
    QLineEditX *lineEditMajorSubsystemVersion;
    QLineEditX *lineEditMinorSubsystemVersion;
    QLineEditX *lineEditWin32VersionValue;
    QLineEditX *lineEditSizeOfImage;
    QLineEditX *lineEditSizeOfHeaders;
    QLineEditX *lineEditCheckSum;
    QLineEditX *lineEditSubsystem;
    QLineEditX *lineEditDllCharacteristics;
    QLineEditX *lineEditSizeOfStackReserve;
    QLineEditX *lineEditSizeOfStackCommit;
    QLineEditX *lineEditSizeOfHeapReserve;
    QLineEditX *lineEditSizeOfHeapCommit;
    QLineEditX *lineEditLoaderFlags;
    QLineEditX *lineEditNumberOfRvaAndSizes;

    unsigned int nOffset;
    unsigned int nSize;

    QPushButtonX *pushButtonMachineExtra;
    QPushButtonX *pushButtonNumberOfSectionsExtra;
    QPushButtonX *pushButtonTimeDateStampExtra;
    QPushButtonX *pushButtonCharacteristicsExtra;
    QPushButtonX *pushButtonMagicExtra;
    QPushButtonX *pushButtonAddressOfEntryPointExtra;
    QPushButtonX *pushButtonSizeOfHeadersExtra;
    QPushButtonX *pushButtonCheckSumExtra;
    QPushButtonX *pushButtonSubsystemExtra;
    QPushButtonX *pushButtonDllCharacteristicsExtra;
};

#endif // DIALOGPENTHEADERS_H
