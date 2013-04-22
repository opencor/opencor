//==============================================================================
// Organisation widget
//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QtSingleApplication>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

OrganisationWidget::OrganisationWidget(QWidget *pParent) :
    DockWidget(pParent)
{
}

//==============================================================================

void OrganisationWidget::openFile(const QString &pFileName) const
{
    // Ask OpenCOR to open the file

    static_cast<SharedTools::QtSingleApplication *>(qApp)->handleAction("gui://openFile/"+pFileName);
}

//==============================================================================

void OrganisationWidget::openFiles(const QStringList &pFileNames) const
{
    // Ask OpenCOR to open the files

    static_cast<SharedTools::QtSingleApplication *>(qApp)->handleAction("gui://openFiles/"+pFileNames.join("|"));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
