#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

namespace OpenCOR {

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager();
    ~PluginManager();
};

}

#endif
