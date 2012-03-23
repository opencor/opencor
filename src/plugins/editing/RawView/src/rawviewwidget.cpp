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

RawViewWidget::RawViewWidget(const QString &pFileName, QWidget *pParent) :
    Widget(pParent),
    mUi(new Ui::RawViewWidget)
{
    // Set up the UI

    mUi->setupUi(this);

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

    QsciScintilla *editor = new QScintillaSupport::QScintilla(fileContents,
                                                              fileIsWritable,
                                                              0, pParent);

    // Set the raw view widget's focus proxy to the editor

    setFocusProxy(editor);

    // Add the editor to the raw view widget

    mUi->verticalLayout->addWidget(editor);
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
