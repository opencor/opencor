#ifndef FILEORGANISERWIDGET_H
#define FILEORGANISERWIDGET_H

#include "commonwidget.h"

#include <QWidget>

class FileOrganiserWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);
};

#endif
