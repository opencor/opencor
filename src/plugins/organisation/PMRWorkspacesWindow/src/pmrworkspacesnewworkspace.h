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
// Create a new PMR workspace dialog
//==============================================================================

#pragma once

//==============================================================================

#include <QDialog>

//==============================================================================

namespace Ui {
    class PmrWorkspacesNewWorkspace;
}

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

class PmrWorkspacesNewWorkspace : public QDialog
{
    Q_OBJECT

public:
    explicit PmrWorkspacesNewWorkspace(QWidget * pParent=0);
    ~PmrWorkspacesNewWorkspace();

    virtual void retranslateUi();

    const QString description() const;
    const QString path() const;
    const QString title() const;

private slots:
    void titleTextChanged(const QString &text);

    void choosePath(const bool &checked);
    void setPathToolTip(const QString &text);


private:
    Ui::PmrWorkspacesNewWorkspace *mGui;
    bool mPathChosen;
};

//==============================================================================

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
