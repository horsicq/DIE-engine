/* Copyright (c) 2020-2026 hors<horsicq@gmail.com>
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

void progressCallback(void *pUserData, XBinary::PDSTRUCT *pPdStruct)
{
    Q_UNUSED(pUserData)

    if (pPdStruct) {
        printf("\r");

        // Display percentage for each valid progress level
        bool bFirst = true;
        for (qint32 i = 0; i < XBinary::N_NUMBER_PDRECORDS; i++) {
            if (pPdStruct->_pdRecord[i].bIsValid) {
                qint32 nPercent = 0;
                if (pPdStruct->_pdRecord[i].nTotal > 0) {
                    nPercent = (qint32)((pPdStruct->_pdRecord[i].nCurrent * 100) / pPdStruct->_pdRecord[i].nTotal);
                    if (nPercent > 100) nPercent = 100;
                }
                printf("[%3d%%]", nPercent);
                bFirst = false;
            }
        }

        if (!bFirst) {
            printf(" : ");

            // Display status for each valid level
            bool bFirstStatus = true;
            for (qint32 i = 0; i < XBinary::N_NUMBER_PDRECORDS; i++) {
                if (pPdStruct->_pdRecord[i].bIsValid) {
                    if (!bFirstStatus) printf("|");
                    if (!pPdStruct->_pdRecord[i].sStatus.isEmpty()) {
                        printf("%s", pPdStruct->_pdRecord[i].sStatus.toUtf8().data());
                    } else {
                        printf("-");
                    }
                    bFirstStatus = false;
                }
            }
        }

        fflush(stdout);

        // Check if all valid levels are complete
        bool bAllComplete = true;
        for (qint32 i = 0; i < XBinary::N_NUMBER_PDRECORDS; i++) {
            if (pPdStruct->_pdRecord[i].bIsValid) {
                if (pPdStruct->_pdRecord[i].nTotal > 0 && pPdStruct->_pdRecord[i].nCurrent < pPdStruct->_pdRecord[i].nTotal) {
                    bAllComplete = false;
                    break;
                }
            }
        }

        if (bAllComplete) {
            printf("\n");
        }
    }
}

XOptions::CR ScanFiles(QList<QString> *pListArgs, XScanEngine::SCAN_OPTIONS *pScanOptions, DiE_Script *pDieScript, XBinary::PDSTRUCT *pPdStruct)
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
            // pdStruct.pCallback = progressCallback;
            //pdStruct.pCallbackUserData = nullptr;

            XScanEngine::SCAN_RESULT scanResult = pDieScript->scanFile(sFileName, pScanOptions, pPdStruct);

            ScanItemModel model(pScanOptions, &(scanResult.listRecords), 1, nullptr);

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

    app.setProperty("dataPathAlt0", "/opt/detect-it-easy");

    XOptions::registerCodecs();

    XBinary::PDSTRUCT pdStruct = XBinary::createPdStruct();

    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2006-2008 Hellsp@wn 2012-%1 hors<horsicq@gmail.com> Web: http://ntinfo.biz").arg(QDate::currentDate().year()));
    parser.setApplicationDescription(sDescription);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("target", "The file or directory to open.");

    QCommandLineOption clRecursiveScan = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_RECURSIVESCAN);
    QCommandLineOption clDeepScan = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_DEEPSCAN);
    QCommandLineOption clHeuristicScan = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_HEURISTICSCAN);
    QCommandLineOption clVerbose = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_VERBOSE);
    QCommandLineOption clAggresiveScan = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_AGGRESSIVESCAN);
    QCommandLineOption clAllTypesScan = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_ALLTYPES);
    QCommandLineOption clFormatResult = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_FORMAT);
    QCommandLineOption clProfiling = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_PROFILING);
    QCommandLineOption clMessages = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_MESSAGES);
    QCommandLineOption clHideUnknown = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_HIDEUNKNOWN);
    QCommandLineOption clEntropy = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_ENTROPY);
    QCommandLineOption clInfo = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_INFO);
    QCommandLineOption clResultAsXml = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_XML);
    QCommandLineOption clResultAsJson = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_JSON);
    QCommandLineOption clResultAsCSV = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_CSV);
    QCommandLineOption clResultAsTSV = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_TSV);
    QCommandLineOption clResultAsPlainText = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_PLAINTEXT);
    QCommandLineOption clDatabaseMain = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_DATABASE);
    QCommandLineOption clDatabaseExtra = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_EXTRADATABASE);
    QCommandLineOption clDatabaseCustom = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_CUSTOMDATABASE);
    QCommandLineOption clShowDatabase = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_SHOWDATABASE);
    QCommandLineOption clStruct = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_STRUCT);
    QCommandLineOption clShowStructs = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_SHOWSTRUCTS);
    QCommandLineOption clTest = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_TEST);
    QCommandLineOption clAddTest = XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_ADDTEST);

    parser.addOption(clRecursiveScan);
    parser.addOption(clDeepScan);
    parser.addOption(clHeuristicScan);
    parser.addOption(clVerbose);
    parser.addOption(clAggresiveScan);
    parser.addOption(clAllTypesScan);
    parser.addOption(clFormatResult);
    parser.addOption(clProfiling);
    parser.addOption(clMessages);
    parser.addOption(clHideUnknown);
    parser.addOption(clEntropy);
    parser.addOption(clInfo);
    parser.addOption(clStruct);
    parser.addOption(clResultAsXml);
    parser.addOption(clResultAsJson);
    parser.addOption(clResultAsCSV);
    parser.addOption(clResultAsTSV);
    parser.addOption(clResultAsPlainText);
    parser.addOption(clDatabaseMain);
    parser.addOption(clDatabaseExtra);
    parser.addOption(clDatabaseCustom);
    parser.addOption(clShowDatabase);
    parser.addOption(clShowStructs);
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
    scanOptions.bShowEntropy = parser.isSet(clEntropy);
    scanOptions.bShowFileInfo = parser.isSet(clInfo);
    scanOptions.bResultAsXML = parser.isSet(clResultAsXml);
    scanOptions.bResultAsJSON = parser.isSet(clResultAsJson);
    scanOptions.bResultAsCSV = parser.isSet(clResultAsCSV);
    scanOptions.bResultAsTSV = parser.isSet(clResultAsTSV);
    scanOptions.bResultAsPlainText = parser.isSet(clResultAsPlainText);
    scanOptions.bIsSort = true;

    scanOptions.sSpecial = parser.value(clStruct);

    scanOptions.sMainDatabasePath = parser.value(clDatabaseMain);
    scanOptions.sExtraDatabasePath = parser.value(clDatabaseExtra);
    scanOptions.sCustomDatabasePath = parser.value(clDatabaseCustom);
    QString sTestDirectory = parser.value(clTest);
    QString sAddTestFilename = parser.value(clAddTest);

    if (scanOptions.sMainDatabasePath == "") {
        scanOptions.sMainDatabasePath = "$data/db";
    }

    if (scanOptions.sExtraDatabasePath == "") {
        scanOptions.sExtraDatabasePath = "$data/db_extra";
    }

    if (scanOptions.sCustomDatabasePath == "") {
        scanOptions.sCustomDatabasePath = "$data/db_custom";
    }

    ConsoleOutput consoleOutput;
    DiE_Script die_script;

    if (parser.isSet(clMessages)) {
        QObject::connect(&die_script, SIGNAL(errorMessage(QString)), &consoleOutput, SLOT(errorMessage(QString)));
        QObject::connect(&die_script, SIGNAL(warningMessage(QString)), &consoleOutput, SLOT(warningMessage(QString)));
        QObject::connect(&die_script, SIGNAL(infoMessage(QString)), &consoleOutput, SLOT(infoMessage(QString)));
    }

    bool bIsDbUsed = false;
    bool bDbLoaded = false;

    if (parser.isSet(clShowDatabase)) {
        if (!bIsDbUsed) {
            bDbLoaded = die_script.loadDatabase(&scanOptions, &pdStruct);
            bIsDbUsed = true;
        }

        printf("Main database: %s\n", scanOptions.sMainDatabasePath.toUtf8().data());
        printf("Extra database: %s\n", scanOptions.sExtraDatabasePath.toUtf8().data());
        printf("Custom database: %s\n", scanOptions.sCustomDatabasePath.toUtf8().data());

        QList<DiE_Script::SIGNATURE_STATE> list = die_script.getSignatureStates();

        qint32 nNumberOfRecords = list.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            printf("\t%s: %d\n", XBinary::fileTypeIdToString(list.at(i).fileType).toUtf8().data(), list.at(i).nNumberOfSignatures);
        }
    }

    if (parser.isSet(clShowStructs)) {
        XBinary::FT fileType = XBinary::FT_UNKNOWN;

        // if (listArgs.count()) {
        //     fileType = XFormats::getPrefFileType(listArgs.at(0));
        // }

        printf("Structures:\n");

        QList<QString> listMethods = XFileInfo::getMethodNames(fileType);

        qint32 nNumberOfMethods = listMethods.count();

        for (qint32 i = 0; i < nNumberOfMethods; i++) {
            printf("\t%s\n", listMethods.at(i).toUtf8().data());
        }
    } else if (parser.isSet(clTest)) {
        if (!bIsDbUsed) {
            bDbLoaded = die_script.loadDatabase(&scanOptions, &pdStruct);
            bIsDbUsed = true;
        }

        // TODO
    } else if (parser.isSet(clAddTest)) {
        if (!bIsDbUsed) {
            bDbLoaded = die_script.loadDatabase(&scanOptions, &pdStruct);
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
            bDbLoaded = die_script.loadDatabase(&scanOptions, &pdStruct);
        }

        nResult = ScanFiles(&listArgs, &scanOptions, &die_script, &pdStruct);
    } else if (!parser.isSet(clShowDatabase)) {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    if (bIsDbUsed && (!bDbLoaded)) {
        nResult = XOptions::CR_CANNOTFINDDATABASE;
    }

    return nResult;
}
