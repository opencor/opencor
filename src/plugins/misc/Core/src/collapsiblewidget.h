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

class CollapsibleHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CollapsibleHeaderWidget(QWidget *pParent = 0);

    bool isCollapsed() const;
    void setCollapsed(const bool &pCollapsed);

    QString title() const;
    void setTitle(const QString &pTitle);

private:
    bool mCollapsed;

    QToolButton *mButton;
    QLabel *mTitle;

Q_SIGNALS:
    void contentsVisible(const bool &pVisible);

private Q_SLOTS:
    void toggleCollapsedState();
};

//==============================================================================

class CORE_EXPORT CollapsibleWidget : public Widget
{
    Q_OBJECT

public:
    explicit CollapsibleWidget(QWidget *pParent = 0);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool isCollapsed(const int &pIndex) const;
    void setCollapsed(const int &pIndex, const bool &pCollapsed);

    QString headerTitle(const int &pIndex) const;
    void setHeaderTitle(const int &pIndex, const QString &pTitle);

    void addWidget(QWidget *pWidget);

private:
    QList<CollapsibleHeaderWidget *> mHeaders;
    QList<QWidget *> mBodies;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
