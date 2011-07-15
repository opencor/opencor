#include "guiinterface.h"

#include <QAction>
#include <QApplication>

namespace OpenCOR {

GuiSettingsAction::GuiSettingsAction(const GuiSettingsActionType &pType,
                                     QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

GuiSettingsAction::GuiSettingsActionType GuiSettingsAction::type()
{
    // Return the action's type

    return mType;
}

QAction * GuiSettingsAction::action()
{
    // Return the action itself

    return mAction;
}

void GuiSettings::addAction(const GuiSettingsAction::GuiSettingsActionType &pType,
                            QAction *pAction)
{
    // Add a new action to our list

    mActions << GuiSettingsAction(pType, pAction);
}

QList<GuiSettingsAction> GuiSettings::actions()
{
    // Return the settings actions

    return mActions;
}

GuiInterface::GuiInterface(const QString &pPluginName) :
    mPluginName(pPluginName)
{
}

void GuiInterface::initialize(MainWindow *, GuiSettings *)
{
    // Nothing to do by default...
}

void GuiInterface::setLocale(const QString &pLocale)
{
    // Update the plugin's translator

    qApp->removeTranslator(&mTranslator);
    mTranslator.load(QString(":%1_%2").arg(mPluginName, pLocale));
    qApp->installTranslator(&mTranslator);

    // Retranslate the plugin

    retranslateUi();
}

QAction * GuiInterface::newAction(MainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource)
{
    // Create and return an action

    QAction *res = new QAction(pMainWindow);

    res->setCheckable(pCheckable);
    res->setIcon(QIcon(pIconResource));

    return res;
}

void GuiInterface::retranslateUi()
{
    // Nothing to do by default...
}

}
