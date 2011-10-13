//==============================================================================
// Internationalisation interface
//==============================================================================

#include "i18ninterface.h"

//==============================================================================

#include <QApplication>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void I18nInterface::setI18nPluginName(const QString &pI18nPluginName)
{
    // Set the name of the plugin

    mI18nPluginName = pI18nPluginName;
}

//==============================================================================

void I18nInterface::setLocale(const QString &pLocale)
{
    // Update the plugin's translator

    qApp->removeTranslator(&mTranslator);
    mTranslator.load(QString(":%1_%2").arg(mI18nPluginName, pLocale));
    qApp->installTranslator(&mTranslator);

    // Retranslate the plugin

    retranslateUi();
}

//==============================================================================

void I18nInterface::retranslateUi()
{
    // Nothing to do by default...
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
