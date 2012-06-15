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

public:
    explicit CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget *mGui;

    QWidget *mWidget;
    QGridLayout *mLayout;

    CellMLSupport::CellmlFileRdfTriples mRdfTriples;

    enum Type {
        Qualifier,
        Resource,
        Id
    };

    void genericLookup(const QString &pRdfTripleInfo, const Type &pType) const;

Q_SIGNALS:
    void qualifierLookupRequested(const QString &pQualifier) const;
    void resourceLookupRequested(const QString &pResource) const;
    void resourceIdLookupRequested(const QString &pResource,
                                   const QString &pId) const;

private Q_SLOTS:
    void lookupQualifier(const QString &pRdfTripleInfo) const;
    void lookupResource(const QString &pRdfTripleInfo) const;
    void lookupResourceId(const QString &pRdfTripleInfo) const;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
