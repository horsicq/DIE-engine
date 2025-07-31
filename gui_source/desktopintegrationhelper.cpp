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
#include <QDebug>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QProcess>
#include <QFileSystemWatcher>

#ifdef WIN32
#include <Windows.h>
#include <ShlObj.h>
#include <PropIdl.h>
#include <Shobjidl.h>
#endif

DesktopIntegrationHelper::DesktopIntegrationHelper()
    : m_widget(nullptr),
    m_trayIcon(nullptr),
    m_running(false)
{
#ifdef WIN32
    m_taskbarList = nullptr;
    m_comInitialized = false;
#endif
}

void DesktopIntegrationHelper::SetTrayIcon(QSystemTrayIcon* trayIcon) {
    qDebug() << "[Helper] Tray icon injected.";
    GetInstance().m_trayIcon = trayIcon;
}


// Implementation
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
    qDebug() << "Initializing DesktopIntegrationHelper";
    if (m_widget != nullptr) {
        qDebug() << "DesktopIntegrationHelper already initialized";
        return false;
    }
    if (!widget) {
        qDebug() << "Invalid widget provided for initialization";
        return false;
    }
    m_widget = widget;

#ifdef WIN32
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr) && hr != S_FALSE) {
        qDebug() << "COM initialization failed with HRESULT: " << hr;
        m_comInitialized = false;
    } else {
        m_comInitialized = (hr == S_OK || hr == S_FALSE);
        qDebug() << "COM initialized successfully";

        hr = CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&m_taskbarList));
        if (SUCCEEDED(hr)) {
            hr = m_taskbarList->HrInit();
            if (FAILED(hr)) {
                qDebug() << "ITaskbarList3 initialization failed with HRESULT: " << hr;
                m_taskbarList->Release();
                m_taskbarList = nullptr;
            } else {
                qDebug() << "ITaskbarList3 initialized successfully";
            }
        } else {
            qDebug() << "Failed to create ITaskbarList3 with HRESULT: " << hr;
            m_taskbarList = nullptr;
        }
    }
#endif

#ifdef WIN32
    return m_taskbarList != nullptr;
#else
    return true;
#endif
}

void DesktopIntegrationHelper::Uninitialize() {
    auto& instance = GetInstance();
#ifdef WIN32
    if (instance.m_taskbarList) {
        if (instance.m_widget) {
            instance.m_taskbarList->SetProgressState((HWND)instance.m_widget->winId(), TBPF_NOPROGRESS);
            instance.m_taskbarList->SetOverlayIcon((HWND)instance.m_widget->winId(), nullptr, L"");
        }
        instance.m_taskbarList->Release();
        instance.m_taskbarList = nullptr;
    }
#endif

#ifdef WIN32
    if (instance.m_comInitialized) {
        CoUninitialize();
        instance.m_comInitialized = false;
    }
#endif
    instance.m_widget = nullptr;
}

bool DesktopIntegrationHelper::IsAvailable() {
    auto& instance = GetInstance();
#ifdef WIN32
    bool available = instance.m_taskbarList != nullptr || instance.m_trayIcon != nullptr;
#else
    bool available = instance.m_trayIcon != nullptr;
#endif
    if (!available) {
        qDebug() << "DesktopIntegrationHelper not available";
    }
    return available;
}

#ifdef WIN32
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
        } else {
            qDebug() << "Failed to create IShellLink for JumpList task: " << hr;
        }
    }

    IObjectArray* pTaskArray = nullptr;
    hr = pCollection->QueryInterface(IID_PPV_ARGS(&pTaskArray));
    if (SUCCEEDED(hr)) {
        hr = pDestList->AddUserTasks(pTaskArray);
        if (FAILED(hr)) {
        }
        pTaskArray->Release();
    } else {
    }
    pCollection->Release();

    hr = pDestList->CommitList();
    if (FAILED(hr)) {
    }
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
    int result = std::system(command.toUtf8().constData());
    if (result != 0) {
    }
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
                    for (auto it = m_recentFiles.begin(); it != m_recentFiles.end(); ++it) {
                        QString pastNormalized = normalizeFileName(QFileInfo(it.key()).fileName());
                        if (pastNormalized == normalized && it.value().secsTo(now) < 10) {
                            alreadyHandled = true;
                            break;
                        }
                    }

                    if (!alreadyHandled) {
                        m_recentFiles.insert(fullPath, now);
                        qDebug() << "[Download complete] Debounced file:" << fileName;
                        if (m_callback)
                            m_callback(fullPath);
                    } else {
                        qDebug() << "[Suppressed variant] Too similar to recent file:" << fileName;
                    }
                } else {
                    qDebug() << "[Download in progress] Incomplete temp file:" << fileName;
                }
            }
        });
    }, Qt::QueuedConnection);
}

QStringList DesktopIntegrationHelper::detectBrowserDownloadFolders() {
    QStringList paths;

    QString stdDownloads = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (!stdDownloads.isEmpty()) paths << stdDownloads;

    QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

#ifdef Q_OS_WIN

    QString chromePref = home + "/AppData/Local/Google/Chrome/User Data/Default/Preferences";

    QString firefoxProfiles = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                              + "/Mozilla/Firefox/Profiles/";

#elif defined(Q_OS_MAC)

    QString chromePref = home + "/Library/Application Support/Google/Chrome/Default/Preferences";

    QString firefoxProfiles = home + "/Library/Application Support/Firefox/Profiles/";

#elif defined(Q_OS_LINUX)

    QString chromePref = home + "/.config/google-chrome/Default/Preferences";
    if (!QFile::exists(chromePref))
        chromePref = home + "/.config/chromium/Default/Preferences";

    QString firefoxProfiles = home + "/.mozilla/firefox/";
#endif

    QFile chromeFile(chromePref);
    if (chromeFile.exists() && chromeFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(chromeFile.readAll());
        chromeFile.close();

        QString chromePath = doc["download"]["default_directory"].toString();
        if (!chromePath.isEmpty()) paths << QDir::cleanPath(chromePath);
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
