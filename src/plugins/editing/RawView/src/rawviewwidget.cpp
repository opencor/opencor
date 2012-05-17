//==============================================================================
// Raw view widget
//==============================================================================

#include "qscintilla.h"
#include "rawviewwidget.h"

//==============================================================================

#include "ui_rawviewwidget.h"

//==============================================================================

#include <QFileInfo>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

RawViewWidget::RawViewWidget(QWidget *pParent, const QString &pFileName) :
    Widget(pParent),
    mGui(new Ui::RawViewWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and set up a raw Scintilla editor

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

    QsciScintilla *editor = new QScintillaSupport::QScintilla(pParent,
                                                              fileContents,
                                                              fileIsWritable);

    // Set the raw view widget's focus proxy to the editor

    setFocusProxy(editor);

    // Add the editor to the raw view widget

    mGui->layout->addWidget(editor);
}

//==============================================================================

RawViewWidget::~RawViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
