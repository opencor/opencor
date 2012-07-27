//==============================================================================
// CellML annotation view metadata normal view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATANORMALVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATANORMALVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataNormalViewDetailsWidget;
}

//==============================================================================

class QGridLayout;
class QStackedWidget;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataNormalViewDetailsWidget : public QScrollArea,
                                                            public Core::CommonWidget
{
    Q_OBJECT

private:
    enum Type {
        No,
        Qualifier,
        Resource,
        Id
    };

public:
    explicit CellmlAnnotationViewMetadataNormalViewDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataNormalViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(CellMLSupport::CellmlFileElement *pCellmlElement,
                   const QString &pRdfTripleInformation = QString(),
                   const Type &pType = No,
                   const int &pVerticalScrollBarPosition = 0,
                   const bool &pRetranslate = false);

    void addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    Ui::CellmlAnnotationViewMetadataNormalViewDetailsWidget *mGui;

    QStackedWidget *mWidget;

    QWidget *mGridWidget;
    QGridLayout *mGridLayout;

    CellMLSupport::CellmlFileElement *mCellmlElement;

    QString mRdfTripleInformation;
    Type mType;

    bool mLookupInformation;

    int mVerticalScrollBarPosition;
    int mNeighbourRow;

    QMap<QObject *, CellMLSupport::CellmlFileRdfTriple *> mRdfTriplesMapping;

    void genericLookup(const QString &pRdfTripleInformation = QString(),
                       const Type &pType = No,
                       const bool &pRetranslate = false);

    QString rdfTripleInformation(const int &pRow) const;

Q_SIGNALS:
    void qualifierLookupRequested(const QString &pQualifier,
                                  const bool &pRetranslate);
    void resourceLookupRequested(const QString &pResource,
                                 const bool &pRetranslate);
    void idLookupRequested(const QString &pResource, const QString &pId,
                           const bool &pRetranslate);
    void noLookupRequested();

    void metadataRemoved(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

private Q_SLOTS:
    void disableLookupInformation();

    void lookupQualifier(const QString &pRdfTripleInformation);
    void lookupResource(const QString &pRdfTripleInformation);
    void lookupId(const QString &pRdfTripleInformation);

    void removeRdfTriple();

    void showNeighbourRdfTriple();
    void showLastRdfTriple();

    void trackVerticalScrollBarPosition(const int &pPosition);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
