/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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
#include <QSettings>
#include <QTextEdit>

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BiosignalmlDataStoreDialog::BiosignalmlDataStoreDialog(DataStore::DataStore *pDataStore, QWidget *pParent) :
    DataStore::DataStoreDialog(pDataStore, false, pParent)
{
    // Customise our GUI

    setWindowTitle(tr("Export Data"));

    // Create our 'special' settings
    // Note: special in the sense that we don't retrieve them from the plugin
    //       itself since this is not a view, a window or anything like that...

    mSettings = new QSettings();

    mSettings->beginGroup(SettingsPlugins);
    mSettings->beginGroup("BioSignalMLDataStore");
    mSettings->beginGroup("DataStoreDialog");

    // Create a form-like widget

    Core::Widget *widget = new Core::Widget(this);
    QFormLayout *formLayout = qobject_cast<QFormLayout *>(widget->createLayout(Core::Widget::FormLayout));

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

    QLabel *res = new QLabel(pText, this);
    QFont newFont = res->font();

    newFont.setBold(true);

    res->setFont(newFont);

    return res;
}

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
