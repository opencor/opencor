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

    void setValue(const QString &pValue);
    void setUnit(const QString &pUnit);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    QString mUnit;

Q_SIGNALS:
    void valueChanged(DoubleEditorWidget *, const QString &pValue);
    void unitChanged(DoubleEditorWidget *, const QString &pUnit);

    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();

private Q_SLOTS:
    void emitValueChanged(const QString &pValue);
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
