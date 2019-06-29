#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtGui>
#include <QString>
#include <QWidget>

struct DIEPLUGIN
{
    QWidget *pWidget;
    QString sFileName;
    QString sPluginPath;
};

class PluginInterface
{
public:

    virtual bool init(DIEPLUGIN *pDiePlugin)=0;
    virtual QString getName()=0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginInterface,"die.plugininterface/1.0")
QT_END_NAMESPACE

#endif // PLUGININTERFACE_H
