/* Copyright (c) 2026 hors<horsicq@gmail.com> SPDX-License-Identifier: MIT */
#include "databaseupdater.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <cstdio>
#include <csignal>
#ifdef Q_OS_WIN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace {
volatile std::sig_atomic_t interrupted = 0;
void onSignal(int) { interrupted = 1; }
#ifdef Q_OS_WIN
BOOL WINAPI onControl(DWORD event)
{
    if (event != CTRL_C_EVENT && event != CTRL_BREAK_EVENT) return FALSE;
    interrupted = 1; return TRUE;
}
#endif
void writeText(FILE *stream, const QString &text)
{
    QByteArray data = text.toUtf8();
    std::fwrite(data.constData(), 1, size_t(data.size()), stream);
}
void help()
{
    writeText(stdout, QStringLiteral(
        "die-update v") + DIE_UPDATE_VERSION + QStringLiteral(
        "\nDetect It Easy signature database updater\n\nUsage: die-update [options]\n\n"
        "Without options the main database is updated when a newer one is available.\n\n"
        "  -h, --help                Display this help.\n"
        "  -v, --version             Display version information.\n"
        "  -c, --check               Check only (exit 1 when an update is available).\n"
        "  -u, --update              Install an available update (default).\n"
        "  -f, --force               Reinstall even when up to date.\n"
        "  -D, --database <path>     Main database folder.\n"
        "  -U, --url <url>           GitHub release page or ZIP URL.\n"
        "  -s, --settings <ini>      Read this settings file.\n"
        "      --nosettings          Ignore stored settings.\n"
        "  -j, --json                Print the result as JSON.\n"
        "  -q, --quiet               Print errors only.\n\nDefault source: ") + DatabaseUpdater::defaultUrl() + QStringLiteral(
        "\nSettings: Scan/Die/Database/Main/Path, Scan/Die/Database/Main/UpdateUrl\n"
        "Exit codes: 0 current/installed, 1 update available, 2 network, 3 database,\n"
        "4 invalid argument/settings, 5 verification, 6 cancelled, 7 busy.\n"));
}
QJsonValue optional(const QString &value) { return value.isEmpty() ? QJsonValue(QJsonValue::Null) : QJsonValue(value); }
void printJson(bool check, DatabaseUpdater::Status status, const DatabaseUpdater::Info &info)
{
    QJsonObject object;
    object.insert("operation", check ? "check" : "update");
    object.insert("status", DatabaseUpdater::statusName(status));
    object.insert("updateAvailable", info.updateAvailable);
    object.insert("installed", info.installed);
    object.insert("userFolder", info.usedUserFolder);
    object.insert("database", optional(QDir::toNativeSeparators(info.mainPath)));
    object.insert("source", optional(DatabaseUpdater::displayUrl(info.sourceUrl)));
    object.insert("release", optional(info.releaseTag));
    object.insert("archive", optional(DatabaseUpdater::displayUrl(info.archiveUrl)));
    object.insert("archiveSize", info.remoteSize < 0 ? QJsonValue(QJsonValue::Null) : QJsonValue(QString::number(info.remoteSize)));
    object.insert("archiveTime", info.remoteTime ? QJsonValue(QDateTime::fromMSecsSinceEpoch(info.remoteTime * 1000, Qt::UTC).toString("yyyy-MM-ddTHH:mm:ssZ")) : QJsonValue(QJsonValue::Null));
    object.insert("remoteMd5", optional(info.remoteMd5));
    object.insert("installedMd5", optional(info.installedMd5));
    object.insert("installedAt", optional(info.installedAt));
    object.insert("signatures", info.signatures);
    object.insert("message", info.message);
    QByteArray data = QJsonDocument(object).toJson();
    std::fwrite(data.constData(), 1, size_t(data.size()), stdout);
}
}

int main(int argc, char **argv)
{
    QCoreApplication application(argc, argv);
    application.setApplicationName("die");
    application.setOrganizationName("NTInfo");
    application.setOrganizationDomain("ntinfo.biz");
    application.setApplicationVersion(DIE_UPDATE_VERSION);
    DatabaseUpdater::Config config;
    bool check = false, modeSet = false, force = false, json = false, quiet = false, noSettings = false, wantHelp = false, wantVersion = false;
    QString settingsFile, error;
    QStringList arguments = application.arguments();
    for (int i = 1; i < arguments.size() && error.isEmpty(); ++i) {
        QString option = arguments[i];
        auto value = [&]() -> QString {
            if (++i >= arguments.size()) { error = option + " needs a value"; return {}; }
            return arguments[i];
        };
        if (option == "-h" || option == "--help" || option == "-?") wantHelp = true;
        else if (option == "-v" || option == "--version") wantVersion = true;
        else if (option == "-c" || option == "--check" || option == "-u" || option == "--update") {
            bool requested = option == "-c" || option == "--check";
            if (modeSet && requested != check) error = "--check and --update are mutually exclusive";
            check = requested; modeSet = true;
        } else if (option == "-f" || option == "--force") force = true;
        else if (option == "-j" || option == "--json") json = true;
        else if (option == "-q" || option == "--quiet") quiet = true;
        else if (option == "--nosettings") noSettings = true;
        else if (option == "-D" || option == "--database") config.mainPath = value();
        else if (option == "-U" || option == "--url") config.url = value();
        else if (option == "-s" || option == "--settings") settingsFile = value();
        else if (option == "--api-base") config.apiBase = value();
        else if (option == "--noproxy") config.noProxy = true;
        else if (option == "--nouserfolder") config.userFallback = false;
        else if (option == "--timeout") {
            bool valid = false; QString text = value();
            config.timeoutMs = text.toUInt(&valid);
            if (error.isEmpty() && !valid) error = "--timeout requires milliseconds as an unsigned integer";
        } else if (option == "-E" || option == "--extradatabase") error = option + " was removed: only the main database (-D) is updated";
        else error = "unknown option: " + option;
    }
    if (error.isEmpty() && (wantHelp || wantVersion)) {
        if (wantHelp) help(); else writeText(stdout, QString("die-update v%1\n").arg(DIE_UPDATE_VERSION));
        return DatabaseUpdater::Ok;
    }
    if (error.isEmpty() && force && check) error = "--force applies to an update, not to --check";
    if (error.isEmpty() && !settingsFile.isEmpty() && noSettings) error = "--settings and --nosettings are mutually exclusive";
    if (error.isEmpty() && !noSettings) DatabaseUpdater::loadSettings(config, settingsFile, &error);
    if (!error.isEmpty()) { writeText(stderr, "die-update: " + error + '\n'); return DatabaseUpdater::ArgumentError; }
#ifdef Q_OS_WIN
    SetConsoleCtrlHandler(onControl, TRUE);
#else
    std::signal(SIGINT, onSignal); std::signal(SIGTERM, onSignal);
#endif
    DatabaseUpdater::Info info;
    DatabaseUpdater::Status status = check ? DatabaseUpdater::check(config, info, [] { return interrupted != 0; })
                                          : DatabaseUpdater::install(config, info, force, [] { return interrupted != 0; });
    if (json) printJson(check, status, info);
    else {
        if (!quiet) {
            if (!info.mainPath.isEmpty()) writeText(stdout, "Database:   " + QDir::toNativeSeparators(info.mainPath) + '\n');
            if (!config.settingsLocation.isEmpty()) writeText(stdout, "Settings:   " + config.settingsLocation + '\n');
            writeText(stdout, "Source:     " + DatabaseUpdater::displayUrl(info.sourceUrl) + '\n');
            if (!info.releaseTag.isEmpty()) writeText(stdout, "Release:    " + (info.releaseName.isEmpty() ? info.releaseTag : info.releaseName + " (" + info.releaseTag + ')') + '\n');
            if (!info.archiveUrl.isEmpty()) {
                QString archive = "Archive:    " + info.archiveName;
                if (info.remoteSize >= 0) archive += QString(", %1 bytes").arg(info.remoteSize);
                if (info.remoteTime) archive += ", " + QDateTime::fromMSecsSinceEpoch(info.remoteTime * 1000, Qt::UTC).toString("yyyy-MM-ddTHH:mm:ssZ");
                writeText(stdout, archive + '\n');
            }
            if (!info.remoteMd5.isEmpty()) writeText(stdout, "MD5:        " + info.remoteMd5 + '\n');
            if (status == DatabaseUpdater::Ok || status == DatabaseUpdater::Available) {
                QString installed = info.installedMd5.isEmpty() ? (info.hasDatabase ? "no update record" : "none") : info.installedMd5;
                if (!info.installedMd5.isEmpty() && !info.installedAt.isEmpty()) installed += " (" + info.installedAt + ')';
                writeText(stdout, "Installed:  " + installed + '\n');
            }
            if (status == DatabaseUpdater::Available) writeText(stdout, "An update is available. Run die-update to install it.\n");
            else if (status == DatabaseUpdater::Ok) writeText(stdout, info.message + ".\n");
        }
        if (status != DatabaseUpdater::Ok && status != DatabaseUpdater::Available) writeText(stderr, "die-update: " + info.message + '\n');
    }
    return status;
}
