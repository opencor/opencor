//==============================================================================
// CellML file element
//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfilemanager.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Model *pModel) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pModel)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLImport *pCellmlImport) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pCellmlImport)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ImportUnits *pImportUnits) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pImportUnits)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ImportComponent *pImportComponent) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pImportComponent)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Units *pUnits) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pUnits)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Unit *pUnit) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pUnit)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLComponent *pComponent) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pComponent)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::CellMLVariable *pVariable) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pVariable)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Group *pGroup) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pGroup)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::RelationshipRef *pRelationshipRef) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pRelationshipRef)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::ComponentRef *pComponentRef) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pComponentRef)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::Connection *pConnection) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pConnection)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::MapComponents *pMapComponents) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pMapComponents)
{
}

//==============================================================================

CellmlFileElement::CellmlFileElement(CellmlFile *pCellmlFile,
                                     iface::cellml_api::MapVariables *pMapVariables) :
    mCellmlFile(pCellmlFile),
    mCellmlElement(pMapVariables)
{
}

//==============================================================================

CellmlFile * CellmlFileElement::cellmlFile() const
{
    // Return the CellML file from which the CellML element comes

    return mCellmlFile;
}

//==============================================================================

QString CellmlFileElement::cmetaId() const
{
    // Return the CellML element's cmeta:id

    return QString::fromStdWString(mCellmlElement->cmetaId());
}

//==============================================================================

void CellmlFileElement::setCmetaId(const QString &pCmetaId)
{
    // Set the CellML element's new cmeta:id

    if (pCmetaId.compare(cmetaId())) {
        // The cmeta:id is different, so update it

        mCellmlElement->cmetaId(pCmetaId.toStdWString());

        mCellmlFile->setModified(true);
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
