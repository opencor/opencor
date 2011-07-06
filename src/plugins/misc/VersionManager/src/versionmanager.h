#ifndef VERSIONMANAGER_H
#define VERSIONMANAGER_H

#include <QObject>

namespace OpenCOR {
namespace VersionManager {

class VersionManager : public QObject
{
    Q_OBJECT

public:
    explicit VersionManager();
    ~VersionManager();
};

} }

#endif
