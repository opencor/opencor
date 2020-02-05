/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CellML file
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfilecellml10exporter.h"
#include "cellmlfilemanager.h"
#include "centralwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QStringList>
#include <QUrl>

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
    mRdfTriples(CellmlFileRdfTriples(this))
{
    // Reset ourselves

    reset();
}

//==============================================================================

CellmlFile::~CellmlFile()
{
    // Reset ourselves

    try {
        reset();
    } catch (...) {
    }
}

//==============================================================================

void CellmlFile::reset()
{
    // Don't reset ourselves if we were updated

    if (mUpdated) {
        mUpdated = false;

        return;
    }

    // Reset all of our properties

    mModel = nullptr;

    mRdfApiRepresentation = nullptr;
    mRdfDataSource = nullptr;

    for (auto rdfTriple : mRdfTriples) {
        delete rdfTriple;
    }

    mRdfTriples.clear();
    mIssues.clear();

    Core::FileManager::instance()->setDependencies(mFileName, {});

    mLoadingNeeded = true;
    mFullInstantiationNeeded = true;
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

    ObjRef<iface::cellml_api::CellMLImportIterator> importsIter = pModel->imports()->iterateImports();

    for (ObjRef<iface::cellml_api::CellMLImport> import = importsIter->nextImport();
         import != nullptr; import = importsIter->nextImport()) {
        import->add_ref();

        pImportList << import;
        pImportXmlBaseList << pXmlBase;
    }
}

//==============================================================================

bool CellmlFile::fullyInstantiateImports(iface::cellml_api::Model *pModel,
                                         CellmlFileIssues &pIssues)
{
    // Fully instantiate all the imports, but only if we are not directly
    // dealing with our model or if we are dealing with a non CellML 1.0 model,
    // and then keep track of that fact (so we don't fully instantiate everytime
    // we come here)

    Version cellmlVersion = modelVersion(pModel);

    if (   ((pModel != mModel) || mFullInstantiationNeeded)
        && (cellmlVersion != Version::Unknown)
        && (cellmlVersion != Version::Cellml_1_0)) {
        QStringList dependencies;

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
            QList<iface::cellml_api::CellMLImport *> importList;
            QStringList importXmlBaseList;

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

                    QString xlinkHrefString = QString::fromStdWString(import->xlinkHref()->asText());
                    QString url = QUrl(importXmlBase).resolved(xlinkHrefString).toString();
                    bool isLocalFile;
                    QString fileNameOrUrl;
                    bool dummy;
                    QString xmlBaseFileNameOrUrl;

                    Core::checkFileNameOrUrl(url, isLocalFile, fileNameOrUrl);
                    Core::checkFileNameOrUrl(importXmlBase, dummy, xmlBaseFileNameOrUrl);

                    if (fileNameOrUrl == mFileName) {
                        // We want to import ourselves, something we can't do

                        throw std::runtime_error(tr("%1 cannot import itself").arg(fileNameOrUrl).toStdString());
                    }

                    if (mImportContents.contains(fileNameOrUrl)) {
                        // We have already loaded the import contents, so
                        // directly instantiate the import with it

                        import->instantiateFromText(mImportContents.value(fileNameOrUrl).toStdWString());
                    } else {
                        // We haven't already loaded the import contents, so do
                        // so now, with a busy widget if requested and if we are
                        // not dealing with a local file

                        QString fileContents;

                        if (!isLocalFile) {
                            Core::showCentralBusyWidget();
                        }

                        bool res = Core::readFile(fileNameOrUrl, fileContents);

                        if (!isLocalFile) {
                            Core::hideCentralBusyWidget();
                        }

                        if (res) {
                            // We were able to retrieve the import contents, so
                            // instantiate the import with it

                            try {
                                import->instantiateFromText(fileContents.toStdWString());
                            } catch (iface::cellml_api::CellMLException &exception) {
                                // Something went wrong with the instantiation
                                // of the import

                                throw std::runtime_error(tr("<strong>%1</strong> imports <strong>%2</strong>, which contents could not be retrieved (%3)").arg(QDir::toNativeSeparators(xmlBaseFileNameOrUrl),
                                                                                                                                                               xlinkHrefString,
                                                                                                                                                               Core::formatMessage(QString::fromStdWString(exception.explanation))).toStdString());
                            }

                            // Keep track of the import contents

                            mImportContents.insert(fileNameOrUrl, fileContents);

                            // Keep track of the import as being one of our
                            // dependencies, should it be local and should we be
                            // directly dealing with our model

                            if (isLocalFile && (pModel == mModel)) {
                                dependencies << fileNameOrUrl;
                            }
                        } else {
                            throw std::runtime_error(tr("<strong>%1</strong> imports <strong>%2</strong>, which contents could not be retrieved").arg(QDir::toNativeSeparators(xmlBaseFileNameOrUrl),
                                                                                                                                                      xlinkHrefString).toStdString());
                        }
                    }

                    // Now that the import is instantiated, add its own imports
                    // to our list

                    ObjRef<iface::cellml_api::Model> importModel = import->importedModel();

                    if (importModel == nullptr) {
                        throw std::runtime_error(tr("<strong>%1</strong> imports <strong>%2</strong>, which CellML object could not be retrieved").arg(QDir::toNativeSeparators(xmlBaseFileNameOrUrl),
                                                                                                                                                       xlinkHrefString).toStdString());
                    }

                    retrieveImports(isLocalFile?
                                        QUrl::fromLocalFile(fileNameOrUrl).toString():
                                        fileNameOrUrl,
                                    importModel, importList, importXmlBaseList);
                }
            }
        } catch (std::runtime_error &runtimeError) {
            // Something went wrong with the full instantiation of the imports

            pIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the imports could not be fully instantiated (%1)").arg(runtimeError.what()));

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

bool CellmlFile::load(const QString &pFileContents,
                      ObjRef<iface::cellml_api::Model> *pModel,
                      CellmlFileIssues &pIssues)
{
    // Make sure that pIssues is empty

    pIssues.clear();

    // Get a bootstrap object and its model loader

    ObjRef<iface::cellml_api::DOMModelLoader> modelLoader = CreateCellMLBootstrap()->modelLoader();

    // Try to create the model

    try {
        if (pFileContents.isEmpty()) {
            *pModel = modelLoader->loadFromURL(QUrl::fromPercentEncoding(QUrl::fromLocalFile(mFileName).toEncoded()).toStdWString());
        } else {
            *pModel = modelLoader->createFromText(pFileContents.toStdWString());
        }
    } catch (iface::cellml_api::CellMLException &exception) {
        // Something went wrong with the loading of the model

        if (pFileContents.isEmpty()) {
            pIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the model could not be loaded (%1)").arg(Core::formatMessage(QString::fromStdWString(exception.explanation))));
        } else {
            pIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the model could not be created (%1)").arg(Core::formatMessage(QString::fromStdWString(exception.explanation))));
        }

        return false;
    }

    // Update the base URI, should the CellML file be a remote one or its
    // contents be directly passed onto us

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    ObjRef<iface::cellml_api::URI> baseUri = (*pModel)->xmlBase();

    if (fileManagerInstance->isRemote(mFileName)) {
        // We are dealing with a remote file, so its XML base value should point
        // to its remote location

        baseUri->asText(fileManagerInstance->url(mFileName).toStdWString());
    } else if (!pFileContents.isEmpty()) {
        // We are dealing with a file which contents was directly passed onto
        // us, so its XML base value should point to its actual location

        baseUri->asText(mFileName.toStdWString());
    }

    return true;
}

//==============================================================================

void CellmlFile::retrieveCmetaIdsFromCellmlElement(iface::cellml_api::CellMLElement *pElement)
{
    // Keep track of the given CellML element's cmeta:id

    QString cmetaId = QString::fromStdWString(pElement->cmetaId());

    if (!cmetaId.isEmpty()) {
        mUsedCmetaIds << cmetaId;
    }

    // Do the same for all the child elements of the given CellML element

    ObjRef<iface::cellml_api::CellMLElementIterator> childElementsIter = pElement->childElements()->iterate();

    try {
        for (ObjRef<iface::cellml_api::CellMLElement> childElement = childElementsIter->next();
             childElement != nullptr; childElement = childElementsIter->next()) {
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
    static const QString XmlnsCmeta = "xmlns:cmeta";

    if (!pUsedCmetaIds.contains(pElement.attribute(CmetaId))) {
        pElement.attributes().removeNamedItem(CmetaId);
        pElement.attributes().removeNamedItem(XmlnsCmeta);
    }

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

    if (!mLoadingNeeded) {
        return mIssues.isEmpty();
    }

    mLoadingNeeded = false;

    // Try to load the model

    if (!load({}, &mModel, mIssues)) {
        return false;
    }

    // Retrieve all the RDF triples associated with the model and initialise our
    // list of original RDF triples

    ObjRef<iface::cellml_api::RDFRepresentation> rdfRepresentation = mModel->getRDFRepresentation(L"http://www.cellml.org/RDF/API");

    if (rdfRepresentation != nullptr) {
        mRdfApiRepresentation = QueryInterface(rdfRepresentation);

        if (mRdfApiRepresentation != nullptr) {
            mRdfDataSource = mRdfApiRepresentation->source();
            ObjRef<iface::rdf_api::TripleEnumerator> rdfTriplesEnumerator = mRdfDataSource->getAllTriples()->enumerateTriples();

            for (ObjRef<iface::rdf_api::Triple> rdfTriple = rdfTriplesEnumerator->getNextTriple();
                 rdfTriple != nullptr; rdfTriple = rdfTriplesEnumerator->getNextTriple()) {
                mRdfTriples << new CellmlFileRdfTriple(this, rdfTriple);
            }

            mRdfTriples.updateOriginalRdfTriples();
        }
    }

    // Determine which cmeta:ids are currently in use, be they in the various
    // CellML elements or RDF triples

    retrieveCmetaIdsFromCellmlElement(mModel);

    for (auto rdfTriple : mRdfTriples) {
        mUsedCmetaIds << rdfTriple->metadataId();
    }

    mUsedCmetaIds.removeDuplicates();

    return true;
}

//==============================================================================

bool CellmlFile::save(const QString &pFileName)
{
    // Make sure that we are properly loaded and have no issues

    if (mLoadingNeeded || !mIssues.isEmpty()) {
        return false;
    }

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
    //          load()), so that we can properly import CellML files, if needed.
    //          So, now, we need to undo what we did...
    // Note #2: normally, we would be asking QDomDocument::setContent() to
    //          process namespaces, but this would then result in a very messy
    //          serialisation with namespaces being referenced all over the
    //          place. So, in the end, we do everything without processing
    //          namespaces...

    static const QString RdfRdf = "rdf:RDF";

    QDomDocument domDocument;

    domDocument.setContent(QString::fromStdWString(mModel->serialisedText()));

    QDomElement domElement = domDocument.documentElement();

    domElement.attributes().removeNamedItem("xml:base");

    for (QDomElement childElement = domElement.firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        if (childElement.nodeName() == RdfRdf) {
            if (childElement.childNodes().isEmpty()) {
                domElement.removeChild(childElement);
            }

            break;
        }
    }

    QStringList usedCmetaIds;

    for (auto rdfTriple : mRdfTriples) {
        usedCmetaIds << rdfTriple->metadataId();
    }

    usedCmetaIds.removeDuplicates();

    clearCmetaIdsFromCellmlElement(domElement, usedCmetaIds);

    // Write out the contents of our DOM document to our CellML file

    return Core::writeFile(pFileName.isEmpty()?mFileName:pFileName,
                           Core::serialiseDomDocument(domDocument))?
               StandardFile::save(pFileName):
               false;
}

//==============================================================================

bool CellmlFile::update(const QString &pFileName)
{
    // Our model parameters were modified (e.g. through the Simulation
    // Experiment view) and we want to update ourselves accordingly, so save
    // ourselves and keep track of the fact that we were 'simply' updated

    mUpdated = save(pFileName);

    return mUpdated;
}

//==============================================================================

bool CellmlFile::isValid(iface::cellml_api::Model *pModel,
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

    for (uint i = 0, iMax = cellmlValidityErrorSet->nValidityErrors(); i < iMax; ++i) {
        ObjRef<iface::cellml_services::CellMLValidityError> cellmlValidityIssue = cellmlValidityErrorSet->getValidityError(i);
        ObjRef<iface::cellml_services::CellMLRepresentationValidityError> cellmlRepresentationValidityError = QueryInterface(cellmlValidityIssue);

        // Determine the issue's location

        uint line = 0;
        uint column = 0;
        QString importedFileName;
        QString importedFileInfo;

        if (cellmlRepresentationValidityError != nullptr) {
            // We are dealing with a CellML representation issue, so determine
            // its line and column

            line = vacssService->getPositionInXML(cellmlRepresentationValidityError->errorNode(),
                                                  cellmlRepresentationValidityError->errorNodalOffset(),
                                                  &column);
        } else {
            // We are not dealing with a CellML representation issue, so check
            // whether we are dealing with a semantic one

            ObjRef<iface::cellml_services::CellMLSemanticValidityError> cellmlSemanticValidityError = QueryInterface(cellmlValidityIssue);

            if (cellmlSemanticValidityError != nullptr) {
                // We are dealing with a CellML semantic issue, so determine its
                // line and column

                ObjRef<iface::cellml_api::CellMLElement> cellmlElement = cellmlSemanticValidityError->errorElement();
                ObjRef<iface::cellml_api::CellMLDOMElement> cellmlDomElement = QueryInterface(cellmlElement);

                line = vacssService->getPositionInXML(cellmlDomElement->domElement(), 0, &column);

                // Also determine its imported file, if any

                ObjRef<iface::cellml_api::Model> importedCellmlFile = cellmlElement->modelElement();

                if (importedCellmlFile != nullptr) {
                    // Retrieve the imported CellML element

                    ObjRef<iface::cellml_api::CellMLElement> importedCellmlElement = importedCellmlFile->parentElement();

                    if (importedCellmlElement != nullptr) {
                        // Check whether the imported CellML element is an
                        // import CellML element and, if so, retrieve the
                        // relative file path of the corresponding CellML file
                        // and keep track of its absolute path version

                        ObjRef<iface::cellml_api::CellMLImport> importCellmlElement = QueryInterface(importedCellmlElement);

                        if (importCellmlElement != nullptr) {
                            importedFileInfo = QString::fromStdWString(importCellmlElement->xlinkHref()->asText());
                            importedFileName = Core::canonicalFileName(QFileInfo(mFileName).path()+"/"+importedFileInfo);
                        }
                    }
                }
            }
        }

        // Determine the issue's type

        CellmlFileIssue::Type issueType;

        if (cellmlValidityIssue->isWarningOnly()) {
            // We are dealing with a warning

            issueType = CellmlFileIssue::Type::Warning;
        } else {
            // We are dealing with an error

            ++cellmlErrorsCount;

            issueType = CellmlFileIssue::Type::Error;
        }

        // Append the issue to our list, but only if we don't already have it
        // Note: we will already have it if we are importing a faulty CellML
        //       file more than once...

        CellmlFileIssue issue = CellmlFileIssue(issueType, int(line), int(column),
                                                QString::fromStdWString(cellmlValidityIssue->description()),
                                                importedFileName, importedFileInfo);

        if (!pIssues.contains(issue)) {
            pIssues << issue;
        }
    }

    // Sort our issues (since the CellML API may generate them in a non-ordered
    // manner)

    std::sort(pIssues.begin(), pIssues.end(), CellmlFileIssue::compare);

    return cellmlErrorsCount == 0;
}

//==============================================================================

bool CellmlFile::isValid(const QString &pFileContents,
                         ObjRef<iface::cellml_api::Model> *pModel,
                         CellmlFileIssues &pIssues)
{
    // Try to load our model

    if (load(pFileContents, pModel, pIssues)) {
        // The file contents was properly loaded, so make sure that its imports,
        // if any, are fully instantiated

        if (fullyInstantiateImports(*pModel, pIssues)) {
            // Now, we can check whether the file contents is CellML valid

            return isValid(*pModel, pIssues);
        }

        return false;
    }

    return false;
}

//==============================================================================

bool CellmlFile::isValid(const QString &pFileContents,
                         CellmlFileIssues &pIssues)
{
    // Check whether the given file contents is CellML valid, so for this create
    // a temporary model

    ObjRef<iface::cellml_api::Model> model;

    return isValid(pFileContents, &model, pIssues);
}

//==============================================================================

bool CellmlFile::isValid()
{
    // Return whether we are valid

    return isValid({}, &mModel, mIssues);
}

//==============================================================================

CellmlFileIssues CellmlFile::issues() const
{
    // Return our issues

    return mIssues;
}

//==============================================================================

CellmlFileRuntime * CellmlFile::runtime()
{
    // Load (but not reload!) ourselves, if needed

    if (load()) {
        // Make sure that our imports, if any, are fully instantiated before
        // returning our runtime

        return fullyInstantiateImports(mModel, mIssues)?
                   new CellmlFileRuntime(this):
                   nullptr;
    }

    return nullptr;
}

//==============================================================================

QStringList CellmlFile::dependencies()
{
    // Check whether the dependencies need to be retrieved

    if (!mDependenciesNeeded) {
        return Core::FileManager::instance()->dependencies(mFileName);
    }

    // Load (but not reload!) ourselves, if needed

    if (load()) {
        // Make sure that our imports, if any, are fully instantiated

        if (fullyInstantiateImports(mModel, mIssues)) {
            // Now, we can return our dependencies

            return Core::FileManager::instance()->dependencies(mFileName);
        }

        return {};
    }

    return {};
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

        return nullptr;
    }

    // Go through the RDF triples associated with the given CellML element and
    // check whether it is the one we are after

    for (auto rdfTriple : rdfTriples(pElement)) {
        if (   (pQualifier == rdfTriple->qualifierAsString())
            && (pResource == rdfTriple->resource())
            && (pId == rdfTriple->id())) {
            // This is the RDF triple we are after

            return rdfTriple;
        }
    }

    // We couldn't find the RDF triple

    return nullptr;
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::rdfTriple(iface::cellml_api::CellMLElement *pElement,
                                            CellmlFileRdfTriple::ModelQualifier pModelQualifier,
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
                                            CellmlFileRdfTriple::BioQualifier pBioQualifier,
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
                                               CellmlFileRdfTriple::ModelQualifier pModelQualifier,
                                               const QString &pResource,
                                               const QString &pId)
{
    // Add an RDF triple to ourselves

    return mRdfTriples.add(new CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                   pModelQualifier, pResource, pId));
}

//==============================================================================

CellmlFileRdfTriple * CellmlFile::addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                               CellmlFileRdfTriple::BioQualifier pBioQualifier,
                                               const QString &pResource,
                                               const QString &pId)
{
    // Add an RDF Triple to ourselves

    return mRdfTriples.add(new CellmlFileRdfTriple(this, rdfTripleSubject(pElement),
                                                   pBioQualifier, pResource, pId));
}

//==============================================================================

bool CellmlFile::removeRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                 CellmlFileRdfTriple::ModelQualifier pModelQualifier,
                                 const QString &pResource, const QString &pId)
{
    // Remove an RDF triple from ourselves

    return mRdfTriples.remove(rdfTriple(pElement, pModelQualifier, pResource, pId));
}

//==============================================================================

bool CellmlFile::removeRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                 CellmlFileRdfTriple::BioQualifier pBioQualifier,
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

    if (load()) {
        return QString::fromStdWString(mModel->cmetaId());
    }

    return {};
}

//==============================================================================

QString CellmlFile::xmlBase()
{
    // Return our base URI

    if (load()) {
        return QString::fromStdWString(mModel->xmlBase()->asText());
    }

    return {};
}

//==============================================================================

bool CellmlFile::exportTo(const QString &pFileName, Version pVersion)
{
    // Export the model to the required format, after loading it if necessary

    if (load()) {
        // Check that it actually makes sense to export the model

        CellmlFile::Version modelVersion = CellmlFile::modelVersion(mModel);

        switch (pVersion) {
        case Version::Unknown:
        case Version::Cellml_1_1:
            // We cannot export to an unknown or CellML 1.1 format

            return false;
        case Version::Cellml_1_0:
            // To export to CellML 1.0, the model must be in a CellML 1.1 format

            if (modelVersion != Version::Cellml_1_1) {
                return false;
            }

            break;
        }

        // Do the actual export to CellML 1.0, after having fully instantiated
        // all the imports

        if (!fullyInstantiateImports(mModel, mIssues)) {
            return false;
        }

        CellmlFileCellml10Exporter exporter(mModel, pFileName);

        if (!exporter.errorMessage().isEmpty()) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       exporter.errorMessage());
        }

        return exporter.result();
    }

    return false;
}

//==============================================================================

bool CellmlFile::exportTo(const QString &pFileName,
                          const QString &pUserDefinedFormatFileName)
{
    // Export the model to the required format, after loading it if necessary

    if (load()) {
        // Check that the user-defined format file actually exists

        if (!QFile::exists(pUserDefinedFormatFileName)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the user-defined format file does not exist"));

            return false;
        }

        // Make sure that the user-defined format file is valid XML
        // Note: you would normally expect CeLEDSExporter to check this, but all
        //       it does in case of an invalid XML file is crash...

        QByteArray fileContents;

        if (!Core::readFile(pUserDefinedFormatFileName, fileContents)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the user-defined format file could not be read"));

            return false;
        }

        QDomDocument domDocument;

        if (!domDocument.setContent(fileContents)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the user-defined format file is not a valid XML file"));

            return false;
        }

        // Fully instantiate all the imports

        if (!fullyInstantiateImports(mModel, mIssues)) {
            return false;
        }

        // Do the actual export

        ObjRef<iface::cellml_services::CeLEDSExporterBootstrap> celedsExporterBootstrap = CreateCeLEDSExporterBootstrap();
        ObjRef<iface::cellml_services::CodeExporter> codeExporter = celedsExporterBootstrap->createExporterFromText(QString(fileContents).toStdWString());

        if (!celedsExporterBootstrap->loadError().empty()) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the user-defined format file could not be loaded"));

            return false;
        }

        // Save the export or output it to the console, if no file name has been
        // provided

        if (pFileName.isEmpty()) {
            std::cout << QString::fromStdWString(codeExporter->generateCode(mModel)).trimmed().toStdString() << std::endl;
        } else if (!Core::writeFile(pFileName, QString::fromStdWString(codeExporter->generateCode(mModel)))) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the output file could not be saved"));

            return false;
        }

        return true;
    }

    return false;
}

//==============================================================================

CellmlFile::Version CellmlFile::version()
{
    // Return our version

    if (load()) {
        return modelVersion(mModel);
    }

    return Version::Unknown;
}

//==============================================================================

CellmlFile::Version CellmlFile::modelVersion(iface::cellml_api::Model *pModel)
{
    // Return the version of the given CellML model, if any

    if (pModel == nullptr) {
        return Version::Unknown;
    }

    QString cellmlVersion = QString::fromStdWString(pModel->cellmlVersion());

    if (cellmlVersion == CellMLSupport::Cellml_1_0) {
        return Version::Cellml_1_0;
    }

    if (cellmlVersion == CellMLSupport::Cellml_1_1) {
        return Version::Cellml_1_1;
    }

    qWarning("WARNING | %s:%d: a CellML version should not be unknown.", __FILE__, __LINE__);

    return Version::Unknown;
}

//==============================================================================

CellmlFile::Version CellmlFile::fileVersion(const QString &pFileName)
{
    // Return the version of the given CellML file

    ObjRef<iface::cellml_api::Model> model;

    try {
        model = CreateCellMLBootstrap()->modelLoader()->loadFromURL(QUrl::fromPercentEncoding(QUrl::fromLocalFile(pFileName).toEncoded()).toStdWString());
    } catch (...) {
        return Version::Unknown;
    }

    return modelVersion(model);
}

//==============================================================================

CellmlFile::Version CellmlFile::fileContentsVersion(const QString &pFileContents)
{
    // Return the version of the given CellML file contents

    ObjRef<iface::cellml_api::Model> model;

    try {
        model = CreateCellMLBootstrap()->modelLoader()->createFromText(pFileContents.toStdWString());
    } catch (...) {
        return Version::Unknown;
    }

    return modelVersion(model);
}

//==============================================================================

QString CellmlFile::versionAsString(Version pVersion)
{
    // Return the string corresponding to the given version

    switch (pVersion) {
    case Version::Unknown:
        return "???";
    case Version::Cellml_1_0:
        return "CellML 1.0";
    case Version::Cellml_1_1:
        return "CellML 1.1";
    }

    return "???";
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

} // namespace CellMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
