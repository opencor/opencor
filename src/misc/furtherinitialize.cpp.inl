#ifdef GUI_SUPPORT
bool SimulationExperimentViewSimulationWidget::furtherInitialize()
{
    // Customise our simulation widget

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SimulationExperimentViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();
    SimulationExperimentViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();
    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = informationWidget->graphPanelAndGraphsWidget();
    libsedml::SedDocument *sedmlDocument = mSimulation->sedmlFile()->sedmlDocument();
#else
QString Simulation::furtherInitialize() const
{
    // Initialise ourself from a SED-ML document

    libsedml::SedDocument *sedmlDocument = sedmlFile()->sedmlDocument();
#endif
    auto sedmlUniformTimeCourse = static_cast<libsedml::SedUniformTimeCourse *>(sedmlDocument->getSimulation(0));
    auto sedmlOneStep = static_cast<libsedml::SedOneStep *>(sedmlDocument->getSimulation(1));
    double startingPoint = sedmlUniformTimeCourse->getOutputStartTime();
    double endingPoint = sedmlUniformTimeCourse->getOutputEndTime();
    double pointInterval = (endingPoint-startingPoint)/sedmlUniformTimeCourse->getNumberOfPoints();

    if (sedmlOneStep != nullptr) {
        endingPoint += sedmlOneStep->getStep();
    }

#ifdef GUI_SUPPORT
    simulationWidget->startingPointProperty()->setDoubleValue(startingPoint);
    simulationWidget->endingPointProperty()->setDoubleValue(endingPoint);
    simulationWidget->pointIntervalProperty()->setDoubleValue(pointInterval);
#else
    mData->setStartingPoint(startingPoint);
    mData->setEndingPoint(endingPoint);
    mData->setPointInterval(pointInterval);
#endif

    // Try to customise our solvers widget by specifying the ODE solver (and NLA
    // solver, should one be needed) and customising its properties for which we
    // have a KiSAO id

    libsedml::SedAlgorithm *sedmlAlgorithm = sedmlUniformTimeCourse->getAlgorithm();
    QString kisaoId = QString::fromStdString(sedmlAlgorithm->getKisaoID());
#ifdef GUI_SUPPORT

    if (!initializeSolver(sedmlAlgorithm->getListOfAlgorithmParameters(), kisaoId,
                          mContentsWidget->informationWidget()->solversWidget()->odeSolverData())) {
        return false;
    }
#else
    QString error = initializeSolver(sedmlAlgorithm->getListOfAlgorithmParameters(),
                                     kisaoId);

    if (!error.isEmpty()) {
        return error;
    }
#endif

    libsbml::XMLNode *annotation = sedmlUniformTimeCourse->getAnnotation();

    if (annotation != nullptr) {
#ifndef GUI_SUPPORT
        bool mustHaveNlaSolver = false;
#endif
        bool hasNlaSolver = false;
        QString nlaSolverName = QString();

        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            libsbml::XMLNode &nlaSolverNode = annotation->getChild(i);

            if (   (QString::fromStdString(nlaSolverNode.getURI()) == SEDMLSupport::OpencorNamespace)
                && (QString::fromStdString(nlaSolverNode.getName()) == SEDMLSupport::NlaSolver)) {
#ifdef GUI_SUPPORT
                SimulationExperimentViewInformationSolversWidgetData *nlaSolverData = solversWidget->nlaSolverData();
                Core::Properties nlaSolverProperties = Core::Properties();

#else
                mustHaveNlaSolver = true;
#endif
                nlaSolverName = QString::fromStdString(nlaSolverNode.getAttrValue(nlaSolverNode.getAttrIndex(SEDMLSupport::Name.toStdString())));

                for (auto solverInterface : Core::solverInterfaces()) {
                    if (nlaSolverName == solverInterface->solverName()) {
#ifdef GUI_SUPPORT
                        nlaSolverProperties = nlaSolverData->solversProperties().value(solverInterface->solverName());

                        nlaSolverData->solversListProperty()->setValue(nlaSolverName);
#else
                        mData->setNlaSolverName(nlaSolverName);
#endif

                        hasNlaSolver = true;

                        break;
                    }
                }

                if (hasNlaSolver) {
                    for (uint j = 0, jMax = nlaSolverNode.getNumChildren(); j < jMax; ++j) {
                        libsbml::XMLNode &solverPropertyNode = nlaSolverNode.getChild(j);

                        if (   (QString::fromStdString(solverPropertyNode.getURI()) == SEDMLSupport::OpencorNamespace)
                            && (QString::fromStdString(solverPropertyNode.getName()) == SEDMLSupport::SolverProperty)) {
                            QString id = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Id.toStdString())));
                            QString value = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Value.toStdString())));
#ifdef GUI_SUPPORT
                            bool propertySet = false;

                            for (auto solverProperty : nlaSolverProperties) {
                                if (solverProperty->id() == id) {
                                    solverProperty->setValue(value);

                                    propertySet = true;

                                    break;
                                }
                            }

                            if (!propertySet) {
                                simulationError(QObject::tr("the requested solver property (%1) could not be set.").arg(id),
                                                Error::InvalidSimulationEnvironment);

                                return false;
                            }
#else

                            mData->setNlaSolverProperty(id, value);
#endif
                        }
                    }

                    break;
                }
#ifdef GUI_SUPPORT

                simulationError(QObject::tr("the requested solver (%1) could not be found.").arg(nlaSolverName),
                                Error::InvalidSimulationEnvironment);

                return false;
#endif
            }
        }
#ifndef GUI_SUPPORT

        if (mustHaveNlaSolver && !hasNlaSolver) {
            return QObject::tr("the requested solver (%1) could not be found.").arg(nlaSolverName);
        }
#endif
    }

#ifdef GUI_SUPPORT
    // Add/remove some graph panels, so that our final number of graph panels
    // corresponds to the number of 2D outputs mentioned in the SED-ML file
    // Note: no need to pass defaultGraphPanelProperties() to our
    //       removeCurrentGraphPanel() and addGraphPanel() methods since all the
    //       graph panels get fully customised afterwards...

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    int oldNbOfGraphPanels = graphPanelsWidget->graphPanels().count();
    int newNbOfGraphPanels = int(sedmlDocument->getNumOutputs());

    if (oldNbOfGraphPanels < newNbOfGraphPanels) {
        for (int i = 0, iMax = newNbOfGraphPanels-oldNbOfGraphPanels; i < iMax; ++i) {
            graphPanelsWidget->addGraphPanel();
        }
    } else if (oldNbOfGraphPanels > newNbOfGraphPanels) {
        for (int i = 0, iMax = oldNbOfGraphPanels-newNbOfGraphPanels; i < iMax; ++i) {
            graphPanelsWidget->removeCurrentGraphPanel();
        }
    }

    // Customise our graph panel and graphs

    QIntList graphPanelsWidgetSizes = QIntList();

    for (int i = 0; i < newNbOfGraphPanels; ++i) {
        // Customise our graph panel

        auto sedmlPlot2d = static_cast<libsedml::SedPlot2D *>(sedmlDocument->getOutput(uint(i)));
        GraphPanelWidget::GraphPanelWidget *graphPanel = graphPanelsWidget->graphPanels()[i];

        graphPanelAndGraphsWidget->reinitialize(graphPanel);

        annotation = sedmlPlot2d->getAnnotation();

        if (annotation != nullptr) {
            Core::Properties graphPanelProperties = graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel)->properties();

            for (uint j = 0, jMax = annotation->getNumChildren(); j < jMax; ++j) {
                libsbml::XMLNode &sedmlPlot2dPropertiesNode = annotation->getChild(j);

                if (   (QString::fromStdString(sedmlPlot2dPropertiesNode.getURI()) == SEDMLSupport::OpencorNamespace)
                    && (QString::fromStdString(sedmlPlot2dPropertiesNode.getName()) == SEDMLSupport::Properties)) {
                    for (uint k = 0, kMax = sedmlPlot2dPropertiesNode.getNumChildren(); k < kMax; ++k) {
                        libsbml::XMLNode &sedmlPlot2dPropertyNode = sedmlPlot2dPropertiesNode.getChild(k);
                        QString sedmlPlot2dPropertyNodeName = QString::fromStdString(sedmlPlot2dPropertyNode.getName());
                        QString sedmlPlot2dPropertyNodeValue = QString::fromStdString(sedmlPlot2dPropertyNode.getChild(0).getCharacters());

                        if (sedmlPlot2dPropertyNodeName == SEDMLSupport::BackgroundColor) {
                            graphPanelProperties[0]->setValue(sedmlPlot2dPropertyNodeValue);
                        } else if (sedmlPlot2dPropertyNodeName == SEDMLSupport::FontSize) {
                            graphPanelProperties[1]->setValue(sedmlPlot2dPropertyNodeValue);
                        } else if (sedmlPlot2dPropertyNodeName == SEDMLSupport::ForegroundColor) {
                            graphPanelProperties[2]->setValue(sedmlPlot2dPropertyNodeValue);
                        } else if (sedmlPlot2dPropertyNodeName == SEDMLSupport::Height) {
                            graphPanelsWidgetSizes << sedmlPlot2dPropertyNodeValue.toInt();

                        // Grid lines

                        } else if (   (QString::fromStdString(sedmlPlot2dPropertyNode.getURI()) == SEDMLSupport::OpencorNamespace)
                                   && (QString::fromStdString(sedmlPlot2dPropertyNode.getName()) == SEDMLSupport::GridLines)) {
                            Core::Properties gridLinesProperties = graphPanelProperties[3]->properties();

                            for (uint l = 0, lMax = sedmlPlot2dPropertyNode.getNumChildren(); l < lMax; ++l) {
                                libsbml::XMLNode &gridLinesPropertyNode = sedmlPlot2dPropertyNode.getChild(l);
                                QString gridLinesPropertyNodeName = QString::fromStdString(gridLinesPropertyNode.getName());
                                QString gridLinesPropertyNodeValue = QString::fromStdString(gridLinesPropertyNode.getChild(0).getCharacters());

                                if (gridLinesPropertyNodeName == SEDMLSupport::Style) {
                                    gridLinesProperties[0]->setValue(gridLinesPropertyNodeValue);
                                } else if (gridLinesPropertyNodeName == SEDMLSupport::Width) {
                                    gridLinesProperties[1]->setValue(gridLinesPropertyNodeValue);
                                } else if (gridLinesPropertyNodeName == SEDMLSupport::Color) {
                                    gridLinesProperties[2]->setValue(gridLinesPropertyNodeValue);
                                }
                            }

                        // Legend

                        } else if (sedmlPlot2dPropertyNodeName == SEDMLSupport::Legend) {
                            graphPanelProperties[4]->setBooleanValue(sedmlPlot2dPropertyNodeValue == TrueValue);

                        // Point coordinates

                        } else if (   (QString::fromStdString(sedmlPlot2dPropertyNode.getURI()) == SEDMLSupport::OpencorNamespace)
                                   && (QString::fromStdString(sedmlPlot2dPropertyNode.getName()) == SEDMLSupport::PointCoordinates)) {
                            Core::Properties pointCoordinatesProperties = graphPanelProperties[5]->properties();

                            for (uint l = 0, lMax = sedmlPlot2dPropertyNode.getNumChildren(); l < lMax; ++l) {
                                libsbml::XMLNode &pointCoordinatesPropertyNode = sedmlPlot2dPropertyNode.getChild(l);
                                QString pointCoordinatesPropertyNodeName = QString::fromStdString(pointCoordinatesPropertyNode.getName());
                                QString pointCoordinatesPropertyNodeValue = QString::fromStdString(pointCoordinatesPropertyNode.getChild(0).getCharacters());

                                if (pointCoordinatesPropertyNodeName == SEDMLSupport::Style) {
                                    pointCoordinatesProperties[0]->setValue(pointCoordinatesPropertyNodeValue);
                                } else if (pointCoordinatesPropertyNodeName == SEDMLSupport::Width) {
                                    pointCoordinatesProperties[1]->setValue(pointCoordinatesPropertyNodeValue);
                                } else if (pointCoordinatesPropertyNodeName == SEDMLSupport::Color) {
                                    pointCoordinatesProperties[2]->setValue(pointCoordinatesPropertyNodeValue);
                                } else if (pointCoordinatesPropertyNodeName == SEDMLSupport::FontColor) {
                                    pointCoordinatesProperties[3]->setValue(pointCoordinatesPropertyNodeValue);
                                }
                            }

                        // Surrounding area

                        } else if (   (QString::fromStdString(sedmlPlot2dPropertyNode.getURI()) == SEDMLSupport::OpencorNamespace)
                                   && (QString::fromStdString(sedmlPlot2dPropertyNode.getName()) == SEDMLSupport::SurroundingArea)) {
                            Core::Properties surroundingAreaProperties = graphPanelProperties[6]->properties();

                            for (uint l = 0, lMax = sedmlPlot2dPropertyNode.getNumChildren(); l < lMax; ++l) {
                                libsbml::XMLNode &surroundingAreaPropertyNode = sedmlPlot2dPropertyNode.getChild(l);
                                QString surroundingAreaPropertyNodeName = QString::fromStdString(surroundingAreaPropertyNode.getName());
                                QString surroundingAreaPropertyNodeValue = QString::fromStdString(surroundingAreaPropertyNode.getChild(0).getCharacters());

                                if (surroundingAreaPropertyNodeName == SEDMLSupport::BackgroundColor) {
                                    surroundingAreaProperties[0]->setValue(surroundingAreaPropertyNodeValue);
                                } else if (surroundingAreaPropertyNodeName == SEDMLSupport::ForegroundColor) {
                                    surroundingAreaProperties[1]->setValue(surroundingAreaPropertyNodeValue);
                                }
                            }

                        // Title

                        } else if (sedmlPlot2dPropertyNodeName == SEDMLSupport::Title) {
                            graphPanelProperties[7]->setValue(sedmlPlot2dPropertyNodeValue);

                        // X axis

                        } else if (   (QString::fromStdString(sedmlPlot2dPropertyNode.getURI()) == SEDMLSupport::OpencorNamespace)
                                   && (QString::fromStdString(sedmlPlot2dPropertyNode.getName()) == SEDMLSupport::XAxis)) {
                            Core::Properties xAxisProperties = graphPanelProperties[8]->properties();

                            for (uint l = 0, lMax = sedmlPlot2dPropertyNode.getNumChildren(); l < lMax; ++l) {
                                libsbml::XMLNode &xAxisPropertyNode = sedmlPlot2dPropertyNode.getChild(l);
                                QString xAxisPropertyNodeName = QString::fromStdString(xAxisPropertyNode.getName());
                                QString xAxisPropertyNodeValue = QString::fromStdString(xAxisPropertyNode.getChild(0).getCharacters());

                                if (xAxisPropertyNodeName == SEDMLSupport::LogarithmicScale) {
                                    xAxisProperties[0]->setBooleanValue(xAxisPropertyNodeValue == TrueValue);
                                } else if (xAxisPropertyNodeName == SEDMLSupport::Title) {
                                    xAxisProperties[1]->setValue(xAxisPropertyNodeValue);
                                }
                            }

                        // Y axis

                        } else if (   (QString::fromStdString(sedmlPlot2dPropertyNode.getURI()) == SEDMLSupport::OpencorNamespace)
                                   && (QString::fromStdString(sedmlPlot2dPropertyNode.getName()) == SEDMLSupport::YAxis)) {
                            Core::Properties yAxisProperties = graphPanelProperties[9]->properties();

                            for (uint l = 0, lMax = sedmlPlot2dPropertyNode.getNumChildren(); l < lMax; ++l) {
                                libsbml::XMLNode &yAxisPropertyNode = sedmlPlot2dPropertyNode.getChild(l);
                                QString yAxisPropertyNodeName = QString::fromStdString(yAxisPropertyNode.getName());
                                QString yAxisPropertyNodeValue = QString::fromStdString(yAxisPropertyNode.getChild(0).getCharacters());

                                if (yAxisPropertyNodeName == SEDMLSupport::LogarithmicScale) {
                                    yAxisProperties[0]->setBooleanValue(yAxisPropertyNodeValue == TrueValue);
                                } else if (yAxisPropertyNodeName == SEDMLSupport::Title) {
                                    yAxisProperties[1]->setValue(yAxisPropertyNodeValue);
                                }
                            }

                        // Zoom region

                        } else if (   (QString::fromStdString(sedmlPlot2dPropertyNode.getURI()) == SEDMLSupport::OpencorNamespace)
                                   && (QString::fromStdString(sedmlPlot2dPropertyNode.getName()) == SEDMLSupport::ZoomRegion)) {
                            Core::Properties zoomRegionProperties = graphPanelProperties[10]->properties();

                            for (uint l = 0, lMax = sedmlPlot2dPropertyNode.getNumChildren(); l < lMax; ++l) {
                                libsbml::XMLNode &zoomRegionPropertyNode = sedmlPlot2dPropertyNode.getChild(l);
                                QString zoomRegionPropertyNodeName = QString::fromStdString(zoomRegionPropertyNode.getName());
                                QString zoomRegionPropertyNodeValue = QString::fromStdString(zoomRegionPropertyNode.getChild(0).getCharacters());

                                if (zoomRegionPropertyNodeName == SEDMLSupport::Style) {
                                    zoomRegionProperties[0]->setValue(zoomRegionPropertyNodeValue);
                                } else if (zoomRegionPropertyNodeName == SEDMLSupport::Width) {
                                    zoomRegionProperties[1]->setValue(zoomRegionPropertyNodeValue);
                                } else if (zoomRegionPropertyNodeName == SEDMLSupport::Color) {
                                    zoomRegionProperties[2]->setValue(zoomRegionPropertyNodeValue);
                                } else if (zoomRegionPropertyNodeName == SEDMLSupport::FontColor) {
                                    zoomRegionProperties[3]->setValue(zoomRegionPropertyNodeValue);
                                } else if (zoomRegionPropertyNodeName == SEDMLSupport::Filled) {
                                    zoomRegionProperties[4]->setBooleanValue(zoomRegionPropertyNodeValue == TrueValue);
                                } else if (zoomRegionPropertyNodeName == SEDMLSupport::FillColor) {
                                    zoomRegionProperties[5]->setValue(zoomRegionPropertyNodeValue);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Customise our graphs

        graphPanel->removeAllGraphs();

        for (uint j = 0, jMax = sedmlPlot2d->getNumCurves(); j < jMax; ++j) {
            libsedml::SedCurve *sedmlCurve = sedmlPlot2d->getCurve(j);

            libsedml::SedVariable *xVariable = sedmlDocument->getDataGenerator(sedmlCurve->getXDataReference())->getVariable(0);
            libsedml::SedVariable *yVariable = sedmlDocument->getDataGenerator(sedmlCurve->getYDataReference())->getVariable(0);
            QString xCellmlComponent;
            QString yCellmlComponent;
            QString xCellmlVariable;
            QString yCellmlVariable;
            CellMLSupport::CellmlFileRuntimeParameter *xParameter = runtimeParameter(xVariable, xCellmlComponent, xCellmlVariable);
            CellMLSupport::CellmlFileRuntimeParameter *yParameter = runtimeParameter(yVariable, yCellmlComponent, yCellmlVariable);

            if (xParameter == nullptr) {
                if (yParameter == nullptr) {
                    simulationError(QObject::tr("the requested curve (%1) could not be set (the variable %2 in component %3 and the variable %4 in component %5 could not be found).").arg(QString::fromStdString(sedmlCurve->getId()),
                                                                                                                                                                                           xCellmlVariable,
                                                                                                                                                                                           xCellmlComponent,
                                                                                                                                                                                           yCellmlVariable,
                                                                                                                                                                                           yCellmlComponent),
                                    Error::InvalidSimulationEnvironment);

                    return false;
                }

                simulationError(QObject::tr("the requested curve (%1) could not be set (the variable %2 in component %3 could not be found).").arg(QString::fromStdString(sedmlCurve->getId()),
                                                                                                                                                   xCellmlVariable,
                                                                                                                                                   xCellmlComponent),
                                Error::InvalidSimulationEnvironment);

                return false;
            }

            if (yParameter == nullptr) {
                simulationError(QObject::tr("the requested curve (%1) could not be set (the variable %2 in component %3 could not be found).").arg(QString::fromStdString(sedmlCurve->getId()),
                                                                                                                                                   yCellmlVariable,
                                                                                                                                                   yCellmlComponent),
                                Error::InvalidSimulationEnvironment);

                return false;
            }

            bool selected = GraphPanelWidget::DefaultGraphSelected;
            QString title = GraphPanelWidget::DefaultGraphTitle;
            Qt::PenStyle lineStyle = GraphPanelWidget::DefaultGraphLineStyle;
            int lineWidth = GraphPanelWidget::DefaultGraphLineWidth;
            QColor lineColor = GraphPanelWidget::DefaultGraphLineColor;
            QwtSymbol::Style symbolStyle = GraphPanelWidget::DefaultGraphSymbolStyle;
            int symbolSize = GraphPanelWidget::DefaultGraphSymbolSize;
            QColor symbolColor = GraphPanelWidget::DefaultGraphSymbolColor;
            bool symbolFilled = GraphPanelWidget::DefaultGraphSymbolFilled;
            QColor symbolFillColor = GraphPanelWidget::DefaultGraphSymbolFillColor;

            annotation = sedmlCurve->getAnnotation();

            if (annotation != nullptr) {
                for (uint k = 0, kMax = annotation->getNumChildren(); k < kMax; ++k) {
                    libsbml::XMLNode &curvePropertiesNode = annotation->getChild(k);

                    if (   (QString::fromStdString(curvePropertiesNode.getURI()) == SEDMLSupport::OpencorNamespace)
                        && (QString::fromStdString(curvePropertiesNode.getName()) == SEDMLSupport::Properties)) {
                        for (uint l = 0, lMax = curvePropertiesNode.getNumChildren(); l < lMax; ++l) {
                            libsbml::XMLNode &curvePropertyNode = curvePropertiesNode.getChild(l);
                            QString curvePropertyNodeName = QString::fromStdString(curvePropertyNode.getName());
                            QString curvePropertyNodeValue = QString::fromStdString(curvePropertyNode.getChild(0).getCharacters());

                            // Selected

                            if (curvePropertyNodeName == SEDMLSupport::Selected) {
                                selected = curvePropertyNodeValue == TrueValue;

                            // Title

                            } else if (curvePropertyNodeName == SEDMLSupport::Title) {
                                title = curvePropertyNodeValue;

                            // Line

                            } else if (curvePropertyNodeName == SEDMLSupport::Line) {
                                for (uint m = 0, mMax = curvePropertyNode.getNumChildren(); m < mMax; ++m) {
                                    libsbml::XMLNode &linePropertyNode = curvePropertyNode.getChild(m);
                                    QString linePropertyNodeName = QString::fromStdString(linePropertyNode.getName());
                                    QString linePropertyNodeValue = QString::fromStdString(linePropertyNode.getChild(0).getCharacters());

                                    if (linePropertyNodeName == SEDMLSupport::Style) {
                                        lineStyle = SEDMLSupport::lineStyle(linePropertyNodeValue);
                                    } else if (linePropertyNodeName == SEDMLSupport::Width) {
                                        lineWidth = linePropertyNodeValue.toInt();
                                    } else if (linePropertyNodeName == SEDMLSupport::Color) {
                                        lineColor.setNamedColor(linePropertyNodeValue);
                                    }
                                }

                            // Symbol

                            } else if (curvePropertyNodeName == SEDMLSupport::Symbol) {
                                for (uint m = 0, mMax = curvePropertyNode.getNumChildren(); m < mMax; ++m) {
                                    libsbml::XMLNode &symbolPropertyNode = curvePropertyNode.getChild(m);
                                    QString symbolPropertyNodeName = QString::fromStdString(symbolPropertyNode.getName());
                                    QString symbolPropertyNodeValue = QString::fromStdString(symbolPropertyNode.getChild(0).getCharacters());

                                    if (symbolPropertyNodeName == SEDMLSupport::Style) {
                                        symbolStyle = SEDMLSupport::symbolStyle(symbolPropertyNodeValue);
                                    } else if (symbolPropertyNodeName == SEDMLSupport::Size) {
                                        symbolSize = symbolPropertyNodeValue.toInt();
                                    } else if (symbolPropertyNodeName == SEDMLSupport::Color) {
                                        symbolColor.setNamedColor(symbolPropertyNodeValue);
                                    } else if (symbolPropertyNodeName == SEDMLSupport::Filled) {
                                        symbolFilled = symbolPropertyNodeValue == TrueValue;
                                    } else if (symbolPropertyNodeName == SEDMLSupport::FillColor) {
                                        symbolFillColor.setNamedColor(symbolPropertyNodeValue);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            graphPanel->addGraph(new GraphPanelWidget::GraphPanelPlotGraph(xParameter, yParameter, graphPanel),
                                 GraphPanelWidget::GraphPanelPlotGraphProperties(selected, title, lineStyle, lineWidth, lineColor, symbolStyle, symbolSize, symbolColor, symbolFilled, symbolFillColor));
        }
    }

    // Set our graph panels sizes and if none were provided then use some
    // default sizes

    if(graphPanelsWidgetSizes.isEmpty()) {
        for (int i = 0, iMax = graphPanelsWidget->count(); i < iMax; ++i) {
            graphPanelsWidgetSizes << 1;
        }
    }

    graphPanelsWidget->setSizes(graphPanelsWidgetSizes);

    // Select our first graph panel, now that we are fully initialised

    graphPanelsWidget->setActiveGraphPanel(graphPanelsWidget->graphPanels().first());

    // Initialise our trackers, so we know if a SED-ML file or a COMBINE archive
    // has been modified

    initializeTrackers();

    return true;
#else
    return {};
#endif
}
