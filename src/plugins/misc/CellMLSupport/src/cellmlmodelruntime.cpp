//==============================================================================
// CellML model runtime class
//==============================================================================

#include "cellmlmodelruntime.h"

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCeVAS.hxx"
#include "IfaceCUSES.hxx"

#include "CeVASBootstrap.hpp"
#include "CUSESBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

bool CellmlModelRuntime::isValid()
{
    // The runtime is valid if no issue was found

    return mIssues.isEmpty();
}

//==============================================================================

QList<CellmlModelIssue> CellmlModelRuntime::issues()
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

CellmlModelRuntime * CellmlModelRuntime::update(iface::cellml_api::Model *pModel,
                                                const bool &pValidModel)
{
    // Reset ourselves

    reset();

    // Check that the model is valid

    if (pValidModel) {
        // The model is valid, so we can generate our runtime
        // Note: this is based on CodeGenerationState::GenerateCode() from the
        //       CellML API. Indeed, we can't use the CellML API to generate our
        //       runtime, but we can certainly re-use it, so...

        // Create a CellML Variable Association Service (CeVAS) object to help
        // us with the treatment of interconnected CellML variables

        ObjRef<iface::cellml_services::CeVASBootstrap> cevasBootstrap = CreateCeVASBootstrap();
        ObjRef<iface::cellml_services::CeVAS> cevas = cevasBootstrap->createCeVASForModel(pModel);
        QString cevasModelError = QString::fromStdWString(cevas->modelError());

        if (!cevasModelError.isEmpty()) {
            // Something went wrong with the creation of our CeVAS object, so...

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model's variables could not be fully analysed")));

            return this;
        }

        // Create a CellML Units Simplification and Expansion Service (CUSES)
        // object to help us with the processing of physical units

        ObjRef<iface::cellml_services::CUSESBootstrap> cusesBootstrap = CreateCUSESBootstrap();
        ObjRef<iface::cellml_services::CUSES> cuses = cusesBootstrap->createCUSESForModel(pModel, false);
        QString cusesModelError = QString::fromStdWString(cuses->modelError());

        if (!cusesModelError.isEmpty()) {
            // Something went wrong with the creation of our CUSES object, so...

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model's units could not be fully processed")));

            reset();

            return this;
        }
    } else {
        // The model is not valid, so...

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the model is not valid")));
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

void CellmlModelRuntime::reset()
{
    // Reset all of the runtime's properties

//---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
