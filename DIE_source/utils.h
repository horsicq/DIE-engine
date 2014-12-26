#ifndef UTILS_H
#define UTILS_H

#include "_options.h"
#include <QDir>
#include <QCoreApplication>

class Utils
{
public:
    Utils();

    static QString getApplicationPath();
    static QString getPath(QString sFileName);
    static QString getLastDirecory(__DIE_OPTIONS *pOptions);

    static QString convertPath(QString sPath);

    static QString getDataBasePath(__DIE_OPTIONS *pOptions);
    static QString getEditorPath(__DIE_OPTIONS *pOptions);
    static QString getHelpPath(__DIE_OPTIONS *pOptions);
    static QString getInfoPath(__DIE_OPTIONS *pOptions);
    static QString getPluginsPath(__DIE_OPTIONS *pOptions);
    static QString getQSSPath(__DIE_OPTIONS *pOptions);
    static QString getLangsPath(__DIE_OPTIONS *pOptions);
    static QString getScriptsPath(__DIE_OPTIONS *pOptions);
    static QString getSearchPath(__DIE_OPTIONS *pOptions);

    static QString getDefaultFont();
};

#endif // UTILS_H
