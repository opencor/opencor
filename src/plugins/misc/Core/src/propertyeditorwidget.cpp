//==============================================================================
// Property editor widget
//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

#include <float.h>

//==============================================================================

#include <QKeyEvent>
#include <QStandardItem>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

DoubleEditWidget::DoubleEditWidget(const double &pValue, QWidget *pParent) :
    QLineEdit(QString::number(pValue), pParent)
{
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our editor
#endif
}

//==============================================================================

void DoubleEditWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // None of the modifiers is selected

        if (pEvent->key() == Qt::Key_Up) {
            // The user wants to go to the previous property, so...

            emit goToPreviousPropertyRequested();

            // Accept the event

            pEvent->accept();
        } else if (pEvent->key() == Qt::Key_Down) {
            // The user wants to go to the previous property, so...

            emit goToNextPropertyRequested();

            // Accept the event

            pEvent->accept();
        } else {
            // Default handling of the event

            QLineEdit::keyPressEvent(pEvent);
        }
    } else {
        // Default handling of the event

        QLineEdit::keyPressEvent(pEvent);
    }
}

//==============================================================================

PropertyItemDelegate::PropertyItemDelegate() :
    QStyledItemDelegate(),
    mModel(0)
{
}

//==============================================================================

QWidget * PropertyItemDelegate::createEditor(QWidget *pParent,
                                             const QStyleOptionViewItem &pOption,
                                             const QModelIndex &pIndex) const
{
    Q_UNUSED(pOption);

    // Create and return an editor for our double
    // Note: we don't allow the editing of a string, so...

    QStandardItem *item = mModel->itemFromIndex(pIndex);

    DoubleEditWidget *editor = new DoubleEditWidget(item->text().toDouble(),
                                                    pParent);

    // Propagate a few signals

    connect(editor, SIGNAL(goToPreviousPropertyRequested()),
            this, SIGNAL(goToPreviousPropertyRequested()));
    connect(editor, SIGNAL(goToNextPropertyRequested()),
            this, SIGNAL(goToNextPropertyRequested()));

    // Let people know that there is a new editor

    emit openEditor(editor);

    // Return the editor

    return editor;
}

//==============================================================================

void PropertyItemDelegate::setModel(QStandardItemModel *pModel)
{
    // Set the model to be used by us

    if (pModel != mModel)
        mModel = pModel;
}

//==============================================================================

PropertyItem::PropertyItem(const Type &pType, const QString &pValue,
                           const bool &pEditable) :
    QStandardItem(pValue),
    mType(pType)
{
    // Check whether the item should be editable

    if (!pEditable)
        setFlags(flags() & ~Qt::ItemIsEditable);
}

//==============================================================================

int PropertyItem::type() const
{
    // Return the property item's type

    return mType;
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(QWidget *pParent) :
    TreeViewWidget(pParent),
    mPropertyEditor(0),
    mPropertyRow(-1)
{
    // Create our item delegate and set it, after making sure that we handle a
    // few of its signals

    mPropertyItemDelegate = new PropertyItemDelegate();

    connect(mPropertyItemDelegate, SIGNAL(openEditor(QWidget *)),
            this, SLOT(editorOpened(QWidget *)));
    connect(mPropertyItemDelegate, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(editorClosed()));

    connect(mPropertyItemDelegate, SIGNAL(goToPreviousPropertyRequested()),
            this, SLOT(goToPreviousProperty()));
    connect(mPropertyItemDelegate, SIGNAL(goToNextPropertyRequested()),
            this, SLOT(goToNextProperty()));

    setItemDelegate(mPropertyItemDelegate);

    // Further customise ourself

    setSelectionMode(QAbstractItemView::SingleSelection);
}

//==============================================================================

void PropertyEditorWidget::initialize(QStandardItemModel *pModel)
{
    // Update our model and, as a result, our item delegate

    setModel(pModel);

    mPropertyItemDelegate->setModel(pModel);
}

//==============================================================================

PropertyItem * PropertyEditorWidget::newNonEditableString(const QString &pValue)
{
    // Create and return a non-editable item

    return new PropertyItem(PropertyItem::String, pValue, false);
}

//==============================================================================

PropertyItem * PropertyEditorWidget::newEditableDouble(const double &pValue)
{
    // Create and return an editable item

    return new PropertyItem(PropertyItem::Double, QString::number(pValue));
}

//==============================================================================

void PropertyEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   (pEvent->modifiers() & Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_A)) {
        // The user wants to select everything which we don't want to allow,
        // so just accept the event...

        pEvent->accept();
    } else if (   !(pEvent->modifiers() & Qt::ShiftModifier)
               && !(pEvent->modifiers() & Qt::ControlModifier)
               && !(pEvent->modifiers() & Qt::AltModifier)
               && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // None of the modifiers is selected

        if (   (pEvent->key() == Qt::Key_Return)
            || (pEvent->key() == Qt::Key_Enter)) {
            // The user wants to start/stop editing the property

            if (mPropertyEditor)
                // We are currently editing the property, so stop editing it

                editProperty(-1);
            else
                // We are not currently editing the property, so start editing
                // it

                editProperty(mPropertyRow);

            // Accept the event

            pEvent->accept();
        } else if (pEvent->key() == Qt::Key_Up) {
            // The user wants to go the previous property

            goToPreviousProperty();

            // Accept the event

            pEvent->accept();
        } else if (pEvent->key() == Qt::Key_Down) {
            // The user wants to go to the next property

            goToNextProperty();

            // Accept the event

            pEvent->accept();
        } else {
            // Default handling of the event

            TreeViewWidget::keyPressEvent(pEvent);
        }
    } else {
        // Default handling of the event

        TreeViewWidget::keyPressEvent(pEvent);
    }
}

//==============================================================================

void PropertyEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Start/stop the editing of the property

    int propertyRow = indexAt(pEvent->pos()).row();

    if (mPropertyEditor) {
        // We are already editing a property, so either stop its editing or
        // start editing anoher property

        if (propertyRow != mPropertyRow)
            // We want to edit another property

            editProperty(propertyRow);
        else
            // We want to stop editing the property

            editProperty(-1);
    } else {
        // We are not currently editing a property, so start editing the current
        // one

        editProperty(propertyRow);
    }

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void PropertyEditorWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Edit the property, but only if we want to edit a new one

    int propertyRow = indexAt(pEvent->pos()).row();

    if ((propertyRow != -1) && (propertyRow != mPropertyRow))
        editProperty(propertyRow);

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void PropertyEditorWidget::editorOpened(QWidget *pEditor)
{
    // We are starting the editing of a property, so use its editor as our focus
    // proxy and make sure that it immediately gets the focus
    // Note: if we were not to immediately give the editor the focus, then the
    //       central widget would give the focus to the previously focused
    //       widget (see CentralWidget::updateGui()), so...

    setFocusProxy(pEditor);

    pEditor->setFocus();

    // Keep track of some information about the property

    mPropertyEditor = pEditor;
    mPropertyRow    = currentIndex().row();
}

//==============================================================================

void PropertyEditorWidget::editorClosed()
{
    // We have stopped editing a property, so reset our focus proxy and make
    // sure that we get the focus (see editorOpened() above for the reason)

    setFocusProxy(0);

    setFocus();

    // Reset some information about the property

    mPropertyEditor = 0;
}

//==============================================================================

void PropertyEditorWidget::editProperty(const int &pPropertyRow)
{
    // We want to edit a new property, so first stop the editing of the current
    // one, if any

    if (mPropertyEditor) {
        // A property is currently being edited, so commit its data and then
        // close its corresponding editor

        commitData(mPropertyEditor);
        closeEditor(mPropertyEditor, QAbstractItemDelegate::NoHint);

        // Update our state
        // Note: this is very important otherwise our state will remain that of
        //       editing...

        setState(NoState);

        // Call editorClosed() to reset a few things

        editorClosed();
    }

    // Now that the editing of our old property has finished, we can start the
    // editing of our new property, if any

    if (pPropertyRow != -1) {
        // There is a new property to edit, so first select it

        selectItem(pPropertyRow);

        // Now, we can 'properly' edit the property's value
        // Note: the property's value's column is always 1...

        edit(model()->index(pPropertyRow, 1));
    }
}

//==============================================================================

void PropertyEditorWidget::goToNeighbouringProperty(const int &pShift)
{
    // Go to the previous/next property

    int row = currentIndex().row()+pShift;

    if ((row >= 0) && (row < model()->rowCount()))
        // The previous/next property exists, so we can edit it

        editProperty(row);
}

//==============================================================================

void PropertyEditorWidget::goToPreviousProperty()
{
    // Go to the previous property

    goToNeighbouringProperty(-1);
}

//==============================================================================

void PropertyEditorWidget::goToNextProperty()
{
    // Go to the next property

    goToNeighbouringProperty(1);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
