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

#ifndef USERMESSAGEWIDGET_H
#define USERMESSAGEWIDGET_H

//==============================================================================

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

class CORE_EXPORT UserMessageWidget : public QLabel
{
    Q_OBJECT

public:
    explicit UserMessageWidget(const QString &pIcon, const QString &pMessage,
                               const QString &pExtraMessage, QWidget *pParent);
    explicit UserMessageWidget(const QString &pIcon, const QString &pMessage,
                               QWidget *pParent);
    explicit UserMessageWidget(const QString &pIcon, QWidget *pParent);
    explicit UserMessageWidget(QWidget *pParent);

    void setIconMessage(const QString &pIcon, const QString &pMessage,
                        const QString &pExtraMessage = QString());
    void setMessage(const QString &pMessage,
                    const QString &pExtraMessage = QString());

private:
    QString mIcon;
    QString mMessage;
    QString mExtraMessage;

    void constructor(const QString &pIcon = QString(),
                     const QString &pMessage = QString(),
                     const QString &pExtraMessage = QString());
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
