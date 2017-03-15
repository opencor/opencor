/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

    void constructor(const QColor &pSeparatorColor = borderColor());

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
