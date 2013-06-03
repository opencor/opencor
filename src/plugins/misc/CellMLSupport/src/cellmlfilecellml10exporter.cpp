//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================
// Note: this exporter is adapted from David Nickerson's code which is an
//       updated version of Jonathan Cooper's CellML 1.1 to 1.0 converter. The
//       update was to make Jonathan's code work with version 1.12 of the CellML
//       API. In all cases, the original limitations apply...
//       Original code: http://www.cellml.org/tools/jonathan-cooper-s-cellml-1-1-to-1-0-converter/versionconverter-tar.bz2/view
//       Updated code: https://github.com/nickerso/flattenCellML
//==============================================================================

#include "cellmlfilecellml10exporter.h"

//==============================================================================

#include <QApplication>

//==============================================================================

#include "AnnoToolsBootstrap.hpp"
#include "CellMLBootstrap.hpp"
#include "CeVASBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileCellml10Exporter::CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                                       const QString &pFileName) :
    CellmlFileExporter()
{
    // Create an empty CellML 1.0 model

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();
    ObjRef<iface::cellml_api::Model> exportedModel = cellmlBootstrap->createModel(L"1.0");

    // Set the model's name and id

    exportedModel->name(pModel->name());

    std::wstring cmetaId = pModel->cmetaId();

    if (cmetaId.length())
        exportedModel->cmetaId(cmetaId);

    // Create an annotation set to manage annotations

    ObjRef<iface::cellml_services::AnnotationToolService> annotationToolService = CreateAnnotationToolService();

    mAnnotationSet = annotationToolService->createAnnotationSet();

    // Create a CellML Variable Association Service object so that we can find
    // relevant components

    ObjRef<iface::cellml_services::CeVASBootstrap> cevasBootstrap = CreateCeVASBootstrap();
    ObjRef<iface::cellml_services::CeVAS> cevas = cevasBootstrap->createCeVASForModel(pModel);

    std::wstring cevasError = cevas->modelError();

    if (cevasError.length()) {
        mErrorMessage = QObject::tr("a CeVAS object could not be created");

        return;
    }

    // Copy all (i.e. both local and imported) model-level units to the exported
    // model

    ObjRef<iface::cellml_api::UnitsSet> unitsSet = pModel->allUnits();

    copyUnitsSet(unitsSet, exportedModel);

    // Annotate imported components since they may have been renamed

    annotateImportedComponents(pModel);

    // Copy all needed components to the exported model

    copyComponents(cevas);

    // Copy all groups to the exported model

    copyGroups(pModel);

    // Copy all connections to the exported model

    copyConnections(pModel);

    // Deal with 'initial_value="var_name"' occurrences

    propagateInitialValues();

    // Save the exported model

    mResult = saveModel(exportedModel, pFileName);
}

//==============================================================================

void CellmlFileCellml10Exporter::copyUnitsSet(iface::cellml_api::UnitsSet *pUnitsSet,
                                              iface::cellml_api::CellMLElement *pElement)
{
    // Copy all the units to the model/component element, should there be some
    // Note: the element is in a different model to the units, so we can't just
    //       clone them...

    if (!pUnitsSet->length())
        return;

    ObjRef<iface::cellml_api::Model> model = pElement->modelElement();
    ObjRef<iface::cellml_api::UnitsIterator> unitsIterator = pUnitsSet->iterateUnits();

    forever {
        ObjRef<iface::cellml_api::Units> units = unitsIterator->nextUnits();

        if (!units)
            break;

        // Copy the units to the model/component element, but only if it isn't
        // already in this model

        ObjRef<iface::cellml_api::Model> unitsModel = units->modelElement();

        if (unitsModel != model) {
            // Only copy units which haven't already been copied

            std::wstring unitsName = units->name();
            QPair<QString, QString> unitsPair(QString::fromStdWString(unitsName),
                                              QString::fromStdWString(unitsModel->name()));

            if (!mCopiedUnits.contains(unitsPair)) {
                // Keep track of the units pair

                mCopiedUnits << unitsPair;

                // Create a new units

                ObjRef<iface::cellml_api::Units> newUnits = model->createUnits();

                newUnits->name(unitsName);

                bool newUnitsIsBaseUnits = units->isBaseUnits();

                if (newUnitsIsBaseUnits)
                    newUnits->isBaseUnits(newUnitsIsBaseUnits);

                // Copy the units' unit references to the new units

                ObjRef<iface::cellml_api::UnitSet> unitSet = units->unitCollection();

                if (unitSet->length()) {
                    ObjRef<iface::cellml_api::UnitIterator> unitIterator = unitSet->iterateUnits();

                    forever {
                        ObjRef<iface::cellml_api::Unit> unit = unitIterator->nextUnit();

                        if (!unit)
                            break;

                        // Create a new unit reference

                        ObjRef<iface::cellml_api::Unit> newUnit = model->createUnit();

                        newUnit->prefix(unit->prefix());
                        newUnit->multiplier(unit->multiplier());
                        newUnit->offset(unit->offset());
                        newUnit->exponent(unit->exponent());
                        newUnit->units(unit->units());

                        // Add the unit reference to the new units

                        newUnits->addElement(newUnit);
                    }
                }

                // Add the new units to the element

                pElement->addElement(newUnits);
            }
        }
    }
}

//==============================================================================

iface::cellml_api::CellMLComponent * CellmlFileCellml10Exporter::findRealComponent(iface::cellml_api::CellMLComponent *pComponent)
{
    // Find the 'real' component for the given component
    // Note #1: we use CeVAS's algorithm for this, i.e. if the component is an
    //          ImportComponent, then we return the template CellMLComponent
    //          that it is based on in an imported model, otherwise we return
    //          the given component. If no 'real' component exists, then we
    //          return 0...
    // Note #2: we require that a component is not imported more than once in
    //          the same import element, as then multiple components share one
    //          real component...

    pComponent->add_ref();

    forever {
        ObjRef<iface::cellml_api::ImportComponent> importComponent = QueryInterface(pComponent);

        if (!importComponent)
            // We are dealing with a non-import component

            break;

        pComponent->release_ref();

        ObjRef<iface::cellml_api::CellMLElement> importComponentParentElement = importComponent->parentElement();
        ObjRef<iface::cellml_api::CellMLImport> import = QueryInterface(importComponentParentElement);
        ObjRef<iface::cellml_api::Model> model = import->importedModel();
        ObjRef<iface::cellml_api::CellMLComponentSet> components = model->modelComponents();

        pComponent = components->getComponent(importComponent->componentRef());

        if (!pComponent)
            // No 'real' component exists

            break;
    }

    return pComponent;
}

//==============================================================================

void CellmlFileCellml10Exporter::annotateImportedComponents(iface::cellml_api::Model *pModel)
{
    // Recursively annotate all imported components with the name they are given
    // by the importing model

    ObjRef<iface::cellml_api::CellMLImportSet> imports = pModel->imports();

    if (imports->length()) {
        // The model has some imports, so go through them

        ObjRef<iface::cellml_api::CellMLImportIterator> importsIterator = imports->iterateImports();

        forever {
            ObjRef<iface::cellml_api::CellMLImport> import = importsIterator->nextImport();

            if (!import)
                break;

            // We have an import, so recursively annotate the components
            // imported by the given model
            // Note: depth first processing means that imported components
            //       renamed twice end up with the right name...

            ObjRef<iface::cellml_api::Model> importedModel = import->importedModel();

            annotateImportedComponents(importedModel);

            // Annotate the imported components

            ObjRef<iface::cellml_api::ImportComponentSet> importComponents = import->components();

            if (importComponents->length()) {
                // The import includes some imported components, so go through
                // them and annotate them with the name they are given by the
                // importing model

                ObjRef<iface::cellml_api::ImportComponentIterator> importComponentsIterator = importComponents->iterateImportComponents();

                forever {
                    ObjRef<iface::cellml_api::ImportComponent> importComponent = importComponentsIterator->nextImportComponent();

                    if (!importComponent)
                        break;

                    // We have an imported component, so annotate it

                    ObjRef<iface::cellml_api::CellMLComponent> realComponent = findRealComponent(importComponent);

                    mAnnotationSet->setStringAnnotation(realComponent, L"Renamed", importComponent->name());
                }
            }
        }
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::copyComponents(iface::cellml_services::CeVAS *pCevas)
{
Q_UNUSED(pCevas);

//---GRY--- TO BE DONE...
}

//==============================================================================

void CellmlFileCellml10Exporter::copyConnections(iface::cellml_api::Model *pModel)
{
Q_UNUSED(pModel);

//---GRY--- TO BE DONE...
}

//==============================================================================

void CellmlFileCellml10Exporter::copyGroups(iface::cellml_api::Model *pModel)
{
Q_UNUSED(pModel);

//---GRY--- TO BE DONE...
}

//==============================================================================

void CellmlFileCellml10Exporter::propagateInitialValues()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
