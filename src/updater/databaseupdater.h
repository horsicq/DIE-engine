/* Copyright (c) 2026 hors<horsicq@gmail.com> SPDX-License-Identifier: MIT */
#ifndef DATABASEUPDATER_H
#define DATABASEUPDATER_H

#include <QString>
#include <functional>

namespace DatabaseUpdater {
enum Status { Ok = 0, Available = 1, NetworkError = 2, DatabaseError = 3, ArgumentError = 4,
              VerificationError = 5, Cancelled = 6, Busy = 7 };

struct Config {
    QString mainPath;
    QString url;
    QString apiBase;
    QString settingsLocation;
    bool noProxy = false;
    bool userFallback = true;
    quint32 timeoutMs = 0;
};

struct Info {
    QString sourceUrl, archiveUrl, archiveName, releaseName, releaseTag;
    qint64 remoteSize = -1;
    qint64 remoteTime = 0;
    QString remoteMd5, remoteAsset;
    QString mainPath;
    bool mainIsDefault = false;
    bool hasDatabase = false;
    QString installedMd5, installedAsset, installedAt, installedSource;
    bool updateAvailable = false;
    bool installed = false;
    bool usedUserFolder = false;
    int signatures = 0;
    QString message;
};

QString defaultUrl();
QString displayUrl(const QString &url);
QString statusName(Status status);
Status loadSettings(Config &config, const QString &file, QString *error);
Status check(const Config &config, Info &info, const std::function<bool()> &cancelled = {});
Status install(const Config &config, Info &info, bool force, const std::function<bool()> &cancelled = {});
}
#endif
