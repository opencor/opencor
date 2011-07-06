#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "commonwidget.h"

#include <QWidget>

namespace Ui {
    class CentralWidget;
}

namespace OpenCOR {
namespace Core {

class FileManager;
class TabWidget;

class CentralWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *pParent);
    ~CentralWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings);

    bool openFile(const QString &pFileName);
    bool activateFile(const QString &pFileName);

    int nbOfFilesOpened();

    QString activeFileName();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

private:
    Ui::CentralWidget *mUi;

    FileManager *mFileManager;

    TabWidget *mTabWidget;

Q_SIGNALS:
    void fileOpened(const QString &pFileName);
    void fileClosed(const QString &pFileName);
    void fileActivated(const QString &pFileName);

public Q_SLOTS:
    void openFiles(const QStringList &pFileNames);
    bool closeFile(const int &pIndex = -1);
    void closeFiles();
    void fileActivated(const int &pIndex);
};

} }

#endif
