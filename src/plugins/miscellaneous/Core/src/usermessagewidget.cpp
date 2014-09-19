/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// User message widget
//==============================================================================

#include "usermessagewidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QFont>
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

    mIcon = QString();
    mMessage = QString();
    mExtraMessage = QString();

    // Customise our background

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    // Increase the size of our font

    QFont newFont = font();

    newFont.setPointSizeF(1.35*newFont.pointSize());

    setFont(newFont);

    // Some other customisations

    setContextMenuPolicy(Qt::NoContextMenu);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWordWrap(true);

    // 'Initialise' our message

    setIconMessage(pIcon, pMessage, pExtraMessage);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon,
                                     const QString &pMessage,
                                     const QString &pExtraMessage,
                                     QWidget *pParent) :
    QLabel(pParent)
{
    // Construct our object

    constructor(pIcon, pMessage, pExtraMessage);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon,
                                     const QString &pMessage,
                                     QWidget *pParent) :
    QLabel(pParent)
{
    // Construct our object

    constructor(pIcon, pMessage);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon, QWidget *pParent) :
    QLabel(pParent)
{
    // Construct our object

    constructor(pIcon);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(QWidget *pParent) :
    QLabel(pParent)
{
    // Construct our object

    constructor();
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
        mIcon = pIcon;
        mMessage = pMessage;
        mExtraMessage = pExtraMessage;

        if (mExtraMessage.isEmpty())
            setText(QString("<table align=center>"
                            "    <tr valign=middle>"
                            "        <td>"
                            "            <img src=\"%1\"/>"
                            "        </td>"
                            "        <td align=center>"
                            "            <p style=\"margin: 0px;\">"
                            "                %2"
                            "            </p>"
                            "        </td>"
                            "    </tr>"
                            "</table>").arg(mIcon, mMessage));
        else
            setText(QString("<table align=center>"
                            "    <tr valign=middle>"
                            "        <td>"
                            "            <img src=\"%1\"/>"
                            "        </td>"
                            "        <td align=center>"
                            "            <p style=\"margin: 0px;\">"
                            "                %2"
                            "            </p>"
                            "            <p style=\"margin: 0px;\">"
                            "                <small><em>(%3)</em></small>"
                            "            </p>"
                            "        </td>"
                            "    </tr>"
                            "</table>").arg(mIcon, mMessage, mExtraMessage));
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

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
