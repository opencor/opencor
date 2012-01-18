//==============================================================================
// Computer parser class
//==============================================================================
// Note #1: after retrieving/parsing something, we must get ready for the next
//          task and this means getting the next token. Indeed, doing so means
//          that the next task doesn't have to worry about whether the current
//          token is the correct one or not...
// Note #2: the parser is based on the YACC description of the ANSI C language:
//          http://www.lysator.liu.se/c/ANSI-C-grammar-y.html
//==============================================================================


#include "computerparser.h"
#include "computerscanner.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerParser::ComputerParser() :
    mErrors(ComputerErrors()),
    mExternalFunctions(ComputerExternalFunctions())
{
    // Create a scanner

    mScanner = new ComputerScanner();
}

//==============================================================================

ComputerParser::~ComputerParser()
{
    // Delete some internal objects

    delete mScanner;
}

//==============================================================================

ComputerScanner * ComputerParser::scanner()
{
    // Return the computer parser's scanner

    return mScanner;
}

//==============================================================================

ComputerErrors ComputerParser::errors()
{
    // Return the computer parser's error(s)

    return mErrors;
}

//==============================================================================

void ComputerParser::addError(const QString &pMessage,
                              const bool &pExpectedMessage,
                              const bool &pUseCurrentToken,
                              const ComputerScannerToken &pOtherToken,
                              const QString &pExtraInformation)
{
    ComputerScannerToken token = pUseCurrentToken?mScanner->token():pOtherToken;

    if (pExpectedMessage) {
        // First, check that there isn't already an error for that line/column
        // combination

        int tokenLine = token.line();
        int tokenColumn = token.column();

        foreach (const ComputerError &error, mErrors)
            if ((error.line() == tokenLine) && (error.column() == tokenColumn))
                // There is already an error for that line line/column
                // combination, so...

                return;

        mErrors.append(ComputerError(tr("%1 is expected, but '%2' was found").arg(pMessage, token.string()),
                                     tokenLine, tokenColumn));
    } else {
        mErrors.append(ComputerError(pMessage, token.line(), token.column(),
                                     pExtraInformation));
    }
}

//==============================================================================

ComputerFunction * ComputerParser::parseFunction(const QString &pFunction)
{
    // The EBNF grammar of a function is as follows:
    //
    //   Function       = VoidFunction | DoubleFunction ;
    //   VoidFunction   = "void" Identifier "(" FunctionParameters ")" "{" [ Equations ] "}" ;
    //   DoubleFunction = "double" Identifier "(" [ FunctionParameters ] ")" "{" [ Equations ] Return "}" ;

    // Initialise our scanner

    mScanner->initialise(pFunction);

    // Retrieve the type of function that we are dealing with, i.e. a void or a
    // double function

    ComputerFunction *function = new ComputerFunction;

    if (mScanner->token().symbol() == ComputerScannerToken::Void) {
        // We are dealing with a void function, so set the function as such

        function->setType(ComputerFunction::Void);
    } else if (mScanner->token().symbol() == ComputerScannerToken::Double) {
        // We are dealing with a double function, so set the function as such

        function->setType(ComputerFunction::Double);
    } else {
        addError(tr("either 'void' or 'double'"));

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be an identifier

    if (mScanner->token().symbol() == ComputerScannerToken::Identifier) {
        // We got an identifier, so set the name of the function

        function->setName(mScanner->token().string());
    } else {
        addError(tr("an identifier"));

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be an opening bracket

    if (mScanner->token().symbol() != ComputerScannerToken::OpeningBracket) {
        addError("'('");

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // Parse the different function parameters

    if (!parseFunctionParameters(function)) {
        // Something went wrong with the parsing of the different function
        // parameters, so...

        delete function;

        return 0;
    }

    // The current token must be a closing bracket

    if (mScanner->token().symbol() != ComputerScannerToken::ClosingBracket) {
        addError("')'");

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be an opening curly bracket

    if (mScanner->token().symbol() != ComputerScannerToken::OpeningCurlyBracket) {
        addError("'{'");

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // Parse the different equations

    if (!parseEquations(function)) {
        // Something went wrong with the parsing of the different equations,
        // so...

        delete function;

        return 0;
    }

    // Parse the return statement, but only in the case of a double function

    if (   (function->type() == ComputerFunction::Double)
        && !parseReturn(function)) {
        // Something went wrong with the parsing of the return statement, so...

        delete function;

        return 0;
    }

    // The current token must be a closing curly bracket

    if (mScanner->token().symbol() != ComputerScannerToken::ClosingCurlyBracket) {
        addError("'}'");

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be EOF

    if (mScanner->token().symbol() != ComputerScannerToken::Eof) {
        addError("EOF");

        delete function;

        return 0;
    }

    // Everything went fine, so...

    return function;
}

//==============================================================================

ComputerEquation * ComputerParser::parseEquation(const QString &pEquation)
{
//---GRY--- TO BE DONE...
}

//==============================================================================

bool ComputerParser::parseFunctionParameters(ComputerFunction *pFunction)
{
    // The EBNF grammar of a list of function parameters is as follows:
    //
    //   FunctionParameters = FunctionParameter { "," FunctionParameter } ;

    // We must have 1+/0+ function parameters in the case of a void/double
    // function

    bool needAtLeastOneFunctionParameter = pFunction->type() == ComputerFunction::Void;

    if (parseFunctionParameter(pFunction, needAtLeastOneFunctionParameter))
        // The first function parameter was properly parsed, so look for other
        // function parameters

        // The current token must be "," if we are to have another function
        // parameter definition

        while (mScanner->token().symbol() == ComputerScannerToken::Comma) {
            mScanner->getNextToken();

            // We must then have the function parameter definition itself

            if (!parseFunctionParameter(pFunction))
                // Something went wrong with the parsing of the function
                // parameter definition, so...

                return false;
        }
    else
        // Something went wrong with the parsing of the function parameter
        // definition, but it should only be reported as an error if we expected
        // a function parameter

        return !needAtLeastOneFunctionParameter;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerParser::parseFunctionParameter(ComputerFunction *pFunction,
                                            const bool &pNeeded)
{
    // The EBNF grammar of a function parameter is as follows:
    //
    //   FunctionParameter = "double" "*" Identifier ;

    // The current token must be "double"

    if (mScanner->token().symbol() != ComputerScannerToken::Double) {
        if (pNeeded)
            // We need a function parameter definition, so...

            addError("'double'");

        return false;
    }

    mScanner->getNextToken();

    // The current token must be "*"

    if (mScanner->token().symbol() != ComputerScannerToken::Times) {
        addError("'*'");

        return false;
    }

    mScanner->getNextToken();

    // The current token must be an identifier

    if (mScanner->token().symbol() == ComputerScannerToken::Identifier) {
        // We got an identifier, so try to add it as the name of a new function
        // parameter

        if (!pFunction->addParameter(mScanner->token().string())) {
            // The function parameter already exists, so...

            addError(tr("there is already a function parameter called '%1'").arg(mScanner->token().string()),
                     false);

            return false;
        }
    } else {
        addError(tr("an identifier"));

        return false;
    }

    mScanner->getNextToken();

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerParser::parseEquations(ComputerFunction *pFunction)
{
    // The EBNF grammar of a series of equations is as follows:
    //
    //   Equations = { Equation } ;
    //   Equation  = Identifier "[" IntegerValue "]" "=" EquationRHS ";" ;

    while (mScanner->token().symbol() == ComputerScannerToken::Identifier) {
        // The current token is an identifier which is what we would expect for
        // the beginning of an equation, so we get ready for the parsing of an
        // equation

        ComputerEquation *equation = new ComputerEquation();

        // Set the name of the equation parameter array

        QString arrayName;
        int arrayIndex;

        arrayName = mScanner->token().string();

        mScanner->getNextToken();

        // The current token must be "["

        if (mScanner->token().symbol() != ComputerScannerToken::OpeningSquareBracket) {
            addError("'['");

            delete equation;

            return false;
        }

        mScanner->getNextToken();

        // The current token must be an integer value

        if (mScanner->token().symbol() == ComputerScannerToken::IntegerValue) {
            // We got an integer value, so set the index of the equation
            // parameter array

            arrayIndex = mScanner->token().string().toInt();
        } else {
            addError(tr("an integer"));

            delete equation;

            return false;
        }

        mScanner->getNextToken();

        // The current token must be "]"

        if (mScanner->token().symbol() != ComputerScannerToken::ClosingSquareBracket) {
            addError("']'");

            delete equation;

            return false;
        }

        mScanner->getNextToken();

        // The current token must be "="

        if (mScanner->token().symbol() != ComputerScannerToken::Equal) {
            addError("'='");

            delete equation;

            return false;
        }

        mScanner->getNextToken();

        // Parse the RHS of an equation

        if (!parseEquationRhs(pFunction)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete equation;

            return false;
        }

        // The current token must be ";"

        if (mScanner->token().symbol() != ComputerScannerToken::SemiColon) {
            addError("';'");

            delete equation;

            return false;
        }

        mScanner->getNextToken();

        // The equation was successfully parsed, so add it to the list of
        // functions

        pFunction->addEquation(equation);
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

typedef bool (*ParseGenericExpression)(ComputerParser *, ComputerFunction *);

bool parseLogicalOrExpression(ComputerParser *pParser,
                              ComputerFunction *pFunction);
bool parseLogicalAndExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction);
bool parseInclusiveOrExpression(ComputerParser *pParser,
                                ComputerFunction *pFunction);
bool parseExclusiveOrExpression(ComputerParser *pParser,
                                ComputerFunction *pFunction);
bool parseAndExpression(ComputerParser *pParser,
                        ComputerFunction *pFunction);
bool parseEqualityExpression(ComputerParser *pParser,
                             ComputerFunction *pFunction);
bool parseRelationalExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction);
bool parseAdditiveExpression(ComputerParser *pParser,
                             ComputerFunction *pFunction);
bool parseMultiplicativeExpression(ComputerParser *pParser,
                                   ComputerFunction *pFunction);
bool parseUnaryExpression(ComputerParser *pParser,
                          ComputerFunction *pFunction);
bool parsePrimaryExpression(ComputerParser *pParser,
                            ComputerFunction *pFunction);

//==============================================================================

bool parseGenericExpression(ComputerParser *pParser,
                            ComputerFunction *pFunction,
                            const ComputerScannerToken::Symbols &pSymbols,
                            ParseGenericExpression pParseGenericExpression)
{
    // Parse the generic expression

    if (!pParseGenericExpression(pParser, pFunction))
        // Something went wrong with the parsing of the generic expression,
        // so...

        return false;

    // Check whether the current token's symbol is one of those we are after

    while (pSymbols.contains(pParser->scanner()->token().symbol())) {
        // We got the right symbol

        pParser->scanner()->getNextToken();

        // Parse the generic expression

        if (!pParseGenericExpression(pParser, pFunction))
            // Something went wrong with the parsing of the generic expression,
            // so...

            return false;
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseLogicalOrExpression(ComputerParser *pParser,
                              ComputerFunction *pFunction)
{
    // The EBNF grammar of a logical Or expression is as follows:
    //
    //   LogicalOrExpression = [ LogicalOrExpression "||" ] LogicalAndExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::LogicalOr,
                                parseLogicalAndExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseLogicalAndExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction)
{
    // The EBNF grammar of a logical And expression is as follows:
    //
    //   LogicalAndExpression = [ LogicalAndExpression "&&" ] InclusiveOrExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::LogicalAnd,
                                parseInclusiveOrExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseInclusiveOrExpression(ComputerParser *pParser,
                                ComputerFunction *pFunction)
{
    // The EBNF grammar of an inclusive Or expression is as follows:
    //
    //   InclusiveOrExpression = [ InclusiveOrExpression "|" ] ExclusiveOrExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::InclusiveOr,
                                parseExclusiveOrExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseExclusiveOrExpression(ComputerParser *pParser,
                                ComputerFunction *pFunction)
{
    // The EBNF grammar of an exclusive Or expression is as follows:
    //
    //   ExclusiveOrExpression = [ ExclusiveOrExpression "^" ] AndExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::ExclusiveOr,
                                parseAndExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseAndExpression(ComputerParser *pParser, ComputerFunction *pFunction)
{
    // The EBNF grammar of an And expression is as follows:
    //
    //   AndExpression = [ AndExpression "&" EqualityExpression ] ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::And,
                                parseEqualityExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseEqualityExpression(ComputerParser *pParser,
                             ComputerFunction *pFunction)
{
    // The EBNF grammar of an equality expression is as follows:
    //
    //   EqualityExpression = [ EqualityExpression ( "==" | "!=" ) ] RelationalExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::EqualEqual
                                                                << ComputerScannerToken::NotEqual,
                                parseRelationalExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseRelationalExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction)
{
    // The EBNF grammar of a relational expression is as follows:
    //
    //   RelationalExpression = [ RelationalExpression ( "<" | ">" | "<=" | ">=" ) ] AdditiveExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::LowerThan
                                                                << ComputerScannerToken::GreaterThan
                                                                << ComputerScannerToken::LowerOrEqualThan
                                                                << ComputerScannerToken::GreaterOrEqualThan,
                                parseAdditiveExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseAdditiveExpression(ComputerParser *pParser,
                             ComputerFunction *pFunction)
{
    // The EBNF grammar of an additive expression is as follows:
    //
    //   AdditiveExpression = [ AdditiveExpression ( "+" | "-" ) ] MultiplicativeExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::Plus
                                                                << ComputerScannerToken::Minus,
                                parseMultiplicativeExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseMultiplicativeExpression(ComputerParser *pParser,
                                   ComputerFunction *pFunction)
{
    // The EBNF grammar of a multiplicative expression is as follows:
    //
    //   MultiplicativeExpression = [ MultiplicativeExpression ( "*" | "/" | "%" ) ] UnaryExpression ;

    if (!parseGenericExpression(pParser, pFunction,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::Times
                                                                << ComputerScannerToken::Divide
                                                                << ComputerScannerToken::Percentage,
                                parseUnaryExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseUnaryExpression(ComputerParser *pParser, ComputerFunction *pFunction)
{
    // The EBNF grammar of a unary expression is as follows:
    //
    //   UnaryExpression =   PrimaryExpression
    //                     | ( ( "+" | "-" | "!" ) UnaryExpression ) ;

    static const ComputerScannerToken::Symbols unaryOperatorSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::Plus
                                                                                                      << ComputerScannerToken::Minus
                                                                                                      << ComputerScannerToken::ExclamationMark;

    // Check whether the current token's symbol is one of those we are after

    while (unaryOperatorSymbols.contains(pParser->scanner()->token().symbol())) {
        // We got the right symbol

        pParser->scanner()->getNextToken();

        // Parse the unary expression

        if (!parseUnaryExpression(pParser, pFunction))
            // Something went wrong with the parsing of the unary expression,
            // so...

            return false;
    }

    // Parse the primary expression

    if (!parsePrimaryExpression(pParser, pFunction))
        // Something went wrong with the parsing of the primary expression,
        // so...

        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parsePrimaryExpression(ComputerParser *pParser,
                            ComputerFunction *pFunction)
{
    // The EBNF grammar of a primary expression is as follows:
    //
    //   PrimaryExpression        =   Identifier [ "[" IntegerValue "]" ]
    //                              | IntegerValue
    //                              | DoubleValue
    //                              | ( FunctionWithOneArgument "(" EquationRHS ")" ) ;
    //                              | ( FunctionWithTwoArguments "(" EquationRHS "," EquationRHS ")" ) ;
    //                              | ( "(" EquationRHS ")" ) ;
    //   FunctionWithOneArgument  = "sin" | "cos" | "tan" ;
    //   FunctionWithTwoArguments = "pow" ;

    // Check whether the current token's symbol is an identifier, an integer
    // value, a double value, a function with one argument, a function with two
    // arguments or "("

    static const ComputerScannerToken::Symbols oneArgumentFunctionSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::Sin
                                                                                                            << ComputerScannerToken::Cos
                                                                                                            << ComputerScannerToken::Tan;
    static const ComputerScannerToken::Symbols twoArgumentFunctionSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::Pow;


    if (pParser->scanner()->token().symbol() == ComputerScannerToken::Identifier) {
        pParser->scanner()->getNextToken();

        // Check whether the current token is "["

        if (pParser->scanner()->token().symbol() == ComputerScannerToken::OpeningSquareBracket) {
            pParser->scanner()->getNextToken();

            // The current token must be an integer value

            if (pParser->scanner()->token().symbol() != ComputerScannerToken::IntegerValue) {
                pParser->addError(QObject::tr("an integer"));

                return false;
            }

            pParser->scanner()->getNextToken();

            // The current token must be "]"

            if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingSquareBracket) {
                pParser->addError("']'");

                return false;
            }

            pParser->scanner()->getNextToken();
        }
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::IntegerValue) {
        pParser->scanner()->getNextToken();
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::DoubleValue) {
        pParser->scanner()->getNextToken();
    } else if (oneArgumentFunctionSymbols.contains(pParser->scanner()->token().symbol())) {
        pParser->scanner()->getNextToken();

        // The current token must "("

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::OpeningBracket) {
            pParser->addError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        if (!pParser->parseEquationRhs(pFunction))
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            return false;

        // The current token must ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->addError("')'");

            return false;
        }

        pParser->scanner()->getNextToken();
    } else if (twoArgumentFunctionSymbols.contains(pParser->scanner()->token().symbol())) {
        pParser->scanner()->getNextToken();

        // The current token must "("

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::OpeningBracket) {
            pParser->addError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        if (!pParser->parseEquationRhs(pFunction))
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            return false;

        // The current token must ","

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
            pParser->addError("','");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        if (!pParser->parseEquationRhs(pFunction))
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            return false;

        // The current token must ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->addError("')'");

            return false;
        }

        pParser->scanner()->getNextToken();
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::OpeningBracket) {
        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        if (!pParser->parseEquationRhs(pFunction))
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            return false;

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->addError("')'");

            return false;
        }

        pParser->scanner()->getNextToken();
    } else {
        // We didn't get any of the above symbols, so...

        return false;
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerParser::parseEquationRhs(ComputerFunction *pFunction)
{
    // The EBNF grammar of an equation's RHS is as follows:
    //
    //   EquationRHS =   LogicalOrExpression
    //                 | ( LogicalOrExpression "?" EquationRHS ":" EquationRHS ) ;

    // Parse a logical Or expression

    if (!parseLogicalOrExpression(this, pFunction))
        // Something went wrong with the parsing of a logical Or expression,
        // so...

        return false;

    // Check whether the current token is "?"

    while (mScanner->token().symbol() == ComputerScannerToken::QuestionMark) {
        // We got "?" which means that we should have a conditional statement

        mScanner->getNextToken();

        // Parse the RHS of an equation

        if (!parseEquationRhs(pFunction))
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            return false;

        // The current token must be ":"

        if (mScanner->token().symbol() != ComputerScannerToken::Colon) {
            addError("':'");

            return false;
        }

        mScanner->getNextToken();

        // Parse the RHS of an equation

        if (!parseEquationRhs(pFunction))
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            return false;
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerParser::parseReturn(ComputerFunction *pFunction)
{
    // The EBNF grammar of a return statement is as follows:
    //
    //   Return = "return" EquationRHS ";" ;

    // The current token must be "return"

    if (mScanner->token().symbol() != ComputerScannerToken::Return) {
        addError("'return'");

        return false;
    }

    mScanner->getNextToken();

    // Parse the equivalent of the RHS of an equation

    if (!parseEquationRhs(pFunction))
        return false;

    // The current token must be ";"

    if (mScanner->token().symbol() != ComputerScannerToken::SemiColon) {
        addError("';'");

        return false;
    }

    mScanner->getNextToken();

    // Everything went fine, so...

    return true;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
