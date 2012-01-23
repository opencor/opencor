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

    ComputerFunction * parseFunction(const QString &pFunction);
    ComputerEquation * parseEquation(const QString &pEquation);

    bool parseEquationRhs(ComputerFunction *pFunction,
                          ComputerEquation * &pRhsEquation);

private:
    ComputerScanner *mScanner;

    ComputerErrors mErrors;

    ComputerExternalFunctions mExternalFunctions;

    void reset(const QString &pFunction);

    bool parseFunctionParameters(ComputerFunction *pFunction);
    bool parseFunctionParameter(ComputerFunction *pFunction,
                                const bool &pNeeded = true);
    bool parseEquations(ComputerFunction *pFunction);
    bool parseReturn(ComputerFunction *pFunction);
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
