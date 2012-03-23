//==============================================================================
// Raw CellML view widget
//==============================================================================

#include "qscintilla.h"
#include "rawcellmlviewwidget.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_rawcellmlviewwidget.h"

//==============================================================================

#include <QFileInfo>
#include <QTextStream>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

RawCellmlViewWidget::RawCellmlViewWidget(const QString &pFileName,
                                         QWidget *pParent) :
    Widget(pParent),
    mUi(new Ui::RawCellmlViewWidget)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a viewer

    Viewer::ViewerWidget *viewer = new Viewer::ViewerWidget(pParent);

    // Create and set up a Scintilla editor with an XML lexer associated to it

    QFile file(pFileName);
    QString fileContents = QString();
    bool fileIsWritable = false;

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        // We could open the file, so retrieve its contents and whether it can
        // be written to

        fileContents = QTextStream(&file).readAll();
        fileIsWritable = !(QFileInfo(pFileName).isWritable());

        // We are done with the file, so close it

        file.close();
    }

    QsciScintilla *editor = new QScintillaSupport::QScintilla(fileContents,
                                                              fileIsWritable,
                                                              new QsciLexerXML(pParent),
                                                              pParent);

    // Add the viewer and editor to the raw CellML view widget

    mUi->verticalLayout->addWidget(viewer);
    mUi->verticalLayout->addWidget(editor);
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
