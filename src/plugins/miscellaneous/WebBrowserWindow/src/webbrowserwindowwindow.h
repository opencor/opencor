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
// Web Browser window
//==============================================================================

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

class QMenu;

//==============================================================================

namespace Ui {
    class WebBrowserWindowWindow;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class ProgressBarWidget;
}   // namespace Core

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
}   // namespace WebViewerWidget

//==============================================================================

namespace WebBrowserWindow {

//==============================================================================

class WebBrowserWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit WebBrowserWindowWindow(QWidget *pParent);
    ~WebBrowserWindowWindow();

    virtual void retranslateUi();
protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::WebBrowserWindowWindow *mGui;

    WebViewerWidget::WebViewerWidget *mWebBrowserWidget;

    QMenu *mContextMenu;

    QString mUrl;
    int mZoomLevel;

    Core::ProgressBarWidget *mProgressBarWidget;

    void setZoomLevel(const int &pZoomLevel);

private slots:
    void on_urlValue_returnPressed();
    void on_refreshButton_clicked();

    void on_actionClear_triggered();
    void on_actionBack_triggered();
    void on_actionForward_triggered();
    void on_actionCopy_triggered();
    void on_actionNormalSize_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionPrint_triggered();
    void on_actionInspect_triggered();

    void updateActions();
    void urlChanged(const QUrl &pUrl);
    void documentChanged();
    void showCustomContextMenu() const;
    void loadProgress(const int &pProgress);
    void loadFinished();
    void resetProgressBar();
};

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
