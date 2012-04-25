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

namespace Core {
    class BorderedWidget;
}   // namespace Core

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
    ~RawCellmlViewWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);

private:
    Ui::RawCellmlViewWidget *mUi;

    QSplitter *mVerticalSplitter;
    QMap<QString, Core::BorderedWidget *> mEditors;

    Core::BorderedWidget *mViewer;
    Core::BorderedWidget *mEditor;

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
