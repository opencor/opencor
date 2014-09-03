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
// Help window
//==============================================================================

#ifndef HELPWINDOWWINDOW_H
#define HELPWINDOWWINDOW_H

//==============================================================================

#include "windowwidget.h"

//==============================================================================

class QHelpEngine;
class QMenu;
class QUrl;

//==============================================================================

namespace Ui {
    class HelpWindowWindow;
}

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

class HelpWindowWidget;

//==============================================================================

class HelpWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit HelpWindowWindow(QWidget *pParent);
    ~HelpWindowWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::HelpWindowWindow *mGui;

    HelpWindowWidget *mHelpWidget;

    QHelpEngine *mHelpEngine;

    QString mQchFileName;
    QString mQhcFileName;

    QMenu *mContextMenu;

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionBack_triggered();
    void on_actionForward_triggered();
    void on_actionCopy_triggered();
    void on_actionNormalSize_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionPrint_triggered();

    void showCustomContextMenu(const QPoint &pPosition) const;
};

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
