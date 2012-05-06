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
        MathmlElement,
        Group,
        RelationshipRef,
        ComponentRef,
        Connection,
        ComponentMapping,
        VariableMapping,
        Metadata
    };

    explicit CellmlAnnotationViewDetailsWidget(QWidget *pParent);
    ~CellmlAnnotationViewDetailsWidget();

    virtual void retranslateUi();

    void update(const Type &pType,
                CellMLSupport::CellmlFileElement *pElement = 0);
    void update(const Type &pType,
                CellMLSupport::CellmlFileMathmlElement *pMathmlElement);

private:
    Ui::CellmlAnnotationViewDetailsWidget *mGui;

    Type mType;

    CellMLSupport::CellmlFileElement *mElement;
    CellMLSupport::CellmlFileMathmlElement *mMathmlElement;

    QLabel *mCmetaIdLabel;
    QLineEdit *mCmetaIdValue;

    QLabel *mNameLabel;
    QLabel *mNameValue;

    QLabel *mUriLabel;
    QLabel *mUriValue;

    QLabel *mReferenceNameLabel;
    QLabel *mReferenceNameValue;

    QLabel *mUnitLabel;
    QLabel *mUnitValue;

    QLabel *mInitialValueLabel;
    QLabel *mInitialValueValue;

    QLabel *mPublicInterfaceLabel;
    QLabel *mPublicInterfaceValue;

    QLabel *mPrivateInterfaceLabel;
    QLabel *mPrivateInterfaceValue;

    QLabel *mRelationshipRefLabel;
    QLabel *mRelationshipRefValue;

    QLabel *mComponentRefLabel;
    QLabel *mComponentRefValue;

    void updateGui(const Type &pType,
                   CellMLSupport::CellmlFileElement *pElement,
                   CellMLSupport::CellmlFileMathmlElement *pMathmlElement,
                   const bool &pNeedRetranslating = false);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
