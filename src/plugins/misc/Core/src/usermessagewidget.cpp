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

void UserMessageWidget::constructor(const QString &pIcon,
                                    const QString &pMessage)
{
    // Some initialisations

    mGui = new Ui::UserMessageWidget();

    mIcon = pIcon;
    mMessage = pMessage;

    // Set up the GUI

    mGui->setupUi(this);

    // Customise the background colour

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    // Increase the size of the font

    QFont font = mGui->label->font();

    font.setPointSize(1.5*font.pointSize());

    mGui->label->setFont(font);

    // 'Initialise' the label

    updateLabel();
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon,
                                     const QString &pMessage,
                                     QWidget *pParent) :
    Widget(pParent)
{
    // Construct our object

    constructor(pIcon, pMessage);
}

//==============================================================================

UserMessageWidget::UserMessageWidget(const QString &pIcon, QWidget *pParent) :
    Widget(pParent)
{
    // Construct our object

    constructor(pIcon);
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

QLabel * UserMessageWidget::label() const
{
    // Return our label

    return mGui->label;
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
