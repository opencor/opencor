#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "centralwidget.h"
#include "dockwidget.h"
#include "plugininterface.h"

#include <QMainWindow>
#include <QTranslator>

namespace OpenCOR {

class GuiSettingsCoreAction
{
public:
    enum GuiSettingsCoreActionType
    {
        Undefined,
        File,
    };

    explicit GuiSettingsCoreAction(const GuiSettingsCoreActionType &pType,
                                   QAction *pAction);
    explicit GuiSettingsCoreAction(QMenu *pMenu, QAction *pAction);

    GuiSettingsCoreActionType type() const;
    QMenu *menu() const;
    QAction *action() const;

private:
    GuiSettingsCoreActionType mType;
    QMenu *mMenu;
    QAction *mAction;
};

class GuiSettingsCorePlugin
{
public:
    explicit GuiSettingsCorePlugin(Core::CentralWidget *pCentralWidget);

    void addAction(const GuiSettingsCoreAction::GuiSettingsCoreActionType &pType,
                   QAction *pAction = 0);
    void addAction(QMenu *pMenu, QAction *pAction = 0);

    QList<GuiSettingsCoreAction> actions() const;

    Core::CentralWidget *centralWidget() const;

private:
    QList<GuiSettingsCoreAction> mActions;

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

class GuiSettingsCentralWidget
{
public:
    explicit GuiSettingsCentralWidget(Core::CentralWidget *pCentralWidget);

    Core::CentralWidget *centralWidget() const;

private:
    Core::CentralWidget *mCentralWidget;
};

class GuiSettingsAction
{
public:
    enum GuiSettingsActionType
    {
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
    void addCentralWidget(Core::CentralWidget *pCentralWidget);

    void addAction(const GuiSettingsAction::GuiSettingsActionType &pType,
                   QAction *pAction);

    void addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                   Core::DockWidget *pWindow);

    QList<GuiSettingsCentralWidget> centralWidget() const;
    QList<GuiSettingsAction> actions() const;
    QList<GuiSettingsWindow> windows() const;

private:
    QList<GuiSettingsCentralWidget> mCentralWidget;
    QList<GuiSettingsAction> mActions;
    QList<GuiSettingsWindow> mWindows;
};

class GuiInterface : public PluginInterface
{
public:
    explicit GuiInterface(const QString &pPluginName);

    virtual void initialize(QMainWindow *);

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

    static QAction * newAction(QMainWindow *pMainWindow,
                               const bool &pCheckable = false,
                               const QString &pIconResource = QString());

    void retranslateAction(QAction *pAction, const QString &pText,
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
