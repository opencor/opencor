#ifndef ONEFIELDWINDOW_H
#define ONEFIELDWINDOW_H

#include <QDialog>

namespace Ui {
    class OneFieldWindow;
}

class OneFieldWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OneFieldWindow(const QString &pTitle, const QString &pFieldName,
                            const QString &pInfoMsg, QWidget *pParent = 0);
    ~OneFieldWindow();

    QString fieldValue();

private:
    Ui::OneFieldWindow *mUi;

    QString mInfoMsg;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif
