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
#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    qsrand(QTime::currentTime().msec());

    //    ui->labelVersion->setText(__VERSION);

    QString sInfoText=ui->labelInfo->text();

    sInfoText=sInfoText.replace("#VERSION#",QString("%1 %2").arg(__DIE).arg(__VERSION));
    sInfoText=sInfoText.replace("#DATE#",__BUILDDATE);

    ui->labelInfo->setText(sInfoText);

    QString sComponentsText=ui->labelComponents->text();

    sComponentsText=sComponentsText.replace("#QT_VERSION#",QT_VERSION_STR);
    sComponentsText=sComponentsText.replace("#QWT_VERSION#",QWT_VERSION_STR);

    ui->labelComponents->setText(sComponentsText);

    m_manager=new QNetworkAccessManager(this);

    connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    shortcut1=new QShortcut(QKeySequence(__KeySequence_tab1), this);
    QObject::connect(shortcut1, SIGNAL(activated()), this, SLOT(tabInfo()));
    shortcut2=new QShortcut(QKeySequence(__KeySequence_tab2), this);
    QObject::connect(shortcut2, SIGNAL(activated()), this, SLOT(tabLibs()));
    shortcut3=new QShortcut(QKeySequence(__KeySequence_tab3), this);
    QObject::connect(shortcut3, SIGNAL(activated()), this, SLOT(tabThanks()));

    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonCheckUpdate->setShortcut(QKeySequence(__KeySequence_altU));
    ui->pushButtonAvatar->setShortcut(QKeySequence(__KeySequence_altR));

    on_pushButtonAvatar_clicked();

    tabInfo();
}

DialogAbout::~DialogAbout()
{
    delete(m_manager);

    delete(shortcut1);
    delete(shortcut2);
    delete(shortcut3);

    delete ui;
}

void DialogAbout::setData(__DIE_OPTIONS *pOptions)
{
    setOptions(pOptions);
}

void DialogAbout::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogAbout::on_pushButtonCheckUpdate_clicked()
{
    m_manager->get(QNetworkRequest(QUrl(__UPDATEFILE)));
    ui->pushButtonCheckUpdate->setEnabled(false);
}

void DialogAbout::replyFinished(QNetworkReply* pReply)
{
    ui->pushButtonCheckUpdate->setEnabled(true);

    if(pReply->error()==QNetworkReply::NoError)
    {
        QByteArray data=pReply->readAll();
        QString _sVersion(data);

        QString sVersion=_sVersion.section(" ",-1,-1);

        if(sVersion.toDouble()<=QString(__VERSION).toDouble())
        {
            QMessageBox::information(this,tr("Update information"),tr("No update available"));
        }
        else
        {
            if(QMessageBox::information(this,tr("Update information"),QString("%1\r\n\r\n%2\r\n\r\n%3").arg(tr("New version available")).arg(sVersion).arg(tr("Go to download page?")),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl(__HOMEPAGE));
            }
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Network error"),pReply->errorString());
    }
}

void DialogAbout::tabInfo()
{
    ui->tabWidgetAbout->setCurrentIndex(0);
}

void DialogAbout::tabLibs()
{
    ui->tabWidgetAbout->setCurrentIndex(1);
}

void DialogAbout::tabThanks()
{
    ui->tabWidgetAbout->setCurrentIndex(2);
}

void DialogAbout::on_listWidgetThanks_currentTextChanged(const QString &currentText)
{
    if(currentText=="Nick Sm")
    {
        setButtonImage(":/new/avatars/pics/avatars/NickSM.bmp");
    }
    else if(currentText=="hypn0")
    {
        setButtonImage(":/new/avatars/pics/avatars/hypn0.bmp");
    }
    else if(currentText=="ajax")
    {
        setButtonImage(":/new/avatars/pics/avatars/ajax.bmp");
    }
    else if(currentText=="==DJ==[ZLO]")
    {
        setButtonImage(":/new/avatars/pics/avatars/DrZlo.bmp");
    }
    else if(currentText=="ADMIN-CRACK")
    {
        setButtonImage(":/new/avatars/pics/avatars/ADMINCRACK.bmp");
    }
    else if(currentText=="mushr00m")
    {
        setButtonImage(":/new/avatars/pics/avatars/mushr00m.bmp");
    }
    else if(currentText=="gurololicon")
    {
        setButtonImage(":/new/avatars/pics/avatars/gurololicon.bmp");
    }
    else if(currentText=="Diabolic")
    {
        setButtonImage(":/new/avatars/pics/avatars/Diabolic.bmp");
    }
    else if(currentText=="SkyRE")
    {
        setButtonImage(":/new/avatars/pics/avatars/SkyRE.bmp");
    }
    else if(currentText=="user_")
    {
        setButtonImage(":/new/avatars/pics/avatars/user_.bmp");
    }
    else if(currentText=="void")
    {
        setButtonImage(":/new/avatars/pics/avatars/void.bmp");
    }
    else if(currentText=="NikolayD")
    {
        setButtonImage(":/new/avatars/pics/avatars/NikolayD.bmp");
    }
    else if(currentText=="4kusNick")
    {
        setButtonImage(":/new/avatars/pics/avatars/4kusNick.bmp");
    }
    else if(currentText=="BoRoV")
    {
        setButtonImage(":/new/avatars/pics/avatars/BoRoV.bmp");
    }
    else if(currentText=="HandMill")
    {
        setButtonImage(":/new/avatars/pics/avatars/HandMill.bmp");
    }
    else if(currentText=="ELF_7719116")
    {
        setButtonImage(":/new/avatars/pics/avatars/ELF_7719116.bmp");
    }
    else if(currentText=="SReg")
    {
        setButtonImage(":/new/avatars/pics/avatars/SReg.bmp");
    }
    else if(currentText=="Zorn")
    {
        setButtonImage(":/new/avatars/pics/avatars/Zorn.bmp");
    }
    else if(currentText=="BAHEK")
    {
        setButtonImage(":/new/avatars/pics/avatars/BAHEK.bmp");
    }
    else if(currentText=="GPcH")
    {
        setButtonImage(":/new/avatars/pics/avatars/GPcH.bmp");
    }
    else if(currentText=="Airenikus")
    {
        setButtonImage(":/new/avatars/pics/avatars/Airenikus.bmp");
    }
    else if(currentText=="crc1")
    {
        setButtonImage(":/new/avatars/pics/avatars/crc1.bmp");
    }
    else if(currentText=="DimitarSerg")
    {
        setButtonImage(":/new/avatars/pics/avatars/DimitarSerg.bmp");
    }
    else if(currentText=="RedBlkJck")
    {
        setButtonImage(":/new/avatars/pics/avatars/RedBlkJck.bmp");
    }
    else if(currentText=="JMPer")
    {
        setButtonImage(":/new/avatars/pics/avatars/JMPer.bmp");
    }
    else if(currentText=="exet0l")
    {
        setButtonImage(":/new/avatars/pics/avatars/exet0l.bmp");
    }
    else if(currentText=="Mick Grove")
    {
        setButtonImage(":/new/avatars/pics/avatars/MickGrove.bmp");
    }
    else if(currentText=="Levis")
    {
        setButtonImage(":/new/avatars/pics/avatars/Levis.bmp");
    }
    else if(currentText=="sendersu")
    {
        setButtonImage(":/new/avatars/pics/avatars/sendersu.bmp");
    }
    else if(currentText=="ekse0x")
    {
        setButtonImage(":/new/avatars/pics/avatars/ekse0x.bmp");
    }
    else if(currentText=="Slinger")
    {
        setButtonImage(":/new/avatars/pics/avatars/Slinger.bmp");
    }
    else if(currentText=="DenCoder")
    {
        setButtonImage(":/new/avatars/pics/avatars/DenCoder.bmp");
    }
    else if(currentText=="deniskore")
    {
        setButtonImage(":/new/avatars/pics/avatars/deniskore.bmp");
    }
    else if(currentText=="Hellspawn")
    {
        setButtonImage(":/new/avatars/pics/avatars/hellspawn.bmp");
    }
    else if(currentText=="Dazz")
    {
        setButtonImage(":/new/avatars/pics/avatars/Dazz.bmp");
    }
    else if(currentText=="A.S.L.")
    {
        setButtonImage(":/new/avatars/pics/avatars/ASL.bmp");
    }
    else if(currentText=="DICI BF")
    {
        setButtonImage(":/new/avatars/pics/avatars/DICIBF.bmp");
    }
    else if(currentText=="Javier Nieto")
    {
        setButtonImage(":/new/avatars/pics/avatars/JavierNieto.bmp");
    }
    else if(currentText=="Jason Hood")
    {
        setButtonImage(":/new/avatars/pics/avatars/JasonHood.bmp");
    }
    else if(currentText=="Tembo")
    {
        setButtonImage(":/new/avatars/pics/avatars/Tembo.bmp");
    }
    else if(currentText.section(" ",0,0)=="Bartosz")
    {
        setButtonImage(":/new/avatars/pics/avatars/Bartosz.bmp");
    }
    else if(currentText=="Jeffrey Wu")
    {
        setButtonImage(":/new/avatars/pics/avatars/JeffreyWu.bmp");
    }
    else if(currentText=="Murcielago")
    {
        setButtonImage(":/new/avatars/pics/avatars/Murcielago.bmp");
    }
    else if(currentText=="Eliseu Filipi")
    {
        setButtonImage(":/new/avatars/pics/avatars/EliseuFilipi.bmp");
    }
    else if(currentText=="dartraiden")
    {
        setButtonImage(":/new/avatars/pics/avatars/dartraiden.bmp");
    }
    else if(currentText=="Jupiter")
    {
        setButtonImage(":/new/avatars/pics/avatars/Jupiter.bmp");
    }
    else if(currentText=="d3dm4u5")
    {
        setButtonImage(":/new/avatars/pics/avatars/d3dm4u5.bmp");
    }
    else if(currentText=="Metabolic")
    {
        setButtonImage(":/new/avatars/pics/avatars/Metabolic.bmp");
    }
    else if(currentText=="Adler")
    {
        setButtonImage(":/new/avatars/pics/avatars/Adler.bmp");
    }
    else if(currentText=="mrexodia")
    {
        setButtonImage(":/new/avatars/pics/avatars/mrexodia.bmp");
    }
    else if(currentText=="Rinat Aminow")
    {
        setButtonImage(":/new/avatars/pics/avatars/RinatAminow.bmp");
    }
    else if(currentText=="ktg kbh")
    {
        setButtonImage(":/new/avatars/pics/avatars/ktgkbh.bmp");
    }
}

void DialogAbout::setButtonImage(QString sImageName)
{
    QPixmap pixmap(sImageName);
    QIcon ButtonIcon(pixmap);
    ui->pushButtonAvatar->setIcon(ButtonIcon);
    ui->pushButtonAvatar->setIconSize(pixmap.rect().size());
}

void DialogAbout::on_pushButtonAvatar_clicked()
{
    int nRandom=qrand()%(ui->listWidgetThanks->count());

    ui->listWidgetThanks->setCurrentRow(nRandom);
}

bool DialogAbout::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonCheckUpdate->setToolTip(tr("Check update"));
        ui->pushButtonOK->setToolTip(tr("Quit"));
        ui->tabWidgetAbout->setTabToolTip(0,QString("%1(Alt+1)").arg(tr("Info")));
        ui->tabWidgetAbout->setTabToolTip(1,QString("%1(Alt+2)").arg(tr("Libraries")));
        ui->tabWidgetAbout->setTabToolTip(2,QString("%1(Alt+3)").arg(tr("Thanks")));
        ui->pushButtonAvatar->setToolTip(tr("Avatar"));
        ui->labelComponents->setToolTip(tr("Used libraries"));
        ui->labelInfo->setToolTip(tr("Information"));
        ui->pushButtonLogo->setToolTip(tr("Logo"));
    }

    return true;
}
