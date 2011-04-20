#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QList>
#include <QObject>

class QString;
class QTimer;

class Document
{
public:
    enum DocumentStatus
    {
        Changed,
        Unchanged,
        Deleted
    };

    explicit Document(const QString &pFileName);

    QString fileName() const;

    DocumentStatus check();

private:
    QString mFileName;
    QString mSha1;

    QString sha1() const;
};

class DocumentManager : public QObject
{
    Q_OBJECT

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

    explicit DocumentManager(const int &pTimerInterval = 1000);
    ~DocumentManager();

    ManageStatus manage(const QString &pFileName);
    UnmanageStatus unmanage(const QString &pFileName);

    int count() const;

private:
    QTimer *mTimer;
    QList<Document *> mDocuments;

    Document * isManaged(const QString &pFileName);

Q_SIGNALS:
    void fileContentsChanged(const QString &pFileName);
    void fileDeleted(const QString &pFileName);

private Q_SLOTS:
    void check();
};

#endif
