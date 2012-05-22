//==============================================================================
// CellML annotation view CellML element details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLELEMENTDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWCELLMLELEMENTDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewCellmlElementDetailsWidget;
}

//==============================================================================

class QFormLayout;
class QComboBox;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewCellmlElementDetailsWidget : public QScrollArea,
                                                       public Core::CommonWidget
{
    Q_OBJECT

public:
    enum Type
    {
        Model,
        Import,
        ImportUnit,
        ImportComponent,
        Unit,
        UnitElement,
        Component,
        Variable,
        Group,
        RelationshipReference,
        ComponentReference,
        Connection,
        ComponentMapping,
        VariableMapping
    };

    struct Item
    {
        Type type;
        CellMLSupport::CellmlFileElement *element;
        int number;
    };

    typedef QList<Item> Items;

    explicit CellmlAnnotationViewCellmlElementDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewCellmlElementDetailsWidget();

    virtual void retranslateUi();

    static Item item(const Type &pType,
                     CellMLSupport::CellmlFileElement *pElement,
                     const int &pNumber = -1);

    void updateGui(const Items &pItems);

    QComboBox * cmetaIdValue() const;

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewCellmlElementDetailsWidget *mGui;

    Items mItems;

    QWidget *mWidget;
    QFormLayout *mLayout;
    QComboBox *mCmetaIdValue;

    void addRowToCellmlFormLayout(const QString &pLabel, const QString &pValue);

    QString typeAsString(const Type &pType) const;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
