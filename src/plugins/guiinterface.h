#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "mainwindow.h"
#include "plugininterface.h"

namespace OpenCOR {

class GuiAction
{
public:
    enum GuiActionType
    {
        Help
    };

    explicit GuiAction(const GuiActionType &pType, const bool &pCheckable,
                       const QString &pIconResource);

    GuiActionType type();
    bool checkable();
    QString iconResource();

private:
    GuiActionType mType;
    bool mCheckable;
    QString mIconResource;
};

class GuiInterface : public PluginInterface
{
public:
    explicit GuiInterface(const QString &pPluginName);

    virtual void initialize(MainWindow *);

    QList<GuiAction> actions();

    void setLocale(const QString &pLocale);

protected:
    QList<GuiAction> mActions;

private:
    QString mPluginName;

    QTranslator mTranslator;

    virtual void retranslateUi();
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
