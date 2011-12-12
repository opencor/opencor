//==============================================================================
// CellML model class
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

CellmlModelIssue::CellmlModelIssue(const Type &pType, const uint32_t &pLine,
                                   const uint32_t &pColumn,
                                   const QString &pMessage) :
    mType(pType),
    mLine(pLine),
    mColumn(pColumn),
    mMessage(pMessage)
{
}

//==============================================================================

CellmlModelIssue::Type CellmlModelIssue::type() const
{
    // Return the issue's type

    return mType;
}

//==============================================================================

uint32_t CellmlModelIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

uint32_t CellmlModelIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString CellmlModelIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

CellmlModel::CellmlModel(const QString &pFileName) :
    mFileName(pFileName),
    mModel(0)
{
    // Get a bootstrap object

    mCellmlBootstrap = CreateCellMLBootstrap();

    // Retrieve the model loader

    mModelLoader = mCellmlBootstrap->modelLoader();
}

//==============================================================================

void CellmlModel::reset()
{
    // Reset all of the model's properties

    delete mModel; mModel = 0;

    mIssues.clear();
}

//==============================================================================

bool CellmlModel::load()
{
    if (mModel) {
        // The model is already loaded, so...

        return true;
    } else {
        // Try to load the model

        try {
            mModel = mModelLoader->loadFromURL(QUrl::fromLocalFile(mFileName).toString().toStdWString().c_str());
        } catch (iface::cellml_api::CellMLException &) {
            // Something went wrong with the loading of the model, generate an
            // error message

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            CellMLSupport::Undefined,
                                            CellMLSupport::Undefined,
                                            QString("the model could not be loaded (%1)").arg(QString::fromWCharArray(mModelLoader->lastErrorMessage()))));

            return false;
        }

        // In the case of a non CellML 1.0 model, we want all imports to be
        // fully instantiated

        if (QString::fromWCharArray(mModel->cellmlVersion()).compare(Cellml_1_0))
            mModel->fullyInstantiateImports();

        // All done, so...

        return true;
    }
}

//==============================================================================

bool CellmlModel::reload()
{
    // We want to reload the model, so we must first reset it

    reset();

    // Now, we can try to load the model

    return load();
}

//==============================================================================

bool CellmlModel::isValid()
{
    // Load (but not reload!) the model, if needed

    if (load()) {
        // The model was properly loaded (or was already loaded), so check
        // whether it is CellML valid

        ObjRef<iface::cellml_services::VACSService> vacss = CreateVACSService();
        ObjRef<iface::cellml_services::CellMLValidityErrorSet> cellmlValidityErrorSet = vacss->validateModel(mModel);

        // Determine the number of errors and warnings
        // Note: CellMLValidityErrorSet::nValidityErrors() returns any type of
        //       validation issue, be it an error or a warning, so we need to
        //       determine the number of true errors

        uint32_t nbOfCellmlIssues = cellmlValidityErrorSet->nValidityErrors();
        uint32_t nbOfCellmlErrors = 0;

        for (uint32_t i = 0; i < nbOfCellmlIssues; ++i) {
            ObjRef<iface::cellml_services::CellMLValidityError> cellmlValidityIssue = cellmlValidityErrorSet->getValidityError(i);
            CellmlModelIssue::Type issueType;

            // Determine the issue's location

            DECLARE_QUERY_INTERFACE_OBJREF(cellmlRepresentationValidityError, cellmlValidityIssue,
                                           cellml_services::CellMLRepresentationValidityError);

            uint32_t line = CellMLSupport::Undefined;
            uint32_t column = CellMLSupport::Undefined;

            if (cellmlRepresentationValidityError) {
                ObjRef<iface::dom::Node> errorNode = cellmlRepresentationValidityError->errorNode();

                line = vacss->getPositionInXML(errorNode,
                                               cellmlRepresentationValidityError->errorNodalOffset(),
                                               &column);
            } else {
                DECLARE_QUERY_INTERFACE_OBJREF(cellmlSemanticValidityError, cellmlValidityIssue,
                                               cellml_services::CellMLSemanticValidityError);

                if (cellmlSemanticValidityError) {
                    ObjRef<iface::cellml_api::CellMLElement> cellmlElement = cellmlSemanticValidityError->errorElement();

                    DECLARE_QUERY_INTERFACE_OBJREF(cellmlDomElement, cellmlElement, cellml_api::CellMLDOMElement);

                    ObjRef<iface::dom::Element> domElement = cellmlDomElement->domElement();

                    line = vacss->getPositionInXML(domElement, 0, &column);
                }
            }

            // Determine the issue's type

            if (cellmlValidityIssue->isWarningOnly()) {
                // We are dealing with a warning

                issueType = CellmlModelIssue::Warning;
            } else {
                // We are dealing with an error

                ++nbOfCellmlErrors;

                issueType = CellmlModelIssue::Error;
            }

            // Append the issue to our list

            mIssues.append(CellmlModelIssue(issueType, line, column,
                                            QString::fromStdWString(cellmlValidityIssue->description())));
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
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
