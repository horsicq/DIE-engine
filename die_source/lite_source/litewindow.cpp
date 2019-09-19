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
#include "litewindow.h"
#include "ui_litewindow.h"

LiteWindow::LiteWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LiteWindow)
{
    ui->setupUi(this);

#if  defined(_WIN32)

#else
    ui->checkBoxAddToContextMenu->setEnabled(false);
#endif

    this->setWindowTitle(QString("%1 %2 Lite").arg(__DIE).arg(__VERSION));

    options.bShowErrors=false;
    options.bFullScan=false;
    options.bScanShowOptionsDIE=true;
    options.bScanShowVersionDIE=true;
    options.bShowScanTime=false;
    options.sm=SM_DIE;
    options.pMutexResult=0;

    Scan::die_loadScripts(&options);

    ui->checkBoxAddToContextMenu->setChecked(getShellState());

    setAcceptDrops(true);

    if(QCoreApplication::arguments().count()>1)
    {
        openFile(QCoreApplication::arguments().at(1));
    }
}

LiteWindow::~LiteWindow()
{
    delete ui;
}

void LiteWindow::on_pushButtonOpen_clicked()
{
    QString sFileName;

    sFileName = QFileDialog::getOpenFileName(this, tr("Open file..."),"", tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        ui->lineEditFileName->setText("");
        ui->textEdit->clear();

        openFile(sFileName);
    }
}

void LiteWindow::openFile(QString sFileName)
{
    QString sTemp;

    options.bScanDeepScanDIE=ui->checkBoxDeepScan->isChecked();

    ui->textEdit->clear();

    //    ui->tableWidgetResult->clear();
    //    ui->tableWidgetResult->setRowCount(0);
    //    ui->lineEditScanTime->setText("");

    if(!sFileName.isEmpty())
    {
        QFileInfo fi;

        while(true)
        {
            fi.setFile(sFileName);

            if(!fi.exists())
            {
                sTemp=sFileName;
                fi.setFile(sTemp.replace("Program Files (x86)","Program Files"));

                if(fi.exists())
                {
                    sFileName=fi.filePath();
                }
            }

            if(fi.isSymLink())
            {
                sFileName=fi.symLinkTarget();
            }
            else
            {
                break;
            }
        }

        QDir dir;
        dir.setPath(sFileName);

        if(dir.exists())
        {
            //            emit directory(sFileName);
        }
        else
        {
            ui->lineEditFileName->setText(sFileName);

            scan=new Scan;
            QThread* thread =new QThread;
            scan->moveToThread(thread);

            QObject::connect(scan,SIGNAL(die_appendSignatureSignal(QString)),this,SLOT(appendSignature(QString)));
            QObject::connect(scan,SIGNAL(appendError(QString)),this,SLOT(appendError(QString)));

            connect(thread, SIGNAL(started()), scan, SLOT(process()));
            connect(scan, SIGNAL(_finished()), thread, SLOT(quit()));
            connect(scan, SIGNAL(_finished()), scan, SLOT(deleteLater()));
            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

            scan->setData(sFileName,&options);

            thread->start();
        }
    }
}

void LiteWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void LiteWindow::appendSignature(QString sString)
{
    ui->textEdit->append(sString);
}

void LiteWindow::appendError(QString sString)
{
    if(options.bShowErrors)
    {
        appendSignature(QString("<font color=\"red\">[Error]%1</font>").arg(sString));
    }
}


void LiteWindow::on_checkBoxShowErrors_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    if(ui->checkBoxShowErrors->checkState())
    {
        options.bShowErrors=true;
    }
    else
    {
        options.bShowErrors=false;
    }
}

void LiteWindow::on_checkBoxAddToContextMenu_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    QSettings settings1("HKEY_CLASSES_ROOT\\exefile\\shell\\Detect it easy lite",QSettings::NativeFormat);
    QSettings settings2("HKEY_CLASSES_ROOT\\dllfile\\shell\\Detect it easy lite",QSettings::NativeFormat);
    QSettings settings3("HKEY_CLASSES_ROOT\\sysfile\\shell\\Detect it easy lite",QSettings::NativeFormat);

    QSettings settings4("HKEY_CLASSES_ROOT\\exefile\\shell\\Detect it easy lite\\command",QSettings::NativeFormat);
    QSettings settings5("HKEY_CLASSES_ROOT\\dllfile\\shell\\Detect it easy lite\\command",QSettings::NativeFormat);
    QSettings settings6("HKEY_CLASSES_ROOT\\sysfile\\shell\\Detect it easy lite\\command",QSettings::NativeFormat);

    bool bCheck=false;

    if(ui->checkBoxAddToContextMenu->checkState())
    {
        bCheck=true;
        settings4.setValue(".","\""+QCoreApplication::applicationFilePath().replace("/","\\")+"\" \"%1\"");
        settings5.setValue(".","\""+QCoreApplication::applicationFilePath().replace("/","\\")+"\" \"%1\"");
        settings6.setValue(".","\""+QCoreApplication::applicationFilePath().replace("/","\\")+"\" \"%1\"");
    }
    else
    {
        settings1.clear();
        settings2.clear();
        settings3.clear();
    }

    bool bShell=getShellState();

    if(bCheck!=bShell)
    {
        if(bCheck)
        {
            QMessageBox::critical(this,"Register error","Cannot register extensions.\nNeed administrator privileges.");
        }
        else
        {
            QMessageBox::critical(this,"Register error","Cannot unregister extensions.\nNeed administrator privileges.");
        }

        ui->checkBoxAddToContextMenu->setChecked(!bCheck);

        ui->checkBoxAddToContextMenu->setEnabled(false);

    }


}

bool LiteWindow::getShellState()
{
    bool bResult=true;

    QSettings settings1("HKEY_CLASSES_ROOT\\exefile\\shell",QSettings::NativeFormat);
    QSettings settings2("HKEY_CLASSES_ROOT\\dllfile\\shell",QSettings::NativeFormat);
    QSettings settings3("HKEY_CLASSES_ROOT\\sysfile\\shell",QSettings::NativeFormat);


    if(settings1.value("Detect it easy lite/command/Default").toString()=="")
    {
        bResult=false;
    }

    if(settings2.value("Detect it easy lite/command/Default").toString()=="")
    {
        bResult=false;
    }

    if(settings3.value("Detect it easy lite/command/Default").toString()=="")
    {
        bResult=false;
    }

    return bResult;
}
void LiteWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void LiteWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void LiteWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void LiteWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData=event->mimeData();

    if(mimeData->hasUrls())
    {
        QStringList pathList;
        QList<QUrl> urlList=mimeData->urls();

        for(int i=0; (i<urlList.size())&&(i<32); ++i)
        {
            pathList.append(urlList.at(i).toLocalFile());
        }


        openFile(pathList.at(0));

    }
}
