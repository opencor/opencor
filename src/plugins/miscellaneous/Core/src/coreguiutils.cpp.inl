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
// Core GUI utilities
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
    // Return the border colour
//---GRY--- TO BE DONE...

return Qt::red;
}

//==============================================================================

QColor highlightColor()
{
    // Return the highlight colour

    return qApp->palette().color(QPalette::Highlight);
}

//==============================================================================

QColor linkColor()
{
    // Return the link colour

    return qApp->palette().color(QPalette::Link);
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
