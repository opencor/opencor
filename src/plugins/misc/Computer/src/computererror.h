//==============================================================================
// Computer error class
//==============================================================================

#ifndef COMPUTERERROR_H
#define COMPUTERERROR_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class COMPUTER_EXPORT ComputerError
{
public:
    explicit ComputerError(const QString &pMessage = QString(),
                           const int &pLine = 0, const int &pColumn = 0);

    bool isEmpty() const;

    QString message() const;
    QString formattedMessage() const;
    int line() const;
    int column() const;

private:
    QString mMessage;
    int mLine;
    int mColumn;
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
