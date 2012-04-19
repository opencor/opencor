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
    mUnitsList(CellmlFileImportUnitsList()),
    mComponentList(CellmlFileImportComponentList())
{
}

//==============================================================================

CellmlFileImport::~CellmlFileImport()
{
    // Delete some internal objects

    foreach (CellmlFileImportUnits *units, mUnitsList)
        delete units;

    foreach (CellmlFileImportComponent *component, mComponentList)
        delete component;
}

//==============================================================================

void CellmlFileImport::addUnits(const QString &pCmetaId, const QString &pName,
                                const QString &pReferenceName)
{
    // Add the units import to our list

    mUnitsList.append(new CellmlFileImportUnits(pCmetaId, pName,
                                                pReferenceName));
}

//==============================================================================

void CellmlFileImport::addComponent(const QString &pCmetaId,
                                    const QString &pName,
                                    const QString &pReferenceName)
{
    // Add the component import to our list

    mComponentList.append(new CellmlFileImportComponent(pCmetaId, pName,
                                                        pReferenceName));
}

//==============================================================================

QString CellmlFileImport::uri() const
{
    // Return's the import's URI

    return mUri;
}

//==============================================================================

CellmlFileImportUnitsList CellmlFileImport::unitsList() const
{
    // Return's the import's list of units

    return mUnitsList;
}

//==============================================================================

CellmlFileImportComponentList CellmlFileImport::componentList() const
{
    // Return's the import's list of components

    return mComponentList;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
