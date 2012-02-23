//==============================================================================
// Computer error class
//==============================================================================

#include "computererror.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerError::ComputerError(const QString &pMessage,
                             const int &pLine, const int &pColumn) :
    mMessage(pMessage),
    mLine(pLine),
    mColumn(pColumn)
{
}

//==============================================================================

bool ComputerError::isEmpty() const
{
    // Return whether the error is empty

    return mMessage.isEmpty() && !mLine && !mColumn;
}

//==============================================================================

QString ComputerError::message() const
{
    // Return the error's message

    return mMessage;
}

//==============================================================================

QString ComputerError::formattedMessage() const
{
    // Return the error's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage.left(1).toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

int ComputerError::line() const
{
    // Return the error's line

    return mLine;
}

//==============================================================================

int ComputerError::column() const
{
    // Return the error's column

    return mColumn;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
