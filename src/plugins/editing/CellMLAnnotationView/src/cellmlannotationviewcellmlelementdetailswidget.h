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
class QLineEdit;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlDetailsWidget;

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

    explicit CellmlAnnotationViewCellmlElementDetailsWidget(CellmlAnnotationViewCellmlDetailsWidget *pParent);
    ~CellmlAnnotationViewCellmlElementDetailsWidget();

    virtual void retranslateUi();

    virtual QWidget * focusProxyWidget() const;

    static Item item(const Type &pType,
                     CellMLSupport::CellmlFileElement *pElement,
                     const int &pNumber = -1);

    void updateGui(const Items &pItems);
    void finalizeGui();

private:
    CellmlAnnotationViewCellmlDetailsWidget *mParent;

    Ui::CellmlAnnotationViewCellmlElementDetailsWidget *mGui;

    Items mItems;

    QWidget *mWidget;
    QFormLayout *mLayout;
    QLineEdit *mCmetaIdValue;

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
