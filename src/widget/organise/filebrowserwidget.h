#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "commonwidget.h"

#include <QWidget>

class FileBrowserWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    FileBrowserWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);
};

#endif
