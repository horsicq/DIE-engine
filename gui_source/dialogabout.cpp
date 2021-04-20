// Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#include "dialogabout.h"
#include "ui_dialogabout.h"

struct USERINFO
{
    QString sUser;
    QString sImage;
};

USERINFO _userinfo_records[]=
{
    {QString("Nick Sm"),                QString("NickSM")},
    {QString("hypn0"),                  QString("hypn0")},
    {QString("ajax"),                   QString("ajax")},
    {QString("==DJ==[ZLO]"),            QString("DrZlo")},
    {QString("ADMIN-CRACK"),            QString("ADMINCRACK")},
    {QString("mushr00m"),               QString("mushr00m")},
    {QString("gurololicon"),            QString("gurololicon")},
    {QString("Diabolic"),               QString("Diabolic")},
    {QString("user_"),                  QString("user_")},
    {QString("void"),                   QString("void")},
    {QString("NikolayD"),               QString("NikolayD")},
    {QString("4kusNick"),               QString("4kusNick")},
    {QString("BoRoV"),                  QString("BoRoV")},
    {QString("HandMill"),               QString("HandMill")},
    {QString("ELF_7719116"),            QString("ELF_7719116")},
    {QString("SReg"),                   QString("SReg")},
    {QString("Zorn"),                   QString("Zorn")},
    {QString("BAHEK"),                  QString("BAHEK")},
    {QString("GPcH"),                   QString("GPcH")},
    {QString("Airenikus"),              QString("Airenikus")},
    {QString("crc1"),                   QString("crc1")},
    {QString("DimitarSerg"),            QString("DimitarSerg")},
    {QString("RedBlkJck"),              QString("RedBlkJck")},
    {QString("JMPer"),                  QString("JMPer")},
    {QString("exet0l"),                 QString("exet0l")},
    {QString("Mick Grove"),             QString("MickGrove")},
    {QString("Levis"),                  QString("Levis")},
    {QString("sendersu"),               QString("sendersu")},
    {QString("ekse0x"),                 QString("ekse0x")},
    {QString("Slinger"),                QString("Slinger")},
    {QString("DenCoder"),               QString("DenCoder")},
    {QString("deniskore"),              QString("deniskore")},
    {QString("Hellsp@wn"),              QString("hellspawn")},
    {QString("Dazz"),                   QString("Dazz")},
    {QString("A.S.L."),                 QString("ASL")},
    {QString("DICI BF"),                QString("DICIBF")},
    {QString("Javier Nieto"),           QString("JavierNieto")},
    {QString("Jason Hood"),             QString("JasonHood")},
    {QString("Tembo"),                  QString("Tembo")},
    {QString("Bartosz Wójcik"),         QString("Bartosz")},
    {QString("Jeffrey Wu"),             QString("JeffreyWu")},
    {QString("Murcielago"),             QString("Murcielago")},
    {QString("Eliseu Filipi"),          QString("EliseuFilipi")},
    {QString("dartraiden"),             QString("dartraiden")},
    {QString("Jupiter"),                QString("Jupiter")},
    {QString("d3dm4u5"),                QString("d3dm4u5")},
    {QString("Metabolic"),              QString("Metabolic")},
    {QString("Adler"),                  QString("Adler")},
    {QString("mrexodia"),               QString("mrexodia")},
    {QString("Rinat Aminow"),           QString("RinatAminow")},
    {QString("ktg kbh"),                QString("ktgkbh")},
    {QString("David Zimmer"),           QString("DavidZimmer")},
    {QString("Fernando Mercês"),        QString("FernandoMerces")},
    {QString("JuJu2015"),               QString("JuJu2015")},
    {QString("Nomade"),                 QString("Nomade")},
    {QString("JaeHuyng Lee"),           QString("JaeHuyngLee")},
    {QString("misonothx"),              QString("misonothx")},
    {QString("Elite_r"),                QString("Elite_r")},
};

DialogAbout::DialogAbout(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    ui->tabWidgetAbout->setCurrentIndex(0);

    QString sInfoText=ui->labelInfo->text();

    sInfoText=sInfoText.replace("#VERSION#",QString("%1 v%2").arg(X_APPLICATIONDISPLAYNAME).arg(X_APPLICATIONVERSION));
    sInfoText=sInfoText.replace("#DATE#",__DATE__);

    ui->labelInfo->setText(sInfoText);

    QString sComponentsText=ui->labelComponents->text();

    sComponentsText=sComponentsText.replace("#QT#",QT_VERSION_STR);
    sComponentsText=sComponentsText.replace("#QWT#",QWT_VERSION_STR);
    sComponentsText=sComponentsText.replace("#CAPSTONE#",QString("%1.%2.%3").arg(CS_VERSION_MAJOR).arg(CS_VERSION_MINOR).arg(CS_VERSION_EXTRA));

    ui->labelComponents->setText(sComponentsText);

    int nCount=sizeof(_userinfo_records)/sizeof(USERINFO);

    {
        QSignalBlocker blocker1(ui->listWidgetThanks);

        for(int i=0;i<nCount;i++)
        {
            QListWidgetItem *pItem=new QListWidgetItem;
            pItem->setText(_userinfo_records[i].sUser);
            pItem->setData(Qt::UserRole,QString("://images/avatars/%1.png").arg(_userinfo_records[i].sImage));

            ui->listWidgetThanks->insertItem(i,pItem);
        }
    }

    if(nCount)
    {
        random();
    }
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogAbout::on_pushButtonCheckUpdates_clicked()
{
    QDesktopServices::openUrl(QUrl(X_UPDATEPAGE));
}

void DialogAbout::on_listWidgetThanks_currentItemChanged(QListWidgetItem *pCurrent, QListWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        QString sImagePath=pCurrent->data(Qt::UserRole).toString();

        QPixmap pixmap(sImagePath);
        QIcon buttonIcon(pixmap);
        ui->pushButtonAvatar->setIcon(buttonIcon);
        ui->pushButtonAvatar->setIconSize(pixmap.rect().size());
    }
}

void DialogAbout::on_pushButtonAvatar_clicked()
{
    random();
}

void DialogAbout::random()
{
    int nCount=sizeof(_userinfo_records)/sizeof(USERINFO);

    if(nCount)
    {
        ui->listWidgetThanks->setCurrentRow(XBinary::random(nCount));
    }
}
