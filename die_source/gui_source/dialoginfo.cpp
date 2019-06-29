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
#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);


    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogInfo::~DialogInfo()
{
    delete ui;
}

void DialogInfo::setData(__DIE_OPTIONS *pOptions,QString &sString)
{
    this->sString=sString;

    setOptions(pOptions);
}

bool DialogInfo::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->textEditInfo->setToolTip(tr("Information"));
    }

    if(pOptions->bScanShowVersionDIE)
    {
        QString sTemp=this->sString;

        int nTemp=10;

        int i=sTemp.size()-1;

        for(; i>=0; i--)
        {
            if(sTemp[i]==')')
            {
                if(nTemp==10)
                {
                    nTemp=0;
                }

                nTemp--;
            }
            else if(sTemp[i]=='(')
            {
                if(nTemp==10)
                {
                    nTemp=0;
                }

                nTemp++;
            }

            if(nTemp==0)
            {
                break;
            }
        }

        if(i)
        {
            this->sString=sTemp.left(i);
        }
    }

    //    if(this->sString.count("(")==2)
    //    {
    //        this->sString=this->sString.section("(",0,1);
    //    }
    //    else
    //    {
    //        this->sString=this->sString.section("(",0,0);
    //    }


    if(pOptions->bScanShowOptionsDIE)
    {
        this->sString=this->sString.section("[",0,0);
    }


    QFile file;
    QString sFileName=this->sString;
    sFileName=sFileName.replace("\\","_");
    sFileName=sFileName.replace("/","_");

    if(sFileName.size()>0)
    {
        if(sFileName.at(0)==QChar('.'))
        {
            sFileName[0]=QChar('_');
        }
    }

    this->setWindowTitle(this->sString);

    //sFileName=Utils::getInfoPath(pOptions)+QDir::separator()+QString("%1.html").arg(sFileName);

    //#ifndef QT_NO_DEBUG
    //    qDebug("Open info file: %s",sFileName.toAscii().data());
    //#endif

    //    QMessageBox::critical(0,"",file.fileName());

    sFileName=Utils::getInfoPath(pOptions)+QDir::separator()+QString("%1.html").arg(sFileName);

    file.setFileName(sFileName);

    if(file.open(QFile::ReadOnly))
    {
        QByteArray data = file.readAll();
        //        QTextCodec *codec = Qt::codecForHtml(data);
        //        QString str = codec->toUnicode(data);
        QString str=QString::fromUtf8(data.data());

        if(Qt::mightBeRichText(str))
        {
            ui->textEditInfo->setHtml(str);
        }
        else
        {
            ui->textEditInfo->setPlainText(str);
        }
    }

    return true;
}

void DialogInfo::on_pushButtonOK_clicked()
{
    this->close();
}
