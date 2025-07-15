#pragma once

#include <QSystemTrayIcon>
#include <QWidget>

#ifdef _WIN32
#include <windows.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <propkey.h>
#include <propvarutil.h>
#endif

class DesktopIntegrationHelper {
public:
    struct JumpListTask {
        std::wstring exePath;
        std::wstring arguments;
        std::wstring description;
    };

    static bool Initialize(QWidget* widget);
    static void Uninitialize();
    static bool IsAvailable();

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
    QSystemTrayIcon* m_trayIcon = nullptr;
};

#ifdef _WIN32
#include <windows.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <propkey.h>
#include <propvarutil.h>
#endif
