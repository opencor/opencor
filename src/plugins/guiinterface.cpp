#include "guiinterface.h"

#include <QApplication>

namespace OpenCOR {

GuiAction::GuiAction(const GuiActionType &pType, const bool &pCheckable,
                     const QString &pIconResource) :
    mType(pType),
    mCheckable(pCheckable),
    mIconResource(pIconResource)
{
}

GuiAction::GuiActionType GuiAction::type()
{
    // Return the action's type

    return mType;
}

bool GuiAction::checkable()
{
    // Return whether the action is checkable

    return mCheckable;
}

QString GuiAction::iconResource()
{
    // Return the action's icon resource

    return mIconResource;
}

GuiInterface::GuiInterface(const QString &pPluginName) :
    mPluginName(pPluginName)
{
}

void GuiInterface::initialize(MainWindow *)
{
    // Nothing to do by default...
}

QList<GuiAction> GuiInterface::actions()
{
    // Return the plugin's actions that are to be created and incorporated into
    // OpenCOR

    return mActions;
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

void GuiInterface::retranslateUi()
{
    // Nothing to do by default...
}

}
