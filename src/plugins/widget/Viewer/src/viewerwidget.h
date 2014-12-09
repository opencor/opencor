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
// Viewer widget
//==============================================================================

#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

//==============================================================================

#include "viewerglobal.h"
#include "widget.h"

//==============================================================================

#include <QDomNode>

//==============================================================================

#include "qwt_mml_document.h"

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

class VIEWER_EXPORT ViewerWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    void updateSettings(ViewerWidget *pViewerWidget);

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
    virtual void paintEvent(QPaintEvent *pEvent);

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

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

private Q_SLOTS:
    void showCustomContextMenu(const QPoint &pPosition) const;

    void updateViewer();

    void copyToClipboard();
};

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
