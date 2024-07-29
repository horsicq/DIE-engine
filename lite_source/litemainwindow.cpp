/* Copyright (c) 2020-2024 hors<horsicq@gmail.com>
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
#include "litemainwindow.h"

#include "ui_litemainwindow.h"

LiteMainWindow::LiteMainWindow(QWidget *pParent) : QMainWindow(pParent), ui(new Ui::LiteMainWindow)
{
    ui->setupUi(this);

    g_bInit = false;

    XOptions::registerCodecs();

    setWindowTitle(XOptions::getTitle(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));

    g_xOptions.setName(X_OPTIONSFILELITE);

    g_xOptions.addID(XOptions::ID_SCAN_FLAG_RECURSIVE, true);
    g_xOptions.addID(XOptions::ID_SCAN_FLAG_DEEP, true);
    g_xOptions.addID(XOptions::ID_SCAN_FLAG_HEURISTIC, false);
    g_xOptions.addID(XOptions::ID_SCAN_FLAG_VERBOSE, true);
    g_xOptions.addID(XOptions::ID_SCAN_FLAG_ALLTYPES, false);
    g_xOptions.addID(XOptions::ID_SCAN_DATABASEPATH, "$data/db");
    g_xOptions.addID(XOptions::ID_SCAN_EXTRADATABASEPATH, "$data/db_extra");
    g_xOptions.addID(XOptions::ID_SCAN_CUSTOMDATABASEPATH, "$data/db_custom");

    g_xOptions.load();

    ui->comboBoxFlags->setData(XScanEngine::getScanFlags(), XComboBoxEx::CBTYPE_FLAGS, 0, tr("Flags"));

    quint64 nFlags = XScanEngine::getScanFlagsFromGlobalOptions(&g_xOptions);

    ui->comboBoxFlags->setValue(nFlags);

    setAcceptDrops(true);
    installEventFilter(this);

    XOptions::setMonoFont(ui->plainTextEditResult);

    g_pDieScript = new DiE_Script;

    if (QCoreApplication::arguments().count() > 1) {
        QString sFileName = QCoreApplication::arguments().at(1);

        processFile(sFileName);
    }
}

LiteMainWindow::~LiteMainWindow()
{ 
    quint64 nFlags=ui->comboBoxFlags->getValue().toULongLong();

    XScanEngine::setScanFlagsToGlobalOptions(&g_xOptions, nFlags);

    g_xOptions.save();

    delete ui;
    delete g_pDieScript;
}

void LiteMainWindow::processFile(const QString &sFileName)
{
    QString _sFileName = sFileName;

    ui->plainTextEditResult->clear();

    _sFileName = QDir().toNativeSeparators(_sFileName);

    ui->lineEditFileName->setText(_sFileName);

    XFormats::setFileTypeComboBox(XBinary::FT_UNKNOWN, _sFileName, ui->comboBoxType);

    process();
}

void LiteMainWindow::on_pushButtonScan_clicked()
{
    process();
}

void LiteMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void LiteMainWindow::on_pushButtonOpenFile_clicked()
{
    QString sDirectory = "";  // mb TODO

    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file") + QString("..."), sDirectory, tr("All files") + QString(" (*)"));

    if (!sFileName.isEmpty()) {
        processFile(sFileName);
    }
}

void LiteMainWindow::process()
{
    QString _sFileName = ui->lineEditFileName->text().trimmed();

    if (_sFileName != "") {
        XScanEngine::SCAN_OPTIONS scanOptions = {};

        scanOptions.bShowType = true;
        scanOptions.bShowVersion = true;
        scanOptions.bShowInfo = true;
        scanOptions.fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

        quint64 nFlags=ui->comboBoxFlags->getValue().toULongLong();
        XScanEngine::setScanFlags(&scanOptions, nFlags);

        if (!g_bInit) {
            g_pDieScript->initDatabase();
            g_pDieScript->loadDatabase(&scanOptions, g_xOptions.getDatabasePath(), "main");
            g_pDieScript->loadDatabase(&scanOptions, g_xOptions.getExtraDatabasePath(), "extra");
            g_pDieScript->loadDatabase(&scanOptions, g_xOptions.getCustomDatabasePath(), "custom");

            g_bInit = true;
        }

        XScanEngine::SCAN_RESULT scanResult = g_pDieScript->scanFile(_sFileName, &scanOptions);

        ScanItemModel model(&scanOptions, &(scanResult.listRecords), 1);

        ui->plainTextEditResult->setPlainText(model.toFormattedString());

        ui->labelScanTime->setText(QString("%1 %2").arg(scanResult.nScanTime).arg(tr("msec")));
    }
}

void LiteMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void LiteMainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void LiteMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();

        if (urlList.count()) {
            QString sFileName = urlList.at(0).toLocalFile();

            sFileName = XBinary::convertFileName(sFileName);

            processFile(sFileName);
        }
    }
}

void LiteMainWindow::keyPressEvent(QKeyEvent *pEvent)
{
    if (pEvent->key() == Qt::Key_Escape) {
        this->close();
    }

    QWidget::keyPressEvent(pEvent);
}
