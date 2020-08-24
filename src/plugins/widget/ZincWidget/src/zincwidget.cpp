/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Zinc widget
//==============================================================================

#include "zincwidget.h"

//==============================================================================

#include <QMenu>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QTimer>

//==============================================================================

#include <array>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/result.hpp"
#include "zincend.h"

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

    if ((pSceneViewerEvent.getChangeFlags() & OpenCMISS::Zinc::Sceneviewerevent::CHANGE_FLAG_REPAINT_REQUIRED) != 0) {
        QTimer::singleShot(0, mZincWidget, QOverload<>::of(&ZincWidget::update));
    }
}

//==============================================================================

ZincWidget::ZincWidget(QWidget *pParent) :
    QOpenGLWidget(pParent),
    Core::CommonWidget(this),
    mZincWidgetSceneViewerCallback(this)
{
}

//==============================================================================

void ZincWidget::reset()
{
    // Stop forwarding the fact that our context is going to be destroyed

    disconnect(QOpenGLWidget::context(), &QOpenGLContext::aboutToBeDestroyed,
               this, &ZincWidget::contextAboutToBeDestroyed);

    // Get people to reset things by letting them know that our OpenGL context
    // is about to be destroyed

    emit contextAboutToBeDestroyed();

    // (Re)initialise OpenGL

    QTimer::singleShot(0, this, &ZincWidget::initializeGL);
}

//==============================================================================

OpenCMISS::Zinc::Context ZincWidget::context() const
{
    // Return our context

    return mContext;
}

//==============================================================================

void ZincWidget::setContext(const OpenCMISS::Zinc::Context &pContext)
{
    // Set our context

    mContext = pContext;

    // Create our scene viewer and have it have the same OpenGL properties as
    // QOpenGLWidget

    mSceneViewer = mContext.getSceneviewermodule().createSceneviewer(OpenCMISS::Zinc::Sceneviewer::BUFFERING_MODE_DOUBLE,
                                                                     OpenCMISS::Zinc::Sceneviewer::STEREO_MODE_DEFAULT);

    mSceneViewer.setProjectionMode(OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PERSPECTIVE);

    checkDevicePixelRatio(true);

    // Further customise our scene viewer

    mSceneViewer.setScene(mContext.getDefaultRegion().getScene());
    mSceneViewer.setScenefilter(mContext.getScenefiltermodule().getDefaultScenefilter());

    // Use a white background by default

    std::array<double, 4> backgroundColor = { 1.0, 1.0, 1.0, 1.0 };

    mSceneViewer.setBackgroundColourRGBA(backgroundColor.data());

    // Get ourselves a scene viewer notifier and set its callback to our
    // callback class

    mSceneViewerNotifier = mSceneViewer.createSceneviewernotifier();

    mSceneViewerNotifier.setCallback(mZincWidgetSceneViewerCallback);

    // We are all set, so view all of our scene viewer

    mSceneViewer.viewAll();
}

//==============================================================================

QMenu * ZincWidget::contextMenu() const
{
    // Return our context menu

    return mContextMenu;
}

//==============================================================================

void ZincWidget::setContextMenu(QMenu *pContextMenu)
{
    // Set our context menu

    mContextMenu = pContextMenu;
}

//==============================================================================

OpenCMISS::Zinc::Sceneviewer ZincWidget::sceneViewer() const
{
    // Return our scene viewer

    return mSceneViewer;
}

//==============================================================================

ZincWidget::ProjectionMode ZincWidget::projectionMode()
{
    // Return our projection mode

    OpenCMISS::Zinc::Sceneviewer::ProjectionMode projectionMode = mSceneViewer.getProjectionMode();

    if (projectionMode == OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PARALLEL) {
        return ProjectionMode::Parallel;
    }

    if (projectionMode == OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PERSPECTIVE) {
        return ProjectionMode::Perspective;
    }

    return ProjectionMode::Invalid;
}

//==============================================================================

void ZincWidget::setProjectionMode(ProjectionMode pProjectionMode)
{
    // Set our projection mode

    switch (pProjectionMode) {
    case ProjectionMode::Parallel:
        mSceneViewer.setProjectionMode(OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PARALLEL);

        break;
    case ProjectionMode::Perspective:
        mSceneViewer.setProjectionMode(OpenCMISS::Zinc::Sceneviewer::PROJECTION_MODE_PERSPECTIVE);

        break;
    case ProjectionMode::Invalid:
        // Invalid, so do nothing

        break;
    }
}

//==============================================================================

int ZincWidget::viewParameters(double *pEye, double *pLookAt, double *pUp,
                               double &pAngle)
{
    // Return our view parameters

    int res = mSceneViewer.getLookatParameters(pEye, pLookAt, pUp);

    if (res == OpenCMISS::Zinc::Result::RESULT_OK) {
        pAngle = mSceneViewer.getViewAngle();
    }

    return res;
}

//==============================================================================

void ZincWidget::setViewParameters(double *pEye, double *pLookAt, double *pUp,
                                   double &pAngle)
{
    // Set our view parameters

    mSceneViewer.beginChange();
        mSceneViewer.setLookatParametersNonSkew(pEye, pLookAt, pUp);
        mSceneViewer.setViewAngle(pAngle);
    mSceneViewer.endChange();
}

//==============================================================================

OpenCMISS::Zinc::Scenefilter ZincWidget::sceneFilter()
{
    // Return our scene filter

    return mSceneViewer.getScenefilter();
}

//==============================================================================

void ZincWidget::setSceneFilter(const OpenCMISS::Zinc::Scenefilter &pSceneFilter)
{
    // Set our scene filter

    mSceneViewer.setScenefilter(pSceneFilter);
}

//==============================================================================

double ZincWidget::tumbleRate()
{
    // Return our tumble rate

    return mSceneViewer.getTumbleRate();
}

//==============================================================================

void ZincWidget::setTumbleRate(double pTumbleRate)
{
    // Set our tumble rate

    mSceneViewer.setTumbleRate(pTumbleRate);
}

//==============================================================================

int ZincWidget::project(double *pInCoordinates, double *pOutCoordinates)
{
    // Project the given point in global coordinates into window pixel
    // coordinates with the origin at the window's top left pixel
    // Note: the Z pixel coordinate is a depth, which is mapped so that -1 is on
    //       the far clipping plane and +1 is on the near clipping plane...

    return mSceneViewer.transformCoordinates(OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WORLD,
                                             OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WINDOW_PIXEL_TOP_LEFT,
                                             OpenCMISS::Zinc::Scene(), pInCoordinates, pOutCoordinates);
}

//==============================================================================

int ZincWidget::unproject(double *pInCoordinates, double *pOutCoordinates)
{
    // Unproject the given point in window pixel coordinates where the origin is
    // at the window's top left pixel into global coordinates
    // Note: the Z pixel coordinate is a depth, which is mapped so that -1 is on
    //       the far clipping plane, and +1 is on the near clipping plane...

    return mSceneViewer.transformCoordinates(OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WINDOW_PIXEL_TOP_LEFT,
                                             OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WORLD,
                                             OpenCMISS::Zinc::Scene(), pInCoordinates, pOutCoordinates);
}

//==============================================================================

void ZincWidget::viewAll()
{
    // View all of our scene viewer

    mSceneViewer.viewAll();
}

//==============================================================================

double ZincWidget::fps() const
{
    // Return our FPS

    return mFps;
}

//==============================================================================

void ZincWidget::checkDevicePixelRatio(bool pForceSettingViewportSize)
{
    // Check our device pixel ratio

    int newDevicePixelRatio = devicePixelRatio();
    bool hasNewDevicePixelRatio = newDevicePixelRatio != mDevicePixelRatio;

    if (pForceSettingViewportSize || hasNewDevicePixelRatio) {
        // Keep track of our new device pixel ratio and update the viewport size
        // of our scene viewer

        mDevicePixelRatio = newDevicePixelRatio;

        mSceneViewer.setViewportSize(newDevicePixelRatio*width(), newDevicePixelRatio*height());

        // Ask ourselves to resize so that we can properly take advantage of our
        // new device pixel ratio

        if (!pForceSettingViewportSize && hasNewDevicePixelRatio) {
            QResizeEvent event(size(), QSize());

            resizeEvent(&event);
        }

        // Let people know that our device pixel ratio has changed, if needed

        if (hasNewDevicePixelRatio) {
            emit devicePixelRatioChanged(newDevicePixelRatio);
        }
    }
}

//==============================================================================

void ZincWidget::resetFps()
{
    // Reset our FPS internals

    mNbOfFrames = 0;

    mFpsClock.start();
}

//==============================================================================

void ZincWidget::initializeGL()
{
    // Reset our FPS

    resetFps();

    // Forward the fact that our context is going to be destroyed

    connect(QOpenGLWidget::context(), &QOpenGLContext::aboutToBeDestroyed,
            this, &ZincWidget::contextAboutToBeDestroyed);

    // Let people know that our graphics is initialised

    emit graphicsInitialized();
}

//==============================================================================

void ZincWidget::paintGL()
{
    // Make sure that we are using the correct device pixel ratio

    checkDevicePixelRatio();

    // Have our scene viewer render its scene

    mSceneViewer.renderScene();

    // Update our FPS, if needed

    ++mNbOfFrames;

    int fpsClockElapsed = mFpsClock.elapsed();

    if (fpsClockElapsed >= 1000) {
        mFps = 1000.0*mNbOfFrames/fpsClockElapsed;

        resetFps();
    }
}

//==============================================================================

void ZincWidget::resizeGL(int pWidth, int pHeight)
{
    Q_UNUSED(pWidth)
    Q_UNUSED(pHeight)

    // Update the viewport size of our scene viewer by forcing our device pixel
    // ratio to be checked

    checkDevicePixelRatio(true);
}

//==============================================================================

OpenCMISS::Zinc::Sceneviewerinput::ButtonType ZincWidget::buttonMap(const Qt::MouseButton &pButton) const
{
    // Map the given button to a Zinc button

    if (pButton == Qt::LeftButton) {
        return OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_LEFT;
    }

    if (pButton == Qt::MiddleButton) {
        return OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_MIDDLE;
    }

    if (pButton == Qt::XButton1) {
        return OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_SCROLL_DOWN;
    }

    if (pButton == Qt::XButton2) {
        return OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_SCROLL_UP;
    }

    if (pButton == Qt::RightButton) {
        return OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_RIGHT;
    }

    return OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_INVALID;
}

//==============================================================================

OpenCMISS::Zinc::Sceneviewerinput::ModifierFlags ZincWidget::modifierMap(const Qt::KeyboardModifiers &pModifiers) const
{
    // Map the given modifiers to Zinc modifiers

    OpenCMISS::Zinc::Sceneviewerinput::ModifierFlags res = OpenCMISS::Zinc::Sceneviewerinput::MODIFIER_FLAG_NONE;

    if ((pModifiers & Qt::ShiftModifier) != 0) {
        res |= OpenCMISS::Zinc::Sceneviewerinput::MODIFIER_FLAG_SHIFT;
    }

    if ((pModifiers & Qt::ControlModifier) != 0)  {
        res |= OpenCMISS::Zinc::Sceneviewerinput::MODIFIER_FLAG_CONTROL;
    }

    if ((pModifiers & Qt::AltModifier) != 0)  {
        res |= OpenCMISS::Zinc::Sceneviewerinput::MODIFIER_FLAG_ALT;
    }

    return res;
}

//==============================================================================

void ZincWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Get our scene viewer to handle the given mouse move event

    OpenCMISS::Zinc::Sceneviewerinput sceneInput = mSceneViewer.createSceneviewerinput();

    sceneInput.setEventType(OpenCMISS::Zinc::Sceneviewerinput::EVENT_TYPE_MOTION_NOTIFY);

    if (pEvent->type() == QEvent::Leave) {
        sceneInput.setPosition(-1, -1);
    } else {
        sceneInput.setPosition(pEvent->x(), pEvent->y());
    }

    mSceneViewer.processSceneviewerinput(sceneInput);

    // The mouse has moved, so we definitely won't need to show our context menu

    mNeedContextMenu = false;
}

//==============================================================================

void ZincWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Get our scene viewer to handle the given mouse press event

    OpenCMISS::Zinc::Sceneviewerinput sceneInput = mSceneViewer.createSceneviewerinput();

    sceneInput.setButtonType(buttonMap(pEvent->button()));
    sceneInput.setEventType(OpenCMISS::Zinc::Sceneviewerinput::EVENT_TYPE_BUTTON_PRESS);
    sceneInput.setModifierFlags(modifierMap(pEvent->modifiers()));
    sceneInput.setPosition(pEvent->x(), pEvent->y());

    mSceneViewer.processSceneviewerinput(sceneInput);

    // Check whether we might need to show our context menu

    mNeedContextMenu = pEvent->button() == Qt::RightButton;
}

//==============================================================================

void ZincWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Get our scene viewer to handle the given mouse release event

    OpenCMISS::Zinc::Sceneviewerinput sceneInput = mSceneViewer.createSceneviewerinput();

    sceneInput.setButtonType(buttonMap(pEvent->button()));
    sceneInput.setEventType(OpenCMISS::Zinc::Sceneviewerinput::EVENT_TYPE_BUTTON_RELEASE);
    sceneInput.setPosition(pEvent->x(), pEvent->y());

    mSceneViewer.processSceneviewerinput(sceneInput);

    // Show our context menu, if still needed and possible

    if (mNeedContextMenu && (mContextMenu != nullptr)) {
        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void ZincWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Get our scene viewer to handle the given wheel event by first pretending
    // to press the right mouse button

    OpenCMISS::Zinc::Sceneviewerinput sceneInput = mSceneViewer.createSceneviewerinput();

    sceneInput.setButtonType(OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_RIGHT);
    sceneInput.setEventType(OpenCMISS::Zinc::Sceneviewerinput::EVENT_TYPE_BUTTON_PRESS);
    sceneInput.setPosition(pEvent->x(), pEvent->y());

    mSceneViewer.processSceneviewerinput(sceneInput);

    // Next, pretend to move the right mouse button

    sceneInput = mSceneViewer.createSceneviewerinput();

    sceneInput.setEventType(OpenCMISS::Zinc::Sceneviewerinput::EVENT_TYPE_MOTION_NOTIFY);
    sceneInput.setPosition(pEvent->x(), int(pEvent->y()-0.1*pEvent->delta()));

    mSceneViewer.processSceneviewerinput(sceneInput);

    // Finally, pretend to release the right mouse button

    sceneInput = mSceneViewer.createSceneviewerinput();

    sceneInput.setButtonType(OpenCMISS::Zinc::Sceneviewerinput::BUTTON_TYPE_RIGHT);
    sceneInput.setEventType(OpenCMISS::Zinc::Sceneviewerinput::EVENT_TYPE_BUTTON_RELEASE);
    sceneInput.setPosition(pEvent->x(), pEvent->y());

    mSceneViewer.processSceneviewerinput(sceneInput);
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

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
