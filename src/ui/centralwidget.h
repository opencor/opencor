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

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

    bool openFile(const QString &pFileName);
    QString closeFile(const int &pIndex = -1);

    bool activateFile(const QString &pFileName);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

private:
    Ui::CentralWidget *mUi;

    TabWidget *mTabWidget;

Q_SIGNALS:
    void filesDropped(const QStringList &pFileNames);
    void fileClosed(const QString &pFileName);

private Q_SLOTS:
    void closeTab(const int &pIndex);
};

#endif
