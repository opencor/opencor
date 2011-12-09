//==============================================================================
// CellML model class
//==============================================================================

#include "cellmlmodel.h"

//==============================================================================

#include <QUrl>

//==============================================================================

#include "CellMLBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

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
}

//==============================================================================

bool CellmlModel::load()
{
    if (mModel) {
        // The model is already loaded, so...

        return true;
    } else {
        // Try to load the model

        mErrorMessages.clear();

        try {
            mModel = mModelLoader->loadFromURL(QUrl::fromLocalFile(mFileName).toString().toStdWString().c_str());
        } catch (iface::cellml_api::CellMLException &) {
            // Something went wrong with the loading of the model, generate an
            // error message

            mErrorMessages.append(QString("CellML model loader error: %1").arg(QString::fromWCharArray(mModelLoader->lastErrorMessage())));

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
        // Everything went as expected, so...

        return true;
    } else {
        // Something went wrong with the loading of the model, so...

        return false;
    }
}

//==============================================================================

QStringList CellmlModel::errorMessages()
{
    // Return the error message(s)

    return mErrorMessages;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
