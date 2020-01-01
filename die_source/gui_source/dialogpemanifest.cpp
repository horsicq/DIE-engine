// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#include "dialogpemanifest.h"
#include "ui_dialogpemanifest.h"

QString GetTab(int nIndent)
{
    QString sResult;

    while(nIndent)
    {
        sResult.append("    ");

        nIndent--;
    }

    return sResult;
}

DialogPEManifest::DialogPEManifest(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEManifest)
{
    ui->setupUi(this);


    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEManifest::~DialogPEManifest()
{
    delete ui;
}

void DialogPEManifest::on_pushButtonOK_clicked()
{
    this->close();
}
bool DialogPEManifest::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->textEditManifest->setToolTip(tr("Manifest"));
    }

    QByteArray baTemp=pefile->getResourceIdData(24);

    ui->textEditManifest->setText(baTemp.data());

    QXmlInputSource _xml;
    _xml.setData(baTemp);
    QXmlSimpleReader xmlreader;

    int nIndent=0;
    QString sTemp;
    bool bContinue=false;

    if(xmlreader.parse(&_xml))
    {
        QXmlStreamReader xml(baTemp);

        while(!xml.atEnd())
        {
            if(xml.readNext())
            {
                if(xml.isStartElement())
                {
                    sTemp=GetTab(nIndent);
                    sTemp.append("[");
                    sTemp.append(xml.name());
                    sTemp.append("] ");
                    //                    sTemp.append(xml.text());

                    //                    if(!xml.isWhitespace())
                    //                    {
                    //                        ui->textEditInfo->append(xml.text().toString());
                    //                    }

                    ui->textEditInfo->append(sTemp);

                    //                    qDebug("Count %d",xml.attributes().count());

                    for(int i=0; i<xml.attributes().count(); i++)
                    {
                        sTemp=GetTab(nIndent+1);
                        sTemp.append(xml.attributes().at(i).name().toString());
                        sTemp.append(": ");
                        sTemp.append(xml.attributes().at(i).value().toString());
                        ui->textEditInfo->append(sTemp);
                    }

                    bContinue=true;

                    nIndent++;
                }
                else if(xml.isEndElement())
                {
                    if(nIndent)
                    {
                        nIndent--;
                    }

                }
                else if(!xml.isWhitespace())
                {
                    ui->textEditInfo->append(sTemp=GetTab(nIndent)+xml.text().toString());
                }
            }
        }
    }
    else
    {
        //        qDebug("Invalid manifest");
        emit appendError("Invalid manifest");
    }

    return true;
}
