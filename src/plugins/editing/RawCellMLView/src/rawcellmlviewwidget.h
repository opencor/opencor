//==============================================================================
// Raw CellML view widget
//==============================================================================

#ifndef RAWCELLMLVIEWWIDGET_H
#define RAWCELLMLVIEWWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

namespace Ui {
    class RawCellmlViewWidget;
}

//==============================================================================

class QSplitter;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace QScintillaSupport {
    class QScintilla;
}   // namespace QScintillaSupport

//==============================================================================

namespace Viewer {
    class ViewerWidget;
}   // namespace Viewer

//==============================================================================

namespace RawCellMLView {

//==============================================================================

class RawCellmlViewWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit RawCellmlViewWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);

private:
    Ui::RawCellmlViewWidget *mUi;

    QSplitter *mVerticalSplitter;
    QMap<QString, QScintillaSupport::QScintilla *> mEditors;

    Viewer::ViewerWidget *mViewer;
    QScintillaSupport::QScintilla *mEditor;

    int mViewerHeight;
    int mEditorHeight;

private Q_SLOTS:
    void verticalSplitterMoved();
};

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
