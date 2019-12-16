/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Help window widget
//==============================================================================

#pragma once

//==============================================================================

#include "webviewerwidget.h"

//==============================================================================

#include <QNetworkReply>

//==============================================================================

class QHelpEngine;

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

class HelpWindowNetworkReply : public QNetworkReply
{
public:
    explicit HelpWindowNetworkReply(const QNetworkRequest &pRequest,
                                    const QByteArray &pData);

    void abort() override;
    qint64 bytesAvailable() const override;

protected:
    qint64 readData(char *pData, qint64 pMaxlen) override;

private:
    QByteArray mData;
};

//==============================================================================

class HelpWindowNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit HelpWindowNetworkAccessManager(QHelpEngine *pHelpEngine,
                                            QObject *pParent);

protected:
    QNetworkReply * createRequest(Operation pOperation,
                                  const QNetworkRequest &pRequest,
                                  QIODevice *pOutgoingData = nullptr) override;

private:
    QHelpEngine *mHelpEngine;

    QString mErrorMessageTemplate;
};

//==============================================================================

class HelpWindowWidget : public WebViewerWidget::WebViewerWidget
{
    Q_OBJECT

public:
    explicit HelpWindowWidget(QWidget *pParent);
    ~HelpWindowWidget() override;

    void retranslateUi() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    bool isUrlSchemeSupported(const QString &pUrlScheme) override;

private:
    QHelpEngine *mHelpEngine;

    QString mQchFileName;
    QString mQhcFileName;
};

//==============================================================================

} // namespace HelpWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
