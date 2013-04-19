//==============================================================================
// CellML file class
//==============================================================================

#include "cellmlfile.h"
#include "filemanager.h"

//==============================================================================

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QUrl>

//==============================================================================

#ifdef Q_OS_LINUX
    #include <stdint.h>
    // Note: the above header file is required on Linux, so we can use uint32_t
    //       (required to 'properly' make use of the CellML API). Now, we would
    //       normally have
    //           #include <cstdint>
    //       but this is not supported by the current ISO C++ standard. Indeed,
    //       to include it will generate the following error at compile time:
    //           error: #error This file requires compiler and library support
    //           for the upcoming ISO C++ standard, C++0x. This support is
    //           currently experimental, and must be enabled with the -std=c++0x
    //           or -std=gnu++0x compiler options.
    //       and well, we don't want to enable either of these options since
    //       support is still only experimental, so...
#endif

//==============================================================================

#include "IfaceVACSS.hxx"

#include "CellMLBootstrap.hpp"
#include "VACSSBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFile::CellmlFile(const QString &pFileName) :
    mFileName(pFileName),
    mModel(0),
    mRdfApiRepresentation(0),
    mRdfDataSource(0),
    mRdfTriples(CellmlFileRdfTriples(this))
{
    // Instantiate our runtime object

    mRuntime = new CellmlFileRuntime();

    // Reset ourselves

    reset();
}

//==============================================================================

CellmlFile::~CellmlFile()
{
    // Delete some internal objects

    reset();

    delete mRuntime;
}

//==============================================================================

void CellmlFile::reset()
{
    // Reset all of the file's properties

    mModel = 0;

    mRdfApiRepresentation = 0;
    mRdfDataSource = 0;

    mUriBase = QString();

    foreach (CellmlFileRdfTriple *rdfTriple, mRdfTriples)
        delete rdfTriple;

    mRdfTriples.clear();

    mValid = true;

    mIssues.clear();

    mLoadingNeeded       = true;
    mValidNeeded         = true;
    mRuntimeUpdateNeeded = true;
}

//==============================================================================

iface::cellml_api::Model * CellmlFile::model() const
{
    // Return the model associated with our CellML file

    return mModel;
}

//==============================================================================

iface::rdf_api::DataSource * CellmlFile::rdfDataSource() const
{
    // Return the data source associated with our CellML file

    return mRdfDataSource;
}

//==============================================================================

bool CellmlFile::load()
{
    if (!mLoadingNeeded)
        // The file is already loaded, so...

        return true;

    // Reset any issues that we may have found before

    mIssues.clear();

    // Get a bootstrap object and its model loader

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();
    ObjRef<iface::cellml_api::DOMModelLoader> modelLoader = cellmlBootstrap->modelLoader();

    // Try to load the model

    try {
        mModel = modelLoader->loadFromURL(QUrl::fromPercentEncoding(QUrl::fromLocalFile(mFileName).toEncoded()).toStdWString());
    } catch (iface::cellml_api::CellMLException &) {
        // Something went wrong with the loading of the model, so...

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("the model could not be loaded (%1)").arg(QString::fromStdWString(modelLoader->lastErrorMessage())));

        return false;
    }

    // In the case of a non CellML 1.0 model, we want all the imports to be
    // fully instantiated

    if (QString::fromStdWString(mModel->cellmlVersion()).compare(Cellml_1_0))
        try {
            mModel->fullyInstantiateImports();
        } catch (...) {
            // Something went wrong with the full instantiation of the imports,
            // so...

            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the model's imports could not be fully instantiated"));

            return false;
        }

    // Retrieve the URI base

    ObjRef<iface::cellml_api::URI> xmlBase = mModel->xmlBase();

    mUriBase = QString::fromStdWString(xmlBase->asText());

    // Retrieve all the RDF triples associated with the model

    ObjRef<iface::cellml_api::RDFRepresentation> rdfRepresentation = mModel->getRDFRepresentation(L"http://www.cellml.org/RDF/API");

    if (rdfRepresentation) {
        mRdfApiRepresentation = QueryInterface(rdfRepresentation);

        if (mRdfApiRepresentation) {
            mRdfDataSource = mRdfApiRepresentation->source();
            ObjRef<iface::rdf_api::TripleSet> rdfTriples = mRdfDataSource->getAllTriples();
            ObjRef<iface::rdf_api::TripleEnumerator> rdfTriplesEnumerator = rdfTriples->enumerateTriples();

            forever {
                ObjRef<iface::rdf_api::Triple> rdfTriple = rdfTriplesEnumerator->getNextTriple();

                if (!rdfTriple)
                    break;

                mRdfTriples << new CellmlFileRdfTriple(this, rdfTriple);
            }
        }
    }

    // All done, so...

    mLoadingNeeded = false;

    return true;
}

//==============================================================================

bool CellmlFile::reload()
{
    // We want to reload the file, so we must first reset it

    reset();

    // Now, we can try to (re)load the file

    return load();
}

//==============================================================================

bool CellmlFile::save(const QString &pNewFileName)
{
    // Determine the file name to use for the CellML file

    QString fileName = pNewFileName.isEmpty()?mFileName:pNewFileName;

    // Make sure that the RDF API representation is up to date by updating its
    // data source

    mRdfApiRepresentation->source(mRdfDataSource);

    // (Create and) open the file for writing

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // The file can't be opened, so...

        file.remove();

        return false;
    }

    // Write out the contents of the CellML file to the file

    QTextStream out(&file);

    out << QString::fromStdWString(mModel->serialisedText());

    file.close();

    // The CellML file being saved, it cannot be modified (should it have been
    // before)
    // Note: we must do this before making sure that mFileName is up to date
    //       since we need the old file name to update the modified status of
    //       the CellML file...

    setModified(false);

    // Make sure that mFileName is up to date

    mFileName = fileName;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool CellmlFile::isValid()
{
    if (!mValidNeeded)
        // The file has already been validated, so...

        return mValid;

    // Load (but not reload!) the file, if needed

    if (load()) {
        // The file was properly loaded (or was already loaded), so check
        // whether it is CellML valid
        // Note: validateModel() is somewhat slow, but there is (unfortunately)
        //       nothing we can do about it. Then, there is getPositionInXML()
        //       which is painfully slow, but unlike for validateModel() its use
        //       is not essential (even though it would be nice from an
        //       end-user's perspective). So, rather than retrieve the
        //       line/column of every single warning/error, we only keep track
        //       of the various warnings/errors and only retrieve their
        //       corresponding line/column when requested (definitely not neat
        //       from an end-user's perspective, but we just can't afford the
        //       time it takes to fully validate a model that has many
        //       warnings/errors)...

        ObjRef<iface::cellml_services::VACSService> vacssService = CreateVACSService();
        ObjRef<iface::cellml_services::CellMLValidityErrorSet> cellmlValidityErrorSet = vacssService->validateModel(mModel);

        // Determine the number of errors and warnings
        // Note: CellMLValidityErrorSet::nValidityErrors() returns any type of
        //       validation issue, be it an error or a warning, so we need to
        //       determine the number of true errors

        int cellmlErrorsCount = 0;

        for (int i = 0, iMax = cellmlValidityErrorSet->nValidityErrors(); i < iMax; ++i) {
            ObjRef<iface::cellml_services::CellMLValidityError> cellmlValidityIssue = cellmlValidityErrorSet->getValidityError(i);
            ObjRef<iface::cellml_services::CellMLRepresentationValidityError> cellmlRepresentationValidityError = QueryInterface(cellmlValidityIssue);

            // Determine the issue's location

            uint32_t line = 0;
            uint32_t column = 0;
            QString importedFile = QString();

            if (cellmlRepresentationValidityError) {
                // We are dealing with a CellML representation issue, so
                // determine its line and column

                ObjRef<iface::dom::Node> errorNode = cellmlRepresentationValidityError->errorNode();

                line = vacssService->getPositionInXML(errorNode,
                                                      cellmlRepresentationValidityError->errorNodalOffset(),
                                                      &column);
            } else {
                // We are not dealing with a CellML representation issue, so
                // check whether we are dealing with a semantic one

                ObjRef<iface::cellml_services::CellMLSemanticValidityError> cellmlSemanticValidityError = QueryInterface(cellmlValidityIssue);

                if (cellmlSemanticValidityError) {
                    // We are dealing with a CellML semantic issue, so determine
                    // its line and column

                    ObjRef<iface::cellml_api::CellMLElement> cellmlElement = cellmlSemanticValidityError->errorElement();
                    ObjRef<iface::cellml_api::CellMLDOMElement> cellmlDomElement = QueryInterface(cellmlElement);

                    ObjRef<iface::dom::Element> domElement = cellmlDomElement->domElement();

                    line = vacssService->getPositionInXML(domElement, 0,
                                                          &column);

                    // Also determine its imported file, if any

                    forever {
                        // Retrieve the CellML element's parent

                        ObjRef<iface::cellml_api::CellMLElement> cellmlElementParent = cellmlElement->parentElement();

                        if (!cellmlElementParent)
                            break;

                        // Check whether the parent is an imported file

                        ObjRef<iface::cellml_api::Model> importedCellmlFile = QueryInterface(cellmlElementParent);

                        if (!importedCellmlFile)
                            break;

                        // Retrieve the imported CellML element

                        ObjRef<iface::cellml_api::CellMLElement> importedCellmlElement = importedCellmlFile->parentElement();

                        if (!importedCellmlElement)
                            break;

                        // Check whether the imported CellML element is an
                        // import CellML element

                        ObjRef<iface::cellml_api::CellMLImport> importCellmlElement = QueryInterface(importedCellmlElement);

                        if (!importCellmlElement)
                            break;

                        ObjRef<iface::cellml_api::URI> xlinkHref = importCellmlElement->xlinkHref();

                        importedFile = QString::fromStdWString(xlinkHref->asText());

                        break;
                    }
                }
            }

            // Determine the issue's type

            CellmlFileIssue::Type issueType;

            if (cellmlValidityIssue->isWarningOnly()) {
                // We are dealing with a warning

                issueType = CellmlFileIssue::Warning;
            } else {
                // We are dealing with an error

                ++cellmlErrorsCount;

                issueType = CellmlFileIssue::Error;
            }

            // Append the issue to our list

            mIssues << CellmlFileIssue(issueType,
                                       QString::fromStdWString(cellmlValidityIssue->description()),
                                       line, column, importedFile);
        }

        if (cellmlErrorsCount)
            // There are CellML errors, so...

            mValid = false;
        else
            // Everything went as expected, so...

            mValid = true;

        mValidNeeded = false;

        return mValid;
    } else {
        // Something went wrong with the loading of the file, so...

        return false;
    }
}

//==============================================================================

bool CellmlFile::isModified() const
{
    // Return whether the file has been modified

    return Core::FileManager::instance()->isModified(mFileName);
}

//==============================================================================

void CellmlFile::setModified(const bool &pModified) const
{
    // Set the modified status of the file

    Core::FileManager::instance()->setModified(mFileName, pModified);
}

//==============================================================================

CellmlFileIssues CellmlFile::issues() const
{
    // Return the file's issue(s)

    return mIssues;
}

//==============================================================================

CellmlFileRuntime * CellmlFile::runtime()
{
    if (!mRuntimeUpdateNeeded)
        // There is no need for the runtime to be updated, so...

        return mRuntime;

    // Load (but not reload!) the file, if needed

    if (load()) {
        // The file is loaded, so return an updated version of its runtime

        mRuntime->update(this);

        mRuntimeUpdateNeeded = !mRuntime->isValid();

        return mRuntime;
    } else {
        // The file coudln't be loaded, so...

        return 0;
    }
}

//==============================================================================

QString CellmlFile::fileName() const
{
    // Return the CellML file's file name

    return mFileName;
}

//==============================================================================

CellmlFileRdfTriples & CellmlFile::rdfTriples()
{
    // Return all the RDF triples associated with the CellML file

    return mRdfTriples;
}

//==============================================================================

CellmlFileRdfTriples CellmlFile::rdfTriples(iface::cellml_api::CellMLElement *pElement) const
{
    // Return all the RDF triples associated with the given element

    return mRdfTriples.contains(pElement);
}

//==============================================================================

bool CellmlFile::rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                                 const QString &pQualifier,
                                 const QString &pResource,
                                 const QString &pId) const
{
    // Return whether the given RDF triple is associated with the CellML element

    if (QString::fromStdWString(pElement->cmetaId()).isEmpty())
        // The CellML element doesn't have a 'proper' cmeta:id, so...

        return false;

    // Go through the RDF triples associated with the CellML element and check
    // whether one of them corresponds to the given RDF triple

    foreach (CellmlFileRdfTriple *rdfTriple, rdfTriples(pElement))
        if (   !pQualifier.compare(rdfTriple->qualifierAsString())
            && !pResource.compare(rdfTriple->resource())
            && !pId.compare(rdfTriple->id()))
            // This is the RDF triple we are after, so...

            return true;

    // We couldn't find the RDF triple, so...

    return false;
}

//==============================================================================

bool CellmlFile::rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                                 const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                 const QString &pResource, const QString &pId) const
{
    // Call our generic rdfTripleExists() function

    return rdfTripleExists(pElement,
                           CellMLSupport::CellmlFileRdfTriple::modelQualifierAsString(pModelQualifier),
                           pResource, pId);
}

//==============================================================================

bool CellmlFile::rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                                 const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                 const QString &pResource, const QString &pId) const
{
    // Call our generic rdfTripleExists() function

    return rdfTripleExists(pElement,
                           CellMLSupport::CellmlFileRdfTriple::bioQualifierAsString(pBioQualifier),
                           pResource, pId);
}

//==============================================================================

QString CellmlFile::rdfTripleSubject(iface::cellml_api::CellMLElement *pElement) const
{
    // Make sure that we have a 'proper' cmeta:id or generate one, if needed

    QString cmetaId = QString::fromStdWString(pElement->cmetaId());

    if (cmetaId.isEmpty()) {
        // We don't have a 'proper' cmeta:id for the element, so we need to
        // generate one and in order to do so, we need to know what cmeta:ids
        // are currently in use in the CellML file

        QStringList cmetaIds = QStringList();

        foreach (CellmlFileRdfTriple *rdfTriple, mRdfTriples) {
            QString cmetaId = rdfTriple->metadataId();

            if (!cmetaIds.contains(cmetaId))
                cmetaIds << cmetaId;
        }

        // Now, we try different cmeta:id values until we find one which is not
        // present in our list

        int counter = 0;

        forever {
            cmetaId = QString("id_%1").arg(++counter, 5, 10, QChar('0'));

            if (!cmetaIds.contains(cmetaId)) {
                // We have found a unique cmeta:id, so update our CellML element
                // and leave

                pElement->cmetaId(cmetaId.toStdWString());

                setModified(true);

                break;
            }
        }
    }

    // Return the subject which should be used for an RDF triple

    return uriBase()+"#"+cmetaId;
}

//==============================================================================

CellMLSupport::CellmlFileRdfTriple * CellmlFile::addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                                              const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                                              const QString &pResource,
                                                              const QString &pId)
{
    // Add an RDF triple to our CellML file

    return mRdfTriples.add(new CellMLSupport::CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                                  pModelQualifier, pResource, pId));
}

//==============================================================================

CellMLSupport::CellmlFileRdfTriple * CellmlFile::addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                                              const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                                              const QString &pResource,
                                                              const QString &pId)
{
    // Add an RDF Triple to our CellML file

    return mRdfTriples.add(new CellMLSupport::CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                                  pBioQualifier, pResource, pId));
}

//==============================================================================

QString CellmlFile::uriBase() const
{
    // Return the CellML file's URI base

    return mUriBase;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
