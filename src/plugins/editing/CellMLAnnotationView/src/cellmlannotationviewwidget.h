//==============================================================================
// CellML annotation view widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWWIDGET_H
#define CELLMLANNOTATIONVIEWWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewWidget;
}

//==============================================================================

class QComboBox;
class QLabel;
class QLineEdit;
class QWebView;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewListsWidget;
class CellmlAnnotationViewDetailsWidget;
class CellMLAnnotationViewPlugin;

//==============================================================================

class CellmlAnnotationViewWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(QWidget *pParent,
                                        CellMLAnnotationViewPlugin *pPluginParent,
                                        const QString &pFileName);
    ~CellmlAnnotationViewWidget();

    virtual void retranslateUi();

    CellMLAnnotationViewPlugin * pluginParent() const;

    CellmlAnnotationViewListsWidget * listsWidget() const;
    CellmlAnnotationViewDetailsWidget * detailsWidget() const;

    void updateWebViewerWithQualifierDetails(QWebView *pWebView,
                                             const QString &pQualifier,
                                             const bool &pRetranslate);
    void updateWebViewerWithResourceDetails(QWebView *pWebView,
                                            const QString &pResource,
                                            const bool &pRetranslate);
    void updateWebViewerWithResourceIdDetails(QWebView *pWebView,
                                              const QString &pResource,
                                              const QString &pId,
                                              const bool &pRetranslate);

private:
    Ui::CellmlAnnotationViewWidget *mGui;

    CellMLAnnotationViewPlugin *mPluginParent;

    CellMLSupport::CellmlFile *mCellmlFile;

    CellmlAnnotationViewListsWidget *mListsWidget;
    CellmlAnnotationViewDetailsWidget *mDetailsWidget;

    QString mModelQualifierSvg;
    QString mBiologyQualifierSvg;

    QString mQualifierInformationTemplate;

    QMap<QWebView *, QUrl> oldWebViewUrls;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);

private Q_SLOTS:
    void emitSplitterMoved();

    void updateTabOrder(QLineEdit *pTermValue, QComboBox *pQualifierValue);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
