//==============================================================================
// Common widget
//==============================================================================

#include "commonwidget.h"
#include "coreutils.h"
#include "dockwidget.h"

//==============================================================================

#include <QApplication>
#include <QColor>
#include <QDesktopWidget>
#include <QFrame>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QSettings>
#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CommonWidget::CommonWidget(QWidget *pParent) :
    mParent(pParent)
{
}

//==============================================================================

QSize CommonWidget::defaultSize(const double &pRatio) const
{
    // The default size of a widget is a ratio of the available geometry

    QRect desktopGeometry = qApp->desktop()->availableGeometry();

    return QSize(pRatio*desktopGeometry.width(),
                 pRatio*desktopGeometry.height());
}

//==============================================================================

void CommonWidget::retranslateUi()
{
    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::loadSettings(QSettings *pSettings)
{
    // Nothing to do by default...

    Q_UNUSED(pSettings);
}

//==============================================================================

void CommonWidget::saveSettings(QSettings *pSettings) const
{
    // Nothing to do by default...

    Q_UNUSED(pSettings);
}

//==============================================================================

void CommonWidget::loadingOfSettingsDone(const Plugins &pLoadedPlugins)
{
    // Nothing to do by default...

    Q_UNUSED(pLoadedPlugins);
}

//==============================================================================

QColor CommonWidget::specificColor(const QString &pColor)
{
    // Return a specific colour

    QColor res;
    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        res = settings.value(pColor).value<QColor>();
    settings.endGroup();

    return res;
}

//==============================================================================

QColor CommonWidget::borderColor()
{
    // Return the border colour
    // Note: we retrieve it from our settings which is updated by our plugin
    //       itself (see CorePlugin::retrieveBorderColor())...

    return specificColor(SettingsBorderColor);
}

//==============================================================================

QColor CommonWidget::baseColor()
{
    // Return the base colour
    // Note: we retrieve it from our settings which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsBaseColor);
}

//==============================================================================

QColor CommonWidget::windowColor()
{
    // Return the window colour
    // Note: we retrieve it from our settings which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsWindowColor);
}

//==============================================================================

QColor CommonWidget::highlightColor()
{
    // Return the highlight colour
    // Note: we retrieve it from our settings which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsHighlightColor);
}

//==============================================================================

void CommonWidget::drawBorder(const bool &pDockedTop, const bool &pDockedLeft,
                              const bool &pDockedBottom, const bool &pDockedRight,
                              const bool &pFloatingTop, const bool &pFloatingLeft,
                              const bool &pFloatingBottom, const bool &pFloatingRight)
{
    // Draw a border around the widget

    DockWidget *dockWidget = qobject_cast<DockWidget *>(mParent);
    QWidget *widget = dynamic_cast<QWidget *>(this);

    if (dockWidget && widget) {
        // The castings were successful, so our parent is really a DockWidget,
        // we are really a QWidget, and to top it all our parent is floating (or
        // we want to force the drawing), so let's go ahead...

        QPainter painter(widget);

        QPen newPen = painter.pen();

        newPen.setColor(borderColor());

        painter.setPen(newPen);

        bool isDocked = !dockWidget->isFloating();
        QRect border = widget->rect();

        if ((pDockedTop && isDocked) || (pFloatingTop && !isDocked))
            painter.drawLine(border.topLeft(), border.topRight());

        if ((pDockedLeft && isDocked) || (pFloatingLeft && !isDocked))
            painter.drawLine(border.topLeft(), border.bottomLeft());

        if ((pDockedBottom && isDocked) || (pFloatingBottom && !isDocked))
            painter.drawLine(border.bottomLeft(), border.bottomRight());

        if ((pDockedRight && isDocked) || (pFloatingRight && !isDocked))
            painter.drawLine(border.topRight(), border.bottomRight());
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
