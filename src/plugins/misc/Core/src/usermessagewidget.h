//==============================================================================
// User message widget
//==============================================================================

#ifndef USERMESSAGEWIDGET_H
#define USERMESSAGEWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "widget.h"

//==============================================================================

namespace Ui {
    class UserMessageWidget;
}

//==============================================================================

class QLabel;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT UserMessageWidget : public Widget
{
    Q_OBJECT

public:
    explicit UserMessageWidget(const QString &pIcon, const QString &pMessage,
                               QWidget *pParent = 0);
    explicit UserMessageWidget(const QString &pIcon, QWidget *pParent = 0);
    ~UserMessageWidget();

    void setIcon(const QString &pIcon);
    void setMessage(const QString &pMessage);

    QLabel *label() const;

private:
    Ui::UserMessageWidget *mGui;

    QString mIcon;
    QString mMessage;

    void constructor(const QString &pIcon,
                     const QString &pMessage = QString());

    void updateLabel();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
