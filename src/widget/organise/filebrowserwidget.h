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

    virtual void retranslateUi();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

protected:
    virtual QSize sizeHint() const;

private:
    QFileSystemModel *mFileSystemModel;
};

#endif
