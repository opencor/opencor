#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "commonwidget.h"

#include <QWidget>

namespace Ui {
    class CentralWidget;
}

class TabWidget;

class CentralWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *pParent);
    ~CentralWidget();

    void openFile(const QString &pFileName);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

private:
    Ui::CentralWidget *mUi;

    TabWidget *mTabWidget;

Q_SIGNALS:
    void filesDropped(const QStringList &pFileNames);
};

#endif
