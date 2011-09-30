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

    virtual void setup(const QList<Plugin *> &);
};

}

Q_DECLARE_INTERFACE(OpenCOR::CoreInterface, "OpenCOR.CoreInterface")

#endif
