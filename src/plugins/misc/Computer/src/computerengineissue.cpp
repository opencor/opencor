//==============================================================================
// Computer engine issue class
//==============================================================================

#include "computerengineissue.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEngineIssue::ComputerEngineIssue(const QString &pMessage,
                                         const int &pLine, const int &pColumn,
                                         const QString &pExtraInformation) :
    mMessage(pMessage),
    mLine(pLine),
    mColumn(pColumn),
    mExtraInformation(pExtraInformation)
{
}

//==============================================================================

QString ComputerEngineIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString ComputerEngineIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage.left(1).toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

int ComputerEngineIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int ComputerEngineIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString ComputerEngineIssue::extraInformation() const
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
