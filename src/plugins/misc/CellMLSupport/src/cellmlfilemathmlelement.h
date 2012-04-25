//==============================================================================
// CellML file MathML element
//==============================================================================

#ifndef CELLMLFILEMATHMLELEMENT_H
#define CELLMLFILEMATHMLELEMENT_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

#include "IfaceMathML_content_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileMathmlElement
{
public:
    explicit CellmlFileMathmlElement(iface::mathml_dom::MathMLElement *pMathmlElement);

    QString className() const;
    QString mathElementStyle() const;
    QString id() const;
    QString xref() const;
    QString href() const;

private:
    QString mClassName;
    QString mMathElementStyle;
    QString mId;
    QString mXref;
    QString mHref;
};

//==============================================================================

typedef QList<CellmlFileMathmlElement *> CellmlFileMathmlElements;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
