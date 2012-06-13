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
    class RawCellmlViewWidget;
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

class RawCellmlViewWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit RawCellmlViewWidget(QWidget *pParent);
    ~RawCellmlViewWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);
    bool isManaged(const QString &pFileName) const;
    void finalize(const QString &pFileName);

private:
    Ui::RawCellmlViewWidget *mGui;

    QMap<QString, Core::BorderedWidget *> mBorderedEditors;

    Viewer::ViewerWidget *mViewer;
    Core::BorderedWidget *mBorderedViewer;

    Core::BorderedWidget *mBorderedEditor;

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
