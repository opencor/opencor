//==============================================================================
// CellML element item
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLELEMENTITEM_H
#define CELLMLANNOTATIONVIEWCELLMLELEMENTITEM_H

//==============================================================================

#include "cellmlfile.h"

//==============================================================================

#include <QStandardItem>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlElementItem : public QStandardItem
{
public:
    enum Type
    {
        None                  = QStandardItem::UserType,
        Error                 = QStandardItem::UserType+1,
        Warning               = QStandardItem::UserType+2,
        Model                 = QStandardItem::UserType+3,
        Import                = QStandardItem::UserType+4,
        ImportUnit            = QStandardItem::UserType+5,
        ImportComponent       = QStandardItem::UserType+6,
        Unit                  = QStandardItem::UserType+7,
        UnitElement           = QStandardItem::UserType+8,
        Component             = QStandardItem::UserType+9,
        Variable              = QStandardItem::UserType+10,
        Group                 = QStandardItem::UserType+11,
        RelationshipReference = QStandardItem::UserType+12,
        ComponentReference    = QStandardItem::UserType+13,
        Connection            = QStandardItem::UserType+14,
        ComponentMapping      = QStandardItem::UserType+15,
        VariableMapping       = QStandardItem::UserType+16
    };

    explicit CellmlElementItem(const bool &pError, const QString &pText);
    explicit CellmlElementItem(const Type &pType, const QString &pText);
    explicit CellmlElementItem(const Type &pType,
                               CellMLSupport::CellmlFileElement *pElement,
                               const int pNumber = -1);

    bool isCategory() const;
    virtual int type() const;
    int number() const;

    CellMLSupport::CellmlFileElement * element() const;

private:
    bool mCategory;
    Type mType;
    int mNumber;

    CellMLSupport::CellmlFileElement *mElement;

    void setIcon(const Type &pType);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
