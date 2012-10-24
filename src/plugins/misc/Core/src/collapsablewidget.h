//==============================================================================
// Collapsable widget
//==============================================================================

#ifndef COLLAPSABLEWIDGET_H
#define COLLAPSABLEWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "widget.h"

//==============================================================================

class QLabel;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT CollapsableWidget : public Widget
{
    Q_OBJECT

public:
    explicit CollapsableWidget(const QString &pTitle = QString(),
                               QWidget *pBody = 0, QWidget *pParent = 0);

    QString title() const;
    void setTitle(const QString &pTitle);

    QWidget * body() const;
    void setBody(QWidget *pBody);

private:
    QLabel *mTitle;
    QWidget *mBody;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
