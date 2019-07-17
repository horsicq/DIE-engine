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
#include "dialogpeversioninfo.h"
#include "ui_dialogpeversioninfo.h"

DialogPEVersionInfo::DialogPEVersionInfo(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEVersionInfo)
{
    ui->setupUi(this);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEVersionInfo::~DialogPEVersionInfo()
{
    delete ui;
}

void DialogPEVersionInfo::on_pushButtonOK_clicked()
{
    this->close();
}
bool DialogPEVersionInfo::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->textEditVersion->setToolTip(tr("Version"));
    }

    QByteArray baTemp=pefile->getResourceIdData(16);

    int nLength=0;
    int nOffset=0;
    int nOffset2=0;
    int nTemp=0;
    int nSize=0;
    int nSize2=0;

    VersionResource vr(&baTemp);
    connect(&vr,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    if(vr.isValid())
    {
        nLength=vr.getLength(0);

        ui->textEditVersion->append("[Fixed Info]");
        ui->textEditVersion->append(QString("    dwSignature: 0x%1").arg(vr.getFixedFileInfo_Signature(),8,16,QChar('0')));
        ui->textEditVersion->append(QString("    dwStrucVersion: 0x%1 (version: %2)").arg(vr.getFixedFileInfo_StrucVersion(),8,16,QChar('0')).arg(vr.versionToString(vr.getFixedFileInfo_StrucVersion())));
        ui->textEditVersion->append(QString("    dwFileVersionMS: 0x%1 (version: %2)").arg(vr.getFixedFileInfo_FileVersionMS(),8,16,QChar('0')).arg(vr.versionToString(vr.getFixedFileInfo_FileVersionMS())));
        ui->textEditVersion->append(QString("    dwFileVersionLS: 0x%1 (version: %2)").arg(vr.getFixedFileInfo_FileVersionLS(),8,16,QChar('0')).arg(vr.versionToString(vr.getFixedFileInfo_FileVersionLS())));
        ui->textEditVersion->append(QString("    dwProductVersionMS: 0x%1 (version: %2)").arg(vr.getFixedFileInfo_ProductVersionMS(),8,16,QChar('0')).arg(vr.versionToString(vr.getFixedFileInfo_ProductVersionMS())));
        ui->textEditVersion->append(QString("    dwProductVersionLS: 0x%1 (version: %2)").arg(vr.getFixedFileInfo_ProductVersionLS(),8,16,QChar('0')).arg(vr.versionToString(vr.getFixedFileInfo_ProductVersionLS())));
        ui->textEditVersion->append(QString("    dwFileFlagsMask: 0x%1").arg(vr.getFixedFileInfo_FileFlagsMask(),8,16,QChar('0')));
        ui->textEditVersion->append(QString("    dwFileFlags: 0x%1 (flags: %2)").arg(vr.getFixedFileInfo_FileFlags(),8,16,QChar('0')).arg(vr.fileFlagsToString(vr.getFixedFileInfo_FileFlags())));
        ui->textEditVersion->append(QString("    dwFileOS: 0x%1 (OS: %2)").arg(vr.getFixedFileInfo_FileOS(),8,16,QChar('0')).arg(vr.fileOSToString(vr.getFixedFileInfo_FileOS())));
        ui->textEditVersion->append(QString("    dwFileType: 0x%1 (Type: %2)").arg(vr.getFixedFileInfo_FileType(),8,16,QChar('0')).arg(vr.fileTypeToString(vr.getFixedFileInfo_FileType())));
        ui->textEditVersion->append(QString("    dwFileSubtype: 0x%1").arg(vr.getFixedFileInfo_FileSubtype(),8,16,QChar('0')));
        ui->textEditVersion->append(QString("    dwFileDateMS: 0x%1").arg(vr.getFixedFileInfo_FileDateMS(),8,16,QChar('0')));
        ui->textEditVersion->append(QString("    dwFileDateLS: 0x%1").arg(vr.getFixedFileInfo_FileDateLS(),8,16,QChar('0')));

        if(vr.isChildrenPresent())
        {
            nOffset=vr.getChildrenOffset();
            nLength-=nOffset;

            while(nLength>0)
            {
                //nSize2=ALIGN_UP(vr.getLength(nOffset),4);
                nSize2=vr.getLength(nOffset);

                if(nSize2==0)
                {
                    break;
                }

                if(vr.getKey(nOffset)=="StringFileInfo")
                {
                    ui->textEditVersion->append("[StringFileInfo]");
                    nOffset=vr.getNextHeaderOffset(nOffset);
                    nSize=vr.getLength(nOffset);

                    nOffset2=vr.getNextHeaderOffset(nOffset);
                    nSize=nSize-(nOffset2-nOffset);
                    nOffset=nOffset2;

                    while(nSize>0)
                    {
                        nTemp=ALIGN_UP(vr.getLength(nOffset),4);

                        if(nTemp==0)
                        {
                            break;
                        }

                        ui->textEditVersion->append(QString("    [%1] %2").arg(vr.getKey(nOffset)).arg(vr.getValue(nOffset)));

                        nSize-=nTemp;
                        nOffset+=nTemp;
                    }
                }
                else if(vr.getKey(nOffset)=="VarFileInfo")
                {
                    ui->textEditVersion->append("[VarFileInfo]");
                    nOffset=vr.getNextHeaderOffset(nOffset);
                    nSize=vr.getLength(nOffset);

                    nOffset2=vr.getNextHeaderOffset(nOffset);
                    nSize=nSize-(nOffset2-nOffset);
                    nOffset=nOffset2;

                    while(nSize>0)
                    {
                        ui->textEditVersion->append(QString("    Translation: 0x%1").arg(vr.getDword(nOffset),8,16,QChar('0')));
                        nSize-=4;
                        nOffset+=4;
                    }
                }

                nLength-=nSize2;
            }
        }
    }

    return true;
}
