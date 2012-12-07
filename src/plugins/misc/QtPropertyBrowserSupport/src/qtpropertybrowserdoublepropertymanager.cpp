//==============================================================================
// QtPropertyBrowser double property manager class
//==============================================================================

#include "qtpropertybrowserdoublepropertymanager.h"

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

DoublePropertyManager::DoublePropertyManager(QObject *pParent) :
    QtAbstractPropertyManager(pParent),
    mData(QMap<const QtProperty *, Data>())
{
}

//==============================================================================

double DoublePropertyManager::value(QtProperty *pProperty) const
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return 0.0;

    // Return the property's value

    return mData[pProperty].value;
}

//==============================================================================

void DoublePropertyManager::setValue(QtProperty *pProperty,
                                     const double &pValue)
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return;

    // Set the property's value

    Data data = mData[pProperty];

    if (pValue != data.value) {
        data.value = pValue;

        mData[pProperty] = data;

        // Let people know that the property's value has changed

        emit propertyChanged(pProperty);
        emit valueChanged(pProperty, pValue);
    }
}

//==============================================================================

QString DoublePropertyManager::unit(QtProperty *pProperty) const
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return QString();

    // Return the property's unit

    return mData[pProperty].unit;
}

//==============================================================================

void DoublePropertyManager::setUnit(QtProperty *pProperty, const QString &pUnit)
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return;

    // Set the property's unit

    Data data = mData[pProperty];

    if (pUnit.compare(data.unit)) {
        data.unit = pUnit;

        mData[pProperty] = data;

        // Let people know that the property's unit has changed

        emit unitChanged(pProperty, pUnit);
    }
}

//==============================================================================

QString DoublePropertyManager::valueText(const QtProperty *pProperty) const
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return QString();

    // Return our property as a string

    return QString::number(mData[pProperty].value);
}

//==============================================================================

void DoublePropertyManager::initializeProperty(QtProperty *pProperty)
{
    // Initialise the property

    mData[pProperty] = Data();
}

//==============================================================================

void DoublePropertyManager::uninitializeProperty(QtProperty *pProperty)
{
    // Uninitialise the property

    mData.remove(pProperty);
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
