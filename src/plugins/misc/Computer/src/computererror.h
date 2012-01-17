//==============================================================================
// Computer error class
//==============================================================================

#ifndef COMPUTERERROR_H
#define COMPUTERERROR_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class COMPUTER_EXPORT ComputerError
{
public:
    explicit ComputerError(const QString &pMessage,
                           const int &pLine = 0, const int &pColumn = 0,
                           const QString &pExtraInformation = QString());

    bool isEmpty() const;

    QString message() const;
    QString formattedMessage() const;
    int line() const;
    int column() const;
    QString extraInformation() const;

private:
    QString mMessage;
    int mLine;
    int mColumn;
    QString mExtraInformation;
};

//==============================================================================

typedef QList<ComputerError> ComputerErrors;

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
