//==============================================================================
// CellML annotation view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewDetailsWidget;
}

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    enum Type
    {
        Empty,
        Model,
        Import,
        Unit,
        UnitElement,
        Component,
        Variable,
        MathmlElement,
        Group,
        RelationshipRef,
        ComponentRef,
        Connection,
        Metadata
    };

    explicit CellmlAnnotationViewDetailsWidget(QWidget *pParent);
    ~CellmlAnnotationViewDetailsWidget();

    virtual void retranslateUi();

    void setEmpty();
    void setModel(CellMLSupport::CellmlFileModel *pModel);
    void setImport();
    void setUnit();
    void setUnitElement();
    void setComponent();
    void setVariable();
    void setMathmlElement();
    void setGroup();
    void setRelationshipRef();
    void setComponentRef();
    void setConnection();
    void setMetadata();

private:
    Ui::CellmlAnnotationViewDetailsWidget *mUi;

    Type mType;

    void setType(const Type &pType);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
