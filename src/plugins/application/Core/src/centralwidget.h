#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "commonwidget.h"
#include "guiinterface.h"

#include <QMap>
#include <QWidget>

namespace Ui {
    class CentralWidget;
}

class QTabBar;

namespace OpenCOR {
namespace Core {

class FileManager;
class TabWidget;

class CentralViewWidget
{
public:
    explicit CentralViewWidget(Plugin *pPlugin,
                               const GuiViewSettings &pSettings);

    Plugin * plugin() const;
    GuiViewSettings settings() const;

private:
    Plugin *mPlugin;
    GuiViewSettings mSettings;
};

class CentralWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *pParent = 0);
    ~CentralWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool openFile(const QString &pFileName);
    bool activateFile(const QString &pFileName);

    int nbOfFilesOpened() const;

    QString activeFileName() const;

    bool isModeEnabled(const GuiViewSettings::Mode &pMode) const;

    void addView(Plugin *pPlugin, const GuiViewSettings &pSettings);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

private:
    Ui::CentralWidget *mUi;

    FileManager *mFileManager;

    QTabBar *mModes;
    QMap<int, GuiViewSettings::Mode> mTabsMode;

    TabWidget *mFiles;

    QMap<GuiViewSettings::Mode, QTabBar *> mViews;

    QList<GuiViewSettings::Mode> mRequiredModes;

    QList<CentralViewWidget> mSupportedViews;

    void updateGui() const;

    void newViews(const GuiViewSettings::Mode &pMode);

Q_SIGNALS:
    void fileOpened(const QString &pFileName);
    void fileClosed(const QString &pFileName);
    void fileSelected(const QString &pFileName);

public Q_SLOTS:
    void openFiles(const QStringList &pFileNames);
    bool closeFile(const int &pIndex = -1);
    void closeFiles();
    void fileSelected(const int &pIndex);
    void modeSelected(const int &);
};

} }

#endif
