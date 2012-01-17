//==============================================================================
// Computer issue class
//==============================================================================

#ifndef COMPUTERISSUE_H
#define COMPUTERISSUE_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class COMPUTER_EXPORT ComputerIssue
{
public:
    explicit ComputerIssue(const QString &pMessage,
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

typedef QList<ComputerIssue> ComputerIssues;

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
