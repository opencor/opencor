#ifndef MMLVIEWERWIDGET_H
#define MMLVIEWERWIDGET_H

#include "commonwidget.h"

#include <QtMmlWidget>

class MmlViewerWidget : public QtMmlWidget, public CommonWidget
{
    Q_OBJECT

public:
    MmlViewerWidget(QWidget *pParent = 0);
    ~MmlViewerWidget();

    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

    bool setContent(const QString &pContent, QString *pErrorMsg = 0,
                    int *pErrorLine = 0, int *pErrorColumn = 0);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    QtMmlWidget *testMmlWidget;
};

#endif
