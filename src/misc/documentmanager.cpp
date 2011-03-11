#include "documentmanager.h"

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

DocumentManager::Status DocumentManager::manage(const QString &pFileName)
{
    if (isManaged(pFileName)) {
        // The document is already managed, so...

        return AlreadyManaged;
    } else {
        // The document isn't already managed, so add it to our list of managed
        // documents

        mDocuments.append(new Document(pFileName));

        return Added;
    }
}

DocumentManager::Status DocumentManager::unmanage(const QString &pFileName)
{
    Document *document = isManaged(pFileName);

    if (document) {
        // The document is managed, so we can remove it

        mDocuments.removeAt(mDocuments.indexOf(document));

        return Removed;
    } else {
        // The document isn't managed, so...

        return NotManaged;
    }

    return Removed;
}

Document *DocumentManager::isManaged(const QString &pFileName)
{
    foreach (Document *document, mDocuments)
        if (document->fileName() == pFileName)
            // The document has been found meaning it is managed

            return document;

    // The document couldn't be found meaning it's not managed

    return 0;
}
