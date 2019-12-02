/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CellML Text view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmleditingviewwidget.h"
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

namespace EditorWidget {
    class EditorListWidget;
    class EditorWidget;
} // namespace EditorWidget

//==============================================================================

namespace QScintillaWidget {
    class QScintillaWidget;
} // namespace QScintillaWidget

//==============================================================================

namespace CellMLTextView {

//==============================================================================

class CellmlTextViewWidgetEditingWidget;

//==============================================================================

class CellmlTextViewWidgetData
{
public:
    explicit CellmlTextViewWidgetData(CellmlTextViewWidgetEditingWidget *pEditingWidget,
                                      const QString &pSha1, bool pValid,
                                      CellMLSupport::CellmlFile::Version pCellmlVersion,
                                      const QDomNode &pDocumentationNode,
                                      const QDomDocument &pRdfNodes);
    ~CellmlTextViewWidgetData();

    void retranslateUi();

    CellmlTextViewWidgetEditingWidget * editingWidget() const;

    QString sha1() const;
    void setSha1(const QString &pSha1);

    bool isValid() const;

    CellMLSupport::CellmlFile::Version cellmlVersion() const;
    void setCellmlVersion(CellMLSupport::CellmlFile::Version pCellmlVersion);

    QDomNode documentationNode() const;
    QDomDocument rdfNodes() const;

    QString fileContents() const;
    void setFileContents(const QString &pFileContents);

    QString convertedFileContents() const;
    void setConvertedFileContents(const QString &pConvertedFileContents);

private:
    CellmlTextViewWidgetEditingWidget *mEditingWidget;
    QString mSha1;
    bool mValid;
    CellMLSupport::CellmlFile::Version mCellmlVersion;
    QDomNode mDocumentationNode;
    QDomDocument mRdfNodes;
    QString mFileContents;
    QString mConvertedFileContents;
};

//==============================================================================

class CellmlTextViewWidgetEditingWidget : public CellMLEditingView::CellmlEditingViewWidget
{
    Q_OBJECT

public:
    explicit CellmlTextViewWidgetEditingWidget(const QString &pContents,
                                               bool pReadOnly,
                                               QsciLexer *pLexer,
                                               QWidget *pParent);

    bool handleEditorKeyPressEvent(QKeyEvent *pEvent) override;

private:
    bool commentOrUncommentLine(EditorWidget::EditorWidget *pEditor,
                                int pLineNumber, bool pCommentLine);
};

//==============================================================================

class CellmlTextViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit CellmlTextViewWidget(QWidget *pParent);

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    void retranslateUi() override;

    void initialize(const QString &pFileName, bool pUpdate = true);
    void finalize(const QString &pFileName);

    void fileSaved(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    EditorWidget::EditorWidget * editorWidget(const QString &pFileName) const;

    QWidget * widget(const QString &pFileName) override;

    bool isEditorWidgetUseable(const QString &pFileName) const;
    bool isEditorWidgetContentsModified(const QString &pFileName) const;

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName,
                  bool &pNeedFeedback);

    QList<QWidget *> statusBarWidgets() const override;

    void reformat(const QString &pFileName);

    bool validate(const QString &pFileName, QString &pExtra);

private:
    bool mNeedLoadingSettings = true;
    QString mSettingsGroup;

    CellmlTextViewWidgetEditingWidget *mEditingWidget = nullptr;

    QMap<QString, CellmlTextViewWidgetData *> mData;

    CellMLTextViewConverter mConverter;
    CellmlTextViewParser mParser;

    QList<EditorWidget::EditorListWidget *> mEditorLists;

    QMap<QString, QString> mPresentationMathmlEquations;

    Core::MathmlConverter mMathmlConverter;

    QString mContentMathmlEquation;

    bool parse(const QString &pFileName, QString &pExtra, bool pOnlyErrors);
    bool parse(const QString &pFileName, QString &pExtra);
    bool parse(const QString &pFileName, bool pOnlyErrors = false);

    bool isComment(int pPosition) const;

    QString partialStatement(int pPosition, int &pFromPosition,
                             int &pToPosition) const;
    QString beginningOfPiecewiseStatement(int &pPosition) const;
    QString endOfPiecewiseStatement(int &pPosition) const;

private slots:
    void updateViewer();

    void selectFirstItemInEditorList();

    void mathmlConversionDone(const QString &pContentMathml,
                              const QString &pPresentationMathml);
};

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
