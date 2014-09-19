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
// CellML annotation view metadata web view widget
//==============================================================================

#include "cellmlannotationviewmetadatawebviewwidget.h"

//==============================================================================

#include <QEvent>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataWebViewWidget::CellmlAnnotationViewMetadataWebViewWidget(QWidget *pParent) :
    QWebView(pParent),
    mResettingCursor(false)
{
    // Customise ourselves

    setAcceptDrops(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

//==============================================================================

bool CellmlAnnotationViewMetadataWebViewWidget::event(QEvent *pEvent)
{
    // Override the change of the cursor when hovering some text

    if (mResettingCursor) {
        return true;
    } else if (    (pEvent->type() == QEvent::CursorChange)
        &&  (cursor().shape() == Qt::IBeamCursor)
        && !mResettingCursor) {
        mResettingCursor = true;

        setCursor(Qt::ArrowCursor);

        mResettingCursor = false;

        return true;
    } else {
        return QWebView::event(pEvent);
    }
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
