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

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepositoryWindow {

//==============================================================================

class CellmlModelRepositoryWindowModel
{
public:
    explicit CellmlModelRepositoryWindowModel(const QString &pUrl,
                                              const QString &pName);

    QString url() const;
    QString name() const;

private:
    QString mUrl;
    QString mName;
};

//==============================================================================

typedef QList<CellmlModelRepositoryWindowModel> CellmlModelRepositoryWindowModels;

//==============================================================================

class CellmlModelRepositoryWindowWidget : public Core::WebViewWidget,
                                          public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindowWidget(QWidget *pParent);
    ~CellmlModelRepositoryWindowWidget();

    virtual void retranslateUi();

    void initialize(const CellmlModelRepositoryWindowModels &pModels,
                    const QString &pErrorMessage);

    void filter(const QString &pFilter);

    void addModelFiles(const QString &pUrl, const QStringList &pSourceFiles);
    void showModelFiles(const QString &pUrl, const bool &pShow = true);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);

private:
    Ui::CellmlModelRepositoryWindowWidget *mGui;

    QMenu *mContextMenu;

    QString mOutputTemplate;

    QStringList mModelNames;
    QMap<QString, int> mModelUrlsIds;

    QString mErrorMessage;

    int mNumberOfFilteredModels;

    QString mUrl;

Q_SIGNALS:
    void cloneModel(const QString &pUrl, const QString &pDescription);
    void showModelFiles(const QString &pUrl, const QString &pDescription);

    void modelFileOpenRequested(const QString &pUrl);

private Q_SLOTS:
    void on_actionCopy_triggered();

    void linkClicked();

    void showCustomContextMenu();
};

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
