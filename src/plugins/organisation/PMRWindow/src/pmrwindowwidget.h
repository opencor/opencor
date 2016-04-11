/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR widget
//==============================================================================

#ifndef PMRWINDOWWIDGET_H
#define PMRWINDOWWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"
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

class PmrWindowExposure
{
public:
    explicit PmrWindowExposure(const QString &pUrl, const QString &pName);

    bool operator<(const PmrWindowExposure &pExposure) const;

    QString url() const;
    QString name() const;

private:
    QString mUrl;
    QString mName;
};

//==============================================================================

typedef QList<PmrWindowExposure> PmrWindowExposures;

//==============================================================================

class PmrWindowWidget : public Core::WebViewWidget, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit PmrWindowWidget(QWidget *pParent);
    ~PmrWindowWidget();

    virtual void retranslateUi();

    void initialize(const PmrWindowExposures &pExposures,
                    const QString &pErrorMessage, const QString &pFilter,
                    const bool &pInternetConnectionAvailable);

    void filter(const QString &pFilter);

    void addExposureFiles(const QString &pUrl,
                          const QStringList &pExposureFiles);
    void showExposureFiles(const QString &pUrl, const bool &pShow = true);

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

#endif

//==============================================================================
// End of file
//==============================================================================
