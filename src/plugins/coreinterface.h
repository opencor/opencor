#ifndef COREINTERFACE_H
#define COREINTERFACE_H

#include <QtPlugin>

#include <QList>

namespace OpenCOR {

class Plugin;

class CoreInterface
{
public:
    virtual void initialize();
    virtual void finalize();

    void setLoadedPlugins(const QList<Plugin *> &pLoadedPlugins);

protected:
    QList<Plugin *> mLoadedPlugins;
};

}

Q_DECLARE_INTERFACE(OpenCOR::CoreInterface, "OpenCOR.CoreInterface")

#endif
