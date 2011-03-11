#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QList>
#include <QString>

class Document
{
public:
    Document(const QString &pFileName);

private:
    QString mFileName;
};

class DocumentManager
{
public:
    DocumentManager();

private:
    QList<Document *> mDocuments;
};

#endif
