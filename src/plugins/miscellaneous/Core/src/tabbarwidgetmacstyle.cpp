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
// Tab bar widget Mac style
//==============================================================================

#include "tabbarwidgetmacstyle.h"

//==============================================================================

#include <QPainter>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

void TabBarWidgetMacStyle::drawControl(ControlElement pElement,
                                       const QStyleOption *pOption,
                                       QPainter *pPainter,
                                       const QWidget *pWidget) const
{
    // Draw a tab bar tab label
    // Note: anything else is done by our parent...

    if (pElement == CE_TabBarTabLabel) {
        // Note: adapted from QCommonStyle::drawControl()...

        if (auto tab = qstyleoption_cast<const QStyleOptionTab *>(pOption)) {
            uint alignment = Qt::AlignCenter|Qt::TextShowMnemonic;

            if (styleHint(SH_UnderlineShortcut, pOption, pWidget) == 0) {
                alignment |= Qt::TextHideMnemonic;
            }

            bool isVerticalTab =    (tab->shape == QTabBar::RoundedWest)
                                 || (tab->shape == QTabBar::RoundedEast)
                                 || (tab->shape == QTabBar::TriangularWest)
                                 || (tab->shape == QTabBar::TriangularEast);
            QRect tabRect = tab->rect;

            if (isVerticalTab) {
                pPainter->save();

                int x, y, rotation;

                if (   (tab->shape == QTabBar::RoundedEast)
                    || (tab->shape == QTabBar::TriangularEast)) {
                    x = tabRect.x()+tabRect.width();
                    y = tabRect.y();

                    rotation = 90;
                } else {
                    x = tabRect.x();
                    y = tabRect.y()+tabRect.height();

                    rotation = -90;
                }

                QTransform transform = QTransform::fromTranslate(x, y);

                transform.rotate(rotation);

                pPainter->setTransform(transform, true);
            }

            QRect iconRect;

            tabLayout(tab, pWidget, &tabRect, &iconRect);

            if (!tab->icon.isNull()) {
                pPainter->drawPixmap(iconRect.x(), iconRect.y(),
                                     tab->icon.pixmap(pWidget->window()->windowHandle(),
                                                      tab->iconSize,
                                                      ((tab->state & State_Enabled) != 0)?
                                                          QIcon::Normal:
                                                          QIcon::Disabled,
                                                      ((tab->state & State_Selected) != 0)?
                                                          QIcon::On:
                                                          QIcon::Off));
            }

            drawItemText(pPainter, tabRect, int(alignment), tab->palette,
                         (tab->state & State_Enabled) != 0, tab->text,
                         (   ((tab->state & State_Selected) != 0)
                          && ((tab->state & State_Active) != 0))?
                             QPalette::BrightText:
                             QPalette::WindowText);

            if (isVerticalTab) {
                pPainter->restore();
            }

            if ((tab->state & State_HasFocus) != 0) {
                const int Offset = 1+pixelMetric(PM_DefaultFrameWidth);

                int x1 = tab->rect.left();
                int x2 = tab->rect.right()-1;

                QStyleOptionFocusRect option;

                option.QStyleOption::operator=(*tab);

                option.rect.setRect(x1+1+Offset, tab->rect.y()+Offset,
                                    x2-x1-2*Offset, tab->rect.height()-2*Offset);

                drawPrimitive(PE_FrameFocusRect, &option, pPainter, pWidget);
            }
        }
    } else {
        QProxyStyle::drawControl(pElement, pOption, pPainter, pWidget);
    }
}

//==============================================================================

QRect TabBarWidgetMacStyle::subElementRect(SubElement pElement,
                                           const QStyleOption *pOption,
                                           const QWidget *pWidget) const
{
    // Determine and return the size of tab bar tab text
    // Note: anything else is done by our parent...

    if (pElement == SE_TabBarTabText) {
        // Note: adapted from QCommonStyle::subElementRect()...

        if (auto tab = qstyleoption_cast<const QStyleOptionTab *>(pOption)) {
            QRect res;
            QRect dummy;

            tabLayout(tab, pWidget, &res, &dummy);

            return res;
        }

        return {};
    }

    return QProxyStyle::subElementRect(pElement, pOption, pWidget);
}

//==============================================================================

void TabBarWidgetMacStyle::tabLayout(const QStyleOptionTab *pOption,
                                     const QWidget *pWidget, QRect *pTextRect,
                                     QRect *pIconRect) const
{
    // Compute our tab layout
    // Note: adapted from QCommonStylePrivate::tabLayout()...

    Q_ASSERT(pTextRect);
    Q_ASSERT(pIconRect);

    QRect textRect = pOption->rect;
    bool verticalTab =    (pOption->shape == QTabBar::RoundedWest)
                       || (pOption->shape == QTabBar::RoundedEast)
                       || (pOption->shape == QTabBar::TriangularWest)
                       || (pOption->shape == QTabBar::TriangularEast);

    if (verticalTab) {
        textRect.setRect(0, 0, textRect.height(), textRect.width());
    }

    if (!pOption->leftButtonSize.isEmpty()) {
        textRect.adjust(-4, 0, 0, 0);
    }

    int horizontalShift = pixelMetric(QStyle::PM_TabBarTabShiftHorizontal, pOption, pWidget);
    int verticalShift = pixelMetric(QStyle::PM_TabBarTabShiftVertical, pOption, pWidget);
    int horizontalPadding = pixelMetric(QStyle::PM_TabBarTabHSpace, pOption, pWidget)/2;
    int verticalPadding = pixelMetric(QStyle::PM_TabBarTabVSpace, pOption, pWidget)/2;

    if (   (pOption->shape == QTabBar::RoundedSouth)
        || (pOption->shape == QTabBar::TriangularSouth)) {
        verticalShift = -verticalShift;
    }

    textRect.adjust(horizontalPadding, verticalShift-verticalPadding,
                    horizontalShift-horizontalPadding, verticalPadding);

    if ((pOption->state & QStyle::State_Selected) != 0) {
        textRect.setTop(textRect.top()-verticalShift);
        textRect.setRight(textRect.right()-horizontalShift);
    }

    if (!pOption->leftButtonSize.isEmpty()) {
        textRect.setLeft( textRect.left()
                         +(verticalTab?
                               pOption->leftButtonSize.height():
                               pOption->leftButtonSize.width()));
    }

    if (!pOption->rightButtonSize.isEmpty()) {
        textRect.setRight( textRect.right()
                          -(verticalTab?
                                pOption->rightButtonSize.height():
                                pOption->rightButtonSize.width()));
    }

    if (!pOption->icon.isNull()) {
        QSize iconSize = pOption->iconSize;

        if (!iconSize.isValid()) {
            int iconExtent = pixelMetric(QStyle::PM_SmallIconSize);

            iconSize = QSize(iconExtent, iconExtent);
        }

        QSize tabIconSize = pOption->icon.actualSize(iconSize,
                                                     ((pOption->state & QStyle::State_Enabled) != 0)?
                                                         QIcon::Normal:
                                                         QIcon::Disabled,
                                                     ((pOption->state & QStyle::State_Selected) != 0)?
                                                         QIcon::On:
                                                         QIcon::Off);

        tabIconSize = QSize(qMin(tabIconSize.width(), iconSize.width()),
                            qMin(tabIconSize.height(), iconSize.height()));

        *pIconRect = QRect(textRect.left(), textRect.center().y()-tabIconSize.height()/2,
                           tabIconSize.width(), tabIconSize.height());

        if (!verticalTab) {
            *pIconRect = visualRect(pOption->direction, pOption->rect, *pIconRect);
        }

        textRect.setLeft(textRect.left()+tabIconSize.width()+4);
    }

    if (!verticalTab) {
        textRect = visualRect(pOption->direction, pOption->rect, textRect);
    }

    *pTextRect = textRect;
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
