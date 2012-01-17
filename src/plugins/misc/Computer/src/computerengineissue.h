//==============================================================================
// Computer engine issue class
//==============================================================================

#ifndef COMPUTERENGINEISSUE_H
#define COMPUTERENGINEISSUE_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class COMPUTER_EXPORT ComputerEngineIssue
{
public:
    explicit ComputerEngineIssue(const QString &pMessage,
                                 const int &pLine = 0, const int &pColumn = 0,
                                 const QString &pExtraInformation = QString());

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

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
