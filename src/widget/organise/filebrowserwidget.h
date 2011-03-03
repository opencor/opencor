#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "commonwidget.h"

#include <QTreeView>

class QFileSystemModel;

class FileBrowserWidget : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    FileBrowserWidget(QWidget *pParent = 0);
    ~FileBrowserWidget();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

    QString homeFolder();
    void gotoHomeFolder();

    QString path(const QModelIndex &pIndex);
    QString currentPath();
    bool currentPathVisible();

protected:
    virtual QSize sizeHint() const;

    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QFileSystemModel *mFileSystemModel;

    bool mNeedDefaultColumnWidth;

    QString mInitPathDir;
    QString mInitPath;

private Q_SLOTS:
    void directoryLoaded(const QString &pPath);
};

#endif
