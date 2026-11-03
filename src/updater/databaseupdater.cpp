/* Copyright (c) 2026 hors<horsicq@gmail.com> SPDX-License-Identifier: MIT */
#include "databaseupdater.h"
#include "databasearchive.h"
#include "dieupdater.h"

#include <QAuthenticator>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkProxyFactory>
#include <QNetworkProxyQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QSaveFile>
#include <QSettings>
#include <QTemporaryDir>
#include <QThread>
#include <QTimer>
#include <QUrl>
#include <climits>
#include <memory>
#ifdef Q_OS_WIN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace DatabaseUpdater {
namespace {
const qint64 MaxArchive = 256LL * 1024 * 1024;
const QString StampName = QStringLiteral("die-update.ini");
const QString StagePrefix = QStringLiteral(".die-update-");
const QString MainKey = QStringLiteral("Scan/Die/Database/Main/Path");
const QString UrlKey = QStringLiteral("Scan/Die/Database/Main/UpdateUrl");

bool stopped(const std::function<bool()> &cancelled) { return cancelled && cancelled(); }
QString sanitize(QString value)
{
    for (int i = 0; i < value.size(); ++i) {
        ushort c = value.at(i).unicode();
        if (c < 32 || (c >= 127 && c <= 159)) value[i] = QChar('?');
    }
    return value;
}
Status fail(Info &info, Status status, const QString &message)
{
    info.message = sanitize(message);
    return status;
}
QString absolute(const QString &path) { return QDir::cleanPath(QFileInfo(path).absoluteFilePath()); }
QString canonical(const QString &path)
{
    QFileInfo file(absolute(path));
    QString result = file.canonicalFilePath();
    if (result.isEmpty()) {
        QString parent = file.dir().canonicalPath();
        result = parent.isEmpty() ? file.absoluteFilePath() : QDir(parent).filePath(file.fileName());
    }
    result = QDir::cleanPath(QDir::fromNativeSeparators(result));
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    result = result.toLower();
#endif
    return result;
}
bool within(const QString &inner, const QString &outer)
{
    return inner == outer || inner.startsWith(outer.endsWith('/') ? outer : outer + '/');
}
bool portable()
{
    QDir app(QCoreApplication::applicationDirPath());
    return QFileInfo(app.filePath("PORTABLE")).isFile() || QFileInfo(app.filePath("portable")).isFile();
}
QString userDataPath()
{
#ifndef Q_OS_WIN
    if (geteuid() == 0) {
        QString home = QString::fromLocal8Bit(qgetenv("HOME"));
        struct stat state;
        if (home.isEmpty() || ::stat(QFile::encodeName(home).constData(), &state) || state.st_uid != 0) return {};
#ifdef Q_OS_MAC
        return QDir(home).filePath("Library/Application Support/NTInfo/cdie");
#else
        return QDir(home).filePath(".local/share/NTInfo/cdie");
#endif
    }
#endif
    QString result = DieUpdater::getUserDataPath();
    return result.isEmpty() ? QString() : absolute(result);
}
QString defaultDatabase()
{
    QStringList candidates;
    QString user = userDataPath();
    if (!portable() && !user.isEmpty()) candidates << QDir(user).filePath("db");
    QDir app(QCoreApplication::applicationDirPath());
    candidates << app.filePath("db");
#ifdef Q_OS_MAC
    candidates << app.filePath("../Resources/db");
#endif
    candidates << QDir::current().filePath("db");
    for (const QString &candidate : candidates) if (QFileInfo(candidate).isDir()) return absolute(candidate);
    return absolute("db");
}
QString protectedTarget(const QString &path)
{
    QString key = canonical(path);
    if (key.isEmpty()) return QStringLiteral("it cannot be resolved");
    if (QDir(key).isRoot()) return QStringLiteral("it is the root of a drive");
    if (key == canonical(QDir::homePath())) return QStringLiteral("it is the home folder");
    if (within(canonical(QCoreApplication::applicationDirPath()), key)) return QStringLiteral("it holds this program");
    if (within(canonical(QDir::currentPath()), key)) return QStringLiteral("it holds the current folder");
    return {};
}
bool looksLikeDatabase(const QString &path)
{
    QFileInfo target(path);
    if (!target.exists()) return !target.isSymLink();
    if (!target.isDir() || target.isSymLink()) return false;
    const QStringList types = QStringLiteral("Binary COM MSDOS NE LE LX PE ELF MACH MACHOFAT Archive ZIP JAR APK IPA DEX NPM Amiga AtariST DOS16M DOS4G PDF CFBF Image JPEG PNG RAR ISO9660 JavaClass PYC").split(' ');
    QFileInfoList entries = QDir(path).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    if (entries.isEmpty()) return true;
    for (const QFileInfo &entry : entries) {
        if (entry.isFile() && (entry.fileName() == StampName || entry.fileName() == "_init" || entry.fileName().endsWith(".sg"))) return true;
        if (entry.isDir() && types.contains(entry.fileName())) return true;
    }
    return false;
}
Status validateTarget(Info &info)
{
    QString reason = protectedTarget(info.mainPath);
    if (!reason.isEmpty()) return fail(info, ArgumentError, QString("Refusing to replace %1: %2").arg(info.mainPath, reason));
    if (QFileInfo(canonical(info.mainPath)).fileName() == "db_extra")
        return fail(info, ArgumentError, QString("Refusing to replace %1: no longer uses the extra database; -D names the main database folder (db)").arg(info.mainPath));
    if (QFileInfo(info.mainPath).isFile()) return fail(info, ArgumentError, QString("%1 is a file; only a database folder can be updated").arg(info.mainPath));
    if (!looksLikeDatabase(info.mainPath)) return fail(info, ArgumentError, QString("Refusing to replace %1: it does not look like a signature database").arg(info.mainPath));
    return Ok;
}
QString parseMd5(const QByteArray &data)
{
    QRegularExpression expression(QStringLiteral("^\\s*([0-9a-fA-F]{32})(?:\\s|$)"));
    QRegularExpressionMatch match = expression.match(QString::fromLatin1(data));
    return match.hasMatch() ? match.captured(1).toLower() : QString();
}
QMap<QString, QString> plainIni(const QString &path)
{
    QMap<QString, QString> result;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return result;
    while (!file.atEnd()) {
        QString line = QString::fromUtf8(file.readLine()).trimmed();
        int equals = line.indexOf('=');
        if (equals > 0) result.insert(line.left(equals), line.mid(equals + 1));
    }
    return result;
}
void readStamp(Info &info)
{
    auto values = plainIni(QDir(info.mainPath).filePath(StampName));
    info.installedMd5 = parseMd5(values.value("Md5").toLatin1());
    info.installedAsset = values.value("Asset");
    info.installedAt = values.value("Installed");
    info.installedSource = values.value("Source");
}
bool writeFile(const QString &path, const QByteArray &data)
{
    QSaveFile file(path);
    return file.open(QIODevice::WriteOnly) && file.write(data) == data.size() && file.commit();
}
bool writeStamp(const Info &info, const QString &md5, const QString &time)
{
    QString text = "; Written by die-update. Identifies the archive this database came from.\n[Update]\n";
    text += "Source=" + displayUrl(info.sourceUrl) + "\nArchive=" + displayUrl(info.archiveUrl) + '\n';
    if (!info.releaseTag.isEmpty()) text += "Release=" + info.releaseTag + '\n';
    text += "Md5=" + md5 + '\n';
    if (!info.remoteAsset.isEmpty()) text += "Asset=" + info.remoteAsset + '\n';
    text += "Installed=" + time + '\n';
    return writeFile(QDir(info.mainPath).filePath(StampName), text.toUtf8());
}

struct Response {
    Status status = Ok;
    int http = 0;
    QByteArray data;
    QString message;
};
Response fetch(const Config &config, const QString &address, qint64 limit,
               const std::function<bool()> &cancelled, QFile *output = nullptr)
{
    Response result;
    QUrl url(address);
    QNetworkAccessManager manager;
    if (config.noProxy) manager.setProxy(QNetworkProxy::NoProxy);
    // Use Qt's platform proxy resolver instead of a second networking library.
    if (!config.noProxy) {
        QList<QNetworkProxy> proxies = QNetworkProxyFactory::systemProxyForQuery(QNetworkProxyQuery(url));
        if (!proxies.isEmpty()) manager.setProxy(proxies.first());
    }
    QObject::connect(&manager, &QNetworkAccessManager::authenticationRequired,
                     [&url](QNetworkReply *, QAuthenticator *auth) {
        auth->setUser(url.userName());
        auth->setPassword(url.password());
    });
    for (int redirects = 0; redirects <= 10; ++redirects) {
        if (stopped(cancelled)) { result.status = Cancelled; result.message = "Cancelled"; return result; }
        if (!url.isValid() || url.host().isEmpty() || (url.scheme() != "http" && url.scheme() != "https")) {
            result.status = ArgumentError; result.message = "Not an http or https address: " + displayUrl(url.toString()); return result;
        }
        QNetworkRequest request(url);
        request.setRawHeader("User-Agent", QByteArray("die-update/") + DIE_UPDATE_VERSION + " QtNetwork");
        request.setRawHeader("Accept", "application/vnd.github+json, */*");
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);
#endif
        QNetworkReply *reply = manager.get(request);
        reply->setReadBufferSize(1024 * 1024);
        QEventLoop loop;
        QTimer timeout, poll;
        timeout.setSingleShot(true);
        timeout.setInterval(config.timeoutMs ? int(qMin<quint32>(config.timeoutMs, INT_MAX)) : 30000);
        poll.setInterval(50);
        qint64 received = 0;
        result.data.clear();
        auto consume = [&]() {
            if (result.status != Ok) return;
            QByteArray chunk = reply->readAll();
            if (chunk.isEmpty()) return;
            timeout.start();
            received += chunk.size();
            if (received > limit) {
                result.status = VerificationError; result.message = "The download exceeds the permitted size"; reply->abort(); return;
            }
            int http = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (http >= 300 && http < 400) return;
            if (output) {
                if (output->write(chunk) != chunk.size()) {
                    result.status = DatabaseError; result.message = "Cannot write the downloaded database"; reply->abort();
                }
            } else result.data += chunk;
        };
        QObject::connect(reply, &QNetworkReply::readyRead, consume);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        QObject::connect(&timeout, &QTimer::timeout, [&]() {
            result.status = NetworkError; result.message = "The update server timed out"; reply->abort();
        });
        QObject::connect(&poll, &QTimer::timeout, [&]() {
            if (stopped(cancelled)) { result.status = Cancelled; result.message = "Cancelled"; reply->abort(); }
        });
        timeout.start(); poll.start();
        loop.exec(); consume(); timeout.stop(); poll.stop();
        result.http = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        QNetworkReply::NetworkError error = reply->error();
        delete reply;
        if (result.status != Ok) return result;
        if (!redirect.isEmpty() && result.http >= 300 && result.http < 400) {
            QUrl next = url.resolved(redirect);
            if (url.scheme() == "https" && next.scheme() != "https") {
                result.status = NetworkError; result.message = "Refusing an insecure download redirect"; return result;
            }
            url = next;
            continue;
        }
        if (error != QNetworkReply::NoError || result.http < 200 || result.http >= 300) {
            result.status = NetworkError;
            result.message = result.http ? QString("The update server returned HTTP %1").arg(result.http) : QString("Cannot reach the update server");
        }
        return result;
    }
    result.status = NetworkError; result.message = "Too many download redirects"; return result;
}
Status checksum(const Config &config, Info &info, const QString &url, bool listed,
                const std::function<bool()> &cancelled)
{
    Response response = fetch(config, url, 1024 * 1024, cancelled);
    if (response.status == Cancelled) return fail(info, Cancelled, response.message);
    if (response.http == 404 || response.http == 403 || response.http == 401) return Ok;
    if (response.status != Ok) return fail(info, response.status, "Cannot read the checksum: " + response.message);
    info.remoteMd5 = parseMd5(response.data);
    if (listed && info.remoteMd5.isEmpty()) return fail(info, VerificationError, displayUrl(url) + " does not hold an MD5 checksum");
    return Ok;
}

struct ReleaseUrl {
    QString base, owner, repo, tag, asset;
    bool valid = false, latest = false;
};
ReleaseUrl releaseUrl(const QUrl &url)
{
    ReleaseUrl result;
    QStringList parts = url.path(QUrl::FullyEncoded).split('/');
    parts.removeAll(QString());
    if (parts.size() < 2) return result;
    for (QString &part : parts) part = QUrl::fromPercentEncoding(part.toUtf8());
    if (url.host().compare("github.com", Qt::CaseInsensitive) != 0 && (parts.size() < 3 || parts[2] != "releases")) return result;
    if (parts.size() > 2 && parts[2] != "releases") return result;
    result.owner = parts[0]; result.repo = parts[1];
    QUrl base = url;
    base.setPath(QString());
    base.setQuery(QString());
    base.setFragment(QString());
    result.base = base.toString(QUrl::FullyEncoded);
    if (parts.size() <= 3 || (parts.size() == 4 && parts[3] == "latest")) result.latest = true;
    else if (parts.size() == 5 && parts[3] == "tag") result.tag = parts[4];
    else if (parts.size() == 6 && parts[3] == "download") { result.tag = parts[4]; result.asset = parts[5]; }
    else return result;
    result.valid = true;
    return result;
}
QString encoded(const QString &value) { return QString::fromLatin1(QUrl::toPercentEncoding(value)); }
QString downloadUrl(const ReleaseUrl &release, const QString &tag, const QString &asset)
{
    return release.base + '/' + encoded(release.owner) + '/' + encoded(release.repo) + "/releases/download/" + encoded(tag) + '/' + encoded(asset);
}
Status querySource(const Config &config, Info &info, const std::function<bool()> &cancelled)
{
    QUrl url(info.sourceUrl);
    if (!url.isValid() || url.host().isEmpty() || (url.scheme() != "http" && url.scheme() != "https"))
        return fail(info, ArgumentError, "Not an http or https address: " + displayUrl(info.sourceUrl));
    ReleaseUrl release = releaseUrl(url);
    if (!release.valid) {
        info.archiveUrl = info.sourceUrl;
        info.archiveName = sanitize(QFileInfo(url.path()).fileName());
        if (info.archiveName.isEmpty()) info.archiveName = "db.zip";
        return (!url.hasQuery() && !url.hasFragment()) ? checksum(config, info, info.sourceUrl + ".md5", false, cancelled) : Ok;
    }
    info.archiveName = release.asset.isEmpty() ? "db.zip" : release.asset;
    QString api = config.apiBase;
    if (api.isEmpty()) api = url.host().compare("github.com", Qt::CaseInsensitive) == 0 ? "https://api.github.com" : release.base + "/api/v3";
    while (api.endsWith('/')) api.chop(1);
    QString endpoint = api + "/repos/" + encoded(release.owner) + '/' + encoded(release.repo) + "/releases/";
    endpoint += release.latest ? "latest" : "tags/" + encoded(release.tag);
    Response response = fetch(config, endpoint, 16 * 1024 * 1024, cancelled);
    if (response.status == Cancelled || response.status == ArgumentError || response.status == VerificationError)
        return fail(info, response.status, response.message);
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(response.data, &jsonError);
    bool valid = response.status == Ok && jsonError.error == QJsonParseError::NoError && document.isObject() && document.object().value("assets").isArray();
    if (valid) {
        QJsonObject object = document.object();
        info.releaseTag = sanitize(object.value("tag_name").toString());
        info.releaseName = sanitize(object.value("name").toString());
        QJsonObject archive, digest;
        for (const QJsonValue &value : object.value("assets").toArray()) {
            QJsonObject asset = value.toObject();
            if (asset.value("name").toString() == info.archiveName) archive = asset;
            if (asset.value("name").toString() == info.archiveName + ".md5") digest = asset;
        }
        if (archive.isEmpty()) return fail(info, NetworkError, QString("The release %1 has no %2").arg(info.releaseTag, sanitize(info.archiveName)));
        info.archiveUrl = archive.value("browser_download_url").toString();
        if (info.archiveUrl.isEmpty()) info.archiveUrl = downloadUrl(release, info.releaseTag, info.archiveName);
        info.remoteSize = qint64(archive.value("size").toDouble(-1));
        QDateTime uploaded = QDateTime::fromString(archive.value("updated_at").toString(), Qt::ISODate);
        info.remoteTime = uploaded.isValid() ? uploaded.toMSecsSinceEpoch() / 1000 : 0;
        info.remoteAsset = QString("%1:%2:%3").arg(qint64(archive.value("id").toDouble())).arg(info.remoteSize).arg(info.remoteTime);
        if (!digest.isEmpty()) {
            QString digestUrl = digest.value("browser_download_url").toString();
            if (digestUrl.isEmpty()) digestUrl = downloadUrl(release, info.releaseTag, info.archiveName + ".md5");
            return checksum(config, info, digestUrl, true, cancelled);
        }
        return Ok;
    }
    bool mayFallback = !release.latest && ((!release.asset.isEmpty() && response.http != 0) ||
                       response.http == 401 || response.http == 403 || response.http == 429 || response.http >= 500 || response.status == Ok);
    if (!mayFallback) return fail(info, NetworkError, response.message.isEmpty() ? "Cannot read the release" : response.message);
    info.releaseTag = sanitize(release.tag);
    info.archiveUrl = downloadUrl(release, release.tag, info.archiveName);
    return checksum(config, info, downloadUrl(release, release.tag, info.archiveName + ".md5"), false, cancelled);
}
Status prepare(const Config &config, Info &info)
{
    info = Info();
    info.sourceUrl = config.url;
    if (info.sourceUrl.isEmpty() || info.sourceUrl == "https://github.com/horsicq/Detect-It-Easy/releases/tag/db") info.sourceUrl = defaultUrl();
    info.mainIsDefault = config.mainPath.isEmpty();
    info.mainPath = info.mainIsDefault ? defaultDatabase() : absolute(config.mainPath);
    Status status = validateTarget(info);
    if (status != Ok) return status;
    info.hasDatabase = QFileInfo(info.mainPath).isDir();
    if (info.hasDatabase) readStamp(info);
    return Ok;
}
void decide(Info &info)
{
    if (!info.hasDatabase) info.updateAvailable = true;
    else if (!info.remoteMd5.isEmpty() && !info.installedMd5.isEmpty()) info.updateAvailable = info.remoteMd5 != info.installedMd5;
    else if (!info.remoteAsset.isEmpty() && !info.installedAsset.isEmpty()) info.updateAvailable = info.remoteAsset != info.installedAsset;
    else info.updateAvailable = true;
    info.message = info.updateAvailable ? "An update is available" : "The database is up to date";
}
} // namespace

QString defaultUrl() { return QStringLiteral("https://github.com/horsicq/Detect-It-Easy/releases/tag/current-database"); }
QString displayUrl(const QString &url)
{
    QUrl parsed(url);
    if (parsed.isValid()) return sanitize(parsed.toString(QUrl::RemoveUserInfo));
    QString value = url;
    value.replace(QRegularExpression("(://)[^/?#]*@"), "\\1");
    return sanitize(value);
}
QString statusName(Status status)
{
    static const char *const names[] = {"ok", "update-available", "network-error", "database-error", "invalid-argument", "verification-failed", "cancelled", "busy"};
    return status >= Ok && status <= Busy ? QString::fromLatin1(names[status]) : QStringLiteral("unknown");
}
Status loadSettings(Config &config, const QString &file, QString *error)
{
    QString path = file;
    if (!file.isEmpty() && !QFileInfo(file).isFile()) { if (error) *error = "Settings file not found: " + file; return ArgumentError; }
    if (path.isEmpty() && portable()) path = QDir(QCoreApplication::applicationDirPath()).filePath("die.ini");
    std::unique_ptr<QSettings> settings;
    if (!path.isEmpty()) settings.reset(new QSettings(path, QSettings::IniFormat));
    else {
#ifdef Q_OS_MAC
        settings.reset(new QSettings(QSettings::NativeFormat, QSettings::UserScope, "ntinfo.biz", "die"));
#else
        settings.reset(new QSettings(QSettings::NativeFormat, QSettings::UserScope, "NTInfo", "die"));
#endif
    }
    settings->setFallbacksEnabled(false);
    config.settingsLocation = QDir::toNativeSeparators(settings->fileName());
    QString main = settings->value(MainKey).toString();
    QString url = settings->value(UrlKey).toString();
    if (settings->status() == QSettings::AccessError) { if (error) *error = "Cannot read the settings in " + config.settingsLocation; return ArgumentError; }
    if (!path.isEmpty() && settings->status() == QSettings::FormatError) {
        QFile input(path);
        if (!input.open(QIODevice::ReadOnly)) { if (error) *error = "Cannot read the settings in " + path; return ArgumentError; }
        QString section;
        while (!input.atEnd()) {
            QString line = QString::fromUtf8(input.readLine()).trimmed();
            if (line.startsWith('[') && line.contains(']')) { section = line.mid(1, line.indexOf(']') - 1); if (section == "General") section.clear(); continue; }
            if (line.startsWith(';') || line.startsWith('#')) continue;
            int equals = line.indexOf('='); if (equals < 1) continue;
            QString key = (section.isEmpty() ? "" : section + '/') + line.left(equals).trimmed(); key.replace('\\', '/');
            QString value = line.mid(equals + 1).trimmed();
            if (value.size() >= 2 && value.startsWith('"') && value.endsWith('"')) value = value.mid(1, value.size() - 2);
            if (key == MainKey && main.isEmpty()) main = value;
            if (key == UrlKey && url.isEmpty()) url = value;
        }
    }
    if (!portable() && (main == "$data/db" || main == "$data\\db")) main.clear();
    if (config.mainPath.isEmpty() && !main.isEmpty()) {
        if (main == "$data" || main.startsWith("$data/") || main.startsWith("$data\\"))
            main = QDir(QCoreApplication::applicationDirPath()).filePath(main.size() > 5 ? main.mid(6) : QString());
        config.mainPath = main;
    }
    if (config.url.isEmpty()) config.url = url;
    return Ok;
}
Status check(const Config &config, Info &info, const std::function<bool()> &cancelled)
{
    Status status = prepare(config, info);
    if (status != Ok) return status;
    status = querySource(config, info, cancelled);
    if (status != Ok) return status;
    decide(info);
    return info.updateAvailable ? Available : Ok;
}

namespace {
struct UpdateLock {
    QString path;
#ifdef Q_OS_WIN
    HANDLE handle = INVALID_HANDLE_VALUE;
#else
    int fd = -1;
#endif
    Status acquire(const QString &parent)
    {
        path = QDir(parent).filePath(".die-update.lock");
#ifdef Q_OS_WIN
        handle = CreateFileW(reinterpret_cast<LPCWSTR>(path.utf16()), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                             OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_DELETE_ON_CLOSE, nullptr);
        if (handle != INVALID_HANDLE_VALUE) return Ok;
        DWORD error = GetLastError();
        return error == ERROR_SHARING_VIOLATION || (error == ERROR_ACCESS_DENIED && QFileInfo::exists(path)) ? Busy : DatabaseError;
#else
        for (int attempt = 0; attempt < 10; ++attempt) {
            fd = ::open(QFile::encodeName(path).constData(), O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0644);
            if (fd < 0) return DatabaseError;
            struct flock lock = {};
            lock.l_type = F_WRLCK; lock.l_whence = SEEK_SET;
            if (::fcntl(fd, F_SETLK, &lock)) {
                int error = errno; ::close(fd); fd = -1;
                return error == EACCES || error == EAGAIN ? Busy : DatabaseError;
            }
            struct stat opened, named;
            if (!::fstat(fd, &opened) && !::stat(QFile::encodeName(path).constData(), &named) &&
                opened.st_dev == named.st_dev && opened.st_ino == named.st_ino) return Ok;
            ::close(fd); fd = -1;
        }
        return Busy;
#endif
    }
    ~UpdateLock()
    {
#ifdef Q_OS_WIN
        if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
#else
        if (fd >= 0) { ::unlink(QFile::encodeName(path).constData()); ::close(fd); }
#endif
    }
};
bool renameFolder(const QString &from, const QString &to)
{
    for (int attempt = 0; attempt < 30; ++attempt) {
        if (QDir().rename(from, to)) return true;
#ifdef Q_OS_WIN
        QThread::msleep(100);
#else
        break;
#endif
    }
    return false;
}
void recoverStages(const QString &parent, const QString &keep)
{
    QFileInfoList stages = QDir(parent).entryInfoList(QStringList(StagePrefix + '*'), QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot);
    for (const QFileInfo &stage : stages) {
        if (stage.isSymLink() || stage.absoluteFilePath() == keep) continue;
        auto records = plainIni(QDir(stage.absoluteFilePath()).filePath("targets.ini"));
        bool retain = false;
        QFileInfoList oldFolders = QDir(stage.absoluteFilePath()).entryInfoList(QStringList("old-*"), QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
        for (const QFileInfo &old : oldFolders) {
            QString origin = records.value(old.fileName());
            // Only siblings can be targets of this updater's same-volume swap.
            if (old.isSymLink() || !old.isDir() || origin.isEmpty() || !QFileInfo(origin).isAbsolute() ||
                canonical(QFileInfo(origin).absolutePath()) != canonical(parent) || !protectedTarget(origin).isEmpty()) {
                retain = true; continue;
            }
            if (!QFileInfo::exists(origin)) {
                if (!renameFolder(old.absoluteFilePath(), origin)) retain = true;
            } else if (!looksLikeDatabase(origin)) retain = true;
        }
        if (!retain) QDir(stage.absoluteFilePath()).removeRecursively();
    }
}
bool defaultReadOnly(const QString &path)
{
    QString user = userDataPath();
    if (!user.isEmpty() && within(canonical(path), canonical(user))) return false;
    if (!qgetenv("DIE_READONLY_INSTALL").isEmpty() || !qgetenv("CDIE_READONLY_INSTALL").isEmpty() || !qgetenv("APPIMAGE").isEmpty()) return true;
#if defined(DIE_SYSTEM_INSTALL) && DIE_SYSTEM_INSTALL
    return true;
#elif defined(Q_OS_MAC)
    return path.contains(".app/Contents/");
#else
    return false;
#endif
}
bool useUserFolder(Info &info)
{
    QString user = userDataPath();
    if (user.isEmpty()) return false;
    info.mainPath = absolute(QDir(user).filePath("db"));
    info.usedUserFolder = true;
    return true;
}
void normalizeModes(const QString &path)
{
#ifndef Q_OS_WIN
    QFile::Permissions directoryMode = QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther;
    QFile::Permissions fileMode = QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther;
    QFile::setPermissions(path, directoryMode);
    QDirIterator iterator(path, QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        QFileInfo entry = iterator.fileInfo();
        if (!entry.isSymLink()) QFile::setPermissions(entry.filePath(), entry.isDir() ? directoryMode : fileMode);
    }
#else
    Q_UNUSED(path)
#endif
}
Status performInstall(const Config &config, Info &info, bool force, const std::function<bool()> &cancelled)
{
    if (info.remoteSize > MaxArchive) return fail(info, VerificationError, "The archive exceeds the permitted size");
    bool fallback = info.mainIsDefault && config.userFallback && !portable();
    if (fallback && defaultReadOnly(info.mainPath) && !useUserFolder(info))
        return fail(info, DatabaseError, "The installed database is read-only and there is no per-user folder for an update");
    // Destroy staging before releasing the lock, including on early returns.
    UpdateLock lock;
    std::unique_ptr<QTemporaryDir> stage;
    QString parent;
    for (;;) {
        Status targetStatus = validateTarget(info);
        if (targetStatus != Ok) return targetStatus;
        parent = QFileInfo(info.mainPath).absolutePath();
        QDir().mkpath(parent);
        stage.reset(new QTemporaryDir(QDir(parent).filePath(StagePrefix + "XXXXXX")));
        if (stage->isValid()) break;
        if (fallback && !info.usedUserFolder && useUserFolder(info)) continue;
        return fail(info, DatabaseError, "Cannot write to the database folder " + parent);
    }
    Status status = lock.acquire(parent);
    if (status != Ok) return fail(info, status, (status == Busy ? "Another database update is running in " : "Cannot lock ") + parent);
    recoverStages(parent, stage->path());
    if (stopped(cancelled)) return fail(info, Cancelled, "Cancelled");
    QString zip = QDir(stage->path()).filePath("archive.zip");
    QFile download(zip);
    if (!download.open(QIODevice::WriteOnly)) return fail(info, DatabaseError, "Cannot write the downloaded database");
    Response response = fetch(config, info.archiveUrl, MaxArchive, cancelled, &download);
    download.close();
    if (response.status != Ok) return fail(info, response.status, response.message);
    QFile archive(zip);
    if (!archive.open(QIODevice::ReadOnly)) return fail(info, DatabaseError, "Cannot read the downloaded database");
    if (info.remoteSize >= 0 && archive.size() != info.remoteSize) return fail(info, VerificationError, "The downloaded size does not match the release");
    QCryptographicHash hash(QCryptographicHash::Md5);
    while (!archive.atEnd()) {
        if (stopped(cancelled)) return fail(info, Cancelled, "Cancelled");
        QByteArray data = archive.read(1024 * 1024);
        if (data.isEmpty() && archive.error() != QFile::NoError) return fail(info, VerificationError, "Cannot read the downloaded database");
        hash.addData(data);
    }
    archive.close();
    QString md5 = QString::fromLatin1(hash.result().toHex());
    if (!info.remoteMd5.isEmpty() && info.remoteMd5 != md5) return fail(info, VerificationError, "MD5 mismatch: expected " + info.remoteMd5 + ", got " + md5);
    if (info.remoteMd5.isEmpty() && !force && md5 == info.installedMd5 && !info.usedUserFolder) {
        if (!info.remoteAsset.isEmpty() && info.remoteAsset != info.installedAsset) {
            QString now = QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddTHH:mm:ssZ");
            if (writeStamp(info, md5, now)) { info.installedAsset = info.remoteAsset; info.installedAt = now; }
        }
        info.updateAvailable = false;
        return fail(info, Ok, "The database is up to date");
    }
    QString extracted = QDir(stage->path()).filePath("x");
    QString error;
    DatabaseArchive::Result extractedStatus = DatabaseArchive::extractAndValidate(zip, extracted, &info.signatures, &error, cancelled);
    if (extractedStatus != DatabaseArchive::Success)
        return fail(info, extractedStatus == DatabaseArchive::Cancelled ? Cancelled : VerificationError, error);
    QString fresh = QDir(extracted).filePath("db");
    if (!QFileInfo(fresh).isDir() || info.signatures <= 0) return fail(info, VerificationError, "The archive does not contain a usable database");
    normalizeModes(fresh);
    if (stopped(cancelled)) return fail(info, Cancelled, "Cancelled");
    if (!writeFile(QDir(stage->path()).filePath("targets.ini"), ("old-db=" + info.mainPath + '\n').toUtf8()))
        return fail(info, DatabaseError, "Cannot record the database replacement");
    QString old = QDir(stage->path()).filePath("old-db");
    bool movedOld = QFileInfo::exists(info.mainPath);
    // No cancellation between these renames: complete the swap or restore it.
    if (movedOld && !renameFolder(info.mainPath, old)) return fail(info, DatabaseError, "Cannot replace " + info.mainPath + "; close programs that use it and try again");
    if (!renameFolder(fresh, info.mainPath)) {
        if (movedOld && !renameFolder(old, info.mainPath)) stage->setAutoRemove(false);
        return fail(info, DatabaseError, "Cannot move the new database to " + info.mainPath);
    }
    info.installed = true; info.hasDatabase = true; info.updateAvailable = false;
    info.installedMd5 = md5; info.installedAsset = info.remoteAsset;
    info.installedSource = info.sourceUrl;
    info.installedAt = QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddTHH:mm:ssZ");
    if (!writeStamp(info, md5, info.installedAt)) {
        info.installedMd5.clear();
        info.message = QString("Installed %1 signatures in %2, but could not write die-update.ini; the next check will offer the update again").arg(info.signatures).arg(info.mainPath);
    } else info.message = QString("Installed %1 signatures in %2%3").arg(info.signatures).arg(info.mainPath, info.usedUserFolder ? " (the default database folder is read-only)" : "");
    return Ok;
}
} // namespace

Status install(const Config &config, Info &info, bool force, const std::function<bool()> &cancelled)
{
    Status status = check(config, info, cancelled);
    if (status == Available || (status == Ok && force)) status = performInstall(config, info, force, cancelled);
    return status;
}

} // namespace DatabaseUpdater
