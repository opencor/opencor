//==============================================================================
// CellML file issue
//==============================================================================

#include "cellmlfileissue.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileIssue::CellmlFileIssue(const Type &pType, const QString &pMessage,
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

CellmlFileIssue::Type CellmlFileIssue::type() const
{
    // Return the issue's type

    return mType;
}

//==============================================================================

QString CellmlFileIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString CellmlFileIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage.at(0).toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

int CellmlFileIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int CellmlFileIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString CellmlFileIssue::importedFile() const
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
