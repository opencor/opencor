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

#include <QTimer>

//==============================================================================

#include "opencmiss/zinc/context.hpp"

//==============================================================================

namespace OpenCOR {
namespace ZincWidget {

//==============================================================================

ZincWidgetSceneViewerCallback::ZincWidgetSceneViewerCallback(ZincWidget *pZincWidget) :
    mZincWidget(pZincWidget)
{
}

//==============================================================================

void ZincWidgetSceneViewerCallback::operator()(const OpenCMISS::Zinc::Sceneviewerevent &pSceneViewerEvent)
{
    // Ask our Zinc widget to update itself if repainting is required

    if (pSceneViewerEvent.getChangeFlags() & OpenCMISS::Zinc::Sceneviewerevent::CHANGE_FLAG_REPAINT_REQUIRED)
        QTimer::singleShot(0, mZincWidget, SLOT(update()));
}

//==============================================================================

ZincWidget::ZincWidget(const QString &pName, QWidget *pParent) :
    QOpenGLWidget(pParent),
    Core::CommonWidget(this),
    mZincWidgetSceneViewerCallback(this)
{
    // Create our context

    mContext = new OpenCMISS::Zinc::Context(pName.toUtf8().constData());
}

//==============================================================================

ZincWidget::~ZincWidget()
{
    // Delete some internal objects

    delete mContext;
}

//==============================================================================

ZincWidget::ProjectionMode ZincWidget::projectionMode()
{
    // Return our project mode

    switch (mSceneViewer.getProjectionMode()) {
    case OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PARALLEL:
        return Parallel;
    case OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PERSPECTIVE:
        return Perspective;
    default:
        return Invalid;
    }
}

//==============================================================================

void ZincWidget::setProjectionMode(const ProjectionMode &pProjectionMode)
{
    // Set our project mode

    switch (pProjectionMode) {
    case Parallel:
        mSceneViewer.setProjectionMode(OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PARALLEL);

        break;
    case Perspective:
        mSceneViewer.setProjectionMode(OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PERSPECTIVE);

        break;
    default:
        // Invalid, so do nothing

        ;
    }
}

//==============================================================================

void ZincWidget::initializeGL()
{
    // Create a scene viewer and have it have the same OpenGL properties as
    // QOpenGLWidget

    mSceneViewer = mContext->getSceneviewermodule().createSceneviewer(OpenCMISS::Zinc::Sceneviewer::BUFFERING_MODE_DOUBLE,
                                                                      OpenCMISS::Zinc::Sceneviewer::STEREO_MODE_DEFAULT);

    mSceneViewer.setProjectionMode(OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PERSPECTIVE);
    mSceneViewer.setViewportSize(width(), height());

    // Further customise our scene viewer

    mSceneViewer.setScene(mContext->getDefaultRegion().getScene());
    mSceneViewer.setScenefilter(mContext->getScenefiltermodule().getDefaultScenefilter());

    // Get ourselves a scene viewer notifier and set to it our callback class

    mSceneViewerNotifier = mSceneViewer.createSceneviewernotifier();

    mSceneViewerNotifier.setCallback(mZincWidgetSceneViewerCallback);

    // We are all set, so view all of our scene viewer

    mSceneViewer.viewAll();
}

//==============================================================================

void ZincWidget::paintGL()
{
    // Have our scene viewer render its scene

    mSceneViewer.renderScene();
}

//==============================================================================

void ZincWidget::resizeGL(int pWidth, int pHeight)
{
    // Have our scene viewer resize its viewport

    mSceneViewer.setViewportSize(pWidth, pHeight);
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
