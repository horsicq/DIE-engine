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

#ifdef _WIN32
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

    // Static interface
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

#ifdef _WIN32
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

    // Instance methods called by static interface
    bool InitializeInternal(QWidget* widget);
    void UninitializeInternal();
    bool IsAvailableInternal() const;
    void startMonitoringInternal();
    void stopMonitoringInternal();
    void addPathInternal(const QString &path);
    void setCallbackInternal(std::function<void(const QString &)> callback);
    void ShowToastNotificationInternal(const QString& message, const QString& appId, QSystemTrayIcon::MessageIcon icon, int timeoutMs);

#ifdef _WIN32
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
#ifdef _WIN32
    ITaskbarList3* m_taskbarList = nullptr;
    bool m_comInitialized = false;
#endif
    QSystemTrayIcon* m_trayIcon = nullptr;
    QStringList m_paths;
    QMap<QString, QDateTime> m_recentFiles;
    QSet<QString> m_knownFiles;
    std::function<void(const QString &)> m_callback;
    bool m_running;
};
