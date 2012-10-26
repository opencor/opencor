//==============================================================================
// Collapsible widget
//==============================================================================

#ifndef COLLAPSIBLEWIDGET_H
#define COLLAPSIBLEWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"
#include "widget.h"

//==============================================================================

class QFrame;
class QLabel;
class QScrollArea;
class QToolButton;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT CollapsibleWidget : public Widget
{
    Q_OBJECT

public:
    explicit CollapsibleWidget(const QString &pTitle, QWidget *pBody,
                               QWidget *pParent = 0);
    explicit CollapsibleWidget(QWidget *pParent = 0);

    QString title() const;
    void setTitle(const QString &pTitle);

    QWidget * body() const;
    void setBody(QWidget *pBody);

    void setCollapsed(const bool &pCollapsed);
    bool isCollapsed() const;

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    bool mCollapsed;

    QLabel *mTitle;
    QToolButton *mButton;

    QFrame *mSeparator;

    QScrollArea *mScrollArea;
    QWidget *mBody;

    int mFirstHeightUpdate;

    void constructor(const QString &pTitle = QString(), QWidget *pBody = 0);

    void updateGui(const bool &pCollapsed);

private Q_SLOTS:
    void toggleCollapsibleState();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
