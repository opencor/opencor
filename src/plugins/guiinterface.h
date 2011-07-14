#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "mainwindow.h"
#include "plugininterface.h"

namespace OpenCOR {

class GuiInterface : public PluginInterface
{
public:
    explicit GuiInterface(const QString &pPluginName);

    virtual void initialize(MainWindow *);

    void setLocale(const QString &pLocale);

private:
    QString mPluginName;

    QTranslator mTranslator;

    virtual void retranslateUi();
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
