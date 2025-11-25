/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
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
#include "guimainwindow.h"

#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *pParent) : QMainWindow(pParent), ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonAbout, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonOptions, XOptions::ICONTYPE_OPTION);
    XOptions::adjustToolButton(ui->toolButtonDemangle, XOptions::ICONTYPE_DEMANGLE);
    XOptions::adjustToolButton(ui->toolButtonExit, XOptions::ICONTYPE_EXIT);
    XOptions::adjustToolButton(ui->toolButtonOpenFile, XOptions::ICONTYPE_OPEN, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonShortcuts, XOptions::ICONTYPE_SHORTCUT);
    XOptions::adjustToolButton(ui->toolButtonRecentFiles, XOptions::ICONTYPE_LIST, Qt::ToolButtonIconOnly);

    ui->toolButtonAbout->setToolTip(tr("About"));
    ui->toolButtonOptions->setToolTip(tr("Options"));
    ui->toolButtonDemangle->setToolTip(tr("Demangle"));
    ui->toolButtonExit->setToolTip(tr("Exit"));
    ui->toolButtonOpenFile->setToolTip(tr("Open file"));
    ui->toolButtonShortcuts->setToolTip(tr("Shortcuts"));
    ui->toolButtonRecentFiles->setToolTip(tr("Recent files"));
    ui->lineEditFileName->setToolTip(tr("File name"));
    ui->checkBoxAdvanced->setToolTip(tr("Advanced"));

#ifdef USE_YARA
    XYara::initialize();
#endif
    XOptions::registerCodecs();

    g_bFullScreen = false;

    setWindowTitle(XOptions::getTitle(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));

    setAcceptDrops(true);

    g_xOptions.setName(X_OPTIONSFILE);

#ifdef Q_OS_WIN
    g_xOptions.addID(XOptions::ID_VIEW_QSS, "orange_fix");
#else
    g_xOptions.addID(XOptions::ID_VIEW_QSS, "");
#endif
    g_xOptions.addID(XOptions::ID_VIEW_ADVANCED, false);
    g_xOptions.addID(XOptions::ID_VIEW_STYLE, "Fusion");
    g_xOptions.addID(XOptions::ID_VIEW_LANG, "System");
    g_xOptions.addID(XOptions::ID_VIEW_FONT_CONTROLS, XOptions::getDefaultFont().toString());
    g_xOptions.addID(XOptions::ID_VIEW_FONT_TABLEVIEWS, XOptions::getMonoFont().toString());
    g_xOptions.addID(XOptions::ID_VIEW_FONT_TREEVIEWS, XOptions::getDefaultFont().toString());
    g_xOptions.addID(XOptions::ID_VIEW_FONT_TEXTEDITS, XOptions::getMonoFont().toString());
    g_xOptions.addID(XOptions::ID_VIEW_STAYONTOP, false);
    g_xOptions.addID(XOptions::ID_VIEW_SINGLEAPPLICATION, false);
    g_xOptions.addID(XOptions::ID_FILE_SAVELASTDIRECTORY, true);
    g_xOptions.addID(XOptions::ID_FILE_SAVEBACKUP, true);
    g_xOptions.addID(XOptions::ID_FILE_SAVERECENTFILES, true);
#ifdef Q_OS_WIN
    g_xOptions.addID(XOptions::ID_FILE_CONTEXT, "*");
#endif

    DIEOptionsWidget::setDefaultValues(&g_xOptions);
    SearchSignaturesOptionsWidget::setDefaultValues(&g_xOptions);
    XHexViewOptionsWidget::setDefaultValues(&g_xOptions);
    XDisasmViewOptionsWidget::setDefaultValues(&g_xOptions);
    XOnlineToolsOptionsWidget::setDefaultValues(&g_xOptions);
    XInfoDBOptionsWidget::setDefaultValues(&g_xOptions);

    g_xOptions.addID(XOptions::ID_SCAN_ENGINE, "auto");
#ifdef USE_YARA
    g_xOptions.addID(XOptions::ID_SCAN_YARARULESPATH, "$data/yara_rules");
#endif
    g_xOptions.load();

    g_xShortcuts.setName(X_SHORTCUTSFILE);
    g_xShortcuts.setNative(g_xOptions.isNative(), g_xOptions.getApplicationDataPath());

    g_xShortcuts.addId(X_ID_FILE_OPEN);
    g_xShortcuts.addId(X_ID_FILE_EXIT);
    g_xShortcuts.addId(X_ID_VIEW_FULLSCREEN);

    g_xShortcuts.addGroup(XShortcuts::GROUPID_HEX);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_DISASM);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_TABLE);
    g_xShortcuts.load();

    ui->widgetFormats->setGlobal(&g_xShortcuts, &g_xOptions);

    connect(&g_xOptions, SIGNAL(openFile(QString)), this, SLOT(_process(QString)));
    connect(&g_xOptions, SIGNAL(errorMessage(QString)), this, SLOT(errorMessageSlot(QString)));

    g_pRecentFilesMenu = g_xOptions.createRecentFilesMenu(this);

    ui->toolButtonRecentFiles->setEnabled(g_xOptions.getRecentFiles().count());

    adjustView();
    memset(shortCuts, 0, sizeof shortCuts);
    updateShortcuts();

    bool bIsAdvanced = g_xOptions.getValue(XOptions::ID_VIEW_ADVANCED).toBool();

    if (!bIsAdvanced) {
        setAdvanced(bIsAdvanced);
    }

    ui->checkBoxAdvanced->setChecked(bIsAdvanced);

    if (QCoreApplication::arguments().count() > 1) {
        QString sFileName = QCoreApplication::arguments().at(1);

        _process(sFileName);
    }
}

GuiMainWindow::~GuiMainWindow()
{
    g_xOptions.setValue(XOptions::ID_VIEW_ADVANCED, ui->checkBoxAdvanced->isChecked());

    g_xOptions.save();
    g_xShortcuts.save();

    delete ui;
#ifdef USE_YARA
    XYara::finalize();
#endif
}

void GuiMainWindow::on_toolButtonExit_clicked()
{
    exitSlot();
}

void GuiMainWindow::on_toolButtonAbout_clicked()
{
    DialogAbout dialogAbout(this);
    dialogAbout.setGlobal(&g_xShortcuts, &g_xOptions);

    dialogAbout.exec();
}

void GuiMainWindow::on_toolButtonOptions_clicked()
{
    DialogOptions dialogOptions(this, &g_xOptions, XOptions::GROUPID_FILE);
    dialogOptions.setGlobal(&g_xShortcuts, &g_xOptions);

    dialogOptions.exec();

    adjustView();
    adjustFile();
}

void GuiMainWindow::on_toolButtonDemangle_clicked()
{
    DialogDemangle dialogDemangle(this);

    dialogDemangle.exec();
}

QString GuiMainWindow::getCurrentFileName()
{
    return ui->lineEditFileName->text();
}

void GuiMainWindow::adjustView()
{
    g_xOptions.adjustStayOnTop(this);
    g_xOptions.adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);

    ui->widgetFormats->adjustView();
}

void GuiMainWindow::updateShortcuts()
{
    for (qint32 i = 0; i < __SC_SIZE; i++) {
        if (shortCuts[i]) {
            delete shortCuts[i];
            shortCuts[i] = nullptr;
        }
    }

    if (!shortCuts[SC_OPENFILE]) shortCuts[SC_OPENFILE] = new QShortcut(g_xShortcuts.getShortcut(X_ID_FILE_OPEN), this, SLOT(openFileSlot()));
    if (!shortCuts[SC_EXIT]) shortCuts[SC_EXIT] = new QShortcut(g_xShortcuts.getShortcut(X_ID_FILE_EXIT), this, SLOT(exitSlot()));
    if (!shortCuts[SC_FULLSCREEN]) shortCuts[SC_FULLSCREEN] = new QShortcut(g_xShortcuts.getShortcut(X_ID_VIEW_FULLSCREEN), this, SLOT(fullScreenSlot()));
}

void GuiMainWindow::adjustFile()
{
    QString sFileName = getCurrentFileName();

    g_xOptions.setLastFileName(sFileName);

    ui->toolButtonRecentFiles->setEnabled(g_xOptions.getRecentFiles().count());
}

void GuiMainWindow::setAdvanced(bool bState)
{
    if (bState) {
        ui->toolButtonDemangle->show();

    } else {
        ui->toolButtonDemangle->hide();
    }

    ui->widgetFormats->setAdvanced(bState);
}

void GuiMainWindow::errorMessageSlot(const QString &sText)
{
    QMessageBox::critical(this, tr("Error"), sText);
}

void GuiMainWindow::_process(const QString &sName)
{
    if (sName != "") {
        ui->lineEditFileName->setText(QDir().toNativeSeparators(sName));

        ui->widgetFormats->setFileName(sName, g_xOptions.isScanAfterOpen());

        adjustFile();
    }
}

void GuiMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();

        if (urlList.count()) {
            QString sFileName = urlList.at(0).toLocalFile();

            sFileName = XBinary::convertFileName(sFileName);

            _process(sFileName);
        }
    }
}

void GuiMainWindow::on_toolButtonOpenFile_clicked()
{
    openFileSlot();
}

void GuiMainWindow::on_toolButtonShortcuts_clicked()
{
    DialogShortcuts dialogShortcuts(this);
    dialogShortcuts.setGlobal(&g_xShortcuts, &g_xOptions);
    dialogShortcuts.setData(&g_xShortcuts);

    dialogShortcuts.exec();

    updateShortcuts();
}

void GuiMainWindow::on_toolButtonRecentFiles_clicked()
{
    g_pRecentFilesMenu->exec(QCursor::pos());

    ui->toolButtonRecentFiles->setEnabled(g_xOptions.getRecentFiles().count());
}

void GuiMainWindow::on_checkBoxAdvanced_toggled(bool bChecked)
{
    setAdvanced(bChecked);
}

void GuiMainWindow::on_lineEditFileName_returnPressed()
{
    QString sFileName = XBinary::convertFileName(ui->lineEditFileName->text());

    _process(sFileName);
}

void GuiMainWindow::exitSlot()
{
    this->close();
}

void GuiMainWindow::openFileSlot()
{
    QString sDirectory = g_xOptions.getLastDirectory();

    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file") + QString("..."), sDirectory, tr("All files") + QString(" (*)"));

    if (!sFileName.isEmpty()) {
        _process(sFileName);
    }
}

void GuiMainWindow::fullScreenSlot()
{
    // TODO mainWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint)
    g_bFullScreen = (!g_bFullScreen);

    if (g_bFullScreen) {
        showFullScreen();
    } else {
        showNormal();
    }
}
