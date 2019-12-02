/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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
    void mouseDoubleClickEvent(QMouseEvent *pEvent) override;

signals:
    void doubleClicked();
};

//==============================================================================

class CORE_EXPORT CollapsibleHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CollapsibleHeaderWidget(bool pCollapsible, QWidget *pParent);

    void setFirstHeader(bool pFirstHeader);
    void setLastHeader(bool pLastHeader);

    bool isCollapsable() const;

    bool isCollapsed() const;
    void setCollapsed(bool pCollapsed);

    QString title() const;
    void setTitle(const QString &pTitle);

    QMenu * menu() const;
    void setMenu(QMenu *pMenu);

private:
    bool mCollapsed = false;

    bool mLastHeader = false;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QToolButton *mButton;
    CollapsibleHeaderTitleWidget *mTitle;
    QToolButton *mMenu;

    QMenu *mMenuMenu = nullptr;

    void updateBottomSeparatorVisibleStatus();

signals:
    void widgetVisible(bool pVisible);

private slots:
    void toggleCollapsedState();
    void showMenu();
};

//==============================================================================

class CORE_EXPORT CollapsibleWidget : public Widget
{
    Q_OBJECT

public:
    explicit CollapsibleWidget(QWidget *pParent);

    int count() const;

    bool isCollapsed(int pIndex) const;
    void setCollapsed(int pIndex, bool pCollapsed);

    QString headerTitle(int pIndex) const;
    void setHeaderTitle(int pIndex, const QString &pTitle);

    CollapsibleHeaderWidget * addWidget(QWidget *pWidget,
                                        bool pCollapsible = true);

private:
    QVBoxLayout *mLayout;

    QList<CollapsibleHeaderWidget *> mHeaders;

signals:
    void collapsed(int pIndex, bool pCollapsed);

private slots:
    void emitCollapsed();
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
