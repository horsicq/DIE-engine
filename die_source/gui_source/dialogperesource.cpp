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
#include "dialogperesource.h"
#include "ui_dialogperesource.h"

DialogPEResource::DialogPEResource(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEResource)
{
    ui->setupUi(this);

    actHex=new QAction(tr("Hex"), this);
    actDump=new QAction(tr("Dump"), this);
    actVersion=new QAction(tr("Version"), this);
    actManifest=new QAction(tr("Manifest"), this);

    connect(actHex,SIGNAL(triggered()),this,SLOT(hex()));
    connect(actDump,SIGNAL(triggered()),this,SLOT(dump()));
    connect(actVersion,SIGNAL(triggered()),this,SLOT(version()));
    connect(actManifest,SIGNAL(triggered()),this,SLOT(manifest()));

    ui->pushButtonVersion->setShortcut(QKeySequence(__KeySequence_altV));
    ui->pushButtonManifest->setShortcut(QKeySequence(__KeySequence_altM));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
}

DialogPEResource::~DialogPEResource()
{
    delete ui;
}

void DialogPEResource::on_pushButtonOK_clicked()
{
    this->close();
}
bool DialogPEResource::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->treeWidgetResource->setToolTip(tr("Resource tree"));
        ui->pushButtonManifest->setToolTip(tr("Manifest"));
        ui->pushButtonVersion->setToolTip(tr("Version Info"));
    }

    ui->pushButtonManifest->setEnabled(pefile->isManifestPresent());
    ui->pushButtonVersion->setEnabled(pefile->isVersionInfoPresent());

    QByteArray baXml=pefile->getResourceAsXML();
    QXmlStreamReader xml(baXml);
    QTreeWidgetItem *item0=0;
    QTreeWidgetItem *item1=0;
    int nIndex=0;
    QString sName;


    while(!xml.atEnd())
    {
        if(xml.readNextStartElement())
        {
            if(xml.name()=="Entry")
            {
                if(xml.attributes().value("Level").toString()=="0")
                {
                    item0=new QTreeWidgetItem;

                    sName=xml.attributes().value("Name").toString();

                    if(sName!="")
                    {

                        sName="\'"+sName+"\'";
                    }
                    else
                    {
                        sName=pefile->resourceIdToString(xml.attributes().value("Id").toString().toInt());
                    }

                    item0->setText(0,sName);

                    ui->treeWidgetResource->insertTopLevelItem(nIndex++,item0);
                }
                else if(xml.attributes().value("Level").toString()=="1")
                {

                    item1=new QTreeWidgetItem(item0);

                    sName=xml.attributes().value("Name").toString();

                    if(sName!="")
                    {

                        sName="\'"+sName+"\'";
                    }
                    else
                    {
                        sName=xml.attributes().value("Id").toString();
                    }

                    item1->setText(0,sName);
                    item0->addChild(item1);


                }
            }
            else if(xml.name()=="DataEntry")
            {
                QString sDataString=xml.attributes().value("Offset").toString()+":"+xml.attributes().value("Size").toString();

                if(item1)
                {
                    item1->setData(0,Qt::UserRole,sDataString);
                }

            }
        }
    }

    return true;
}

void DialogPEResource::on_treeWidgetResource_customContextMenuRequested(const QPoint &pos)
{
    //    qDebug("context menu");
    //    qDebug(ui->treeWidgetResource->currentItem()->data(0,Qt::UserRole).toString().toAscii().data());


    if(ui->treeWidgetResource->currentItem()->data(0,Qt::UserRole).toString()!="")
    {
        QMenu menu;

        if(ui->treeWidgetResource->currentItem()->parent()->text(0)=="RT_VERSION")
        {
            menu.addAction(actVersion);
        }
        else if(ui->treeWidgetResource->currentItem()->parent()->text(0)=="RT_MANIFEST")
        {
            menu.addAction(actManifest);
        }

        menu.addAction(actHex);
        menu.addAction(actDump);

        menu.exec(ui->treeWidgetResource->mapToGlobal(pos));
    }
}

void DialogPEResource::hex()
{
    unsigned int nOffset=ui->treeWidgetResource->currentItem()->data(0,Qt::UserRole).toString().section(":",0,0).toInt();
    unsigned int nSize=ui->treeWidgetResource->currentItem()->data(0,Qt::UserRole).toString().section(":",1,1).toInt();


    DialogHDE dialoghde(this);

    //    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Resource"));

    dialoghde.exec();
}

void DialogPEResource::dump()
{

    unsigned int nOffset=ui->treeWidgetResource->currentItem()->data(0,Qt::UserRole).toString().section(":",0,0).toInt();
    unsigned int nSize=ui->treeWidgetResource->currentItem()->data(0,Qt::UserRole).toString().section(":",1,1).toInt();
    //    int nSection=ui->tableWidgetSections->selectedItems().at(0)->row();

    QString sFileName;

    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),pOptions->sLastDirectory+QDir::separator()+QString("%1.bin").arg(tr("Resource")), tr("Dump files (*.bin);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(pOptions->bSaveLastDirectory)
        {
            pOptions->sLastDirectory=Utils::getPath(sFileName);
        }

        pefile->dump(sFileName,nOffset,nSize);
    }
}

void DialogPEResource::version()
{
    DialogPEVersionInfo dialogpeversioninfo(this);

    dialogpeversioninfo.setData(pOptions,pefile);

    dialogpeversioninfo.exec();
}

void DialogPEResource::manifest()
{
    DialogPEManifest dialogpemanifest(this);

    dialogpemanifest.setData(pOptions,pefile);

    dialogpemanifest.exec();
}

void DialogPEResource::on_pushButtonManifest_clicked()
{
    manifest();
}

void DialogPEResource::on_pushButtonVersion_clicked()
{
    version();
}
