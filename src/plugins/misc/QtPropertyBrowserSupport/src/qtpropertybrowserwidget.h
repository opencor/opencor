//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#ifndef QTPROPERTYBROWSERWIDGET_H
#define QTPROPERTYBROWSERWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "qtpropertybrowsersupportglobal.h"

//==============================================================================

#include <QTreeWidget>

//==============================================================================

#include <QtAbstractPropertyManager>
#include <QtTreePropertyBrowser>

//==============================================================================

class QtVariantProperty;
class QtVariantPropertyManager;

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class QtPropertyBrowserTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    int rowHeight(const QModelIndex &pIndex) const;
};

//==============================================================================

class DoublePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT

public:
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
};

//==============================================================================

class QTPROPERTYBROWSERSUPPORT_EXPORT QtPropertyBrowserWidget : public QtTreePropertyBrowser,
                                                                public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit QtPropertyBrowserWidget(const bool &pAutoResizeHeight,
                                     QWidget *pParent = 0);
    explicit QtPropertyBrowserWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QSize sizeHint() const;

    QtVariantProperty * addProperty(const int pType,
                                    const QString &pName = QString());

    void selectFirstProperty();

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    QtVariantPropertyManager *mPropertyManager;

    bool mAutoResizeHeight;

    void constructor(const bool &pAutoResizeHeight = false);

private Q_SLOTS:
    void resizeHeight();
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
