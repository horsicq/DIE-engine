// Copyright (c) 2012-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <QString>
#include <QList>
#include <QMutex>
#ifdef USE_NFD
#include "staticscan.h"
#include "staticscanitemmodel.h"
#endif
#ifdef USE_YARA
#include "qyara.h"
#endif

#define __DIE "Detect It Easy"
#define __VERSION "2.02"
#define __BUILDDATE __DATE__
#define __UPDATEFILE "http://ntinfo.biz/files/die_version"
#define __HOMEPAGE "http://ntinfo.biz/index.php/detect-it-easy"
#define __TOOLTIPRELAY 1000

#define DIE_SHOWERRORS              0x00000001
#define DIE_SHOWOPTIONS             0x00000002
#define DIE_SHOWVERSION             0x00000004
#define DIE_SHOWENTROPY             0x00000008
#define DIE_SINGLELINEOUTPUT        0x00000010
#define DIE_SHOWFILEFORMATONCE      0x00000020
#define DIE_FULLSCAN                0x00000040
#define DIE_DEEPSCAN                0x00000080

struct __SIGNATURE
{
    QString sName;
    QString sText;
};

enum SCAN_METHODS
{
    SM_DIE=0,
#ifdef USE_NFD
    SM_NFD,
#endif
#ifdef USE_YARA
    SM_YARA,
#endif
#ifdef USE_PEID
    SM_PEID
#endif
};

struct __DIE_RESULT
{
    QString sSignature;
    QString sFileType;
    QString sType;
    QString sName;
};

struct __DIE_OPTIONS
{
    int nDialogOpen;
    bool bStayOnTop;
    bool bScanAfterOpen;
    bool bSaveLastDirectory;
    bool bShowTooltips;
    bool bScanShowVersionDIE;
    bool bScanShowOptionsDIE;
    bool bScanDeepScanDIE;
    bool bScanResizeToContentsDIE;
#ifdef USE_NFD
    bool bScanDeepScanNFD;
    bool bScanScanOverlayNFD;
#endif
    bool bShowErrors;
    bool bShowScanTime;
    bool bScanSubfolders;
    bool bResizeableMainWindow;
    bool bShowEntropy;
    bool bSingleLineOutput;
    bool bShowFileFormatOnce;
    bool bFullScan;
    bool bSectionsEntropyAfterOpen;
    bool bSingleApplication;
    double fThreshold;
    unsigned int nEntropyGraph;
    QString sLastDirectory;
    QString sPluginName;
    QList<__SIGNATURE> listPEScripts;
    QList<__SIGNATURE> listBinaryScripts;
    QList<__SIGNATURE> listTextScripts;
    QList<__SIGNATURE> listELFScripts;
    QList<__SIGNATURE> listMSDOSScripts;
    QList<__SIGNATURE> listMACHScripts;
    QString sStyle;
    QString sStyleSheet;
    bool bEnableLanguage;
    QString sLanguage;
    bool bEnableFont;
    QString sFont;
    QString sFontHEX;
    unsigned int nAddressWidth;
    QString sEndianness;
    bool bEnablePlugins;
    QString sCodec;
//    int nNumberOfResults;
    QString sDataBasePath;
    QString sEditorPath;
    QString sHelpPath;
    QString sInfoPath;
    QString sPluginsPath;
    QString sQSSPath;
    QString sLangsPath;
    QString sScriptsPath;
    QString sSearchPath;
#ifdef USE_YARA
    QString sDataBaseYARAPath;
    QString sDataBaseYARA;
#endif
    SCAN_METHODS sm;
    QMutex *pMutexResult;
    QList<__DIE_RESULT> die_listResult;
#ifdef USE_NFD
    SpecAbstract::SCAN_RESULT nfd_result;
#endif
    bool bDieLoaded;
#ifdef USE_YARA
    bool bYaraLoaded;
    QYara::RESULT yara_result;
#endif
};

#define __StayOnTop "General/StayOnTop"
#define __SingleApplication "General/SingleApplication"
#define __ScanAfterOpen "Scan/ScanAfterOpen"
#define __ScanShowVersionDIE "Scan/ShowVersionDIE"
#define __ScanShowOptionsDIE "Scan/ShowOptionsDIE"
#define __ScanDeepScanDIE "Scan/ScanDeepDIE"
#define __ScanResizeColumnsToContent "Scan/ResizeColumnsToContent"
#ifdef USE_NFD
#define __ScanDeepScanNFD "Scan/ScanDeepNFD"
#define __ScanScanOverlayNFD "Scan/ScanOverlayNFD"
#endif
#define __ScanMethod "Scan/Method"
#define __ShowErrors "Scan/ShowErrors"
#define __ShowScanTime "Scan/ShowScanTime"
#define __SectionsEntropyAfterOpen "Entropy/SectionsEntropyAfterOpen"
#define __EntropyGraph "Entropy/EntropyGraph"
#define __Threshold "Entropy/Threshold"
#define __ShowTooltips "General/ShowTooltips"
#define __SaveLastDirectory "General/SaveLastDirectory"
#define __ResizeableMainWindow "Appearance/ResizeableMainWindow"
#define __SaveLastDirectory "General/SaveLastDirectory"
#define __LastDirectory "General/LastDirectory"
#define __Style "Appearance/Style"
#define __StyleSheet "Appearance/StyleSheet"
#define __Codec "Appearance/Codec"
#define __EnableFont "Appearance/FontEnable"
#define __Font "Appearance/Font"
#define __EnableLanguage "Appearance/EnableLanguage"
#define __Language "Appearance/Language"
#define __FontHEX "HEX/Font"
#define __AddressWidthHEX "HEX/AddressWidth"
#define __Endianness "HEX/Endianness"
#define __EnablePlugins "Plugins/EnablePlugins"

#define __DataBasePath "Paths/DataBase"
#define __EditorPath "Paths/Editor"
#define __HelpPath "Paths/Help"
#define __InfoPath "Paths/Info"
#define __PluginsPath "Paths/Plugins"
#define __QSSPath "Paths/QSS"
#define __LangsPath "Paths/Langs"
#define __ScriptsPath "Paths/Scripts"
#define __SearchPath "Paths/Search"
#ifdef USE_YARA
#define __DataBaseYARAPath "Paths/DataBaseYARA"
#endif

#define __DefaultDataBasePath "$app/db"
#define __DefaultEditorPath "$app/editor"
#define __DefaultHelpPath "$app/help"
#define __DefaultInfoPath "$app/info"
#define __DefaultPluginsPath "$app/die_plugins"
#define __DefaultQSSPath "$app/qss"
#define __DefaultLangsPath "$app/lang"
#define __DefaultScriptsPath "$app/scripts"
#define __DefaultSearchPath "$app/search"
#ifdef USE_YARA
#define __DefaultDataBaseYARAPath "$app/yara/packer.yar"
#endif

// KeySequences
#define __KeySequence_tab1 "Alt+1"
#define __KeySequence_tab2 "Alt+2"
#define __KeySequence_tab3 "Alt+3"
#define __KeySequence_tab4 "Alt+4"
#define __KeySequence_tab5 "Alt+5"
#define __KeySequence_tab6 "Alt+6"
#define __KeySequence_tab7 "Alt+7"
#define __KeySequence_tab8 "Alt+8"
#define __KeySequence_tab9 "Alt+9"
#define __KeySequence_altU "Alt+U"
#define __KeySequence_altA "Alt+A"
#define __KeySequence_altB "Alt+B"
#define __KeySequence_altE "Alt+E"
#define __KeySequence_altI "Alt+I"
#define __KeySequence_altF "Alt+F"
#define __KeySequence_altR "Alt+R"
#define __KeySequence_altS "Alt+S"
#define __KeySequence_altP "Alt+P"
#define __KeySequence_altH "Alt+H"
#define __KeySequence_altV "Alt+V"
#define __KeySequence_altM "Alt+M"
#define __KeySequence_altO "Alt+O"
#define __KeySequence_altD "Alt+D"
#define __KeySequence_altN "Alt+N"
#define __KeySequence_ctrlA "Ctrl+A"
#define __KeySequence_ctrlD "Ctrl+D"
#define __KeySequence_ctrlR "Ctrl+R"
#define __KeySequence_ctrlS "Ctrl+S"
#define __KeySequence_ctrlF "Ctrl+F"
#define __KeySequence_ctrlG "Ctrl+G"
#define __KeySequence_ctrlI "Ctrl+I"
#define __KeySequence_ctrlL "Ctrl+L"
#define __KeySequence_ctrlN "Ctrl+N"
#define __KeySequence_ctrlE "Ctrl+E"
#define __KeySequence_ctrlW "Ctrl+W"

#define __KeySequence_ctrlaltG "Ctrl+Alt+G"
#define __KeySequence_ctrlaltE "Ctrl+Alt+E"
#define __KeySequence_ctrlaltO "Ctrl+Alt+O"
#define __KeySequence_ctrlaltS "Ctrl+Alt+S"
#define __KeySequence_ctrlaltU "Ctrl+Alt+U"
#define __KeySequence_ctrlaltL "Ctrl+Alt+L"
#define __KeySequence_ctrlaltI "Ctrl+Alt+I"

#define __KeySequence_save "Ctrl+S"
#define __KeySequence_open "Ctrl+O"
#define __KeySequence_saveandquit "Ctrl+Alt+S"
#define __KeySequence_hex "Alt+H"
#define __KeySequence_quit "Alt+X"

#endif // _OPTIONS_H
