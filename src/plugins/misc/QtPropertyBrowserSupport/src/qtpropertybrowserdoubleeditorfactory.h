//==============================================================================
// QtPropertyBrowser double editor factory class
//==============================================================================

#ifndef QTPROPERTYBROWSERDOUBLEEDITORFACTORY_H
#define QTPROPERTYBROWSERDOUBLEEDITORFACTORY_H

//==============================================================================

#include "qtpropertybrowserdoublepropertymanager.h"

//==============================================================================

#include <QtAbstractEditorFactoryBase>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class DoubleEditorWidget;

//==============================================================================

class DoubleEditorFactory : public QtAbstractEditorFactory<DoublePropertyManager>
{
    Q_OBJECT

public:
    explicit DoubleEditorFactory(QObject *pParent = 0);

protected:
    virtual void connectPropertyManager(DoublePropertyManager *pManager);
    virtual void disconnectPropertyManager(DoublePropertyManager *pManager);

    virtual QWidget * createEditor(DoublePropertyManager *pManager,
                                   QtProperty *pProperty, QWidget *pParent);

private:
    QMap<QtProperty *, QList<DoubleEditorWidget *> > mDoubleEditors;
    QMap<DoubleEditorWidget *, QtProperty *> mProperties;

Q_SIGNALS:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();

private Q_SLOTS:
    void editorDestroyed(QObject *pEditor);

    void unitChanged(QtProperty *pProperty, const QString &pUnit);
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
