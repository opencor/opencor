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
// CellML file RDF triple
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfilerdftriple.h"
#include "corecliutils.h"

//==============================================================================

#include <QRegularExpression>
#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                         iface::rdf_api::Triple *pRdfTriple,
                                         Type pType,
                                         ModelQualifier pModelQualifier,
                                         BioQualifier pBioQualifier,
                                         const QString &pResource,
                                         const QString &pId)
{
    mCellmlFile = pCellmlFile;

    mRdfTriple = pRdfTriple;

    mType = pType;

    mModelQualifier = pModelQualifier;
    mBioQualifier = pBioQualifier;

    mResource = pResource;
    mId = pId;
}

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                         iface::rdf_api::Triple *pRdfTriple) :
    CellmlFileRdfTriple(pCellmlFile, pRdfTriple, Type::Unknown,
                        ModelQualifier::ModelUnknown, BioQualifier::BioUnknown,
                        {}, {})
{
    // Retrieve the RDF triple's subject, predicate and object information

    mSubject = new CellmlFileRdfTripleElement(pRdfTriple->subject());
    mPredicate = new CellmlFileRdfTripleElement(pRdfTriple->predicate());
    mObject = new CellmlFileRdfTripleElement(pRdfTriple->object());

    // Determine the type of the RDF triple

    // Note: at this stage, we only recognise model and bio(logy) qualifiers as
    //       described at https://co.mbine.org/standards/qualifiers. This means
    //       that the predicate of the RDF triple must have one of the following
    //       two forms:
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

    static const QRegularExpression ModelRegEx = QRegularExpression("^model:");
    static const QRegularExpression BioRegEx = QRegularExpression("^bio:");

    for (int i = int(ModelQualifier::FirstModelQualifier); i <= int(ModelQualifier::LastModelQualifier); ++i) {
        if (mPredicate->asString() == QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifierAsString(ModelQualifier(i)).remove(ModelRegEx))) {
            // It looks like we might be dealing with a model qualifier

            mType = Type::BioModelsDotNetQualifier;

            mModelQualifier = ModelQualifier(i);

            break;
        }
    }

    if (mType == Type::Unknown) {
        for (int i = int(BioQualifier::FirstBioQualifier); i <= int(BioQualifier::LastBioQualifier); ++i) {
            if (mPredicate->asString() == QString("http://biomodels.net/biology-qualifiers/%1").arg(bioQualifierAsString(BioQualifier(i)).remove(BioRegEx))) {
                // It looks like we might be dealing with a model qualifier

                mType = Type::BioModelsDotNetQualifier;

                mBioQualifier = BioQualifier(i);

                break;
            }
        }
    }

    if (mType == Type::BioModelsDotNetQualifier) {
        // We seem to be dealing with either a model or a bio(logy) qualifier,
        // so try to decode its object, which should be either a valid MIRIAM
        // URN or a valid identifiers.org URI

        if (!decodeTerm(mObject->asString(), mResource, mId)) {
            // The object is neither a valid MIRIAM URN nor a valid
            // identifiers.org URI, which means that the RDF triple is not a
            // valid model/bio(logy) qualifier

            mType = Type::Unknown;

            mModelQualifier = ModelQualifier::ModelUnknown;
            mBioQualifier = BioQualifier::BioUnknown;
        }
    }
}

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                         const QString &pSubject,
                                         ModelQualifier pModelQualifier,
                                         const QString &pResource,
                                         const QString &pId) :
    CellmlFileRdfTriple(pCellmlFile, nullptr, Type::BioModelsDotNetQualifier,
                        pModelQualifier, BioQualifier::BioUnknown, pResource,
                        pId)
{
    // Create our RDF triple elements

    static const QRegularExpression ModelRegEx = QRegularExpression("^model:");

    mSubject = new CellmlFileRdfTripleElement(pSubject);
    mPredicate = new CellmlFileRdfTripleElement(QString("http://biomodels.net/model-qualifiers/%1").arg(modelQualifierAsString(pModelQualifier).remove(ModelRegEx)));
    mObject = new CellmlFileRdfTripleElement(QString("http://identifiers.org/%1/%2").arg(pResource,
                                                                                         pId));
}

//==============================================================================

CellmlFileRdfTriple::CellmlFileRdfTriple(CellmlFile *pCellmlFile,
                                         const QString &pSubject,
                                         BioQualifier pBioQualifier,
                                         const QString &pResource,
                                         const QString &pId) :
    CellmlFileRdfTriple(pCellmlFile, nullptr, Type::BioModelsDotNetQualifier,
                        ModelQualifier::ModelUnknown, pBioQualifier, pResource,
                        pId)
{
    // Create our RDF triple elements

    static const QRegularExpression BioRegEx = QRegularExpression("^bio:");

    mSubject = new CellmlFileRdfTripleElement(pSubject);
    mPredicate = new CellmlFileRdfTripleElement(QString("http://biomodels.net/biology-qualifiers/%1").arg(bioQualifierAsString(pBioQualifier).remove(BioRegEx)));
    mObject = new CellmlFileRdfTripleElement(QString("http://identifiers.org/%1/%2").arg(pResource,
                                                                                         pId));
}

//==============================================================================

CellmlFileRdfTriple::~CellmlFileRdfTriple()
{
    // Delete/release some internal objects

    delete mSubject;
    delete mPredicate;
    delete mObject;
}

//==============================================================================

iface::rdf_api::Triple * CellmlFileRdfTriple::rdfTriple() const
{
    // Return the RDF triple's CellML API RDFtriple

    return mRdfTriple;
}

//==============================================================================

void CellmlFileRdfTriple::setRdfTriple(iface::rdf_api::Triple *pRdfTriple)
{
    // Set the RDF triple's CellML API RDF triple

    mRdfTriple = nullptr; // This will release the previous one, if any
    mRdfTriple = pRdfTriple;
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
    // Return the RDF triple's metadata id, if its subject is a URI reference

    if (mSubject->type() == CellmlFileRdfTripleElement::Type::UriReference) {
        QString uriReference = mSubject->uriReference();
        int hashPosition = uriReference.lastIndexOf('#');

        if (hashPosition != -1) {
            return uriReference.right(uriReference.length()-hashPosition-1);
        }

        return {};
    }

    return {};
}

//==============================================================================

QString CellmlFileRdfTriple::qualifierAsString() const
{
    // Return the RDF triple's model or bio(logy) qualifier as a string

    return (mModelQualifier != ModelQualifier::ModelUnknown)?
                modelQualifierAsString():
                (mBioQualifier != BioQualifier::BioUnknown)?
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

QString CellmlFileRdfTriple::modelQualifierAsString(ModelQualifier pModelQualifier)
{
    // Return the given RDF triple's model qualifier as a string

    switch (pModelQualifier) {
    case ModelQualifier::ModelUnknown:
        return "model:unknown";
    case ModelQualifier::ModelIs:
        return "model:is";
    case ModelQualifier::ModelIsDerivedFrom:
        return "model:isDerivedFrom";
    case ModelQualifier::ModelIsDescribedBy:
        return "model:isDescribedBy";
    case ModelQualifier::ModelIsInstanceOf:
        return "model:isInstanceOf";
    case ModelQualifier::ModelHasInstance:
        return "model:hasInstance";
    }

    return "model:unknown";
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
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

QString CellmlFileRdfTriple::bioQualifierAsString(BioQualifier pBioQualifier)
{
    // Return the given RDF triple's bio(logy) qualifier as a string

    switch (pBioQualifier) {
    case BioQualifier::BioUnknown:
        return "bio:unknown";
    case BioQualifier::BioEncodes:
        return "bio:encodes";
    case BioQualifier::BioHasPart:
        return "bio:hasPart";
    case BioQualifier::BioHasProperty:
        return "bio:hasProperty";
    case BioQualifier::BioHasVersion:
        return "bio:hasVersion";
    case BioQualifier::BioIs:
        return "bio:is";
    case BioQualifier::BioIsDescribedBy:
        return "bio:isDescribedBy";
    case BioQualifier::BioIsEncodedBy:
        return "bio:isEncodedBy";
    case BioQualifier::BioIsHomologTo:
        return "bio:isHomologTo";
    case BioQualifier::BioIsPartOf:
        return "bio:isPartOf";
    case BioQualifier::BioIsPropertyOf:
        return "bio:isPropertyOf";
    case BioQualifier::BioIsVersionOf:
        return "bio:isVersionOf";
    case BioQualifier::BioOccursIn:
        return "bio:occursIn";
    case BioQualifier::BioHasTaxon:
        return "bio:hasTaxon";
    }

    return "bio:unknown";
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

QStringList CellmlFileRdfTriple::qualifiersAsStringList()
{
    // Return (in alphabetical order) all the qualifiers which we support

    return { bioQualifierAsString(BioQualifier::BioEncodes),
             bioQualifierAsString(BioQualifier::BioHasPart),
             bioQualifierAsString(BioQualifier::BioHasProperty),
             bioQualifierAsString(BioQualifier::BioHasVersion),
             bioQualifierAsString(BioQualifier::BioIs),
             bioQualifierAsString(BioQualifier::BioIsDescribedBy),
             bioQualifierAsString(BioQualifier::BioIsEncodedBy),
             bioQualifierAsString(BioQualifier::BioIsHomologTo),
             bioQualifierAsString(BioQualifier::BioIsPartOf),
             bioQualifierAsString(BioQualifier::BioIsPropertyOf),
             bioQualifierAsString(BioQualifier::BioIsVersionOf),
             bioQualifierAsString(BioQualifier::BioOccursIn),
             bioQualifierAsString(BioQualifier::BioHasTaxon),
             modelQualifierAsString(ModelQualifier::ModelIs),
             modelQualifierAsString(ModelQualifier::ModelIsDerivedFrom),
             modelQualifierAsString(ModelQualifier::ModelIsDescribedBy),
             modelQualifierAsString(ModelQualifier::ModelIsInstanceOf),
             modelQualifierAsString(ModelQualifier::ModelHasInstance) };
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
        return CellmlFileRdfTriple::Type::Empty;
    }

    // There is at least one RDF triple, so retrieve the subject and type of the
    // first RDF triple and consider its type as the default type for all our
    // RDF triples

    CellmlFileRdfTriple *firstRdfTriple = first();

    QString subject = firstRdfTriple->subject()->asString();
    CellmlFileRdfTriple::Type res = firstRdfTriple->type();

    // Go through the RDF triples and make sure that their type is
    // consistent with that of the first RDF triple

    for (auto rdfTriple : *this) {
        if (   (rdfTriple->subject()->asString() != subject)
            || (rdfTriple->type() != res)) {
            // The subject and/or the type of the current RDF triple is
            // different from that of the first RDF triple, so consider the
            // overall type of the RDF triples to be unknown

            return CellmlFileRdfTriple::Type::Unknown;
        }
    }

    return res;
}

//==============================================================================

bool CellmlFileRdfTriple::decodeTerm(const QString &pTerm, QString &pResource,
                                     QString &pId)
{
    // Decode the term, based on whether it matches that of a MIRIAN URN or an
    // identifiers.org URI

    static const QRegularExpression MiriamUrnRegEx = QRegularExpression("^urn:miriam:"+ResourceRegEx+":"+IdRegEx);
    static const QRegularExpression IdentifierDotOrgRegEx = QRegularExpression("^http://identifiers.org/"+ResourceRegEx+"/#?"+IdRegEx);

    if (MiriamUrnRegEx.match(pTerm).hasMatch()) {
        // The term is a MIRIAM URN, so retrieve its corresponding resource and
        // id

        QStringList miriamUrnList = pTerm.split(':');

        pResource = Core::stringFromPercentEncoding(miriamUrnList[2]);
        pId = Core::stringFromPercentEncoding(miriamUrnList[3]);

        return true;
    }

    if (IdentifierDotOrgRegEx.match(pTerm).hasMatch()) {
        // The term is an identifiers.org URI, so retrieve its corresponding
        // resource and id

        QString identifiersDotOrgUri = pTerm;
        // Note: the above is because pTerm is a const, so we can't directly use
        //       QString::remove() on it...
        QStringList identifiersDotOrgUriList = identifiersDotOrgUri.remove("http://identifiers.org/").split('/');

        pResource = Core::stringFromPercentEncoding(identifiersDotOrgUriList[0]);
        pId = Core::stringFromPercentEncoding(identifiersDotOrgUriList[1]);

        return true;
    }

    // Not a term we can recognise

    pResource = "???";
    pId = "???";

    return false;
}

//==============================================================================

void CellmlFileRdfTriples::recursiveAssociatedWith(CellmlFileRdfTriples &pRdfTriples,
                                                   CellmlFileRdfTriple *pRdfTriple) const
{
    // Add pRdfTriple to pRdfTriples, but only if it's not already part of
    // pRdfTriples
    // Note: indeed, a given RDF triple may be referenced more than once...

    if (pRdfTriples.contains(pRdfTriple)) {
        return;
    }

    pRdfTriples << pRdfTriple;

    // Recursively add all the RDF triples, which subject matches that of
    // pRdfTriple's object

    QString rdfTripleObject = pRdfTriple->object()->asString();

    for (auto rdfTriple : *this) {
        if (rdfTriple->subject()->asString() == rdfTripleObject) {
            recursiveAssociatedWith(pRdfTriples, rdfTriple);
        }
    }
}

//==============================================================================

CellmlFileRdfTriples CellmlFileRdfTriples::associatedWith(iface::cellml_api::CellMLElement *pElement) const
{
    // Return all the RDF triples that are directly or indirectly associated
    // with the given element's metadata id

    CellmlFileRdfTriples res = CellmlFileRdfTriples(mCellmlFile);
    QString cmetaId = QString::fromStdWString(pElement->cmetaId());

    for (auto rdfTriple : *this) {
        // Retrieve the RDF triple's subject so we can determine whether it's
        // from the group of RDF triples in which we are interested

        if (cmetaId == rdfTriple->metadataId()) {
            // It's the correct metadata id, so add it to our list

            recursiveAssociatedWith(res, rdfTriple);
        }
    }

    return res;
}

//==============================================================================

CellmlFileRdfTriple * CellmlFileRdfTriples::add(CellmlFileRdfTriple *pRdfTriple)
{
    // Add the given RDF triple

    append(pRdfTriple);

    // Create a CellML API version of the RDF triple

    ObjRef<iface::rdf_api::DataSource> dataSource = mCellmlFile->rdfDataSource();
    ObjRef<iface::rdf_api::Resource> subject = already_AddRefd<iface::rdf_api::Resource>(dataSource->getURIReference(pRdfTriple->subject()->asString().toStdWString()));
    ObjRef<iface::rdf_api::Resource> predicate = already_AddRefd<iface::rdf_api::Resource>(dataSource->getURIReference(pRdfTriple->predicate()->asString().toStdWString()));
    ObjRef<iface::rdf_api::Node> object = already_AddRefd<iface::rdf_api::Node>(dataSource->getURIReference(pRdfTriple->object()->asString().toStdWString()));

    subject->createTripleOutOf(predicate, object);

    // Keep track of the CellML API version of the RDF triple in pRdfTriple

    pRdfTriple->setRdfTriple(subject->getTripleOutOfByPredicateAndObject(predicate, object));

    // An RDF triple has been added, so update the CellML file's modified status

    updateCellmlFileModifiedStatus();

    return pRdfTriple;
}

//==============================================================================

bool CellmlFileRdfTriples::removeRdfTriples(const CellmlFileRdfTriples &pRdfTriples)
{
    // Remove all the given RDF triples

    if (!pRdfTriples.isEmpty()) {
        for (auto rdfTriple : pRdfTriples) {
            // Remove the RDF triple

            removeOne(rdfTriple);

            // Remove the CellML API version of the RDF triple from its data
            // source

            rdfTriple->rdfTriple()->unassert();

            // Delete our RDF triple

            delete rdfTriple;
        }

        // Some RDF triples have been removed, so update the CellML file's
        // modified status

        updateCellmlFileModifiedStatus();

        return true;
    }

    return false;
}

//==============================================================================

bool CellmlFileRdfTriples::remove(CellmlFileRdfTriple *pRdfTriple)
{
    // Call our generic remove function

    CellmlFileRdfTriples rdfTriples = CellmlFileRdfTriples(mCellmlFile);

    rdfTriples << pRdfTriple;

    return removeRdfTriples(rdfTriples);
}

//==============================================================================

bool CellmlFileRdfTriples::remove(iface::cellml_api::CellMLElement *pElement)
{
    // Call our generic remove function

    return removeRdfTriples(associatedWith(pElement));
}

//==============================================================================

bool CellmlFileRdfTriples::removeAll()
{
    // Call our generic remove function

    return removeRdfTriples(*this);
}

//==============================================================================

QStringList CellmlFileRdfTriples::asStringList() const
{
    // Return the RDF triples as a list of sorted strings

    QStringList res;

    for (auto rdfTriple : *this) {
        res << QString("%1|%2|%3").arg(rdfTriple->subject()->asString(),
                                       rdfTriple->predicate()->asString(),
                                       rdfTriple->object()->asString());
    }

    res.sort(Qt::CaseInsensitive);

    return res;
}

//==============================================================================

void CellmlFileRdfTriples::updateOriginalRdfTriples()
{
    // Keep track of our current RDF triples, which will be considered as our
    // original RDF triples, so we can determine whether a CellML file should be
    // considered modified (see updateCellmlFileModifiedStatus())

    mOriginalRdfTriples = asStringList();
}

//==============================================================================

void CellmlFileRdfTriples::updateCellmlFileModifiedStatus()
{
    // Determine whether our CellML file should be considered modified based on
    // whether our current RDF triples are the same as our original ones

    if (count() != mOriginalRdfTriples.count()) {
        mCellmlFile->setModified(true);
    } else {
        mCellmlFile->setModified(asStringList() != mOriginalRdfTriples);
    }
}

//==============================================================================

} // namespace CellMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
