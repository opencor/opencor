//==============================================================================
// CellML file import
//==============================================================================

#include "cellmlfileimport.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImport::CellmlFileImport(const QString &pUri) :
    mUri(pUri)
{
}

//==============================================================================

void CellmlFileImport::addUnits(const QString &pName,
                                const QString &pReferenceName)
{
    // Add the units import to our list

    mUnits.insert(pName, pReferenceName);
}

//==============================================================================

void CellmlFileImport::addComponent(const QString &pName,
                                    const QString &pReferenceName)
{
    // Add the component import to our list

    mComponents.insert(pName, pReferenceName);
}

//==============================================================================

QString CellmlFileImport::uri() const
{
    // Return's the import's URI

    return mUri;
}

//==============================================================================

QMap<QString, QString> CellmlFileImport::units() const
{
    // Return's the import's units

    return mUnits;
}

//==============================================================================

QMap<QString, QString> CellmlFileImport::components() const
{
    // Return's the import's components

    return mComponents;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
