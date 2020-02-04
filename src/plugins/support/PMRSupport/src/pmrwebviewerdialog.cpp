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
// PMR Web viewer dialog
//==============================================================================

#include "plugin.h"
#include "pmrsupportplugin.h"
#include "pmrwebviewerdialog.h"
#include "progressbarwidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QMainWindow>
#include <QVBoxLayout>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebViewerDialog::PmrWebViewerDialog(QWidget *pParent) :
    Core::Dialog(pParent)
{
    // Customise our settings

    mSettings.beginGroup(SettingsPlugins);
    mSettings.beginGroup(PluginName);
    mSettings.beginGroup("PmrWebViewerDialog");

    // Customise ourselves

    auto layout = new QVBoxLayout(this);

    layout->setContentsMargins({});

    setLayout(layout);

    mWebViewer = new WebViewerWidget::WebViewerWidget(this);

    mWebViewer->webView()->setContextMenuPolicy(Qt::NoContextMenu);

    mWebViewer->showProgressBar();

    layout->addWidget(mWebViewer);

    QMainWindow *mainWindow = Core::mainWindow();

    resize(int(0.7*mainWindow->width()),
           int(0.7*mainWindow->height()));

    retranslateUi();
}

//==============================================================================

void PmrWebViewerDialog::retranslateUi()
{
    // Retranslate ourselves

    setWindowTitle(tr("PMR Authentication"));
}

//==============================================================================

bool PmrWebViewerDialog::isLoadFinished() const
{
    // Return whether our loading is finished

    return qFuzzyIsNull(mWebViewer->progressBarWidget()->value());
}

//==============================================================================

void PmrWebViewerDialog::load(const QUrl &pUrl)
{
    // Get our Web viewer to load the given URL

    mWebViewer->webView()->load(pUrl);
}

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
