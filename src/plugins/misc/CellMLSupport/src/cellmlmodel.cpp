//==============================================================================
// CellML model class
//==============================================================================
//---GRY--- NOTE THAT OUR CURRENT USE OF THE CellML API IS *WRONG*. INDEED, WE
//          ARE ASSUMING THAT THE BINARIES IMPLEMENT A CLEANED UP C++ INTERFACE
//          (SEE https://tracker.physiomeproject.org/show_bug.cgi?id=3108) WHILE
//          THIS IS NOT (YET) THE CASE. STILL, WE PREFER TO HAVE MEMORY LEAKS,
//          ETC. AND BE READY FOR WHEN BINARIES ARE UPDATED RATHER THAN HAVE
//          UGLY CODE, ETC. THIS BEING SAID, THERE IS STILL THE ISSUE OF
//          DECLARE_QUERY_INTERFACE_OBJREF WHICH CAN'T BE MIMICKED AT THIS POINT
//          (SEE https://tracker.physiomeproject.org/show_bug.cgi?id=3165)
//==============================================================================

#include "cellmlmodel.h"

//==============================================================================

#include <QUrl>

//==============================================================================

#include "IfaceVACSS.hxx"

#include "CellMLBootstrap.hpp"
#include "VACSSBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlModel::CellmlModel(const QString &pFileName) :
    mFileName(pFileName),
    mModel(0)
{
    // Instantiate our runtime object

    mRuntime = new CellmlModelRuntime();
}

//==============================================================================

CellmlModel::~CellmlModel()
{
    // Delete some internal objects

    delete mRuntime;
}

//==============================================================================

void CellmlModel::reset()
{
    // Reset all of the model's properties

    /*delete mModel;*/ mModel = 0;
    //---GRY--- WE CANNOT delete mModel AT THIS STAGE. FOR THIS, WE WOULD NEED
    //          TO USE THE CLEANED UP C++ INTERFACE (SEE THE MAIN COMMENT AT THE
    //          BEGINNING OF THIS FILE)
}

//==============================================================================

bool CellmlModel::load()
{
    if (mModel) {
        // The model is already loaded, so...

        return true;
    } else {
        // Reset any issues that we may have found before

        mIssues.clear();

        // Get a bootstrap object

        ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();

        // Get its model loader

        ObjRef<iface::cellml_api::DOMModelLoader> modelLoader = cellmlBootstrap->modelLoader();

        // Try to load the model

        try {
            mModel = modelLoader->loadFromURL(QUrl::fromLocalFile(mFileName).toString().toStdWString().c_str());
        } catch (iface::cellml_api::CellMLException &) {
            // Something went wrong with the loading of the model, so...

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model could not be loaded (%1)").arg(QString::fromStdWString(modelLoader->lastErrorMessage()))));

            return false;
        }

        // In the case of a non CellML 1.0 model, we want all imports to be
        // fully instantiated

        if (QString::fromStdWString(mModel->cellmlVersion()).compare(Cellml_1_0))
            try {
                mModel->fullyInstantiateImports();
            } catch (...) {
                // Something went wrong with the full instantiation of the
                // imports, so...

                reset();

                mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                                tr("the model's imports could not be fully instantiated")));

                return false;
            }

        // All done, so...

        return true;
    }
}

//==============================================================================

bool CellmlModel::reload()
{
    // We want to reload the model, so we must first reset it

    reset();

    // Now, we can try to (re)load the model

    return load();
}

//==============================================================================

bool CellmlModel::isValid()
{
    // Load (but not reload!) the model, if needed

    if (load()) {
        // The model was properly loaded (or was already loaded), so check
        // whether it is CellML valid

        ObjRef<iface::cellml_services::VACSService> vacssService = CreateVACSService();
        ObjRef<iface::cellml_services::CellMLValidityErrorSet> cellmlValidityErrorSet = vacssService->validateModel(mModel);

        // Determine the number of errors and warnings
        // Note: CellMLValidityErrorSet::nValidityErrors() returns any type of
        //       validation issue, be it an error or a warning, so we need to
        //       determine the number of true errors

        uint32_t nbOfCellmlIssues = cellmlValidityErrorSet->nValidityErrors();
        uint32_t nbOfCellmlErrors = 0;

        for (uint32_t i = 0; i < nbOfCellmlIssues; ++i) {
            // Determine the issue's location

            ObjRef<iface::cellml_services::CellMLValidityError> cellmlValidityIssue = cellmlValidityErrorSet->getValidityError(i);

            DECLARE_QUERY_INTERFACE_OBJREF(cellmlRepresentationValidityError, cellmlValidityIssue,
                                           cellml_services::CellMLRepresentationValidityError);

            uint32_t line = 0;
            uint32_t column = 0;
            QString importedModel = QString();

            if (cellmlRepresentationValidityError) {
                // We are dealing with a CellML representation issue, so
                // determine its line and column

                ObjRef<iface::dom::Node> errorNode = cellmlRepresentationValidityError->errorNode();

                line = vacssService->getPositionInXML(errorNode,
                                                      cellmlRepresentationValidityError->errorNodalOffset(),
                                                      &column);
            } else {
                // We are not dealing with a CellML representation issue, so
                // check whether we are dealing with a semantic one

                DECLARE_QUERY_INTERFACE_OBJREF(cellmlSemanticValidityError, cellmlValidityIssue,
                                               cellml_services::CellMLSemanticValidityError);

                if (cellmlSemanticValidityError) {
                    // We are dealing with a CellML semantic issue, so determine
                    // its line and column

                    ObjRef<iface::cellml_api::CellMLElement> cellmlElement = cellmlSemanticValidityError->errorElement();

                    DECLARE_QUERY_INTERFACE_OBJREF(cellmlDomElement, cellmlElement, cellml_api::CellMLDOMElement);

                    ObjRef<iface::dom::Element> domElement = cellmlDomElement->domElement();

                    line = vacssService->getPositionInXML(domElement, 0,
                                                          &column);

                    // Also determine its imported model, if any

                    while (true) {
                        // Retrieve the CellML element's parent

                        iface::cellml_api::CellMLElement *cellmlElementParent = already_AddRefd<iface::cellml_api::CellMLElement>(cellmlElement->parentElement());

                        if (!cellmlElementParent)
                            // There is no parent, so...

                            break;

                        // Check whether the parent is an imported model

                        DECLARE_QUERY_INTERFACE_OBJREF(importedCellmlModel,
                                                       cellmlElementParent,
                                                       cellml_api::Model);

                        if (!importedCellmlModel)
                            // This is not a model, so...

                            continue;

                        // Retrieve the imported CellML element

                        ObjRef<iface::cellml_api::CellMLElement> importedCellmlElement = importedCellmlModel->parentElement();

                        if (!importedCellmlElement)
                            // This is not an imported CellML element, so...

                            break;

                        // Check whether the imported CellML element is an
                        // import CellML element

                        DECLARE_QUERY_INTERFACE(importCellmlElement,
                                                importedCellmlElement,
                                                cellml_api::CellMLImport);

                        if (!importCellmlElement)
                            // This is not an import CellML element, so...

                            break;

                        ObjRef<iface::cellml_api::URI> href = importCellmlElement->xlinkHref();

                        importedModel = QString::fromStdWString(href->asText());

                        break;
                    }
                }
            }

            // Determine the issue's type

            CellmlModelIssue::Type issueType;

            if (cellmlValidityIssue->isWarningOnly()) {
                // We are dealing with a warning

                issueType = CellmlModelIssue::Warning;
            } else {
                // We are dealing with an error

                ++nbOfCellmlErrors;

                issueType = CellmlModelIssue::Error;
            }

            // Append the issue to our list

            mIssues.append(CellmlModelIssue(issueType,
                                            QString::fromStdWString(cellmlValidityIssue->description()),
                                            line, column, importedModel));
        }

        if (nbOfCellmlErrors)
            // There are CellML errors, so...

            return false;

        // Everything went as expected, so...

        return true;
    } else {
        // Something went wrong with the loading of the model, so...

        return false;
    }
}

//==============================================================================

QList<CellmlModelIssue> CellmlModel::issues()
{
    // Return the model's issue(s)

    return mIssues;
}

//==============================================================================

CellmlModelRuntime * CellmlModel::runtime()
{
    // Return an updated version of our runtime object

    return mRuntime->update(mModel, isValid());
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
