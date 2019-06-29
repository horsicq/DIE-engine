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
#include "dialogpeboundimport.h"
#include "ui_dialogpeboundimport.h"

DialogPEBoundImport::DialogPEBoundImport(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEBoundImport)
{
    ui->setupUi(this);

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEBoundImport::~DialogPEBoundImport()
{
    delete ui;
}

void DialogPEBoundImport::on_pushButtonOK_clicked()
{
    this->close();
}
bool DialogPEBoundImport::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
    }

    QTreeWidgetItem *item0;
    QTreeWidgetItem *item1;

    unsigned int nImports=pefile->getBoundImport_NumberOfImports();

    int nIndex=0;
    int nChilds=0;

    QDateTime dt;
    QString sName;

    for(int i=0; i<nImports; i++)
    {
        dt.setTime_t(pefile->getBoundImport_TimeDateStamp(i));
        sName=QString("%1(%2)").arg(pefile->getBoundImport_ModuleNameAsString(i)).arg(dt.toString());

        if(nChilds)
        {
            item1=new QTreeWidgetItem(item0);
            item1->setText(0,sName);
            item0->addChild(item1);
            nChilds--;
        }
        else
        {
            item0=new QTreeWidgetItem;
            item0->setText(0,sName);
            nChilds=pefile->getBoundImport_NumberOfModuleForwarderRefs(i);
            ui->treeWidgetBoundImport->insertTopLevelItem(nIndex++,item0);
        }

    }

    return true;
}


