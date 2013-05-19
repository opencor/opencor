//==============================================================================
// CellML file issue
//==============================================================================

#include "cellmlfileissue.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellMLFileIssue::CellMLFileIssue(const Type &pType, const QString &pMessage,
                                 const int &pLine, const int &pColumn,
                                 const QString &pImportedFile) :
    mType(pType),
    mMessage(pMessage),
    mLine(pLine),
    mColumn(pColumn),
    mImportedFile(pImportedFile)
{
}

//==============================================================================

CellMLFileIssue::Type CellMLFileIssue::type() const
{
    // Return the issue's type

    return mType;
}

//==============================================================================

QString CellMLFileIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString CellMLFileIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage[0].toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

int CellMLFileIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int CellMLFileIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString CellMLFileIssue::importedFile() const
{
    // Return the issue's imported file

    return mImportedFile;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
