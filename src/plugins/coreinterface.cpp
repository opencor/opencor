#include "coreinterface.h"

namespace OpenCOR {

void CoreInterface::initialize()
{
    // Nothing to do by default...
}

void CoreInterface::finalize()
{
    // Nothing to do by default...
}

void CoreInterface::setParameters(const QList<Plugin *> &pLoadedPlugins)
{
    // Set the loaded plugins

    mLoadedPlugins = pLoadedPlugins;
}

}
