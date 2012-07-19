//==============================================================================
// CellMLAnnotationView plugin
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWPLUGIN_H
#define CELLMLANNOTATIONVIEWPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

class QSettings;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationViewPluginInfo();

//==============================================================================

class CellmlAnnotationViewWidget;

//==============================================================================

class CellMLAnnotationViewPlugin : public QObject, public CoreInterface,
                                   public GuiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit CellMLAnnotationViewPlugin();
    ~CellMLAnnotationViewPlugin();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QWidget * viewWidget(const QString &pFileName);
    virtual bool hasViewWidget(const QString &pFileName) const;
    virtual void deleteViewWidget(const QString &pFileName);
    virtual QString viewName();

    virtual void retranslateUi();

private:
    QList<int> mSizes;
    QList<int> mMetadataDetailsWidgetSizes;

    QMap<QString, CellmlAnnotationViewWidget *> mViewWidgets;

private Q_SLOTS:
    void splitterMoved(const QList<int> &pSizes);
    void metadataDetailsWidgetSplitterMoved(const QList<int> &pSizes);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
