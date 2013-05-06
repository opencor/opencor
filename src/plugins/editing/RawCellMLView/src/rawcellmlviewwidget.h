//==============================================================================
// Raw CellML view widget
//==============================================================================

#ifndef RAWCELLMLVIEWWIDGET_H
#define RAWCELLMLVIEWWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class RawCellMLViewWidget;
}

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

class RawCellMLViewWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit RawCellMLViewWidget(QWidget *pParent = 0);
    ~RawCellMLViewWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);
    bool isManaged(const QString &pFileName) const;
    void finalize(const QString &pFileName);

private:
    Ui::RawCellMLViewWidget *mGui;

    Core::BorderedWidget *mBorderedViewer;
    Viewer::ViewerWidget *mViewer;

    Core::BorderedWidget *mBorderedEditor;
    QMap<QString, Core::BorderedWidget *> mBorderedEditors;

    int mBorderedViewerHeight;
    int mBorderedEditorHeight;

private Q_SLOTS:
    void splitterMoved();
};

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
