//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#include "cellmlfilecellml10exporter.h"

//==============================================================================

#include <QApplication>

//==============================================================================

#include "IfaceCeVAS.hxx"

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

    // Save the exported model

    mResult = saveModel(exportedModel, pFileName);
}

//==============================================================================

void CellmlFileCellml10Exporter::copyUnitsSet(iface::cellml_api::UnitsSet *pUnitsSet,
                                              iface::cellml_api::CellMLElement *pElement)
{
    // Copy all the units to the model/component element
    // Note: the element is in a different model to the units, so we can't just
    //       clone them...

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

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
