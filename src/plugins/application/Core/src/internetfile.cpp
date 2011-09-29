#include "internetfile.h"

namespace OpenCOR {
namespace Core {

InternetFile::InternetFile(QObject *pParent) :
    mParent(pParent)
{
}

QNetworkReply * InternetFile::get(const QString &pPath)
{
    // Create a QNetworkAccessManager which will be used to retrieve our
    // Internet file

    QNetworkAccessManager * networkAccessManager = new QNetworkAccessManager(mParent);

    // Make sure that we get told when the download of our Internet file is
    // complete

    connect(networkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(finished(QNetworkReply *)) );

    // Get our Internet file

    mNetworkReply = networkAccessManager->get(QNetworkRequest(QUrl(pPath)));

    // Block until our request is complete

    mLoop.exec();

    // Return the request status

    return mNetworkReply;
}

void InternetFile::finished(QNetworkReply *pNetworkReply)
{
    // End the loop only if we have got the correct network reply

    if (pNetworkReply == mNetworkReply)
        mLoop.exit();
}

} }
