#include "guiinterface.h"

#include <QApplication>

namespace OpenCOR {

GuiInterface::GuiInterface(const QString &pPluginName) :
    mPluginName(pPluginName)
{
}

void GuiInterface::initialize(MainWindow *)
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

void GuiInterface::retranslateUi()
{
    // Nothing to do by default...
}

}
