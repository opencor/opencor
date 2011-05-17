#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QObject>

namespace OpenCOR {

class UpdateManager : public QObject
{
    Q_OBJECT

public:
    explicit UpdateManager();
    ~UpdateManager();
};

}

#endif
