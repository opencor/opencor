#ifndef COMMANDVIEWERWIDGET_H
#define COMMANDVIEWERWIDGET_H

#include "commonwidget.h"

#include <QtMmlWidget>

class CommandViewerWidget : public QtMmlWidget, public CommonWidget
{
    Q_OBJECT

public:
    CommandViewerWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

protected:
    virtual QSize sizeHint() const;
};

#endif
