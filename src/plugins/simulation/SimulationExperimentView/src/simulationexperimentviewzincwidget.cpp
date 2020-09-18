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
// Zinc window
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "simulationexperimentviewzincwidget.h"
#include "toolbarwidget.h"
#include "zincwidget.h"

//==============================================================================

#include <QCheckBox>
#include <QDir>
#include <QDoubleValidator>
#include <QDragEnterEvent>
#include <QtGui>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QSlider>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/result.hpp"
    #include "opencmiss/zinc/fieldfiniteelement.hpp"
    #include "opencmiss/zinc/streamregion.hpp"
    #include "opencmiss/zinc/spectrum.hpp"
    #include "opencmiss/zinc/fieldtime.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewZincWidget::SimulationExperimentViewZincWidget(QWidget *pParent) :
    Core::Widget(pParent)
{
    mTimeValues = new double(0.);

    QLayout *layout = createLayout();

    mMappingFileLabel = new QLabel(this);
    mMapNodeVariables = new QMap<int, _variable>();
    mMapNodeValues = new QMap<int, double*>();
    //TODO remove this
    loadMappingFile("please open something, gimme a mapping file !");

    layout->addWidget(mMappingFileLabel);

    // Create and connect our menu actions

    mActionAxes = Core::newAction(true,this);
    mActionPoints = Core::newAction(true,this);
    mActionLines = Core::newAction(true,this);
    mActionSurfaces = Core::newAction(true,this);
    mActionIsosurfaces = Core::newAction(true,this);

    //TODO put this in settings
    mActionAxes->setChecked(true);
    mActionPoints->setChecked(true);
    mActionLines->setChecked(true);
    mActionSurfaces->setChecked(true);
    mActionIsosurfaces->setChecked(true);

    mActionAxes->setText("Axes");
    mActionPoints->setText("Points");
    mActionLines->setText("Lines");
    mActionSurfaces->setText("Surfaces");
    mActionIsosurfaces->setText("Isosurfaces");

    connect(mActionAxes, &QAction::triggered,
            this, &SimulationExperimentViewZincWidget::actionAxesTriggered);
    connect(mActionPoints, &QAction::triggered,
            this, &SimulationExperimentViewZincWidget::actionPointsTriggered);
    connect(mActionLines, &QAction::triggered,
            this, &SimulationExperimentViewZincWidget::actionLinesTriggered);
    connect(mActionSurfaces, &QAction::triggered,
            this, &SimulationExperimentViewZincWidget::actionSurfacesTriggered);
    connect(mActionIsosurfaces, &QAction::triggered,
            this, &SimulationExperimentViewZincWidget::actionIsosurfacesTriggered);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create and populate our context menu

    QMenu *contextMenu = new QMenu(this);

    contextMenu->addAction(mActionAxes);
    contextMenu->addAction(mActionPoints);
    contextMenu->addAction(mActionLines);
    contextMenu->addAction(mActionSurfaces);
    contextMenu->addAction(mActionIsosurfaces);

    // Create and add a Zinc widget

    mZincWidget = new ZincWidget::ZincWidget(this);

    mZincWidget->setContextMenu(contextMenu);

//    connect(mZincWidget, &ZincWidget::ZincWidget::contextAboutToBeDestroyed,
//            this, &SimulationExperimentViewZincWidget::createAndSetZincContext);
    connect(mZincWidget, &ZincWidget::ZincWidget::graphicsInitialized,
            this, &SimulationExperimentViewZincWidget::graphicsInitialized);
    connect(mZincWidget, &ZincWidget::ZincWidget::devicePixelRatioChanged,
            this, &SimulationExperimentViewZincWidget::devicePixelRatioChanged);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    layout->addWidget(new Core::BorderedWidget(mZincWidget,
                                                     false, true, true, true));
#else
    layout->addWidget(mZincWidget);
#endif

    // Create and add our time label and check box
    Core::Widget *timeWidget = new Core::Widget(QSize(), this);

    timeWidget->createLayout(Core::Widget::Layout::Horizontal);

    mTimeLabel = new QLabel(timeWidget);

    mTimeLabel->setEnabled(false);
    mTimeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    timeWidget->layout()->addWidget(mTimeLabel);

    mToolBarWidget = new Core::ToolBarWidget();

        mLogCheckBox = new QCheckBox(timeWidget);

        mLogCheckBox->setEnabled(false);
        mLogCheckBox->setText(tr("Log colors"));

        connect(mLogCheckBox, &QCheckBox::toggled,
                this, &SimulationExperimentViewZincWidget::setSpectrumScale);

        mToolBarWidget->addWidget(mLogCheckBox);

        mLogAmpliLineEdit = new QLineEdit(timeWidget);
        mLogAmpliLineEdit->setValidator(new QDoubleValidator(timeWidget));
        mLogAmpliLineEdit->setEnabled(false);
        mLogAmpliLineEdit->setText("1.00");

        connect(mLogAmpliLineEdit, &QLineEdit::textEdited,
                this, &SimulationExperimentViewZincWidget::changedSpectrumExageration);

        mToolBarWidget->addWidget(mLogAmpliLineEdit);

        QRect availableGeometry = qApp->primaryScreen()->availableGeometry();

        mSpeedWidget = new QwtWheel(mTimeLabel);
            mSpeedWidget->setBorderWidth(0);
            mSpeedWidget->setFixedSize(int(0.07*availableGeometry.width()),
                                       mSpeedWidget->height()/2);
            mSpeedWidget->setFocusPolicy(Qt::NoFocus);
            mSpeedWidget->setRange(1, 50);
            mSpeedWidget->setWheelBorderWidth(0);
            mSpeedWidget->setValue(0);
        connect(mSpeedWidget, &QwtWheel::valueChanged,
                this, &SimulationExperimentViewZincWidget::setTimeStep);

        mToolBarWidget->addWidget(mSpeedWidget);
        mToolBarWidget->addSeparator();

        mTimeCheckBox = new QCheckBox(timeWidget);

        mTimeCheckBox->setEnabled(false);
        mTimeCheckBox->setText(tr("Auto"));

        connect(mTimeCheckBox, &QCheckBox::toggled,
                this, &SimulationExperimentViewZincWidget::autoMode);

        mToolBarWidget->addWidget(mTimeCheckBox);

    timeWidget->layout()->addWidget(mToolBarWidget);

    layout->addWidget(timeWidget);

    // Create and add our time slider

    mTimeSlider = new QSlider(this);

    mTimeSlider->setEnabled(false);
    mTimeSlider->setOrientation(Qt::Horizontal);

    connect(mTimeSlider, &QSlider::valueChanged,
            this, &SimulationExperimentViewZincWidget::timeSliderValueChanged);

    layout->addWidget(mTimeSlider);


    // Create Zinc Context

    createAndSetZincContext();

    initializeZincRegion();

    // Customise our timer

    connect(&mTimer, &QTimer::timeout,
            this, &SimulationExperimentViewZincWidget::timerTimeOut);


}

//==============================================================================

SimulationExperimentViewZincWidget::~SimulationExperimentViewZincWidget()
{
    // Keep track of the fact that we are shutting down

    mShuttingDown = true;

    // Delete the temporary copy of our .exfile file

    delete mTimeValues;
}

//==============================================================================

void SimulationExperimentViewZincWidget::retranslateUi()
{
}

//==============================================================================

void SimulationExperimentViewZincWidget::loadMappingFile(QString pFileName)
{
    // save and display the new file name

    mMappingFileName = pFileName;

    mMapNodeVariables->clear();

    QFile file;
    file.setFileName(pFileName);

    if (!file.exists()) {
        mMappingFileLabel->setText("no mapping file opened");
        return;
    }

    mMappingFileLabel->setText(mMappingFileName.split("/").last());

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    //take element from json objects

    QJsonDocument jsonDocument = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject jsonObject = jsonDocument.object();
    QStringList meshFiles = jsonObject.value("meshfiles").toVariant().toStringList();

    int id;
    QString component, variable;

    for (auto mapPointJson : jsonObject.value("map").toArray()) {
        QJsonObject mapPoint = mapPointJson.toObject();

        id = mapPoint.value("node").toInt();
        component = mapPoint.value("component").toString();
        variable = mapPoint.value("variable").toString();

        mMapNodeVariables->insert(id,{component, variable});
    }
}

//==============================================================================

void SimulationExperimentViewZincWidget::loadZincMeshFiles(const QStringList &pZincMeshFiles)
{
    // Keep track of Zinc mesh files and reset our scene viewer description

    mZincMeshFileNames = pZincMeshFiles;

    // Reset our Zinc widget

    mZincWidget->reset();

    mZincContext.setDefaultRegion(mZincContext.createRegion());
    mZincWidget->sceneViewer().setScene(mZincContext.getDefaultRegion().getScene());

    // hide spectrum accesses

    mLogCheckBox->setEnabled(false);
    mLogAmpliLineEdit->setEnabled(false);

    // Fill the region with the new model

    initializeZincRegion();

}

//==============================================================================


void SimulationExperimentViewZincWidget::initData(quint64 pDataSize, double pMinimumTime, double pMaximumTime,
                  double pTimeInterval, QMap<QString, double *> &pMapVariableValues)
{
    // Initialise our data with the setup of the experiment

    mDataSize = 0;
    mValueMin = mDefaultValue;
    mValueMax = mDefaultValue;

    // set our time values, crating a dummy one to initialize the dataField if have nothing

    if (pDataSize > 0) {
        mTimeValues = new double[pDataSize];
    } else {
        // to initialize the field with a dummy value
        mTimeValues = new double(0.);
    }

    for (quint64 i = 0; i < pDataSize; ++i) {
        mTimeValues[i] = pMinimumTime + double(i)*pTimeInterval;
    }

    // clear the groups from the variables received

    QMap<QString, double *> mapVariableValues = QMap<QString, double *>();

    for (QString variable : pMapVariableValues.keys()) {
        QStringList variablePieces = variable.split(".");
        int len = variablePieces.length();
        if (len > 2) {
            QString cleanVariable = variablePieces[len-2]+"."+variablePieces[len-1];
            mapVariableValues.insert(cleanVariable, pMapVariableValues.value(variable));
        } else {
            mapVariableValues.insert(variable, pMapVariableValues.value(variable));
        }
    }

    // link node numbers and double *values

    mMapNodeValues->clear();
    for (int nodeId : mMapNodeVariables->keys()) {
        _variable _variable = mMapNodeVariables->value(nodeId);
        QString variable = _variable.component+"."+_variable.variable;

        // check if values are provided for this node
        if (mapVariableValues.contains(variable)) {
            mMapNodeValues->insert(nodeId, mapVariableValues.value(variable));

        }
    }

    // Reset our field

    if (pDataSize>0) {
        updateNodeValues(0, pDataSize,true);
    } else {
        updateNodeValues(0, 1, true);
    }

    // Set the range of valid times in our default time keeper

    mTimeKeeper.setMinimumTime(pMinimumTime);
    mTimeKeeper.setMaximumTime(pMaximumTime);

    mTimeSlider->setMinimum(int(pMinimumTime/pTimeInterval));
    mTimeSlider->setMaximum(int(pMaximumTime/pTimeInterval));

    // Disable our time-related widgets

    mTimeLabel->setEnabled(false);
    mTimeCheckBox->setEnabled(false);
    mTimeSlider->setEnabled(false);

    mTimeCheckBox->setChecked(false);
    mTimeSlider->setValue(mTimeSlider->minimum());
}

//==============================================================================

void SimulationExperimentViewZincWidget::addData(int pDataSize)
{

    updateNodeValues(mDataSize,pDataSize);

    mSpectrum.beginChange();
        mSpectrum.setMaterialOverwrite(true);
    mSpectrum.endChange();

    mDataSize = pDataSize;

    // Enable our time-related widgets, if all the data has been added

    if (pDataSize-1 == mTimeSlider->maximum()) {
        mTimeLabel->setEnabled(true);
        mTimeCheckBox->setEnabled(true);
        mTimeSlider->setEnabled(true);

        mTimeCheckBox->setChecked(true);
    }
}

//==============================================================================

void SimulationExperimentViewZincWidget::updateNodeValues(int pValueBegin, int pValueEnd, bool pReset)
{
    Q_UNUSED(pValueBegin)
    auto fieldModule = mZincContext.getDefaultRegion().getFieldmodule();

    fieldModule.beginChange();

        OpenCMISS::Zinc::Nodeset nodeSet = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        qDebug("nodeSet %d/%d",nodeSet.isValid(),true);

        OpenCMISS::Zinc::Nodetemplate nodeTemplate = nodeSet.createNodetemplate();
        qDebug("mNodeTemplate %d/%d",nodeTemplate.isValid(),true);
        qDebug("defineField %d", nodeTemplate.defineField(mDataField));

        OpenCMISS::Zinc::Timesequence timeSequence = fieldModule.getMatchingTimesequence(pValueEnd, mTimeValues);
        qDebug("timeSequence %d/%d",timeSequence.isValid(),true);
        qDebug("setTimesequence %d", nodeTemplate.setTimesequence(mDataField,timeSequence));

        OpenCMISS::Zinc::Fieldcache fieldCache = fieldModule.createFieldcache();

        //for (int nodeId : mMapNodeValues->keys()) {
        OpenCMISS::Zinc::Nodeiterator nodeIter = nodeSet.createNodeiterator();
        OpenCMISS::Zinc::Node node;

        while ((node = nodeIter.next()).isValid()) {

            int nodeId = node.getIdentifier();
            fieldCache.setNode(node);
            node.merge(nodeTemplate);

            //TODO
            // it shouldn't start at 0 but at pValueBegin, but it seems that
            // the values before it are reset to 0 in this case

            if (!pReset && mMapNodeValues->contains(nodeId)) {
                for (int i = 0; i < pValueEnd; ++i) {
                    fieldCache.setTime(mTimeValues[i]);
                    double *target = mMapNodeValues->value(nodeId)+i;
                    mDataField.assignReal(fieldCache, 1, target);
                    //qDebug("assigningReal %d",mDataField.assignReal(fieldCache, 1 ,mMapNodeValues->value(nodeId)+i));
                    //qDebug("assigning %d %d %f",nodeId, i,*target);

                    if (mValueMin > *target) {
                        mValueMin = *target;
                    }
                    if (mValueMax < *target) {
                        mValueMax = *target;
                    }
                }
            } else {
                for (int i = 0; i < pValueEnd; ++i) {
                    fieldCache.setTime(mTimeValues[i]);
                    mDataField.assignReal(fieldCache, 1, &mDefaultValue);
                }
            }
        }
    fieldModule.endChange();

    mSpectrum.beginChange();
        auto firstComponent = mSpectrum.getFirstSpectrumcomponent();
        firstComponent.setRangeMinimum(mValueMin);
        firstComponent.setRangeMaximum(mValueMax);
        qDebug("min %f max %f",mValueMin,mValueMax);
        firstComponent.setExtendBelow(true);
        firstComponent.setExtendAbove(true);
    mSpectrum.endChange();
}

//==============================================================================

void SimulationExperimentViewZincWidget::createAndSetZincContext()
{
    // Make sure that we are not shutting down (i.e. skip the case where we are
    // coming here as a result of closing OpenCOR)

    if (mShuttingDown) {
        return;
    }

    // Keep track of our current scene viewer's description, if needed
    //TODO move to the right place when know what it is
    //mZincSceneViewerDescription = mDroppedZincMeshFiles?
    //                                  nullptr:
    //                                  mZincWidget->sceneViewer().writeDescription();

    // Create and set our Zinc context

    mZincContext = OpenCMISS::Zinc::Context("SimulationViewZincWidget");

    mZincContext.getMaterialmodule().defineStandardMaterials();
    mZincContext.getGlyphmodule().defineStandardGlyphs();

    mZincWidget->setContext(mZincContext);

    // Retrieve the default time keeper

    OpenCMISS::Zinc::Timekeepermodule timeKeeperModule = mZincContext.getTimekeepermodule();

    mTimeKeeper = timeKeeperModule.getDefaultTimekeeper();
}

//==============================================================================

void SimulationExperimentViewZincWidget::initializeZincRegion()
{

    // Add the Zinc mesh files to our default region, or show a tri-linear cube
    // if there are no Zinc mesh files

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();
    OpenCMISS::Zinc::StreaminformationRegion sir = region.createStreaminformationRegion();

    for (const auto &zincMeshFileName : mZincMeshFileNames) {
        sir.createStreamresourceFile(zincMeshFileName.toUtf8().constData());
    }

    region.read(sir);

    // Create a field magnitude for our default region

    OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();
    OpenCMISS::Zinc::Sceneviewer sceneViewer = mZincWidget->sceneViewer();
    OpenCMISS::Zinc::Scene scene = region.getScene();
    sceneViewer.setScene(scene);

    fieldModule.beginChange();

        fieldModule.createFieldTimeValue(mTimeKeeper);

        OpenCMISS::Zinc::Fielditerator fieldIterator = fieldModule.createFielditerator();
        OpenCMISS::Zinc::Field field = fieldIterator.next();

        while (field.isValid()) {
            if (    field.isTypeCoordinate()
                && (field.getValueType() == OpenCMISS::Zinc::Field::VALUE_TYPE_REAL)
                && (field.getNumberOfComponents() <= 3)
                &&  field.castFiniteElement().isValid()) {

                mCoordinates = field.castFiniteElement();
                break;
            }
            field = fieldIterator.next();
        }

        //mMagnitude = fieldModule.createFieldMagnitude(mCoordinates);

        //mMagnitude.setManaged(true);

        mDataField = fieldModule.createFieldFiniteElement(1);
        qDebug("mDataField %d/%d", mDataField.isValid(),true);

        OpenCMISS::Zinc::Spectrummodule spectrumModule = mZincContext.getSpectrummodule();
        mSpectrum = spectrumModule.getDefaultSpectrum();

        // setup spectrum

        mSpectrum.beginChange();
            //mSpectrum.setMaterialOverwrite(false); //doesn't work
            OpenCMISS::Zinc::Spectrumcomponent firstComponent = mSpectrum.getFirstSpectrumcomponent();
            //firstComponent.setScaleType(OpenCMISS::Zinc::Spectrumcomponent::SCALE_TYPE_LOG);
            //firstComponent.setExaggeration(100.);
        mSpectrum.endChange();

        // show spectrum accesses

        mLogCheckBox->setEnabled(true);

        // hic sunt dracones

        OpenCMISS::Zinc::Mesh mesh = fieldModule.findMeshByDimension(3);

        OpenCMISS::Zinc::Elementtemplate elementTemplate = mesh.createElementtemplate();
        OpenCMISS::Zinc::Elementiterator elementiter = mesh.createElementiterator();
        OpenCMISS::Zinc::Element element;
        while ((element = elementiter.next()).isValid()) {
            OpenCMISS::Zinc::Elementfieldtemplate eft = element.getElementfieldtemplate(mCoordinates, 1);//  # use interpolation of first coordinate component, or -1 for all (works only if all defined identically).
            elementTemplate.defineField(mDataField, -1, eft); // -1 = all components, but there is only 1
            element.merge(elementTemplate);
        }

        auto timeField = fieldModule.createFieldTimeValue(mTimeKeeper);

    fieldModule.endChange();

    // Show/hide graphics on the scene of our default region

    scene.beginChange();
        // Axes

        OpenCMISS::Zinc::GraphicsPoints axes = scene.createGraphicsPoints();
        OpenCMISS::Zinc::Materialmodule materialModule = mZincContext.getMaterialmodule();

        axes.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_POINT);
        axes.setMaterial(materialModule.findMaterialByName("blue"));

        mAxesFontPointSize = axes.getGraphicspointattributes().getFont().getPointSize();
        mAxesAttributes = axes.getGraphicspointattributes();

        // Points

        OpenCMISS::Zinc::GraphicsPoints points = scene.createGraphicsPoints();

        points.setCoordinateField(mCoordinates);
        points.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        points.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("green"));

        mPointsAttributes = points.getGraphicspointattributes();
        points.setSpectrum(mSpectrum);
        //points.getGraphicspointattributes().setLabelField(mDataField);

        points.setDataField(mDataField);

        // Lines

        mLines = scene.createGraphicsLines();

        mLines.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("black"));
        mLines.setSpectrum(mSpectrum);
        mLines.setDataField(mDataField);

        // Surfaces

        mSurfaces = scene.createGraphicsSurfaces();

        mSurfaces.setSpectrum(mSpectrum);
        mSurfaces.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("white"));
        mSurfaces.setDataField(mDataField);

        double doubleValue;

        // Isosurfaces
    /*
        OpenCMISS::Zinc::Tessellation tessellation = mZincContext.getTessellationmodule().createTessellation();
        int intValue = 8;

        tessellation.setManaged(true);
        tessellation.setMinimumDivisions(1, &intValue);

        mIsosurfaces = scene.createGraphicsContours();

        double doubleValue = 1.0;

        mIsosurfaces.setIsoscalarField(mMagnitude);
        mIsosurfaces.setListIsovalues(1, &doubleValue);
        mIsosurfaces.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("gold"));
        mIsosurfaces.setSpectrum(mSpectrum);
        mIsosurfaces.setTessellation(tessellation);
    */
    scene.endChange();

    // initialisation of the dataField

    if (mDataSize==0) {
        //put just zeros at time 0
        updateNodeValues(0, 1, true);
    } else {
        updateNodeValues(0, mDataSize);
    }
    // Display all our objects

    showHideGraphics(GraphicsType::All);

    // Update our scene using our initial device pixel ratio

    devicePixelRatioChanged(mZincWidget->devicePixelRatio());

    // Make sure that the mesh is visible

    mZincWidget->viewAll();

    // Customise the size of our axes and points

    double left, right, bottom, top, nearPlane, farPlane;

    mZincWidget->sceneViewer().getViewingVolume(&left, &right, &bottom, &top, &nearPlane, &farPlane);

    doubleValue = 0.65*right;

    mAxesAttributes.setBaseSize(1, &doubleValue);

    doubleValue = 0.02*right;

    mPointsAttributes.setBaseSize(1, &doubleValue);
}

//==============================================================================
//TODO use this
void SimulationExperimentViewZincWidget::graphicsInitialized()
{
    // Set our 'new' scene viewer's description

    mZincWidget->sceneViewer().readDescription(mZincSceneViewerDescription);
}

//==============================================================================

void SimulationExperimentViewZincWidget::devicePixelRatioChanged(int pDevicePixelRatio)
{
    // Update our scene using the given device pixel ratio

    OpenCMISS::Zinc::Scene scene = mZincContext.getDefaultRegion().getScene();

    scene.beginChange();
        scene.createGraphicsPoints().getGraphicspointattributes().getFont().setPointSize(pDevicePixelRatio*mAxesFontPointSize);
    scene.endChange();
}

//==============================================================================

void SimulationExperimentViewZincWidget::timeSliderValueChanged(int pTime)
{
    // Update our scene

    double time = mTimeValues[pTime];

    mTimeLabel->setText(tr("Time: %1 s").arg(time));

    //OpenCMISS::Zinc::Timekeepermodule timeKeeperModule = mZincContext.getTimekeepermodule();

    //mTimeKeeper = timeKeeperModule.getDefaultTimekeeper();

    mTimeKeeper.setTime(time);
}


//==============================================================================

void SimulationExperimentViewZincWidget::timerTimeOut()
{
    // Update our scene

    int value = mTimeSlider->value();

    if (value + mTimeStep > mTimeSlider->maximum()) {
        value = mTimeSlider->minimum();
    } else {
        value += mTimeStep;
    }

    mTimeSlider->setValue(value);
}

//==============================================================================

void SimulationExperimentViewZincWidget::autoMode()
{
    // Enable/disable our timer

    if (mTimeCheckBox->isChecked()) {
        mTimer.start();
    } else {
        mTimer.stop();
    }
}

//==============================================================================

void SimulationExperimentViewZincWidget::showHideGraphics(GraphicsType pGraphicsType)
{
    // Show/hide graphics on the scene of our default region

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();
    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        // Axes
Q_UNUSED(pGraphicsType)

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Axes)) {
            mAxesAttributes.setGlyphShapeType(mActionAxes->isChecked()?
                                                  OpenCMISS::Zinc::Glyph::SHAPE_TYPE_AXES_XYZ:
                                                  OpenCMISS::Zinc::Glyph::SHAPE_TYPE_NONE);
        }

        // Points

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Points)) {
            mPointsAttributes.setGlyphShapeType(mActionPoints->isChecked()?
                                                    OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE:
                                                    OpenCMISS::Zinc::Glyph::SHAPE_TYPE_NONE);
        }

        // Lines

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Lines)) {
            mLines.setCoordinateField(mActionLines->isChecked()?
                                          mCoordinates:
                                          OpenCMISS::Zinc::Field());
        }

        // Surfaces

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Surfaces)) {
            mSurfaces.setCoordinateField(mActionSurfaces->isChecked()?
                                             mCoordinates:
                                             OpenCMISS::Zinc::Field());
        }

        // Isosurfaces

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Isosurfaces)) {
            mIsosurfaces.setCoordinateField(mActionIsosurfaces->isChecked()?
                                                mCoordinates:
                                                OpenCMISS::Zinc::Field());
        }
    scene.endChange();
}

//==============================================================================

void SimulationExperimentViewZincWidget::setTimeStep(int value)
{
    mTimeStep = int(pow(1.1,value));
}

//==============================================================================

void SimulationExperimentViewZincWidget::actionAxesTriggered()
{
    // Show/hide our axes

    showHideGraphics(GraphicsType::Axes);
}

//==============================================================================

void SimulationExperimentViewZincWidget::actionPointsTriggered()
{
    // Show/hide our points

    showHideGraphics(GraphicsType::Points);
}

//==============================================================================

void SimulationExperimentViewZincWidget::actionLinesTriggered()
{
    // Show/hide our lines

    showHideGraphics(GraphicsType::Lines);
}

//==============================================================================

void SimulationExperimentViewZincWidget::actionSurfacesTriggered()
{
    // Show/hide our surfaces

    showHideGraphics(GraphicsType::Surfaces);
}

//==============================================================================

void SimulationExperimentViewZincWidget::actionIsosurfacesTriggered()
{
    // Show/hide our isosurfaces

    showHideGraphics(GraphicsType::Isosurfaces);
}

//==============================================================================

void SimulationExperimentViewZincWidget::changedSpectrumExageration(QString pValue)
{
    mSpectrum.getFirstSpectrumcomponent().setExaggeration(pValue.toFloat());
}

//==============================================================================

void SimulationExperimentViewZincWidget::setSpectrumScale()
{
    mSpectrum.getFirstSpectrumcomponent().setScaleType(mLogCheckBox->isChecked()?
                                                           OpenCMISS::Zinc::Spectrumcomponent::SCALE_TYPE_LOG:
                                                           OpenCMISS::Zinc::Spectrumcomponent::SCALE_TYPE_LINEAR);

    mLogAmpliLineEdit->setEnabled(mLogCheckBox->isChecked());
}

//==============================================================================

} // namespace ZincWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
