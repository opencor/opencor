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

class LogoWidget;

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
    void setIsEnabled(const bool &pIsEnabled);

    QTabBar * views() const;

    CentralWidgetViewNames * viewNames() const;

    CentralWidgetViewInterfaces * viewInterfaces() const;
    CentralWidgetViewSettings * viewSettings() const;

private:
    bool mIsEnabled;

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

    Ui::CentralWidget *mUi;

    Status mStatus;

    QTabBar *mModeTabs;
    QTabBar *mFileTabs;

    QStackedWidget *mContents;

    LogoWidget *mLogoView;

    QWidget *mNoView;
    QLabel *mNoViewMsg;

    QMap<GuiViewSettings::Mode, CentralWidgetMode *> mModes;

    int modeTabIndex(const GuiViewSettings::Mode &pMode) const;

    void addMode(const GuiViewSettings::Mode &pMode);

    void saveModeSettings(QSettings *pSettings,
                          const GuiViewSettings::Mode &pMode) const;

    void addModeView(Plugin *pPlugin, GuiViewSettings *pSettings,
                     const GuiViewSettings::Mode &pMode);

    void updateModeGui(const GuiViewSettings::Mode &pMode,
                       GuiInterface * &pGuiInterface, int &pViewIndex);

    QString modeViewName(const GuiViewSettings::Mode &pMode);

    void updateNoViewMsg();

Q_SIGNALS:
    void fileOpened(const QString &pFileName);
    void fileClosed(const QString &pFileName);
    void fileSelected(const QString &pFileName);

public Q_SLOTS:
    void openFiles(const QStringList &pFileNames);
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
