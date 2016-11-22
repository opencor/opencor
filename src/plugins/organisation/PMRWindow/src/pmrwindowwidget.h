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
// PMR window widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "pmrexposure.h"
#include "webviewerwidget.h"

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace PMRWindow {

//==============================================================================

class PmrWindowWidget : public WebViewerWidget::WebViewerWidget
{
    Q_OBJECT

public:
    explicit PmrWindowWidget(QWidget *pParent);

    virtual void retranslateUi();

    void initialize(const PMRSupport::PmrExposures &pExposures,
                    const QString &pFilter,
                    const QString &pErrorMessage);

    void filter(const QString &pFilter);

protected:
    virtual QSize sizeHint() const;

private:
    QMenu *mContextMenu;

    QAction *mCopyAction;

    QStringList mExposureNames;
    QBoolList mExposureDisplayed;
    QMap<QString, int> mExposureUrlId;

    bool mInitialized;

    QString mTemplate;
    QString mErrorMessage;

    int mNumberOfFilteredExposures;

    QString mExposureUrl;

    QString message() const;

    void showExposureFiles(const QString &pUrl, const bool &pShow = true);

signals:
    void cloneWorkspaceRequested(const QString &pUrl);
    void exposureFilesRequested(const QString &pUrl);

    void openExposureFileRequested(const QString &pUrl);

public slots:
    void addAndShowExposureFiles(const QString &pUrl,
                                 const QStringList &pExposureFiles);

private slots:
    void copy();

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
