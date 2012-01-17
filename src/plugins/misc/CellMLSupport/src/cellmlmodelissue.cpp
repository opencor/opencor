//==============================================================================
// CellML model issue
//==============================================================================

#include "cellmlmodelissue.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlModelIssue::CellmlModelIssue(const Type &pType, const QString &pMessage,
                                   const uint32_t &pLine,
                                   const uint32_t &pColumn,
                                   const QString &pImportedModel) :
    mType(pType),
    mMessage(pMessage),
    mLine(pLine),
    mColumn(pColumn),
    mImportedModel(pImportedModel)
{
}

//==============================================================================

CellmlModelIssue::Type CellmlModelIssue::type() const
{
    // Return the issue's type

    return mType;
}

//==============================================================================

QString CellmlModelIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString CellmlModelIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage.left(1).toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

uint32_t CellmlModelIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

uint32_t CellmlModelIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString CellmlModelIssue::importedModel() const
{
    // Return the issue's imported model

    return mImportedModel;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
