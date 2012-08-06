//==============================================================================
// CellML file MathML element
//==============================================================================
//---GRY--- NEED TO THINK OF A WAY TO KEEP TRACK OF CHANGES TO A MathML ELEMENT,
//          I.E. BE ABLE TO TELL WHETHER A CellML FILE HAS BEEN MODIFIED...
//==============================================================================

#include "cellmlfilemathmlelement.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileMathmlElement::CellmlFileMathmlElement(iface::mathml_dom::MathMLElement *pMathmlElement) :
    mClassName(QString::fromStdWString(pMathmlElement->className())),
    mMathElementStyle(QString::fromStdWString(pMathmlElement->mathElementStyle())),
    mId(QString::fromStdWString(pMathmlElement->id())),
    mXref(QString::fromStdWString(pMathmlElement->xref())),
    mHref(QString::fromStdWString(pMathmlElement->href()))
{
}

//==============================================================================

QString CellmlFileMathmlElement::className() const
{
    // Return the MathML element's class name

    return mClassName;
}

//==============================================================================

QString CellmlFileMathmlElement::mathElementStyle() const
{
    // Return the MathML element's math element style

    return mMathElementStyle;
}

//==============================================================================

QString CellmlFileMathmlElement::id() const
{
    // Return the MathML element's id

    return mId;
}

//==============================================================================

QString CellmlFileMathmlElement::xref() const
{
    // Return the MathML element's xref

    return mXref;
}

//==============================================================================

QString CellmlFileMathmlElement::href() const
{
    // Return the MathML element's href

    return mHref;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
