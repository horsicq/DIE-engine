#pragma once

#include <QSystemTrayIcon>
#include <QWidget>
#include <QPointer>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QThread>
#include <functional>
#include <thread>
#include <chrono>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <propkey.h>
#include <propvarutil.h>
#endif


class DesktopIntegrationHelper : public QObject{
public:
    struct JumpListTask {
        std::wstring exePath;
        std::wstring arguments;
        std::wstring description;
    };

    static bool Initialize(QWidget* widget);
    static void Uninitialize();
    static bool IsAvailable();
    static void startMonitoring();
    static void stopMonitoring();
    static void addPath(const QString &path);
    static void setCallback(std::function<void(const QString &)> callback);
    static QStringList detectBrowserDownloadFolders();
    static void SetTrayIcon(QSystemTrayIcon* trayIcon);
    QString normalizeFileName(const QString& fileName);
    static void setProgressCallback(std::function<void(const QString&, qint64, qint64)> callback);
    QUrl getUrlForCrdownloadFile(const QString& fullPath);
    static qint64 getRemoteFileSize(const QUrl& url);
    static QString formatBytes(qint64 bytes);
    static QString formatSpeed(qint64 bytesPerSecond);
    static QString formatDownloadProgress(const QString& filePath, qint64 received, qint64 total, qint64 speed = 0);

#ifdef Q_OS_WIN
    static void SetProgressValue(int value, int max);
    static void SetProgressState(TBPFLAG state);
    static void SetOverlayIcon(HICON hIcon, LPCWSTR description);
    static void ClearOverlayIcon();
    static void FlashTaskbar(bool flash, int count);
    static bool AddThumbnailButtons(THUMBBUTTON* buttons, UINT count);
    static bool UpdateThumbnailButtons(THUMBBUTTON* buttons, UINT count);
    static void SetThumbnailTooltip(LPCWSTR tooltip);
    static void SetThumbnailClip(RECT* rect);
    static void ClearThumbnailClip();
    static bool AddJumpListTasks(const std::vector<JumpListTask>& tasks);
#endif

    static void ShowToastNotification(const QString& message, const QString& appId, QSystemTrayIcon::MessageIcon icon, int timeoutMs);

private:
    DesktopIntegrationHelper();
    ~DesktopIntegrationHelper();
    DesktopIntegrationHelper(const DesktopIntegrationHelper&) = delete;
    DesktopIntegrationHelper& operator=(const DesktopIntegrationHelper&) = delete;
    static DesktopIntegrationHelper& GetInstance();
    void setProgressCallbackInternal(std::function<void(const QString&, qint64, qint64)> callback);
    QList<std::function<void(const QString &)>> m_callbacks;
    bool InitializeInternal(QWidget* widget);
    void UninitializeInternal();
    bool IsAvailableInternal() const;
    void startMonitoringInternal();
    void stopMonitoringInternal();
    void addPathInternal(const QString &path);
    void setCallbackInternal(std::function<void(const QString &)> callback);
    void ShowToastNotificationInternal(const QString& message, const QString& appId, QSystemTrayIcon::MessageIcon icon, int timeoutMs);
    void addCallbackInternal(std::function<void(const QString &)> callback);
#ifdef Q_OS_WIN
    void SetProgressValueInternal(int value, int max);
    void SetProgressStateInternal(TBPFLAG state);
    void SetOverlayIconInternal(HICON hIcon, LPCWSTR description);
    void ClearOverlayIconInternal();
    void FlashTaskbarInternal(bool flash, int count);
    bool AddThumbnailButtonsInternal(THUMBBUTTON* buttons, UINT count);
    bool UpdateThumbnailButtonsInternal(THUMBBUTTON* buttons, UINT count);
    void SetThumbnailTooltipInternal(LPCWSTR tooltip);
    void SetThumbnailClipInternal(RECT* rect);
    void ClearThumbnailClipInternal();
    bool AddJumpListTasksInternal(const std::vector<JumpListTask>& tasks);


#endif

    void monitorPath(const QString &path);

    // Members
    QWidget* m_widget = nullptr;
#ifdef Q_OS_WIN
    ITaskbarList3* m_taskbarList = nullptr;
    bool m_comInitialized = false;
#endif
    QSet<QString> m_activeDownloads;
    QSystemTrayIcon* m_trayIcon = nullptr;
    QStringList m_paths;
    QMap<QString, QDateTime> m_recentFiles;
    QSet<QString> m_knownFiles;
    std::function<void(const QString &)> m_callback;
    bool m_running;
};
