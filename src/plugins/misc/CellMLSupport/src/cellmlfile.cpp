//==============================================================================
// CellML file class
//==============================================================================

#include "cellmlfile.h"
#include "filemanager.h"

//==============================================================================

#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QTime>
#include <QUrl>

//==============================================================================

#ifdef Q_WS_X11
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
    mCellmlApiModel(0),
    mModel(0),
    mImports(CellmlFileImports()),
    mUnits(CellmlFileUnits()),
    mComponents(CellmlFileComponents()),
    mGroups(CellmlFileGroups()),
    mConnections(CellmlFileConnections()),
    mRdfTriples(CellmlFileRdfTriples(this)),
    mValid(true)
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
    // Note: setting mCellmlApiModel to zero will automatically delete the
    //       current instance, if any

    mCellmlApiModel = 0;

    delete mModel;

    clearImports();
    clearUnits();
    clearComponents();
    clearGroups();
    clearConnections();
    clearRdfTriples();

    mIssues.clear();

    mLoadingNeeded       = true;
    mValidNeeded         = true;
    mRuntimeUpdateNeeded = true;
}

//==============================================================================

bool CellmlFile::load()
{
    if (!mLoadingNeeded)
        // The file is already loaded, so...

        return true;

    // Reset any issues that we may have found before

    mIssues.clear();

    // Get a bootstrap object

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();

    // Get its model loader

    ObjRef<iface::cellml_api::DOMModelLoader> modelLoader = cellmlBootstrap->modelLoader();

    // Try to load the model

#ifdef QT_DEBUG
    qDebug("---------------------------------------");
    qDebug("%s:", qPrintable(mFileName));
#endif

    try {
#ifdef QT_DEBUG
        QTime time;

        time.start();
#endif

        mCellmlApiModel = modelLoader->loadFromURL(QUrl::fromLocalFile(mFileName).toString().toStdWString().c_str());

#ifdef QT_DEBUG
        qDebug(" - CellML Loading time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif
    } catch (iface::cellml_api::CellMLException &) {
        // Something went wrong with the loading of the model, so...

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("the model could not be loaded (%1)").arg(QString::fromStdWString(modelLoader->lastErrorMessage())));

        return false;
    }

    // In the case of a non CellML 1.0 model, we want all imports to be fully
    // instantiated

    if (QString::fromStdWString(mCellmlApiModel->cellmlVersion()).compare(Cellml_1_0))
        try {
#ifdef QT_DEBUG
            QTime time;

            time.start();
#endif

            mCellmlApiModel->fullyInstantiateImports();

#ifdef QT_DEBUG
            qDebug(" - CellML full instantiation time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif
        } catch (...) {
            // Something went wrong with the full instantiation of the imports,
            // so...

            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the model's imports could not be fully instantiated"));

            return false;
        }

    // Extract/retrieve various things from mCellmlApiModel

    mModel = new CellmlFileModel(this, mCellmlApiModel);

    // Iterate through the imports and add them to our list

    ObjRef<iface::cellml_api::CellMLImportSet> imports = mCellmlApiModel->imports();
    ObjRef<iface::cellml_api::CellMLImportIterator> importIterator = imports->iterateImports();

    forever {
        ObjRef<iface::cellml_api::CellMLImport> import = importIterator->nextImport();

        if (import)
            // We have an import, so add it to our list

            mImports << new CellmlFileImport(this, import);
        else
            // No more imports, so...

            break;
    }

    // Iterate through the units and add them to our list

    ObjRef<iface::cellml_api::UnitsSet> units = mCellmlApiModel->localUnits();
    ObjRef<iface::cellml_api::UnitsIterator> unitIterator = units->iterateUnits();

    forever {
        ObjRef<iface::cellml_api::Units> unit = unitIterator->nextUnits();

        if (unit)
            // We have a unit, so add it to our list

            mUnits << new CellmlFileUnit(this, unit);
        else
            // No more units, so...

            break;
    }

    // Iterate through the components and add them to our list

    ObjRef<iface::cellml_api::CellMLComponentSet> components = mCellmlApiModel->localComponents();
    ObjRef<iface::cellml_api::CellMLComponentIterator> componentIterator = components->iterateComponents();

    forever {
        ObjRef<iface::cellml_api::CellMLComponent> component = componentIterator->nextComponent();

        if (component)
            // We have a component, so add it to our list

            mComponents << new CellmlFileComponent(this, component);
        else
            // No more components, so...

            break;
    }

    // Iterate through the groups and add them to our list

    ObjRef<iface::cellml_api::GroupSet> groups = mCellmlApiModel->groups();
    ObjRef<iface::cellml_api::GroupIterator> groupIterator = groups->iterateGroups();

    forever {
        ObjRef<iface::cellml_api::Group> group = groupIterator->nextGroup();

        if (group)
            // We have a group, so add it to our list

            mGroups << new CellmlFileGroup(this, group);
        else
            // No more groups, so...

            break;
    }

    // Iterate through the connections and add them to our list

    ObjRef<iface::cellml_api::ConnectionSet> connections = mCellmlApiModel->connections();
    ObjRef<iface::cellml_api::ConnectionIterator> connectionIterator = connections->iterateConnections();

    forever {
        ObjRef<iface::cellml_api::Connection> connection = connectionIterator->nextConnection();

        if (connection)
            // We have a connection, so add it to our list

            mConnections << new CellmlFileConnection(this, connection);
        else
            // No more connections, so...

            break;
    }

    // Retrieve all the RDF triples associated with the model

    ObjRef<iface::cellml_api::RDFRepresentation> rdfRepresentation = mCellmlApiModel->getRDFRepresentation(L"http://www.cellml.org/RDF/API");

    if (rdfRepresentation) {
        ObjRef<iface::rdf_api::RDFAPIRepresentation> rdfApiRepresentation;
        QUERY_INTERFACE(rdfApiRepresentation, rdfRepresentation,
                        rdf_api::RDFAPIRepresentation);

        if (rdfApiRepresentation) {
            ObjRef<iface::rdf_api::DataSource> dataSource = rdfApiRepresentation->source();
            ObjRef<iface::rdf_api::TripleSet> triples = dataSource->getAllTriples();
            ObjRef<iface::rdf_api::TripleEnumerator> tripleEnumerator = triples->enumerateTriples();

            forever {
                ObjRef<iface::rdf_api::Triple> triple = tripleEnumerator->getNextTriple();

                if (triple)
                    // We have a triple, so add it to our list

                    mRdfTriples << new CellmlFileRdfTriple(this, triple);
                else
                    // No more triples, so...

                    break;
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

    // (Create and) open the file for writing

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // The file can't be opened, so...

        file.remove();

        return false;
    }

    // Write out the contents of the CellML file to the file

    QTextStream out(&file);

    out << QString::fromStdWString(mCellmlApiModel->serialisedText());

    file.close();

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

#ifdef QT_DEBUG
        QTime time;

        time.start();
#endif

        ObjRef<iface::cellml_services::VACSService> vacssService = CreateVACSService();
        ObjRef<iface::cellml_services::CellMLValidityErrorSet> cellmlValidityErrorSet = vacssService->validateModel(mCellmlApiModel);

#ifdef QT_DEBUG
        qDebug(" - CellML validation time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

        // Determine the number of errors and warnings
        // Note: CellMLValidityErrorSet::nValidityErrors() returns any type of
        //       validation issue, be it an error or a warning, so we need to
        //       determine the number of true errors

        int cellmlErrorsCount = 0;

#ifdef QT_DEBUG
        time.restart();
#endif

        for (int i = 0, iMax = cellmlValidityErrorSet->nValidityErrors(); i < iMax; ++i) {
            ObjRef<iface::cellml_services::CellMLValidityError> cellmlValidityIssue = cellmlValidityErrorSet->getValidityError(i);

            DECLARE_QUERY_INTERFACE_OBJREF(cellmlRepresentationValidityError, cellmlValidityIssue,
                                           cellml_services::CellMLRepresentationValidityError);

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

                DECLARE_QUERY_INTERFACE_OBJREF(cellmlSemanticValidityError, cellmlValidityIssue,
                                               cellml_services::CellMLSemanticValidityError);

                if (cellmlSemanticValidityError) {
                    // We are dealing with a CellML semantic issue, so determine
                    // its line and column

                    ObjRef<iface::cellml_api::CellMLElement> cellmlElement = cellmlSemanticValidityError->errorElement();

                    DECLARE_QUERY_INTERFACE_OBJREF(cellmlDomElement, cellmlElement,
                                                   cellml_api::CellMLDOMElement);

                    ObjRef<iface::dom::Element> domElement = cellmlDomElement->domElement();

                    line = vacssService->getPositionInXML(domElement, 0,
                                                          &column);

                    // Also determine its imported file, if any

                    forever {
                        // Retrieve the CellML element's parent

                        ObjRef<iface::cellml_api::CellMLElement> cellmlElementParent = cellmlElement->parentElement();

                        if (!cellmlElementParent)
                            // There is no parent, so...

                            break;

                        // Check whether the parent is an imported file

                        DECLARE_QUERY_INTERFACE_OBJREF(importedCellmlFile, cellmlElementParent,
                                                       cellml_api::Model);

                        if (!importedCellmlFile)
                            // This is not an imported file, so...

                            continue;

                        // Retrieve the imported CellML element

                        ObjRef<iface::cellml_api::CellMLElement> importedCellmlElement = importedCellmlFile->parentElement();

                        if (!importedCellmlElement)
                            // This is not an imported CellML element, so...

                            break;

                        // Check whether the imported CellML element is an
                        // import CellML element

                        DECLARE_QUERY_INTERFACE_OBJREF(importCellmlElement, importedCellmlElement,
                                                       cellml_api::CellMLImport);

                        if (!importCellmlElement)
                            // This is not an import CellML element, so...

                            break;

                        ObjRef<iface::cellml_api::URI> href = importCellmlElement->xlinkHref();

                        importedFile = QString::fromStdWString(href->asText());

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

#ifdef QT_DEBUG
        qDebug(" - CellML warnings vs. errors time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

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

        mRuntime->update(mCellmlApiModel);

        mRuntimeUpdateNeeded = !mRuntime->isValid();

        return mRuntime;
    } else {
        // The file isn't valid, so reset its runtime

        return mRuntime->update();
    }
}

//==============================================================================

QString CellmlFile::fileName() const
{
    // Return the CellML file's file name

    return mFileName;
}

//==============================================================================

CellmlFileModel * CellmlFile::model() const
{
    // Return the CellML file's model

    return mModel;
}

//==============================================================================

CellmlFileImports * CellmlFile::imports()
{
    // Return the CellML file's imports

    return &mImports;
}

//==============================================================================

CellmlFileUnits * CellmlFile::units()
{
    // Return the CellML file's units

    return &mUnits;
}

//==============================================================================

CellmlFileComponents * CellmlFile::components()
{
    // Return the CellML file's components

    return &mComponents;
}

//==============================================================================

CellmlFileGroups * CellmlFile::groups()
{
    // Return the CellML file's groups

    return &mGroups;
}

//==============================================================================

CellmlFileConnections * CellmlFile::connections()
{
    // Return the CellML file's connections

    return &mConnections;
}

//==============================================================================

CellmlFileRdfTriples * CellmlFile::rdfTriples()
{
    // Return the CellML file's RDF triples

    return &mRdfTriples;
}

//==============================================================================

QString CellmlFile::uriBase() const
{
    // Return the CellML file's URI base

    ObjRef<iface::cellml_api::URI> xmlBase = mCellmlApiModel->xmlBase();

    return QString::fromStdWString(xmlBase->asText());
}

//==============================================================================

CellmlFileRdfTriples CellmlFile::rdfTriples(const QString &pCmetaId) const
{
    // Return all the RDF triples which are directly or indirectly associated
    // with pCmetaId

    return mRdfTriples.contains(pCmetaId);
}

//==============================================================================

void CellmlFile::clearImports()
{
    // Delete all the imports and clear our list

    foreach (CellmlFileImport *import, mImports)
        delete import;

    mImports.clear();
}

//==============================================================================

void CellmlFile::clearUnits()
{
    // Delete all the units and clear our list

    foreach (CellmlFileUnit *unit, mUnits)
        delete unit;

    mUnits.clear();
}

//==============================================================================

void CellmlFile::clearComponents()
{
    // Delete all the components and clear our list

    foreach (CellmlFileComponent *component, mComponents)
        delete component;

    mComponents.clear();
}

//==============================================================================

void CellmlFile::clearGroups()
{
    // Delete all the groups and clear our list

    foreach (CellmlFileGroup *group, mGroups)
        delete group;

    mGroups.clear();
}

//==============================================================================

void CellmlFile::clearConnections()
{
    // Delete all the connections and clear our list

    foreach (CellmlFileConnection *connection, mConnections)
        delete connection;

    mConnections.clear();
}

//==============================================================================

void CellmlFile::clearRdfTriples()
{
    // Delete all the RDF triples and clear our list

    foreach (CellmlFileRdfTriple *triple, mRdfTriples)
        delete triple;

    mRdfTriples.clear();
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
