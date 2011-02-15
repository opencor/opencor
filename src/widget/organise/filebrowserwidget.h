#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "commonwidget.h"

#include <QWidget>

class FileBrowserWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);
};

#endif
