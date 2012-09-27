//==============================================================================
// Computer error class
//==============================================================================

#include "computererror.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerError::ComputerError(const QString &pMessage) :
    mMessage(pMessage)
{
}

//==============================================================================

bool ComputerError::isEmpty() const
{
    // Return whether the error is empty

    return mMessage.isEmpty();
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

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
