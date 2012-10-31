//==============================================================================
// User message widget
//==============================================================================

#ifndef USERMESSAGEWIDGET_H
#define USERMESSAGEWIDGET_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QLabel>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT UserMessageWidget : public QLabel
{
    Q_OBJECT

public:
    explicit UserMessageWidget(const QString &pIcon, const QString &pMessage,
                               QWidget *pParent = 0);
    explicit UserMessageWidget(const QString &pIcon, QWidget *pParent = 0);

    void setIcon(const QString &pIcon);
    void setMessage(const QString &pMessage);

private:
    QString mIcon;
    QString mMessage;

    void constructor(const QString &pIcon,
                     const QString &pMessage = QString());

    void updateMessage();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
