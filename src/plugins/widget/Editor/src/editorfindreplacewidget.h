/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Editor find/replace widget
//==============================================================================

#pragma once

//==============================================================================

#include "widget.h"

//==============================================================================

#include <QColor>
#include <QString>

//==============================================================================

class QLabel;
class QTextEdit;

//==============================================================================

namespace Ui {
    class EditorFindReplaceWidget;
}

//==============================================================================

namespace OpenCOR {
namespace Editor {

//==============================================================================

class EditorFindReplaceWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit EditorFindReplaceWidget(QWidget *pParent);
    ~EditorFindReplaceWidget();

    virtual void retranslateUi();

    bool isCaseSensitive() const;
    bool searchWholeWordsOnly() const;
    bool useRegularExpression() const;

    void setReadOnly(const bool &pReadOnly);

    void updateFrom(EditorFindReplaceWidget *pFindReplace);

    bool isFindPreviousNextAvailable() const;

    void selectFindText() const;

    QString findText() const;
    void setFindText(const QString &pFindText);

    QString replaceText() const;

    bool findEditHasFocus() const;
    bool replaceEditHasFocus() const;

    bool isActive() const;
    void setActive(const bool &pActive);

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::EditorFindReplaceWidget *mGui;

    QAction *mDropDownAction;

    QAction *mCaseSensitiveAction;
    QAction *mWholeWordsOnlyAction;
    QAction *mRegularExpressionAction;

    QAction *mClearFindTextAction;
    QAction *mClearReplaceTextAction;

    bool mActive;

    void updateHeight();
    void updateStyleSheet();

Q_SIGNALS:
    void keyPressed(QKeyEvent *pEvent, bool &pHandled);

    void findTextChanged(const QString &pText);

    void canFindReplace(const bool &pCanFindReplace);

    void findPreviousRequested();
    void findNextRequested();

    void replaceRequested();
    void replaceAndFindRequested();
    void replaceAllRequested();

private Q_SLOTS:
    void on_findPreviousButton_clicked();
    void on_findNextButton_clicked();

    void on_replaceButton_clicked();
    void on_replaceAndFindButton_clicked();
    void on_replaceAllButton_clicked();

    void searchOptionChanged();

    void updateClearFindTextAction(const QString &pText);
    void updateClearReplaceTextAction(const QString &pText);
};

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
