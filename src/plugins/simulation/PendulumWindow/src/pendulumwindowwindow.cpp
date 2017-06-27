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
    mAxesFontPointSize(0),
    mCurrentDataSize(0),
    mTimeValues(0),
    mR0Values(0),
    mQ1Values(0),
    mThetaValues(0)
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

    mTimeLabel->setEnabled(false);
    mTimeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    timeWidget->layout()->addWidget(mTimeLabel);

    mTimeCheckBox = new QCheckBox(timeWidget);

    mTimeCheckBox->setEnabled(false);
    mTimeCheckBox->setText(tr("Auto"));

    connect(mTimeCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(autoMode()));

    timeWidget->layout()->addWidget(mTimeCheckBox);

    mGui->layout->addWidget(timeWidget);

    // Create and add our time slider

    mTimeSlider = new QSlider(this);

    mTimeSlider->setEnabled(false);
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
}

//==============================================================================

void PendulumWindowWindow::initData(const int &pDataSize,
                                    const double &pMinimumTime,
                                    const double &pMaximumTime,
                                    const double &pTimeInterval,
                                    double *pTimeValues, double *pR0Values,
                                    double *pQ1Values, double *pThetaValues)
{
    // Initialise our data

    mCurrentDataSize = 0;

    mTimeValues = pTimeValues;
    mR0Values = pR0Values;
    mQ1Values = pQ1Values;
    mThetaValues = pThetaValues;

    // Get the field module of our default region and do a few things with it

    OpenCMISS::Zinc::Region defaultRegion = mZincContext->getDefaultRegion();

    mFieldModule = defaultRegion.getFieldmodule();

    mFieldModule.beginChange();
        // Declare our stored finite element fields

        mR0 = mFieldModule.createFieldFiniteElement(1);
        mQ1 = mFieldModule.createFieldFiniteElement(1);
        mTheta = mFieldModule.createFieldFiniteElement(1);

        // Defining fields as functions of other fields

        OpenCMISS::Zinc::FieldAdd r = mFieldModule.createFieldAdd(mR0, mQ1);

        // Define cylindrical polar coordinates

        OpenCMISS::Zinc::Field coordinatesData[] = { r, mTheta };
        OpenCMISS::Zinc::FieldConcatenate coordinates = mFieldModule.createFieldConcatenate(2, coordinatesData);

        coordinates.setCoordinateSystemType(OpenCMISS::Zinc::Field::COORDINATE_SYSTEM_TYPE_CYLINDRICAL_POLAR);

        // Define a constant field at the (default rectangular cartesian) origin

        const double rcOriginData[] = { 0.0, 0.0, 0.0 };

        OpenCMISS::Zinc::FieldConstant rcOrigin = mFieldModule.createFieldConstant(3, rcOriginData);

        // Define a field converting the polar coordinates to rectangular
        // cartesian

        OpenCMISS::Zinc::FieldCoordinateTransformation rcCoordinates = mFieldModule.createFieldCoordinateTransformation(coordinates);

        rcCoordinates.setCoordinateSystemType(OpenCMISS::Zinc::Field::COORDINATE_SYSTEM_TYPE_RECTANGULAR_CARTESIAN);

        // Get the difference from rcCoordinates to rcOrigin

        OpenCMISS::Zinc::FieldSubtract delta = mFieldModule.createFieldSubtract(rcCoordinates, rcOrigin);

        // Create a single node with storage for time-varying mR0, mQ1 and
        // mTheta

        OpenCMISS::Zinc::Timesequence timeSequence = mFieldModule.getMatchingTimesequence(pDataSize, pTimeValues);
        OpenCMISS::Zinc::Nodeset nodeSet = mFieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        OpenCMISS::Zinc::Nodetemplate nodeTemplate = nodeSet.createNodetemplate();

        nodeTemplate.defineField(mR0);
        nodeTemplate.defineField(mQ1);
        nodeTemplate.defineField(mTheta);

        nodeTemplate.setTimesequence(mQ1, timeSequence);
        nodeTemplate.setTimesequence(mTheta, timeSequence);

        // Create a single node with the above field definitions

        OpenCMISS::Zinc::Node node = nodeSet.createNode(1, nodeTemplate);

        // Create a single 1D element with only 1D xi coordinates to provide a
        // domain for visualising the coordinates time path

        OpenCMISS::Zinc::Mesh mesh = mFieldModule.findMeshByDimension(1);
        OpenCMISS::Zinc::Elementtemplate elementTemplate = mesh.createElementtemplate();

        elementTemplate.setElementShapeType(OpenCMISS::Zinc::Element::SHAPE_TYPE_LINE);

        mesh.createElement(1, elementTemplate);
    mFieldModule.endChange();

    mFieldModule.beginChange();
        // Create a field looking up the node coordinates at time as a function
        // of element xi
        // Note: Zinc has a known defect in that the xi field doesn't appear
        //       until change caching ends, hence the need to call endChange()
        //       and beginChange() above, to get things to work as expected...

        OpenCMISS::Zinc::Field xi = mFieldModule.findFieldByName("xi");
        OpenCMISS::Zinc::FieldComponent xi1 = mFieldModule.createFieldComponent(xi, 1);

        // Fixed scale factor to work for the entire range of times
        // Note: if we are reading times during solution, we could dynamically
        //       change it (and the fine tessellation below)...

        const double constantData[] = { 100.0 };

        OpenCMISS::Zinc::FieldConstant fieldConstant = mFieldModule.createFieldConstant(1, constantData);
        OpenCMISS::Zinc::FieldMultiply xi1Time = mFieldModule.createFieldMultiply(xi1, fieldConstant);

        // xiCoordinates returns node's value of rcCoordinates at the current
        // time on any other domain

        OpenCMISS::Zinc::FieldNodeLookup nodeCoordinates = mFieldModule.createFieldNodeLookup(rcCoordinates, node);

        // xi1TimeNodeCoordinates converts the time variation to be spatial,
        // showing the values of nodeCoordinates at xi1Time

        OpenCMISS::Zinc::FieldTimeLookup xi1TimeNodeCoordinates = mFieldModule.createFieldTimeLookup(nodeCoordinates, xi1Time);

        // Assign parameters at the node for the above fields

        mFieldCache = mFieldModule.createFieldcache();

        mFieldCache.setNode(node);
    mFieldModule.endChange();

    // Use a fine tessellation with as many divisions as time steps, so that we
    // visualise the time path of coordinates on the element with sufficient
    // resolution

    OpenCMISS::Zinc::Scene scene = defaultRegion.getScene();
    OpenCMISS::Zinc::Tessellationmodule tessellationModule = scene.getTessellationmodule();
    OpenCMISS::Zinc::Tessellation tessellation = tessellationModule.createTessellation();

    const int tessellationData[] = { pDataSize };

    tessellation.setMinimumDivisions(1, tessellationData);

    // Also increase the circle divisions quality of the default points
    // tessellation (so the cylinder used for the weights looks better than the
    // default 12 divisions)

    tessellationModule.getDefaultPointsTessellation().setCircleDivisions(36);

    // Set the range of valid times in the default time keeper

    OpenCMISS::Zinc::Timekeepermodule timeKeeperModule = mZincContext->getTimekeepermodule();

    mTimeKeeper = timeKeeperModule.getDefaultTimekeeper();

    mTimeKeeper.setMinimumTime(pMinimumTime);
    mTimeKeeper.setMaximumTime(pMaximumTime);

    mTimeSlider->setMinimum(pMinimumTime/pTimeInterval);
    mTimeSlider->setMaximum(pMaximumTime/pTimeInterval);

    updateScene(pTimeValues[0]);

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

    // Make sure that all of our Zinc scene is visible

    mZincWidget->viewAll();
}

//==============================================================================

void PendulumWindowWindow::addData(const int &pCurrentDataSize)
{
    // Assign the time-varying parameters for mR0, mQ1 and mTheta

    mFieldModule.beginChange();
        for (int i = mCurrentDataSize; i < pCurrentDataSize; ++i) {
            mFieldCache.setTime(mTimeValues[i]);

            mR0.assignReal(mFieldCache, 1, mR0Values+i);
            mQ1.assignReal(mFieldCache, 1, mQ1Values+i);
            mTheta.assignReal(mFieldCache, 1, mThetaValues+i);
        }
    mFieldModule.endChange();

    mZincWidget->viewAll();

    mCurrentDataSize = pCurrentDataSize;

    // Enable/disable our time-related widgets

    mTimeLabel->setEnabled(pCurrentDataSize);
    mTimeCheckBox->setEnabled(pCurrentDataSize);
    mTimeSlider->setEnabled(pCurrentDataSize);

    mTimeSlider->setValue(pCurrentDataSize);
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

    double time = 0.01*pTime;

    mTimeLabel->setText(tr("Time: %1 s").arg(time));

    mTimeKeeper.setTime(time);
}

//==============================================================================

void PendulumWindowWindow::updateScene()
{
    // Update our scene

    int value = mTimeSlider->value();

    if (value == mTimeSlider->maximum())
        value = 0;
    else
        ++value;

    mTimeSlider->setValue(value);
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
