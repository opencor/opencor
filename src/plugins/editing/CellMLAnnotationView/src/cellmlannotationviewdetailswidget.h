//==============================================================================
// CellML annotation view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QScrollArea>
#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewDetailsWidget;
}

//==============================================================================

class QFormLayout;
class QLabel;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewDetailsWidget : public QScrollArea,
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
        QString name;
    };

    typedef QList<Item> Items;

    explicit CellmlAnnotationViewDetailsWidget(QWidget *pParent,
                                               CellMLSupport::CellmlFile *pCellmlFile);
    ~CellmlAnnotationViewDetailsWidget();

    virtual void retranslateUi();

    static Item item(const Type &pType,
                     CellMLSupport::CellmlFileElement *pElement,
                     const QString &pName = QString());

    void updateGui(const Items &pItems);
    void updateGui(const QString &pMetadataGroupName);

    QWidget * focusProxyWidget();

private:
    Ui::CellmlAnnotationViewDetailsWidget *mGui;

    CellMLSupport::CellmlFile *mCellmlFile;

    Items mItems;
    QString mMetadataGroupName;

    QWidget *mWidget;
    QFormLayout *mFormLayout;
    QLineEdit *mCmetaIdValue;

    void addRowToFormLayout(const QString &pLabel, const QString &pValue);

    void updateGui(const Items &pItems, const QString &pMetadataGroupName);

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
