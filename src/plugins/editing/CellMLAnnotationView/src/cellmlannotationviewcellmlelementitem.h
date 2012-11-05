//==============================================================================
// CellML annotation view CellML element item
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLELEMENTITEM_H
#define CELLMLANNOTATIONVIEWCELLMLELEMENTITEM_H

//==============================================================================

#include <QStandardItem>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileElement;
}   // namespace CellMLSupport

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlElementItem : public QStandardItem
{
public:
    enum Type
    {
        Error                 = QStandardItem::UserType,
        Warning               = QStandardItem::UserType+1,
        Model                 = QStandardItem::UserType+2,
        Import                = QStandardItem::UserType+3,
        ImportUnit            = QStandardItem::UserType+4,
        ImportComponent       = QStandardItem::UserType+5,
        Unit                  = QStandardItem::UserType+6,
        UnitElement           = QStandardItem::UserType+7,
        Component             = QStandardItem::UserType+8,
        Variable              = QStandardItem::UserType+9,
        Group                 = QStandardItem::UserType+10,
        RelationshipReference = QStandardItem::UserType+11,
        ComponentReference    = QStandardItem::UserType+12,
        Connection            = QStandardItem::UserType+13,
        ComponentMapping      = QStandardItem::UserType+14,
        VariableMapping       = QStandardItem::UserType+15
    };

    explicit CellmlAnnotationViewCellmlElementItem(const bool &pError, const QString &pText);
    explicit CellmlAnnotationViewCellmlElementItem(const Type &pType, const QString &pText);
    explicit CellmlAnnotationViewCellmlElementItem(const Type &pType,
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
