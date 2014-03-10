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

#include <Qt>

//==============================================================================

#include <QAction>
#include <QCursor>
#include <QIcon>
#include <QMenu>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QPoint>
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
    mValidContents(true)
{
    // Create our context menu

    mContextMenu = new QMenu(this);

    mOptimiseFontSizeAction = new QAction(this);

    mOptimiseFontSizeAction->setCheckable(true);
    mOptimiseFontSizeAction->setChecked(true);

    mContextMenu->addAction(mOptimiseFontSizeAction);

    connect(mOptimiseFontSizeAction, SIGNAL(triggered()),
            this, SLOT(update()));

    // We want a context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Retranslate ourselves, so that our action is properly set

    retranslateUi();
}

//==============================================================================

void ViewerWidget::retranslateUi()
{
    // Retranslate our actions

    mOptimiseFontSizeAction->setText(tr("Optimise Font Size"));
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

    // Try to set our contents to our MathML document

    if (mMathmlDocument.setContent(pContents)) {
        mContents = pContents;
        mValidContents = true;

        // Determine (the inverse of) the size of our contents when rendered
        // using a font size of 100 points
        // Note: when setting the contents, QwtMathMLDocument recomputes its
        //       layout. Now, because we want the contents to be rendered as
        //       optimally as possible, we use a big font size, so that when we
        //       actually need to render the contents (see paintEvent()), we can
        //       do so optimally...

        mMathmlDocument.setBaseFontPointSize(100);

        QSizeF mathmlDocumentSize = mMathmlDocument.size();

        mOneOverMathmlDocumentWidth  = 1.0/mathmlDocumentSize.width();
        mOneOverMathmlDocumentHeight = 1.0/mathmlDocumentSize.height();
    } else {
        mContents = QString();
        mValidContents = pContents.isEmpty();
    }

    // Update ourselves

    update();
}

//==============================================================================

bool ViewerWidget::validContents() const
{
    // Return whether our contents is valid

    return mValidContents;
}

//==============================================================================

void ViewerWidget::setValidContents(const bool &pValidContents)
{
    // Keep track of whether our contents is valid

    if (pValidContents == mValidContents)
        return;

    mContents = QString();
    mValidContents = pValidContents;

    // Update ourselves

    update();
}

//==============================================================================

bool ViewerWidget::optimiseFontSize() const
{
    // Return whether we optimise our font size

    return mOptimiseFontSizeAction->isChecked();
}

//==============================================================================

void ViewerWidget::setOptimiseFontSize(const bool &pOptimiseFontSize)
{
    // Keep track of whether we should optimise our font size

    if (pOptimiseFontSize == mOptimiseFontSizeAction->isChecked())
        return;

    mOptimiseFontSizeAction->setChecked(pOptimiseFontSize);

    // Update ourselves

    update();
}

//==============================================================================

void ViewerWidget::paintEvent(QPaintEvent *pEvent)
{
    // Make sure that we have a width and height

    QRectF rect = pEvent->rect();

    if (!rect.width() || !rect.height())
        return;

    // Clear our background

    QPainter painter(this);
    QColor backgroundColor = QColor(palette().color(QPalette::Base));

    painter.fillRect(rect, backgroundColor);

    // Render our contents or show a warning sign, depending on whether our
    // contents is valid

    if (mValidContents) {
        // Customise our MathML document
        // Note: to go for 100% of the 'optimal' font size might result in the
        //       edges of the contents being clipped on Windows (compared to
        //       Linux and OS X) or in some cases on Linux and OS X (e.g. if the
        //       contents includes a square root), hence we go for 75% of the
        //       'optimal' font size instead...

        mMathmlDocument.setBackgroundColor(backgroundColor);
        mMathmlDocument.setForegroundColor(QColor(palette().color(QPalette::Text)));

        mMathmlDocument.setBaseFontPointSize(mOptimiseFontSizeAction->isChecked()?
                                                 qRound(75.0*qMin(mOneOverMathmlDocumentWidth*width(),
                                                                  mOneOverMathmlDocumentHeight*height())):
                                                 font().pointSize());

        // Render our contents

        QSizeF mathmlDocumentSize = mMathmlDocument.size();

        mMathmlDocument.paint(&painter, QPointF(0.5*(rect.width()-mathmlDocumentSize.width()),
                                                0.5*(rect.height()-mathmlDocumentSize.height())));
    } else {
        // Our contents is not valid, so render a stretched warning icon in our
        // center

        QIcon icon = QIcon(":Viewer_warning");
        QSize iconSize = icon.availableSizes().first();

        int painterRectWidth = iconSize.width();
        int painterRectHeight = iconSize.height();

        if (rect.width() < rect.height())
            painterRectHeight *= rect.height()/rect.width();
        else
            painterRectWidth *= rect.width()/rect.height();

        if (painterRectHeight % 2)
            ++painterRectHeight;

        if (painterRectWidth % 2)
            ++painterRectWidth;

        QRect painterRect = QRect(0, 0, painterRectWidth, painterRectHeight);
        // Note: both painterRectWidth and painterRectHeight have even values.
        //       This ensures that when we get resized, then the warning icon
        //       remains nicely centered (while it would otherwise shifts to the
        //       left/right or top/bottom, depending on our width or height,
        //       respectively...

        painter.setWindow(painterRect);

        icon.paint(&painter, painterRect);
    }

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

void ViewerWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Show our custom context menu

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
