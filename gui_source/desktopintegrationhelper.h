#pragma once

#include <QSystemTrayIcon>
#include <QWidget>
#include <QPointer>
#include <QDebug>
#include <QIcon>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QFileSystemWatcher>
#include <QCoreApplication>

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

    static bool Initialize(QWidget* widget);
    static void Uninitialize();
    static bool IsAvailable();
	static QStringList detectBrowserDownloadFolders();
	QString normalizeFileName(const QString& fileName);
    void monitorPath(const QString &path);


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
    bool InitializeInternal(QWidget* widget);

    QWidget* m_widget = nullptr;
#ifdef _WIN32
    ITaskbarList3* m_taskbarList = nullptr;
    bool m_comInitialized = false;
#endif
    std::function<void(const QString &)> m_callback;
    QStringList m_paths;
    QMap<QString, QDateTime> m_recentFiles;
    QSet<QString> m_knownFiles;
    QPointer<QSystemTrayIcon> m_trayIcon;
};