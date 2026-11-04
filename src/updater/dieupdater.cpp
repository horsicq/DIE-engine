/* Copyright (c) 2026 hors<horsicq@gmail.com>
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
#include "dieupdater.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

QString DieUpdater::getProgramPath()
{
    QString sResult = QString::fromLocal8Bit(qgetenv("DIE_UPDATER"));

    if (sResult.isEmpty()) {
        sResult = QCoreApplication::applicationDirPath() + QDir::separator() + QString("die-update");
#ifdef Q_OS_WIN
        sResult += QString(".exe");
#endif
    }

    return QDir::cleanPath(sResult);
}

QStringList DieUpdater::getArguments(bool bCheck, const QString &sDatabase, bool bJson)
{
    QStringList listResult;

    if (bCheck) {
        listResult.append(QString("--check"));
    }
    if (!sDatabase.isEmpty()) {
        listResult.append(QString("--database"));
        listResult.append(sDatabase);
    }
    if (bJson) {
        listResult.append(QString("--json"));
    }

    return listResult;
}

qint32 DieUpdater::run(bool bCheck, const QString &sDatabase, bool bJson, QString *pError)
{
    QString sProgram = getProgramPath();

    if (!QFileInfo(sProgram).isFile()) {
        if (pError) {
            *pError = QCoreApplication::translate("DieUpdater", "Cannot find the database updater: %1").arg(sProgram);
        }
        return CANNOT_RUN;
    }

    fflush(stdout);
    fflush(stderr);

    QProcess process;
    process.setProcessChannelMode(QProcess::ForwardedChannels);
    process.start(sProgram, getArguments(bCheck, sDatabase, bJson));

    if (!process.waitForStarted()) {
        if (pError) {
            *pError = QCoreApplication::translate("DieUpdater", "Cannot run %1: %2").arg(sProgram, process.errorString());
        }
        return CANNOT_RUN;
    }

    process.waitForFinished(-1);

    if (process.exitStatus() != QProcess::NormalExit) {
        if (pError) {
            *pError = QCoreApplication::translate("DieUpdater", "%1 terminated unexpectedly").arg(sProgram);
        }
        return CANNOT_RUN;
    }

    return process.exitCode();
}

QString DieUpdater::getUserDataPath()
{
    QString sOverride = QString::fromLocal8Bit(qgetenv("DIE_USER_DATA_DIR"));

    if (!sOverride.isEmpty()) {
        return QDir::cleanPath(sOverride);
    }

#ifdef Q_OS_WIN
    QString sBase = QString::fromLocal8Bit(qgetenv("LOCALAPPDATA"));
    return sBase.isEmpty() ? QString() : QDir::cleanPath(sBase + QString("/NTInfo/die"));
#elif defined(Q_OS_MAC)
    QString sBase = QString::fromLocal8Bit(qgetenv("HOME"));
    return sBase.isEmpty() ? QString() : QDir::cleanPath(sBase + QString("/Library/Application Support/NTInfo/die"));
#else
    QString sBase = QString::fromLocal8Bit(qgetenv("XDG_DATA_HOME"));
    if (!sBase.startsWith(QChar('/'))) {
        sBase.clear();
    }
    if (!sBase.isEmpty()) {
        return QDir::cleanPath(sBase + QString("/NTInfo/die"));
    }
    sBase = QString::fromLocal8Bit(qgetenv("HOME"));
    return sBase.isEmpty() ? QString() : QDir::cleanPath(sBase + QString("/.local/share/NTInfo/die"));
#endif
}

QString DieUpdater::resolveDatabasePath(const QString &sPath)
{
    if ((sPath != QString("$data/db")) && (sPath != QString("$data\\db"))) {
        return sPath;
    }

    QString sApplicationDir = QCoreApplication::applicationDirPath();
    bool bPortable = QFileInfo(sApplicationDir + QString("/portable")).isFile() || QFileInfo(sApplicationDir + QString("/PORTABLE")).isFile();

    if (!bPortable) {
        QString sUserDatabase = getUserDataPath();
        if (!sUserDatabase.isEmpty()) {
            sUserDatabase += QString("/db");
            if (QDir(sUserDatabase).exists()) {
                return QDir::cleanPath(sUserDatabase);
            }
        }
    }

    return sPath;
}
