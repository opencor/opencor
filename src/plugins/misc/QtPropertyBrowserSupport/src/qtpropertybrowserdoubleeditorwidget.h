//==============================================================================
// QtPropertyBrowser double editor widget class
//==============================================================================

#ifndef QTPROPERTYBROWSERDOUBLEEDITORWIDGET_H
#define QTPROPERTYBROWSERDOUBLEEDITORWIDGET_H

//==============================================================================

#include <QLineEdit>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class DoubleEditorWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit DoubleEditorWidget(QWidget *pParent = 0);

    void setUnit(const QString &pUnit);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    QString mUnit;

Q_SIGNALS:
    void unitChanged(const QString &pUnit);

    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
