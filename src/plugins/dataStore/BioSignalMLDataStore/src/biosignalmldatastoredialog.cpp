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
// BioSignalML data store dialog
//==============================================================================

#include "biosignalmldatastoredialog.h"
#include "datastoreinterface.h"
#include "widget.h"

//==============================================================================

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BiosignalmlDataStoreDialog::BiosignalmlDataStoreDialog(DataStore::DataStore *pDataStore,
                                                       const QMap<int, QIcon> &pIcons,
                                                       QWidget *pParent) :
    DataStore::DataStoreDialog("BioSignalMLDataStore", pDataStore, false,
                               pIcons, pParent)
{
    // Customise our GUI

    setWindowTitle(tr("Export Data"));

    // Create a form-like widget

    auto widget = new Core::Widget(this);
    auto formLayout = qobject_cast<QFormLayout *>(widget->createLayout(Core::Widget::Layout::Form));

    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    // Add a label/field for our short name

    mNameValue = new QLineEdit(this);

#ifdef Q_OS_MAC
    mNameValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    formLayout->addRow(boldLabel(tr("Name:")), mNameValue);

    // Add a label/field for our author

    mAuthorValue = new QLineEdit(this);

#ifdef Q_OS_MAC
    mAuthorValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    formLayout->addRow(boldLabel(tr("Author:")), mAuthorValue);

    // Add a label/field for our description

    mDescriptionValue = new QTextEdit(this);

    formLayout->addRow(boldLabel(tr("Description:")), mDescriptionValue);

    // Make our short name value our focus proxy and add our widget to ourselves

    widget->setFocusProxy(mNameValue);

    addWidget(widget);
}

//==============================================================================

QString BiosignalmlDataStoreDialog::name() const
{
    // Return our name

    return mNameValue->text();
}

//==============================================================================

QString BiosignalmlDataStoreDialog::author() const
{
    // Return our author

    return mAuthorValue->text();
}

//==============================================================================

QString BiosignalmlDataStoreDialog::description() const
{
    // Return our description

    return mDescriptionValue->toPlainText();
}

//==============================================================================

QLabel * BiosignalmlDataStoreDialog::boldLabel(const QString &pText)
{
    // Create and return a label after having made it bold

    auto res = new QLabel(pText, this);
    QFont newFont = res->font();

    newFont.setBold(true);

    res->setFont(newFont);

    return res;
}

//==============================================================================

} // namespace BioSignalMLDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
