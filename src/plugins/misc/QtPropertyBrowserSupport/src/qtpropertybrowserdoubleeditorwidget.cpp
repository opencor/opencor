//==============================================================================
// QtPropertyBrowser double editor widget class
//==============================================================================

#include "qtpropertybrowserdoubleeditorwidget.h"

//==============================================================================

#include <QKeyEvent>
#include <QRegExpValidator>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

DoubleEditorWidget::DoubleEditorWidget(QWidget *pParent) :
    QLineEdit(pParent),
    mUnit(QString())
{
    // Customise ourselves

#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our editor
#endif
    setFrame(false);

    // Set a validator which accepts any double

    setValidator(new QRegExpValidator(QRegExp("^[+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?$"), this));
}

//==============================================================================

void DoubleEditorWidget::keyPressEvent(QKeyEvent *pEvent)
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

void DoubleEditorWidget::setUnit(const QString &pUnit)
{
    // Update our unit, if needed

    if (!pUnit.compare(mUnit))
        return;

    mUnit = pUnit;

    // Let people know that the unit has changed

    emit unitChanged(pUnit);
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
