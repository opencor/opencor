//==============================================================================
// Property editor widget
//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

#include <float.h>

//==============================================================================

#include <QHeaderView>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSettings>
#include <QStandardItem>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

LineEditWidget::LineEditWidget(QWidget *pParent) :
    QLineEdit(pParent)
{
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our editor
#endif
}

//==============================================================================

void LineEditWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // None of the modifiers is selected

        if (pEvent->key() == Qt::Key_Up) {
            // The user wants to go to the previous property

            emit goToPreviousPropertyRequested();

            // Accept the event

            pEvent->accept();
        } else if (pEvent->key() == Qt::Key_Down) {
            // The user wants to go to the next property

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

DoubleEditWidget::DoubleEditWidget(QWidget *pParent) :
    LineEditWidget(pParent)
{
//---GRY--- USE A SPECIFIC MASK FOR DOUBLES...
}

//==============================================================================

QWidget * PropertyItemDelegate::createEditor(QWidget *pParent,
                                             const QStyleOptionViewItem &pOption,
                                             const QModelIndex &pIndex) const
{
    Q_UNUSED(pOption);
    Q_UNUSED(pIndex);

    // Create and return an editor for our item, based on its type

    QWidget *editor;

    switch (static_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex)->type()) {
    case PropertyItem::Double:
        editor = new DoubleEditWidget(pParent);

        break;
    default:
        // PropertyItem::String

        editor = new LineEditWidget(pParent);
    }

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

bool PropertyItemDelegate::eventFilter(QObject *pObject, QEvent *pEvent)
{
    // We want to handle key events ourselves, so...

    if(pEvent->type()==QEvent::KeyPress)
        return false;
    else
        return QStyledItemDelegate::eventFilter(pObject, pEvent);
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

    // Further customise ourselves

    setSelectionMode(QAbstractItemView::SingleSelection);

    header()->setMovable(false);
}

//==============================================================================

static const QString SettingsColumnWidth = "ColumnWidth";

//==============================================================================

void PropertyEditorWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column

    QString columnWidthKey;

    for (int i = 0, iMax = header()->count(); i < iMax; ++i) {
        columnWidthKey = SettingsColumnWidth+QString::number(i);

        setColumnWidth(i, pSettings->value(columnWidthKey,
                                           columnWidth(i)).toInt());

        // On Windows, no matter what, scroll bars will always be shown after
        // setting the columns' width, so... we decrease the last column's width
        // by the width of the vertical scroll bar. Not nice, I know, but...
        // Note: that hack doesn't seem required on either Linux or OS X, but we
        //       never know, so we also do it on those platforms...

        if (i == iMax-1)
            setColumnWidth(i, columnWidth(i)-verticalScrollBar()->width());
    }
}

//==============================================================================

void PropertyEditorWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column

    for (int i = 0, iMax = header()->count(); i < iMax; ++i)
        pSettings->setValue(SettingsColumnWidth+QString::number(i),
                            columnWidth(i));
}

//==============================================================================

void PropertyEditorWidget::setModel(QAbstractItemModel *pModel)
{
    // Stop tracking data changes in the old model

    if (model())
        disconnect(model(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                   this, SLOT(updateHeight()));

    // Update our model

    TreeViewWidget::setModel(pModel);

    // Update our height following data changes

    connect(model(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateHeight()));
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
    } else if (   (pEvent->key() == Qt::Key_Return)
               || (pEvent->key() == Qt::Key_Enter)) {
        // The user wants to start/stop editing the property

        if (mPropertyEditor)
            // We are currently editing the property, so stop editing it

            editProperty(-1);
        else
            // We are not currently editing the property, so start editing it
            // it
            // Note: we could use mPropertyRow, but if it was to be equal to -1
            //       we would have to use currentIndex().row(), so we might as
            //       well use the latter all the time...

            editProperty(currentIndex().row());

        // Accept the event

        pEvent->accept();
    } else if (pEvent->key() == Qt::Key_Escape) {
        // The user wants to cancel the editing of the property

        cancelPropertyEditing();

        // Accept the event

        pEvent->accept();
    } else if (   !(pEvent->modifiers() & Qt::ShiftModifier)
               && !(pEvent->modifiers() & Qt::ControlModifier)
               && !(pEvent->modifiers() & Qt::AltModifier)
               && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // None of the modifiers is selected

        if (pEvent->key() == Qt::Key_Up) {
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

void PropertyEditorWidget::updateHeight()
{
    // Update our height based on the number of rows

    int newHeight = header()->height();
    QAbstractItemModel *itemModel = model();

    for (int i = 0, iMax = itemModel->rowCount(); i < iMax; ++i)
        newHeight += rowHeight(itemModel->index(i, 0));

    setFixedHeight(newHeight);
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

void PropertyEditorWidget::editProperty(const int &pPropertyRow,
                                        const bool &pCommitData)
{
    // We want to edit a new property, so first stop the editing of the current
    // one, if any

    if (mPropertyEditor) {
        // A property is currently being edited, so commit its data and then
        // close its corresponding editor

        if (pCommitData)
            commitData(mPropertyEditor);

        closeEditor(mPropertyEditor,
                    pCommitData?
                        QAbstractItemDelegate::SubmitModelCache:
                        QAbstractItemDelegate::RevertModelCache);

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

void PropertyEditorWidget::cancelPropertyEditing()
{
    // The user wants to cancel the editing of the property

    editProperty(-1, false);
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
