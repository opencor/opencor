/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Pendulum window
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "pendulumwindowwindow.h"
#include "widget.h"
#include "zincwidget.h"

//==============================================================================

#include "ui_pendulumwindowwindow.h"

//==============================================================================

#include <QCheckBox>
#include <QDir>
#include <QLabel>

//==============================================================================

#include "opencmiss/zinc/fieldarithmeticoperators.hpp"
#include "opencmiss/zinc/fieldcomposite.hpp"
#include "opencmiss/zinc/fieldconstant.hpp"
#include "opencmiss/zinc/fieldcoordinatetransformation.hpp"
#include "opencmiss/zinc/fieldfiniteelement.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/fieldtime.hpp"
#include "opencmiss/zinc/fieldvectoroperators.hpp"

//==============================================================================

namespace OpenCOR {
namespace PendulumWindow {

//==============================================================================

PendulumWindowWindow::PendulumWindowWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::PendulumWindowWindow),
    mZincContext(0),
    mZincSceneViewerDescription(0),
    mAxesFontPointSize(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and add a Zinc widget

    mZincWidget = new ZincWidget::ZincWidget(this);

    connect(mZincWidget, SIGNAL(contextAboutToBeDestroyed()),
            this, SLOT(createAndSetZincContext()));
    connect(mZincWidget, SIGNAL(graphicsInitialized()),
            this, SLOT(graphicsInitialized()));
    connect(mZincWidget, SIGNAL(devicePixelRatioChanged(const int &)),
            this, SLOT(devicePixelRatioChanged(const int &)));

    mGui->layout->addWidget(new Core::BorderedWidget(mZincWidget,
                                                     true, true, true, true));

    // Create and add our time label and check box

    Core::Widget *timeWidget = new Core::Widget(QSize(), this);

    timeWidget->createLayout(Core::Widget::HorizontalLayout);

    mTimeLabel = new QLabel(timeWidget);

    mTimeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    timeWidget->layout()->addWidget(mTimeLabel);

    mTimeCheckBox = new QCheckBox(timeWidget);

    mTimeCheckBox->setText(tr("Auto"));

    connect(mTimeCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(autoMode()));

    timeWidget->layout()->addWidget(mTimeCheckBox);

    mGui->layout->addWidget(timeWidget);

    // Create and add our time slider

    mTimeSlider = new QSlider(this);

    mTimeSlider->setOrientation(Qt::Horizontal);

    connect(mTimeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(updateScene(const int &)));

    mGui->layout->addWidget(mTimeSlider);

    // Create and set our Zinc context

    createAndSetZincContext();

    // Customise our timer

    connect(&mTimer, SIGNAL(timeout()),
            this, SLOT(updateScene()));
}

//==============================================================================

PendulumWindowWindow::~PendulumWindowWindow()
{
    // Delete some internal objects

    delete mZincContext;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PendulumWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);
}

//==============================================================================

void PendulumWindowWindow::createAndSetZincContext()
{
    // Keep track of our current scene viewer's description

    mZincSceneViewerDescription = mZincWidget->sceneViewer().writeDescription();

    // Create and set our Zinc context

    mZincContext = new OpenCMISS::Zinc::Context("PendulumWindowWindow");

    mZincContext->getMaterialmodule().defineStandardMaterials();
    mZincContext->getGlyphmodule().defineStandardGlyphs();

    mZincWidget->setContext(mZincContext);

    // Some data

    #include "data.inc"

    // Get the field module of our default region and do a few things with it

    OpenCMISS::Zinc::Region defaultRegion = mZincContext->getDefaultRegion();
    OpenCMISS::Zinc::Fieldmodule fieldModule = defaultRegion.getFieldmodule();

    fieldModule.beginChange();
        // Declare our stored finite element fields

        OpenCMISS::Zinc::FieldFiniteElement r0 = fieldModule.createFieldFiniteElement(1);
        OpenCMISS::Zinc::FieldFiniteElement q1 = fieldModule.createFieldFiniteElement(1);
        OpenCMISS::Zinc::FieldFiniteElement theta = fieldModule.createFieldFiniteElement(1);

        // Defining fields as functions of other fields

        OpenCMISS::Zinc::FieldAdd r = fieldModule.createFieldAdd(r0, q1);

        // Define cylindrical polar coordinates

        OpenCMISS::Zinc::Field coordinatesData[] = { r, theta };
        OpenCMISS::Zinc::FieldConcatenate coordinates = fieldModule.createFieldConcatenate(2, coordinatesData);

        coordinates.setCoordinateSystemType(OpenCMISS::Zinc::Field::COORDINATE_SYSTEM_TYPE_CYLINDRICAL_POLAR);

        // Define a constant field at the (default rectangular cartesian) origin

        const double rcOriginData[] = { 0.0, 0.0, 0.0 };

        OpenCMISS::Zinc::FieldConstant rcOrigin = fieldModule.createFieldConstant(3, rcOriginData);

        // Define a field converting the polar coordinates to rectangular
        // cartesian

        OpenCMISS::Zinc::FieldCoordinateTransformation rcCoordinates = fieldModule.createFieldCoordinateTransformation(coordinates);

        rcCoordinates.setCoordinateSystemType(OpenCMISS::Zinc::Field::COORDINATE_SYSTEM_TYPE_RECTANGULAR_CARTESIAN);

        // Get the difference from rcCoordinates to rcOrigin

        OpenCMISS::Zinc::FieldSubtract delta = fieldModule.createFieldSubtract(rcCoordinates, rcOrigin);

        // Create a single node with storage for constant r0 and time-varying q1
        // and theta

        OpenCMISS::Zinc::Timesequence timeSequence = fieldModule.getMatchingTimesequence(DataSize, timeValues);
        OpenCMISS::Zinc::Nodeset nodeSet = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        OpenCMISS::Zinc::Nodetemplate nodeTemplate = nodeSet.createNodetemplate();

        nodeTemplate.defineField(r0);
        nodeTemplate.defineField(q1);
        nodeTemplate.defineField(theta);

        nodeTemplate.setTimesequence(q1, timeSequence);
        nodeTemplate.setTimesequence(theta, timeSequence);

        // Create a single node with the above field definitions

        OpenCMISS::Zinc::Node node = nodeSet.createNode(1, nodeTemplate);

        // Create a single 1D element with only 1D xi coordinates to provide a
        // domain for visualising the coordinates time path

        OpenCMISS::Zinc::Mesh mesh = fieldModule.findMeshByDimension(1);
        OpenCMISS::Zinc::Elementtemplate elementTemplate = mesh.createElementtemplate();

        elementTemplate.setElementShapeType(OpenCMISS::Zinc::Element::SHAPE_TYPE_LINE);

        mesh.createElement(1, elementTemplate);
    fieldModule.endChange();

    fieldModule.beginChange();
        // Create a field looking up the node coordinates at time as a function
        // of element xi
        // Note: Zinc has a known defect in that the xi field doesn't appear
        //       until change caching ends, hence the need to call endChange()
        //       and beginChange() above, to get things to work as expected...

        OpenCMISS::Zinc::Field xi = fieldModule.findFieldByName("xi");
        OpenCMISS::Zinc::FieldComponent xi1 = fieldModule.createFieldComponent(xi, 1);

        // Fixed scale factor to work for the entire range of times
        // Note: if we are reading times during solution, we could dynamically
        //       change it (and the fine tessellation below)...

        const double constantData[] = { 100.0 };

        OpenCMISS::Zinc::FieldConstant fieldConstant = fieldModule.createFieldConstant(1, constantData);
        OpenCMISS::Zinc::FieldMultiply xi1Time = fieldModule.createFieldMultiply(xi1, fieldConstant);

        // xiCoordinates returns node's value of rcCoordinates at the current
        // time on any other domain

        OpenCMISS::Zinc::FieldNodeLookup nodeCoordinates = fieldModule.createFieldNodeLookup(rcCoordinates, node);

        // xiTimeNodeCoordinates converts the time variation to be spatial,
        // showing the values of nodeCoordinates at xi1Time

        OpenCMISS::Zinc::FieldTimeLookup xi1TimeNodeCoordinates = fieldModule.createFieldTimeLookup(nodeCoordinates, xi1Time);

        // Assign parameters at the node for the above fields

        OpenCMISS::Zinc::Fieldcache fieldCache = fieldModule.createFieldcache();

        fieldCache.setNode(node);

        // First the constant value for r0 at the node

        const double r0Data[] = { r0Value };

        r0.assignReal(fieldCache, 1, r0Data);

        // The above could also be done with FieldFiniteElement's
        // setNodeParameters() method, particularly if we have derivatives and
        // versions next assign the time-varying parameters for q1 and theta,
        // here for all times, but you may do it only up to the times you know,
        // but be sure to adjust the time keeper min/max and current time to
        // match what parameters are properly set

        double q1Data[1];
        double thetaData[1];

        for (int i = 0; i < DataSize; ++i) {
            fieldCache.setTime(timeValues[i]);

            q1Data[0] = q1Values[i];
            thetaData[0] = thetaValues[i];

            q1.assignReal(fieldCache, 1, q1Data);
            theta.assignReal(fieldCache, 1, thetaData);
        }
    fieldModule.endChange();

    // Use a fine tessellation with as many divisions as time steps, so that we
    // visualise the time path of coordinates on the element with sufficient
    // resolution

    OpenCMISS::Zinc::Scene scene = defaultRegion.getScene();
    OpenCMISS::Zinc::Tessellationmodule tessellationModule = scene.getTessellationmodule();
    OpenCMISS::Zinc::Tessellation tessellation = tessellationModule.createTessellation();

    const int tessellationData[] = { DataSize };

    tessellation.setMinimumDivisions(1, tessellationData);

    // Also increase the circle divisions quality of the default points
    // tessellation (so the cylinder used for the weights looks better than the
    // default 12 divisions)

    tessellationModule.getDefaultPointsTessellation().setCircleDivisions(36);

    // Set the range of valid times in the default time keeper

    OpenCMISS::Zinc::Timekeepermodule timeKeeperModule = mZincContext->getTimekeepermodule();

    mTimeKeeper = timeKeeperModule.getDefaultTimekeeper();

    mTimeKeeper.setMinimumTime(timeValues[0]);
    mTimeKeeper.setMaximumTime(timeValues[DataSize-1]);

    mTimeSlider->setMinimum(timeValues[0]);
    mTimeSlider->setMaximum(100*timeValues[DataSize-1]);

    updateScene(timeValues[0]);

    // Now set up some graphics

    scene.beginChange();
        OpenCMISS::Zinc::Materialmodule materialModule = scene.getMaterialmodule();

        // Draw the axes at the origin

        OpenCMISS::Zinc::GraphicsPoints axes = scene.createGraphicsPoints();

        axes.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_POINT);

        OpenCMISS::Zinc::Graphicspointattributes pointAttributes = axes.getGraphicspointattributes();

        double pointAttributesData[] = { 1.0, 1.0, 1.0 };

        pointAttributes.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_AXES_XYZ);
        pointAttributes.setBaseSize(3, pointAttributesData);

        axes.setMaterial(materialModule.findMaterialByName("blue"));

        // Make a thin cylinder glyph representing the string of the pendulum,
        // which starts from rcOrigin extending in the direction and magnitude
        // of delta

        OpenCMISS::Zinc::GraphicsPoints string = scene.createGraphicsPoints();

        string.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        string.setCoordinateField(rcOrigin);

        pointAttributes = string.getGraphicspointattributes();

        pointAttributes.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_CYLINDER);
        pointAttributes.setOrientationScaleField(delta);

        pointAttributesData[0] = 0.0;
        pointAttributesData[1] = 0.05;
        pointAttributesData[2] = 0.05;

        pointAttributes.setBaseSize(3, pointAttributesData);

        pointAttributesData[0] = 1.0;
        pointAttributesData[1] = 0.0;
        pointAttributesData[2] = 0.0;

        pointAttributes.setScaleFactors(3, pointAttributesData);

        string.setMaterial(materialModule.findMaterialByName("silver"));

        // Make a cylinder to represent the weight

        OpenCMISS::Zinc::GraphicsPoints weight = scene.createGraphicsPoints();

        weight.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        weight.setCoordinateField(rcCoordinates);

        pointAttributes = weight.getGraphicspointattributes();

        pointAttributes.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_CYLINDER_SOLID);
        pointAttributes.setOrientationScaleField(delta);

        pointAttributesData[0] = 0.5;
        pointAttributesData[1] = 0.5;
        pointAttributesData[2] = 0.5;

        pointAttributes.setBaseSize(3, pointAttributesData);

        pointAttributesData[0] = 0.0;
        pointAttributesData[1] = 0.0;
        pointAttributesData[2] = 0.0;

        pointAttributes.setScaleFactors(3, pointAttributesData);

        weight.setMaterial(materialModule.findMaterialByName("gold"));

        // Draw the time path of the pendulum coordinates

        OpenCMISS::Zinc::GraphicsLines path = scene.createGraphicsLines();

        path.setCoordinateField(xi1TimeNodeCoordinates);
        path.setTessellation(tessellation);
        path.setMaterial(materialModule.findMaterialByName("grey50"));
    scene.endChange();
}

//==============================================================================

void PendulumWindowWindow::graphicsInitialized()
{
    // Set our 'new' scene viewer's description

    OpenCMISS::Zinc::Sceneviewer sceneViewer = mZincWidget->sceneViewer();

    sceneViewer.readDescription(mZincSceneViewerDescription);

    // Our Zinc widget has had its graphics initialised, so now we can set its
    // background colour

    double backgroundColor[4] = { 1.0, 1.0, 1.0, 1.0 };

    sceneViewer.setBackgroundColourRGBA(backgroundColor);

    // Our initial look at and eye positions, and up vector

    const double lookAtPosition[] = { 5.0, 0.0, 0.0 };
    const double eyePosition[] = { 5.0, 0.0, -25.0 };
    const double upVector[] = { -1.0, 0.0, 0.0 };

    sceneViewer.setLookatPosition(lookAtPosition);
    sceneViewer.setEyePosition(eyePosition);
    sceneViewer.setUpVector(upVector);
}

//==============================================================================

void PendulumWindowWindow::devicePixelRatioChanged(const int &pDevicePixelRatio)
{
    // Update our scene using the given devide pixel ratio

    OpenCMISS::Zinc::Scene scene = mZincContext->getDefaultRegion().getScene();

    scene.beginChange();
        scene.createGraphicsPoints().getGraphicspointattributes().getFont().setPointSize(pDevicePixelRatio*mAxesFontPointSize);
    scene.endChange();
}

//==============================================================================

void PendulumWindowWindow::updateScene(const int &pTime)
{
    // Update our scene

    if (mTimeKeeper.isValid()) {
        double time = 0.01*pTime;

        mTimeLabel->setText(tr("Time: %1 s").arg(time));

        mTimeKeeper.setTime(time);
    }
}

//==============================================================================

void PendulumWindowWindow::updateScene()
{
    // Update our scene

    if (mTimeKeeper.isValid()) {
        int value = mTimeSlider->value();

        if (value == mTimeSlider->maximum())
            value = 0;
        else
            ++value;

        mTimeSlider->setValue(value);
    }
}

//==============================================================================

void PendulumWindowWindow::autoMode()
{
    // Enable/disable our timer

    if (mTimeCheckBox->isChecked())
        mTimer.start();
    else
        mTimer.stop();
}

//==============================================================================

}   // namespace PendulumWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
