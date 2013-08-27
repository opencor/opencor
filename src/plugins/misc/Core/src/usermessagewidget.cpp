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

namespace OpenCOR {
namespace Core {

//==============================================================================

void UserMessageWidget::constructor(const QString &pIcon,
                                    const QString &pMessage)
{
    // Some initialisations

    mIcon = pIcon;
    mMessage = pMessage;

    // Customise our background

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    // Increase the size of our font

    QFont newFont = font();

    newFont.setPointSize(1.5*newFont.pointSize());

    setFont(newFont);

    // Some other customisations

    setContextMenuPolicy(Qt::NoContextMenu);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWordWrap(true);

    // 'Initialise' our message

    updateMessage();
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

void UserMessageWidget::updateMessage()
{
    // Update our message by setting the icon to the left and the message itself
    // to the right

    setText(QString("<table align=center>"
                    "    <tr valign=middle>"
                    "        <td align=right>"
                    "            <img src=\"%1\"/>"
                    "        </td>"
                    "        <td align=left>"
                    "            %2"
                    "        </td>"
                    "    </tr>"
                    "</table>").arg(mIcon, mMessage));
}

//==============================================================================

void UserMessageWidget::setIcon(const QString &pIcon)
{
    // Set the icon

    if (pIcon.compare(mIcon)) {
        mIcon = pIcon;

        updateMessage();
    }
}

//==============================================================================

void UserMessageWidget::setMessage(const QString &pMessage)
{
    // Set the message

    if (pMessage.compare(mMessage)) {
        mMessage = pMessage;

        updateMessage();
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
