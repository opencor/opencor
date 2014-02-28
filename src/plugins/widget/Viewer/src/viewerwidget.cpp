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
// Viewer widget
//==============================================================================

#include "viewerwidget.h"

//==============================================================================

#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QRectF>

//==============================================================================

#include "qwt_mathml_text_engine.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

ViewerWidget::ViewerWidget(QWidget *pParent) :
    Widget(pParent),
    mMathmlDocument(QwtMathMLDocument()),
    mOneOverMathmlDocumentWidth(0),
    mOneOverMathmlDocumentHeight(0),
    mContents(QString()),
    mOptimiseFontSize(true)
{
}

//==============================================================================

QString ViewerWidget::contents() const
{
    // Return our contents

    return mContents;
}

//==============================================================================

void ViewerWidget::setContents(const QString &pContents)
{
    // Set our contents

    if (!pContents.compare(mContents))
        return;

    // Keep track of our contents

    mContents = pContents;

    // Determine (the inverse of) the size of our contents when rendered using a
    // font size of 100 points
    // Note: when setting the contents, QwtMathMLDocument recomputes its layout.
    //       Now, because we want the contents to be rendered as optimally as
    //       possible, we use a big font size, so that when we actually need to
    //       render the contents (see paintEvent()), we can do so optimally...

    mMathmlDocument.setBaseFontPointSize(100);
    mMathmlDocument.setContent(mContents);

    QSizeF mathmlDocumentSize = mMathmlDocument.size();

    mOneOverMathmlDocumentWidth  = 1.0/mathmlDocumentSize.width();
    mOneOverMathmlDocumentHeight = 1.0/mathmlDocumentSize.height();

    // Update ourselves

    update();
}

//==============================================================================

bool ViewerWidget::optimiseFontSize() const
{
    // Return whether we optimise our font size

    return mOptimiseFontSize;
}

//==============================================================================

void ViewerWidget::setOptimiseFontSize(const bool &pOptimiseFontSize)
{
    // Keep track of whether we should optimise our font size

    if (pOptimiseFontSize == mOptimiseFontSize)
        return;

    mOptimiseFontSize = pOptimiseFontSize;

    // Update ourselves

    update();
}

//==============================================================================

void ViewerWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter painter(this);

    // Clear our background

    QRectF rect = pEvent->rect();
    QColor backgroundColor = QColor(palette().color(QPalette::Base));

    painter.fillRect(rect, backgroundColor);

    // Customise our MathML document
    // Note: to go for 100% of the 'optimal' font size might result in the edges
    //       of the contents being clipped on Windows (compared to Linux and OS
    //       X) or in some cases on Linux and OS X (e.g. if the contents
    //       includes a square root), hence we go for 75% of the 'optimal' font
    //       size instead...

    mMathmlDocument.setBackgroundColor(backgroundColor);
    mMathmlDocument.setForegroundColor(QColor(palette().color(QPalette::Text)));

    mMathmlDocument.setBaseFontPointSize(mOptimiseFontSize?
                                             qRound(75.0*qMin(mOneOverMathmlDocumentWidth*width(),
                                                              mOneOverMathmlDocumentHeight*height())):
                                             font().pointSize());

    // Render our contents

    QSizeF mathmlDocumentSize = mMathmlDocument.size();

    mMathmlDocument.paint(&painter, QPointF(0.5*(rect.width()-mathmlDocumentSize.width()),
                                            0.5*(rect.height()-mathmlDocumentSize.height())));

    // Accept the event

    pEvent->accept();
}

//==============================================================================

QSize ViewerWidget::minimumSizeHint() const
{
    // Suggest a default minimum size for our viewer widget

    return defaultSize(0.03);
}

//==============================================================================

QSize ViewerWidget::sizeHint() const
{
    // Suggest a default size for our viewer widget
    // Note: this is critical if we want a docked widget, with a viewer widget
    //       on it, to have a decent size when docked to the main window...

    return defaultSize(0.1);
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
