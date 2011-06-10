#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>

namespace OpenCOR {

class Plugin : public QObject
{
    Q_OBJECT

public:
    explicit Plugin();
    ~Plugin();
};

}

#endif
