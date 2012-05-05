//==============================================================================
// Central widget
//==============================================================================

#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "guiinterface.h"

//==============================================================================

#include <QMap>
#include <QTabBar>
#include <QWidget>

//==============================================================================

namespace Ui {
    class CentralWidget;
}

//==============================================================================

class QLabel;
class QStackedWidget;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

namespace Core {

//==============================================================================

typedef QMap<int, QString> CentralWidgetViewNames;

typedef QMap<int, GuiInterface *> CentralWidgetViewInterfaces;
typedef QMap<int, GuiViewSettings *> CentralWidgetViewSettings;

//==============================================================================

class CentralWidgetMode
{
public:
    explicit CentralWidgetMode(CentralWidget *pOwner);
    ~CentralWidgetMode();

    bool isEnabled() const;
    void setEnabled(const bool &pEnabled);

    QTabBar * views() const;

    CentralWidgetViewNames * viewNames() const;

    CentralWidgetViewInterfaces * viewInterfaces() const;
    CentralWidgetViewSettings * viewSettings() const;

private:
    bool mEnabled;

    QTabBar *mViews;

    CentralWidgetViewNames * mViewNames;

    CentralWidgetViewInterfaces * mViewInterfaces;
    CentralWidgetViewSettings * mViewSettings;
};

//==============================================================================

class CentralWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *pParent);
    ~CentralWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void loadingOfSettingsDone(const Plugins &);

    bool openFile(const QString &pFileName);
    bool activateFile(const QString &pFileName);

    int openedFilesCount() const;

    QString activeFileName() const;

    bool isModeEnabled(const GuiViewSettings::Mode &pMode) const;

    void addView(Plugin *pPlugin, GuiViewSettings *pSettings);

    QTabBar * newTabBar(const QTabBar::Shape &pShape,
                        const bool &pMovable = false,
                        const bool &pTabsClosable = false);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

private:
    enum Status {
        Starting,
        Running,
        Stopping
    };

    Ui::CentralWidget *mGui;

    Status mStatus;

    QTabBar *mModeTabs;
    QTabBar *mFileTabs;

    QStackedWidget *mContents;

    QWidget *mLogoView;

    QWidget *mNoView;
    QLabel *mNoViewMsg;

    QMap<GuiViewSettings::Mode, CentralWidgetMode *> mModes;

    int modeTabIndex(const GuiViewSettings::Mode &pMode) const;

    void addMode(const GuiViewSettings::Mode &pMode);

    void loadModeSettings(QSettings *pSettings,
                          const GuiViewSettings::Mode &pCurrentMode,
                          const GuiViewSettings::Mode &pMode);
    void saveModeSettings(QSettings *pSettings,
                          const GuiViewSettings::Mode &pMode) const;

    void addModeView(Plugin *pPlugin, GuiViewSettings *pSettings,
                     const GuiViewSettings::Mode &pMode);

    void updateModeGui(const GuiViewSettings::Mode &pMode,
                       GuiInterface * &pGuiInterface, int &pModeViewIndex);

    QString modeViewName(const GuiViewSettings::Mode &pMode);

    void updateNoViewMsg();

Q_SIGNALS:
    void fileOpened(const QString &pFileName);
    void fileClosed(const QString &pFileName);
    void fileSelected(const QString &pFileName);

    void navigateFilesEnabled(const bool &pEnabled);
    void closeFilesEnabled(const bool &pEnabled);

public Q_SLOTS:
    void openFiles(const QStringList &pFileNames);

private Q_SLOTS:
    void previousFile();
    void nextFile();
    bool closeFile(const int &pIndex = -1);
    void closeAllFiles();
    void fileSelected(const int &pIndex);
    void updateGui();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
