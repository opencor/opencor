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
// CellML Model Repository widget
//==============================================================================

#ifndef CELLMLMODELREPOSITORYWINDOWWIDGET_H
#define CELLMLMODELREPOSITORYWINDOWWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "webviewwidget.h"

//==============================================================================

namespace Ui {
    class CellmlModelRepositoryWindowWidget;
}

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepositoryWindow {

//==============================================================================

class CellmlModelRepositoryWindowWidget : public Core::WebViewWidget,
                                          public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindowWidget(QWidget *pParent);
    ~CellmlModelRepositoryWindowWidget();

    virtual void retranslateUi();

    void initialize(const QStringList &pModelNames,
                    const QStringList &pModelUrls,
                    const QString &pErrorMessage);

    void filter(const QString &pFilter);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);

private:
    Ui::CellmlModelRepositoryWindowWidget *mGui;

    QString mOutputTemplate;

    QStringList mModelNames;
    QString mErrorMessage;

    int mNumberOfModels;
    int mNumberOfFilteredModels;

private Q_SLOTS:
    void openLink(const QUrl &pUrl);
};

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
