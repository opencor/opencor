#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "plugininterface.h"

#include <QMainWindow>
#include <QTranslator>

namespace OpenCOR {
namespace Core {
    class CentralWidget;
    class DockWidget;
}

class GuiSettingsCorePlugin
{
public:
    explicit GuiSettingsCorePlugin(Core::CentralWidget *pCentralWidget);

    Core::CentralWidget *centralWidget() const;

private:
    Core::CentralWidget *mCentralWidget;
};

class GuiSettingsHelpPlugin
{
public:
    explicit GuiSettingsHelpPlugin(QAction *pHelpAction,
                                   Core::DockWidget *pHelpWindow);

    QAction *helpAction() const;
    Core::DockWidget *helpWindow() const;

private:
    QAction *mHelpAction;
    Core::DockWidget *mHelpWindow;
};

class GuiSettingsMenu
{
public:
    enum GuiSettingsMenuType
    {
        View
    };

    explicit GuiSettingsMenu(const GuiSettingsMenuType &pType,
                             QMenu *pMenu);

    GuiSettingsMenuType type() const;
    QMenu *menu() const;

private:
    GuiSettingsMenuType mType;
    QMenu *mMenu;
};

class GuiSettingsAction
{
public:
    enum GuiSettingsActionType
    {
        File
    };

    explicit GuiSettingsAction(const GuiSettingsActionType &pType,
                               QAction *pAction);

    GuiSettingsActionType type() const;
    QAction *action() const;

private:
    GuiSettingsActionType mType;
    QAction *mAction;
};

class GuiSettingsWindow
{
public:
    explicit GuiSettingsWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
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
    void addMenu(const GuiSettingsMenu::GuiSettingsMenuType &pType,
                 QMenu *pMenu);
    void addAction(const GuiSettingsAction::GuiSettingsActionType &pType,
                   QAction *pAction = 0);
    void addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                   Core::DockWidget *pWindow);

    QList<GuiSettingsMenu> menus() const;
    QList<GuiSettingsAction> actions() const;
    QList<GuiSettingsWindow> windows() const;

private:
    QList<GuiSettingsMenu> mMenus;
    QList<GuiSettingsAction> mActions;
    QList<GuiSettingsWindow> mWindows;
};

class GuiInterface : public PluginInterface
{
public:
    enum Mode
    {
        None,
        Editing,
        Simulation,
        Analysis
    };

    explicit GuiInterface(const QString &pPluginName);

    virtual void initialize(const QList<Plugin *> &, QMainWindow *);

    virtual Mode requiredMode() const;

    GuiSettings settings() const;
    void * data() const;

    QString pluginName() const;

    void setLocale(const QString &pLocale);

protected:
    GuiSettings mSettings;
    void *mData;
    // Note: mData is used only by the Core and Help plugins which are both one
    //       of a kind and therefore require special treatment (as opposed to
    //       generic treatment)

    static QMenu * newMenu(QMainWindow *pMainWindow);
    static QAction * newAction(QMainWindow *pMainWindow,
                               const bool &pCheckable = false,
                               const QString &pIconResource = QString());

    static void retranslateMenu(QMenu *pMenu, const QString &pTitle);
    static void retranslateAction(QAction *pAction, const QString &pText,
                                  const QString &pStatusTip,
                                  const QString &pShortcut = QString());

private:
    QString mPluginName;

    QTranslator mTranslator;

    virtual void retranslateUi();
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
