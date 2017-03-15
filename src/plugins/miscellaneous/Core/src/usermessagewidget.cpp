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

void UserMessageWidget::constructor(const QString &pIcon,
                                    const QString &pMessage,
                                    const QString &pExtraMessage)
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
                                     const QString &pExtraMessage,
                                     QWidget *pParent) :
    QLabel(pParent),
    CommonWidget(this)
{
    // Construct our object

    constructor(pIcon, pMessage, pExtraMessage);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon,
                                     const QString &pMessage,
                                     QWidget *pParent) :
    QLabel(pParent),
    CommonWidget(this)
{
    // Construct our object

    constructor(pIcon, pMessage);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon, QWidget *pParent) :
    QLabel(pParent),
    CommonWidget(this)
{
    // Construct our object

    constructor(pIcon);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(QWidget *pParent) :
    QLabel(pParent),
    CommonWidget(this)
{
    // Construct our object

    constructor();
}

//==============================================================================

QSize UserMessageWidget::sizeHint() const
{
    // Suggest a default size for our user message widget
    // Note: this is critical if we want a docked widget, with a user message
    //       widget on it, to have a decent size when docked to the main
    //       window...

    return defaultSize(0.15);
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
