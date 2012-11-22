//==============================================================================
// Widget
//==============================================================================

#ifndef WIDGET_H
#define WIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT Widget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit Widget(const QSize &pSizeHint, QWidget *pParent = 0);
    explicit Widget(QWidget *pParent = 0);

protected:
    virtual QSize sizeHint() const;

private:
    QSize mSizeHint;

Q_SIGNALS:
    void fileTabIcon(const QString &pFileName, const QIcon &pIcon);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
