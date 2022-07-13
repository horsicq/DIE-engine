/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "dialogabout.h"
#include "ui_dialogabout.h"

struct USERINFO
{
    QString sUser;
    QString sImage;
};

USERINFO _userinfo_records[]=
{
    {QString("4kusNick"),               QString("4kusNick")},
    {QString("==DJ==[ZLO]"),            QString("DrZlo")},
    {QString("A.S.L."),                 QString("ASL")},
    {QString("ADMIN-CRACK"),            QString("ADMINCRACK")},
    {QString("Adam(Hexacorn)"),         QString("Hexacorn")},
    {QString("Adler"),                  QString("Adler")},
    {QString("Adric Net"),              QString("AdricNet")},
    {QString("Airenikus"),              QString("Airenikus")},
    {QString("Ali Hadi"),               QString("AliHadi")},
    {QString("Anderson Leite"),         QString("AndersonLeite")},
    {QString("Arnaud Dovi(class101)"),  QString("ArnaudDovi")},
    {QString("BAHEK"),                  QString("BAHEK")},
    {QString("Bartosz Wójcik"),         QString("Bartosz")},
    {QString("BoRoV"),                  QString("BoRoV")},
    {QString("Christopher Layne"),      QString("clayne")},
    {QString("DICI BF"),                QString("DICIBF")},
    {QString("David Zimmer"),           QString("DavidZimmer")},
    {QString("Dazz"),                   QString("Dazz")},
    {QString("DenCoder"),               QString("DenCoder")},
    {QString("Diabolic"),               QString("Diabolic")},
    {QString("DimitarSerg"),            QString("DimitarSerg")},
    {QString("Dr.FarFar"),              QString("DrFarFar")},
    {QString("ELF_7719116"),            QString("ELF_7719116")},
    {QString("Eliseu Filipi"),          QString("EliseuFilipi")},
    {QString("Elite_r"),                QString("Elite_r")},
    {QString("Fernando Mercês"),        QString("FernandoMerces")},
    {QString("Filip Navara"),           QString("FilipNavara")},
    {QString("FrenchYeti"),             QString("FrenchYeti")},
    {QString("GPcH"),                   QString("GPcH")},
    {QString("Gilad Reich"),            QString("GiladReich")},
    {QString("HandMill"),               QString("HandMill")},
    {QString("Hellsp@wn"),              QString("hellspawn")},
    {QString("JMPer"),                  QString("JMPer")},
    {QString("JaeHuyng Lee"),           QString("JaeHuyngLee")},
    {QString("Jason Hood"),             QString("JasonHood")},
    {QString("Javi VR"),                QString("JaviVR")},
    {QString("Javier Nieto"),           QString("JavierNieto")},
    {QString("Jeffrey Wu"),             QString("JeffreyWu")},
    {QString("Jiří Vinopal"),           QString("JiriVinopal")},
    {QString("João Vitor"),             QString("JoaoVitor")},
    {QString("JuJu2015"),               QString("JuJu2015")},
    {QString("Jupiter"),                QString("Jupiter")},
    {QString("Kaens"),                  QString("Kaens")},
    {QString("Leandro Fróes"),          QString("LeandroFroes")},
    {QString("Levis"),                  QString("Levis")},
    {QString("LinXP"),                  QString("LinXP")},
    {QString("Metabolic"),              QString("Metabolic")},
    {QString("Mick Grove"),             QString("MickGrove")},
    {QString("Murcielago"),             QString("Murcielago")},
    {QString("Nick Sm"),                QString("NickSM")},
    {QString("NikolayD"),               QString("NikolayD")},
    {QString("Nomade"),                 QString("Nomade")},
    {QString("Nông Hoàng Tú"),          QString("NongHoangTu")},
    {QString("RedBlkJck"),              QString("RedBlkJck")},
    {QString("Rinat Aminow"),           QString("RinatAminow")},
    {QString("SReg"),                   QString("SReg")},
    {QString("Slinger"),                QString("Slinger")},
    {QString("Tembo"),                  QString("Tembo")},
    {QString("Tilt"),                   QString("Tilt")},
    {QString("Zorn"),                   QString("Zorn")},
    {QString("ajax"),                   QString("ajax")},
    {QString("crc1"),                   QString("crc1")},
    {QString("d3dm4u5"),                QString("d3dm4u5")},
    {QString("dartraiden"),             QString("dartraiden")},
    {QString("deniskore"),              QString("deniskore")},
    {QString("ekse0x"),                 QString("ekse0x")},
    {QString("exet0l"),                 QString("exet0l")},
    {QString("fr0zenbag"),              QString("fr0zenbag")},
    {QString("graysuit"),               QString("Gray")},
    {QString("gurololicon"),            QString("gurololicon")},
    {QString("hypn0"),                  QString("hypn0")},
    {QString("ktg kbh"),                QString("ktgkbh")},
    {QString("misonothx"),              QString("misonothx")},
    {QString("mrexodia"),               QString("mrexodia")},
    {QString("mushr00m"),               QString("mushr00m")},
    {QString("phithon"),                QString("phithon")},
    {QString("sendersu"),               QString("sendersu")},
    {QString("user_"),                  QString("user_")},
    {QString("void"),                   QString("void")},
};

DialogAbout::DialogAbout(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    XAboutWidget::DATA _data={};

    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">%1</span></p>")
            .arg(XOptions::getTitle(X_APPLICATIONDISPLAYNAME,X_APPLICATIONVERSION));
    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">Copyright (C) 2006-2008 Hellsp@wn</span></p>");
    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">Copyright (C) 2012-2022 Hors</span></p>");
    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">%1: </span><a href=\"mailto:horsicq@gmail.com\"><span style=\" text-decoration: underline; color:#ff0000;\">horsicq@gmail.com</span></a></p>")
            .arg(tr("Bugreports"));
    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">%1: </span><a href=\"http://ntinfo.biz\"><span style=\" text-decoration: underline; color:#ff0000;\">http://ntinfo.biz</span></a></p>")
            .arg(tr("Website"));
    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">%1(Paypal): </span><a href=\"ntinfo.re@gmail.com\"><span style=\" text-decoration: underline; color:#ff0000;\">ntinfo.re@gmail.com</span></a></p>")
            .arg(tr("Donate"));
    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">%1(BTC): </span><a href=\"3DqddVBX9PKqMvNPXZ3gPHBNNRtD9CnmJo\"><span style=\" text-decoration: underline; color:#ff0000;\">3DqddVBX9PKqMvNPXZ3gPHBNNRtD9CnmJo</span></a></p>")
            .arg(tr("Donate"));
    _data.sInfo+=QString("<p align=\"center\"><span style=\" font-weight:600;\">%1: </span><a href=\"https://github.com/horsicq/DIE-engine\"><span style=\" text-decoration: underline; color:#ff0000;\">https://github.com/horsicq/DIE-engine</span></a></p>")
            .arg(tr("Source code"));

    _data.sLibraries+=QString("<p align=\"center\"><span style=\" font-weight:600;\">QT Library %1 </span><a href=\"http://qt-project.org\"><span style=\" text-decoration: underline; color:#ff0000;\">http://qt-project.org</span></a></p>")
            .arg(QT_VERSION_STR);
    _data.sLibraries+=QString("<p align=\"center\"><span style=\" font-weight:600;\">QWT Library %1 </span><a href=\"http://qwt.sourceforge.net\"><span style=\" text-decoration: underline; color:#ff0000;\">http://qwt.sourceforge.net</span></a></p>")
            .arg(QWT_VERSION_STR);
    _data.sLibraries+=QString("<p align=\"center\"><span style=\" font-weight:600;\">Capstone %1.%2.%3 </span><a href=\"http://www.capstone-engine.org\"><span style=\" text-decoration: underline; color:#ff0000;\">http://www.capstone-engine.org</span></a></p>")
            .arg(QString::number(CS_VERSION_MAJOR),QString::number(CS_VERSION_MINOR),QString::number(CS_VERSION_EXTRA));

//    _data.sLogoPath=":/images/about.png";
    _data.sUpdatesLink="https://github.com/horsicq/DIE-engine/releases";

#ifndef QT_DEBUG
    _data.sThanksPath=XOptions::getApplicationDataPath()+QDir::separator()+"images"+QDir::separator()+"thanks";
#else
    QDir dir(XOptions::getApplicationDataPath());

    dir.cdUp();
    dir.cdUp();

    _data.sThanksPath=dir.absolutePath()+QDir::separator()+"images"+QDir::separator()+"thanks";
#endif

    ui->widgetAbout->setData(_data);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushButtonOK_clicked()
{
    this->close();
}
