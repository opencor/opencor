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
public:
    enum ManageStatus
    {
        DoesNotExist,
        Added,
        AlreadyManaged
    };

    enum UnmanageStatus
    {
        Removed,
        NotManaged
    };

    ~DocumentManager();

    ManageStatus manage(const QString &pFileName);
    UnmanageStatus unmanage(const QString &pFileName);

private:
    QList<Document *> mDocuments;

    Document *isManaged(const QString &pFileName);
};

#endif
