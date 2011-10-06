#ifndef QWTPLUGIN_H
#define QWTPLUGIN_H

#include "plugininfo.h"

namespace OpenCOR {
namespace Qwt {

PLUGININFO_FUNC QwtPluginInfo();

class QwtPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
