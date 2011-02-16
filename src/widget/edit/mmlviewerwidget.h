#ifndef MMLVIEWERWIDGET_H
#define MMLVIEWERWIDGET_H

#include "commonwidget.h"

#include <QtMmlWidget>

class MmlViewerWidget : public QtMmlWidget, public CommonWidget
{
    Q_OBJECT

public:
    MmlViewerWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

protected:
    virtual QSize sizeHint() const;
};

#endif
