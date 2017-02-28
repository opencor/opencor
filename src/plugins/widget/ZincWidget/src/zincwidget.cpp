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
// Zinc widget
//==============================================================================

#include "zincwidget.h"

//==============================================================================

#include "opencmiss/zinc/context.hpp"

//==============================================================================

namespace OpenCOR {
namespace ZincWidget {

//==============================================================================

ZincWidget::ZincWidget(QWidget *pParent) :
    QOpenGLWidget(pParent),
    Core::CommonWidget(this)
{
    // Create our context

    mContext = new OpenCMISS::Zinc::Context("ZincWindowWidget");
}

//==============================================================================

ZincWidget::~ZincWidget()
{
    // Delete some internal objects

    delete mContext;
}

//==============================================================================

QSize ZincWidget::sizeHint() const
{
    // Suggest a default size for ourselves
    // Note: this is critical if we want a docked widget, with ourselves on it,
    //       to have a decent size when docked to the main window...

    return defaultSize(0.15);
}

//==============================================================================

}   // namespace WebViewerWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
