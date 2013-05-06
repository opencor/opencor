//==============================================================================
// CellML file RDF triple
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfilerdftriple.h"

//==============================================================================

#include <QRegularExpression>
#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

void CellMLFileRdfTriple::constructor(CellMLFile *pCellMLFile,
                                      iface::rdf_api::Triple *pRdfTriple,
                                      const Type &pType,
                                      const ModelQualifier &pModelQualifier,
                                      const BioQualifier &pBioQualifier,
                                      const QString &pResource,
                                      const QString &pId)
{
    mCellMLFile = pCellMLFile;

    mRdfTriple = pRdfTriple;

    mType = pType;

    mModelQualifier = pModelQualifier;
    mBioQualifier = pBioQualifier;

    mResource = pResource;
    mId = pId;
}

//==============================================================================

CellMLFileRdfTriple::CellMLFileRdfTriple(CellMLFile *pCellMLFile,
                                         iface::rdf_api::Triple *pRdfTriple)
{
    // Construct ourselves

    constructor(pCellMLFile, pRdfTriple, Unknown,
                ModelUnknown, BioUnknown, QString(), QString());

    // Retrieve the RDF triple's subject, predicate and object information

    ObjRef<iface::rdf_api::Resource> subject = pRdfTriple->subject();
    ObjRef<iface::rdf_api::Resource> predicate = pRdfTriple->predicate();
    ObjRef<iface::rdf_api::Node> object = pRdfTriple->object();

    mSubject   = new CellMLFileRdfTripleElement(subject);
    mPredicate = new CellMLFileRdfTripleElement(predicate);
    mObject    = new CellMLFileRdfTripleElement(object);

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
        if (!mPredicate->asString().compare(QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifierAsString(ModelQualifier(i)).remove(QRegularExpression("^model:"))))) {
            // It looks like we might be dealing with a model qualifier

            mType = BioModelsDotNetQualifier;

            mModelQualifier = ModelQualifier(i);

            break;
        }

    if (mType == Unknown)
        for (int i = FirstBioQualifier; i <= LastBioQualifier; ++i)
            if (!mPredicate->asString().compare(QString("http://biomodels.net/biology-qualifiers/%1").arg(bioQualifierAsString(BioQualifier(i)).remove(QRegularExpression("^bio:"))))){
                // It looks like we might be dealing with a model qualifier

                mType = BioModelsDotNetQualifier;

                mBioQualifier = BioQualifier(i);

                break;
            }

    if (mType == BioModelsDotNetQualifier)
        // We seem to be dealing with either a model or a bio(logy) qualifier,
        // so we try to decode its object which should be either a valid MIRIAM
        // URN or a valid identifiers.org URI

        if (!decodeTerm(mObject->asString(), mResource, mId)) {
            // The object is neither a valid MIRIAM URN nor a valid
            // identifiers.org URI which means that the RDF triple is not a
            // valid model/bio(logy) qualifier, so...

            mType = Unknown;

            mModelQualifier = ModelUnknown;
            mBioQualifier   = BioUnknown;
        }
}

//==============================================================================

CellMLFileRdfTriple::CellMLFileRdfTriple(CellMLFile *pCellMLFile,
                                         const QString pSubject,
                                         const ModelQualifier &pModelQualifier,
                                         const QString &pResource,
                                         const QString &pId)
{
    // Construct ourselves

    constructor(pCellMLFile, 0, BioModelsDotNetQualifier,
                pModelQualifier, BioUnknown, pResource, pId);

    // Create our RDF triple elements

    mSubject   = new CellMLFileRdfTripleElement(pSubject);
    mPredicate = new CellMLFileRdfTripleElement(QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifierAsString(pModelQualifier).remove(QRegularExpression("^model:"))));
    mObject    = new CellMLFileRdfTripleElement(QString("http://identifiers.org/%1/%2").arg(pResource, pId));
}

//==============================================================================

CellMLFileRdfTriple::CellMLFileRdfTriple(CellMLFile *pCellMLFile,
                                         const QString pSubject,
                                         const BioQualifier &pBioQualifier,
                                         const QString &pResource,
                                         const QString &pId)
{
    // Construct ourselves

    constructor(pCellMLFile, 0, BioModelsDotNetQualifier,
                ModelUnknown, pBioQualifier, pResource, pId);

    // Create our RDF triple elements

    mSubject   = new CellMLFileRdfTripleElement(pSubject);
    mPredicate = new CellMLFileRdfTripleElement(QString("http://biomodels.net/biology-qualifiers/%1").arg(bioQualifierAsString(pBioQualifier).remove(QRegularExpression("^bio:"))));
    mObject    = new CellMLFileRdfTripleElement(QString("http://identifiers.org/%1/%2").arg(pResource, pId));
}

//==============================================================================

CellMLFileRdfTriple::~CellMLFileRdfTriple()
{
    // Delete/release some internal objects

    delete mSubject;
    delete mPredicate;
    delete mObject;
}

//==============================================================================

iface::rdf_api::Triple * CellMLFileRdfTriple::rdfTriple() const
{
    // Return the RDF triple's CellML API RDFtriple

    return mRdfTriple;
}

//==============================================================================

void CellMLFileRdfTriple::setRdfTriple(iface::rdf_api::Triple *pRdfTriple)
{
    // Set the RDF triple's CellML API RDF triple

    mRdfTriple = 0;   // This will release the previous one, if any
    mRdfTriple = pRdfTriple;
}

//==============================================================================

CellMLFileRdfTripleElement * CellMLFileRdfTriple::subject() const
{
    // Return the RDF triple's subject

    return mSubject;
}

//==============================================================================

CellMLFileRdfTripleElement * CellMLFileRdfTriple::predicate() const
{
    // Return the RDF triple's predicate

    return mPredicate;
}

//==============================================================================

CellMLFileRdfTripleElement * CellMLFileRdfTriple::object() const
{
    // Return the RDF triple's object

    return mObject;
}

//==============================================================================

CellMLFileRdfTriple::Type CellMLFileRdfTriple::type() const
{
    // Return the RDF triple's type

    return mType;
}

//==============================================================================

QString CellMLFileRdfTriple::metadataId() const
{
    // Return the RDF triple's metadata id

    if (mSubject->type() == CellMLFileRdfTripleElement::UriReference)
        // The subject of our RDF triple is a URI reference, so we can retrieve
        // its metadata id

        return mSubject->uriReference().remove(QRegularExpression("^"+QRegularExpression::escape(mCellMLFile->uriBase())+"#?"));
    else
        // We don't recognise the subject of our RDF triple, so...

        return QString();
}

//==============================================================================

QString CellMLFileRdfTriple::qualifierAsString() const
{
    // Return the RDF triple's model or bio(logy) qualifier as a string

    return (mModelQualifier != ModelUnknown)?
                modelQualifierAsString():
                (mBioQualifier != BioUnknown)?
                    bioQualifierAsString():
                    "unknown";
}

//==============================================================================

CellMLFileRdfTriple::ModelQualifier CellMLFileRdfTriple::modelQualifier() const
{
    // Return the RDF triple's model qualifier

    return mModelQualifier;
}

//==============================================================================

QString CellMLFileRdfTriple::modelQualifierAsString() const
{
    // Return the RDF triple's model qualifier as a string

    return modelQualifierAsString(mModelQualifier);
}

//==============================================================================

QString CellMLFileRdfTriple::modelQualifierAsString(const ModelQualifier &pModelQualifier)
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

CellMLFileRdfTriple::BioQualifier CellMLFileRdfTriple::bioQualifier() const
{
    // Return the RDF triple's bio(logy) qualifier

    return mBioQualifier;
}

//==============================================================================

QString CellMLFileRdfTriple::bioQualifierAsString() const
{
    // Return the RDF triple's bio(logy) qualifier as a string

    return bioQualifierAsString(mBioQualifier);
}

//==============================================================================

QString CellMLFileRdfTriple::bioQualifierAsString(const BioQualifier &pBioQualifier)
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

QStringList CellMLFileRdfTriple::qualifiersAsStringList()
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

QString CellMLFileRdfTriple::resource() const
{
    // Return the RDF triple's resource

    return mResource;
}

//==============================================================================

QString CellMLFileRdfTriple::id() const
{
    // Return the RDF triple's id

    return mId;
}

//==============================================================================

CellMLFileRdfTriples::CellMLFileRdfTriples(CellMLFile *pCellMLFile) :
    mCellMLFile(pCellMLFile)
{
}

//==============================================================================

CellMLFileRdfTriple::Type CellMLFileRdfTriples::type() const
{
    // Return the type of the RDF triples
    // Note: if the RDF triples are of different types each then the overall
    //       type is considered to be unknown

    if (isEmpty()) {
        // There are no RDF triples, so...

        return CellMLFileRdfTriple::Empty;
    } else {
        // There is at least one RDF triple, so retrieve the subject and type of
        // the first RDF triple and consider its type as the default type for
        // all our RDF triples

        CellMLFileRdfTriple *rdfTriple = first();

        QString subject = rdfTriple->subject()->asString();
        CellMLFileRdfTriple::Type res = rdfTriple->type();

        // Go through the RDF triples and make sure that their type is
        // consistent with that of the first RDF triple

        foreach (CellMLFileRdfTriple *rdfTriple, *this)
            if (   rdfTriple->subject()->asString().compare(subject)
                || (rdfTriple->type() != res))
                // The subject and/or the type of the current RDF triple is
                // different from that of the first RDF triple, so...

                return CellMLFileRdfTriple::Unknown;

        // All of the RDF triples have the same type, so...

        return res;
    }
}

//==============================================================================

bool CellMLFileRdfTriple::decodeTerm(const QString &pTerm, QString &pResource,
                                     QString &pId)
{
    // Decode the term, based on whether it matches that of a MIRIAN URN or an
    // identifiers.org URI

    bool res = true;

    if (QRegularExpression("^urn:miriam:"+ResourceRegExp+":"+IdRegExp).match(pTerm).hasMatch()) {
        // The term is a MIRIAM URN, so retrieve its corresponding resource and
        // id

        QStringList miriamUrnList = pTerm.split(":");

        pResource = miriamUrnList[2];
        pId       = miriamUrnList[3].replace("%3A", ":");
    } else if (QRegularExpression("^http://identifiers.org/"+ResourceRegExp+"/#?"+IdRegExp).match(pTerm).hasMatch()) {
        // The term is an identifiers.org URI, so retrieve its corresponding
        // resource and id

        QString identifiersDotOrgUri = pTerm;
        // Note: the above is because pTerm is a const, so we can't directly use
        //       QString::remove() on it...
        QStringList identifiersDotOrgUriList = identifiersDotOrgUri.remove("http://identifiers.org/").split("/");

        pResource = identifiersDotOrgUriList[0];
        pId       = identifiersDotOrgUriList[1].replace("%3A", ":");

        // Remove the leading '#', if any, from the id
        // Note: semanticSBML does, for example, prepend a '#'...

        if (pId[0] == '#')
            pId = pId.right(pId.size()-1);
    } else {
        // Not a term we can recognise, so...

        pResource = "???";
        pId       = "???";

        res = false;

#ifdef QT_DEBUG
        qWarning("WARNING | %s:%d: '%s' is not a valid RDF term", __FILE__, __LINE__, "COUCOU");
#endif
    }

    // Return the result of our decoding

    return res;
}

//==============================================================================

void CellMLFileRdfTriples::recursiveContains(CellMLFileRdfTriples &pRdfTriples,
                                             CellMLFileRdfTriple *pRdfTriple) const
{
    // Add pRdfTriple to pRdfTriples

    pRdfTriples << pRdfTriple;

    // Recursively add all the RDF triples which subject matches pRdfTriple's
    // object

    foreach (CellMLFileRdfTriple *rdfTriple, *this)
        if (!rdfTriple->subject()->asString().compare(pRdfTriple->object()->asString()))
            recursiveContains(pRdfTriples, rdfTriple);
}

//==============================================================================

CellMLFileRdfTriples CellMLFileRdfTriples::contains(iface::cellml_api::CellMLElement *pElement) const
{
    // Return all the RDF triples which are directly or indirectly associated
    // with the given metadata id

    CellMLFileRdfTriples res = CellMLFileRdfTriples(mCellMLFile);
    QString cmetaId = QString::fromStdWString(pElement->cmetaId());

    foreach (CellMLFileRdfTriple *rdfTriple, *this)
        // Retrieve the RDF triple's subject so we can determine whether it's
        // from the group of RDF triples in which we are interested

        if (!cmetaId.compare(rdfTriple->metadataId()))
            // It's the correct metadata id, so add it to our list

            recursiveContains(res, rdfTriple);

    return res;
}

//==============================================================================

CellMLFileRdfTriple * CellMLFileRdfTriples::add(CellMLFileRdfTriple *pRdfTriple)
{
    // Add the given RDF triple

    append(pRdfTriple);

    // Create a CellML API version of the RDF triple

    ObjRef<iface::rdf_api::DataSource> dataSource = mCellMLFile->rdfDataSource();

    ObjRef<iface::rdf_api::Resource> subject = already_AddRefd<iface::rdf_api::Resource>(dataSource->getURIReference(pRdfTriple->subject()->asString().toStdWString()));
    ObjRef<iface::rdf_api::Resource> predicate = already_AddRefd<iface::rdf_api::Resource>(dataSource->getURIReference(pRdfTriple->predicate()->asString().toStdWString()));
    ObjRef<iface::rdf_api::Node> object = already_AddRefd<iface::rdf_api::Node>(dataSource->getURIReference(pRdfTriple->object()->asString().toStdWString()));

    subject->createTripleOutOf(predicate, object);

    // Keep track of the CellML API version of the RDF triple in pRdfTriple
    // Note: in debug mode, we also check that we actually managed to retrieve
    //       the CellML API version of our RDF triple...

    ObjRef<iface::rdf_api::Triple> rdfTriple = subject->getTripleOutOfByPredicateAndObject(predicate, object);

    Q_ASSERT(rdfTriple);

    pRdfTriple->setRdfTriple(rdfTriple);

    // An RDF triple has been added, so...

    mCellMLFile->setModified(true);

    return pRdfTriple;
}

//==============================================================================

void CellMLFileRdfTriples::removeRdfTriples(const CellMLFileRdfTriples &pRdfTriples)
{
    // Remove all the given RDF triples

    if (pRdfTriples.count()) {
        foreach (CellMLFileRdfTriple *rdfTriple, pRdfTriples) {
            // Remove the RDF triple

            removeOne(rdfTriple);

            // Remove the CellML API version of the RDF triple from its data
            // source

            rdfTriple->rdfTriple()->unassert();

            // Delete our RDF triple

            delete rdfTriple;
        }

        // Some RDF triples have been removed, so...

        mCellMLFile->setModified(true);
    }
}

//==============================================================================

void CellMLFileRdfTriples::remove(CellMLFileRdfTriple *pRdfTriple)
{
    // Call our generic remove function

    CellMLFileRdfTriples rdfTriples = CellMLFileRdfTriples(mCellMLFile);

    rdfTriples << pRdfTriple;

    removeRdfTriples(rdfTriples);
    // Note: yes, we must declare rdfTriples and add pRdfTriple to it before
    //       passing it to removeRdfTriples()...
}

//==============================================================================

void CellMLFileRdfTriples::remove(iface::cellml_api::CellMLElement *pElement)
{
    // Call our generic remove function

    removeRdfTriples(contains(pElement));
}

//==============================================================================

void CellMLFileRdfTriples::removeAll()
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
