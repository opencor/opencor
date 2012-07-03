//==============================================================================
// User message widget
//==============================================================================

#include "usermessagewidget.h"

//==============================================================================

#include "ui_usermessagewidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

UserMessageWidget::UserMessageWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::UserMessageWidget),
    mIcon(QString()),
    mMessage(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Customise the background colour

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);
}

//==============================================================================

UserMessageWidget::~UserMessageWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void UserMessageWidget::updateLabel()
{
    // Update the label by setting the icon to the left and the message to the
    // right

    mGui->label->setText(QString("<table align=center>"
                                 "    <tr valign=middle>"
                                 "        <td align=right>"
                                 "            <img src=\"%1\"/>"
                                 "        </td>"
                                 "        <td align=left>"
                                 "            %2"
                                 "        </td>"
                                 "    </tr>"
                                 "</table>").arg(mIcon, mMessage));
}

//==============================================================================

void UserMessageWidget::setIcon(const QString &pIcon)
{
    // Set the icon

    if (pIcon.compare(mIcon)) {
        mIcon = pIcon;

        updateLabel();
    }
}

//==============================================================================

void UserMessageWidget::setMessage(const QString &pMessage)
{
    // Set the message

    if (pMessage.compare(mMessage)) {
        mMessage = pMessage;

        updateLabel();
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
