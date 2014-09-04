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
// Widget
//==============================================================================

#ifndef WIDGET_H
#define WIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT Widget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *pParent);
    explicit Widget(const QSize &pSizeHint, QWidget *pParent);

protected:
    virtual QSize sizeHint() const;

    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    QSize mSizeHint;

Q_SIGNALS:
    void resized(const QSize &pSize, const QSize &pOldSize);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
