/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// PMR support
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrsupport.h"

//==============================================================================

#include <QFileDialog>
#include <QMainWindow>

//==============================================================================

#include "libgit2begin.h"
    #include "git2/errors.h"
    #include "git2/repository.h"
#include "libgit2end.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

QString getEmptyDirectory()
{
    // Retrieve and return the name of an empty directory

    return Core::getEmptyDirectory(QObject::tr("Select Empty Directory"));
}

//==============================================================================

QString getNonGitDirectory()
{
    // Retrieve and return the name of a non-Git directory

    QFileDialog dialog(Core::mainWindow(), QObject::tr("Select Directory"),
                       Core::activeDirectory());

    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    forever {
        if (dialog.exec() != QDialog::Accepted) {
            break;
        }

        QString res = Core::canonicalDirName(dialog.selectedFiles().first());

        if (!res.isEmpty()) {
            // We have retrieved a file name, so update our active directory

            Core::setActiveDirectory(res);

            // Check whether the directory is writable

            if (!Core::isDirectory(res)) {
                Core::warningMessageBox(QObject::tr("Select Directory"),
                                        QObject::tr("Please choose a writable directory."));

                continue;
            }

            // Check whether the directory is a Git directory

            if (isGitDirectory(res)) {
                Core::warningMessageBox(QObject::tr("Select Directory"),
                                        QObject::tr("Please choose a non-Git directory."));

                continue;
            }
        }

        return res;
    }

    return {};
}

//==============================================================================

bool isGitDirectory(const QString &pDirName)
{
    // Return whether the given directory is a Git directory

    if (pDirName.isEmpty()) {
        return false;
    }

    git_repository *gitRepository = nullptr;

    if (git_repository_open(&gitRepository, pDirName.toUtf8().constData()) == GIT_OK) {
        git_repository_free(gitRepository);

        return true;
    }

    return false;
}

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
