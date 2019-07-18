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
#include "dialognewsignature.h"
#include "ui_dialognewsignature.h"

DialogNewSignature::DialogNewSignature(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogNewSignature)
{
    ui->setupUi(this);

    ui->lineEditFileName->setText("New signature");
    ui->lineEditAuthor->setText("anonymous");
    ui->lineEditType->setText("unknown");

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_save));
}

DialogNewSignature::~DialogNewSignature()
{
    delete ui;
}

void DialogNewSignature::setData(__DIE_OPTIONS *pOptions,QString sFolder,QString *psNew)
{
    this->sFolder=sFolder;
    this->psNew=psNew;

    QDir dir(Utils::getEditorPath(pOptions)+QDir::separator()+"templates");
    QFileInfoList dircontent=dir.entryInfoList(QStringList()<<"*.tpl");


    for(int i=0; i<dircontent.count(); i++)
    {
        ui->comboBoxTemplate->addItem(dircontent.at(i).baseName(),dircontent.at(i).absoluteFilePath());
    }


    QDir dir2(Utils::getEditorPath(pOptions)+QDir::separator()+"templates"+QDir::separator()+sFolder);
    QFileInfoList dircontent2=dir2.entryInfoList(QStringList()<<"*.tpl");


    for(int i=0; i<dircontent2.count(); i++)
    {
        ui->comboBoxTemplate->addItem(QString("(%1)%2").arg(sFolder,dircontent2.at(i).baseName()),dircontent2.at(i).absoluteFilePath());
    }

    setOptions(pOptions);
}

void DialogNewSignature::on_lineEditFileName_textChanged(const QString &arg1)
{
    if(ui->lineEditName->text()=="")
    {
        ui->lineEditName->setText(arg1);
    }
}

void DialogNewSignature::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogNewSignature::on_pushButtonOK_clicked()
{
    if(ui->lineEditFileName->text()=="")
    {
        QMessageBox::critical(this,tr("Error"),tr("Empty file name"));

        return;
    }

    QString sFileName=Utils::getDataBasePath(pOptions)+QDir::separator()+sFolder+QDir::separator()+ui->lineEditFileName->text()+".sg";

    if(QFile::exists(sFileName))
    {
        if(QMessageBox::information(this,tr("Information"),tr("File %1 exists. Replace?").arg(sFileName),QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
        {
            return;
        }
    }

    QFile fileTemplate;
    QString sTemplateFileName=ui->comboBoxTemplate->itemData(ui->comboBoxTemplate->currentIndex()).toString();
    QString sTemplate;

    fileTemplate.setFileName(sTemplateFileName);

    if(fileTemplate.open(QIODevice::ReadOnly))
    {
        sTemplate=fileTemplate.readAll();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("Cannot open template: %1").arg(sTemplateFileName));

        return;
    }

    QFile fileReplace;

    fileReplace.setFileName(sFileName);

    if(fileReplace.open(QIODevice::ReadWrite))
    {
        *psNew=ui->lineEditFileName->text();

        sTemplate.replace("@AUTHOR@",ui->lineEditAuthor->text());
        sTemplate.replace("@COMMENT@",ui->textEditComment->toPlainText().replace("\n","\r\n"));
        sTemplate.replace("@TYPE@",ui->lineEditType->text());
        sTemplate.replace("@NAME@",ui->lineEditName->text());

        //        QString sText;

        //        sText.append("// DIE's signature file\r\n");
        //        sText.append(QString("// Author: %1\r\n").arg(ui->lineEditAuthor->text()));
        //        sText.append(QString("/*\r\n%1\r\n*/\r\n").arg(ui->textEditComment->toPlainText().replace("\n","\r\n")));
        //        sText.append("function detect(bShowType,bShowVersion,bShowOptions)\r\n");
        //        sText.append("{\r\n");
        //        sText.append(QString("    var sType=\"%1\";\r\n").arg(ui->lineEditType->text()));
        //        sText.append(QString("    var sName=\"%1\";\r\n").arg(ui->lineEditName->text()));
        //        sText.append("    var sVersion=\"-\";\r\n");
        //        sText.append("    var sOptions=\"-\";\r\n");
        //        sText.append("    var sResult=\"\";\r\n");
        //        sText.append("    var nDetected=0;\r\n");
        //        sText.append("    \r\n");
        //        sText.append("    // Start of user's code\r\n");
        //        sText.append("    \r\n");
        //        sText.append("    \r\n");
        //        sText.append("    \r\n");
        //        sText.append("    \r\n");
        //        sText.append("    // End of user's code\r\n");
        //        sText.append("    \r\n");
        //        sText.append("    if(nDetected)\r\n");
        //        sText.append("    {\r\n");
        //        sText.append("        if(bShowType)\r\n");
        //        sText.append("        {\r\n");
        //        sText.append("            sResult+=sType+\": \";\r\n");
        //        sText.append("        }\r\n");
        //        sText.append("        sResult+=sName;\r\n");
        //        sText.append("        if(bShowVersion)\r\n");
        //        sText.append("        {\r\n");
        //        sText.append("            sResult+=\"(\"+sVersion+\")\";\r\n");
        //        sText.append("        }\r\n");
        //        sText.append("        if(bShowOptions)\r\n");
        //        sText.append("        {\r\n");
        //        sText.append("            sResult+=\"[\"+sOptions+\"]\";\r\n");
        //        sText.append("        }\r\n");
        //        sText.append("    }\r\n");
        //        sText.append("    \r\n");
        //        sText.append("    return sResult;\r\n");
        //        sText.append("}\r\n");


        fileReplace.resize(0);
        fileReplace.write(sTemplate.toLatin1().data());
        fileReplace.close();

        this->close();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("Cannot save file: %1").arg(sFileName));

        return;
    }
}

bool DialogNewSignature::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
    }

    return true;
}
