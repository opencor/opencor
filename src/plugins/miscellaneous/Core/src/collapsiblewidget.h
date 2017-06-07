/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Collapsible widget
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "coreguiutils.h"
#include "widget.h"

//==============================================================================

#include <QLabel>

//==============================================================================

class QFrame;
class QToolButton;
class QVBoxLayout;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CollapsibleHeaderTitleWidget : public QLabel
{
    Q_OBJECT

public:
    explicit CollapsibleHeaderTitleWidget(QWidget *pParent);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent);

signals:
    void doubleClicked();
};

//==============================================================================

class CollapsibleHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CollapsibleHeaderWidget(const QColor &pSeparatorColor,
                                     const bool &pCollapsible,
                                     QWidget *pParent);

    void setFirstHeader(const bool &pFirstHeader);
    void setLastHeader(const bool &pFirstHeader);

    bool isCollapsable() const;

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
    CollapsibleHeaderTitleWidget *mTitle;

    void updateBottomSeparatorVisibleStatus();

signals:
    void widgetVisible(const bool &pVisible);

private slots:
    void toggleCollapsedState();
};

//==============================================================================

class CORE_EXPORT CollapsibleWidget : public Widget
{
    Q_OBJECT

public:
    explicit CollapsibleWidget(const QColor &pSeparatorColor, QWidget *pParent);
    explicit CollapsibleWidget(QWidget *pParent);

    int count() const;

    bool isCollapsed(const int &pIndex) const;
    void setCollapsed(const int &pIndex, const bool &pCollapsed);

    QString headerTitle(const int &pIndex) const;
    void setHeaderTitle(const int &pIndex, const QString &pTitle);

    void addWidget(QWidget *pWidget, const bool &pCollapsible = true);

private:
    QVBoxLayout *mLayout;

    QColor mSeparatorColor;

    QList<CollapsibleHeaderWidget *> mHeaders;

signals:
    void collapsed(const int &pIndex, const bool &pCollapsed);

private slots:
    void emitCollapsed();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
