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
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
