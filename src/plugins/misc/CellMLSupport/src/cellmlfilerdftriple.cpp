//==============================================================================
// CellML file RDF triple
//==============================================================================

#include "cellmlfilerdftriple.h"

//==============================================================================

#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(iface::rdf_api::Triple *pRdfTriple) :
    mType(Unknown),
    mModelQualifierType(ModelUnknown),
    mBioQualifierType(BioUnknown),
    mMiriamUrn(QString())
{
    // Retrieve the RDF triple's subject, predicate and object information

    ObjRef<iface::rdf_api::Resource> subject = pRdfTriple->subject();
    ObjRef<iface::rdf_api::Resource> predicate = pRdfTriple->predicate();
    ObjRef<iface::rdf_api::Node> object = pRdfTriple->object();

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
    //       where <xxx> and <yyy> are one of the values in modelQualifiers and
    //       bioQualifiers below. The object of the RDF triple must also have
    //       the following form:
    //
    //          urn:miriam:<collection>:<identifier>
    //
    //       For example:
    //
    //          urn:miriam:uniprot:P62158

    QStringList modelQualifiers = QStringList() << "is" << "isDerivedFrom"
                                                << "isDescribedBy";
    QStringList bioQualifiers = QStringList() << "encodes" << "hasPart"
                                              << "hasProperty" << "hasVersion"
                                              << "is" << "isDescribedBy"
                                              << "isEncodedBy" << "isHomologTo"
                                              << "isPartOf" << "isPropertyOf"
                                              << "isVersionOf" << "occursIn"
                                              << "hasTaxon";

    for (int i = 0, iMax = modelQualifiers.count(); i < iMax; ++i)
        if (!mPredicate->asString().compare(QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifiers[i]))) {
            // It looks like we might be dealing with a model qualifier

            mType = BioModelNetQualifier;

            mModelQualifierType = (ModelQualifierType) (i+1);

            break;
        }

    if (mType == Unknown)
        for (int i = 0, iMax = bioQualifiers.count(); i < iMax; ++i)
            if (!mPredicate->asString().compare(QString("http://biomodels.net/biology-qualifiers/%1").arg(bioQualifiers[i]))){
                // It looks like we might be dealing with a model qualifier

                mType = BioModelNetQualifier;

                mBioQualifierType = (BioQualifierType) (i+1);

                break;
            }

    if (mType == BioModelNetQualifier) {
        // We seem to be dealing with either a model or a bio(logy) qualifier,
        // so check whether its object is either a valid MIRIAM URN or a valid
        // identifiers.org URI

        QString objectAsString = mObject->asString();

        if (QRegExp("^urn:miriam:[a-zA-Z0-9\\._%]+:[a-zA-Z0-9\\._%]+").exactMatch(objectAsString)) {
            // The object is a valid MIRIAM URN, so...

            mMiriamUrn = objectAsString;
        } else if (QRegExp("^http://identifiers.org/[a-zA-Z0-9\\._:]+/[a-zA-Z0-9\\._:]+").exactMatch(objectAsString)) {
            // The object is a valid identifiers.org URI, so retrieve its
            // corresponding MIRIAM URN

            QStringList uriParts = objectAsString.remove("http://identifiers.org/").replace(":", "%3A").split("/");

            mMiriamUrn = "urn:miriam:"+uriParts[0]+":"+uriParts[1];
        } else {
            // The object is neither a valid MIRIAM URN nor a valid
            // identifiers.org URI which means that the RDF triple is not a
            // valid model/bio(logy) qualifier, so...

            mType = Unknown;

            mModelQualifierType = ModelUnknown;
            mBioQualifierType   = BioUnknown;
        }
    }
}

//==============================================================================

CellmlFileRdfTriple::~CellmlFileRdfTriple()
{
    // Delete some internal objects

    delete mSubject;
    delete mPredicate;
    delete mObject;
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

CellmlFileRdfTriple::ModelQualifierType CellmlFileRdfTriple::modelQualifierType() const
{
    // Return the RDF triple's model qualifier type

    return mModelQualifierType;
}

//==============================================================================

QString CellmlFileRdfTriple::modelQualifierTypeAsString() const
{
    // Return the RDF triple's model qualifier type as a string

    switch (mModelQualifierType) {
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

CellmlFileRdfTriple::BioQualifierType CellmlFileRdfTriple::bioQualifierType() const
{
    // Return the RDF triple's bio(logy) qualifier type

    return mBioQualifierType;
}

//==============================================================================

QString CellmlFileRdfTriple::bioQualifierTypeAsString() const
{
    // Return the RDF triple's bio(logy) qualifier type as a string

    switch (mBioQualifierType) {
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

QString CellmlFileRdfTriple::miriamUrn() const
{
    // Return the RDF triple's MIRIAM URN

    return mMiriamUrn;
}

//==============================================================================

QString CellmlFileRdfTriple::resource() const
{
    // Return the RDF triple's resource from its MIRIAM URN

    if (!mMiriamUrn.isEmpty())
        return QString(mMiriamUrn.split(":").at(2)).replace("%3A", ":");
    else
        return QString();
}

//==============================================================================

QString CellmlFileRdfTriple::id() const
{
    // Return the RDF triple's id from its MIRIAM URN

    if (!mMiriamUrn.isEmpty())
        return QString(mMiriamUrn.split(":").at(3)).replace("%3A", ":");
    else
        return QString();
}

//==============================================================================

CellmlFileRdfTriple::Type CellmlFileRdfTriples::type() const
{
    // Return the type of the RDF triples
    // Note: if the RDF triples are of different types each then the overall
    //       type is considered to be unknown

    if (isEmpty()) {
        // There are no RDF triples, so...

        return CellmlFileRdfTriple::Unknown;
    } else {
        // There is at least one RDF triple, so retrieve the subject and type of
        // the first RDF triple and consider its type as the default type for
        // all our RDF triples

        CellmlFileRdfTriple *crtRdfTriple = first();

        QString subject = crtRdfTriple->subject()->asString();
        CellmlFileRdfTriple::Type res = crtRdfTriple->type();

        // Go through the remaining RDF triples and make sure that their type
        // is consistent with that of the first one

        for (int i = 2, iMax = count(); i < iMax; ++i) {
            crtRdfTriple = at(i);

            if (   crtRdfTriple->subject()->asString().compare(subject)
                || (crtRdfTriple->type() != res))
                // The subject and/or the type of the current RDF triple is
                // different from that of the first RDF triple, so...

                return CellmlFileRdfTriple::Unknown;
        }

        // All of the RDF triples have the same type, so...

        return res;
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
