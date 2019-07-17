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
#include <QApplication>
#include <QtSingleApplication>
#include <QDebug>
#include <QTextCodec>
#include "handleoptions.h"
#include "mainwindow.h"

#ifdef Q_OS_WIN
#ifdef QT_DEBUG
//#include "../../../VisualLeakDetector/include/vld.h"
#endif
#endif

int main(int argc, char *argv[])
{
    //    qDebug("Start application Argc %d %s",argc,argc==2?argv[1]:"no ars");
#if QT_VERSION_MINOR >= 0x06
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    //#ifdef Q_OS_MAC
    //    QStringList listPlugins;
    //    listPlugins.append(qApp->applicationDirPath()+"/../PlugIns");
    //    qApp->setLibraryPaths(listPlugins);
    //#endif
#ifdef Q_OS_MAC
#ifndef QT_DEBUG
    QString sLibraryPath=QString(argv[0]);
    sLibraryPath=sLibraryPath.remove("MacOS/die")+"PlugIns";
    QCoreApplication::setLibraryPaths(QStringList(sLibraryPath));
    //qDebug(sLibraryPath.toLatin1().data());
#endif
#endif
    QtSingleApplication app(argc, argv);

    //    qDebug(QLocale::system().name().toAscii().data());
    //#ifdef QT_DEBUG
    //        QStringList list=QLocale::system().uiLanguages();
    //        qDebug()<<list;
    //#endif
    //    int nFontStatus=QFontDatabase::addApplicationFont(":/fonts/windows_latin.ttf");

    QTranslator translator;
    __DIE_OPTIONS _options= {0};
    HandleOptions::load(&_options);

    if(_options.bEnableLanguage)
    {
        QString sLangsPath=Utils::convertPath(_options.sLangsPath.replace("/",QDir::separator()));

        bool bLoad=false;

        if((_options.sLanguage=="")||(_options.sLanguage=="default"))
        {
            bLoad=translator.load(QLocale::system(),"die","_",sLangsPath,".qm");
        }
        else
        {
            bLoad=translator.load(_options.sLanguage,sLangsPath);
        }

        if(bLoad)
        {
            qApp->installTranslator(&translator);
        }
        //        else
        //        {
        //            _options.sLanguage="default";
        //        }
    }

    if(_options.sCodec!="")
    {
#if (QT_VERSION_MAJOR<5)
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(_options.sCodec.toLatin1().data()));
#endif
    }

    if(_options.sStyle!="")
    {
        qApp->setStyle(_options.sStyle);
    }

    if(_options.sStyleSheet!="")
    {
        QString sQSSPath=Utils::convertPath(_options.sQSSPath.replace("/",QDir::separator()));
        QFile file;
        file.setFileName(sQSSPath+QDir::separator()+QString("%1.qss").arg(_options.sStyleSheet));

        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray baQss=file.readAll();
            qApp->setStyleSheet(baQss.data());
            file.close();
        }
    }

    if(_options.bEnableFont&&(_options.sFont!=""))
    {
        QFont _Font;
        _Font.fromString(_options.sFont);
        qApp->setFont(_Font);
    }

    // TODO
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("ISO 8859-5"));
    //    QTextCodec::setCodecForTr(QTextCodec::codecForName("ISO 8859-5"));

    //    HandleOptions::save(&_options);

    MainWindow w;

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),&w, SLOT(openFile(const QString&)));

    if(_options.bSingleApplication)
    {
        if(app.isRunning())
        {
            if(QCoreApplication::arguments().count()>1)
            {

                app.sendMessage(QCoreApplication::arguments().at(1));
            }

            //app.sendMessage("Test");
            return 0;
        }
    }

    if(_options.bSingleApplication)
    {
        app.setActivationWindow(&w);
    }

    w.show();

    return app.exec();
}
