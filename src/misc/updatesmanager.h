#ifndef UPDATESMANAGER_H
#define UPDATESMANAGER_H

#include <QObject>

namespace OpenCOR {

class UpdatesManager : public QObject
{
    Q_OBJECT

public:
    explicit UpdatesManager();
    ~UpdatesManager();
};

}

#endif
