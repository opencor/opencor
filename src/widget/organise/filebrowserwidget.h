#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "commonwidget.h"

#include <QTreeView>

class QFileSystemModel;

class FileBrowserWidget : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWidget(const QString &pName, QWidget *pParent);
    ~FileBrowserWidget();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

    bool gotoPath(const QString &pPath, const bool &pExpand = false);

    QString homeFolder();
    void gotoHomeFolder(const bool &pExpand = false);

    QString currentPath();
    QString currentPathDir();
    QString currentPathParent();

protected:
    virtual QSize sizeHint() const;

    virtual bool viewportEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QFileSystemModel *mFileSystemModel;

    bool mNeedDefColWidth;

    QString mInitPathDir;
    QString mInitPath;

private Q_SLOTS:
    void directoryLoaded(const QString &pPath);
};

#endif
