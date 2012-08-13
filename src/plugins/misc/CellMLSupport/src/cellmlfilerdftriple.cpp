//==============================================================================
// CellML file RDF triple
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfilerdftriple.h"

//==============================================================================

#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                         iface::rdf_api::Triple *pCellmlApiRdfTriple) :
    mCellmlFile(pCellmlFile),
    mCellmlApiRdfTriple(pCellmlApiRdfTriple),
    mType(Unknown),
    mModelQualifier(ModelUnknown),
    mBioQualifier(BioUnknown),
    mResource(QString()),
    mId(QString())
{
    // Retrieve the RDF triple's subject, predicate and object information

    ObjRef<iface::rdf_api::Resource> subject = pCellmlApiRdfTriple->subject();
    ObjRef<iface::rdf_api::Resource> predicate = pCellmlApiRdfTriple->predicate();
    ObjRef<iface::rdf_api::Node> object = pCellmlApiRdfTriple->object();

    mSubject   = new CellmlFileRdfTripleElement(subject);
    mPredicate = new CellmlFileRdfTripleElement(predicate);
    mObject    = new CellmlFileRdfTripleElement(object);

    // Determine the type of the RDF triple

    // Note: at this stage, we only recognise model and bio(logy) qualifiers as
    //       described at http://biomodels.net/qualifiers/. This means that the
    //       predicate of the RDF triple must have one of the following two
    //       forms:
    //
    //          http://biomodels.net/model-qualifiers/<xxx>
    //          http://biomodels.net/biology-qualifiers/<yyy>
    //
    //       where <xxx> and <yyy> are one of the values returned by
    //       modelQualifierAsString() and bioQualifierAsString() below (minus
    //       the "model:" and "bio:" parts, respectively). The object of the RDF
    //       triple must have one of the following two formats:
    //
    //          urn:miriam:<resource>:<identifier>
    //          http://identifiers.org/<resource>/<identifier>
    //
    //       For example:
    //
    //          urn:miriam:uniprot:Q4KLA0
    //          http://identifiers.org/uniprot/Q4KLA0
    //
    //       The former is a MIRIAM URN while the latter is an identifiers.org
    //       URI. MIRIAM URNs are being deprecated in favour of identifiers.org
    //       URIs. Still, we should and do support both formats and keep track
    //       of their information in the form of a resource and an id...

    for (int i = FirstModelQualifier; i <= LastModelQualifier; ++i)
        if (!mPredicate->asString().compare(QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifierAsString(ModelQualifier(i)).remove(QRegExp("^model:"))))) {
            // It looks like we might be dealing with a model qualifier

            mType = BioModelsDotNetQualifier;

            mModelQualifier = ModelQualifier(i);

            break;
        }

    if (mType == Unknown)
        for (int i = FirstBioQualifier; i <= LastBioQualifier; ++i)
            if (!mPredicate->asString().compare(QString("http://biomodels.net/biology-qualifiers/%1").arg(bioQualifierAsString(BioQualifier(i)).remove(QRegExp("^bio:"))))){
                // It looks like we might be dealing with a model qualifier

                mType = BioModelsDotNetQualifier;

                mBioQualifier = BioQualifier(i);

                break;
            }

    if (mType == BioModelsDotNetQualifier) {
        // We seem to be dealing with either a model or a bio(logy) qualifier,
        // so check whether its object is either a valid MIRIAM URN or a valid
        // identifiers.org URI

        QString objectAsString = mObject->asString();

        if (QRegExp("^urn:miriam:[a-zA-Z0-9\\._%]+:[a-zA-Z0-9\\._%]+").exactMatch(objectAsString)) {
            // The object is a valid MIRIAM URN, so retrieve its corresponding
            // resource and id

            decodeMiriamUrn(objectAsString, mResource, mId);
        } else if (QRegExp("^http://identifiers.org/[a-zA-Z0-9\\._:]+/[a-zA-Z0-9\\._:]+").exactMatch(objectAsString)) {
            // The object is a valid identifiers.org URI, so retrieve its
            // corresponding resource and id

            decodeIdentifiersDotOrgUri(objectAsString, mResource, mId);
        } else {
            // The object is neither a valid MIRIAM URN nor a valid
            // identifiers.org URI which means that the RDF triple is not a
            // valid model/bio(logy) qualifier, so...

            mType = Unknown;

            mModelQualifier = ModelUnknown;
            mBioQualifier   = BioUnknown;
        }
    }
}

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                         const QString pSubject,
                                         const ModelQualifier &pModelQualifier,
                                         const QString &pResource,
                                         const QString &pId) :
    mCellmlFile(pCellmlFile),
    mCellmlApiRdfTriple(0),
    mType(BioModelsDotNetQualifier),
    mModelQualifier(pModelQualifier),
    mBioQualifier(BioUnknown),
    mResource(pResource),
    mId(pId)
{
    // Create our RDF triple elements

    mSubject   = new CellmlFileRdfTripleElement(pSubject);
    mPredicate = new CellmlFileRdfTripleElement(QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifierAsString(pModelQualifier).remove(QRegExp("^model:"))));
    mObject    = new CellmlFileRdfTripleElement(QString("http://identifiers.org/%1/%2").arg(pResource, pId));
}

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                         const QString pSubject,
                                         const BioQualifier &pBioQualifier,
                                         const QString &pResource,
                                         const QString &pId) :
    mCellmlFile(pCellmlFile),
    mCellmlApiRdfTriple(0),
    mType(BioModelsDotNetQualifier),
    mModelQualifier(ModelUnknown),
    mBioQualifier(pBioQualifier),
    mResource(pResource),
    mId(pId)
{
    // Create our RDF triple elements

    mSubject   = new CellmlFileRdfTripleElement(pSubject);
    mPredicate = new CellmlFileRdfTripleElement(QString("http://biomodels.net/biology-qualifiers/%1").arg(bioQualifierAsString(pBioQualifier).remove(QRegExp("^bio:"))));
    mObject    = new CellmlFileRdfTripleElement(QString("http://identifiers.org/%1/%2").arg(pResource, pId));
}

//==============================================================================

CellmlFileRdfTriple::~CellmlFileRdfTriple()
{
    // Delete/release some internal objects

    delete mSubject;
    delete mPredicate;
    delete mObject;

    if (mCellmlApiRdfTriple)
        mCellmlApiRdfTriple->release_ref();
}

//==============================================================================

iface::rdf_api::Triple * CellmlFileRdfTriple::cellmlApiRdfTriple() const
{
    // Return the RDF triple's CellML API RDFtriple

    return mCellmlApiRdfTriple;
}

//==============================================================================

void CellmlFileRdfTriple::setCellmlApiRdfTriple(iface::rdf_api::Triple *pCellmlApiRdfTriple)
{
    // Set the RDF triple's CellML API RDF triple

    mCellmlApiRdfTriple = pCellmlApiRdfTriple;
}

//==============================================================================

CellmlFileRdfTripleElement * CellmlFileRdfTriple::subject() const
{
    // Return the RDF triple's subject

    return mSubject;
}

//==============================================================================

CellmlFileRdfTripleElement * CellmlFileRdfTriple::predicate() const
{
    // Return the RDF triple's predicate

    return mPredicate;
}

//==============================================================================

CellmlFileRdfTripleElement * CellmlFileRdfTriple::object() const
{
    // Return the RDF triple's object

    return mObject;
}

//==============================================================================

CellmlFileRdfTriple::Type CellmlFileRdfTriple::type() const
{
    // Return the RDF triple's type

    return mType;
}

//==============================================================================

QString CellmlFileRdfTriple::metadataId() const
{
    // Return the RDF triple's metadata id

    if (mSubject->type() == CellmlFileRdfTripleElement::UriReference)
        // The subject of our RDF triple is a URI reference, so we can retrieve
        // its metadata id

        return mSubject->uriReference().remove(QRegExp("^"+QRegExp::escape(mCellmlFile->uriBase())+"#?"));
    else
        // We don't recognise the subject of our RDF triple, so...

        return QString();
}

//==============================================================================

QString CellmlFileRdfTriple::qualifierAsString() const
{
    // Return the RDF triple's model or bio(logy) qualifier as a string

    return (mModelQualifier != ModelUnknown)?
                modelQualifierAsString():
                (mBioQualifier != BioUnknown)?
                    bioQualifierAsString():
                    "unknown";
}

//==============================================================================

CellmlFileRdfTriple::ModelQualifier CellmlFileRdfTriple::modelQualifier() const
{
    // Return the RDF triple's model qualifier

    return mModelQualifier;
}

//==============================================================================

QString CellmlFileRdfTriple::modelQualifierAsString() const
{
    // Return the RDF triple's model qualifier as a string

    return modelQualifierAsString(mModelQualifier);
}

//==============================================================================

QString CellmlFileRdfTriple::modelQualifierAsString(const ModelQualifier &pModelQualifier)
{
    // Return the RDF triple's model qualifier as a string

    switch (pModelQualifier) {
    case ModelIs:
        return "model:is";
    case ModelIsDerivedFrom:
        return "model:isDerivedFrom";
    case ModelIsDescribedBy:
        return "model:isDescribedBy";
    default:
        // ModelUnknown

        return "model:unknown";
    }
}

//==============================================================================

CellmlFileRdfTriple::BioQualifier CellmlFileRdfTriple::bioQualifier() const
{
    // Return the RDF triple's bio(logy) qualifier

    return mBioQualifier;
}

//==============================================================================

QString CellmlFileRdfTriple::bioQualifierAsString() const
{
    // Return the RDF triple's bio(logy) qualifier as a string

    return bioQualifierAsString(mBioQualifier);
}

//==============================================================================

QString CellmlFileRdfTriple::bioQualifierAsString(const BioQualifier &pBioQualifier)
{
    // Return the RDF triple's bio(logy) qualifier as a string

    switch (pBioQualifier) {
    case BioEncodes:
        return "bio:encodes";
    case BioHasPart:
        return "bio:hasPart";
    case BioHasProperty:
        return "bio:hasProperty";
    case BioHasVersion:
        return "bio:hasVersion";
    case BioIs:
        return "bio:is";
    case BioIsDescribedBy:
        return "bio:isDescribedBy";
    case BioIsEncodedBy:
        return "bio:isEncodedBy";
    case BioIsHomologTo:
        return "bio:isHomologTo";
    case BioIsPartOf:
        return "bio:isPartOf";
    case BioIsPropertyOf:
        return "bio:isPropertyOf";
    case BioIsVersionOf:
        return "bio:isVersionOf";
    case BioOccursIn:
        return "bio:occursIn";
    case BioHasTaxon:
        return "bio:hasTaxon";
    default:
        // BioUnknown

        return "bio:unknown";
    }
}

//==============================================================================

QStringList CellmlFileRdfTriple::qualifiersAsStringList()
{
    // Return (in alphabetical order) all the qualifiers which we support

    return QStringList() << bioQualifierAsString(BioEncodes)
                         << bioQualifierAsString(BioHasPart)
                         << bioQualifierAsString(BioHasProperty)
                         << bioQualifierAsString(BioHasVersion)
                         << bioQualifierAsString(BioIs)
                         << bioQualifierAsString(BioIsDescribedBy)
                         << bioQualifierAsString(BioIsEncodedBy)
                         << bioQualifierAsString(BioIsHomologTo)
                         << bioQualifierAsString(BioIsPartOf)
                         << bioQualifierAsString(BioIsPropertyOf)
                         << bioQualifierAsString(BioIsVersionOf)
                         << bioQualifierAsString(BioOccursIn)
                         << bioQualifierAsString(BioHasTaxon)
                         << modelQualifierAsString(ModelIs)
                         << modelQualifierAsString(ModelIsDerivedFrom)
                         << modelQualifierAsString(ModelIsDescribedBy);
}

//==============================================================================

QString CellmlFileRdfTriple::resource() const
{
    // Return the RDF triple's resource

    return mResource;
}

//==============================================================================

QString CellmlFileRdfTriple::id() const
{
    // Return the RDF triple's id

    return mId;
}

//==============================================================================

CellmlFileRdfTriples::CellmlFileRdfTriples(CellmlFile *pCellmlFile) :
    mCellmlFile(pCellmlFile)
{
}

//==============================================================================

CellmlFileRdfTriple::Type CellmlFileRdfTriples::type() const
{
    // Return the type of the RDF triples
    // Note: if the RDF triples are of different types each then the overall
    //       type is considered to be unknown

    if (isEmpty()) {
        // There are no RDF triples, so...

        return CellmlFileRdfTriple::Empty;
    } else {
        // There is at least one RDF triple, so retrieve the subject and type of
        // the first RDF triple and consider its type as the default type for
        // all our RDF triples

        CellmlFileRdfTriple *rdfTriple = first();

        QString subject = rdfTriple->subject()->asString();
        CellmlFileRdfTriple::Type res = rdfTriple->type();

        // Go through the RDF triples and make sure that their type is
        // consistent with that of the first RDF triple

        foreach (CellmlFileRdfTriple *rdfTriple, *this)
            if (   rdfTriple->subject()->asString().compare(subject)
                || (rdfTriple->type() != res))
                // The subject and/or the type of the current RDF triple is
                // different from that of the first RDF triple, so...

                return CellmlFileRdfTriple::Unknown;

        // All of the RDF triples have the same type, so...

        return res;
    }
}

//==============================================================================

void CellmlFileRdfTriple::decodeMiriamUrn(const QString &pMiriamUrn,
                                          QString &pResource, QString &pId)
{
    // Decode the MIRIAM URN to retrieve the corresponding resource and id

    QStringList miriamUrnList = pMiriamUrn.split(":");

    pResource = miriamUrnList[2].replace("%3A", ":");
    pId       = miriamUrnList[3].replace("%3A", ":");
}

//==============================================================================

void CellmlFileRdfTriple::decodeIdentifiersDotOrgUri(const QString &pIdentifiersDotOrgUri,
                                                     QString &pResource,
                                                     QString &pId)
{
    // Decode the identifiers.org URI to retrieve the corresponding resource and
    // id

    QString identifiersDotOrgUri = pIdentifiersDotOrgUri;
    // Note: the above is because pIdentifiersDotOrgUri is a const, so we can't
    //       directly use QString::remove() on it...
    QStringList identifiersDotOrgUriList = identifiersDotOrgUri.remove("http://identifiers.org/").split("/");

    pResource = identifiersDotOrgUriList[0];
    pId       = identifiersDotOrgUriList[1];
}

//==============================================================================

void CellmlFileRdfTriples::recursiveContains(CellmlFileRdfTriples &pRdfTriples,
                                             CellmlFileRdfTriple *pRdfTriple) const
{
    // Add pRdfTriple to pRdfTriples

    pRdfTriples << pRdfTriple;

    // Recursively add all the RDF triples which subject matches pRdfTriple's
    // object

    foreach (CellmlFileRdfTriple *rdfTriple, *this)
        if (!rdfTriple->subject()->asString().compare(pRdfTriple->object()->asString()))
            recursiveContains(pRdfTriples, rdfTriple);
}

//==============================================================================

CellmlFileRdfTriples CellmlFileRdfTriples::contains(const QString &pMetadataId) const
{
    // Return all the RDF triples which are directly or indirectly associated
    // with the given metadata id

    CellmlFileRdfTriples res = CellmlFileRdfTriples(mCellmlFile);

    foreach (CellmlFileRdfTriple *rdfTriple, *this)
        // Retrieve the RDF triple's subject so we can determine whether it's
        // from the group of RDF triples in which we are interested

        if (!pMetadataId.compare(rdfTriple->metadataId()))
            // It's the correct metadata id, so add it to our list

            recursiveContains(res, rdfTriple);

    return res;
}

//==============================================================================

CellmlFileRdfTriple * CellmlFileRdfTriples::add(CellmlFileRdfTriple *pRdfTriple)
{
    // Add the given RDF triple

    append(pRdfTriple);

    // Create a CellML API version of the RDF triple

    iface::rdf_api::DataSource *dataSource = mCellmlFile->cellmlApiRdfDataSource();

    ObjRef<iface::rdf_api::Resource> subject = already_AddRefd<iface::rdf_api::Resource>(dataSource->getURIReference(pRdfTriple->subject()->asString().toStdWString()));
    ObjRef<iface::rdf_api::Resource> predicate = already_AddRefd<iface::rdf_api::Resource>(dataSource->getURIReference(pRdfTriple->predicate()->asString().toStdWString()));
    ObjRef<iface::rdf_api::Node> object = already_AddRefd<iface::rdf_api::Node>(dataSource->getURIReference(pRdfTriple->object()->asString().toStdWString()));

    subject->createTripleOutOf(predicate, object);

    // Keep track of the CellML API version of the RDF triple in pRdfTriple
    // Note: for this, we scan through all the RDF triples which we can find in
    //       the data source until we find the one we have just added...

    ObjRef<iface::rdf_api::TripleSet> rdfTriples = dataSource->getAllTriples();
    ObjRef<iface::rdf_api::TripleEnumerator> rdfTriplesEnumerator = rdfTriples->enumerateTriples();
    iface::rdf_api::Triple *rdfTriple;

    forever {
        rdfTriple = rdfTriplesEnumerator->getNextTriple();

        if (!rdfTriple)
            // Note: we should never reach this point since this would mean that
            //       we couldn't retrieve the CellML API version of the RDF
            //       triple which we have just created above...

            break;

        ObjRef<iface::rdf_api::Resource> cellmlApiRdfTripleSubject = rdfTriple->subject();
        ObjRef<iface::rdf_api::Resource> cellmlApiRdfTriplePredicate = rdfTriple->predicate();
        ObjRef<iface::rdf_api::Node> cellmlApiRdfTripleObject = rdfTriple->object();

        if (   !CellmlFileRdfTripleElement(cellmlApiRdfTripleSubject).asString().compare(pRdfTriple->subject()->asString())
            && !CellmlFileRdfTripleElement(cellmlApiRdfTriplePredicate).asString().compare(pRdfTriple->predicate()->asString())
            && !CellmlFileRdfTripleElement(cellmlApiRdfTripleObject).asString().compare(pRdfTriple->object()->asString())) {
            // This is the RDF triple we are after, so keep track of it and
            // leave this loop

            pRdfTriple->setCellmlApiRdfTriple(rdfTriple);

            break;
        } else {
            // Not the RDF triple we are after, so...

            rdfTriple->release_ref();
        }
    }

    // Check that we actually managed to retrieve the CellML API version of our
    // RDF triple

    Q_ASSERT(rdfTriple);

    // An RDF triple has been added, so...

    mCellmlFile->setModified(true);

    return pRdfTriple;
}

//==============================================================================

void CellmlFileRdfTriples::removeRdfTriples(const CellmlFileRdfTriples &pRdfTriples)
{
    // Remove all the given RDF triples

    if (pRdfTriples.count()) {
        foreach (CellmlFileRdfTriple *rdfTriple, pRdfTriples) {
            // Remove the RDF triple

            removeOne(rdfTriple);

            // Remove the CellML API version of the RDF triple from its data
            // source

            rdfTriple->cellmlApiRdfTriple()->unassert();

            // Delete our RDF triple

            delete rdfTriple;
        }

        // Some RDF triples have been removed, so...

        mCellmlFile->setModified(true);
    }
}

//==============================================================================

void CellmlFileRdfTriples::remove(CellmlFileRdfTriple *pRdfTriple)
{
    // Call our generic remove function

    CellmlFileRdfTriples rdfTriples = CellmlFileRdfTriples(mCellmlFile);

    rdfTriples << pRdfTriple;

    removeRdfTriples(rdfTriples);
    // Note: yes, we must declare rdfTriples and add pRdfTriple to it before
    //       passing it to removeRdfTriples()...
}

//==============================================================================

void CellmlFileRdfTriples::remove(const QString &pMetadataId)
{
    // Call our generic remove function

    removeRdfTriples(contains(pMetadataId));
}

//==============================================================================

void CellmlFileRdfTriples::removeAll()
{
    // Call our generic remove function

    removeRdfTriples(*this);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
