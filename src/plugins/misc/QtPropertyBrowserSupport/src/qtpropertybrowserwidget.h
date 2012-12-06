//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#ifndef QTPROPERTYBROWSERWIDGET_H
#define QTPROPERTYBROWSERWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "qtpropertybrowsersupportglobal.h"

//==============================================================================

#include <QTreeWidget>

//==============================================================================

#include <QtAbstractPropertyManager>
#include <QtTreePropertyBrowser>

//==============================================================================

class QtVariantProperty;
class QtVariantPropertyManager;

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class QtPropertyBrowserTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    int rowHeight(const QModelIndex &pIndex) const;
};

//==============================================================================

class DoublePropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT

public:
    explicit DoublePropertyManager(QObject *pParent = 0);

    QString unit(QtProperty *pProperty) const;
    void setUnit(QtProperty *pProperty, const QString &pUnit);

protected:
    virtual QString valueText(const QtProperty *pProperty) const;

    virtual void initializeProperty(QtProperty *pProperty);
    virtual void uninitializeProperty(QtProperty *pProperty);

private:
    struct Data {
        double value;
        QString unit;
    };

    QMap<const QtProperty *, Data> mData;

Q_SIGNALS:
    void unitChanged(QtProperty *pProperty, const QString &pUnit);
};

//==============================================================================

class DoubleEditWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit DoubleEditWidget(QWidget *pParent = 0);

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

class DoubleEditFactory : public QtAbstractEditorFactory<DoublePropertyManager>
{
    Q_OBJECT

public:
    explicit DoubleEditFactory(QObject *pParent = 0);

protected:
    virtual void connectPropertyManager(DoublePropertyManager *pManager);
    virtual void disconnectPropertyManager(DoublePropertyManager *pManager);

    virtual QWidget * createEditor(DoublePropertyManager *pManager,
                                   QtProperty *pProperty, QWidget *pParent);

private:
    QMap<QtProperty *, QList<DoubleEditWidget *> > mEditors;
    QMap<DoubleEditWidget *, QtProperty *> mProperties;

Q_SIGNALS:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();

private Q_SLOTS:
    void editorDestroyed(QObject *pEditor);

    void unitChanged(QtProperty *pProperty, const QString &pUnit);
};

//==============================================================================

class QTPROPERTYBROWSERSUPPORT_EXPORT QtPropertyBrowserWidget : public QtTreePropertyBrowser,
                                                                public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit QtPropertyBrowserWidget(const bool &pAutoResizeHeight,
                                     QWidget *pParent = 0);
    explicit QtPropertyBrowserWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QSize sizeHint() const;

    QtVariantProperty * addProperty(const int pType,
                                    const QString &pName = QString());

    void selectFirstProperty();

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    QtVariantPropertyManager *mPropertyManager;

    bool mAutoResizeHeight;

    void constructor(const bool &pAutoResizeHeight = false);

private Q_SLOTS:
    void resizeHeight();
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
