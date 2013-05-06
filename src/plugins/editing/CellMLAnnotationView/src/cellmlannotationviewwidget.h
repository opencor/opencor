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
    class CellMLAnnotationViewWidget;
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

class CellMLAnnotationViewCellMLListWidget;
class CellMLAnnotationViewMetadataDetailsWidget;
class CellMLAnnotationViewPlugin;

//==============================================================================

class CellMLAnnotationViewWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellMLAnnotationViewWidget(CellMLAnnotationViewPlugin *pPluginParent,
                                        const QString &pFileName,
                                        QWidget *pParent = 0);
    ~CellMLAnnotationViewWidget();

    virtual void retranslateUi();

    QString pluginViewName() const;

    CellMLSupport::CellMLFile * cellmlFile() const;

    CellMLAnnotationViewCellMLListWidget * cellmlList() const;
    CellMLAnnotationViewMetadataDetailsWidget * metadataDetails() const;

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
    Ui::CellMLAnnotationViewWidget *mGui;

    CellMLAnnotationViewPlugin *mPluginParent;

    CellMLSupport::CellMLFile *mCellMLFile;

    CellMLAnnotationViewCellMLListWidget *mCellMLList;
    CellMLAnnotationViewMetadataDetailsWidget *mMetadataDetails;

    QString mModelQualifierSvg;
    QString mBiologyQualifierSvg;

    QString mQualifierInformationTemplate;

    QMap<QWebView *, QUrl> oldWebViewUrls;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

private Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);
    void emitSplitterMoved();

    void addRdfTriple(CellMLSupport::CellMLFileRdfTriple *pRdfTriple) const;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
