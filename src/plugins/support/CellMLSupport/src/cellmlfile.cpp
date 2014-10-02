/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML file class
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfilecellml10exporter.h"
#include "cellmlfilecellml11exporter.h"
#include "cliutils.h"
#include "filemanager.h"

//==============================================================================

#include <QDomDocument>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QTextStream>
#include <QUrl>

//==============================================================================

#ifdef Q_OS_LINUX
    #include <stdint.h>
    // Note: the above header file is required on Linux, so we can use uint32_t
    //       (required to 'properly' make use of the CellML API). Now, we would
    //       normally use
    //           #include <cstdint>
    //       but this is not supported by the current ISO C++ standard. Indeed,
    //       to include it will generate the following error at compile time:
    //           error: #error This file requires compiler and library support
    //           for the upcoming ISO C++ standard, C++0x. This support is
    //           currently experimental, and must be enabled with the -std=c++0x
    //           or -std=gnu++0x compiler options.
    //       and well, we don't want to enable either of these options since
    //       support is still only experimental...
#endif

//==============================================================================

#include "IfaceCeLEDSExporter.hxx"
#include "IfaceVACSS.hxx"

#include "CeLEDSExporterBootstrap.hpp"
#include "CellMLBootstrap.hpp"
#include "VACSSBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFile::CellmlFile(const QString &pFileName) :
    mFileName(Core::nativeCanonicalFileName(pFileName)),
    mModel(0),
    mRdfApiRepresentation(0),
    mRdfDataSource(0),
    mRdfTriples(CellmlFileRdfTriples(this))
{
    // Instantiate our runtime object

    mRuntime = new CellmlFileRuntime(this);

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

    foreach (CellmlFileRdfTriple *rdfTriple, mRdfTriples)
        delete rdfTriple;

    mRdfTriples.clear();

    mValid = true;

    mIssues.clear();

    mLoadingNeeded = true;
    mValidNeeded = true;
    mRuntimeUpdateNeeded = true;

    mImportContents.clear();
}

//==============================================================================

iface::cellml_api::Model * CellmlFile::model()
{
    // Return the model associated with our CellML file, after loading it if
    // necessary

    load();

    return mModel;
}

//==============================================================================

iface::rdf_api::DataSource * CellmlFile::rdfDataSource()
{
    // Return the data source associated with our CellML file, after loading it
    // if necessary

    load();

    return mRdfDataSource;
}

//==============================================================================

void CellmlFile::retrieveImports(iface::cellml_api::Model *pModel,
                                 QList<iface::cellml_api::CellMLImport *> &pImportList,
                                 QStringList &pImportXmlBaseList,
                                 const QString &pXmlBase)
{
    // Retrieve all the imports of the given model

    ObjRef<iface::cellml_api::CellMLImportSet> imports = pModel->imports();
    ObjRef<iface::cellml_api::CellMLImportIterator> importsIterator = imports->iterateImports();

    for (ObjRef<iface::cellml_api::CellMLImport> import = importsIterator->nextImport();
         import; import = importsIterator->nextImport()) {
        import->add_ref();

        pImportList << import;
        pImportXmlBaseList << pXmlBase;
    }
}

//==============================================================================

bool CellmlFile::fullyInstantiateImports(iface::cellml_api::Model *pModel,
                                         CellmlFileIssues &pIssues)
{
    // Fully instantiate all the imports, but only if we are dealing with a non
    // CellML 1.0 model

    if (QString::fromStdWString(pModel->cellmlVersion()).compare(CellMLSupport::Cellml_1_0)) {
        try {
            // Note: the below is based on CDA_Model::fullyInstantiateImports().
            //       Indeed, CDA_Model::fullyInstantiateImports() doesn't work
            //       with CellML imports that rely on https (see
            //       https://github.com/opencor/opencor/issues/417), so rather
            //       than calling CDA_CellMLImport::instantiate(), we call
            //       CDA_CellMLImport::instantiateFromText() instead, which
            //       requires loading the imported CellML file. Otherwise, to
            //       speed things up as much as possible, we cache the contents
            //       of the URLs that we load...

            // Retrieve the list of imports, together with their XML base values

            ObjRef<iface::cellml_api::URI> uri = pModel->xmlBase();
            QList<iface::cellml_api::CellMLImport *> importList = QList<iface::cellml_api::CellMLImport *>();
            QStringList importXmlBaseList = QStringList();

            retrieveImports(pModel, importList, importXmlBaseList,
                            QString::fromStdWString(uri->asText()));

            // Instantiate all the imports in our list

            while (!importList.isEmpty()) {
                // Retrieve the first import and instantiate it, if needed

                ObjRef<iface::cellml_api::CellMLImport> import = importList.first();
                QString importXmlBase = importXmlBaseList.first();

                importList.removeFirst();
                importXmlBaseList.removeFirst();

                if (!import->wasInstantiated()) {
                    // Note: CDA_CellMLImport::instantiate() would normally be
                    //       called, but it doesn't work with https, so we
                    //       retrieve the contents of the import ourselves and
                    //       instantiate it from text instead...

                    ObjRef<iface::cellml_api::URI> xlinkHref = import->xlinkHref();
                    QString url = QUrl(importXmlBase).resolved(QString::fromStdWString(xlinkHref->asText())).toString();
                    bool isLocalFile;
                    QString fileNameOrUrl;

                    Core::checkFileNameOrUrl(url, isLocalFile, fileNameOrUrl);

                    if (mImportContents.contains(fileNameOrUrl)) {
                        // We have already loaded the import contents, so
                        // directly instantiate the import with it

                        import->instantiateFromText(mImportContents.value(fileNameOrUrl).toStdWString());
                    } else {
                        // We haven't already loaded the import contents, so do
                        // so

                        QString fileContents;

                        if (   ( isLocalFile && Core::readTextFromFile(fileNameOrUrl, fileContents))
                            || (!isLocalFile && Core::readTextFromUrl(fileNameOrUrl, fileContents))) {
                            // We were able to retrieve the import contents, so
                            // instantiate the import with it

                            import->instantiateFromText(fileContents.toStdWString());

                            // Keep track of the import contents

                            mImportContents.insert(fileNameOrUrl, fileContents);
                        } else {
                            throw(std::exception());
                        }
                    }

                    // Now that the import is instantiated, add its own imports
                    // to our list

                    ObjRef<iface::cellml_api::Model> importModel = import->importedModel();

                    if (!importModel)
                        throw(std::exception());

                    retrieveImports(importModel, importList, importXmlBaseList,
                                    isLocalFile?
                                        QUrl::fromLocalFile(fileNameOrUrl).toString():
                                        fileNameOrUrl);
                }
            }
        } catch (...) {
            // Something went wrong with the full instantiation of the imports

            pIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the imports could not be fully instantiated"));

            return false;
        }
    }

    return true;
}

//==============================================================================

bool CellmlFile::doLoad(const QString &pFileName, const QString &pFileContents,
                        ObjRef<iface::cellml_api::Model> *pModel,
                        CellmlFileIssues &pIssues)
{
    // Make sure that pIssues is empty

    pIssues.clear();

    // Get a bootstrap object and its model loader

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();
    ObjRef<iface::cellml_api::DOMModelLoader> modelLoader = cellmlBootstrap->modelLoader();

    // Try to create the model

    try {
        if (pFileContents.isEmpty())
            *pModel = modelLoader->loadFromURL(QUrl::fromPercentEncoding(QUrl::fromLocalFile(pFileName).toEncoded()).toStdWString());
        else
            *pModel = modelLoader->createFromText(pFileContents.toStdWString());
    } catch (iface::cellml_api::CellMLException &) {
        // Something went wrong with the loading of the model

        if (pFileContents.isEmpty())
            pIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the model could not be loaded (%1)").arg(QString::fromStdWString(modelLoader->lastErrorMessage())));
        else
            pIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the model could not be created (%1)").arg(QString::fromStdWString(modelLoader->lastErrorMessage())));

        return false;
    }

    // Update the XML base value, should the CellML file be a remote one or its
    // contents be directly passed onto us

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    ObjRef<iface::cellml_api::URI> uri = (*pModel)->xmlBase();

    if (fileManagerInstance->isRemote(pFileName))
        // We are dealing with a remote file, so its XML base value should point
        // to its remote location

        uri->asText(fileManagerInstance->url(pFileName).toStdWString());
    else if (!pFileContents.isEmpty())
        // We are dealing with a file which contents was directly passed onto
        // us, so its XML base value should point to its actual location

        uri->asText(pFileName.toStdWString());

    return true;
}

//==============================================================================

bool CellmlFile::load()
{
    // Check whether the file is already loaded

    if (!mLoadingNeeded)
        return true;

    // Consider the file loaded
    // Note: even when we can't load the file, we still consider it 'loaded'
    //       since we at least tried to load it, so unless the file gets
    //       modified (and we are to reload it), we are 'fine'...

    mLoadingNeeded = false;

    // Try to load the model

    if (!doLoad(mFileName, QString(), &mModel, mIssues))
        return false;

    // Retrieve all the RDF triples associated with the model

    ObjRef<iface::cellml_api::RDFRepresentation> rdfRepresentation = mModel->getRDFRepresentation(L"http://www.cellml.org/RDF/API");

    if (rdfRepresentation) {
        mRdfApiRepresentation = QueryInterface(rdfRepresentation);

        if (mRdfApiRepresentation) {
            mRdfDataSource = mRdfApiRepresentation->source();
            ObjRef<iface::rdf_api::TripleSet> rdfTriples = mRdfDataSource->getAllTriples();
            ObjRef<iface::rdf_api::TripleEnumerator> rdfTriplesEnumerator = rdfTriples->enumerateTriples();

            for (ObjRef<iface::rdf_api::Triple> rdfTriple = rdfTriplesEnumerator->getNextTriple();
                 rdfTriple; rdfTriple = rdfTriplesEnumerator->getNextTriple())
                mRdfTriples << new CellmlFileRdfTriple(this, rdfTriple);
        }
    }

    return true;
}

//==============================================================================

bool CellmlFile::reload()
{
    // We want to reload the file, so we must first reset everything

    reset();

    // Now, we can try to (re)load the file

    return load();
}

//==============================================================================

bool CellmlFile::save(const QString &pNewFileName)
{
    // Check whether the file needs loading or contains issues

    if (mLoadingNeeded || !mIssues.isEmpty())
        return false;

    // Determine the file name to use for the CellML file

    QString newFileName = pNewFileName.isEmpty()?mFileName:pNewFileName;

    // Make sure that the RDF API representation is up to date by updating its
    // data source

    mRdfApiRepresentation->source(mRdfDataSource);

    // (Create and) open the file for writing

    QFile file(newFileName);

    if (!file.open(QIODevice::WriteOnly)) {
        // The file can't be opened, so delete it

        file.remove();

        return false;
    }

    // Write out the contents of the CellML file to the file

    QTextStream out(&file);

    out << QString::fromStdWString(mModel->serialisedText());

    file.close();

    // The CellML file being saved, it cannot be modified (should it have been
    // before)
    // Note: we must do this before updating mFileName (should it be given a new
    //       value) since we use it to update our modified status...

    setModified(false);

    // Make sure that mFileName is up to date

    mFileName = newFileName;

    // Everything went fine

    return true;
}

//==============================================================================

bool CellmlFile::doIsValid(iface::cellml_api::Model *pModel,
                           CellmlFileIssues &pIssues)
{
    // Check whether the given model is CellML valid
    // Note: validateModel() is somewhat slow, but there is (unfortunately)
    //       nothing we can do about it. Then, there is getPositionInXML() which
    //       is painfully slow, but unlike for validateModel() its use is not
    //       essential (even though it would be nice from an end-user's
    //       perspective). So, rather than retrieve the line/column of every
    //       single warning/error, we only keep track of the various
    //       warnings/errors and only retrieve their corresponding line/column
    //       when requested (definitely not neat from an end-user's perspective,
    //       but we just can't afford the time it takes to fully validate a
    //       model that has many warnings/errors)...

    // Make sure that pIssues is empty

    pIssues.clear();

    // Determine the number of errors and warnings
    // Note: CellMLValidityErrorSet::nValidityErrors() returns any type of
    //       validation issue, be it an error or a warning, so we need to
    //       determine the number of true errors

    ObjRef<iface::cellml_services::VACSService> vacssService = CreateVACSService();
    ObjRef<iface::cellml_services::CellMLValidityErrorSet> cellmlValidityErrorSet = vacssService->validateModel(pModel);

    int cellmlErrorsCount = 0;

    for (int i = 0, iMax = cellmlValidityErrorSet->nValidityErrors(); i < iMax; ++i) {
        ObjRef<iface::cellml_services::CellMLValidityError> cellmlValidityIssue = cellmlValidityErrorSet->getValidityError(i);
        ObjRef<iface::cellml_services::CellMLRepresentationValidityError> cellmlRepresentationValidityError = QueryInterface(cellmlValidityIssue);

        // Determine the issue's location

        uint32_t line = 0;
        uint32_t column = 0;
        QString importedFile = QString();

        if (cellmlRepresentationValidityError) {
            // We are dealing with a CellML representation issue, so determine
            // its line and column

            ObjRef<iface::dom::Node> errorNode = cellmlRepresentationValidityError->errorNode();

            line = vacssService->getPositionInXML(errorNode,
                                                  cellmlRepresentationValidityError->errorNodalOffset(),
                                                  &column);
        } else {
            // We are not dealing with a CellML representation issue, so check
            // whether we are dealing with a semantic one

            ObjRef<iface::cellml_services::CellMLSemanticValidityError> cellmlSemanticValidityError = QueryInterface(cellmlValidityIssue);

            if (cellmlSemanticValidityError) {
                // We are dealing with a CellML semantic issue, so determine its
                // line and column

                ObjRef<iface::cellml_api::CellMLElement> cellmlElement = cellmlSemanticValidityError->errorElement();
                ObjRef<iface::cellml_api::CellMLDOMElement> cellmlDomElement = QueryInterface(cellmlElement);

                ObjRef<iface::dom::Element> domElement = cellmlDomElement->domElement();

                line = vacssService->getPositionInXML(domElement, 0, &column);

                // Also determine its imported file, if any

                ObjRef<iface::cellml_api::CellMLElement> cellmlElementParent = cellmlElement->parentElement();

                if (cellmlElementParent) {
                    // Check whether the parent is an imported file

                    ObjRef<iface::cellml_api::Model> importedCellmlFile = QueryInterface(cellmlElementParent);

                    if (importedCellmlFile) {
                        // Retrieve the imported CellML element

                        ObjRef<iface::cellml_api::CellMLElement> importedCellmlElement = importedCellmlFile->parentElement();

                        if (importedCellmlElement) {
                            // Check whether the imported CellML element is an
                            // import CellML element

                            ObjRef<iface::cellml_api::CellMLImport> importCellmlElement = QueryInterface(importedCellmlElement);

                            if (importCellmlElement) {
                                ObjRef<iface::cellml_api::URI> xlinkHref = importCellmlElement->xlinkHref();

                                importedFile = QString::fromStdWString(xlinkHref->asText());
                            }
                        }
                    }
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

        pIssues << CellmlFileIssue(issueType, line, column,
                                   QString::fromStdWString(cellmlValidityIssue->description()),
                                   importedFile);
    }

    // Sort our issues

    std::sort(pIssues.begin(), pIssues.end());

    return !cellmlErrorsCount;
}

//==============================================================================

bool CellmlFile::isValid()
{
    // Check whether the file has already been validated

    if (!mValidNeeded)
        return mValid;

    // Load (but not reload!) the file, if needed

    if (load()) {
        // The file was properly loaded (or was already loaded), so make sure
        // that its imports, if any, are fully instantiated

        if (fullyInstantiateImports(mModel, mIssues)) {
            // Now, we can check whether the file is CellML valid

            mValid = doIsValid(mModel, mIssues);

            mValidNeeded = false;

            return mValid;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

//==============================================================================

bool CellmlFile::isValid(const QString &pFileName, const QString &pFileContents,
                         CellmlFileIssues &pIssues)
{
    // Check whether the given file contents is CellML valid, so first create a
    // model

    ObjRef<iface::cellml_api::Model> model;

    if (doLoad(pFileName, pFileContents, &model, pIssues)) {
        // The file contents was properly loaded, so make sure that its imports,
        // if any, are fully instantiated

        if (fullyInstantiateImports(model, pIssues)) {
            // Now, we can check whether the file contents is CellML valid

            return doIsValid(model, pIssues);
        } else {
            return false;
        }
    } else {
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
    // Check whether the runtime needs to be updated

    if (!mRuntimeUpdateNeeded)
        return mRuntime;

    // Load (but not reload!) the file, if needed

    if (load()) {
        // The file was properly loaded (or was already loaded), so make sure
        // that its imports, if any, are fully instantiated

        if (fullyInstantiateImports(mModel, mIssues)) {
            // Now, we can return an updated version of its runtime

            mRuntime->update();

            mRuntimeUpdateNeeded = false;

            return mRuntime;
        } else {
            return 0;
        }
    } else {
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

void CellmlFile::setFileName(const QString &pFileName)
{
    // Set the CellML file's file name

    mFileName = pFileName;
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

    return mRdfTriples.associatedWith(pElement);
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::rdfTriple(iface::cellml_api::CellMLElement *pElement,
                                            const QString &pQualifier,
                                            const QString &pResource,
                                            const QString &pId) const
{
    // Return the RDF triple, associated with the given CellML element, which
    // qualifier, resource and id match those given

    if (QString::fromStdWString(pElement->cmetaId()).isEmpty())
        // The given CellML element doesn't have a 'proper' cmeta:id, so it
        // cannot have RDF triples associated with it

        return 0;

    // Go through the RDF triples associated with the given CellML element and
    // check whether it is the one we are after

    foreach (CellmlFileRdfTriple *rdfTriple, rdfTriples(pElement))
        if (   !pQualifier.compare(rdfTriple->qualifierAsString())
            && !pResource.compare(rdfTriple->resource())
            && !pId.compare(rdfTriple->id())) {
            // This is the RDF triple we are after

            return rdfTriple;
        }

    // We couldn't find the RDF triple

    return 0;
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::rdfTriple(iface::cellml_api::CellMLElement *pElement,
                                            const CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                            const QString &pResource,
                                            const QString &pId) const
{
    // Call our generic rdfTriple() function

    return rdfTriple(pElement,
                     CellmlFileRdfTriple::modelQualifierAsString(pModelQualifier),
                     pResource, pId);
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::rdfTriple(iface::cellml_api::CellMLElement *pElement,
                                            const CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                            const QString &pResource,
                                            const QString &pId) const
{
    // Call our generic rdfTriple() function

    return rdfTriple(pElement,
                     CellmlFileRdfTriple::bioQualifierAsString(pBioQualifier),
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

        while (true) {
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

    return xmlBase()+"#"+cmetaId;
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                               const CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                               const QString &pResource,
                                               const QString &pId)
{
    // Add an RDF triple to our CellML file

    return mRdfTriples.add(new CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                   pModelQualifier, pResource, pId));
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                               const CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                               const QString &pResource,
                                               const QString &pId)
{
    // Add an RDF Triple to our CellML file

    return mRdfTriples.add(new CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                   pBioQualifier, pResource, pId));
}

//==============================================================================

bool CellmlFile::removeRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                 const CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                 const QString &pResource, const QString &pId)
{
    // Remove an RDF triple from our CellML file

    return mRdfTriples.remove(rdfTriple(pElement, pModelQualifier, pResource, pId));
}

//==============================================================================

bool CellmlFile::removeRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                 const CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                 const QString &pResource, const QString &pId)
{
    // Remove an RDF triple from our CellML file

    return mRdfTriples.remove(rdfTriple(pElement, pBioQualifier, pResource, pId));
}

//==============================================================================

QString CellmlFile::xmlBase() const
{
    // Return the CellML file's URI base

    ObjRef<iface::cellml_api::URI> uri = mModel->xmlBase();

    return QString::fromStdWString(uri->asText());
}

//==============================================================================

bool CellmlFile::exportTo(const QString &pFileName, const Version &pVersion)
{
    // Export the model to the required format, after loading it if necessary

    if (load()) {
        // Reset any issues that we may have found before

        mIssues.clear();

        // Check that it actually makes sense to export the model

        switch (pVersion) {
        case Cellml_1_1: {
            // To export to CellML 1.1, the model must be in a non CellML 1.1
            // format

            if (!QString::fromStdWString(mModel->cellmlVersion()).compare(CellMLSupport::Cellml_1_1))
                return false;

            break;
        }
        default:   // Cellml_1_0
            // To export to CellML 1.0, the model must be in a non CellML 1.0
            // format

            if (!QString::fromStdWString(mModel->cellmlVersion()).compare(CellMLSupport::Cellml_1_0))
                return false;
        }

        // Fully instantiate all the imports

        if (!fullyInstantiateImports(mModel, mIssues))
            return false;

        // Do the actual export

        switch (pVersion) {
        case Cellml_1_1: {
            CellmlFileCellml11Exporter exporter(mModel, pFileName);

            if (exporter.errorMessage().size())
                mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                           exporter.errorMessage());

            return exporter.result();
        }
        default:   // Cellml_1_0
            CellmlFileCellml10Exporter exporter(mModel, pFileName);

            if (exporter.errorMessage().size())
                mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                           exporter.errorMessage());

            return exporter.result();
        }
    } else {
        return false;
    }
}

//==============================================================================

bool CellmlFile::exportTo(const QString &pFileName,
                          const QString &pUserDefinedFormatFileName)
{
    // Export the model to the required format, after loading it if necessary

    if (load()) {
        // Reset any issues that we may have found before

        mIssues.clear();

        // Check that the user-defined format file actually exists

        if (!QFile::exists(pUserDefinedFormatFileName)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the user-defined format file does not exist"));

            return false;
        }

        // Make sure that the user-defined format file is valid XML
        // Note: you would normally expect CeLEDSExporter to check this, but all
        //       it does in case of an invalid XML file is crash...

        QString userDefinedFormatFileContents;

        if (!Core::readTextFromFile(pUserDefinedFormatFileName, userDefinedFormatFileContents)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the user-defined format file could not be read"));

            return false;
        }

        QDomDocument domDocument;

        if (!domDocument.setContent(userDefinedFormatFileContents)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the user-defined format file is not a valid XML file"));

            return false;
        }

        // Fully instantiate all the imports

        if (!fullyInstantiateImports(mModel, mIssues))
            return false;

        // Do the actual export

        ObjRef<iface::cellml_services::CeLEDSExporterBootstrap> celedsExporterBootstrap = CreateCeLEDSExporterBootstrap();
        ObjRef<iface::cellml_services::CodeExporter> codeExporter = celedsExporterBootstrap->createExporterFromText(userDefinedFormatFileContents.toStdWString());

        if (celedsExporterBootstrap->loadError().length()) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the user-defined format file could not be loaded"));

            return false;
        }

        Core::writeTextToFile(pFileName, QString::fromStdWString(codeExporter->generateCode(mModel)));

        return true;
    } else {
        return false;
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
