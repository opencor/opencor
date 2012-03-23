//==============================================================================
// Raw CellML view widget
//==============================================================================

#include "qscintilla.h"
#include "rawcellmlviewwidget.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_rawcellmlviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QSplitter>
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

    // Create our vertical splitter

    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);

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

    // Populate our vertical splitter and use as much space as possible for the
    // editor (by asking the viewer to take only 10% of the desktop's height and
    // the editor to take whatever it can), and add it to our raw CellML view
    // widget

    verticalSplitter->addWidget(viewer);
    verticalSplitter->addWidget(editor);

    verticalSplitter->setSizes(QList<int>() << 0.1*qApp->desktop()->screenGeometry().height()
                                            << qApp->desktop()->screenGeometry().height());

    mUi->verticalLayout->addWidget(verticalSplitter);
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
