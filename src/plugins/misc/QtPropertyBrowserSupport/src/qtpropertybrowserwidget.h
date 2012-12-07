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

#include <QtTreePropertyBrowser>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class DoublePropertyManager;

//==============================================================================

class QtPropertyBrowserTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    int rowHeight(const QModelIndex &pIndex) const;
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

    QtProperty *addDoubleProperty(const QString &pName = QString());

    double doublePropertyValue(QtProperty *pProperty) const;
    void setDoublePropertyValue(QtProperty *pProperty, const double &pValue);

    QString doublePropertyUnit(QtProperty *pProperty) const;
    void setDoublePropertyUnit(QtProperty *pProperty, const QString &pUnit);

    void selectFirstProperty();

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    DoublePropertyManager *mDoublePropertyManager;

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
