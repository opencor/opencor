#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "mainwindow.h"
#include "plugininterface.h"

namespace OpenCOR {

class GuiSettings;

class GuiSettingsAction {
    friend class GuiSettings;

public:
    enum GuiSettingsActionType
    {
        Help
    };

    explicit GuiSettingsAction(const GuiSettingsActionType &pType,
                               QAction *pAction);

    GuiSettingsActionType type();
    QAction *action();

private:
    GuiSettingsActionType mType;
    QAction *mAction;
};

class GuiSettings
{
public:
    void addAction(const GuiSettingsAction::GuiSettingsActionType &pType,
                   QAction *pAction);

    QList<GuiSettingsAction> actions();

private:
    QList<GuiSettingsAction> mActions;
};

class GuiInterface : public PluginInterface
{
public:
    explicit GuiInterface(const QString &pPluginName);

    virtual void initialize(MainWindow *, GuiSettings *);

    void setLocale(const QString &pLocale);

protected:
    QAction * newAction(MainWindow *pMainWindow, const bool &pCheckable,
                        const QString &pIconResource);

private:
    QString mPluginName;

    QTranslator mTranslator;

    virtual void retranslateUi();
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
