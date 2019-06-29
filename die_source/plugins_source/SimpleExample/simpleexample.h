#ifndef SIMPLEEXAMPLE_H
#define SIMPLEEXAMPLE_H

#include <QObject>
#include <QPushButton>
#include "../plugininterface.h"


class SimpleExample : public QObject, PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    bool init(DIEPLUGIN *pDiePlugin);
    QString getName();

private slots:
    void process();
private:
    DIEPLUGIN *pDiePlugin;
    QPushButton *button;
};

#endif // SIMPLEEXAMPLE_H
