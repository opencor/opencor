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

#include "coreguiutils.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QBuffer>
#include <QFont>
#include <QIcon>
#include <QSizePolicy>
#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon,
                                     const QString &pMessage,
                                     const QString &pExtraMessage,
                                     QWidget *pParent) :
    QLabel(pParent),
    CommonWidget(this)
{
    // Some initialisations

    mScale = 0.0;
    mDefaultFontScale = 1.35*font().pointSize();

    mIcon = pIcon;
    mMessage = pMessage;
    mExtraMessage = pExtraMessage;

    // Some customisations

    setContextMenuPolicy(Qt::NoContextMenu);
    setScale(1.0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QLabel {"
                  "     background-color: white;"
                  "}");
    setWordWrap(true);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon,
                                     const QString &pMessage,
                                     QWidget *pParent) :
    UserMessageWidget(pIcon, pMessage, QString(), pParent)
{
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon, QWidget *pParent) :
    UserMessageWidget(pIcon, QString(), QString(), pParent)
{
}

//==============================================================================

UserMessageWidget::UserMessageWidget(QWidget *pParent) :
    UserMessageWidget(QString(), QString(), QString(), pParent)
{
}

//==============================================================================

void UserMessageWidget::setScale(const double &pScale)
{
    // Scale ourselves, if needed

    if (pScale != mScale) {
        mScale = pScale;

        QFont newFont = font();

        newFont.setPointSizeF(pScale*mDefaultFontScale);

        setFont(newFont);

        updateGui();
    }
}

//==============================================================================

void UserMessageWidget::updateGui()
{
    // Set our message as HTML
    // Note: we want our icon to have a final size of 32px by 32px. However, it
    //       may be that it has a different size to begin with. Normally, we
    //       would rely on QLabel's HTML support (and in particular on the width
    //       and height attributes of the img element) to have our icon resized
    //       for us, but this results in a pixelated and therefore ugly image.
    //       So, instead, we retrieve a data URI for our resized icon...

    if (mIcon.isEmpty() && mMessage.isEmpty() && mExtraMessage.isEmpty()) {
        setText(QString());
    } else {
        static const QString Message = "<table align=center>\n"
                                       "    <tbody>\n"
                                       "        <tr valign=middle>\n"
                                       "%1"
                                       "            <td>\n"
                                       "                %2\n"
                                       "            </td>\n"
                                       "        </tr>\n"
                                       "%3"
                                       "    </tbody>\n"
                                       "</table>\n";
        static const QString Icon = "            <td>\n"
                                    "                <img src=\"%1\">\n"
                                    "            </td>\n";
        static const QString ExtraMessage = "        <tr valign=middle>\n"
                                            "%1"
                                            "            <td align=center>\n"
                                            "                <small><em>(%2)</em></small>\n"
                                            "            </td>\n"
                                            "        </tr>\n";
        static const QString IconSpace = "            <td/>\n";

        setText(Message.arg(mIcon.isEmpty()?
                                QString():
                                Icon.arg(iconDataUri(mIcon, mScale*32, mScale*32)),
                            mMessage,
                            mExtraMessage.isEmpty()?
                                QString():
                                ExtraMessage.arg(mIcon.isEmpty()?
                                                     QString():
                                                     IconSpace,
                                                 mExtraMessage)));
    }
}

//==============================================================================

void UserMessageWidget::setIcon(const QString &pIcon)
{
    // Set our icon

    setIconMessage(pIcon, mMessage, mExtraMessage);
}

//==============================================================================

void UserMessageWidget::setIconMessage(const QString &pIcon,
                                       const QString &pMessage,
                                       const QString &pExtraMessage)
{
    // Set our message, if needed

    if (   pIcon.compare(mIcon)
        || pMessage.compare(mMessage)
        || pExtraMessage.compare(mExtraMessage)) {
        // Keep track of the new values for our icon, message and extra message

        mIcon = pIcon;
        mMessage = pMessage;
        mExtraMessage = pExtraMessage;

        // Update ourselves

        updateGui();
    }
}

//==============================================================================

void UserMessageWidget::setMessage(const QString &pMessage,
                                   const QString &pExtraMessage)
{
    // Set our message

    setIconMessage(mIcon, pMessage, pExtraMessage);
}

//==============================================================================

void UserMessageWidget::resetMessage()
{
    // Reset our message

    setIconMessage(QString(), QString());
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
