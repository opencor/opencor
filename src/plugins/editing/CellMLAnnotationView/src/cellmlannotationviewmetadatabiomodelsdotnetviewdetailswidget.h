//==============================================================================
// CellML annotation view metadata BioModels.Net view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATABIOMODELSDOTNETVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATABIOMODELSDOTNETVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget;
}

//==============================================================================

class QGridLayout;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget : public QScrollArea,
                                                                     public Core::CommonWidget
{
    Q_OBJECT

private:
    enum Type {
        Unknown,
        Qualifier,
        Resource,
        Id
    };

public:
    explicit CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples,
                   const QString &pRdfTripleInfo = QString(),
                   const Type &pType = Unknown);

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget *mGui;

    QWidget *mWidget;
    QGridLayout *mLayout;

    CellMLSupport::CellmlFileRdfTriples mRdfTriples;

    QString mRdfTripleInfo;
    Type mType;

    void genericLookup(const QString &pRdfTripleInfo, const Type &pType);

Q_SIGNALS:
    void qualifierLookupRequested(const QString &pQualifier) const;
    void resourceLookupRequested(const QString &pResource) const;
    void resourceIdLookupRequested(const QString &pResource,
                                   const QString &pId) const;

private Q_SLOTS:
    void lookupQualifier(const QString &pRdfTripleInfo);
    void lookupResource(const QString &pRdfTripleInfo);
    void lookupResourceId(const QString &pRdfTripleInfo);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
