//==============================================================================
// Common widget
//==============================================================================

#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "plugin.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <QColor>
#include <QString>

//==============================================================================

class QSettings;
class QSize;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

// The default font family and size below were taken from Qt Creator

#ifdef Q_WS_WIN
    static const QString DefaultFontFamily = "Courier";
    static const int DefaultFontSize = 10;
#elif defined(Q_WS_MAC)
    static const QString DefaultFontFamily = "Monaco";
    static const int DefaultFontSize = 12;
#else
    static const QString DefaultFontFamily = "Monospace";
    static const int DefaultFontSize = 9;
#endif

//==============================================================================

class CORE_EXPORT CommonWidget
{
public:
    explicit CommonWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void loadingOfSettingsDone(const Plugins &pLoadedPlugins);

    static QColor borderColor();

protected:
    QSize defaultSize(const double &pRatio) const;

    void drawBorder(const bool &pDockedTop, const bool &pDockedLeft,
                    const bool &pDockedBottom, const bool &pDockedRight,
                    const bool &pFloatingTop, const bool &pFloatingLeft,
                    const bool &pFloatingBottom, const bool &pFloatingRight);

private:
    QWidget *mParent;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
