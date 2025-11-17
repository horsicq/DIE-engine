/* Copyright (c) 2025 hors<horsicq@gmail.com>
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

#include "desktopintegrationhelper.h"
#include "qjsonarray.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include <QDebug>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QProcess>
#include <QFileSystemWatcher>
#include <QtConcurrent>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <ShlObj.h>
#include <PropIdl.h>
#include <Shobjidl.h>
#endif

std::function<void(const QString&, qint64, qint64)> m_progressCallback;

DesktopIntegrationHelper::DesktopIntegrationHelper()
    : m_widget(nullptr),
    m_trayIcon(nullptr),
    m_running(false)
{
#ifdef Q_OS_WIN
    m_taskbarList = nullptr;
    m_comInitialized = false;
#endif
}

void DesktopIntegrationHelper::SetTrayIcon(QSystemTrayIcon* trayIcon) {
    GetInstance().m_trayIcon = trayIcon;
}

DesktopIntegrationHelper::~DesktopIntegrationHelper() {
    Uninitialize();
}

DesktopIntegrationHelper& DesktopIntegrationHelper::GetInstance() {
    static DesktopIntegrationHelper instance;
    return instance;
}

bool DesktopIntegrationHelper::Initialize(QWidget* widget) {
    return GetInstance().InitializeInternal(widget);
}

bool DesktopIntegrationHelper::InitializeInternal(QWidget* widget) {
    if (m_widget != nullptr) {
        return false;
    }
    if (!widget) {
        return false;
    }
    m_widget = widget;
#ifdef Q_OS_WIN
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr) && hr != S_FALSE) {
        m_comInitialized = false;
    } else {
        m_comInitialized = (hr == S_OK || hr == S_FALSE);
        hr = CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&m_taskbarList));
        if (SUCCEEDED(hr)) {
            hr = m_taskbarList->HrInit();
            if (FAILED(hr)) {
                m_taskbarList->Release();
                m_taskbarList = nullptr;
            }
        } else {
            m_taskbarList = nullptr;
        }
    }
#endif
#ifdef Q_OS_WIN
    return m_taskbarList != nullptr;
#else
    return true;
#endif
}

void DesktopIntegrationHelper::Uninitialize() {
    auto& instance = GetInstance();
#ifdef Q_OS_WIN
    if (instance.m_taskbarList) {
        if (instance.m_widget) {
            instance.m_taskbarList->SetProgressState((HWND)instance.m_widget->winId(), TBPF_NOPROGRESS);
            instance.m_taskbarList->SetOverlayIcon((HWND)instance.m_widget->winId(), nullptr, L"");
        }
        instance.m_taskbarList->Release();
        instance.m_taskbarList = nullptr;
    }
#endif
#ifdef Q_OS_WIN
    if (instance.m_comInitialized) {
        CoUninitialize();
        instance.m_comInitialized = false;
    }
#endif
    instance.m_widget = nullptr;
}

bool DesktopIntegrationHelper::IsAvailable() {
    auto& instance = GetInstance();
#ifdef Q_OS_WIN
    bool available = instance.m_taskbarList != nullptr || instance.m_trayIcon != nullptr;
#else
    bool available = instance.m_trayIcon != nullptr;
#endif
    return available;
}

#ifdef Q_OS_WIN
void DesktopIntegrationHelper::SetProgressValue(int value, int max) {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget) {
        instance.m_taskbarList->SetProgressValue((HWND)instance.m_widget->winId(), value, max);
    }
}

void DesktopIntegrationHelper::SetProgressState(TBPFLAG state) {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget) {
        instance.m_taskbarList->SetProgressState((HWND)instance.m_widget->winId(), state);
    }
}

void DesktopIntegrationHelper::SetOverlayIcon(HICON hIcon, LPCWSTR description) {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget) {
        instance.m_taskbarList->SetOverlayIcon((HWND)instance.m_widget->winId(), hIcon, description);
    }
}

void DesktopIntegrationHelper::ClearOverlayIcon() {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget) {
        instance.m_taskbarList->SetOverlayIcon((HWND)instance.m_widget->winId(), nullptr, L"");
    }
}

void DesktopIntegrationHelper::FlashTaskbar(bool flash, int count) {
    auto& instance = GetInstance();
    if (instance.m_widget) {
        FLASHWINFO flashInfo = { sizeof(FLASHWINFO) };
        flashInfo.hwnd = (HWND)instance.m_widget->winId();
        flashInfo.dwFlags = flash ? (FLASHW_TRAY | FLASHW_TIMERNOFG) : FLASHW_STOP;
        flashInfo.uCount = count;
        flashInfo.dwTimeout = 0;
        FlashWindowEx(&flashInfo);
    }
}

bool DesktopIntegrationHelper::AddThumbnailButtons(THUMBBUTTON* buttons, UINT count) {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget && count <= 7) {
        HRESULT hr = instance.m_taskbarList->ThumbBarAddButtons((HWND)instance.m_widget->winId(), count, buttons);
        return SUCCEEDED(hr);
    }
    return false;
}

bool DesktopIntegrationHelper::UpdateThumbnailButtons(THUMBBUTTON* buttons, UINT count) {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget && count <= 7) {
        HRESULT hr = instance.m_taskbarList->ThumbBarUpdateButtons((HWND)instance.m_widget->winId(), count, buttons);
        return SUCCEEDED(hr);
    }
    return false;
}

void DesktopIntegrationHelper::SetThumbnailTooltip(LPCWSTR tooltip) {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget) {
        instance.m_taskbarList->SetThumbnailTooltip((HWND)instance.m_widget->winId(), tooltip);
    }
}

void DesktopIntegrationHelper::SetThumbnailClip(RECT* rect) {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget) {
        instance.m_taskbarList->SetThumbnailClip((HWND)instance.m_widget->winId(), rect);
    }
}

void DesktopIntegrationHelper::ClearThumbnailClip() {
    auto& instance = GetInstance();
    if (instance.m_taskbarList && instance.m_widget) {
        instance.m_taskbarList->SetThumbnailClip((HWND)instance.m_widget->winId(), nullptr);
    }
}

bool DesktopIntegrationHelper::AddJumpListTasks(const std::vector<JumpListTask>& tasks) {
    auto& instance = GetInstance();
    ICustomDestinationList* pDestList = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_DestinationList, nullptr, CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&pDestList));
    if (FAILED(hr)) {
        return false;
    }
    UINT maxSlots;
    IObjectArray* pRemoved = nullptr;
    hr = pDestList->BeginList(&maxSlots, IID_PPV_ARGS(&pRemoved));
    if (SUCCEEDED(hr)) {
        if (pRemoved) pRemoved->Release();
    } else {
        pDestList->Release();
        return false;
    }
    IObjectCollection* pCollection = nullptr;
    hr = CoCreateInstance(CLSID_EnumerableObjectCollection, nullptr, CLSCTX_INPROC_SERVER,
                          IID_PPV_ARGS(&pCollection));
    if (FAILED(hr)) {
        pDestList->Release();
        return false;
    }
    for (const auto& task : tasks) {
        IShellLink* pLink = nullptr;
        hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&pLink));
        if (SUCCEEDED(hr)) {
            pLink->SetPath(task.exePath.c_str());
            pLink->SetArguments(task.arguments.c_str());
            pLink->SetDescription(task.description.c_str());
            pCollection->AddObject(pLink);
            pLink->Release();
        }
    }
    IObjectArray* pTaskArray = nullptr;
    hr = pCollection->QueryInterface(IID_PPV_ARGS(&pTaskArray));
    if (SUCCEEDED(hr)) {
        pDestList->AddUserTasks(pTaskArray);
        pTaskArray->Release();
    }
    pCollection->Release();
    hr = pDestList->CommitList();
    pDestList->Release();
    return SUCCEEDED(hr);
}
#endif

void DesktopIntegrationHelper::ShowToastNotification(const QString& message,
                                                     const QString& appId,
                                                     QSystemTrayIcon::MessageIcon icon,
                                                     int timeoutMs)
{
    auto& instance = GetInstance();
#ifdef Q_OS_LINUX
    QString command = QString("notify-send -t %1 \"%2\" \"%3\"")
                          .arg(timeoutMs).arg(appId, message);
    std::system(command.toUtf8().constData());
#else
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }
    if (!instance.m_trayIcon) {
        return;
    }
    if (!instance.m_trayIcon->isVisible()) {
        instance.m_trayIcon->setVisible(true);
    }
    instance.m_trayIcon->showMessage(appId, message, icon, timeoutMs);
#endif
}

void DesktopIntegrationHelper::addPath(const QString &path) {
    DesktopIntegrationHelper& instance = GetInstance();
    instance.addPathInternal(path);
}

void DesktopIntegrationHelper::addPathInternal(const QString &path) {
    if (!m_paths.contains(path)) {
        m_paths.append(path);
    }
}

void DesktopIntegrationHelper::stopMonitoring() {
    DesktopIntegrationHelper& instance = GetInstance();
    instance.stopMonitoringInternal();
}

void DesktopIntegrationHelper::setCallback(std::function<void(const QString &)> callback) {
    DesktopIntegrationHelper& instance = GetInstance();
    instance.setCallbackInternal(callback);
}

void DesktopIntegrationHelper::addCallbackInternal(std::function<void(const QString &)> callback) {
    m_callbacks.append(callback);
}

void DesktopIntegrationHelper::startMonitoring() {
    DesktopIntegrationHelper& instance = GetInstance();
    instance.startMonitoringInternal();
}

void DesktopIntegrationHelper::startMonitoringInternal() {
    m_running = true;
    for (const QString &path : m_paths) {
        std::thread([this, path]() {
            monitorPath(path);
        }).detach();
    }
}

void DesktopIntegrationHelper::stopMonitoringInternal() {
    m_running = false;
}

void DesktopIntegrationHelper::setCallbackInternal(std::function<void(const QString &)> callback) {
    m_callback = callback;
}

QString DesktopIntegrationHelper::normalizeFileName(const QString& fileName)
{
    QString result = fileName;
    result.remove(QRegularExpression("\\(\\d+\\)"));
    return result;
}

void DesktopIntegrationHelper::monitorPath(const QString& folder)
{
    QMetaObject::invokeMethod(qApp, [this, folder]() {
        QFileSystemWatcher* watcher = new QFileSystemWatcher(qApp);
        watcher->addPath(folder);
        QDir path(folder);
        for (const QFileInfo& info : path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
            m_knownFiles.insert(info.absoluteFilePath());
        }
        connect(watcher, &QFileSystemWatcher::directoryChanged, this, [this, folder]() {
            QDir path(folder);
            QFileInfoList currentFiles = path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
            QSet<QString> currentSet;
            for (const QFileInfo& info : currentFiles) {
                currentSet.insert(info.absoluteFilePath());
            }
            QSet<QString> newFiles = currentSet - m_knownFiles;
            QSet<QString> removedFiles = m_knownFiles - currentSet;
            m_knownFiles = currentSet;
            for (const QString& fullPath : newFiles) {
                QFileInfo info(fullPath);
                QString fileName = info.fileName();
                const bool isFinalFile = !fileName.endsWith(".crdownload") &&
                                         !fileName.endsWith(".part") &&
                                         !fileName.endsWith(".tmp") &&
                                         !fileName.endsWith(".download");
                if (isFinalFile) {
                    QDateTime now = QDateTime::currentDateTimeUtc();
                    QString normalized = normalizeFileName(fileName);
                    bool alreadyHandled = false;
                    QString possibleCrdownload = fullPath + ".crdownload";
                    if (m_activeDownloads.contains(possibleCrdownload)) {
                        m_activeDownloads.remove(possibleCrdownload);
                    } else {
                        for (auto it = m_recentFiles.begin(); it != m_recentFiles.end(); ++it) {
                            QString pastNormalized = normalizeFileName(QFileInfo(it.key()).fileName());
                            if (pastNormalized == normalized && it.value().secsTo(now) < 10) {
                                alreadyHandled = true;
                                break;
                            }
                        }
                    }
                    if (!alreadyHandled) {
                        std::thread([this, fullPath, fileName, now]() {
                            bool fileReady = false;
                            for (int attempt = 0; attempt < 50 && m_running; attempt++) {
                                QFileInfo check(fullPath);
                                if (!check.exists()) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                    continue;
                                }
                                qint64 fileSize = check.size();
                                if (fileSize == 0) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                    continue;
                                }
                                QFile testFile(fullPath);
                                if (!testFile.open(QIODevice::ReadOnly)) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                    continue;
                                }
                                QByteArray testRead = testFile.read(16);
                                testFile.close();
                                if (testRead.isEmpty()) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                    continue;
                                }
                                fileReady = true;
                                break;
                            }
                            if (fileReady) {
                                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                                QMetaObject::invokeMethod(qApp, [this, fullPath, now]() {
                                    m_recentFiles.insert(fullPath, now);
                                    if (m_callback) {
                                        m_callback(fullPath);
                                    }
                                }, Qt::QueuedConnection);
                            }
                        }).detach();
                    }
                } else {
                    m_activeDownloads.insert(fullPath);
                    std::thread([this, fullPath]() {
                        QFileInfo info(fullPath);
                        qint64 lastSize = 0;
                        qint64 expectedSize = -1;
                        qint64 totalSize = -1;
                        bool hasReliableTotal = false;
                        QUrl url = getUrlForCrdownloadFile(fullPath);
                        if (url.isValid()) {
                            expectedSize = getRemoteFileSize(url);
                            if (expectedSize > 0) {
                                totalSize = expectedSize;
                                hasReliableTotal = true;
                            }
                        }
                        for (int i = 0; i < 300 && m_running; ++i) {
                            QFile file(fullPath);
                            if (!file.exists()) {
                                break;
                            }
                            qint64 currentSize = file.size();
                            if (currentSize != lastSize && m_progressCallback) {
                                QDateTime now = QDateTime::currentDateTime();
                                qint64 speed = 0;
                                QMetaObject::invokeMethod(qApp, [this, fullPath, currentSize, totalSize, hasReliableTotal, speed]() {
                                    if (m_progressCallback) {
                                        m_progressCallback(fullPath, currentSize, hasReliableTotal ? totalSize : -1);
                                    }
                                }, Qt::QueuedConnection);
                                lastSize = currentSize;
                            }
                            if (i > 20 && currentSize == lastSize && currentSize > 0) {
                                std::this_thread::sleep_for(std::chrono::seconds(2));
                                if (!file.exists()) {
                                    break;
                                }
                            }
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        }
                        QMetaObject::invokeMethod(qApp, [this, fullPath]() {
                            m_activeDownloads.remove(fullPath);
                        }, Qt::QueuedConnection);
                    }).detach();
                }
            }
        });
    }, Qt::QueuedConnection);
}

QString DesktopIntegrationHelper::formatBytes(qint64 bytes) {
    constexpr qint64 KB = 1024;
    constexpr qint64 MB = 1024 * KB;
    constexpr qint64 GB = 1024 * MB;
    if (bytes < KB)
        return QString::number(bytes) + " B";
    else if (bytes < MB)
        return QString::number(bytes / double(KB), 'f', 2) + " KB";
    else if (bytes < GB)
        return QString::number(bytes / double(MB), 'f', 2) + " MB";
    else
        return QString::number(bytes / double(GB), 'f', 2) + " GB";
}

QString DesktopIntegrationHelper::formatSpeed(qint64 bytesPerSecond) {
    if (bytesPerSecond <= 0) return "";
    return formatBytes(bytesPerSecond) + "/s";
}

QString DesktopIntegrationHelper::formatDownloadProgress(const QString& filePath, qint64 received, qint64 total, qint64 speed) {
    QFileInfo info(filePath);
    QString fileName = info.fileName();
    QString receivedStr = formatBytes(received);
    QString progressText;
    if (total > 0) {
        QString totalStr = formatBytes(total);
        double percentage = (100.0 * received) / total;
        progressText = QString("%1 — %2 of %3").arg(fileName, receivedStr, totalStr);
        if (percentage >= 1.0 && percentage <= 100.0) {
            progressText += QString(" (%1%)").arg(QString::number(percentage, 'f', 1));
        }
    } else {
        progressText = QString("%1 — %2 downloaded").arg(fileName, receivedStr);
    }
    if (speed > 0) {
        QString speedStr = formatSpeed(speed);
        progressText += QString(" — %1").arg(speedStr);
    }
    return progressText;
}

QStringList DesktopIntegrationHelper::detectBrowserDownloadFolders() {
    QStringList paths;
    QString stdDownloads = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (!stdDownloads.isEmpty()) paths << QDir::cleanPath(stdDownloads);
    QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#ifdef Q_OS_WIN
    QString chromeUserData = home + "/AppData/Local/Google/Chrome/User Data/";
    QString firefoxProfiles = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                              + "/Mozilla/Firefox/Profiles/";
#elif defined(Q_OS_MAC)
    QString chromeUserData = home + "/Library/Application Support/Google/Chrome/";
    QString firefoxProfiles = home + "/Library/Application Support/Firefox/Profiles/";
#elif defined(Q_OS_LINUX)
    QString chromeUserData = home + "/.config/google-chrome/";
    if (!QDir(chromeUserData).exists())
        chromeUserData = home + "/.config/chromium/";
    QString firefoxProfiles = home + "/.mozilla/firefox/";
#endif
    QDir chromeDir(chromeUserData);
    for (const QString& profile : chromeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString prefsPath = chromeDir.filePath(profile + "/Preferences");
        QFile prefsFile(prefsPath);
        if (prefsFile.exists() && prefsFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(prefsFile.readAll());
            prefsFile.close();
            QString chromePath = doc["download"]["default_directory"].toString();
            if (!chromePath.isEmpty()) paths << QDir::cleanPath(chromePath);
        }
    }
    QDir dir(firefoxProfiles);
    for (const QString& profile : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString prefs = dir.filePath(profile + "/prefs.js");
        QFile f(prefs);
        if (f.open(QIODevice::ReadOnly)) {
            while (!f.atEnd()) {
                QByteArray line = f.readLine();
                if (line.contains("browser.download.dir")) {
                    int start = line.indexOf("\"");
                    int end = line.lastIndexOf("\"");
                    if (start > 0 && end > start) {
                        QString ffPath = QString::fromUtf8(line.mid(start + 1, end - start - 1));
                        paths << QDir::cleanPath(ffPath);
                    }
                    break;
                }
            }
            f.close();
        }
    }
    return paths.removeDuplicates(), paths;
}

QUrl DesktopIntegrationHelper::getUrlForCrdownloadFile(const QString& fullPath)
{
    QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString chromeUserData = home + "/AppData/Local/Google/Chrome/User Data/";
    QDir chromeDir(chromeUserData);
    for (const QString& profile : chromeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString prefsPath = chromeDir.filePath(profile + "/Preferences");
        QFile prefsFile(prefsPath);
        if (!prefsFile.exists() || !prefsFile.open(QIODevice::ReadOnly)) continue;
        QJsonDocument doc = QJsonDocument::fromJson(prefsFile.readAll());
        prefsFile.close();
        QJsonObject root = doc.object();
        QJsonObject downloads = root.value("download").toObject();
        QJsonArray items = downloads.value("download_items").toArray();
        for (const QJsonValue& item : items) {
            QJsonObject obj = item.toObject();
            QString targetPath = obj.value("target_path").toString();
            QString url = obj.value("url").toString();
            if (QDir::cleanPath(targetPath) == QDir::cleanPath(fullPath)) {
                return QUrl(url);
            }
        }
    }
    return QUrl();
}

qint64 DesktopIntegrationHelper::getRemoteFileSize(const QUrl& url)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply* reply = manager.head(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QVariant lengthHeader = reply->header(QNetworkRequest::ContentLengthHeader);
    reply->deleteLater();
    return lengthHeader.isValid() ? lengthHeader.toLongLong() : -1;
}

void DesktopIntegrationHelper::setProgressCallback(std::function<void(const QString&, qint64, qint64)> callback) {
    DesktopIntegrationHelper& instance = GetInstance();
    instance.setProgressCallbackInternal(callback);
}

void DesktopIntegrationHelper::setProgressCallbackInternal(std::function<void(const QString&, qint64, qint64)> callback)
{
    m_progressCallback = callback;
}
