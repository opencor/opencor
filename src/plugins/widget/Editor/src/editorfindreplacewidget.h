/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Editor find/replace widget
//==============================================================================

#ifndef EDITORFINDREPLACEWIDGET_H
#define EDITORFINDREPLACEWIDGET_H

//==============================================================================

#include "editorglobal.h"
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

class EDITOR_EXPORT EditorFindReplaceWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit EditorFindReplaceWidget(QWidget *pParent);

    virtual void retranslateUi();

    bool caseSensitive() const;
    bool wholeWordsOnly() const;
    bool regularExpression() const;

    void setReadOnly(const bool &pReadOnly);

    void updateFrom(EditorFindReplaceWidget *pFindReplace);

    QString findText() const;
    QString replaceText() const;

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

    void updateHeight();
    void updateStyleSheet();

Q_SIGNALS:
    void keyPressed(QKeyEvent *pEvent, bool &pHandled);

    void findTextChanged(const QString &pText);

    void findPreviousRequested();
    void findNextRequested();

private Q_SLOTS:
    void on_findPreviousButton_clicked();
    void on_findNextButton_clicked();

    void searchOptionChanged();

    void updateClearFindTextAction(const QString &pText);
    void updateClearReplaceTextAction(const QString &pText);
};

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
