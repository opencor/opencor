//==============================================================================
// CellML file import
//==============================================================================

#include "cellmlfileimport.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImport::CellmlFileImport(const QString &pCmetaId, const QString &pUri) :
    CellmlFileElement(pCmetaId),
    mUri(pUri),
    mUnits(CellmlFileImportUnits()),
    mComponents(CellmlFileImportComponents())
{
}

//==============================================================================

CellmlFileImport::~CellmlFileImport()
{
    // Delete some internal objects

    foreach (CellmlFileImportUnit *unit, mUnits)
        delete unit;

    foreach (CellmlFileImportComponent *component, mComponents)
        delete component;
}

//==============================================================================

void CellmlFileImport::addUnits(const QString &pCmetaId, const QString &pName,
                                const QString &pReferenceName)
{
    // Add the units import to our list

    mUnits.append(new CellmlFileImportUnit(pCmetaId, pName, pReferenceName));
}

//==============================================================================

void CellmlFileImport::addComponent(const QString &pCmetaId,
                                    const QString &pName,
                                    const QString &pReferenceName)
{
    // Add the component import to our list

    mComponents.append(new CellmlFileImportComponent(pCmetaId, pName,
                                                     pReferenceName));
}

//==============================================================================

QString CellmlFileImport::uri() const
{
    // Return's the import's URI

    return mUri;
}

//==============================================================================

CellmlFileImportUnits CellmlFileImport::units() const
{
    // Return's the import's units

    return mUnits;
}

//==============================================================================

CellmlFileImportComponents CellmlFileImport::components() const
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
