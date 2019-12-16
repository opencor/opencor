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
// Remote file dialog
//==============================================================================

#include "remotefiledialog.h"

//==============================================================================

#include <QApplication>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

RemoteFileDialog::RemoteFileDialog(const QString &pTitle, QWidget *pParent) :
    Dialog(pParent)
{
    // Customise our settings

    mSettings.beginGroup("RemoteFileDialog");

    // Customise ourselves
//---OPENCOR--- THE ORIGINAL PLAN WAS TO HAVE A REGULAR EXPRESSION TO VALIDATE A
//              URL, BUT IT LOOKS LIKE THERE MIGHT BE AN ISSUE WITH
//              QRegularExpressionValidator, SO WE SIMPLY ALLOW FREE TEXT FOR
//              NOW (SEE https://bugreports.qt.io/browse/QTBUG-38034)...

    setWindowTitle(pTitle);

    auto layout = new QGridLayout(this);

    setLayout(layout);

    mUrlLabel = new QLabel(tr("URL:"), this);
    mUrlValue = new QLineEdit(this);

    mUrlValue->setMinimumWidth(qApp->primaryScreen()->availableGeometry().width()/5);

    layout->addWidget(mUrlLabel, 0, 0);
    layout->addWidget(mUrlValue, 0, 1);

    mButtonBox = new QDialogButtonBox(QDialogButtonBox::Open|QDialogButtonBox::Cancel, this);

    layout->addWidget(mButtonBox, 1, 0, 1, 2);

    layout->setSizeConstraint(QLayout::SetFixedSize);

    changed();

    // Some connections to handle our remote file dialog

    connect(mUrlValue, &QLineEdit::textChanged,
            this, &RemoteFileDialog::changed);

    connect(mButtonBox, &QDialogButtonBox::accepted,
            this, &RemoteFileDialog::accept);
    connect(mButtonBox, &QDialogButtonBox::rejected,
            this, &RemoteFileDialog::reject);
}

//==============================================================================

QString RemoteFileDialog::url() const
{
    // Return our URL

    return mUrlValue->text();
}

//==============================================================================

void RemoteFileDialog::changed()
{
    // Enable/disable the open button depending on whether we have some text for
    // the remote file

    mButtonBox->button(QDialogButtonBox::Open)->setEnabled(!mUrlValue->text().isEmpty());
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
