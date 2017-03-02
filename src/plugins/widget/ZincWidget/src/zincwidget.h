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

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "zincwidgetglobal.h"

//==============================================================================

#include <QOpenGLWidget>

//==============================================================================

#include "opencmiss/zinc/sceneviewer.hpp"

//==============================================================================

namespace OpenCMISS {
namespace Zinc {
    class Context;
}   // namespace Zinc
}   // namespace OpenCMISS

//==============================================================================

namespace OpenCOR {
namespace ZincWidget {

//==============================================================================

class ZincWidget;

//==============================================================================

class ZincWidgetSceneViewerCallback : public OpenCMISS::Zinc::Sceneviewercallback
{
public:
    explicit ZincWidgetSceneViewerCallback(ZincWidget *pZincWidget);

private:
    ZincWidget *mZincWidget;

    virtual void operator()(const OpenCMISS::Zinc::Sceneviewerevent &pSceneViewerEvent);
};

//==============================================================================

class ZINCWIDGET_EXPORT ZincWidget : public QOpenGLWidget,
                                     public Core::CommonWidget
{
    Q_OBJECT

public:
    enum ProjectionMode {
        Invalid,
        Parallel,
        Perspective
    };

    explicit ZincWidget(const QString &pName, QWidget *pParent);
    ~ZincWidget();

    ProjectionMode projectionMode() const;
    void setProjectionMode(const ProjectionMode &pProjectionMode);

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int pWidth, int pHeight);

    virtual QSize sizeHint() const;

private:
    OpenCMISS::Zinc::Context *mContext;
    OpenCMISS::Zinc::Sceneviewer mSceneViewer;
    OpenCMISS::Zinc::Sceneviewernotifier mSceneViewerNotifier;

    ZincWidgetSceneViewerCallback mZincWidgetSceneViewerCallback;
};

//==============================================================================

}   // namespace ZincWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
