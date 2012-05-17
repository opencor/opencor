//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWCELLMLDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewCellmlDetailsWidget;
}

//==============================================================================

class QFormLayout;
class QLineEdit;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewDetailsWidget;

//==============================================================================

class CellmlAnnotationViewCellmlDetailsWidget : public QScrollArea,
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

    explicit CellmlAnnotationViewCellmlDetailsWidget(CellmlAnnotationViewDetailsWidget *pParent);
    ~CellmlAnnotationViewCellmlDetailsWidget();

    virtual void retranslateUi();

    virtual QWidget * focusProxyWidget() const;

    static Item item(const Type &pType,
                     CellMLSupport::CellmlFileElement *pElement,
                     const int &pNumber = -1);

    void updateGui(const Items &pItems);

private:
    CellmlAnnotationViewDetailsWidget *mParent;

    Ui::CellmlAnnotationViewCellmlDetailsWidget *mGui;

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
