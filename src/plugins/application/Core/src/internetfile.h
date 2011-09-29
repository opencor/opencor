#ifndef INTERNETFILE_H
#define INTERNETFILE_H

#include "coreglobal.h"

#include <QEventLoop>
#include <QNetworkReply>

namespace OpenCOR {
namespace Core {

class CORE_EXPORT InternetFile : public QObject
{
    Q_OBJECT

public:
    explicit InternetFile(QObject *pParent = 0);

    QNetworkReply * get(const QString &pUrl);

private:
    QObject *mParent;

    QNetworkReply *mNetworkReply;

    QEventLoop mLoop;

private slots:
    void finished(QNetworkReply *pNetworkReply);
};

} }

#endif
