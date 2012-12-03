//==============================================================================
// Raw view widget
//==============================================================================

#include "rawviewwidget.h"

//==============================================================================

#include <QFileInfo>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

RawViewWidget::RawViewWidget(const QString &pFileName, QWidget *pParent) :
    QScintillaWidget(pParent)
{
    // Customise ourselves

    QFile file(pFileName);
    QString fileContents = QString();
    bool fileIsReadOnly = false;

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        // We could open the file, so retrieve its contents and whether it can
        // be written to

        fileContents = QTextStream(&file).readAll();
        fileIsReadOnly = !(QFileInfo(pFileName).isWritable());

        // We are done with the file, so close it

        file.close();
    }

    setContents(fileContents);
    setReadOnly(fileIsReadOnly);
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
