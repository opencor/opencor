#ifdef GUI_SUPPORT
bool SimulationExperimentViewSimulationWidget::initializeSolver(const libsedml::SedListOfAlgorithmParameters *pSedmlAlgorithmParameters,
                                                                const QString &pKisaoId,
                                                                SimulationExperimentViewInformationSolversWidgetData *pSolverData)
#else
QString Simulation::initializeSolver(const libsedml::SedListOfAlgorithmParameters *pSedmlAlgorithmParameters,
                                     const QString &pKisaoId) const
#endif
{
    // Initialise our solver using the given SED-ML algorithm parameters and
    // KiSAO id

    SolverInterface *solverInterface = nullptr;
#ifdef GUI_SUPPORT
    Core::Properties solverProperties = Core::Properties();
#endif

    for (auto coreSolverInterface : Core::solverInterfaces()) {
        if (coreSolverInterface->id(pKisaoId) == coreSolverInterface->solverName()) {
            solverInterface = coreSolverInterface;
#ifdef GUI_SUPPORT
            solverProperties = pSolverData->solversProperties().value(coreSolverInterface->solverName());

            pSolverData->solversListProperty()->setValue(coreSolverInterface->solverName());
#else

            mData->setOdeSolverName(coreSolverInterface->solverName());
#endif

            break;
        }
    }

    if (solverInterface == nullptr) {
#ifdef GUI_SUPPORT
        simulationError(QObject::tr("the requested solver (%1) could not be found.").arg(pKisaoId),
                        Error::InvalidSimulationEnvironment);

        return false;
#else
        return QObject::tr("the requested solver (%1) could not be found.").arg(pKisaoId);
#endif
    }

    for (uint i = 0, iMax = pSedmlAlgorithmParameters->getNumAlgorithmParameters(); i < iMax; ++i) {
        const libsedml::SedAlgorithmParameter *sedmlAlgorithmParameter = pSedmlAlgorithmParameters->get(i);
        QString parameterKisaoId = QString::fromStdString(sedmlAlgorithmParameter->getKisaoID());
#ifdef GUI_SUPPORT
        QString id = solverInterface->id(parameterKisaoId);
        bool propertySet = false;

        for (auto solverProperty : solverProperties) {
            if (solverProperty->id() == id) {
                QVariant solverPropertyValue = QString::fromStdString(sedmlAlgorithmParameter->getValue());

                switch (solverProperty->type()) {
                case Core::Property::Type::Section:
                    // We should never come here...

#ifdef QT_DEBUG
                    qFatal("FATAL ERROR | %s:%d: a solver property cannot be of section type.", __FILE__, __LINE__);
#else
                    break;
#endif
                case Core::Property::Type::String:
                    solverProperty->setValue(solverPropertyValue.toString());

                    break;
                case Core::Property::Type::Integer:
                case Core::Property::Type::IntegerGe0:
                case Core::Property::Type::IntegerGt0:
                    solverProperty->setIntegerValue(solverPropertyValue.toInt());

                    break;
                case Core::Property::Type::Double:
                case Core::Property::Type::DoubleGe0:
                case Core::Property::Type::DoubleGt0:
                    solverProperty->setDoubleValue(solverPropertyValue.toDouble());

                    break;
                case Core::Property::Type::List:
                    solverProperty->setListValue(solverPropertyValue.toString());

                    break;
                case Core::Property::Type::Boolean:
                    solverProperty->setBooleanValue(solverPropertyValue.toBool());

                    break;
                case Core::Property::Type::Color:
                    // We should never come here...

#ifdef QT_DEBUG
                    qFatal("FATAL ERROR | %s:%d: a solver property cannot be of colour type.", __FILE__, __LINE__);
#else
                    break;
#endif
                }

                propertySet = solverProperty->type() != Core::Property::Type::Section;

                break;
            }
        }

        if (!propertySet) {
            simulationError(QObject::tr("the requested solver property (%1) could not be set.").arg(parameterKisaoId),
                            Error::InvalidSimulationEnvironment);

            return false;
        }
#else

        mData->setOdeSolverProperty(solverInterface->id(parameterKisaoId),
                                    QString::fromStdString(sedmlAlgorithmParameter->getValue()));
#endif
    }

#ifdef GUI_SUPPORT
    return true;
#else
    return {};
#endif
}
