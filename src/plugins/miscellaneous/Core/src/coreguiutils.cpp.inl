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
// Core GUI utilities
//==============================================================================

QMainWindow * mainWindow()
{
    // Retrieve and return our main window

    static bool firstTime = true;
    static QMainWindow *res = 0;

    if (firstTime) {
        foreach (QWidget *widget, qApp->topLevelWidgets()) {
            if (widget->inherits("OpenCOR::MainWindow")) {
                res = qobject_cast<QMainWindow *>(widget);

                break;
            }
        }

        firstTime = false;
    }

    Q_ASSERT(res);

    return res;
}

//==============================================================================

void showEnableAction(QAction *pAction, const bool &pVisible,
                      const bool &pEnabled)
{
    // Show/enable or hide/disable the given action

    pAction->setVisible(pVisible);
    pAction->setEnabled(pVisible && pEnabled);
}

//==============================================================================

QColor baseColor()
{
    // Return the base colour

    return qApp->palette().color(QPalette::Base);
}

//==============================================================================

QColor borderColor()
{
    // Return the mid colour, which we consider as the colour to be used for a
    // 'normal' border
    // Note: this is not quite the correct colour, but nonetheless the one that
    //       is closest to it. Indeed, to use a hidden widget of sorts to
    //       retrieve the colour of a border works fine on all our supported
    //       operating systems except Windows 10 (while it works fine on
    //       previous versions of Windows) where it has a side effect that
    //       prevents OpenCOR from retrieving the size of its main window. Not
    //       only that, but we can still see the 'hidden' widget...

    return qApp->palette().color(QPalette::Mid);
}

//==============================================================================

QColor highlightColor()
{
    // Return the highlight colour

    return qApp->palette().color(QPalette::Highlight);
}

//==============================================================================

QColor shadowColor()
{
    // Return the shadow colour

    return qApp->palette().color(QPalette::Shadow);
}

//==============================================================================

QColor windowColor()
{
    // Return the window colour

    return qApp->palette().color(QPalette::Window);
}

//==============================================================================
// End of file
//==============================================================================
