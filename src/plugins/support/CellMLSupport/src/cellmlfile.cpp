/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML file
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfilecellml10exporter.h"
#include "cellmlfilecellml11exporter.h"
#include "cellmlfilemanager.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QDomDocument>
#include <QFile>
#include <QStringList>
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

#include "cellmlapibegin.h"
    #include "IfaceCeLEDSExporter.hxx"
    #include "IfaceVACSS.hxx"

    #include "CeLEDSExporterBootstrap.hpp"
    #include "CellMLBootstrap.hpp"
    #include "VACSSBootstrap.hpp"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFile::CellmlFile(const QString &pFileName) :
    StandardSupport::StandardFile(pFileName),
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
    // Reset ourselves

    reset();

    // Delete some internal objects

    delete mRuntime;
}

//==============================================================================

void CellmlFile::reset()
{
    // Reset all of our properties

    mModel = 0;

    mRdfApiRepresentation = 0;
    mRdfDataSource = 0;

    Core::resetList(mRdfTriples);

    mIssues.clear();

    Core::FileManager::instance()->setDependencies(mFileName, QStringList());

    mLoadingNeeded = true;
    mFullInstantiationNeeded = true;
    mRuntimeUpdateNeeded = true;
    mDependenciesNeeded = true;

    mImportContents.clear();

    mUsedCmetaIds.clear();
}

//==============================================================================

iface::cellml_api::Model * CellmlFile::model()
{
    // Return the model associated with our CellML file, after loading ourselves
    // if necessary

    load();

    return mModel;
}

//==============================================================================

iface::rdf_api::DataSource * CellmlFile::rdfDataSource()
{
    // Return the data source associated with our CellML file, after loading
    // ourselves if necessary

    load();

    return mRdfDataSource;
}

//==============================================================================

void CellmlFile::retrieveImports(const QString &pXmlBase,
                                 iface::cellml_api::Model *pModel,
                                 QList<iface::cellml_api::CellMLImport *> &pImportList,
                                 QStringList &pImportXmlBaseList)
{
    // Retrieve all the imports of the given model

    ObjRef<iface::cellml_api::CellMLImportSet> imports = pModel->imports();
    ObjRef<iface::cellml_api::CellMLImportIterator> importsIter = imports->iterateImports();

    for (ObjRef<iface::cellml_api::CellMLImport> import = importsIter->nextImport();
         import; import = importsIter->nextImport()) {
        import->add_ref();

        pImportList << import;
        pImportXmlBaseList << pXmlBase;
    }
}

//==============================================================================

bool CellmlFile::fullyInstantiateImports(iface::cellml_api::Model *pModel,
                                         CellmlFileIssues &pIssues,
                                         const bool &pWithBusyWidget)
{
    // Fully instantiate all the imports, but only if we are not directly
    // dealing with our model or if we are dealing with a non CellML 1.0 model,
    // and then keep track of that fact (so we don't fully instantiate everytime
    // we come here)

    Version cellmlVersion = version(pModel);

    if (   ((pModel != mModel) || mFullInstantiationNeeded)
        && (cellmlVersion != Unknown) && (cellmlVersion != Cellml_1_0)) {
        QStringList dependencies = QStringList();

        try {
            // Note: the below is based on CDA_Model::fullyInstantiateImports().
            //       Indeed, CDA_Model::fullyInstantiateImports() doesn't work
            //       with CellML imports that rely on https (see issue #417), so
            //       rather than calling CDA_CellMLImport::instantiate(), we
            //       call CDA_CellMLImport::instantiateFromText() instead, which
            //       requires loading the imported CellML file. Otherwise, to
            //       speed things up as much as possible, we cache the contents
            //       of the URLs that we load...

            // Retrieve the list of imports, together with their XML base values

            ObjRef<iface::cellml_api::URI> baseUri = pModel->xmlBase();
            QList<iface::cellml_api::CellMLImport *> importList = QList<iface::cellml_api::CellMLImport *>();
            QStringList importXmlBaseList = QStringList();

            retrieveImports(QString::fromStdWString(baseUri->asText()),
                            pModel, importList, importXmlBaseList);

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

                    if (!fileNameOrUrl.compare(mFileName)) {
                        // We want to import ourselves, something we can't do

                        throw(std::exception());
                    } else if (mImportContents.contains(fileNameOrUrl)) {
                        // We have already loaded the import contents, so
                        // directly instantiate the import with it

                        import->instantiateFromText(mImportContents.value(fileNameOrUrl).toStdWString());
                    } else {
                        // We haven't already loaded the import contents, so do
                        // so now

                        QString fileContents;

                        if (   ( isLocalFile &&  Core::readFileContentsFromFile(fileNameOrUrl, fileContents))
                            || (!isLocalFile &&  pWithBusyWidget && Core::readFileContentsFromUrlWithBusyWidget(fileNameOrUrl, fileContents))
                            || (!isLocalFile && !pWithBusyWidget && Core::readFileContentsFromUrl(fileNameOrUrl, fileContents))) {
                            // We were able to retrieve the import contents, so
                            // instantiate the import with it

                            import->instantiateFromText(fileContents.toStdWString());

                            // Keep track of the import contents

                            mImportContents.insert(fileNameOrUrl, fileContents);

                            // Keep track of the import as being one of our
                            // dependencies, should it be local and should we be
                            // directly dealing with our model

                            if (isLocalFile && (pModel == mModel))
                                dependencies << fileNameOrUrl;
                        } else {
                            throw(std::exception());
                        }
                    }

                    // Now that the import is instantiated, add its own imports
                    // to our list

                    ObjRef<iface::cellml_api::Model> importModel = import->importedModel();

                    if (!importModel)
                        throw(std::exception());

                    retrieveImports(isLocalFile?
                                        QUrl::fromLocalFile(fileNameOrUrl).toString():
                                        fileNameOrUrl,
                                    importModel, importList, importXmlBaseList);
                }
            }
        } catch (...) {
            // Something went wrong with the full instantiation of the imports

            pIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the imports could not be fully instantiated"));

            return false;
        }

        // Finalise a few things, should we be directly dealing with our model

        if (pModel == mModel) {
            mFullInstantiationNeeded = false;
            mDependenciesNeeded = false;

            // Set the dependencies for our CellML file

            Core::FileManager::instance()->setDependencies(mFileName, dependencies);
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
    } catch (iface::cellml_api::CellMLException &exception) {
        // Something went wrong with the loading of the model

        if (pFileContents.isEmpty()) {
            pIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the model could not be loaded (%1)").arg(Core::formatMessage(QString::fromStdWString(exception.explanation))));
        } else {
            pIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the model could not be created (%1)").arg(Core::formatMessage(QString::fromStdWString(exception.explanation))));
        }

        return false;
    }

    // Update the base URI, should the CellML file be a remote one or its
    // contents be directly passed onto us

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    ObjRef<iface::cellml_api::URI> baseUri = (*pModel)->xmlBase();

    if (fileManagerInstance->isRemote(pFileName)) {
        // We are dealing with a remote file, so its XML base value should point
        // to its remote location

        baseUri->asText(fileManagerInstance->url(pFileName).toStdWString());
    } else if (!pFileContents.isEmpty()) {
        // We are dealing with a file which contents was directly passed onto
        // us, so its XML base value should point to its actual location

        baseUri->asText(pFileName.toStdWString());
    }

    return true;
}

//==============================================================================

void CellmlFile::retrieveCmetaIdsFromCellmlElement(iface::cellml_api::CellMLElement *pElement)
{
    // Keep track of the given CellML element's cmeta:id

    QString cmetaId = QString::fromStdWString(pElement->cmetaId());

    if (!cmetaId.isEmpty())
        mUsedCmetaIds << cmetaId;

    // Do the same for all the child elements of the given CellML element

    ObjRef<iface::cellml_api::CellMLElementSet> childElements = pElement->childElements();
    ObjRef<iface::cellml_api::CellMLElementIterator> childElementsIter = childElements->iterate();

    try {
        for (ObjRef<iface::cellml_api::CellMLElement> childElement = childElementsIter->next();
             childElement; childElement = childElementsIter->next()) {
            retrieveCmetaIdsFromCellmlElement(childElement);
        }
    } catch (...) {
        // Note: we should never reach this point, but it may still happen if a
        //       CellML file contains a child element that is not known to the
        //       CellML API. We are taking the view that this is a limitation of
        //       the CellML API and shouldn't therefore generate an error for
        //       something that should have been working fine in the first
        //       place...
    }
}

//==============================================================================

void CellmlFile::clearCmetaIdsFromCellmlElement(const QDomElement &pElement,
                                                const QStringList &pUsedCmetaIds)
{
    // Remove the given CellML element's cmeta:id, if it is not actually being
    // used

    static const QString CmetaId = "cmeta:id";

    if (!pUsedCmetaIds.contains(pElement.attribute(CmetaId)))
        pElement.attributes().removeNamedItem(CmetaId);

    // Do the same for all the child elements of the given CellML element

    for (QDomElement childElement = pElement.firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        clearCmetaIdsFromCellmlElement(childElement, pUsedCmetaIds);
    }
}

//==============================================================================

bool CellmlFile::load()
{
    // Check whether we are already loaded and without any issues

    if (!mLoadingNeeded)
        return mIssues.isEmpty();

    mLoadingNeeded = false;

    // Try to load the model

    if (!doLoad(mFileName, QString(), &mModel, mIssues))
        return false;

    // Retrieve all the RDF triples associated with the model and initialise our
    // list of original RDF triples

    ObjRef<iface::cellml_api::RDFRepresentation> rdfRepresentation = mModel->getRDFRepresentation(L"http://www.cellml.org/RDF/API");

    if (rdfRepresentation) {
        mRdfApiRepresentation = QueryInterface(rdfRepresentation);

        if (mRdfApiRepresentation) {
            mRdfDataSource = mRdfApiRepresentation->source();
            ObjRef<iface::rdf_api::TripleSet> rdfTriples = mRdfDataSource->getAllTriples();
            ObjRef<iface::rdf_api::TripleEnumerator> rdfTriplesEnumerator = rdfTriples->enumerateTriples();

            for (ObjRef<iface::rdf_api::Triple> rdfTriple = rdfTriplesEnumerator->getNextTriple();
                 rdfTriple; rdfTriple = rdfTriplesEnumerator->getNextTriple()) {
                mRdfTriples << new CellmlFileRdfTriple(this, rdfTriple);
            }

            mRdfTriples.updateOriginalRdfTriples();
        }
    }

    // Determine which cmeta:ids are currently in use, be they in the various
    // CellML elements or RDF triples

    retrieveCmetaIdsFromCellmlElement(mModel);

    foreach (CellmlFileRdfTriple *rdfTriple, mRdfTriples)
        mUsedCmetaIds << rdfTriple->metadataId();

    mUsedCmetaIds.removeDuplicates();

    return true;
}

//==============================================================================

bool CellmlFile::save(const QString &pFileName)
{
    // Make sure that we are properly loaded and have no issues

    if (mLoadingNeeded || !mIssues.isEmpty())
        return false;

    // Make sure that the RDF API representation is up to date by updating its
    // data source

    mRdfApiRepresentation->source(mRdfDataSource);

    // Update our list of original RDF triples

    mRdfTriples.updateOriginalRdfTriples();

    // Get a DOM representation of our CellML file and remove its XML base
    // value, its RDF child node (should there be no annotations) and all the
    // cmeta:id's (in CellML elements) that are not used in the CellML file's
    // annotations
    // Note #1: as part of good practices, a CellML file should never contain an
    //          XML base value. Yet, upon loading a CellML file, we set one (see
    //          doLoad()), so that we can properly import CellML files, if
    //          needed. So, now, we need to undo what we did...
    // Note #2: normally, we would be asking QDomDocument::setContent() to
    //          process namespaces, but this would then result in a very messy
    //          serialisation with namespaces being referenced all over the
    //          place. So, in the end, we do everything without processing
    //          namespaces...

    QDomDocument domDocument;

    domDocument.setContent(QString::fromStdWString(mModel->serialisedText()));

    QDomElement domElement = domDocument.documentElement();

    domElement.attributes().removeNamedItem("xml:base");

    for (QDomElement childElement = domElement.firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        if (!childElement.nodeName().compare("rdf:RDF")) {
            if (!childElement.childNodes().count())
                domElement.removeChild(childElement);

            break;
        }
    }

    QStringList usedCmetaIds = QStringList();

    foreach (CellmlFileRdfTriple *rdfTriple, mRdfTriples)
        usedCmetaIds << rdfTriple->metadataId();

    usedCmetaIds.removeDuplicates();

    clearCmetaIdsFromCellmlElement(domElement, usedCmetaIds);

    // Determine the file name to use for the CellML file

    QString fileName = pFileName.isEmpty()?mFileName:pFileName;

    // Write out the contents of our DOM document to our CellML file

    if (!Core::writeFileContentsToFile(fileName, Core::serialiseDomDocument(domDocument)))
        return false;

    // Our CellML file being saved, it cannot be modified (should it have been
    // before)
    // Note: we must do this before updating mFileName (should it be given a new
    //       value) since we use it to update our modified status...

    setModified(false);

    // Make sure that mFileName is up to date

    mFileName = fileName;

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

    // Sort our issues (since the CellML API may generate them in a non-ordered
    // manner)

    std::sort(pIssues.begin(), pIssues.end(), CellmlFileIssue::compare);

    return !cellmlErrorsCount;
}

//==============================================================================

bool CellmlFile::isValid(const QString &pFileContents,
                         CellmlFileIssues &pIssues, const bool &pWithBusyWidget)
{
    // Check whether the given file contents is CellML valid, so first create a
    // model

    ObjRef<iface::cellml_api::Model> model;

    if (doLoad(mFileName, pFileContents, &model, pIssues)) {
        // The file contents was properly loaded, so make sure that its imports,
        // if any, are fully instantiated

        if (fullyInstantiateImports(model, pIssues, pWithBusyWidget)) {
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
    // Return whether we have been modified

    return Core::FileManager::instance()->isModified(mFileName);
}

//==============================================================================

void CellmlFile::setModified(const bool &pModified) const
{
    // Set our modified status

    Core::FileManager::instance()->setModified(mFileName, pModified);
}

//==============================================================================

CellmlFileIssues CellmlFile::issues() const
{
    // Return our issues

    return mIssues;
}

//==============================================================================

CellmlFileRuntime * CellmlFile::runtime(const bool &pWithBusyWidget)
{
    // Check whether the runtime needs to be updated

    if (!mRuntimeUpdateNeeded)
        return mRuntime;

    // Load (but not reload!) ourselves, if needed

    if (load()) {
        // Make sure that our imports, if any, are fully instantiated

        if (fullyInstantiateImports(mModel, mIssues, pWithBusyWidget)) {
            // Now, we can return an updated version of our runtime

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

QStringList CellmlFile::dependencies(const bool &pWithBusyWidget)
{
    // Check whether the dependencies need to be retrieved

    if (!mDependenciesNeeded)
        return Core::FileManager::instance()->dependencies(mFileName);

    // Load (but not reload!) ourselves, if needed

    if (load()) {
        // Make sure that our imports, if any, are fully instantiated

        if (fullyInstantiateImports(mModel, mIssues, pWithBusyWidget)) {
            // Now, we can return our dependencies

            return Core::FileManager::instance()->dependencies(mFileName);
        } else {
            return QStringList();
        }
    } else {
        return QStringList();
    }
}

//==============================================================================

CellmlFileRdfTriples & CellmlFile::rdfTriples()
{
    // Return all the RDF triples associated with ourselves

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

    if (QString::fromStdWString(pElement->cmetaId()).isEmpty()) {
        // The given CellML element doesn't have a 'proper' cmeta:id, so it
        // cannot have RDF triples associated with it

        return 0;
    }

    // Go through the RDF triples associated with the given CellML element and
    // check whether it is the one we are after

    foreach (CellmlFileRdfTriple *rdfTriple, rdfTriples(pElement)) {
        if (   !pQualifier.compare(rdfTriple->qualifierAsString())
            && !pResource.compare(rdfTriple->resource())
            && !pId.compare(rdfTriple->id())) {
            // This is the RDF triple we are after

            return rdfTriple;
        }
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

QString CellmlFile::rdfTripleSubject(iface::cellml_api::CellMLElement *pElement)
{
    // Make sure that we have a 'proper' cmeta:id or generate one, if needed

    QString cmetaId = QString::fromStdWString(pElement->cmetaId());

    if (cmetaId.isEmpty()) {
        // We don't have a 'proper' cmeta:id for the CellML element, so we need
        // to generate one and in order to do so, we need to try different
        // cmeta:id values until we find one that is not used

        int counter = 0;

        while (true) {
            cmetaId = QString("id_%1").arg(++counter, 9, 10, QChar('0'));

            if (!mUsedCmetaIds.contains(cmetaId)) {
                // We have found a unique cmeta:id, so update our CellML
                // element, consider ourselves modified, update our list of
                // cmeta:ids and leave

                pElement->cmetaId(cmetaId.toStdWString());

                setModified(true);

                mUsedCmetaIds << cmetaId;

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
    // Add an RDF triple to ourselves

    return mRdfTriples.add(new CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                   pModelQualifier, pResource, pId));
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                               const CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                               const QString &pResource,
                                               const QString &pId)
{
    // Add an RDF Triple to ourselves

    return mRdfTriples.add(new CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                   pBioQualifier, pResource, pId));
}

//==============================================================================

bool CellmlFile::removeRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                 const CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                 const QString &pResource, const QString &pId)
{
    // Remove an RDF triple from ourselves

    return mRdfTriples.remove(rdfTriple(pElement, pModelQualifier, pResource, pId));
}

//==============================================================================

bool CellmlFile::removeRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                 const CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                 const QString &pResource, const QString &pId)
{
    // Remove an RDF triple from ourselves

    return mRdfTriples.remove(rdfTriple(pElement, pBioQualifier, pResource, pId));
}

//==============================================================================

QStringList CellmlFile::importedFileNames() const
{
    // Return our imported file names

    return mImportContents.keys();
}

//==============================================================================

QString CellmlFile::importedFileContents(const QString &pImportedFileName) const
{
    // Return the contents of our given imported file name

    return mImportContents.value(pImportedFileName);
}

//==============================================================================

QString CellmlFile::cmetaId()
{
    // Return the CellML model's cmeta:id

    if (load())
        return QString::fromStdWString(mModel->cmetaId());
    else
        return QString();
}

//==============================================================================

QString CellmlFile::xmlBase()
{
    // Return our base URI

    if (load()) {
        ObjRef<iface::cellml_api::URI> baseUri = mModel->xmlBase();

        return QString::fromStdWString(baseUri->asText());
    } else {
        return QString();
    }
}

//==============================================================================

bool CellmlFile::exportTo(const QString &pFileName, const Version &pVersion,
                          const bool &pWithBusyWidget)
{
    // Export the model to the required format, after loading it if necessary

    if (load()) {
        // Check that it actually makes sense to export the model

        switch (pVersion) {
        case Unknown:
            // We clearly cannot export to an unknown version

            return false;
        case Cellml_1_0:
            // To export to CellML 1.0, the model must be in a non CellML 1.0
            // format

            if (version(mModel) == Cellml_1_0)
                return false;

            break;
        case Cellml_1_1:
            // To export to CellML 1.1, the model must be in a non CellML 1.1
            // format

            if (version(mModel) == Cellml_1_1)
                return false;

            break;
        }

        // Fully instantiate all the imports

        if (!fullyInstantiateImports(mModel, mIssues, pWithBusyWidget))
            return false;

        // Do the actual export

        switch (pVersion) {
        case Unknown:
            // We clearly cannot export to an unknown version

            return false;
        case Cellml_1_0: {
            CellmlFileCellml10Exporter exporter(mModel, pFileName);

            if (!exporter.errorMessage().isEmpty()) {
                mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                           exporter.errorMessage());
            }

            return exporter.result();
        }
        case Cellml_1_1: {
            CellmlFileCellml11Exporter exporter(mModel, pFileName);

            if (!exporter.errorMessage().isEmpty()) {
                mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                           exporter.errorMessage());
            }

            return exporter.result();
        }
        }
    }

    return false;
}

//==============================================================================

bool CellmlFile::exportTo(const QString &pFileName,
                          const QString &pUserDefinedFormatFileName,
                          const bool &pWithBusyWidget)
{
    // Export the model to the required format, after loading it if necessary

    if (load()) {
        // Check that the user-defined format file actually exists

        if (!QFile::exists(pUserDefinedFormatFileName)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the user-defined format file does not exist"));

            return false;
        }

        // Make sure that the user-defined format file is valid XML
        // Note: you would normally expect CeLEDSExporter to check this, but all
        //       it does in case of an invalid XML file is crash...

        QString userDefinedFormatFileContents;

        if (!Core::readFileContentsFromFile(pUserDefinedFormatFileName, userDefinedFormatFileContents)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the user-defined format file could not be read"));

            return false;
        }

        QDomDocument domDocument;

        if (!domDocument.setContent(userDefinedFormatFileContents)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the user-defined format file is not a valid XML file"));

            return false;
        }

        // Fully instantiate all the imports

        if (!fullyInstantiateImports(mModel, mIssues, pWithBusyWidget))
            return false;

        // Do the actual export

        ObjRef<iface::cellml_services::CeLEDSExporterBootstrap> celedsExporterBootstrap = CreateCeLEDSExporterBootstrap();
        ObjRef<iface::cellml_services::CodeExporter> codeExporter = celedsExporterBootstrap->createExporterFromText(QString(userDefinedFormatFileContents).toStdWString());

        if (celedsExporterBootstrap->loadError().length()) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the user-defined format file could not be loaded"));

            return false;
        }

        // Save the export or output it to the console, if no file name has been
        // provided

        if (pFileName.isEmpty()) {
            std::wcout << QString::fromStdWString(codeExporter->generateCode(mModel)).trimmed().toStdWString() << std::endl;
        } else if (!Core::writeFileContentsToFile(pFileName, QString::fromStdWString(codeExporter->generateCode(mModel)))) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the output file could not be saved"));

            return false;
        }

        return true;
    } else {
        return false;
    }
}

//==============================================================================

CellmlFile::Version CellmlFile::version()
{
    // Return our version

    if (load())
        return CellmlFile::version(mModel);
    else
        return CellmlFile::Unknown;
}

//==============================================================================

CellmlFile::Version CellmlFile::version(iface::cellml_api::Model *pModel)
{
    // Return the version of the given CellML model, if any

    if (!pModel)
        return Unknown;

    QString cellmlVersion = QString::fromStdWString(pModel->cellmlVersion());

    if (!cellmlVersion.compare(CellMLSupport::Cellml_1_0)) {
        return Cellml_1_0;
    } else if (!cellmlVersion.compare(CellMLSupport::Cellml_1_1)) {
        return Cellml_1_1;
    } else {
        qWarning("WARNING | %s:%d: a CellML version should not be unknown.", __FILE__, __LINE__);

        return Unknown;
    }
}

//==============================================================================

CellmlFile::Version CellmlFile::version(const QString &pFileName)
{
    // Return the version of the given CellML file

    CellmlFile *cellmlFile = CellmlFileManager::instance()->cellmlFile(pFileName);

    if (cellmlFile)
        return cellmlFile->version();
    else
        return Unknown;
}

//==============================================================================

QString CellmlFile::versionAsString(const Version &pVersion)
{
    // Return the string corresponding to the given version

    switch (pVersion) {
    case Unknown:
        return "???";
    case Cellml_1_0:
        return "CellML 1.0";
    case Cellml_1_1:
        return "CellML 1.1";
    }

    return "???";
    // Note: we can't reach this point, but without it we may be told that not
    //       all control paths return a value...
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
