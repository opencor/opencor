//==============================================================================
// Computer issue class
//==============================================================================

#include "computerissue.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerIssue::ComputerIssue(const QString &pMessage,
                             const int &pLine, const int &pColumn,
                             const QString &pExtraInformation) :
    mMessage(pMessage),
    mLine(pLine),
    mColumn(pColumn),
    mExtraInformation(pExtraInformation)
{
}

//==============================================================================

bool ComputerIssue::isEmpty() const
{
    // Return whether the issue is empty

    return mMessage.isEmpty() && !mLine && !mColumn && mExtraInformation.isEmpty();
}

//==============================================================================

QString ComputerIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString ComputerIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage.left(1).toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

int ComputerIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int ComputerIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString ComputerIssue::extraInformation() const
{
    // Return the issue's extra information

    return mExtraInformation;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
