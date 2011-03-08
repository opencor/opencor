#include "onefieldwindow.h"

#include "ui_onefieldwindow.h"

#include <QMessageBox>

OneFieldWindow::OneFieldWindow(const QString &pTitle, const QString &pFieldName,
                               const QString &pInfoMsg, QWidget *pParent) :
    QDialog(pParent),
    mUi(new Ui::OneFieldWindow),
    mInfoMsg(pInfoMsg)
{
    // Set up the UI

    mUi->setupUi(this);

    // Set up the title and and field name

    setWindowTitle(pTitle);

    mUi->fieldLabel->setText(pFieldName);

    // Set the focus on the field value

    mUi->fieldValue->setFocus();
}

OneFieldWindow::~OneFieldWindow()
{
    // Delete the UI

    delete mUi;
}

QString OneFieldWindow::fieldValue()
{
    return mUi->fieldValue->text();
}

void OneFieldWindow::on_buttonBox_accepted()
{
    // We want to accept the field value, but before we do so we must ensure
    // that a value has been given

    if (mUi->fieldValue->text().isEmpty()) {
        // The field value is empty, so let the user know

        QMessageBox::information(this, qApp->applicationName(), mInfoMsg);

        // Make sure that the field value has the focus

        mUi->fieldValue->setFocus();
    }
    else {
        // Everything is fine, so...

        accept();
    }
}

void OneFieldWindow::on_buttonBox_rejected()
{
    // We want to cancel the action, so...

    reject();
}
