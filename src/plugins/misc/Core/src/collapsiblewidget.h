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
class QSpacerItem;
class QToolButton;
class QVBoxLayout;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CollapsibleHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CollapsibleHeaderWidget(const QColor &pSeparatorColor,
                                     QWidget *pParent = 0);

    void setFirstHeader(const bool &pFirstHeader);
    void setLastHeader(const bool &pFirstHeader);

    bool isCollapsed() const;
    void setCollapsed(const bool &pCollapsed);

    QString title() const;
    void setTitle(const QString &pTitle);

private:
    bool mCollapsed;

    bool mLastHeader;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QToolButton *mButton;
    QLabel *mTitle;

    void updateBottomSeparatorVisibleStatus();

Q_SIGNALS:
    void widgetVisible(const bool &pVisible);

private Q_SLOTS:
    void toggleCollapsedState();
};

//==============================================================================

class CORE_EXPORT CollapsibleWidget : public Widget
{
    Q_OBJECT

public:
    explicit CollapsibleWidget(const QColor &pSeparatorColor,
                               QWidget *pParent = 0);
    explicit CollapsibleWidget(QWidget *pParent = 0);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool isCollapsed(const int &pIndex) const;
    void setCollapsed(const int &pIndex, const bool &pCollapsed);

    QString headerTitle(const int &pIndex) const;
    void setHeaderTitle(const int &pIndex, const QString &pTitle);

    void addWidget(QWidget *pWidget);

private:
    QVBoxLayout *mLayout;

    QColor mSeparatorColor;

    QList<CollapsibleHeaderWidget *> mHeaders;

    void constructor(const QColor &pSeparatorColor = CommonWidget::borderColor());
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
