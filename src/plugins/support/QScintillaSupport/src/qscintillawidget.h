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
// QScintillaWidget class
//==============================================================================

#ifndef QSCINTILLAWIDGET_H
#define QSCINTILLAWIDGET_H

//==============================================================================

#include "qscintillasupportglobal.h"

//==============================================================================

#include <QFont>

//==============================================================================

#include "Qsci/qsciscintilla.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

class QSCINTILLASUPPORT_EXPORT QScintillaWidget : public QsciScintilla
{
    Q_OBJECT

public:
    explicit QScintillaWidget(const QString &pContents, const bool &pReadOnly,
                              QsciLexer *pLexer, QWidget *pParent = 0);
    explicit QScintillaWidget(const QString &pContents, const bool &pReadOnly,
                              QWidget *pParent = 0);
    explicit QScintillaWidget(QsciLexer *pLexer, QWidget *pParent = 0);
    explicit QScintillaWidget(QWidget *pParent = 0);
    ~QScintillaWidget();

    QMenu * contextMenu() const;
    void setContextMenu(const QList<QAction *> &pContextMenuActions);

    void setContents(const QString &pContents);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *pEvent);
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    QFont mFont;

    QMenu *mContextMenu;

    bool mCanUndo;
    bool mCanRedo;

    void constructor(const QString &pContents = QString(),
                     const bool &pReadOnly = false,
                     QsciLexer *pLexer = 0);

Q_SIGNALS:
    void canUndo(const bool &pCanUndo);
    void canRedo(const bool &pCanRedo);
};

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
