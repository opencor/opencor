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

class QLabel;

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
        VariableMapping,
        Metadata
    };

    explicit CellmlAnnotationViewDetailsWidget(QWidget *pParent);
    ~CellmlAnnotationViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const Type &pType,
                   CellMLSupport::CellmlFileElement *pElement = 0,
                   const bool &pNeedRetranslating = false);

private:
    Ui::CellmlAnnotationViewDetailsWidget *mGui;

    Type mType;
    CellMLSupport::CellmlFileElement *mElement;

    QLabel *mCmetaIdLabel;
    QLineEdit *mCmetaIdValue;

    QLabel *mNameLabel;
    QLabel *mNameValue;

    QLabel *mXlinkHrefLabel;
    QLabel *mXlinkHrefValue;

    QLabel *mUnitReferenceLabel;
    QLabel *mUnitReferenceValue;

    QLabel *mComponentReferenceLabel;
    QLabel *mComponentReferenceValue;

    QLabel *mUnitLabel;
    QLabel *mUnitValue;

    QLabel *mInitialValueLabel;
    QLabel *mInitialValueValue;

    QLabel *mPublicInterfaceLabel;
    QLabel *mPublicInterfaceValue;

    QLabel *mPrivateInterfaceLabel;
    QLabel *mPrivateInterfaceValue;

    QLabel *mRelationshipLabel;
    QLabel *mRelationshipValue;

    QLabel *mRelationshipNamespaceLabel;
    QLabel *mRelationshipNamespaceValue;

    QLabel *mComponentLabel;
    QLabel *mComponentValue;

    QLabel *mFirstComponentLabel;
    QLabel *mFirstComponentValue;

    QLabel *mSecondComponentLabel;
    QLabel *mSecondComponentValue;

    QLabel *mFirstVariableLabel;
    QLabel *mFirstVariableValue;

    QLabel *mSecondVariableLabel;
    QLabel *mSecondVariableValue;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
