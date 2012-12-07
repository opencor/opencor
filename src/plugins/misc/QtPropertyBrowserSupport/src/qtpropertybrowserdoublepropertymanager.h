//==============================================================================
// QtPropertyBrowser double property manager class
//==============================================================================

#ifndef QTPROPERTYBROWSERDOUBLEPROPERTYMANAGER_H
#define QTPROPERTYBROWSERDOUBLEPROPERTYMANAGER_H

//==============================================================================

#include <QtAbstractPropertyManager>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class DoublePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT

public:
    explicit DoublePropertyManager(QObject *pParent = 0);

    double value(QtProperty *pProperty) const;
    void setValue(QtProperty *pProperty, const double &pValue);

    QString unit(QtProperty *pProperty) const;
    void setUnit(QtProperty *pProperty, const QString &pUnit);

protected:
    virtual QString valueText(const QtProperty *pProperty) const;

    virtual void initializeProperty(QtProperty *pProperty);
    virtual void uninitializeProperty(QtProperty *pProperty);

private:
    struct Data {
        double value;
        QString unit;
    };

    QMap<const QtProperty *, Data> mData;

Q_SIGNALS:
    void valueChanged(QtProperty *pProperty, const double &pValue);
    void unitChanged(QtProperty *pProperty, const QString &pUnit);
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
