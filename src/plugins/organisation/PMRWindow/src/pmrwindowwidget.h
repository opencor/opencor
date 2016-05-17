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
// PMR widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"
#include "pmrexposure.h"
#include "webviewwidget.h"

//==============================================================================

namespace Ui {
    class PmrWindowWidget;
}

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace PMRWindow {

//==============================================================================

class PmrWindowWidget : public Core::WebViewWidget, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit PmrWindowWidget(QWidget *pParent);
    ~PmrWindowWidget();

    virtual void retranslateUi();

    void initialize(const PMRSupport::PmrExposures &pExposures,
                    const QString &pErrorMessage, const QString &pFilter,
                    const bool &pInternetConnectionAvailable);

    void filter(const QString &pFilter);

protected:
    virtual QSize sizeHint() const;

private:
    Ui::PmrWindowWidget *mGui;

    QMenu *mContextMenu;

    QStringList mExposureNames;
    QBoolList mExposureDisplayed;
    QMap<QString, int> mExposureUrlId;

    bool mInitialized;

    QString mTemplate;
    QString mErrorMessage;
    bool mInternetConnectionAvailable;

    int mNumberOfFilteredExposures;

    QString mExposureUrl;

    QString message() const;

Q_SIGNALS:
    void cloneWorkspaceRequested(const QString &pUrl);
    void showExposureFilesRequested(const QString &pUrl);

    void openExposureFileRequested(const QString &pUrl);

public Q_SLOTS:
    void addExposureFiles(const QString &pUrl,
                          const QStringList &pExposureFiles);
    void showExposureFiles(const QString &pUrl, const bool &pShow = true);

private Q_SLOTS:
    void on_actionCopy_triggered();

    void linkClicked();
    void linkHovered();

    void showCustomContextMenu();
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
