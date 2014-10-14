#ifndef SCRIPTMACH_H
#define SCRIPTMACH_H

#include "scriptgeneric.h"
//#include "scriptbinary.h"
#include "../machfile.h"
//#include "dialogoptions.h"

class scriptMACH : public scriptGeneric
{
    Q_OBJECT
public:
    explicit scriptMACH(QObject *parent = 0);
    void setData(MACHFile *machfile);

public slots:

    bool compareEP(QString sSignature,unsigned int nOffset=0);
//    bool compareEPn(QString sSignature,unsigned int nOffset);

    QString getString(unsigned int nOffset,unsigned int nSize=50);
    unsigned int readDword(unsigned int nOffset);
    unsigned short readWord(unsigned int nOffset);
    unsigned char readByte(unsigned int nOffset);
    unsigned int getSize();
//    bool compareHeader(QString sSignature);
    bool compare(QString sSignature,unsigned int nOffset=0);
    QString getFileDirectory();
    QString getFileBaseName();
    QString getFileSuffix();
    QString getFileCompleteSuffix();
    int findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue);
    int findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue);
    int findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue);
    int findString(unsigned int nOffset, unsigned int nSize, QString sValue);
    int findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature);
    bool isSectionNamePresent(QString sSectionName);
    bool isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature);
    bool compareReg(QString sSignature,unsigned int nOffset);
    bool isLibraryPresent(QString sLibrary);
    unsigned int getLibraryCurrentVersion(QString sLibrary);
    int getSectionNumber(QString sSectionName);

    QString getGeneralOptions();
    int getNumberOfSections();
    int getNumberOfSegments();

    float calculateEntropy(unsigned int nOffset, unsigned int nSize);
    QString getSignature(unsigned int nOffset,unsigned int nSize);
    QString calculateMD5(unsigned int nOffset, unsigned int nSize);

    unsigned int getSectionFileOffset(int nSection);
    unsigned int getSectionFileSize(int nSection);

    QString getSectionName(int nSection);

    QString getSegmentName(int nSegment);
    unsigned int getSegmentFileSize(int nSegment);
    unsigned int getSegmentFileOffset(int nSegment);

    long long RVAToOffset(unsigned long long nRVA);
    long long VAToOffset(unsigned long long nVA);
    long long OffsetToVA(unsigned long long nOffset);
    long long OffsetToRVA(unsigned long long nOffset);

    unsigned int getOverlaySize();
    unsigned int getOverlayOffset();
    bool isOverlayPresent();

    bool compareOverlay(QString sSignature,unsigned int nOffset=0);
    long long getEntryPointOffset();
private:
    MACHFile *machfile;
};

#endif // SCRIPTMACH_H
