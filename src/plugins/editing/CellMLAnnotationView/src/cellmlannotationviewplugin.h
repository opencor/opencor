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

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QWidget * newViewWidget(const QString &pFileName);
    virtual QString viewName(const int &pViewIndex);

    virtual void retranslateUi();

private:
    QList<int> mHorizontalSizes;
    QList<int> mVerticalSizes;

    QList<CellmlAnnotationViewWidget *> mWidgets;

private Q_SLOTS:
    void horizontalSplitterMoved(const QList<int> &pHorizontalSizes);
    void verticalSplitterMoved(const QList<int> &pVerticalSizes);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
