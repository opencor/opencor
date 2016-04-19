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
// Bordered widget
//==============================================================================

#pragma once

//==============================================================================

#include "widget.h"
#include "coreglobal.h"

//==============================================================================

class QFrame;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT BorderedWidget : public Widget
{
    Q_OBJECT

public:
    explicit BorderedWidget(QWidget *pWidget,
                            const bool &pTop, const bool &pLeft,
                            const bool &pBottom, const bool &pRight);

    QWidget * widget();

    void setTopBorderVisible(const bool &pVisible) const;
    void setLeftBorderVisible(const bool &pVisible) const;
    void setBottomBorderVisible(const bool &pVisible) const;
    void setRightBorderVisible(const bool &pVisible) const;

private:
    QWidget *mWidget;

    QFrame *mTopBorder;
    QFrame *mLeftBorder;
    QFrame *mBottomBorder;
    QFrame *mRightBorder;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
