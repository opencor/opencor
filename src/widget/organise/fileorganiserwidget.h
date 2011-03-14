#ifndef FILEORGANISERWIDGET_H
#define FILEORGANISERWIDGET_H

#include "commonwidget.h"

#include <QWidget>

class FileOrganiserWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    FileOrganiserWidget(const QString &pName, QWidget *pParent);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);
};

#endif
