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
#ifndef DIALOGPECOM_H
#define DIALOGPECOM_H

#include "dialogpegeneric.h"
#include "dialoghde.h"
#include "dialogpemetadata.h"


namespace Ui
{
class DialogPECOM;
}

class DialogPECOM : public DialogPEGeneric
{
    Q_OBJECT

public:
    explicit DialogPECOM(QWidget *parent = 0);
    ~DialogPECOM();

private slots:
    void on_pushButtonOK_clicked();
    bool reload();

    void on_lineEditFlags_textChanged(const QString &arg1);

    void pushButtonMetaDataExtra_clicked();
    void pushButtonResourcesExtra_clicked();
    void pushButtonStrongNameSignatureExtra_clicked();
    void pushButtonCodeManagerTableExtra_clicked();
    void pushButtonVTableFixupsExtra_clicked();
    void pushButtonExportAddressTableJumpsExtra_clicked();
    void pushButtonManagedNativeHeaderExtra_clicked();
    void pushButtonMetaDataHEX_clicked();
    void pushButtonResourcesHEX_clicked();
    void pushButtonStrongNameSignatureHEX_clicked();
    void pushButtonCodeManagerTableHEX_clicked();
    void pushButtonVTableFixupsHEX_clicked();
    void pushButtonExportAddressTableJumpsHEX_clicked();
    void pushButtonManagedNativeHeaderHEX_clicked();

    void on_pushButtonEntryPointExtra_clicked();

private:
    Ui::DialogPECOM *ui;

    QLineEdit *lineEditSignature;
    //    QLineEdit *lineEditStrucVersion;
    //    QLineEdit *lineEditFileVersionMS;
    //    QLineEdit *lineEditFileVersionLS;
    //    QLineEdit *lineEditProductVersionMS;
    //    QLineEdit *lineEditProductVersionLS;
    //    QLineEdit *lineEditFileFlagsMask;
    //    QLineEdit *lineEditFileFlags;
    //    QLineEdit *lineEditFileOS;
    //    QLineEdit *lineEditFileType;
    //    QLineEdit *lineEditFileSubtype;
    //    QLineEdit *lineEditFileDateMS;
    //    QLineEdit *lineEditFileDateLS;
    QLineEditX *lineEditMetaDataAddress;
    QLineEditX *lineEditResourcesAddress;
    QLineEditX *lineEditStrongNameSignatureAddress;
    QLineEditX *lineEditCodeManagerTableAddress;
    QLineEditX *lineEditVTableFixupsAddress;
    QLineEditX *lineEditExportAddressTableJumpsAddress;
    QLineEditX *lineEditManagedNativeHeaderAddress;
    QLineEditX *lineEditMetaDataSize;
    QLineEditX *lineEditResourcesSize;
    QLineEditX *lineEditStrongNameSignatureSize;
    QLineEditX *lineEditCodeManagerTableSize;
    QLineEditX *lineEditVTableFixupsSize;
    QLineEditX *lineEditExportAddressTableJumpsSize;
    QLineEditX *lineEditManagedNativeHeaderSize;

    QPushButton *pushButtonMetaDataExtra;
    QPushButton *pushButtonResourcesExtra;
    QPushButton *pushButtonStrongNameSignatureExtra;
    QPushButton *pushButtonCodeManagerTableExtra;
    QPushButton *pushButtonVTableFixupsExtra;
    QPushButton *pushButtonExportAddressTableJumpsExtra;
    QPushButton *pushButtonManagedNativeHeaderExtra;
    QPushButton *pushButtonMetaDataHEX;
    QPushButton *pushButtonResourcesHEX;
    QPushButton *pushButtonStrongNameSignatureHEX;
    QPushButton *pushButtonCodeManagerTableHEX;
    QPushButton *pushButtonVTableFixupsHEX;
    QPushButton *pushButtonExportAddressTableJumpsHEX;
    QPushButton *pushButtonManagedNativeHeaderHEX;
};

#endif // DIALOGPECOM_H
