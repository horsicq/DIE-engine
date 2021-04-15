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
#include "guimainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include "xsingleapplication.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#ifdef Q_OS_MAC
#ifndef QT_DEBUG
    QString sLibraryPath=QString(argv[0]);
    sLibraryPath=sLibraryPath.remove("MacOS/die")+"PlugIns";
    QCoreApplication::setLibraryPaths(QStringList(sLibraryPath));
#endif
#endif

    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    XSingleApplication app(argc,argv);

    XOptions xOptions;

    xOptions.setName(X_OPTIONSFILE);

    QList<XOptions::ID> listIDs;

    listIDs.append(XOptions::ID_STYLE);
    listIDs.append(XOptions::ID_LANG);
    listIDs.append(XOptions::ID_QSS);
    listIDs.append(XOptions::ID_SINGLEAPPLICATION);

    xOptions.setValueIDs(listIDs);
    xOptions.load();

    if(xOptions.isSingleApplication())
    {
        app.enableSingleInstance();
    }

    int nResult=0;

    if(app.isPrimary())
    {
        XOptions::adjustApplicationView(X_APPLICATIONNAME,&xOptions);

        GuiMainWindow mainWindow;

        QObject::connect(&app,SIGNAL(messageText(QString)),&mainWindow,SLOT(processFile(QString)));

        mainWindow.show();

        nResult=app.exec();
    }

    return nResult;
}
