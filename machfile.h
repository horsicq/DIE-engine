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
#ifndef MACHFILE_H
#define MACHFILE_H

#include "binary.h"

struct load_command_offset
{
    uint32_t cmd;		/* type of load command */
    uint32_t cmdsize;	/* total size of command in bytes */
    uint32_t offset;
};

struct DYLIB_FULL
{
    uint32_t timestamp;
    uint32_t current_version;
    uint32_t compatibility_version;
    QString sVollName;
    QString sShortName;
};

class MACHFile : public Binary
{
    Q_OBJECT
public:
    explicit MACHFile(QObject *parent = 0);
signals:

private slots:

public slots:
    bool isValid();
    bool isMACH64();
    bool isReverse();
    void entryPointLoad();

    unsigned int getHeader_magic();
    unsigned int getHeader_cputype();
    unsigned int getHeader_cpusubtype();
    unsigned int getHeader_filetype();
    unsigned int getHeader_ncmds();
    unsigned int getHeader_sizeofcmds();
    unsigned int getHeader_flags();
    unsigned int getHeader_reserved();

    void setHeader_magic(unsigned int nValue);
    void setHeader_cputype(unsigned int nValue);
    void setHeader_cpusubtype(unsigned int nValue);
    void setHeader_filetype(unsigned int nValue);
    void setHeader_ncmds(unsigned int nValue);
    void setHeader_sizeofcmds(unsigned int nValue);
    void setHeader_flags(unsigned int nValue);
    void setHeader_reserved(unsigned int nValue);

    unsigned int getMachHeaderOffset();
    unsigned int getMachHeaderSize();

    unsigned int getLoadCommand_type(unsigned int nLoadCommand);
    unsigned int getLoadCommand_size(unsigned int nLoadCommand);
    unsigned int getLoadCommand_offset(unsigned int nLoadCommand);
    bool isLoadCommandPresent(unsigned int nLoadCommand);
    bool isSegmentPresent(unsigned int nSegment);
    bool isSectionPresent(unsigned int nSection);

    static QString loadCommandTypeToString(unsigned int nType);


    unsigned int getNumberOfSegments();
    unsigned int getSegmentHeaderOffset(unsigned int nSegment);
    unsigned int getSegmentHeaderSize();

    QString getSegment_segname32(unsigned int nSegment);
    unsigned int getSegment_vmaddr32(unsigned int nSegment);
    unsigned int getSegment_vmsize32(unsigned int nSegment);
    unsigned int getSegment_fileoff32(unsigned int nSegment);
    unsigned int getSegment_filesize32(unsigned int nSegment);
    unsigned int getSegment_maxprot32(unsigned int nSegment);
    unsigned int getSegment_initprot32(unsigned int nSegment);
    unsigned int getSegment_nsects32(unsigned int nSegment);
    unsigned int getSegment_flags32(unsigned int nSegment);

    QString getSegment_segname64(unsigned int nSegment);
    unsigned long long getSegment_vmaddr64(unsigned int nSegment);
    unsigned long long getSegment_vmsize64(unsigned int nSegment);
    unsigned long long getSegment_fileoff64(unsigned int nSegment);
    unsigned long long getSegment_filesize64(unsigned int nSegment);
    unsigned int getSegment_maxprot64(unsigned int nSegment);
    unsigned int getSegment_initprot64(unsigned int nSegment);
    unsigned int getSegment_nsects64(unsigned int nSegment);
    unsigned int getSegment_flags64(unsigned int nSegment);

    unsigned int getSegment_nsects(unsigned int nSegment);

    unsigned int getNumberOfSections();
    unsigned int getSectionHeaderOffset(unsigned int nSection);
    unsigned int getSectionHeaderSize();


    QString getSection_sectname32(unsigned int nSection);
    QString getSection_segname32(unsigned int nSection);
    unsigned int getSection_addr32(unsigned int nSection);
    unsigned int getSection_size32(unsigned int nSection);
    unsigned int getSection_offset32(unsigned int nSection);
    unsigned int getSection_align32(unsigned int nSection);
    unsigned int getSection_reloff32(unsigned int nSection);
    unsigned int getSection_nreloc32(unsigned int nSection);
    unsigned int getSection_flags32(unsigned int nSection);

    QString getSection_sectname64(unsigned int nSection);
    QString getSection_segname64(unsigned int nSection);
    unsigned long long getSection_addr64(unsigned int nSection);
    unsigned long long getSection_size64(unsigned int nSection);
    unsigned int getSection_offset64(unsigned int nSection);
    unsigned int getSection_align64(unsigned int nSection);
    unsigned int getSection_reloff64(unsigned int nSection);
    unsigned int getSection_nreloc64(unsigned int nSection);
    unsigned int getSection_flags64(unsigned int nSection);

    unsigned int RVAToOffset32(unsigned int nRVA);
    unsigned int OffsetToRVA32(unsigned int nOffset);
    unsigned long long OffsetToRVA64(unsigned long long nOffset);
    unsigned long long RVAToOffset64(unsigned long long nRVA);

    virtual unsigned long long OffsetToRVA(unsigned long long nOffset);
    virtual unsigned long long RVAToOffset(unsigned long long nRVA);

    unsigned int VAToOffset32(unsigned int nVA);
    unsigned long long VAToOffset64(unsigned long long nVA);

    virtual unsigned long long VAToOffset(unsigned long long nVA);

    unsigned int OffsetToVA32(unsigned int nOffset);
    unsigned long long OffsetToVA64(unsigned long long nOffset);
    virtual unsigned long long OffsetToVA(unsigned long long nOffset);

    unsigned long long getEntryPoint();

    QList<load_command> getLoadCommands();
    QList<load_command_offset> getLoadCommands_offset();

    QList<segment_command> getSegmentsList32();
    QList<segment_command_64> getSegmentsList64();

    QList<section> getSectionsList32();
    QList<section_64> getSectionsList64();

    bool dumpSegment(QString sFileName,int nSegment);
    bool dumpSection(QString sFileName,int nSection);

    bool isSectionNamePresent(QString sSectionName);
    unsigned int getSectionNumber(QString sSectionName);

    bool isUUIDPresent();
    QByteArray getUUID();
    QString getUUIDAsString();
    bool setUUID(QByteArray baUUID);
    bool setUUIDFromString(QString baUUID);

    QList<DYLIB_FULL> getLibs();
    bool isLibraryPresent(QString sLibrary);
    unsigned int getLibraryCurrentVersion(QString sLibrary);

    unsigned int getSectionFileOffset(int nSection);
    unsigned int getSectionFileSize(int nSection);
    QString getSectionName(int nSection);

    QString getSegmentName(int nSegment);
    unsigned int getSegmentFileSize(int nSegment);
    unsigned int getSegmentFileOffset(int nSegment);

    bool compareEP(QString sSignature,unsigned int nOffset=0);
    unsigned long long getEntryPointOffset();

    unsigned int calculateRawSize();

    unsigned int getOverlaySize();
    unsigned int getOverlayOffset();
    bool isOverlayPresent();

    bool compareOverlay(QString sSignature,unsigned int nOffset=0);
};



#endif // MACHFILE_H
