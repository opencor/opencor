#include "documentmanager.h"

#include <QFileInfo>

Document::Document(const QString &pFileName) :
    mFileName(pFileName)
{
}

QString Document::fileName()
{
    // Return the file name of the document

    return mFileName;
}

DocumentManager::~DocumentManager()
{
    // Remove all the managed documents

    foreach (Document *document, mDocuments)
        delete document;
}

DocumentManager::ManageStatus DocumentManager::manage(const QString &pFileName)
{
    QFileInfo fileInfo = pFileName;

    if (fileInfo.exists()) {
        if (isManaged(fileInfo.canonicalFilePath())) {
            // The document is already managed, so...

            return AlreadyManaged;
        } else {
            // The document isn't already managed, so add it to our list of
            // managed documents

            mDocuments.append(new Document(fileInfo.canonicalFilePath()));

            return Added;
        }
    } else {
        // The document doesn't exist, so...

        return DoesNotExist;
    }
}

DocumentManager::UnmanageStatus DocumentManager::unmanage(const QString &pFileName)
{
    QFileInfo fileInfo = pFileName;

    if (fileInfo.exists()) {
        Document *document = isManaged(fileInfo.canonicalFilePath());

        if (document) {
            // The document is managed, so we can remove it

            mDocuments.removeAt(mDocuments.indexOf(document));

            return Removed;
        } else {
            // The document isn't managed, so...

            return NotManaged;
        }
    } else {
        // The document doesn't exist, so...

        return NotManaged;
    }
}

Document * DocumentManager::isManaged(const QString &pFileName)
{
    foreach (Document *document, mDocuments)
        if (document->fileName() == pFileName)
            // The document has been found meaning it is managed

            return document;

    // The document couldn't be found meaning it's not managed

    return 0;
}

int DocumentManager::count() const
{
    // Return the number of documents currently being managed

    return mDocuments.count();
}
