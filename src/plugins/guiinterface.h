#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "plugininterface.h"

#include <QTranslator>

class QAction;
class QMainWindow;
class QMenu;
class QSettings;
class QTabBar;

namespace OpenCOR {
namespace Core {
    class CentralWidget;
    class DockWidget;
}

static const QString CorePlugin = "Core";
static const QString HelpPlugin = "Help";

class GuiCoreSettings
{
public:
    explicit GuiCoreSettings(Core::CentralWidget *pCentralWidget);

    Core::CentralWidget *centralWidget() const;

private:
    Core::CentralWidget *mCentralWidget;
};

class GuiHelpSettings
{
public:
    explicit GuiHelpSettings(QAction *pHelpAction,
                             Core::DockWidget *pHelpWindow);

    QAction *helpAction() const;
    Core::DockWidget *helpWindow() const;

private:
    QAction *mHelpAction;
    Core::DockWidget *mHelpWindow;
};

class GuiMenuSettings
{
public:
    enum GuiMenuSettingsType
    {
        View
    };

    explicit GuiMenuSettings(const GuiMenuSettingsType &pType,
                             QMenu *pMenu);

    GuiMenuSettingsType type() const;
    QMenu *menu() const;

private:
    GuiMenuSettingsType mType;
    QMenu *mMenu;
};

class GuiMenuActionSettings
{
public:
    enum GuiMenuActionSettingsType
    {
        File
    };

    explicit GuiMenuActionSettings(const GuiMenuActionSettingsType &pType,
                                   QAction *pAction);

    GuiMenuActionSettingsType type() const;
    QAction *action() const;

private:
    GuiMenuActionSettingsType mType;
    QAction *mAction;
};

class GuiViewSettings
{
public:
    enum Mode
    {
        Editing,
        Simulation,
        Analysis
    };

    explicit GuiViewSettings(const Mode &pMode);

    Mode mode() const;

    void setTabBar(QTabBar *pTabBar);
    void setTabIndex(const int &pTabIndex);

    QTabBar * tabBar() const;
    int tabIndex() const;

private:
    Mode mMode;

    QTabBar *mTabBar;
    int mTabIndex;
};

class GuiWindowSettings
{
public:
    explicit GuiWindowSettings(const Qt::DockWidgetArea &pDefaultDockingArea,
                               Core::DockWidget *pWindow);

    Qt::DockWidgetArea defaultDockingArea() const;
    Core::DockWidget *window() const;

private:
    Qt::DockWidgetArea mDefaultDockingArea;
    Core::DockWidget *mWindow;
};

class GuiSettings
{
public:
    ~GuiSettings();

    void addMenu(const GuiMenuSettings::GuiMenuSettingsType &pType,
                 QMenu *pMenu);
    void addMenuAction(const GuiMenuActionSettings::GuiMenuActionSettingsType &pType,
                       QAction *pAction = 0);
    void addView(const GuiViewSettings::Mode &pMode);
    void addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                   Core::DockWidget *pWindow);

    QList<GuiMenuSettings *> menus() const;
    QList<GuiMenuActionSettings *> menuActions() const;
    QList<GuiViewSettings *> views() const;
    QList<GuiWindowSettings *> windows() const;

private:
    QList<GuiMenuSettings *> mMenus;
    QList<GuiMenuActionSettings *> mMenuActions;
    QList<GuiViewSettings *> mViews;
    QList<GuiWindowSettings *> mWindows;
};

class GuiInterface : public PluginInterface
{
public:
    explicit GuiInterface(const QString &pPluginName);
    ~GuiInterface();

    virtual void loadSettings(QSettings *pSettings,
                              const bool &pNeedDefaultSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    GuiSettings * guiSettings() const;
    void * data() const;

    QString pluginName() const;

    void setLocale(const QString &pLocale);

    void setParameters(const QList<Plugin *> &pLoadedPlugins,
                       QMainWindow *pMainWindow);

protected:
    QMainWindow *mMainWindow;

    GuiSettings *mGuiSettings;
    void *mData;
    // Note: mData is used only by the Core and Help plugins which are both one
    //       of a kind and therefore require special treatment (as opposed to
    //       generic treatment)

    virtual void retranslateUi();

    static QMenu * newMenu(QMainWindow *pMainWindow);
    static QAction * newAction(QMainWindow *pMainWindow,
                               const bool &pCheckable = false,
                               const QString &pIconResource = QString(),
                               const bool &pVisible = true);

    static void retranslateMenu(QMenu *pMenu, const QString &pTitle);
    static void retranslateAction(QAction *pAction, const QString &pText,
                                  const QString &pStatusTip,
                                  const QString &pShortcut = QString());

private:
    QString mPluginName;

    QTranslator mTranslator;

    void loadWindowSettings(QSettings *pSettings,
                            const bool &pNeedDefaultSettings,
                            const Qt::DockWidgetArea &pDefaultDockingArea,
                            Core::DockWidget *pWindow);
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
