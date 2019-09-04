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
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    // bIsDialogsopen=false;

    // Shortcuts
    shortcut1 = new QShortcut(QKeySequence(__KeySequence_tab1), this);
    QObject::connect(shortcut1, SIGNAL(activated()), this, SLOT(tabScan()));
    shortcut2 = new QShortcut(QKeySequence(__KeySequence_tab2), this);
    QObject::connect(shortcut2, SIGNAL(activated()), this, SLOT(tabScripts()));
//    shortcut3 = new QShortcut(QKeySequence(__KeySequence_tab3), this);
//    QObject::connect(shortcut3, SIGNAL(activated()), this, SLOT(tabPlugins()));
    shortcut3 = new QShortcut(QKeySequence(__KeySequence_tab3), this);
    QObject::connect(shortcut3, SIGNAL(activated()), this, SLOT(tabLog()));

    ui->pushButtonAbout->setShortcut(QKeySequence(__KeySequence_altA));

    ui->pushButtonClear->setShortcut(QKeySequence(__KeySequence_ctrlaltG));
    ui->pushButtonClearLog->setShortcut(QKeySequence(__KeySequence_ctrlG));
    ui->pushButtonDebug->setShortcut(QKeySequence(__KeySequence_ctrlD));
    ui->pushButtonDirectory->setShortcut(QKeySequence(__KeySequence_altD));
    ui->pushButtonEntropy->setShortcut(QKeySequence(__KeySequence_ctrlE));
    ui->pushButtonExit->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonExtra->setShortcut(QKeySequence(__KeySequence_ctrlaltE));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_altH));
    ui->pushButtonInfo->setShortcut(QKeySequence(__KeySequence_ctrlI));
    ui->pushButtonOpenFile->setShortcut(QKeySequence(__KeySequence_open));
    ui->pushButtonOptions->setShortcut(QKeySequence(__KeySequence_ctrlaltO));
    ui->pushButtonRun->setShortcut(QKeySequence(__KeySequence_ctrlR));
    ui->pushButtonSave->setShortcut(QKeySequence(__KeySequence_ctrlaltS));
    ui->pushButtonSaveLog->setShortcut(QKeySequence(__KeySequence_ctrlS));
    ui->pushButtonScan->setShortcut(QKeySequence(__KeySequence_altF));
    ui->pushButtonSelect->setShortcut(QKeySequence(__KeySequence_altU));
    ui->pushButtonSignatures->setShortcut(QKeySequence(__KeySequence_ctrlaltU));
    ui->pushButtonSearch->setShortcut(QKeySequence(__KeySequence_ctrlF));
    ui->pushButtonFLC->setShortcut(QKeySequence(__KeySequence_ctrlaltL));
    ui->pushButtonScanMethodInfo->setShortcut(QKeySequence(__KeySequence_ctrlaltI));

    connect(&scriptdie, SIGNAL(appendMessage(QString)),this,SLOT(appendScriptMessage(QString)));
    connect(&scriptdie, SIGNAL(appendWarningMessage(QString)),this,SLOT(appendScriptWarningMessage(QString)));
    connect(&scriptdie, SIGNAL(appendErrorMessage(QString)),this,SLOT(appendScriptErrorMessage(QString)));
    connect(&scriptdie, SIGNAL(appendSuccessMessage(QString)),this,SLOT(appendScriptSuccessMessage(QString)));

    scriptdie.setData(this);
    pluginscript.addClass(&scriptdie,"script",Utils::getApplicationPath());

    connect(&__pefile, SIGNAL(appendError(QString)),this,SLOT(appendScriptErrorMessage(QString)));
    connect(&__binary, SIGNAL(appendError(QString)),this,SLOT(appendScriptErrorMessage(QString)));
    connect(&__elffile, SIGNAL(appendError(QString)),this,SLOT(appendScriptErrorMessage(QString)));
    connect(&__msdosfile, SIGNAL(appendError(QString)),this,SLOT(appendScriptErrorMessage(QString)));

    pluginscript.addClass(&__pefile,"PEFile",Utils::getApplicationPath());
    pluginscript.addClass(&__binary,"BinaryFile",Utils::getApplicationPath());
    pluginscript.addClass(&__elffile,"ELFFile",Utils::getApplicationPath());
    pluginscript.addClass(&__msdosfile,"MSDOSFile",Utils::getApplicationPath());

    //    connect(&scriptsql, SIGNAL(appendError(QString)),this,SLOT(appendPluginErrorMessage(QString)));
    //    pluginscript.addClass(&scriptsql,"sql");

    pThreadPlugin=0;

    connect(&pluginscript, SIGNAL(appendError(QString)),this,SIGNAL(appendError(QString)));

    this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_BINARY));
    this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_BINARY));

    actCopyAsString=new QAction("", this);
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));
    //actShowSource=new QAction("", this);
    //connect(actShowSource,SIGNAL(triggered()),this,SLOT(showSource()));

    staticform=0;

    //    options.bStayOnTop=false;
    //    options.bShowTooltips=false;
    //    options.bShowErrors=false;

    this->setWindowTitle(QString("%1 %2").arg(__DIE).arg(__VERSION));
    //    ui->lineEditPluginName->setText("");

    ui->lineEditFileName->setHex(false);
    ui->lineEditType->setHex(false);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);
    ui->lineEditScanTime->setHex(false);
    ui->lineEditScriptName->setHex(false);

    setProgressBarScan(1,1);
    ui->lineEditScanTime->setText("");

    connect(this, SIGNAL(scanSignal()),this,SLOT(scanSlot()),Qt::QueuedConnection);
    connect(this, SIGNAL(directory(QString)),this,SLOT(_directory(QString)),Qt::QueuedConnection);

    controlsEnable(false);

    timerHeuristic.setInterval(1000);
    connect(&timerHeuristic,SIGNAL(timeout()),this,SLOT(setHeuristicEnabled()));
    timerPlugin.setInterval(1000);
    connect(&timerPlugin,SIGNAL(timeout()),this,SLOT(setPluginEnabled()));

    ui->tableWidgetResult_die->setColumnCount(4);

    resizeScanResult(false);

    ui->tableWidgetResult_die->setColumnWidth(2,20);
    ui->tableWidgetResult_die->setColumnWidth(3,20);

    setAcceptDrops(true);

    options.nDialogOpen=0;
    options.bResizeableMainWindow=false;
    options.bStayOnTop=false;
    options.bScanAfterOpen=false;
    options.bSaveLastDirectory=false;
    options.bShowTooltips=false;
    options.bScanShowVersionDIE=false;
    options.bScanShowOptionsDIE=false;
    options.bShowErrors=false;
    options.bScanSubfolders=false;
    options.bResizeableMainWindow=false;
    options.bFullScan=false;
    options.sm=SM_DIE; // TODO
    options.pMutexResult=new QMutex();

    options.bDieLoaded=false;
#ifdef USE_YARA
    options.bYaraLoaded=false;
#endif
    updateOptions();

    bIsScan=false;
    bIsRun=false;
    //    getLastDirectory();

    connect(this, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));

    QList<SCAN_METHODS> listMethods;

    listMethods.append(SM_DIE);
#ifdef USE_NFD
    listMethods.append(SM_NFD);
#endif
#ifdef USE_YARA
    listMethods.append(SM_YARA);
#endif
#ifdef USE_PEID
    listMethods.append(SM_PEID);
#endif

    bIsLoad=true;
    int nIndex=0;

    for(int i=0; i<listMethods.count(); i++)
    {
        QString sText;
        SCAN_METHODS smCurrent=listMethods.at(i);

        if(smCurrent==SM_DIE)
        {
            sText="Detect It Easy";
        }
#ifdef USE_NFD
        else if(smCurrent==SM_NFD)
        {
            sText="Nauz File Detector";
        }
#endif
#ifdef USE_YARA
        else if(smCurrent==SM_YARA)
        {
            sText="YARA";
        }
#endif
#ifdef USE_PEID
        else if(smCurrent==SM_PEID)
        {
            sText="PEiD";
        }
#endif
        ui->comboBoxScanMethod->addItem(sText,smCurrent);

        if(options.sm==smCurrent)
        {
            nIndex=i;
        }
    }

    ui->comboBoxScanMethod->setCurrentIndex(nIndex);
    bIsLoad=false;

    changeMethod(options.sm);

    //

    if(QCoreApplication::arguments().count()>1)
    {
        openFile(QCoreApplication::arguments().at(1));
    }
}

MainWindow::~MainWindow()
{
    emit reset();
    //    scan->_stop();

    //    QSettings settings(Scan::getApplicationPath()+QDir::separator()+"die.ini",QSettings::IniFormat);

    //    settings.setValue("General/LastDirectory",options.sLastDirectory);
    //    qDebug(options.sLastDirectory.toAscii().data());

    HandleOptions::save(&options);

    if(pThreadPlugin)
    {
        pThreadPlugin->quit();
    }

    //    while(!scan->isCompleted())
    //    {

    //    }
    //    delete scan;
    delete ui;
}

//void MainWindow::loadPlugins()
//{
//    ui->listWidgetPlugins->clear();

//    QDir dir(Utils::getPluginsPath(&options)+QDir::separator());
//    QFileInfoList dircontent=dir.entryInfoList(QStringList()<<"*.*");

//    QPluginLoader pluginloader;
//    QListWidgetItem *pListWidgetItem;

//    for(int i=0; i<dircontent.count(); i++)
//    {
//        pluginloader.setFileName(dircontent.at(i).absoluteFilePath());

//        if(!dircontent.at(i).isDir())
//        {
//            if(pluginloader.load())
//            {
//                QObject *plugin=pluginloader.instance();

//                if(plugin)
//                {
//                    PluginInterface *plugininterface=qobject_cast<PluginInterface *>(plugin);

//                    if(plugininterface)
//                    {
//                        pListWidgetItem=new QListWidgetItem;

//                        pListWidgetItem->setText(plugininterface->getName());
//                        pListWidgetItem->setData(Qt::UserRole,dircontent.at(i).absoluteFilePath());

//                        ui->listWidgetPlugins->addItem(pListWidgetItem);
//                    }
//                }

//                pluginloader.unload();
//            }
//            else
//            {
//                _error(pluginloader.errorString());
//            }
//        }
//    }
//}

void MainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void MainWindow::on_pushButtonOptions_clicked()
{
    DialogOptions dialogoptions(this);

    dialogoptions.setOptions(&options);

    dialogoptions.exec();

    updateOptions();
}

void MainWindow::on_pushButtonAbout_clicked()
{
    DialogAbout dialogabout(this);

    dialogabout.setData(&options);

    dialogabout.exec();
}

void MainWindow::on_pushButtonHex_clicked()
{
    PEFile pefile;
    connect(&pefile, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));

    if(pefile.setFileName(ui->lineEditFileName->text()))
    {
        DialogHDE dialoghde(this);

        dialoghde.setData(&options,pefile.getFileName(),0,0,"HEX","","",0,0,"");

        dialoghde.exec();
    }
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    QString sDir;

    if(options.bSaveLastDirectory)
    {
        sDir=options.sLastDirectory;
    }

    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file..."),sDir, tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(options.bSaveLastDirectory)
        {
            options.sLastDirectory=Utils::getPath(sFileName);

            //            qDebug(options.sLastDirectory.toAscii().data());
            //            qDebug(options.sLastDirectory.toAscii().data());
        }

        ui->lineEditFileName->setText("");
        ui->lineEditType->setText("");
        ui->lineEditSize->setText("");
        //sExtra.clear();

        openFile(sFileName);
    }
}

void MainWindow::openFile(QString sFileName)
{
    if(options.nDialogOpen)
    {
        QMessageBox::critical(this,tr("Error"),tr("Cannot open: %1\nPlease, close all DIE's dialogs").arg(sFileName));

        return;
    }

    clearResult();

    QString sTemp;

    if(!sFileName.isEmpty())
    {
#ifdef Q_OS_MAC

        if(sFileName.startsWith("/.file/id="))
        {
            CFStringRef relCFStringRef =
                CFStringCreateWithCString(
                    kCFAllocatorDefault,
                    sFileName.toUtf8().constData(),
                    kCFStringEncodingUTF8
                );
            CFURLRef relCFURL =
                CFURLCreateWithFileSystemPath(
                    kCFAllocatorDefault,
                    relCFStringRef,
                    kCFURLPOSIXPathStyle,
                    false  // isDirectory
                );
            CFErrorRef error=0;
            CFURLRef absCFURL =
                CFURLCreateFilePathURL(
                    kCFAllocatorDefault,
                    relCFURL,
                    &error
                );

            if(!error)
            {
                static const CFIndex maxAbsPathCStrBufLen=4096;
                char absPathCStr[maxAbsPathCStrBufLen];

                if(CFURLGetFileSystemRepresentation(
                            absCFURL,
                            true,  // resolveAgainstBase
                            reinterpret_cast<UInt8 *>(&absPathCStr[0]),
                            maxAbsPathCStrBufLen
                        ))
                {
                    sFileName=QString(absPathCStr);
                }
            }

            CFRelease(absCFURL);
            CFRelease(relCFURL);
            CFRelease(relCFStringRef);
        }

#endif
        QFileInfo fi;

        while(true)
        {
            fi.setFile(sFileName);

            if(!fi.exists())
            {
                sTemp=sFileName;  //mb TODO windows def
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
            emit directory(sFileName);
        }
        else
        {
            ui->lineEditFileName->setText(sFileName);

            //            qDebug("test");
            if(options.bSaveLastDirectory)
            {
                options.sLastDirectory=Utils::getPath(sFileName);

                //                qDebug(options.sLastDirectory.toAscii().data());
            }

            reload();
        }
    }
}

void MainWindow::clearResult()
{
    ui->tableWidgetResult_die->clear();
    ui->tableWidgetResult_die->setRowCount(0);
    ui->lineEditScanTime->setText("");
#ifdef USE_NFD
    ui->treeViewResult_nfd->setModel(0);
#endif
}

void MainWindow::controlsEnable(bool bEnable)
{
    ui->pushButtonHex->setEnabled(bEnable);
    ui->pushButtonEntropy->setEnabled(bEnable);
    ui->pushButtonSearch->setEnabled(bEnable);
    ui->pushButtonFLC->setEnabled(bEnable);

    ui->pushButtonScan->setEnabled(bEnable);
    ui->progressBarScan->setEnabled(bEnable);

    ui->pushButtonExtra->setEnabled(bEnable);
    ui->lineEditScanTime->setEnabled(bEnable);

    ui->tableWidgetResult_die->setEnabled(bEnable);
    ui->treeViewResult_nfd->setEnabled(bEnable);

    ui->lineEditSize->setEnabled(bEnable);
    ui->lineEditType->setEnabled(bEnable);
    ui->labelSize->setEnabled(bEnable);
    ui->labelType->setEnabled(bEnable);
    ui->pushButtonSignatures->setEnabled(bEnable);
    //    ui->pushButtonScanMethodInfo->setEnabled(bEnable);
    //    ui->comboBoxScanMethod->setEnabled(bEnable);
}

void MainWindow::on_lineEditType_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

    controlsEnable(true);
}

bool MainWindow::reload()
{
    Binary binary;
    connect(&binary, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));

    if(binary.setFileName(ui->lineEditFileName->text()))
    {
        QString sType=binary.getType();
        ui->lineEditType->setText(sType);
        ui->lineEditSize->setText(QString("%1").arg(binary.size()));

        if(staticform)
        {
            delete staticform;
            staticform=0;
        }

        if((sType=="PE")||(sType=="PE+(64)"))
        {
            //            this->setMinimumSize(QSize(580, 330));
            //            this->setMaximumSize(QSize(580, 330));

            sCurrentType="PE";


            staticform=new FormPE(this);

            ui->verticalLayoutPanel->addWidget(staticform);
            ((FormPE *)staticform)->setData(ui->lineEditFileName->text(),&options);
            ((FormPE *)staticform)->showTooltips(options.bShowTooltips);
        }
        else if((sType=="ELF")||(sType=="ELF64"))
        {
            //            this->setMinimumSize(QSize(580, 290));
            //            this->setMaximumSize(QSize(580, 290));

            sCurrentType="ELF";

            staticform=new FormELF(this);

            ui->verticalLayoutPanel->addWidget(staticform);
            ((FormELF *)staticform)->setData(ui->lineEditFileName->text(),&options);
            ((FormELF *)staticform)->showTooltips(options.bShowTooltips);
        }
        else if((sType=="MACH")||(sType=="MACH64"))
        {
            //            this->setMinimumSize(QSize(580, 290));
            //            this->setMaximumSize(QSize(580, 290));

            sCurrentType="MACH";


            staticform=new FormMACH(this);

            ui->verticalLayoutPanel->addWidget(staticform);
            ((FormMACH *)staticform)->setData(ui->lineEditFileName->text(),&options);
            ((FormMACH *)staticform)->showTooltips(options.bShowTooltips);
        }
        else if(sType=="MSDOS")
        {
            sCurrentType="MSDOS";

            staticform=new FormMSDOS(this);

            ui->verticalLayoutPanel->addWidget(staticform);
            ((FormMSDOS *)staticform)->setData(ui->lineEditFileName->text(),&options);
            ((FormMSDOS *)staticform)->showTooltips(options.bShowTooltips);
        }
        else
        {
            sCurrentType="";

            //            this->setMinimumSize(QSize(580, 230));
            //            this->setMaximumSize(QSize(580, 230));
        }

        clearResult();

        if(options.bScanAfterOpen)
        {
            emit scanSignal();
        }

        setCurrentSize(options.bResizeableMainWindow);
    }

    return true;
}

void MainWindow::_reload()
{
    Binary binary;
    connect(&binary, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));

    if(binary.setFileName(ui->lineEditFileName->text()))
    {
        QString sType=binary.getType();
        ui->lineEditType->setText(sType);
        ui->lineEditSize->setText(QString("%1").arg(binary.size()));
    }
}

void MainWindow::setProgressBarScan(int nMax, int nValue)
{
    ui->progressBarScan->setMaximum(nMax);
    ui->progressBarScan->setValue(nValue);

    //    if(nMax==nValue)
    //    {
    //        ui->pushButtonScan->setText(tr("Scan"));
    //        bIsScan=false;

    //        ui->lineEditScanTime->setText(tr("%1 ms").arg(scanTime.msecsTo(QTime::currentTime())));
    //        ui->pushButtonDirectory->setEnabled(true);

    //        //        ui->pushButtonScan->setEnabled(true);

    ////        if(ui->tableWidgetResult->rowCount())
    ////        {
    ////            ui->pushButtonExtra->setEnabled(true);
    ////        }

    //        //pefile.close();
    //    }
}

void MainWindow::scanCompleteSlot(int nMs)
{
    if(options.sm==SM_DIE)
    {
        QList<__DIE_RESULT> listResult=options.die_listResult;

        ui->pushButtonExtra->setEnabled(listResult.count());

        for(int i=0; i<listResult.count(); i++)
        {
            __DIE_RESULT record=listResult.at(i);

            QString sSignaturePath=record.sSignature;
            //sText=sText.section(";",1,-1);
            //sExtra.append(sText);
            //sExtra.append("\r\n");
            //sText=sText.section(": ",1,-1);

            if((record.sType!="Nothing found")&&(record.sType!=""))
            {
                int nRowCount=ui->tableWidgetResult_die->rowCount();

                ui->tableWidgetResult_die->setRowCount(nRowCount+1);
                ui->tableWidgetResult_die->setRowHeight(nRowCount,20);

                QTableWidgetItem *item1=new QTableWidgetItem;
                QTableWidgetItem *item2=new QTableWidgetItem;
                QTableWidgetItem *item3=new QTableWidgetItem;
                QTableWidgetItem *item4=new QTableWidgetItem;

                QLineEditX *lineEdit2=new QLineEditX;
                lineEdit2->setHex(false);
                lineEdit2->setAlignment(Qt::AlignCenter);
                lineEdit2->setReadOnly(true);

                item1->setText(record.sType);

                item2->setText(record.sName);
                lineEdit2->setText(record.sName);
                item3->setText("S");
                item3->setData(Qt::UserRole,sSignaturePath);
                item4->setText("?");
                item1->setTextAlignment(Qt::AlignCenter);
                item2->setTextAlignment(Qt::AlignCenter);
                item3->setTextAlignment(Qt::AlignCenter);
                item4->setTextAlignment(Qt::AlignCenter);
                ui->tableWidgetResult_die->setItem(nRowCount,0,item1);
                ui->tableWidgetResult_die->setItem(nRowCount,1,item2);
                ui->tableWidgetResult_die->setIndexWidget(ui->tableWidgetResult_die->model()->index(nRowCount,1),lineEdit2);
                ui->tableWidgetResult_die->setItem(nRowCount,2,item3);
                ui->tableWidgetResult_die->setItem(nRowCount,3,item4);
            }
        }

        if(listResult.count())
        {
            resizeScanResult(options.bScanResizeToContentsDIE);
        }
        else
        {
            resizeScanResult(false);
        }
    }

#ifdef USE_NFD
    else if(options.sm==SM_NFD)
    {
        QList<SpecAbstract::SCAN_STRUCT> listResult=options.nfd_result.listRecords;

        ui->pushButtonExtra->setEnabled(listResult.count());

        StaticScanItemModel *model=new StaticScanItemModel(&listResult);
        ui->treeViewResult_nfd->setModel(model);
        ui->treeViewResult_nfd->expandAll();
    }

#endif
#ifdef USE_YARA
    else if(options.sm==SM_YARA)
    {
        ui->plainTextEditYARA->clear();
        ui->plainTextEditYARA->appendPlainText(QYara::toText(&options.yara_result));

        ui->pushButtonExtra->setEnabled(options.yara_result.listRecords.count());
    }

#endif

    ui->pushButtonScan->setText(tr("Scan"));
    bIsScan=false;

    ui->lineEditScanTime->setText(tr("%1 ms").arg(nMs));
    ui->pushButtonDirectory->setEnabled(true);

    if(options.pMutexResult)
    {
        options.pMutexResult->unlock();
    }
}

void MainWindow::setTop(bool bState)
{
    //    if((!bState)&&(bState==options.bStayOnTop))
    //    {
    //        return;
    //    }

    if(bState)
    {
        if(!(this->windowFlags()&(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint)))
        {
            this->setWindowFlags(this->windowFlags()| Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        }
    }
    else
    {
        if(this->windowFlags()&(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint))
        {
            this->setWindowFlags(this->windowFlags() ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        }
    }

    this->show();
}

void MainWindow::resizeScanResult(bool bContent)
{
    if(bContent)
    {
        ui->tableWidgetResult_die->resizeColumnToContents(0);
        ui->tableWidgetResult_die->resizeColumnToContents(1);
    }
    else
    {
        ui->tableWidgetResult_die->setColumnWidth(0,60);
        ui->tableWidgetResult_die->setColumnWidth(1,340);
    }
}

void MainWindow::on_pushButtonScan_clicked()
{
    scanSlot();
}

void MainWindow::scanSlot()
{
    if(ui->lineEditFileName->text()=="")
    {
        return;
    }

    //sExtra.clear();

    if(bIsScan)
    {
        timerHeuristic.stop();
        ui->pushButtonScan->setEnabled(false);
        //timerHeuristic.start();
        scan->_stop();

        ui->pushButtonScan->setText(tr("Scan"));
        bIsScan=false;
    }
    else
    {
        //pefile.reopen();
        scan=new Scan;

        QThread* thread =new QThread;
        scan->moveToThread(thread);

        connect(this, SIGNAL(reset()), scan, SLOT(_stop()));

        connect(scan, SIGNAL(setProgressBar(int,int)),this,SLOT(setProgressBarScan(int,int)));
        //connect(scan, SIGNAL(die_appendSignatureSignal(QString)),this,SLOT(appendSignature(QString))); // TODO obsolete
        connect(scan, SIGNAL(singleScanComplete(int)),this,SLOT(scanCompleteSlot(int)));
        connect(scan, SIGNAL(appendError(QString)),this,SLOT(_error(QString)));

        connect(thread, SIGNAL(started()), scan, SLOT(process()));
        connect(scan, SIGNAL(_finished()), thread, SLOT(quit()));
        connect(scan, SIGNAL(_finished()), scan, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        scan->setData(ui->lineEditFileName->text(),&options);
        scan->setShowSource(true);

        clearResult();
        thread->start();

        ui->pushButtonScan->setText(tr("Stop"));
        ui->pushButtonExtra->setEnabled(false);

        ui->pushButtonDirectory->setEnabled(false);
        ui->pushButtonScan->setEnabled(false);
        timerHeuristic.start();

        bIsScan=true;
    }
}

void MainWindow::setHeuristicEnabled()
{
    timerHeuristic.stop();
    ui->pushButtonScan->setEnabled(true);
}

void MainWindow::setPluginEnabled()
{
    timerPlugin.stop();
    ui->pushButtonRun->setEnabled(true);
}

void MainWindow::appendScriptMessage(QString sString)
{
    ui->textEditScript->append(sString);
}

void MainWindow::appendScriptWarningMessage(QString sString)
{
    if(ui->checkBoxShowWarnings->isChecked())
    {
        ui->textEditScript->append(QString("<font color=\"yellow\">%1</font>").arg(sString));
    }
}

void MainWindow::appendScriptErrorMessage(QString sString)
{
    if(ui->checkBoxShowErrors->isChecked())
    {
        ui->textEditScript->append(QString("<font color=\"red\">%1</font>").arg(sString));
    }
}

void MainWindow::appendScriptSuccessMessage(QString sString)
{
    if(ui->checkBoxShowSuccesses->isChecked())
    {
        ui->textEditScript->append(QString("<font color=\"green\">%1</font>").arg(sString));
    }
}

void MainWindow::on_pushButtonDirectory_clicked()
{
    // bIsDialogsopen=true;
    _directory("");
    // bIsDialogsopen=false;
}

void MainWindow::on_pushButtonExtra_clicked()
{
    // bIsDialogsopen=true;

    QString sExtra;

    if(options.sm==SM_DIE)
    {
        QList<__DIE_RESULT> listResult=options.die_listResult;

        for(int i=0; i<listResult.count(); i++)
        {
            __DIE_RESULT record=listResult.at(i);

            if(record.sFileType!="")
            {
                sExtra+=QString("%1: %2: %3\r\n").arg(record.sFileType).arg(record.sType).arg(record.sName);
            }
        }
    }
#ifdef USE_NFD
    else if(options.sm==SM_NFD)
    {
        QList<SpecAbstract::SCAN_STRUCT> listResult=options.nfd_result.listRecords;

        StaticScanItemModel model(&listResult);
        sExtra=model.toFormattedString();
    }

#endif
#ifdef USE_YARA
    else if(options.sm==SM_YARA)
    {
        sExtra=QYara::toText(&options.yara_result);
    }
#endif
    DialogExtra dialogextra(this);

    dialogextra.setData(&options,sExtra);

    dialogextra.exec();
    // bIsDialogsopen=false;
}

void MainWindow::_pushButtonInfo()
{
    // bIsDialogsopen=true;

    DialogInfo dialoginfo(this);

    dialoginfo.exec();

    // bIsDialogsopen=false;
}

void MainWindow::_error(QString sString)
{
    if(options.bShowErrors)
    {
        appendLog(QString("<font color=\"red\">[%1]%2</font>").arg(tr("Error")).arg(sString));
    }
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

    if(mimeData->hasUrls())
    {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();

        for(int i = 0; i < urlList.size() && i < 32; ++i)
        {
            pathList.append(urlList.at(i).toLocalFile());

            //qDebug(urlList.at(i).toLocalFile().toLatin1().data());
        }

        ui->tabWidgetMain->setCurrentIndex(0);

        openFile(pathList.at(0));
    }
}

void MainWindow::on_tableWidgetResult_die_clicked(const QModelIndex &index)
{
    //qDebug("__Column: %d",index.column());
    //if(ui->tableWidgetResult_die->item(index.row(),index.column())->text()=="?")
    if(index.column()==2)
    {
        QString sSignaturePath=ui->tableWidgetResult_die->item(index.row(),index.column())->data(Qt::UserRole).toString();
        showSource(sSignaturePath);
    }
    else if(index.column()==3)
    {
        //qDebug("Column: %d",index.column());

        DialogInfo dialoginfo(this);

        // bIsDialogsopen=true;

        QString sText=ui->tableWidgetResult_die->item(index.row(),index.column()-2)->text().trimmed();

        dialoginfo.setData(&options,sText);

        dialoginfo.exec();

        // bIsDialogsopen=false;
    }
    //    else if(index.column()==0)
    //    {
    //        QString sSignaturePath=ui->tableWidgetResult->item(index.row(),index.column())->data(Qt::UserRole).toString();


    //        qDebug(sSignaturePath.toAscii().data());
    //    }
}

void MainWindow::updateOptions()
{
    //    bool bTemp;
    //    double fTemp;
    //    unsigned int nTemp;
    HandleOptions::load(&options);

    setTop(options.bStayOnTop);
    showTooltips(options.bShowTooltips);
    setCurrentSize(options.bResizeableMainWindow);

    if(sCurrentType=="PE")
    {
        ((FormPE *)staticform)->showTooltips(options.bShowTooltips);
    }
    else if(sCurrentType=="ELF")
    {
        ((FormELF *)staticform)->showTooltips(options.bShowTooltips);
    }
    else if(sCurrentType=="MSDOS")
    {
        ((FormMSDOS *)staticform)->showTooltips(options.bShowTooltips);
    }
    else if(sCurrentType=="MACH")
    {
        ((FormMACH *)staticform)->showTooltips(options.bShowTooltips);
    }
}

//void MainWindow::getLastDirectory()
//{
//    QSettings settings(Scan::getApplicationPath()+QDir::separator()+"die.ini",QSettings::IniFormat);

//    options.sLastDirectory=settings.value("General/LastDirectory","").toString();
//}

void MainWindow::on_pushButtonClearLog_clicked()
{
    ui->textEditLog->clear();
}

void MainWindow::on_pushButtonSaveLog_clicked()
{
    // bIsDialogsopen=true;
    QFile file;
    QString sFileName;
    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),options.sLastDirectory+QDir::separator()+QString("%1.txt").arg(tr("Log")), tr("Text files (*.txt);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(options.bSaveLastDirectory)
        {
            options.sLastDirectory=Utils::getPath(sFileName);
        }

        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            file.resize(0);
            file.write(ui->textEditLog->toPlainText().replace("\n","\r\n").toLatin1());
            file.close();
        }
    }

    // bIsDialogsopen=false;
}

void MainWindow::appendLog(QString sString)
{
    ui->textEditLog->append(QString("[%1]%2").arg(QTime::currentTime().toString()).arg(sString));
}

void MainWindow::on_pushButtonEntropy_clicked()
{
    Binary _binary;
    connect(&_binary, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));

    if(_binary.setFileName(ui->lineEditFileName->text()))
    {
        DialogEntropy dialogentropy(this);

        dialogentropy.setData(&options,&_binary,0,0);

        dialogentropy.exec();
    }
}

void MainWindow::_directory(QString sDirectoryName)
{
    DialogDirectoryHeuristicScan ddhs(this);

    ddhs.setData(&options);

    if(sDirectoryName!="")
    {
        ddhs.setDirectory(sDirectoryName);
    }

    ddhs.exec();
}

void MainWindow::showTooltips(bool bShow)
{
    if(bShow)
    {
        ui->pushButtonAbout->setToolTip(tr("Information about program"));
        ui->pushButtonDirectory->setToolTip(tr("Directory scan"));
        ui->pushButtonExit->setToolTip(tr("Exit from program"));
        ui->pushButtonExtra->setToolTip(tr("Extra information"));
        ui->pushButtonHex->setToolTip(tr("HEX/Disasm"));

        ui->pushButtonOpenFile->setToolTip(tr("Open file for scan"));
        ui->pushButtonOptions->setToolTip(tr("Program's options"));
        ui->pushButtonScan->setToolTip(tr("File scan"));
        ui->lineEditScanTime->setToolTip(tr("Scan time"));
        ui->pushButtonEntropy->setToolTip(tr("File entropy"));
        ui->pushButtonSearch->setToolTip(tr("Search in file"));
        ui->pushButtonFLC->setToolTip(tr("File location calculator"));
        ui->progressBarScan->setToolTip(tr("Scan progress"));
        ui->tableWidgetResult_die->setToolTip(tr("Scan result"));
        ui->textEditLog->setToolTip(tr("Log window"));
        ui->pushButtonClearLog->setToolTip(tr("Clear log"));
        ui->pushButtonSaveLog->setToolTip(tr("Save log"));

        ui->lineEditFileName->setToolTip(tr("File name"));
        ui->lineEditSize->setToolTip(tr("File size in bytes"));
        ui->lineEditType->setToolTip(tr("File type"));

        ui->lineEditScriptName->setToolTip(tr("Name of selected script"));
        ui->pushButtonClear->setToolTip(tr("Clear plugin's log"));
        ui->pushButtonSave->setToolTip(tr("Save plugin's log"));

        ui->pushButtonSelect->setToolTip(tr("Select plugin"));
        ui->pushButtonRun->setToolTip(tr("Run plugin"));
        ui->pushButtonInfo->setToolTip(tr("Plugin's info"));
        ui->pushButtonDebug->setToolTip(tr("Run in debugger"));

        ui->textEditScript->setToolTip(tr("Scripts's log"));

        ui->tabWidgetMain->setTabToolTip(0,QString("%1(Alt+1)").arg(tr("Scan")));
        ui->tabWidgetMain->setTabToolTip(1,QString("%1(Alt+2)").arg(tr("Scripts")));
        ui->tabWidgetMain->setTabToolTip(2,QString("%1(Alt+3)").arg(tr("Plugins")));
        ui->tabWidgetMain->setTabToolTip(3,QString("%1(Alt+4)").arg(tr("Log")));
        ui->pushButtonSignatures->setToolTip(tr("Signatures"));
        ui->pushButtonScanMethodInfo->setToolTip(tr("Information"));
        ui->comboBoxScanMethod->setToolTip(tr("Method"));
    }
    else
    {
        ui->pushButtonAbout->setToolTip("");
        ui->pushButtonDirectory->setToolTip("");
        ui->pushButtonExit->setToolTip("");
        ui->pushButtonExtra->setToolTip("");
        ui->pushButtonHex->setToolTip("");

        ui->pushButtonOpenFile->setToolTip("");
        ui->pushButtonOptions->setToolTip("");
        ui->pushButtonScan->setToolTip("");
        ui->lineEditScanTime->setToolTip("");

        ui->pushButtonEntropy->setToolTip("");
        ui->pushButtonSearch->setToolTip("");
        ui->pushButtonFLC->setToolTip("");
        ui->progressBarScan->setToolTip("");
        ui->tableWidgetResult_die->setToolTip("");
        ui->textEditLog->setToolTip("");
        ui->pushButtonClearLog->setToolTip("");
        ui->pushButtonSaveLog->setToolTip("");

        ui->lineEditFileName->setToolTip("");
        ui->lineEditSize->setToolTip("");
        ui->lineEditType->setToolTip("");
        ui->lineEditScriptName->setToolTip("");
        ui->pushButtonClear->setToolTip("");
        ui->pushButtonSave->setToolTip("");

        ui->pushButtonSelect->setToolTip("");
        ui->pushButtonRun->setToolTip("");
        ui->pushButtonInfo->setToolTip("");
        ui->pushButtonDebug->setToolTip("");

        ui->textEditScript->setToolTip("");

        ui->tabWidgetMain->setTabToolTip(0,"");
        ui->tabWidgetMain->setTabToolTip(1,"");
        ui->tabWidgetMain->setTabToolTip(2,"");
        ui->tabWidgetMain->setTabToolTip(3,"");

        ui->pushButtonSignatures->setToolTip("");
        ui->pushButtonScanMethodInfo->setToolTip("");
        ui->comboBoxScanMethod->setToolTip("");
    }
}

/*void MainWindow::on_tableWidgetResult_customContextMenuRequested(const QPoint &pos)
{
    // TODO obsolete
    if(!ui->tableWidgetResult_die->selectedItems().count())
    {
        return;
    }

    if(ui->tableWidgetResult_die->selectedItems().at(0)->column()==0)
    {
        QMenu menu;

        //        menu.addAction(actCopyAsString);
        //        actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetResult->selectedItems().at(0)->text()));
        menu.addAction(actShowSource);
        actShowSource->setText(tr("Show source"));

        menu.exec(ui->tableWidgetResult_die->mapToGlobal(pos));
    }

}*/
void MainWindow::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetResult_die->selectedItems().at(0)->text()));
}

void MainWindow::showSource(QString sSignature)
{
    //QString sSignature=ui->tableWidgetResult_die->selectedItems().at(0)->data(Qt::UserRole).toString();

    DialogSignatures ds(this);

    ds.setData(&options,ui->lineEditFileName->text(),sSignature);

    ds.exec();
}

void MainWindow::on_pushButtonSelect_clicked()
{
    // bIsDialogsopen=true;
    DialogSelectPlugin dsp(this);

    dsp.setData(&options);

    dsp.exec();

    if(options.sPluginName!="")
    {
        pluginscript.callFromFile(Utils::getScriptsPath(&options)+QDir::separator()+options.sPluginName+".sg","load");
        QString sName=pluginscript.callFromFile(Utils::getScriptsPath(&options)+QDir::separator()+options.sPluginName+".sg","name").toString();

        ui->lineEditScriptName->setText(sName);
    }
    // bIsDialogsopen=false;
}

void MainWindow::on_pushButtonRun_clicked()
{
    if(options.sPluginName=="")
    {
        return;
    }

    if(bIsRun)
    {
        pThreadPlugin->quit();
        delete pThreadPlugin;
        pThreadPlugin=0;

        timerPlugin.stop();
        ui->pushButtonRun->setEnabled(true);

        ui->pushButtonRun->setText(tr("Run"));
        bIsRun=false;
    }
    else
    {
        pThreadPlugin= new ThreadPlugin;
        connect(pThreadPlugin,SIGNAL(finished()),this,SLOT(pluginFinished()));
        scriptdie.setData(ui->lineEditFileName->text());

        pThreadPlugin->setData(Utils::getScriptsPath(&options)+QDir::separator()+options.sPluginName+".sg",&pluginscript);
        pThreadPlugin->start(QThread::LowestPriority);

        ui->pushButtonRun->setText(tr("Stop"));

        ui->pushButtonRun->setEnabled(false);
        timerPlugin.start();

        bIsRun=true;
    }

}
void MainWindow::pluginFinished()
{
    timerPlugin.stop();
    ui->pushButtonRun->setEnabled(true);

    ui->pushButtonRun->setText(tr("Run"));
    bIsRun=false;

    delete pThreadPlugin;
    pThreadPlugin=0;
}

void MainWindow::on_pushButtonInfo_clicked()
{
    // bIsDialogsopen=true;
    if(options.sPluginName!="")
    {
        scriptdie.setData(ui->lineEditFileName->text());
        QString sInfo=pluginscript.callFromFile(Utils::getScriptsPath(&options)+QDir::separator()+options.sPluginName+".sg","info").toString();
        QMessageBox::information(this,tr("Plugin information"),sInfo);
    }
    // bIsDialogsopen=false;
}

void MainWindow::on_pushButtonDebug_clicked()
{
    // bIsDialogsopen=true;
    if(options.sPluginName!="")
    {
        QScriptEngineDebugger debugger(this);
        QMainWindow *debugWindow=debugger.standardWindow();
        debugWindow->setWindowModality(Qt::ApplicationModal);
        debugWindow->setWindowTitle(tr("Script debugger"));
        //        debugWindow->resize(600,350);
        debugger.attachTo(&pluginscript);
        debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();

        pluginscript.callFromFile(Utils::getScriptsPath(&options)+QDir::separator()+options.sPluginName+".sg","run");
    }

    // bIsDialogsopen=false;
}

void MainWindow::on_pushButtonClear_clicked()
{
    ui->textEditScript->clear();
}

void MainWindow::on_pushButtonSave_clicked()
{
    // bIsDialogsopen=true;
    QFile file;
    QString sFileName;
    sFileName=QFileDialog::getSaveFileName(this, tr("Save file..."),options.sLastDirectory+QDir::separator()+QString("%1.txt").arg(tr("Script")), tr("Text files (*.txt);;All files (*)"));

    if(!sFileName.isEmpty())
    {
        if(options.bSaveLastDirectory)
        {
            options.sLastDirectory=Utils::getPath(sFileName);
        }

        file.setFileName(sFileName);

        if(file.open(QIODevice::ReadWrite))
        {
            file.resize(0);
            file.write(ui->textEditScript->toPlainText().replace("\n","\r\n").toLatin1());
            file.close();
        }
    }
    // bIsDialogsopen=false;
}

void MainWindow::on_lineEditScriptName_textChanged(const QString &arg1)
{
    ui->pushButtonRun->setEnabled(arg1!="");
    ui->pushButtonInfo->setEnabled(arg1!="");
    ui->pushButtonDebug->setEnabled(arg1!="");
}

void MainWindow::setCurrentSize(bool bState)
{
    if(sCurrentType=="PE")
    {
        this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_PE));

        if(!bState)
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_PE));
        }
        else
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH_MAX, WINDOW_HEIGHT_MAX));
        }
    }
    else if(sCurrentType=="ELF")
    {
        this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_ELF));

        if(!bState)
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_ELF));
        }
        else
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH_MAX, WINDOW_HEIGHT_MAX));
        }
    }
    else if(sCurrentType=="MACH")
    {
        this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_MACH));

        if(!bState)
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_MACH));
        }
        else
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH_MAX, WINDOW_HEIGHT_MAX));
        }
    }
    else if(sCurrentType=="MSDOS")
    {
        this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_MSDOS));

        if(!bState)
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_MSDOS));
        }
        else
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH_MAX, WINDOW_HEIGHT_MAX));
        }
    }
    else
    {
        this->setMinimumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_BINARY));

        if(!bState)
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT_BINARY));
        }
        else
        {
            this->setMaximumSize(QSize(WINDOW_WIDTH_MAX, WINDOW_HEIGHT_MAX));
        }
    }
}

void MainWindow::tabScan()
{
    ui->tabWidgetMain->setCurrentIndex(0);
}

void MainWindow::tabScripts()
{
    ui->tabWidgetMain->setCurrentIndex(1);
}

//void MainWindow::tabPlugins()
//{
//    ui->tabWidgetMain->setCurrentIndex(2);
//}

void MainWindow::tabLog()
{
    ui->tabWidgetMain->setCurrentIndex(2);
}
void MainWindow::on_pushButtonSignatures_clicked()
{
    // bIsDialogsopen=true;
    DialogSignatures ds(this);

    ds.setData(&options,ui->lineEditFileName->text());

    ds.exec();
    // bIsDialogsopen=false;
}

//void MainWindow::on_listWidgetPlugins_itemSelectionChanged()
//{
//    if(ui->listWidgetPlugins->count()==0)
//    {
//        return ;
//    }

//    QObjectList list=ui->widgetPlugin->children();

//    for(int i=0; i<list.count(); i++)
//    {
//        delete list.at(i);
//    }

//    pluginloader.unload();

//    pluginloader.setFileName(ui->listWidgetPlugins->currentItem()->data(Qt::UserRole).toString());

//    if(pluginloader.load())
//    {
//        QObject *plugin = pluginloader.instance();

//        if(plugin)
//        {
//            PluginInterface *plugininterface=qobject_cast<PluginInterface *>(plugin);

//            if(plugininterface)
//            {
//                dieplugin.pWidget=ui->widgetPlugin;
//                dieplugin.sFileName=ui->lineEditFileName->text();
//                dieplugin.sPluginPath=Utils::getPluginsPath(&options);

//                plugininterface->init(&dieplugin);
//            }
//        }
//    }
//}

void MainWindow::on_tabWidgetMain_currentChanged(int index)
{
    Q_UNUSED(index)
//    if(index==2)
//    {
//        // Load plugins
//        if(options.bEnablePlugins)
//        {
//            loadPlugins();
//        }
//    }
}

void MainWindow::on_lineEditFileName_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

//    if(ui->tabWidgetMain->currentIndex()==2)
//    {
//        // Load plugins
//        if(options.bEnablePlugins)
//        {
//            loadPlugins();
//        }
//    }
}

void MainWindow::on_pushButtonSearch_clicked()
{
    Binary _binary;
    connect(&_binary, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));

    if(_binary.setFileName(ui->lineEditFileName->text()))
    {
        DialogSearch dialogsearch(this);

        dialogsearch.setData(&options,&_binary,0,0);

        dialogsearch.exec();
    }
}

void MainWindow::on_pushButtonFLC_clicked()
{
    Binary _binary;
    connect(&_binary, SIGNAL(appendError(QString)), this, SLOT(_error(QString)));

    if(_binary.setFileName(ui->lineEditFileName->text()))
    {
        DialogFLC dialogflc(this);

        dialogflc.setData(&options,&_binary);

        dialogflc.exec();
    }
}

void MainWindow::on_comboBoxScanMethod_currentIndexChanged(int index)
{
    if(!bIsLoad)
    {
        SCAN_METHODS sm=(SCAN_METHODS)ui->comboBoxScanMethod->itemData(index).toInt();
        changeMethod(sm,true);
    }
}

void MainWindow::changeMethod(SCAN_METHODS sm, bool bOpenFile)
{
    options.sm=sm;

    if(sm==SM_DIE)
    {
        if(!options.bDieLoaded)
        {
            Scan::die_loadScripts(&options);
            options.bDieLoaded=true;
        }

        ui->stackedWidgetScan->setCurrentIndex(0);
        ui->pushButtonSignatures->setEnabled(true);
    }
#ifdef USE_NFD
    else if(sm==SM_NFD)
    {
        ui->stackedWidgetScan->setCurrentIndex(1);
        ui->pushButtonSignatures->setEnabled(false);
    }
#endif
#ifdef USE_YARA
    else if(sm==SM_YARA)
    {
        if(!options.bYaraLoaded)
        {
            Scan::yara_loadBase(&options);
            options.bYaraLoaded=true;
        }

        ui->stackedWidgetScan->setCurrentIndex(2);
        ui->pushButtonSignatures->setEnabled(false); // TODO
    }
#endif
#ifdef USE_PEID
    else if(sm==SM_PEID)
    {
        ui->stackedWidgetScan->setCurrentIndex(3);
    }

#endif
    if(options.bScanAfterOpen&&bOpenFile)
    {
        emit scanSignal();
    }
}

void MainWindow::on_pushButtonScanMethodInfo_clicked()
{
    QString sText;

    if(options.sm==SM_DIE)
    {
        sText="die_info";
    }
#ifdef USE_NFD
    else if(options.sm==SM_NFD)
    {
        sText="nfd_info";
    }
#endif
#ifdef USE_YARA
    else if(options.sm==SM_YARA)
    {
        sText="yara_info";
    }
#endif
    DialogInfo dialoginfo(this);

    dialoginfo.setData(&options,sText);

    dialoginfo.exec();
}
