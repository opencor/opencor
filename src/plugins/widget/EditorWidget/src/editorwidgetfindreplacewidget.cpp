/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Editor find/replace widget
//==============================================================================

#include "coreguiutils.h"
#include "editorwidgetfindreplacewidget.h"
#include "i18ninterface.h"

//==============================================================================

#include "ui_editorwidgetfindreplacewidget.h"

//==============================================================================

#include <QAction>
#include <QKeyEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <QToolButton>

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

EditorWidgetFindReplaceWidget::EditorWidgetFindReplaceWidget(QWidget *pParent) :
    Core::Widget(pParent),
    mGui(new Ui::EditorWidgetFindReplaceWidget),
    mActive(false)
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->findEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    mGui->replaceEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    // Create and handle our drop-down menu action

    mDropDownAction = Core::newAction(this);

    mCaseSensitiveAction = Core::newAction(true, this);
    mWholeWordsOnlyAction = Core::newAction(true, this);
    mRegularExpressionAction = Core::newAction(true, this);

    QMenu *dropDownMenu = new QMenu(this);

    dropDownMenu->addAction(mCaseSensitiveAction);
    dropDownMenu->addAction(mWholeWordsOnlyAction);
    dropDownMenu->addAction(mRegularExpressionAction);

    mDropDownAction->setMenu(dropDownMenu);

    mGui->findEdit->addAction(mDropDownAction, QLineEdit::LeadingPosition);

    connect(mCaseSensitiveAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));
    connect(mWholeWordsOnlyAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));
    connect(mRegularExpressionAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));

    // Create and handle our clear find and replace text actions

    mClearFindTextAction = Core::newAction(QIcon(":/EditorWidget/qtCreator/src/plugins/coreplugin/images/editclear.png"), this);
    mClearReplaceTextAction = Core::newAction(QIcon(":/EditorWidget/qtCreator/src/plugins/coreplugin/images/editclear.png"), this);

    connect(mClearFindTextAction, SIGNAL(triggered(bool)),
            mGui->findEdit, SLOT(clear()));

    connect(mClearReplaceTextAction, SIGNAL(triggered(bool)),
            mGui->replaceEdit, SLOT(clear()));

    // Make our find edit widget our focus proxy

    setFocusProxy(mGui->findEdit);

    // Some connections for our find-related widgets

    connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateClearFindTextAction(const QString &)));

    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->findPreviousButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->findNextButton, SLOT(setEnabled(bool)));

    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->replaceButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->replaceAndFindButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->replaceAllButton, SLOT(setEnabled(bool)));

    // A connection for our replace widget

    connect(mGui->replaceEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateClearReplaceTextAction(const QString &)));

    // A few more things , so that we are properly initialised

    retranslateUi();

    searchOptionChanged();

    setActive(true);

    updateStyleSheet();
    updateHeight();
    // Note: just to be safe, we update our height after updating our style
    //       sheet since we change the size of our tool buttons...

    mGui->findPreviousButton->setEnabled(false);
    mGui->findNextButton->setEnabled(false);

    mGui->replaceButton->setEnabled(false);
    mGui->replaceAndFindButton->setEnabled(false);
    mGui->replaceAllButton->setEnabled(false);
}

//==============================================================================

EditorWidgetFindReplaceWidget::~EditorWidgetFindReplaceWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void EditorWidgetFindReplaceWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate our actions

    I18nInterface::retranslateAction(mCaseSensitiveAction, tr("Case Sensitive"),
                                     tr("The search is case sensitive"));
    I18nInterface::retranslateAction(mWholeWordsOnlyAction, tr("Whole Words Only"),
                                     tr("The search is done on whole words only"));
    I18nInterface::retranslateAction(mRegularExpressionAction, tr("Regular Expression"),
                                     tr("The search uses a regular expression"));

    I18nInterface::retranslateAction(mClearFindTextAction, tr("Clear Text"),
                                     tr("Clear the text"));
    I18nInterface::retranslateAction(mClearReplaceTextAction, tr("Clear Text"),
                                     tr("Clear the text"));
}

//==============================================================================

bool EditorWidgetFindReplaceWidget::isCaseSensitive() const
{
    // Return whether the search is to be case sensitive

    return mCaseSensitiveAction->isChecked();
}

//==============================================================================

bool EditorWidgetFindReplaceWidget::searchWholeWordsOnly() const
{
    // Return whether we search whole words only

    return mWholeWordsOnlyAction->isChecked();
}

//==============================================================================

bool EditorWidgetFindReplaceWidget::useRegularExpression() const
{
    // Return whether we use a regular expression

    return mRegularExpressionAction->isChecked();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::setReadOnly(const bool &pReadOnly)
{
    // Show/hide our replace-related widgets based on whether we are in
    // read-only mode

    Core::showEnableWidget(mGui->replaceLabel, !pReadOnly);
    Core::showEnableWidget(mGui->replaceEdit, !pReadOnly);
    Core::showEnableWidget(mGui->replaceButton, !pReadOnly);
    Core::showEnableWidget(mGui->replaceAndFindButton, !pReadOnly);
    Core::showEnableWidget(mGui->replaceAllButton, !pReadOnly);

    // Enable/disable our find spacer

    mGui->findLayout->setStretch(2, !pReadOnly);

    // Disable our replace-related buttons, if needed

    if (findText().isEmpty()) {
        mGui->replaceButton->setEnabled(false);
        mGui->replaceAndFindButton->setEnabled(false);
        mGui->replaceAllButton->setEnabled(false);
    }

    // Update our height

    updateHeight();
}

//==============================================================================

bool EditorWidgetFindReplaceWidget::isFindPreviousNextAvailable() const
{
    // Return whether the find previous/next actions are available

    return !findText().isEmpty();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::selectFindText() const
{
    // Select our find text

    mGui->findEdit->selectAll();
}

//==============================================================================

QString EditorWidgetFindReplaceWidget::findText() const
{
    // Return our find text

    return mGui->findEdit->text();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::setFindText(const QString &pFindText)
{
    // Set our find text and select it

    mGui->findEdit->setText(pFindText);

    selectFindText();
}

//==============================================================================

QString EditorWidgetFindReplaceWidget::replaceText() const
{
    // Return our replace text

    return mGui->replaceEdit->text();
}

//==============================================================================

bool EditorWidgetFindReplaceWidget::findEditHasFocus() const
{
    // Return whether our find edit has the focus

    return mGui->findEdit->hasFocus();
}

//==============================================================================

bool EditorWidgetFindReplaceWidget::replaceEditHasFocus() const
{
    // Return whether our replace edit has the focus

    return mGui->replaceEdit->hasFocus();
}

//==============================================================================

bool EditorWidgetFindReplaceWidget::isActive() const
{
    // Return whether we are active

    return mActive;
}

//==============================================================================

void EditorWidgetFindReplaceWidget::setActive(const bool &pActive)
{
    if (pActive == mActive)
        return;

    // Set our active state

    mActive = pActive;

    if (pActive) {
        connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
                this, SIGNAL(findTextChanged(const QString &)));
    } else {
        disconnect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
                   this, SIGNAL(findTextChanged(const QString &)));
    }
}

//==============================================================================

void EditorWidgetFindReplaceWidget::updateFrom(EditorWidgetFindReplaceWidget *pFindReplace)
{
    // Update our find and replace texts from the given find/replace widget

    mGui->findEdit->setText(pFindReplace->findText());
    mGui->replaceEdit->setText(pFindReplace->replaceText());
}

//==============================================================================

void EditorWidgetFindReplaceWidget::updateHeight()
{
    // Update our layout
    // Note: we shouldn't have to do this, but if the user opens a read-only and
    //       shows ourselves, then the find-related widgets will be too 'low'.
    //       This is because the replace-related widgets get hidden/disabled as
    //       expected, but the layout doesn't get updated before we fix our
    //       height, so we update our layout here and in all cases...

    mGui->layout->update();

    // Update our height

    setFixedHeight(mGui->layout->sizeHint().height());
}

//==============================================================================

void EditorWidgetFindReplaceWidget::updateStyleSheet()
{
    // Change the style of our tool buttons

    QColor shadowColor = Core::shadowColor();

    setStyleSheet(QString("QToolButton {"
                          "    border: none;"
                          "    border-radius: 3px;"
                          "    padding: 1px;"
                          "}"
                          ""
                          "QToolButton:focus {"
                          "    background: rgba(%1, %2, %3, 0.13);"
                          "    border: 1px solid rgba(%1, %2, %3, 0.39);"
                          "}"
                          ""
                          "QToolButton:hover {"
                          "    background: rgba(%1, %2, %3, 0.39);"
                          "}"
                          ""
                          "QToolButton:pressed {"
                          "    background: rgba(%1, %2, %3, 0.79);"
                          "}").arg(QString::number(shadowColor.red()),
                                   QString::number(shadowColor.green()),
                                   QString::number(shadowColor.blue())));
}

//==============================================================================

void EditorWidgetFindReplaceWidget::changeEvent(QEvent *pEvent)
{
    // Check whether the palette has changed and if so then update our style
    // sheet

    if (pEvent->type() == QEvent::PaletteChange)
        updateStyleSheet();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Let people know that a key has been pressed

    bool handled = false;

    emit keyPressed(pEvent, handled);

    // Carry on as normal, if the event wasn't handled

    if (handled) {
        // Accept the event

        pEvent->accept();
    } else {
        // Default handling of the event

        Core::Widget::keyPressEvent(pEvent);
    }
}

//==============================================================================

void EditorWidgetFindReplaceWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Core::Widget::resizeEvent(pEvent);

    // Update our height

    updateHeight();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::on_findPreviousButton_clicked()
{
    // Let people know that we want to find the previous occurrence of the text

    emit findPreviousRequested();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::on_findNextButton_clicked()
{
    // Let people know that we want to find the next occurrence of the text

    emit findNextRequested();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::on_replaceButton_clicked()
{
    // Let people know that we want to replace the current text

    emit replaceRequested();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::on_replaceAndFindButton_clicked()
{
    // Let people know that we want to replace the current text and the find the
    // next occurence of the text

    emit replaceAndFindRequested();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::on_replaceAllButton_clicked()
{
    // Let people know that we want to replace all the occurences of the text

    emit replaceAllRequested();
}

//==============================================================================

void EditorWidgetFindReplaceWidget::searchOptionChanged()
{
    // Update the icon used for the leading position of our find widget

    static const QIcon MagnifierIcon         = QIcon(":/EditorWidget/qtCreator/src/plugins/coreplugin/images/magnifier.png");
    static const QIcon CaseSensitiveIcon     = QIcon(":/EditorWidget/qtCreator/src/plugins/coreplugin/find/images/casesensitively.png");
    static const QIcon WholeWordsOnlyIcon    = QIcon(":/EditorWidget/qtCreator/src/plugins/coreplugin/find/images/wholewords.png");
    static const QIcon RegularExpressionIcon = QIcon(":/EditorWidget/qtCreator/src/plugins/coreplugin/find/images/regexp.png");

    enum {
        IconSize                = 16,
        MagnifierIconWidth      = 17,
        MagnifierIconHeight     = 11,
        CaseSensitiveIconShift  =  6,
        WholeWordsOnlyIconShift =  6,
        RegularExpressionShift  =  7,
        CaseSensitiveIconWidth  =  5,
        WholeWordsOnlyIconWidth =  5,
        RegularExpressionWidth  =  4
    };

    int nbOfOptions =  mCaseSensitiveAction->isChecked()
                      +mWholeWordsOnlyAction->isChecked()
                      +mRegularExpressionAction->isChecked();
    QPixmap dropDownPixmap = QPixmap(IconSize, IconSize);

    dropDownPixmap.fill(Qt::transparent);

    QPainter dropDownPixmapPainter(&dropDownPixmap);

    if (nbOfOptions) {
        int left = ( IconSize-nbOfOptions+1
                    -mCaseSensitiveAction->isChecked()*CaseSensitiveIconWidth
                    -mWholeWordsOnlyAction->isChecked()*WholeWordsOnlyIconWidth
                    -mRegularExpressionAction->isChecked()*RegularExpressionWidth) >> 1;

        if (mCaseSensitiveAction->isChecked()) {
            CaseSensitiveIcon.paint(&dropDownPixmapPainter,
                                    left-CaseSensitiveIconShift, 0,
                                    IconSize, IconSize);

            left += CaseSensitiveIconWidth+1;
        }

        if (mWholeWordsOnlyAction->isChecked()) {
            WholeWordsOnlyIcon.paint(&dropDownPixmapPainter,
                                     left-WholeWordsOnlyIconShift, 0,
                                     IconSize, IconSize);

            left += WholeWordsOnlyIconWidth+1;
        }

        if (mRegularExpressionAction->isChecked()) {
            RegularExpressionIcon.paint(&dropDownPixmapPainter,
                                        left-RegularExpressionShift, 0,
                                        IconSize, IconSize);
        }
    } else {
        // We hack of magnifier icon away so that it ends up looking the way we
        // want it (since it's wider than 16 pixels)

        MagnifierIcon.paint(&dropDownPixmapPainter,
                            -1, ((IconSize-MagnifierIconHeight) >> 1)+1,
                            MagnifierIconWidth, MagnifierIconHeight);

        QPen pen = dropDownPixmapPainter.pen();

        pen.setColor(Qt::white);

        dropDownPixmapPainter.setPen(pen);

        dropDownPixmapPainter.drawPoint(0, 13);
    }

    mDropDownAction->setIcon(dropDownPixmap);
}

//==============================================================================

void EditorWidgetFindReplaceWidget::updateClearFindTextAction(const QString &pText)
{
    // Show/hide our clear text action, based on whether our find widget
    // contains some text

    if (pText.isEmpty())
        mGui->findEdit->removeAction(mClearFindTextAction);
    else
        mGui->findEdit->addAction(mClearFindTextAction, QLineEdit::TrailingPosition);

    // Let people know whether we can find/replace

    emit canFindReplace(!findText().isEmpty());
}

//==============================================================================

void EditorWidgetFindReplaceWidget::updateClearReplaceTextAction(const QString &pText)
{
    // Show/hide our clear text action, based on whether our replace widget
    // contains some text

    if (pText.isEmpty())
        mGui->replaceEdit->removeAction(mClearReplaceTextAction);
    else
        mGui->replaceEdit->addAction(mClearReplaceTextAction, QLineEdit::TrailingPosition);
}

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
