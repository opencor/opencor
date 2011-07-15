#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "dockwidget.h"
#include "mainwindow.h"
#include "plugininterface.h"

namespace OpenCOR {

class GuiSettings;

class GuiSettingsDockWidget {
public:
    explicit GuiSettingsDockWidget(const Qt::DockWidgetArea &pDefaultDockingArea,
                                   Core::DockWidget *pDockWidget);

    Qt::DockWidgetArea defaultDockingArea() const;
    Core::DockWidget *dockWidget() const;

private:
    Qt::DockWidgetArea mDefaultDockingArea;
    Core::DockWidget *mDockWidget;
};

class GuiSettingsAction {
public:
    enum GuiSettingsActionType
    {
        Help
    };

    explicit GuiSettingsAction(const GuiSettingsActionType &pType,
                               QAction *pAction);

    GuiSettingsActionType type() const;
    QAction *action() const;

private:
    GuiSettingsActionType mType;
    QAction *mAction;
};

class GuiSettings
{
public:
    void addDockWidget(const Qt::DockWidgetArea &pDefaultDockingArea,
                       Core::DockWidget *pDockWidget);

    void addAction(const GuiSettingsAction::GuiSettingsActionType &pType,
                   QAction *pAction);

    QList<GuiSettingsAction> actions() const;

private:
    QList<GuiSettingsDockWidget> mDockWidgets;
    QList<GuiSettingsAction> mActions;
};

class GuiInterface : public PluginInterface
{
public:
    explicit GuiInterface(const QString &pPluginName);

    virtual void initialize(MainWindow *);

    GuiSettings settings() const;

    void setLocale(const QString &pLocale);

protected:
    GuiSettings mSettings;

    QAction * newAction(MainWindow *pMainWindow, const bool &pCheckable,
                        const QString &pIconResource) const;

private:
    QString mPluginName;

    QTranslator mTranslator;

    virtual void retranslateUi();
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
