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
// User message widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QLabel>
#include <QString>

//==============================================================================

class QWidget;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT UserMessageWidget : public QLabel, public CommonWidget
{
    Q_OBJECT

public:
    explicit UserMessageWidget(const QString &pIcon, const QString &pMessage,
                               const QString &pExtraMessage, QWidget *pParent);
    explicit UserMessageWidget(const QString &pIcon, const QString &pMessage,
                               QWidget *pParent);
    explicit UserMessageWidget(const QString &pIcon, QWidget *pParent);
    explicit UserMessageWidget(QWidget *pParent);

    void setScale(const double &pScale);

    void setIcon(const QString &pIcon);
    void setIconMessage(const QString &pIcon, const QString &pMessage,
                        const QString &pExtraMessage = QString());
    void setMessage(const QString &pMessage,
                    const QString &pExtraMessage = QString());
    void resetMessage();

private:
    double mScale;
    double mDefaultFontScale;

    QString mIcon;
    QString mMessage;
    QString mExtraMessage;

    void updateGui();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
