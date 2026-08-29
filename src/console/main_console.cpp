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
#include <QDate>

#include "../global.h"
#include "die_script.h"
#include "entropyprocess.h"
#include "xfileinfo.h"
#include "xoptions.h"
#include "xscanengineconsole.h"

// diec on the shared XScanEngineConsole. The overrides keep the established
// die-specific behavior: EntropyProcess/XFileInfo output for -e/-i/-S,
// the signature-state database listing, the --format flag (bFormatResult is
// opt-in for diec, unlike the other scanners), and the --test/--createtest
// stubs.
class DIEConsole : public XScanEngineConsole {
public:
    DIEConsole(QCoreApplication &app, DiE_Script &dieScript, const QString &sDescription);

protected:
    virtual void addEngineOptions(QCommandLineParser *pParser);
    virtual void applyEngineOptions(const QCommandLineParser *pParser, XScanEngine::SCAN_OPTIONS *pScanOptions);
    virtual bool processEngineModes(const QCommandLineParser *pParser, const QStringList &listArgs, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct,
                                    qint32 *pnResult);
    virtual XOptions::CR reportScanErrors(XScanEngine::SCAN_RESULT *pScanResult);
    virtual XOptions::CR showDatabaseState(XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct);
    virtual XOptions::CR showStructsOverview(const QStringList &listArgs, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct);
    virtual XOptions::CR showFileEntropy(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct);
    virtual XOptions::CR showFileInfo(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct);
    virtual XOptions::CR showFileStruct(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct);

private:
    XOptions::CR printFileInfo(const QString &sFileName, const QString &sString, XScanEngine::SCAN_OPTIONS *pScanOptions);

    DiE_Script &m_dieScript;
    QCommandLineOption m_clFormatResult;
    QCommandLineOption m_clTest;
    QCommandLineOption m_clCreateTest;
};

DIEConsole::DIEConsole(QCoreApplication &app, DiE_Script &dieScript, const QString &sDescription)
    : XScanEngineConsole(app, dieScript, sDescription),
      m_dieScript(dieScript),
      m_clFormatResult(XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_FORMAT)),
      m_clTest(XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_TEST)),
      m_clCreateTest(XOptions::getCommandLineOption(XOptions::CONSOLE_OPTION_ID_CREATETEST))
{
}

void DIEConsole::addEngineOptions(QCommandLineParser *pParser)
{
    pParser->addOption(m_clFormatResult);
    pParser->addOption(m_clTest);
    pParser->addOption(m_clCreateTest);
}

void DIEConsole::applyEngineOptions(const QCommandLineParser *pParser, XScanEngine::SCAN_OPTIONS *pScanOptions)
{
    // diec formats detect strings with spaces only on request
    pScanOptions->bFormatResult = pParser->isSet(m_clFormatResult);
}

bool DIEConsole::processEngineModes(const QCommandLineParser *pParser, const QStringList &listArgs, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct,
                                    qint32 *pnResult)
{
    bool bHandled = false;

    if (pParser->isSet(m_clTest)) {
        bool bDbLoaded = m_dieScript.loadDatabase(pScanOptions, pPdStruct);

        // TODO

        if (!bDbLoaded) {
            *pnResult = XOptions::CR_CANNOTFINDDATABASE;
        }

        bHandled = true;
    } else if (pParser->isSet(m_clCreateTest)) {
        bool bDbLoaded = m_dieScript.loadDatabase(pScanOptions, pPdStruct);

        if (listArgs.count() >= 2) {
            QString sAddTestFilename = pParser->value(m_clCreateTest);
            QString sDetectString = listArgs.at(0);
            QString sDirectory = listArgs.at(1);
            printf("Adding test for file '%s' with detect string '%s' in directory '%s'\n", sAddTestFilename.toUtf8().data(), sDetectString.toUtf8().data(),
                   sDirectory.toUtf8().data());

            // TODO
        } else {
            printf("Error: --addtest requires <filename> <detect_string> <directory>\n");
            *pnResult = XOptions::CR_INVALIDPARAMETER;
        }

        if (!bDbLoaded) {
            *pnResult = XOptions::CR_CANNOTFINDDATABASE;
        }

        bHandled = true;
    }

    return bHandled;
}

XOptions::CR DIEConsole::reportScanErrors(XScanEngine::SCAN_RESULT *pScanResult)
{
    // diec historically prints script errors but still exits with success
    printf("%s", XScanEngine::getErrorsString(pScanResult).toUtf8().data());

    return XOptions::CR_SUCCESS;
}

XOptions::CR DIEConsole::showDatabaseState(XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct)
{
    Q_UNUSED(pPdStruct)

    printf("Main database: %s\n", pScanOptions->sMainDatabasePath.toUtf8().data());
    printf("Custom database: %s\n", pScanOptions->sCustomDatabasePath.toUtf8().data());

    QList<DiE_Script::SIGNATURE_STATE> list = m_dieScript.getSignatureStates();

    qint32 nNumberOfRecords = list.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        printf("\t%s: %d\n", XBinary::fileTypeIdToString(list.at(i).fileType).toUtf8().data(), list.at(i).nNumberOfSignatures);
    }

    return XOptions::CR_SUCCESS;
}

XOptions::CR DIEConsole::showStructsOverview(const QStringList &listArgs, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct)
{
    Q_UNUSED(listArgs)
    Q_UNUSED(pScanOptions)
    Q_UNUSED(pPdStruct)

    printf("Structures:\n");

    QList<QString> listMethods = XFileInfo::getMethodNames(XBinary::FT_UNKNOWN);

    qint32 nNumberOfMethods = listMethods.count();

    for (qint32 i = 0; i < nNumberOfMethods; i++) {
        printf("\t%s\n", listMethods.at(i).toUtf8().data());
    }

    return XOptions::CR_SUCCESS;
}

XOptions::CR DIEConsole::showFileEntropy(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct)
{
    Q_UNUSED(pPdStruct)

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

    return XOptions::CR_SUCCESS;
}

XOptions::CR DIEConsole::printFileInfo(const QString &sFileName, const QString &sString, XScanEngine::SCAN_OPTIONS *pScanOptions)
{
    XOptions::CR result = XOptions::CR_SUCCESS;

    QString sResult;

    XFileInfo::OPTIONS options = {};

    options.sString = sString;

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

    return result;
}

XOptions::CR DIEConsole::showFileInfo(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct)
{
    Q_UNUSED(pPdStruct)

    // diec lets an explicit -S structure win over the plain -i info dump
    QString sString = (pScanOptions->sStruct != "") ? pScanOptions->sStruct : QString("Info");

    return printFileInfo(sFileName, sString, pScanOptions);
}

XOptions::CR DIEConsole::showFileStruct(const QString &sFileName, XScanEngine::SCAN_OPTIONS *pScanOptions, XBinary::PDSTRUCT *pPdStruct)
{
    Q_UNUSED(pPdStruct)

    return printFileInfo(sFileName, pScanOptions->sStruct, pScanOptions);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    app.setProperty("dataPathAlt0", "/opt/detect-it-easy");

    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2006-2008 Hellsp@wn 2012-%1 hors<horsicq@gmail.com> Web: http://ntinfo.biz").arg(QDate::currentDate().year()));

    DiE_Script dieScript;
    DIEConsole console(app, dieScript, sDescription);

    return console.process();
}
