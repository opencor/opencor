//==============================================================================
// CellML annotation view metadata edit details widget
//==============================================================================

#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewmetadatalistwidget.h"
#include "cellmlannotationviewwidget.h"
#include "coreutils.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataeditdetailswidget.h"

//==============================================================================

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStackedWidget>
#include <QVariant>
#include <QVBoxLayout>

//==============================================================================

#include <QJsonParser>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataEditDetailsWidget),
    mMainWidget(0),
    mMainLayout(0),
    mFormWidget(0),
    mFormLayout(0),
    mGridWidget(0),
    mGridLayout(0),
    mTerm(QString()),
    mTermUrl(QString()),
    mOtherTermUrl(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a stacked widget which will contain our GUI

    mWidget = new QStackedWidget(this);

    // Add our stacked widget to our scroll area

    setWidget(mWidget);

    // Create a network access manager so that we can then retrieve a list of
    // CellML models from the CellML Model Repository

    mNetworkAccessManager = new QNetworkAccessManager(this);

    // Make sure that we get told when the download of our Internet file is
    // complete

    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(finished(QNetworkReply *)) );
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::~CellmlAnnotationViewMetadataEditDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // For the rest of our GUI, it's easier to just update it, so...

    updateGui();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui(const bool &pPopulate)
{
    // Note: we are using certain layouts to dislay the contents of our view,
    //       but this unfortunately results in some very bad flickering on Mac
    //       OS X. This can, however, be addressed using a stacked widget, so...

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Create a widget which will contain our GUI

    QWidget *newMainWidget = new QWidget(this);
    QVBoxLayout *newMainLayout = new QVBoxLayout(newMainWidget);

    newMainLayout->setMargin(0);

    newMainWidget->setLayout(newMainLayout);

    // Populate our GUI, but only if requested

    QWidget *newFormWidget = 0;
    QFormLayout *newFormLayout = 0;
    QStackedWidget *newStackedWidget = 0;

    QLineEdit *termValue = 0;

    if (pPopulate) {
        // Create a form widget which will contain our qualifier and term fields

        newFormWidget = new QWidget(newMainWidget);
        newFormLayout = new QFormLayout(newFormWidget);

        newFormWidget->setLayout(newFormLayout);

        // Add our qualifier and term fields

        QComboBox *qualifierValue = new QComboBox(newFormWidget);

        qualifierValue->addItems(CellMLSupport::CellmlFileRdfTriple::qualifiersAsStringList());

        newFormLayout->addRow(Core::newLabel(newFormWidget, tr("Qualifier:"), true),
                              qualifierValue);

        termValue = new QLineEdit(newFormWidget);

        connect(termValue, SIGNAL(textChanged(const QString &)),
                this, SLOT(lookupTerm(const QString &)));

        newFormLayout->addRow(Core::newLabel(newFormWidget, tr("Term:"), true),
                              termValue);

        // Let people know that the GUI has been populated

        emit guiPopulated(qualifierValue, termValue);

        // Create a stacked widget which will contain a grid with the results of
        // the lookup

        newStackedWidget = new QStackedWidget(newMainWidget);

        // Add our 'internal' widgets to our new main widget

        newMainLayout->addWidget(newFormWidget);
        newMainLayout->addWidget(Core::newLineWidget(newMainWidget));
        newMainLayout->addWidget(newStackedWidget);
    }

    // Add our new widget to our stacked widget

    mWidget->addWidget(newMainWidget);

    // Remove the contents of our old form layout

    if (mFormWidget)
        for (int i = 0, iMax = mFormLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mFormLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

    // Get rid of our old main widget and layout (and its contents)

    if (mMainWidget) {
        mWidget->removeWidget(mMainWidget);

        for (int i = 0, iMax = mMainLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mMainLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

        delete mMainWidget;
    }

    // Keep track of our new main widgets and layouts

    mMainWidget = newMainWidget;
    mMainLayout = newMainLayout;

    mFormWidget = newFormWidget;
    mFormLayout = newFormLayout;

    // Reset the term which was being looked up, if any

    if (termValue)
        termValue->setText(mTerm);

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui(const Items &pItems,
                                                              const QString &pErrorMsg)
{
//---GRY--- TO BE DONE...

if (pItems.isEmpty()) {
    if (pErrorMsg.isEmpty())
        qDebug(">>> No ontological terms were retrieved...");
    else
        qDebug(">>> Something went wrong: %s", qPrintable(pErrorMsg));
} else {
    foreach (const Item &item, pItems) {
        qDebug(">>> Ontological term:");
        qDebug(">>>    ---> Resource: %s", qPrintable(item.resource));
        qDebug(">>>    ---> Id:       %s", qPrintable(item.id));
        qDebug(">>>    ---> Name:     %s", qPrintable(item.name));
    }
}
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::Item CellmlAnnotationViewMetadataEditDetailsWidget::item(const QString &pResource,
                                                                                                        const QString &pId,
                                                                                                        const QString &pName)
{
    // Return a formatted Item 'object'

    Item res;

    res.resource = pResource;
    res.id       = pId;
    res.name     = pName;

    return res;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupTerm(const QString &pTerm)
{
    // Keep track of the term to lookup

    mTerm = pTerm;

    // Retrieve some possible ontological terms based on the given term

    QString termUrl = QString("http://www.semanticsbml.org/semanticSBML/annotate/search.json?q=%1&full_info=1").arg(pTerm);

    if (mTermUrl.isEmpty()) {
        // No other term is being looked up, so keep track of the given term and
        // look it up

        mTermUrl = termUrl;

        mNetworkAccessManager->get(QNetworkRequest(termUrl));
    } else {
        // Another term is already being looked up, so keep track of the given
        // term for later

        mOtherTermUrl = termUrl;
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::finished(QNetworkReply *pNetworkReply)
{
    // Retrieve the list of ontological terms, should we have retrieved it
    // without any problem

    Items items = Items();
    QString errorMsg = QString();

    if (pNetworkReply->error() == QNetworkReply::NoError) {
        // Parse the JSON code

        QJson::Parser jsonParser;
        bool parsingOk;

        QVariantMap resultMap = jsonParser.parse(pNetworkReply->readAll(), &parsingOk).toMap();

        if (parsingOk) {
            // Retrieve the list of ontological terms

            foreach (const QVariant &ontologicalsTermVariant, resultMap["result"].toList()) {
                QVariantList ontologicalTermVariant = ontologicalsTermVariant.toList();

                for (int i = 0, iMax = ontologicalTermVariant.count(); i < iMax; ++i) {
                    // At this stage, we have an ontological term in the form of
                    // a MIRIAM URN and a name (as well as a URL, but we don't
                    // care about it), so we need to decode the MIRIAM URN to
                    // retrieve the corresponding resource and id

                    QVariantMap ontologicalTermMap = ontologicalTermVariant[i].toMap();

                    QString resource = QString();
                    QString id = QString();

                    CellMLSupport::CellmlFileRdfTriple::decodeMiriamUrn(ontologicalTermMap["uri"].toString(),
                                                                        resource, id);

                    // Add the ontological term to our list

                    items << item(resource, id,
                                  ontologicalTermMap["name"].toString());
                }
            }
        } else {
            // Something went wrong, so...

            errorMsg = jsonParser.errorString();
        }
    } else {
        // Something went wrong, so...

        errorMsg = pNetworkReply->errorString();
    }

    // We are done looking up the term, so...

    mTermUrl = QString();

    // Lookup another term, should there be one to lookup, or update the GUI
    // with the results of the lookup

    if (!mOtherTermUrl.isEmpty()) {
        // There is another term to lookup, so...

        mNetworkAccessManager->get(QNetworkRequest(mOtherTermUrl));

        mOtherTermUrl = QString();
    } else {
        // No other term to lookup, so we can update our GUI with the results of
        // the lookup

static int counter = 0;
qDebug("---[%03d]--------------------------------------", ++counter);
qDebug("URL: %s", qPrintable(pNetworkReply->url().toString()));

        updateGui(items, errorMsg);
    }
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
