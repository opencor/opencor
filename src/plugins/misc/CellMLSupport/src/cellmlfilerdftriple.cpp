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
    mBiologyQualifierType(BiologyUnknown),
    mQualifierUrl(QUrl())
{
    // Retrieve the RDF triple's subject, predicate and object information

    ObjRef<iface::rdf_api::Resource> subject = pRdfTriple->subject();
    ObjRef<iface::rdf_api::Resource> predicate = pRdfTriple->predicate();
    ObjRef<iface::rdf_api::Node> object = pRdfTriple->object();

    mSubject   = new CellmlFileRdfTripleElement(subject);
    mPredicate = new CellmlFileRdfTripleElement(predicate);
    mObject    = new CellmlFileRdfTripleElement(object);

    // Determine the type of the RDF triple

    // Note: at this stage, we only recognise model and biology qualifiers as
    //       described at http://biomodels.net/qualifiers/. This means that the
    //       predicate of the RDF triple must have one of the following two
    //       forms:
    //
    //          http://biomodels.net/model-qualifiers/<xxx>
    //          http://biomodels.net/biology-qualifiers/<yyy>
    //
    //       where <xxx> and <yyy> are one of the values in modelQualifiers and
    //       biologyQualifiers below. The object of the RDF triple must also
    //       have the following form:
    //
    //          urn:miriam:<collection>:<identifier>
    //
    //       For example:
    //
    //          urn:miriam:uniprot:P62158

    QStringList modelQualifiers = QStringList() << "is" << "isDerivedFrom"
                                                << "isDescribedBy";
    QStringList biologyQualifiers = QStringList() << "encodes" << "hasPart"
                                                  << "hasProperty"
                                                  << "hasVersion" << "is"
                                                  << "isDescribedBy"
                                                  << "isEncodedBy"
                                                  << "isHomologTo" << "isPartOf"
                                                  << "isPropertyOf"
                                                  << "isVersionOf" << "occursIn"
                                                  << "hasTaxon";

    for (int i = 0, iMax = modelQualifiers.count(); i < iMax; ++i)
        if (!mPredicate->asString().compare(QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifiers[i]))) {
            // It looks like we might be dealing with a model qualifier

            mType = ModelQualifier;

            mModelQualifierType = (ModelQualifierType) (i+1);

            break;
        }

    if (mType == Unknown)
        for (int i = 0, iMax = biologyQualifiers.count(); i < iMax; ++i)
            if (!mPredicate->asString().compare(QString("http://biomodels.net/biology-qualifiers/%1").arg(biologyQualifiers[i]))){
                // It looks like we might be dealing with a model qualifier

                mType = BiologyQualifier;

                mBiologyQualifierType = (BiologyQualifierType) (i+1);

                break;
            }

    if ((mType == ModelQualifier) || (mType == BiologyQualifier)) {
        // We seem to be dealing with either a model or a biology qualifier, so
        // check that its object is a valid MIRIAM URN

        if (QRegExp("^urn:miriam:[a-zA-Z0-9\\._%]+:[a-zA-Z0-9\\._%]+").exactMatch(mObject->asString())) {
            // The object is a valid MIRIAM URN, so retrieve its corresponding
            // URL

//---GRY--- TEMPORARY QUALIFIER URL... WE NEED TO RETRIEVE THE PROPER QUALIFIER
//          URL USING http://www.ebi.ac.uk/miriamws/main/rest/resolve/XXX

            mQualifierUrl = QUrl("http://www.opencor.ws/");
        } else {
            // The object is not a valid MIRIAM URN which means that the RDF
            // triple is not a valid model/biology qualifier

            mType = Unknown;

            mModelQualifierType   = ModelUnknown;
            mBiologyQualifierType = BiologyUnknown;
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
        return QObject::tr("is");
    case ModelIsDerivedFrom:
        return QObject::tr("is derived from");
    case ModelIsDescribedBy:
        return QObject::tr("is described by");
    default:
        // ModelUnknown

        return "???";
    }
}

//==============================================================================

CellmlFileRdfTriple::BiologyQualifierType CellmlFileRdfTriple::biologyQualifierType() const
{
    // Return the RDF triple's biology qualifier type

    return mBiologyQualifierType;
}

//==============================================================================

QString CellmlFileRdfTriple::biologyQualifierTypeAsString() const
{
    // Return the RDF triple's biology qualifier type as a string

    switch (mBiologyQualifierType) {
    case BiologyEncodes:
        return QObject::tr("encodes");
    case BiologyHasPart:
        return QObject::tr("has part");
    case BiologyHasProperty:
        return QObject::tr("has property");
    case BiologyHasVersion:
        return QObject::tr("has version");
    case BiologyIs:
        return QObject::tr("is");
    case BiologyIsDescribedBy:
        return QObject::tr("is described by");
    case BiologyIsEncodedBy:
        return QObject::tr("is encoded by");
    case BiologyIsHomologTo:
        return QObject::tr("is homolog to");
    case BiologyIsPartOf:
        return QObject::tr("is part of");
    case BiologyIsPropertyOf:
        return QObject::tr("is property of");
    case BiologyIsVersionOf:
        return QObject::tr("is version of");
    case BiologyOccursIn:
        return QObject::tr("occurs in");
    case BiologyHasTaxon:
        return QObject::tr("has taxon");
    default:
        // BiologyUnknown

        return "???";
    }
}

//==============================================================================

QUrl CellmlFileRdfTriple::qualifierUrl() const
{
    // Return the RDF triple's qualifier URL

    return mQualifierUrl;
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
