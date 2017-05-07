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
// MathML Viewer widget
//==============================================================================

#pragma once

//==============================================================================

#include "mathmlviewerwidgetglobal.h"
#include "widget.h"

//==============================================================================

#include <QDomNode>

//==============================================================================

#include "qwt_mml_document.h"

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace MathMLViewerWidget {

//==============================================================================

class MATHMLVIEWERWIDGET_EXPORT MathmlViewerWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit MathmlViewerWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    void updateSettings(MathmlViewerWidget *pMathmlViewerWidget);

    QString contents() const;
    void setContents(const QString &pContents);

    bool error() const;
    void setError(const bool &pError);

    bool optimiseFontSize() const;
    void setOptimiseFontSize(const bool &pOptimiseFontSize);

    bool subscripts() const;
    void setSubscripts(const bool &pSubscripts);

    bool greekSymbols() const;
    void setGreekSymbols(const bool &pGreekSymbols);

    bool digitGrouping() const;
    void setDigitGrouping(const bool &pDigitGrouping);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *pEvent);
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    QwtMathMLDocument mMathmlDocument;

    double mOneOverMathmlDocumentWidth;
    double mOneOverMathmlDocumentHeight;

    QString mContents;
    bool mError;

    QMenu *mContextMenu;

    QAction *mOptimiseFontSizeAction;
    QAction *mSubscriptsAction;
    QAction *mGreekSymbolsAction;
    QAction *mDigitGroupingAction;
    QAction *mCopyToClipboardAction;

    QAction * newAction();

    QString greekSymbolize(const QString &pValue) const;

    QDomElement newMiNode(const QDomNode &pDomNode,
                          const QString &pValue) const;

    void processNode(const QDomNode &pDomNode) const;
    QString processedContents() const;

private slots:
    void updateMathmlViewerWidget();

    void copyToClipboard();
};

//==============================================================================

}   // namespace MathMLViewerWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
