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

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

#include "../global.h"
#include "consoleoutput.h"
#include "die_script.h"
#include "entropyprocess.h"
#include "xfileinfo.h"
#include "xoptions.h"
#include "scanitemmodel.h"

XOptions::CR ScanFiles(QList<QString> *pListArgs, XScanEngine::SCAN_OPTIONS *pScanOptions, DiE_Script *pDieScript)
{
    XOptions::CR result = XOptions::CR_SUCCESS;

    QList<QString> listFileNames;

    for (qint32 i = 0; i < pListArgs->count(); i++) {
        QString sFileName = pListArgs->at(i);

        if (QFileInfo::exists(sFileName)) {
            XBinary::findFiles(sFileName, &listFileNames);
        } else {
            printf("Cannot find: %s\n", sFileName.toUtf8().data());

            result = XOptions::CR_CANNOTFINDFILE;
        }
    }

    bool bShowFileName = listFileNames.count() > 1;

    qint32 nNumberOfFiles = listFileNames.count();

    for (qint32 i = 0; i < nNumberOfFiles; i++) {
        QString sFileName = listFileNames.at(i);

        if (bShowFileName) {
            printf("%s:\n", QDir().toNativeSeparators(sFileName).toUtf8().data());
        }

        if (pScanOptions->bShowEntropy) {
            QString sResult;

            EntropyProcess::DATA epData = EntropyProcess::processRegionsFile(sFileName);

            if (pScanOptions->bResultAsJSON) {
                sResult = EntropyProcess::dataToJsonString(&epData);
            } else if (pScanOptions->bResultAsXML) {
                sResult = EntropyProcess::dataToXmlString(&epData);
            } else if (pScanOptions->bResultAsCSV) {
                sResult = EntropyProcess::dataToCsvString(&epData);
            } else if (pScanOptions->bResultAsTSV) {
                sResult = EntropyProcess::dataToTsvString(&epData);
            } else {
                sResult = EntropyProcess::dataToPlainString(&epData);
            }

            printf("%s", sResult.toUtf8().data());
        } else if ((pScanOptions->bShowFileInfo) || (pScanOptions->sSpecial != "")) {
            QString sResult;

            XFileInfo::OPTIONS options = {};

            if (pScanOptions->sSpecial != "") {
                options.sString = pScanOptions->sSpecial;
            } else {
                options.sString = "Info";
            }

            XFileInfoModel model;

            if (!XFileInfo::processFile(sFileName, &model, options)) {
                result = XOptions::CR_CANNOTOPENFILE;
            }

            if (pScanOptions->bResultAsJSON) {
                sResult = model.toJSON();
            } else if (pScanOptions->bResultAsXML) {
                sResult = model.toXML();
            } else if (pScanOptions->bResultAsCSV) {
                sResult = model.toCSV();
            } else if (pScanOptions->bResultAsTSV) {
                sResult = model.toTSV();
            } else {
                sResult = model.toFormattedString();
            }

            printf("%s", sResult.toUtf8().data());
            printf("\n");
        } else {
            XScanEngine::SCAN_RESULT scanResult = pDieScript->scanFile(sFileName, pScanOptions);

            ScanItemModel model(pScanOptions, &(scanResult.listRecords), 1);

            XBinary::FORMATTYPE formatType = XBinary::FORMATTYPE_TEXT;

            if (pScanOptions->bResultAsCSV) formatType = XBinary::FORMATTYPE_CSV;
            else if (pScanOptions->bResultAsJSON) formatType = XBinary::FORMATTYPE_JSON;
            else if (pScanOptions->bResultAsTSV) formatType = XBinary::FORMATTYPE_TSV;
            else if (pScanOptions->bResultAsXML) formatType = XBinary::FORMATTYPE_XML;
            else if (pScanOptions->bResultAsPlainText) formatType = XBinary::FORMATTYPE_PLAINTEXT;

            if (formatType != XBinary::FORMATTYPE_TEXT) {
                printf("%s\n", model.toString(formatType).toUtf8().data());
            } else {
                // Colored text
                model.coloredOutput();
            }

            //            QList<XBinary::SCANSTRUCT> listResult=DiE_Script::convert(&(scanResult.listRecords));

            //            ScanItemModel model(&listResult);

            //            printf("%s",model.toFormattedString().toUtf8().data());

            if (scanResult.listErrors.count()) {
                printf("%s", DiE_Script::getErrorsString(&scanResult).toUtf8().data());
            }
            printf("\n");
        }
    }

    return result;
}

int main(int argc, char *argv[])
{
    qint32 nResult = XOptions::CR_SUCCESS;

    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    XOptions::registerCodecs();

    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2006-2008 Hellsp@wn 2012-2025 hors<horsicq@gmail.com> Web: http://ntinfo.biz"));
    parser.setApplicationDescription(sDescription);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("target", "The file or directory to open.");

    QCommandLineOption clRecursiveScan(QStringList() << "r"
                                                     << "recursivescan",
                                       "Recursive scan.");
    QCommandLineOption clDeepScan(QStringList() << "d"
                                                << "deepscan",
                                  "Deep scan.");
    QCommandLineOption clHeuristicScan(QStringList() << "u"
                                                     << "heuristicscan",
                                       "Heuristic scan.");
    QCommandLineOption clVerbose(QStringList() << "b"
                                               << "verbose",
                                 "Verbose.");
    QCommandLineOption clAggresiveScan(QStringList() << "g"
                                                     << "aggressivecscan",
                                       "Aggressive scan.");
    QCommandLineOption clAllTypesScan(QStringList() << "a"
                                                    << "alltypes",
                                      "Scan all types.");
    QCommandLineOption clFormatResult(QStringList() << "f"
                                                    << "format",
                                      "Format result.");
    QCommandLineOption clProfiling(QStringList() << "l"
                                                 << "profiling",
                                   "Profiling signatures.");
    QCommandLineOption clHideUnknown(QStringList() << "U"
                                                   << "hideunknown",
                                     "Hide unknown.");
    QCommandLineOption clEntropy(QStringList() << "e"
                                               << "entropy",
                                 "Show entropy.");
    QCommandLineOption clInfo(QStringList() << "i"
                                            << "info",
                              "Show file info.");
    QCommandLineOption clResultAsXml(QStringList() << "x"
                                                   << "xml",
                                     "Result as XML.");
    QCommandLineOption clResultAsJson(QStringList() << "j"
                                                    << "json",
                                      "Result as JSON.");
    QCommandLineOption clResultAsCSV(QStringList() << "c"
                                                   << "csv",
                                     "Result as CSV.");
    QCommandLineOption clResultAsTSV(QStringList() << "t"
                                                   << "tsv",
                                     "Result as TSV.");
    QCommandLineOption clResultAsPlainText(QStringList() << "p"
                                                         << "plaintext",
                                           "Result as Plain Text.");
    QCommandLineOption clDatabaseMain(QStringList() << "D"
                                                    << "database",
                                      "Set database<path>.", "path");
    QCommandLineOption clDatabaseExtra(QStringList() << "E"
                                                     << "extradatabase",
                                       "Set extra database<path>.", "path");
    QCommandLineOption clDatabaseCustom(QStringList() << "C"
                                                      << "customdatabase",
                                        "Set custom database<path>.", "path");
    QCommandLineOption clShowDatabase(QStringList() << "s"
                                                    << "showdatabase",
                                      "Show database.");
    QCommandLineOption clSpecial(QStringList() << "S"
                                               << "special",
                                 "Special file info for <method>. For example -S \"Hash\" or -S \"Hash#MD5\".", "method");
    QCommandLineOption clShowMethods(QStringList() << "m"
                                                   << "showmethods",
                                     "Show all special methods for the file.");
    QCommandLineOption clTest(QStringList() << "test", "Test signatures in <directory>.", "directory");
    QCommandLineOption clAddTest(QStringList() << "addtest", "Add test: --addtest <filename> <detect_string> <directory>.", "filename", "");

    parser.addOption(clRecursiveScan);
    parser.addOption(clDeepScan);
    parser.addOption(clHeuristicScan);
    parser.addOption(clVerbose);
    parser.addOption(clAggresiveScan);
    parser.addOption(clAllTypesScan);
    parser.addOption(clFormatResult);
    parser.addOption(clProfiling);
    parser.addOption(clHideUnknown);
    parser.addOption(clEntropy);
    parser.addOption(clInfo);
    parser.addOption(clSpecial);
    parser.addOption(clResultAsXml);
    parser.addOption(clResultAsJson);
    parser.addOption(clResultAsCSV);
    parser.addOption(clResultAsTSV);
    parser.addOption(clResultAsPlainText);
    parser.addOption(clDatabaseMain);
    parser.addOption(clDatabaseExtra);
    parser.addOption(clDatabaseCustom);
    parser.addOption(clShowDatabase);
    parser.addOption(clShowMethods);
    parser.addOption(clTest);
    parser.addOption(clAddTest);

    parser.process(app);

    QList<QString> listArgs = parser.positionalArguments();

    XScanEngine::SCAN_OPTIONS scanOptions = {};

    scanOptions.bUseCustomDatabase = true;
    scanOptions.bUseExtraDatabase = true;
    scanOptions.bShowType = true;
    scanOptions.bShowInfo = true;
    scanOptions.bShowVersion = true;
    scanOptions.bIsRecursiveScan = parser.isSet(clRecursiveScan);
    scanOptions.bIsDeepScan = parser.isSet(clDeepScan);
    scanOptions.bIsHeuristicScan = parser.isSet(clHeuristicScan);
    scanOptions.bIsVerbose = parser.isSet(clVerbose);
    scanOptions.bIsAggressiveScan = parser.isSet(clAggresiveScan);
    scanOptions.bIsAllTypesScan = parser.isSet(clAllTypesScan);
    scanOptions.bFormatResult = parser.isSet(clFormatResult);
    scanOptions.bHideUnknown = parser.isSet(clHideUnknown);
    scanOptions.bLogProfiling = parser.isSet(clProfiling);
    scanOptions.nBufferSize = 2 * 1024 * 1024;  // TODO
    scanOptions.bShowEntropy = parser.isSet(clEntropy);
    scanOptions.bShowFileInfo = parser.isSet(clInfo);
    scanOptions.bResultAsXML = parser.isSet(clResultAsXml);
    scanOptions.bResultAsJSON = parser.isSet(clResultAsJson);
    scanOptions.bResultAsCSV = parser.isSet(clResultAsCSV);
    scanOptions.bResultAsTSV = parser.isSet(clResultAsTSV);
    scanOptions.bResultAsPlainText = parser.isSet(clResultAsPlainText);
    scanOptions.bIsHighlight = true;
    scanOptions.bIsSort = true;

    scanOptions.sSpecial = parser.value(clSpecial);

    QString sDatabaseMain = parser.value(clDatabaseMain);
    QString sDatabaseExtra = parser.value(clDatabaseExtra);
    QString sDatabaseCustom = parser.value(clDatabaseCustom);
    QString sTestDirectory = parser.value(clTest);
    QString sAddTestFilename = parser.value(clAddTest);

    if (sDatabaseMain == "") {
        sDatabaseMain = XOptions().getApplicationDataPath() + QDir::separator() + "db";
    }

    if (sDatabaseExtra == "") {
        sDatabaseExtra = XOptions().getApplicationDataPath() + QDir::separator() + "db_extra";
    }

    if (sDatabaseCustom == "") {
        sDatabaseCustom = XOptions().getApplicationDataPath() + QDir::separator() + "db_custom";
    }

    ConsoleOutput consoleOutput;
    DiE_Script die_script;

    QObject::connect(&die_script, SIGNAL(errorMessage(QString)), &consoleOutput, SLOT(errorMessage(QString)));
    QObject::connect(&die_script, SIGNAL(warningMessage(QString)), &consoleOutput, SLOT(warningMessage(QString)));
    QObject::connect(&die_script, SIGNAL(infoMessage(QString)), &consoleOutput, SLOT(infoMessage(QString)));

    bool bIsDbUsed = false;
    bool bDbLoaded = false;

    if (parser.isSet(clShowDatabase)) {
        if (!bIsDbUsed) {
            die_script.initDatabase();
            bDbLoaded = die_script.loadDatabase(sDatabaseMain, DiE_ScriptEngine::DT_MAIN, nullptr);
            die_script.loadDatabase(sDatabaseExtra, DiE_ScriptEngine::DT_EXTRA, nullptr);
            die_script.loadDatabase(sDatabaseCustom, DiE_ScriptEngine::DT_CUSTOM, nullptr);
            bIsDbUsed = true;
        }

        printf("Main database: %s\n", sDatabaseMain.toUtf8().data());
        printf("Extra database: %s\n", sDatabaseMain.toUtf8().data());
        printf("Custom database: %s\n", sDatabaseCustom.toUtf8().data());

        QList<DiE_Script::SIGNATURE_STATE> list = die_script.getSignatureStates();

        qint32 nNumberOfRecords = list.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            printf("\t%s: %d\n", XBinary::fileTypeIdToString(list.at(i).fileType).toUtf8().data(), list.at(i).nNumberOfSignatures);
        }
    }

    if (parser.isSet(clShowMethods)) {
        XBinary::FT fileType = XBinary::FT_UNKNOWN;

        if (listArgs.count()) {
            fileType = XBinary::getPrefFileType(listArgs.at(0));
        }

        printf("Methods:\n");

        QList<QString> listMethods = XFileInfo::getMethodNames(fileType);

        qint32 nNumberOfMethods = listMethods.count();

        for (qint32 i = 0; i < nNumberOfMethods; i++) {
            printf("\t%s\n", listMethods.at(i).toUtf8().data());
        }
    } else if (parser.isSet(clTest)) {
        if (!bIsDbUsed) {
            die_script.initDatabase();
            bDbLoaded = die_script.loadDatabase(sDatabaseMain, DiE_ScriptEngine::DT_MAIN, nullptr);
            die_script.loadDatabase(sDatabaseExtra, DiE_ScriptEngine::DT_EXTRA, nullptr);
            die_script.loadDatabase(sDatabaseCustom, DiE_ScriptEngine::DT_CUSTOM, nullptr);
            bIsDbUsed = true;
        }

        // TODO
    } else if (parser.isSet(clAddTest)) {
        if (!bIsDbUsed) {
            die_script.initDatabase();
            bDbLoaded = die_script.loadDatabase(sDatabaseMain, DiE_ScriptEngine::DT_MAIN, nullptr);
            die_script.loadDatabase(sDatabaseExtra, DiE_ScriptEngine::DT_EXTRA, nullptr);
            die_script.loadDatabase(sDatabaseCustom, DiE_ScriptEngine::DT_CUSTOM, nullptr);
            bIsDbUsed = true;
        }

        if (listArgs.count() >= 2) {
            QString sDetectString = listArgs.at(0);
            QString sDirectory = listArgs.at(1);
            printf("Adding test for file '%s' with detect string '%s' in directory '%s'\n", sAddTestFilename.toUtf8().data(), sDetectString.toUtf8().data(),
                   sDirectory.toUtf8().data());

            // TODO
        } else {
            printf("Error: --addtest requires <filename> <detect_string> <directory>\n");
            nResult = XOptions::CR_INVALIDPARAMETER;
        }
    } else if (listArgs.count()) {
        if (!bIsDbUsed) {
            die_script.initDatabase();
            bDbLoaded = die_script.loadDatabase(sDatabaseMain, DiE_ScriptEngine::DT_MAIN, nullptr);
            die_script.loadDatabase(sDatabaseExtra, DiE_ScriptEngine::DT_EXTRA, nullptr);
            die_script.loadDatabase(sDatabaseCustom, DiE_ScriptEngine::DT_CUSTOM, nullptr);
        }

        nResult = ScanFiles(&listArgs, &scanOptions, &die_script);
    } else if (!parser.isSet(clShowDatabase)) {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    if (bIsDbUsed && (!bDbLoaded)) {
        nResult = XOptions::CR_CANNOTFINDDATABASE;
    }

    return nResult;
}
