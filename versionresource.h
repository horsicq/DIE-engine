#ifndef VERSIONRESOURCE_H
#define VERSIONRESOURCE_H

#include <QByteArray>
#include <QObject>
#include "_macros.h"
#if  defined(_WIN32)
#include <windows.h>
#else
#include "__windows.h"
#endif

struct _VS_VERSION_INFO
{
    unsigned short wLength;             /* Length of the version resource */
    unsigned short wValueLength;        /* Length of the value field for this block */
    unsigned short wType;
};

typedef struct __tagVS_FIXEDFILEINFO
{
    unsigned int dwSignature;        // e.g.  0xfeef04bd
    unsigned int dwStrucVersion;     // e.g.  0x00000042 = "0.42"
    unsigned int dwFileVersionMS;    // e.g.  0x00030075 = "3.75"
    unsigned int dwFileVersionLS;    // e.g.  0x00000031 = "0.31"
    unsigned int dwProductVersionMS; // e.g.  0x00030010 = "3.10"
    unsigned int dwProductVersionLS; // e.g.  0x00000031 = "0.31"
    unsigned int dwFileFlagsMask;    // = 0x3F for version "0.42"
    unsigned int dwFileFlags;        // e.g.  VFF_DEBUG | VFF_PRERELEASE
    unsigned int dwFileOS;           // e.g.  VOS_DOS_WINDOWS16
    unsigned int dwFileType;         // e.g.  VFT_DRIVER
    unsigned int dwFileSubtype;      // e.g.  VFT2_DRV_KEYBOARD
    unsigned int dwFileDateMS;       // e.g.  0
    unsigned int dwFileDateLS;       // e.g.  0
} __VS_FIXEDFILEINFO;

#define VS_FF_DEBUG 0x00000001
#define VS_FF_INFOINFERRED 0x00000010
#define VS_FF_PATCHED 0x00000004
#define VS_FF_PRERELEASE 0x00000002
#define VS_FF_PRIVATEBUILD 0x00000008
#define VS_FF_SPECIALBUILD 0x00000020

#define VOS_DOS 0x00010000
#define VOS_NT 0x00040000
#define VOS__WINDOWS16 0x00010000
#define VOS__WINDOWS32 0x00000001
#define VOS_OS216 0x00020000
#define VOS_OS232 0x00030000
#define VOS__PM16 0x00000002
#define VOS__PM32 0x00000003
#define VOS_UNKNOWN 0x00000000

#define VFT_APP 0x00000001
#define VFT_DLL 0x00000002
#define VFT_DRV 0x00000003
#define VFT_FONT 0x00000004
#define VFT_STATIC_LIB 0x00000007
#define VFT_UNKNOWN 0x00000000
#define VFT_VXD 0x00000005


class VersionResource : public QObject
{
    Q_OBJECT
public:
    explicit VersionResource(QByteArray *pbaData);
    void setData(QByteArray *pbaData);
    bool isValid();

    unsigned int getLength(unsigned int nOffset);
    unsigned int getValueLength(unsigned int nOffset);
    unsigned int getType(unsigned int nOffset);
    QString getKey(unsigned int nOffset);

    QString versionToString(unsigned int nValue);
    QString fileFlagsToString(unsigned int nFileFlags);
    QString fileOSToString(unsigned int nFileOS);
    QString fileTypeToString(unsigned int nFileType);

    unsigned int getNextHeaderOffset(unsigned int nOffset);

    unsigned int getFixedFileInfo_Signature();
    unsigned int getFixedFileInfo_StrucVersion();
    unsigned int getFixedFileInfo_FileVersionMS();
    unsigned int getFixedFileInfo_FileVersionLS();
    unsigned int getFixedFileInfo_ProductVersionMS();
    unsigned int getFixedFileInfo_ProductVersionLS();
    unsigned int getFixedFileInfo_FileFlagsMask();
    unsigned int getFixedFileInfo_FileFlags();
    unsigned int getFixedFileInfo_FileOS();
    unsigned int getFixedFileInfo_FileType();
    unsigned int getFixedFileInfo_FileSubtype();
    unsigned int getFixedFileInfo_FileDateMS();
    unsigned int getFixedFileInfo_FileDateLS();

    bool isChildrenPresent();

    unsigned int getChildrenOffset();

    QString getValue(unsigned int nOffset);

    unsigned int getDword(unsigned int nOffset);

    QString getFileVersion();
    QString getStringFileInfo(QString sKey);
signals:
    void appendError(QString sString);
public slots:
private:
    QByteArray *pbaData;
};

#endif // VERSIONRESOURCE_H
