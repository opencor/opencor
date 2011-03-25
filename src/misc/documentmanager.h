#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QList>
#include <QString>

class Document
{
public:
    Document(const QString &pFileName);

    QString fileName();

private:
    QString mFileName;
};

class DocumentManager
{
    enum Status
    {
        DoesNotExist,
        Added,
        AlreadyManaged,
        Removed,
        NotManaged
    };

public:
    ~DocumentManager();

    Status manage(const QString &pFileName);
    void manage(const QStringList &pFileNames);
    Status unmanage(const QString &pFileName);
    void unmanage(const QStringList &pFileNames);

private:
    QList<Document *> mDocuments;

    Document *isManaged(const QString &pFileName);
};

#endif
