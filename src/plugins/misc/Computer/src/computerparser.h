//==============================================================================
// Computer parser class
//==============================================================================

#ifndef COMPUTERPARSER_H
#define COMPUTERPARSER_H

//==============================================================================

#include "computerfunction.h"
#include "computererror.h"
#include "computerscanner.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class ComputerParser : public QObject
{
    Q_OBJECT

public:
    explicit ComputerParser();
    ~ComputerParser();

    ComputerScanner * scanner();

    ComputerErrors errors();

    void addError(const QString &pMessage,
                  const bool &pExpectedMessage = true,
                  const bool &pUseCurrentToken = true,
                  const ComputerScannerToken &pOtherToken = ComputerScannerToken(),
                  const QString &pExtraInformation = QString());

    ComputerFunction parseFunction(const QString &pFunction);
    bool parseEquationRhs(ComputerFunction &pFunction);

private:
    ComputerScanner *mScanner;

    ComputerErrors mErrors;

    ComputerExternalFunctions mExternalFunctions;

    ComputerFunction invalidFunction();

    bool parseFunctionParameter(ComputerFunction &pFunction,
                                const bool &pNeeded = true);
    bool parseFunctionParameters(ComputerFunction &pFunction);
    bool parseEquations(ComputerFunction &pFunction);
    bool parseReturn(ComputerFunction &pFunction);
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
