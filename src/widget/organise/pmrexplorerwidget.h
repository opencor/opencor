#ifndef PMREXPLORERWIDGET_H
#define PMREXPLORERWIDGET_H

#include "commonwidget.h"

#include <QWebView>

class PmrExplorerWidget : public QWebView, public CommonWidget
{
    Q_OBJECT

public:
    PmrExplorerWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

protected:
    virtual QSize sizeHint() const;
};

#endif
