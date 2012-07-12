//==============================================================================
// CellML annotation view CellML element details widget
//==============================================================================

#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewcellmlelementdetailswidget.h"
#include "cellmlannotationviewcellmlelementitem.h"
#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationviewcellmlmetadatadetailswidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewmetadatalistwidget.h"
#include "cellmlannotationviewwidget.h"
#include "coreutils.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmlelementdetailswidget.h"

//==============================================================================

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

bool CellmlAnnotationViewCellmlElementDetailsWidget::Item::operator==(const Item &pItem) const
{
    // Return whether the given item is equal to the current one

    return    (type == pItem.type)
           && (element == pItem.element)
           && (number == pItem.number);
}

//==============================================================================

CellmlAnnotationViewCellmlElementDetailsWidget::CellmlAnnotationViewCellmlElementDetailsWidget(CellmlAnnotationViewWidget *pParent,
                                                                                               CellMLSupport::CellmlFile *pCellmlFile) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mCellmlFile(pCellmlFile),
    mGui(new Ui::CellmlAnnotationViewCellmlElementDetailsWidget),
    mItems(Items()),
    mCmetaIdValue(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create the widget (and its layout) which will contain our GUI

    mWidget = new QWidget(this);
    mLayout = new QFormLayout(mWidget);

    mWidget->setLayout(mLayout);

    // Add our widget to our scroll area

    setWidget(mWidget);
}

//==============================================================================

CellmlAnnotationViewCellmlElementDetailsWidget::~CellmlAnnotationViewCellmlElementDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::retranslateUi()
{
    // Retranslate our GUI
    // Note: we must also update the connection for our cmeta:id widget since it
    //       gets recreated as a result calling updateGui()...

    if (mCmetaIdValue)
        disconnect(mCmetaIdValue, SIGNAL(editTextChanged(const QString &)),
                   this, SLOT(updateCellmlElementMetadataDetails(const QString &)));

    mGui->retranslateUi(this);

    updateGui();

    if (mCmetaIdValue)
        connect(mCmetaIdValue, SIGNAL(editTextChanged(const QString &)),
                this, SLOT(updateCellmlElementMetadataDetails(const QString &)));
}

//==============================================================================

CellmlAnnotationViewCellmlElementDetailsWidget::Item CellmlAnnotationViewCellmlElementDetailsWidget::item(const Type &pType,
                                                                                                          CellMLSupport::CellmlFileElement *pElement,
                                                                                                          const int &pNumber)
{
    // Return a formatted Item 'object'

    Item res;

    res.type    = pType;
    res.element = pElement;
    res.number  = pNumber;

    return res;
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::updateGui(const Items &pItems)
{
    // Stop tracking changes to the cmeta:id value of our CellML element

    if (mCmetaIdValue)
        disconnect(mCmetaIdValue, SIGNAL(editTextChanged(const QString &)),
                   this, SLOT(updateCellmlElementMetadataDetails(const QString &)));

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Keep track of the items

    mItems = pItems;

    // Reset our cmeta:id value widget (in case there are no items to show)

    mCmetaIdValue = 0;

    // Remove everything from our form layout

    for (int i = 0, iMax = mLayout->count(); i < iMax; ++i) {
        QLayoutItem *item = mLayout->takeAt(0);

        delete item->widget();
        delete item;
    }

    // Go through the different items which properties we want to add to the GUI

    for (int i = 0, iLast = pItems.count()-1; i <= iLast; ++i) {
        Item item = pItems[i];

        // Determine which widget should be shown/hidden

        bool showName = false;
        bool showXlinkHref = false;
        bool showUnitReference = false;
        bool showComponentReference = false;
        bool showUnit = false;
        bool showInitialValue = false;
        bool showPublicInterface = false;
        bool showPrivateInterface = false;
        bool showRelationship = false;
        bool showRelationshipNamespace = false;
        bool showComponent = false;
        bool showFirstComponent = false;
        bool showSecondComponent = false;
        bool showFirstVariable = false;
        bool showSecondVariable = false;

        switch (item.type) {
        case Model:
        case Unit:
        case UnitElement:
        case Component:
        case Group:
        case Connection:
            showName = true;

            break;
        case Import:
            showXlinkHref = true;

            break;
        case ImportUnit:
            showName = true;
            showUnitReference = true;

            break;
        case ImportComponent:
            showName = true;
            showComponentReference = true;

            break;
        case Variable:
            showName = true;
            showUnit = true;
            showInitialValue = true;
            showPublicInterface = true;
            showPrivateInterface = true;

            break;
        case RelationshipReference:
            showRelationship = true;
            showRelationshipNamespace = true;

            break;
        case ComponentReference:
            showComponent = true;

            break;
        case ComponentMapping:
            showFirstComponent = true;
            showSecondComponent = true;

            break;
        case VariableMapping:
            showFirstVariable = true;
            showSecondVariable = true;

            break;
        };

        // Add whatever we need
        // Note: as long as all of the widgets' parent is our widget, then they
        //       will get automatically deleted, so no need to delete them in
        //       ~CellmlAnnotationViewCellmlElementDetailsWidget()...

        // Add a bold centered label as a header to let the user know what type
        // of item we are talking about

        mLayout->addRow(Core::newLabel(mWidget, typeAsString(item.type),
                                       true, 1.25, Qt::AlignCenter));

        // Show the item's cmeta:id, keeping in mind that we only want to allow
        // the editing of the cmeta:id of the very first item

        QString cmetaId = item.element->cmetaId();

        if (i == iLast) {
            // This is our 'main' item, so we want to allow the user to edit its
            // cmeta:id. For this, we use a QComboBox, but we also want to have
            // a QPushButton to allow the user to switch to the editing mode of
            // the metadata associated with the cmeta:id, so we create a meta
            // widget which contains both a QComboBox and a QPushButton

            QWidget *cmetaIdWidget = new QWidget(mWidget);

            QHBoxLayout *cmetaIdWidgetLayout = new QHBoxLayout(cmetaIdWidget);

            cmetaIdWidgetLayout->setMargin(0);

            cmetaIdWidget->setLayout(cmetaIdWidgetLayout);

            // Create our cmeta:id value widget

            mCmetaIdValue = new QComboBox(mWidget);

            mCmetaIdValue->addItems(mParent->metadataIds());

            mCmetaIdValue->setEditable(true);

            bool cmetaIdFound = false;

            for (int i = 0, iMax = mCmetaIdValue->count(); i < iMax; ++i)
                if (!cmetaId.compare(mCmetaIdValue->itemText(i))) {
                    // The cmeta:id could be found in our list of metadata ids,
                    // so set our cmeta:id index to it

                    mCmetaIdValue->setCurrentIndex(i);

                    cmetaIdFound = true;

                    break;
                }

            if (!cmetaIdFound)
                // The cmeta:id couldn't be found in our list of metadata ids,
                // so 'manually' set it

                mCmetaIdValue->setEditText(cmetaId);

            // Create our edit button widget

            QPushButton *editButton = new QPushButton(mWidget);
            // Note #1: ideally, we could assign a QAction to our QPushButton,
            //          but this cannot be done, so... we assign a few
            //          properties by hand...
            // Note #2: to use a QToolButton would allow us to assign a QAction
            //          to it, but a QToolButton doesn't look quite the same as
            //          a QPushButton on some platforms, so...

            editButton->setIcon(QIcon(":/oxygen/actions/format-list-unordered.png"));
            editButton->setStatusTip(tr("Edit the metadata"));
            editButton->setToolTip(tr("Edit"));
            editButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            connect(editButton, SIGNAL(clicked()),
                    this, SLOT(editMetadata()));

            // Add our QComboBox and QPushButton to our cmeta:id widget

            cmetaIdWidgetLayout->addWidget(mCmetaIdValue);
            cmetaIdWidgetLayout->addWidget(editButton);

            // Add our cmeta:id widget to our main layout

            mLayout->addRow(Core::newLabel(mWidget, tr("cmeta:id:"), true),
                            cmetaIdWidget);

            // Let people know that the GUI has been populated

            emit guiPopulated(mCmetaIdValue, editButton);

            // Create a connection to let people know whenever the cmeta:id
            // value has changed

            connect(mCmetaIdValue, SIGNAL(editTextChanged(const QString &)),
                    this, SIGNAL(cmetaIdChanged(const QString &)));
        } else {
            // Not our 'main' item, so just display its cmeta:id

            addRow(tr("cmeta:id:"), cmetaId.isEmpty()?"/":cmetaId);
        }

        // Show the item's remaining properties

        if (showName) {
            // Retrieve the name of the CellML element
            // Note: in the case of a group or a connection, there won't be a
            //       name, so we use the item's name, hoping one was provided...

            QString name = ((item.type == Group) || (item.type == Connection))?
                               (item.number == -1)?
                                   "/":
                                   (item.type == Group)?
                                       tr("Group #%1").arg(item.number):
                                       tr("Connection #%1").arg(item.number):
                               static_cast<CellMLSupport::CellmlFileNamedElement *>(item.element)->name();

            addRow(tr("Name:"), name);
        }

        if (showXlinkHref)
            addRow(tr("xlink:href:"),
                   static_cast<CellMLSupport::CellmlFileImport *>(item.element)->xlinkHref());

        if (showUnitReference)
            addRow(tr("Unit reference:"),
                   static_cast<CellMLSupport::CellmlFileImportUnit *>(item.element)->unitReference());

        if (showComponentReference)
            addRow(tr("Component reference:"),
                   static_cast<CellMLSupport::CellmlFileImportComponent *>(item.element)->componentReference());

        if (showUnit)
            addRow(tr("Unit:"),
                   static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->unit());

        if (showInitialValue) {
            QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->initialValue();

            addRow(tr("Initial value:"),
                   initialValue.isEmpty()?"/":initialValue);
        }

        if (showPublicInterface)
            addRow(tr("Public interface:"),
                   static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->publicInterfaceAsString());

        if (showPrivateInterface)
            addRow(tr("Private interface:"),
                   static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->privateInterfaceAsString());

        if (showRelationship)
            addRow(tr("Relationship:"),
                   static_cast<CellMLSupport::CellmlFileRelationshipReference *>(item.element)->relationship());

        if (showRelationshipNamespace) {
            QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(item.element)->relationshipNamespace();

            addRow(tr("Relationship namespace:"),
                   relationshipNamespace.isEmpty()?"/":relationshipNamespace);
        }

        if (showComponent)
            addRow(tr("Component:"),
                   static_cast<CellMLSupport::CellmlFileComponentReference *>(item.element)->component());

        if (showFirstComponent)
            addRow(tr("First component:"),
                   static_cast<CellMLSupport::CellmlFileMapComponents *>(item.element)->firstComponent());

        if (showSecondComponent)
            addRow(tr("Second component:"),
                   static_cast<CellMLSupport::CellmlFileMapComponents *>(item.element)->secondComponent());

        if (showFirstVariable)
            addRow(tr("First variable:"),
                   static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(item.element)->firstVariable());

        if (showSecondVariable)
            addRow(tr("Second variable:"),
                   static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(item.element)->secondVariable());
    }

    // Scroll down to the bottom of ourselves, just in case things don't fit
    // within the viewport

    qApp->processEvents();

    verticalScrollBar()->setValue(verticalScrollBar()->maximum());

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);

    // Re-track changes to the cmeta:id value of our CellML element and update
    // our metadata details GUI

    if (mCmetaIdValue) {
        connect(mCmetaIdValue, SIGNAL(editTextChanged(const QString &)),
                this, SLOT(updateCellmlElementMetadataDetails(const QString &)));

        updateCellmlElementMetadataDetails(mCmetaIdValue->currentText());
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::updateGui()
{
    // Update the GUI using our existing items
    // Note: this can be as a result of, for example, a need to retranslate the
    //       GUI or a change in the metadata...

    QString cmetaId = mCmetaIdValue?mCmetaIdValue->currentText():QString();

    updateGui(mItems);

    updateCellmlElementMetadataDetails(cmetaId);
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::addRow(const QString &pLabel,
                                                            const QString &pValue)
{
    // Add a row to our form layout

    mLayout->addRow(Core::newLabel(mWidget, pLabel, true),
                    Core::newLabel(mWidget, pValue));
}

//==============================================================================

QString CellmlAnnotationViewCellmlElementDetailsWidget::typeAsString(const Type &pType) const
{
    switch (pType) {
    case Import:
        return tr("Import");
    case ImportUnit:
        return tr("Imported unit");
    case ImportComponent:
        return tr("Imported component");
    case Unit:
        return tr("Unit");
    case UnitElement:
        return tr("Unit element");
    case Component:
        return tr("Component");
    case Variable:
        return tr("Variable");
    case Group:
        return tr("Group");
    case RelationshipReference:
        return tr("Relationshop reference");
    case ComponentReference:
        return tr("Component reference");
    case Connection:
        return tr("Connection");
    case ComponentMapping:
        return tr("Component mapping");
    case VariableMapping:
        return tr("Variable mapping");
    default:
        // Model

        return tr("Model");
    }
}

//==============================================================================

QComboBox * CellmlAnnotationViewCellmlElementDetailsWidget::cmetaIdValue() const
{
    // Return our cmeta:id value widget

    return mCmetaIdValue;
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::updateCellmlElementMetadataDetails(const QString &pCmetaId)
{
    // Retrieve the RDF triples for the cmeta:id

    CellMLSupport::CellmlFileRdfTriples rdfTriples = mCellmlFile->rdfTriples(pCmetaId);

    // Check that we are not dealing with the same RDF triples
    // Note: this may happen when manually typing the name of a cmeta:id and the
    //       QComboBox suggesting something for you, e.g. you start typing "C_"
    //       and the QComboBox suggests "C_C" (which will get us here) and then
    //       you finish typing "C_C" (which will also get us here)

    static CellMLSupport::CellmlFileRdfTriples oldRdfTriples = CellMLSupport::CellmlFileRdfTriples(mCellmlFile);

    if (rdfTriples == oldRdfTriples)
        return;

    oldRdfTriples = rdfTriples;

    // Let people know that we want to see some information about the CellML
    // element metadata

    emit cellmlElementMetadataDetailsRequested(rdfTriples);
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::editMetadata()
{
    // Let people know that we would like to edit the metadata

    emit metadataEditingRequested(mCmetaIdValue->currentText());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
