//==============================================================================
// QtPropertyBrowser double editor factory class
//==============================================================================

#include "qtpropertybrowserdoubleeditorfactory.h"
#include "qtpropertybrowserdoubleeditorwidget.h"

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

DoubleEditorFactory::DoubleEditorFactory(QObject *pParent) :
    QtAbstractEditorFactory(pParent),
    mDoubleEditors(QMap<QtProperty *, QList<DoubleEditorWidget *> >()),
    mProperties(QMap<DoubleEditorWidget *, QtProperty *>())
{
}

//==============================================================================

void DoubleEditorFactory::connectPropertyManager(DoublePropertyManager *pManager)
{
    // Keep track of changes to a property's value and unit

    connect(pManager, SIGNAL(valueChanged(QtProperty *, const double &)),
            this, SLOT(valueChanged(QtProperty *, const double &)));
    connect(pManager, SIGNAL(unitChanged(QtProperty *, const QString &)),
            this, SLOT(unitChanged(QtProperty *, const QString &)));
}

//==============================================================================

void DoubleEditorFactory::disconnectPropertyManager(DoublePropertyManager *pManager)
{
    // Stop tracking changes to a property's value and unit

    disconnect(pManager, SIGNAL(valueChanged(QtProperty *, const double &)),
               this, SLOT(valueChanged(QtProperty *, const double &)));
    disconnect(pManager, SIGNAL(unitChanged(QtProperty *, const QString &)),
               this, SLOT(unitChanged(QtProperty *, const QString &)));
}

//==============================================================================

QWidget * DoubleEditorFactory::createEditor(DoublePropertyManager *pManager,
                                            QtProperty *pProperty,
                                            QWidget *pParent)
{
    // Create an editor

    DoubleEditorWidget *res = new DoubleEditorWidget(pParent);

    // Set its unit

    res->setUnit(pManager->unit(pProperty));

    // Keep track of things

    mDoubleEditors[pProperty].append(res);
    mProperties[res] = pProperty;

    // Propagate a few signals

    connect(res, SIGNAL(goToPreviousPropertyRequested()),
            this, SIGNAL(goToPreviousPropertyRequested()));
    connect(res, SIGNAL(goToNextPropertyRequested()),
            this, SIGNAL(goToNextPropertyRequested()));

    // Keep track of when the editor has been destroyed

    connect(res, SIGNAL(destroyed(QObject *)),
            this, SLOT(editorDestroyed(QObject *)));

    // Return our editor

    return res;
}

//==============================================================================

void DoubleEditorFactory::editorDestroyed(QObject *pEditor)
{
    // Remove any trace of the given editor

    QMap<DoubleEditorWidget *, QtProperty *>::ConstIterator doubleEditorsIterator = mProperties.constBegin();

    while (doubleEditorsIterator != mProperties.constEnd()) {
        if (doubleEditorsIterator.key() == pEditor) {
            DoubleEditorWidget *doubleEditor = doubleEditorsIterator.key();
            QtProperty *property = doubleEditorsIterator.value();

            mProperties.remove(doubleEditor);

            mDoubleEditors[property].removeAll(doubleEditor);

            if (mDoubleEditors[property].isEmpty())
                mDoubleEditors.remove(property);

            return;
        }

        ++doubleEditorsIterator;
    }
}

//==============================================================================

void DoubleEditorFactory::valueChanged(QtProperty *pProperty,
                                       const double &pValue)
{
    // Make sure that at least one editor exists for our property

    if (!mDoubleEditors.contains(pProperty))
        return;

    // Make sure that our property has a manager

    if (!propertyManager(pProperty))
        return;

    // Update the value of all our double editors

    QList<DoubleEditorWidget *> doubleEditors = mDoubleEditors[pProperty];
    QListIterator<DoubleEditorWidget *> doubleEditorsIterator(doubleEditors);

    while (doubleEditorsIterator.hasNext()) {
        DoubleEditorWidget *doubleEditor = doubleEditorsIterator.next();

        doubleEditor->setValue(pValue);
    }
}

//==============================================================================

void DoubleEditorFactory::unitChanged(QtProperty *pProperty,
                                      const QString &pUnit)
{
    // Make sure that at least one editor exists for our property

    if (!mDoubleEditors.contains(pProperty))
        return;

    // Make sure that our property has a manager

    if (!propertyManager(pProperty))
        return;

    // Update the unit of all our double editors

    QList<DoubleEditorWidget *> doubleEditors = mDoubleEditors[pProperty];
    QListIterator<DoubleEditorWidget *> doubleEditorsIterator(doubleEditors);

    while (doubleEditorsIterator.hasNext()) {
        DoubleEditorWidget *doubleEditor = doubleEditorsIterator.next();

        doubleEditor->setUnit(pUnit);
    }
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
