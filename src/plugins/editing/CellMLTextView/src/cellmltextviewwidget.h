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
// CellML Text view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "cellmltextviewconverter.h"
#include "cellmltextviewparser.h"
#include "corecliutils.h"
#include "mathmlconverter.h"
#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLEditingView {
    class CellmlEditingViewWidget;
}   // namespace CellMLEditingView

//==============================================================================

namespace EditorWidget {
    class EditorListWidget;
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace QScintillaSupport {
    class QScintillaWidget;
}   // namespace QScintillaSupport

//==============================================================================

namespace CellMLTextView {

//==============================================================================

class CellmlTextViewWidgetData
{
public:
    explicit CellmlTextViewWidgetData(CellMLEditingView::CellmlEditingViewWidget *pEditingWidget,
                                      const QString &pSha1, const bool &pValid,
                                      const CellMLSupport::CellmlFile::Version &pCellmlVersion,
                                      QDomDocument pRdfNodes);
    ~CellmlTextViewWidgetData();

    void retranslateUi();

    CellMLEditingView::CellmlEditingViewWidget * editingWidget() const;

    QString sha1() const;
    void setSha1(const QString &pSha1);

    bool isValid() const;

    CellMLSupport::CellmlFile::Version cellmlVersion() const;
    void setCellmlVersion(const CellMLSupport::CellmlFile::Version &pCellmlVersion);

    QDomDocument rdfNodes() const;

    QString fileContents() const;
    void setFileContents(const QString &pFileContents);

    QString convertedFileContents() const;
    void setConvertedFileContents(const QString &pConvertedFileContents);

private:
    CellMLEditingView::CellmlEditingViewWidget *mEditingWidget;
    QString mSha1;
    bool mValid;
    CellMLSupport::CellmlFile::Version mCellmlVersion;
    QDomDocument mRdfNodes;
    QString mFileContents;
    QString mConvertedFileContents;
};

//==============================================================================

class CellmlTextViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit CellmlTextViewWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName, const bool &pUpdate = true);
    void finalize(const QString &pFileName);

    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    EditorWidget::EditorWidget * editorWidget(const QString &pFileName) const;

    virtual QWidget * widget(const QString &pFileName);

    bool isEditorWidgetUseable(const QString &pFileName) const;
    bool isEditorWidgetContentsModified(const QString &pFileName) const;

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName,
                  bool &pNeedFeedback);

    virtual QList<QWidget *> statusBarWidgets() const;

    void reformat(const QString &pFileName);

    bool validate(const QString &pFileName);

private:
    bool mNeedLoadingSettings;
    QString mSettingsGroup;

    CellMLEditingView::CellmlEditingViewWidget *mEditingWidget;

    QMap<QString, CellmlTextViewWidgetData *> mData;

    CellMLTextViewConverter mConverter;
    CellmlTextViewParser mParser;

    QList<EditorWidget::EditorListWidget *> mEditorLists;

    QMap<QString, QString> mPresentationMathmlEquations;

    Core::MathmlConverter mMathmlConverter;

    QString mContentMathmlEquation;

    bool commentOrUncommentLine(QScintillaSupport::QScintillaWidget *pEditorWidget,
                                const int &pLineNumber,
                                const bool &pCommentLine);

    bool parse(const QString &pFileName, const bool &pOnlyErrors = false);

    QString partialStatement(const int &pPosition, int &pFromPosition,
                             int &pToPosition) const;
    QString beginningOfPiecewiseStatement(int &pPosition) const;
    QString endOfPiecewiseStatement(int &pPosition) const;
    QString statement(const int &pPosition) const;

private slots:
    void editorKeyPressed(QKeyEvent *pEvent, bool &pHandled);

    void updateViewer();

    void selectFirstItemInEditorList(EditorWidget::EditorListWidget *pEditorList = 0);

    void mathmlConversionDone(const QString &pContentMathml,
                              const QString &pPresentationMathml);
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
