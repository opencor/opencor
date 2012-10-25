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
class QPushButton;
class QWebView;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlListWidget;
class CellmlAnnotationViewMetadataDetailsWidget;
class CellMLAnnotationViewPlugin;

//==============================================================================

class CellmlAnnotationViewWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(CellMLAnnotationViewPlugin *pPluginParent,
                                        const QString &pFileName,
                                        QWidget *pParent = 0);
    ~CellmlAnnotationViewWidget();

    virtual void retranslateUi();

    QString pluginViewName() const;

    CellMLSupport::CellmlFile * cellmlFile() const;

    CellmlAnnotationViewCellmlListWidget * cellmlList() const;
    CellmlAnnotationViewMetadataDetailsWidget * metadataDetails() const;

    void updateWebViewerWithQualifierDetails(QWebView *pWebView,
                                             const QString &pQualifier,
                                             const bool &pRetranslate);
    void updateWebViewerWithResourceDetails(QWebView *pWebView,
                                            const QString &pResource,
                                            const bool &pRetranslate);
    void updateWebViewerWithIdDetails(QWebView *pWebView,
                                      const QString &pResource,
                                      const QString &pId,
                                      const bool &pRetranslate);

private:
    Ui::CellmlAnnotationViewWidget *mGui;

    CellMLAnnotationViewPlugin *mPluginParent;

    CellMLSupport::CellmlFile *mCellmlFile;

    CellmlAnnotationViewCellmlListWidget *mCellmlList;
    CellmlAnnotationViewMetadataDetailsWidget *mMetadataDetails;

    QString mModelQualifierSvg;
    QString mBiologyQualifierSvg;

    QString mQualifierInformationTemplate;

    QMap<QWebView *, QUrl> oldWebViewUrls;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

private Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);
    void emitSplitterMoved();

    void addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple) const;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
