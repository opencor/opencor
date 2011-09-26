#ifndef I18NINTERFACE_H
#define I18NINTERFACE_H

#include <QTranslator>

namespace OpenCOR {

class I18nInterface
{
public:
    void setLocale(const QString &pLocale);
    void setPluginName(const QString &pPluginName);

protected:
    virtual void retranslateUi();

private:
    QString mPluginName;

    QTranslator mTranslator;
};

}

Q_DECLARE_INTERFACE(OpenCOR::I18nInterface, "OpenCOR.I18nInterface")

#endif
