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

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT UserMessageWidget : public Widget
{
    Q_OBJECT

public:
    explicit UserMessageWidget(QWidget *pParent,
                               const QString &pIcon = QString(),
                               const QString &pMessage = QString());
    ~UserMessageWidget();

    void setIcon(const QString &pIcon);
    void setMessage(const QString &pMessage);

private:
    Ui::UserMessageWidget *mGui;

    QString mIcon;
    QString mMessage;

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
