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
#include "dialogmsdosheader.h"
#include "ui_dialogmsdosheader.h"

DialogMSDOSHeader::DialogMSDOSHeader(QWidget *parent) :
    DialogMSDOSGeneric(parent),
    ui(new Ui::DialogMSDOSHeader)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);

    QTableWidgetItem *newItem;

    ui->tableWidgetDosHeader->setRowCount(31);
    ui->tableWidgetDosHeader->setColumnCount(2);

    QString sLabels[]= { "e_magic",
                         "e_cblp",
                         "e_cp",
                         "e_crlc",
                         "e_cparhdr",
                         "e_minalloc",
                         "e_maxalloc",
                         "e_ss",
                         "e_sp",
                         "e_csum",
                         "e_ip",
                         "e_cs",
                         "e_lfarlc",
                         "e_ovno",
                         "e_res[0]",
                         "e_res[1]",
                         "e_res[2]",
                         "e_res[3]",
                         "e_oemid",
                         "e_oeminfo",
                         "e_res2[0]",
                         "e_res2[1]",
                         "e_res2[2]",
                         "e_res2[3]",
                         "e_res2[4]",
                         "e_res2[5]",
                         "e_res2[6]",
                         "e_res2[7]",
                         "e_res2[8]",
                         "e_res2[9]",
                         "e_lfanew"
                       };

    lineEdite_magic=new QLineEditX;
    lineEdite_cblp=new QLineEditX;
    lineEdite_cp=new QLineEditX;
    lineEdite_crlc=new QLineEditX;
    lineEdite_cparhdr=new QLineEditX;
    lineEdite_minalloc=new QLineEditX;
    lineEdite_maxalloc=new QLineEditX;
    lineEdite_ss=new QLineEditX;
    lineEdite_sp=new QLineEditX;
    lineEdite_csum=new QLineEditX;
    lineEdite_ip=new QLineEditX;
    lineEdite_cs=new QLineEditX;
    lineEdite_lfarlc=new QLineEditX;
    lineEdite_ovno=new QLineEditX;
    lineEdite_oemid=new QLineEditX;
    lineEdite_oeminfo=new QLineEditX;
    lineEdite_lfanew=new QLineEditX;

    lineEdite_res0=new QLineEditX;
    lineEdite_res1=new QLineEditX;
    lineEdite_res2=new QLineEditX;
    lineEdite_res3=new QLineEditX;

    lineEdite_res20=new QLineEditX;
    lineEdite_res21=new QLineEditX;
    lineEdite_res22=new QLineEditX;
    lineEdite_res23=new QLineEditX;
    lineEdite_res24=new QLineEditX;
    lineEdite_res25=new QLineEditX;
    lineEdite_res26=new QLineEditX;
    lineEdite_res27=new QLineEditX;
    lineEdite_res28=new QLineEditX;
    lineEdite_res29=new QLineEditX;

    lineEdite_magic->setAlignment(Qt::AlignCenter);
    lineEdite_cblp->setAlignment(Qt::AlignCenter);
    lineEdite_cp->setAlignment(Qt::AlignCenter);
    lineEdite_crlc->setAlignment(Qt::AlignCenter);
    lineEdite_cparhdr->setAlignment(Qt::AlignCenter);
    lineEdite_minalloc->setAlignment(Qt::AlignCenter);
    lineEdite_maxalloc->setAlignment(Qt::AlignCenter);
    lineEdite_ss->setAlignment(Qt::AlignCenter);
    lineEdite_sp->setAlignment(Qt::AlignCenter);
    lineEdite_csum->setAlignment(Qt::AlignCenter);
    lineEdite_ip->setAlignment(Qt::AlignCenter);
    lineEdite_cs->setAlignment(Qt::AlignCenter);
    lineEdite_lfarlc->setAlignment(Qt::AlignCenter);
    lineEdite_ovno->setAlignment(Qt::AlignCenter);
    lineEdite_oemid->setAlignment(Qt::AlignCenter);
    lineEdite_oeminfo->setAlignment(Qt::AlignCenter);
    lineEdite_lfanew->setAlignment(Qt::AlignCenter);

    lineEdite_res0->setAlignment(Qt::AlignCenter);
    lineEdite_res1->setAlignment(Qt::AlignCenter);
    lineEdite_res2->setAlignment(Qt::AlignCenter);
    lineEdite_res3->setAlignment(Qt::AlignCenter);

    lineEdite_res20->setAlignment(Qt::AlignCenter);
    lineEdite_res21->setAlignment(Qt::AlignCenter);
    lineEdite_res22->setAlignment(Qt::AlignCenter);
    lineEdite_res23->setAlignment(Qt::AlignCenter);
    lineEdite_res24->setAlignment(Qt::AlignCenter);
    lineEdite_res25->setAlignment(Qt::AlignCenter);
    lineEdite_res26->setAlignment(Qt::AlignCenter);
    lineEdite_res27->setAlignment(Qt::AlignCenter);
    lineEdite_res28->setAlignment(Qt::AlignCenter);
    lineEdite_res29->setAlignment(Qt::AlignCenter);

    lineEdite_magic->setInputMask("HHHH");
    lineEdite_cblp->setInputMask("HHHH");
    lineEdite_cp->setInputMask("HHHH");
    lineEdite_crlc->setInputMask("HHHH");
    lineEdite_cparhdr->setInputMask("HHHH");
    lineEdite_minalloc->setInputMask("HHHH");
    lineEdite_maxalloc->setInputMask("HHHH");
    lineEdite_ss->setInputMask("HHHH");
    lineEdite_sp->setInputMask("HHHH");
    lineEdite_csum->setInputMask("HHHH");
    lineEdite_ip->setInputMask("HHHH");
    lineEdite_cs->setInputMask("HHHH");
    lineEdite_lfarlc->setInputMask("HHHH");
    lineEdite_ovno->setInputMask("HHHH");
    lineEdite_oemid->setInputMask("HHHH");
    lineEdite_oeminfo->setInputMask("HHHH");
    lineEdite_lfanew->setInputMask("HHHHHHHH");

    lineEdite_res0->setInputMask("HHHH");
    lineEdite_res1->setInputMask("HHHH");
    lineEdite_res2->setInputMask("HHHH");
    lineEdite_res3->setInputMask("HHHH");

    lineEdite_res20->setInputMask("HHHH");
    lineEdite_res21->setInputMask("HHHH");
    lineEdite_res22->setInputMask("HHHH");
    lineEdite_res23->setInputMask("HHHH");
    lineEdite_res24->setInputMask("HHHH");
    lineEdite_res25->setInputMask("HHHH");
    lineEdite_res26->setInputMask("HHHH");
    lineEdite_res27->setInputMask("HHHH");
    lineEdite_res28->setInputMask("HHHH");
    lineEdite_res29->setInputMask("HHHH");

    for(int i=0; i<31; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(sLabels[i]);
        ui->tableWidgetDosHeader->setItem(i,0,newItem);
        ui->tableWidgetDosHeader->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetDosHeader->setRowHeight(i,20);
    }

    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(0,1),lineEdite_magic);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(1,1),lineEdite_cblp);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(2,1),lineEdite_cp);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(3,1),lineEdite_crlc);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(4,1),lineEdite_cparhdr);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(5,1),lineEdite_minalloc);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(6,1),lineEdite_maxalloc);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(7,1),lineEdite_ss);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(8,1),lineEdite_sp);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(9,1),lineEdite_csum);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(10,1),lineEdite_ip);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(11,1),lineEdite_cs);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(12,1),lineEdite_lfarlc);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(13,1),lineEdite_ovno);

    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(14,1),lineEdite_res0);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(15,1),lineEdite_res1);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(16,1),lineEdite_res2);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(17,1),lineEdite_res3);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(18,1),lineEdite_oemid);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(19,1),lineEdite_oeminfo);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(20,1),lineEdite_res20);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(21,1),lineEdite_res21);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(22,1),lineEdite_res22);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(23,1),lineEdite_res23);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(24,1),lineEdite_res24);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(25,1),lineEdite_res25);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(26,1),lineEdite_res26);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(27,1),lineEdite_res27);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(28,1),lineEdite_res28);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(29,1),lineEdite_res29);
    ui->tableWidgetDosHeader->setIndexWidget(ui->tableWidgetDosHeader->model()->index(30,1),lineEdite_lfanew);

    ui->tableWidgetDosHeader->setColumnWidth(0,120);

    connect(lineEdite_magic,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_cblp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_cp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_crlc,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_cparhdr,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_minalloc,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_maxalloc,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_ss,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_sp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_csum,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_ip,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_cs,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_lfarlc,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_ovno,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_oemid,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_oeminfo,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_lfanew,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res0,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res1,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res2,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res3,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res20,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res21,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res22,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res23,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res24,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res25,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res26,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res27,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res28,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEdite_res29,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    ui->checkBoxReadOnly->setChecked(true);

    //    connect(&msdosfile,SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_hex));
}

DialogMSDOSHeader::~DialogMSDOSHeader()
{
    delete ui;
}

bool DialogMSDOSHeader::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));

        ui->pushButtonHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("MS DOS Header")));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg(tr("MS DOS Header")));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg(tr("MS DOS Header")));
    }

    nOffset=msdosfile->getDosHeaderOffset();
    nSize=msdosfile->getDosHeaderSize();

    ui->lineEditOffset->setText(QString("%1").arg(nOffset));
    ui->lineEditSize->setText(QString("%1").arg(nSize));

    lineEdite_magic->setText(QString("%1").arg(msdosfile->getDosHeader_magic(),4,16,QChar('0')));
    lineEdite_cblp->setText(QString("%1").arg(msdosfile->getDosHeader_cblp(),4,16,QChar('0')));
    lineEdite_cp->setText(QString("%1").arg(msdosfile->getDosHeader_cp(),4,16,QChar('0')));
    lineEdite_crlc->setText(QString("%1").arg(msdosfile->getDosHeader_crlc(),4,16,QChar('0')));
    lineEdite_cparhdr->setText(QString("%1").arg(msdosfile->getDosHeader_cparhdr(),4,16,QChar('0')));
    lineEdite_minalloc->setText(QString("%1").arg(msdosfile->getDosHeader_minalloc(),4,16,QChar('0')));
    lineEdite_maxalloc->setText(QString("%1").arg(msdosfile->getDosHeader_maxalloc(),4,16,QChar('0')));
    lineEdite_ss->setText(QString("%1").arg(msdosfile->getDosHeader_ss(),4,16,QChar('0')));
    lineEdite_sp->setText(QString("%1").arg(msdosfile->getDosHeader_sp(),4,16,QChar('0')));
    lineEdite_csum->setText(QString("%1").arg(msdosfile->getDosHeader_csum(),4,16,QChar('0')));
    lineEdite_ip->setText(QString("%1").arg(msdosfile->getDosHeader_ip(),4,16,QChar('0')));
    lineEdite_cs->setText(QString("%1").arg(msdosfile->getDosHeader_cs(),4,16,QChar('0')));
    lineEdite_lfarlc->setText(QString("%1").arg(msdosfile->getDosHeader_lfarlc(),4,16,QChar('0')));
    lineEdite_ovno->setText(QString("%1").arg(msdosfile->getDosHeader_ovno(),4,16,QChar('0')));
    lineEdite_oemid->setText(QString("%1").arg(msdosfile->getDosHeader_oemid(),4,16,QChar('0')));
    lineEdite_oeminfo->setText(QString("%1").arg(msdosfile->getDosHeader_oeminfo(),4,16,QChar('0')));
    lineEdite_lfanew->setText(QString("%1").arg(msdosfile->getDosHeader_lfanew(),8,16,QChar('0')));

    lineEdite_res0->setText(QString("%1").arg(msdosfile->getDosHeader_res(0),4,16,QChar('0')));
    lineEdite_res1->setText(QString("%1").arg(msdosfile->getDosHeader_res(1),4,16,QChar('0')));
    lineEdite_res2->setText(QString("%1").arg(msdosfile->getDosHeader_res(2),4,16,QChar('0')));
    lineEdite_res3->setText(QString("%1").arg(msdosfile->getDosHeader_res(3),4,16,QChar('0')));

    lineEdite_res20->setText(QString("%1").arg(msdosfile->getDosHeader_res2(0),4,16,QChar('0')));
    lineEdite_res21->setText(QString("%1").arg(msdosfile->getDosHeader_res2(1),4,16,QChar('0')));
    lineEdite_res22->setText(QString("%1").arg(msdosfile->getDosHeader_res2(2),4,16,QChar('0')));
    lineEdite_res23->setText(QString("%1").arg(msdosfile->getDosHeader_res2(3),4,16,QChar('0')));
    lineEdite_res24->setText(QString("%1").arg(msdosfile->getDosHeader_res2(4),4,16,QChar('0')));
    lineEdite_res25->setText(QString("%1").arg(msdosfile->getDosHeader_res2(5),4,16,QChar('0')));
    lineEdite_res26->setText(QString("%1").arg(msdosfile->getDosHeader_res2(6),4,16,QChar('0')));
    lineEdite_res27->setText(QString("%1").arg(msdosfile->getDosHeader_res2(7),4,16,QChar('0')));
    lineEdite_res28->setText(QString("%1").arg(msdosfile->getDosHeader_res2(8),4,16,QChar('0')));
    lineEdite_res29->setText(QString("%1").arg(msdosfile->getDosHeader_res2(9),4,16,QChar('0')));

    ui->checkBoxReadOnly->setEnabled(!msdosfile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogMSDOSHeader::on_pushButtonHex_clicked()
{
    DialogHDE dialoghde(this);
    dialoghde.setData(pOptions,msdosfile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("MS DOS Header"));

    dialoghde.exec();
}

void DialogMSDOSHeader::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    lineEdite_magic->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_cblp->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_cp->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_crlc->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_cparhdr->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_minalloc->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_maxalloc->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_ss->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_sp->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_csum->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_ip->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_cs->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_lfarlc->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_ovno->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_oemid->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_oeminfo->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_lfanew->setReadOnly(ui->checkBoxReadOnly->checkState());

    lineEdite_res0->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res1->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res2->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res3->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res20->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res21->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res22->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res23->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res24->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res25->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res26->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res27->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res28->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineEdite_res29->setReadOnly(ui->checkBoxReadOnly->checkState());
}

void DialogMSDOSHeader::on_pushButtonApply_clicked()
{
    bool bTemp;

    ui->pushButtonApply->setEnabled(false);

    msdosfile->setDosHeader_magic(lineEdite_magic->text().toUInt(&bTemp,16));

    msdosfile->setDosHeader_cblp(lineEdite_cblp->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_cp(lineEdite_cp->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_crlc(lineEdite_crlc->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_cparhdr(lineEdite_cparhdr->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_minalloc(lineEdite_minalloc->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_maxalloc(lineEdite_maxalloc->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_ss(lineEdite_ss->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_sp(lineEdite_sp->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_csum(lineEdite_csum->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_ip(lineEdite_ip->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_cs(lineEdite_cs->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_lfarlc(lineEdite_lfarlc->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_ovno(lineEdite_ovno->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_oemid(lineEdite_oemid->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_oeminfo(lineEdite_oeminfo->text().toUInt(&bTemp,16));
    msdosfile->setDosHeader_lfanew(lineEdite_lfanew->text().toUInt(&bTemp,16));

    msdosfile->setDosHeader_res(lineEdite_res0->text().toUInt(&bTemp,16),0);
    msdosfile->setDosHeader_res(lineEdite_res1->text().toUInt(&bTemp,16),1);
    msdosfile->setDosHeader_res(lineEdite_res2->text().toUInt(&bTemp,16),2);
    msdosfile->setDosHeader_res(lineEdite_res3->text().toUInt(&bTemp,16),3);

    msdosfile->setDosHeader_res2(lineEdite_res20->text().toUInt(&bTemp,16),0);
    msdosfile->setDosHeader_res2(lineEdite_res21->text().toUInt(&bTemp,16),1);
    msdosfile->setDosHeader_res2(lineEdite_res22->text().toUInt(&bTemp,16),2);
    msdosfile->setDosHeader_res2(lineEdite_res23->text().toUInt(&bTemp,16),3);
    msdosfile->setDosHeader_res2(lineEdite_res24->text().toUInt(&bTemp,16),4);
    msdosfile->setDosHeader_res2(lineEdite_res25->text().toUInt(&bTemp,16),5);
    msdosfile->setDosHeader_res2(lineEdite_res26->text().toUInt(&bTemp,16),6);
    msdosfile->setDosHeader_res2(lineEdite_res27->text().toUInt(&bTemp,16),7);
    msdosfile->setDosHeader_res2(lineEdite_res28->text().toUInt(&bTemp,16),8);
    msdosfile->setDosHeader_res2(lineEdite_res29->text().toUInt(&bTemp,16),9);

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogMSDOSHeader::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogMSDOSHeader::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogMSDOSHeader::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}
