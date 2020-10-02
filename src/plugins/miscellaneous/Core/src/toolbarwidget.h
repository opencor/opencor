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
// Tool bar widget
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QToolBar>
#include <QWidgetAction>

//==============================================================================

class QLabel;
class QLineEdit;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT ToolBarLabelWidgetAction : public QWidgetAction
{
    Q_OBJECT

public:
    ToolBarLabelWidgetAction(QWidget *pParent);

    QList<QLabel *> labels() const;
    bool validLabel(QLabel *pLabel) const;

protected:
    QWidget * createWidget(QWidget *pParent) override;

private:
    void emitCreated(QLabel *pLabel);

signals:
    void created(QLabel *pLabel);
};

//==============================================================================

class CORE_EXPORT ToolBarLineEditWidgetAction : public QWidgetAction
{
    Q_OBJECT

public:
    ToolBarLineEditWidgetAction(QWidget *pParent);

    QList<QLineEdit *> lineEdits() const;
    bool validLineEdit(QLineEdit *pLineEdit) const;

    void setText(const QString &pText);

protected:
    QWidget * createWidget(QWidget *pParent) override;

private:
    void emitCreated(QLineEdit *pLineEdit);
    void emitReturnPressed();

signals:
    void created(QLineEdit *pLineEdit);
    void returnPressed(const QString &pText);
};

//==============================================================================

class CORE_EXPORT ToolBarWidget : public QToolBar
{
    Q_OBJECT

public:
    explicit ToolBarWidget(QWidget *pParent);

    QAction * addSpacerWidgetAction(QSizePolicy::Policy pHorizontalSizePolicy,
                                    QSizePolicy::Policy pVerticalSizePolicy);
    ToolBarLabelWidgetAction * addLabelWidgetAction();
    ToolBarLineEditWidgetAction * addLineEditWidgetAction();
    QAction * addWidgetAction(QWidget *pWidget);
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
