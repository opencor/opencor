/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
                                    const QByteArray &pData,
                                    const QString &pMimeType);

    virtual void abort();
    virtual qint64 bytesAvailable() const;

protected:
    virtual qint64 readData(char *pData, qint64 pMaxlen);

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
    virtual QNetworkReply * createRequest(Operation pOperation,
                                          const QNetworkRequest &pRequest,
                                          QIODevice *pOutgoingData = 0);

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
    ~HelpWindowWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual bool isUrlSchemeSupported(const QString &pUrlScheme);

private:
    QHelpEngine *mHelpEngine;

    QString mQchFileName;
    QString mQhcFileName;
};

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
