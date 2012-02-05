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
    mErrors(ComputerErrors())
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

        mErrors.append(ComputerError(tr("%1 was expected, but '%2' was found instead").arg(pMessage, token.string()),
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

    // Reset/initialise ourselves

    mErrors.clear();
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
    //   FunctionParameter = "double" [ "*" ] Identifier ;

    // The current token must be "double"

    if (mScanner->token().symbol() != ComputerScannerToken::Double) {
        if (pNeeded)
            // We need a function parameter definition, so...

            addError("'double'");

        return false;
    }

    mScanner->getNextToken();

    // Check whether the current token is "*"

    bool pointer = false;

    if (mScanner->token().symbol() == ComputerScannerToken::Times) {
        // We got "*" which means that the function parameter is a pointer

        pointer = true;

        mScanner->getNextToken();
    }

    // The current token must be an identifier

    if (mScanner->token().symbol() == ComputerScannerToken::Identifier) {
        // We got an identifier, so try to add it as the name of a new function
        // parameter

        if (!pFunction->addParameter(ComputerParameter(mScanner->token().string(), pointer))) {
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

        // Set the name of the equation parameter array

        QString arrayName = mScanner->token().string();

        mScanner->getNextToken();

        // The current token must be "["

        if (mScanner->token().symbol() != ComputerScannerToken::OpeningSquareBracket) {
            addError("'['");

            return false;
        }

        mScanner->getNextToken();

        // The current token must be an integer value

        int arrayIndex = -1;

        if (mScanner->token().symbol() == ComputerScannerToken::IntegerValue)
            // We got an integer value, so set the index of the equation
            // parameter array

            arrayIndex = mScanner->token().string().toInt();

        if (arrayIndex < 0) {
            addError(tr("a positive integer"));

            return false;
        }

        mScanner->getNextToken();

        // The current token must be "]"

        if (mScanner->token().symbol() != ComputerScannerToken::ClosingSquareBracket) {
            addError("']'");

            return false;
        }

        mScanner->getNextToken();

        // The current token must be "="

        if (mScanner->token().symbol() != ComputerScannerToken::Equal) {
            addError("'='");

            return false;
        }

        mScanner->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *rhsEquation = 0;

        if (!parseRhsEquation(pFunction, rhsEquation)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete rhsEquation;

            return false;
        }

        // The current token must be ";"

        if (mScanner->token().symbol() != ComputerScannerToken::SemiColon) {
            addError("';'");

            delete rhsEquation;

            return false;
        }

        mScanner->getNextToken();

        // The equation was successfully parsed, so add it to our list

        pFunction->addEquation(new ComputerEquation(ComputerEquation::Assign,
                                                    new ComputerEquation(arrayName, arrayIndex),
                                                    rhsEquation));
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

typedef bool (*ParseGenericExpression)(ComputerParser *, ComputerFunction *,
                                       ComputerEquation * &);

bool parseLogicalOrExpression(ComputerParser *pParser,
                              ComputerFunction *pFunction,
                              ComputerEquation * &pExpression);
bool parseLogicalAndExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction,
                               ComputerEquation * &pExpression);
bool parseEqualityExpression(ComputerParser *pParser,
                             ComputerFunction *pFunction,
                             ComputerEquation * &pExpression);
bool parseRelationalExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction,
                               ComputerEquation * &pExpression);
bool parseAdditiveExpression(ComputerParser *pParser,
                             ComputerFunction *pFunction,
                             ComputerEquation * &pExpression);
bool parseMultiplicativeExpression(ComputerParser *pParser,
                                   ComputerFunction *pFunction,
                                   ComputerEquation * &pExpression);
bool parseUnaryExpression(ComputerParser *pParser,
                          ComputerFunction *pFunction,
                          ComputerEquation * &pExpression);
bool parsePrimaryExpression(ComputerParser *pParser,
                            ComputerFunction *pFunction,
                            ComputerEquation * &pExpression);

//==============================================================================

bool parseGenericExpression(ComputerParser *pParser,
                            ComputerFunction *pFunction,
                            ComputerEquation * &pExpression,
                            const ComputerScannerToken::Symbols &pSymbols,
                            ParseGenericExpression pParseGenericExpression)
{
    // Parse the generic expression

    if (!pParseGenericExpression(pParser, pFunction, pExpression))
        // Something went wrong with the parsing of the generic expression,
        // so...

        return false;

    // Check whether the current token's symbol is one of those we are after and
    // if so parse the generic expression and start over

    while (pSymbols.contains(pParser->scanner()->token().symbol())) {
        // We got the right symbol, so keep track of it (as an equation type)
        // and get the next token

        ComputerEquation::Type equationType = pParser->scanner()->token().equationType();

        pParser->scanner()->getNextToken();

        // Parse the generic expression

        ComputerEquation *otherExpression = 0;

        if (!pParseGenericExpression(pParser, pFunction, otherExpression)) {
            // Something went wrong with the parsing of the generic expression,
            // so...

            delete otherExpression;

            return false;
        }

        // The parsing of the generic expression went fine, so update our
        // expression

        pExpression = new ComputerEquation(equationType,
                                           pExpression, otherExpression);
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseLogicalOrExpression(ComputerParser *pParser,
                              ComputerFunction *pFunction,
                              ComputerEquation * &pExpression)
{
    // The EBNF grammar of a logical Or expression is as follows:
    //
    //   LogicalOrExpression = [ LogicalOrExpression "||" ] LogicalAndExpression ;

    if (!parseGenericExpression(pParser, pFunction, pExpression,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::LogicalOr,
                                parseLogicalAndExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseLogicalAndExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction,
                               ComputerEquation * &pExpression)
{
    // The EBNF grammar of a logical And expression is as follows:
    //
    //   LogicalAndExpression = [ LogicalAndExpression "&&" ] EqualityExpression ;

    if (!parseGenericExpression(pParser, pFunction, pExpression,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::LogicalAnd,
                                parseEqualityExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseEqualityExpression(ComputerParser *pParser,
                             ComputerFunction *pFunction,
                             ComputerEquation * &pExpression)
{
    // The EBNF grammar of an equality expression is as follows:
    //
    //   EqualityExpression = [ EqualityExpression ( "==" | "!=" ) ] RelationalExpression ;

    if (!parseGenericExpression(pParser, pFunction, pExpression,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::EqualEqual
                                                                << ComputerScannerToken::NotEqual,
                                parseRelationalExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseRelationalExpression(ComputerParser *pParser,
                               ComputerFunction *pFunction,
                               ComputerEquation * &pExpression)
{
    // The EBNF grammar of a relational expression is as follows:
    //
    //   RelationalExpression = [ RelationalExpression ( "<" | ">" | "<=" | ">=" ) ] AdditiveExpression ;

    if (!parseGenericExpression(pParser, pFunction, pExpression,
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
                             ComputerFunction *pFunction,
                             ComputerEquation * &pExpression)
{
    // The EBNF grammar of an additive expression is as follows:
    //
    //   AdditiveExpression = [ AdditiveExpression ( "+" | "-" ) ] MultiplicativeExpression ;

    if (!parseGenericExpression(pParser, pFunction, pExpression,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::Plus
                                                                << ComputerScannerToken::Minus,
                                parseMultiplicativeExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseMultiplicativeExpression(ComputerParser *pParser,
                                   ComputerFunction *pFunction,
                                   ComputerEquation * &pExpression)
{
    // The EBNF grammar of a multiplicative expression is as follows:
    //
    //   MultiplicativeExpression = [ MultiplicativeExpression ( "*" | "/" ) ] UnaryExpression ;

    if (!parseGenericExpression(pParser, pFunction, pExpression,
                                ComputerScannerToken::Symbols() << ComputerScannerToken::Times
                                                                << ComputerScannerToken::Divide,
                                parseUnaryExpression))
        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parseUnaryExpression(ComputerParser *pParser, ComputerFunction *pFunction,
                          ComputerEquation * &pExpression)
{
    // The EBNF grammar of a unary expression is as follows:
    //
    //   UnaryExpression =   PrimaryExpression
    //                     | ( ( "+" | "-" | "!" ) UnaryExpression ) ;

    static const ComputerScannerToken::Symbols unaryOperatorSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::Plus
                                                                                                      << ComputerScannerToken::Minus
                                                                                                      << ComputerScannerToken::Not;

    // Check whether the current token's symbol is one of those we are after

    static const QChar Plus  = QChar('+');
    static const QChar Minus = QChar('-');
    static const QChar Not   = QChar('!');

    QString unaryOperators = QString();

    while (unaryOperatorSymbols.contains(pParser->scanner()->token().symbol())) {
        // We got the right symbol, so keep track of it and get the next token
        // Note: we preprend (rather than append) since we later on want to use
        //       the list in reverse order, so...

        switch (pParser->scanner()->token().symbol()) {
        case ComputerScannerToken::Plus:
            unaryOperators = Plus+unaryOperators;

            break;
        case ComputerScannerToken::Minus:
            unaryOperators = Minus+unaryOperators;

            break;
        default:   // Not
            unaryOperators = Not+unaryOperators;
        }

        pParser->scanner()->getNextToken();
    }

   // Parse the primary expression

    if (!parsePrimaryExpression(pParser, pFunction, pExpression))
        // Something went wrong with the parsing of the primary expression,
        // so...

        return false;

    // The parsing of our unary expression went fine, so apply the unary
    // operators, if any

    if (!unaryOperators.isEmpty())
        foreach (const QChar &unaryOperator, unaryOperators)
            if (unaryOperator == Plus)
                pExpression = new ComputerEquation(ComputerEquation::Plus, pExpression);
            else if (unaryOperator == Minus)
                pExpression = new ComputerEquation(ComputerEquation::Minus, pExpression);
            else   // Not
                pExpression = new ComputerEquation(ComputerEquation::Not, pExpression);

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool parsePrimaryExpression(ComputerParser *pParser,
                            ComputerFunction *pFunction,
                            ComputerEquation * &pExpression)
{
    // The EBNF grammar of a primary expression is as follows:
    //
    //   PrimaryExpression              =   Identifier [ "[" IntegerValue "]" ]
    //                                    | IntegerValue
    //                                    | DoubleValue
    //                                    | ( FunctionWithOneArgument "(" EquationRHS ")" ) ;
    //                                    | ( FunctionWithTwoArguments "(" EquationRHS "," EquationRHS ")" ) ;
    //                                    | ( "(" EquationRHS ")" ) ;
    //   FunctionWithOneArgument        =   "fabs" | "exp" | "log" | "ceil" | "floor" | "factorial"
    //                                    | "sin" | "cos" | "tan" | "sinh" | "cosh" | "tanh"
    //                                    | "asin" | "acos" | "atan" | "asinh" | "acosh" | "atanh" ;
    //   FunctionWithTwoArguments       = "arbitraryLog" | "factorOf" | "pow" | "quotient" | "rem" | "xor" ;
    //   FunctionWithTwoOrMoreArguments = "gcd" | "lcm" | "max" | "min" ;

    // Check whether the current token's symbol is an identifier, an integer
    // value, a double value, a function with one argument, a function with two
    // arguments or "("

    static const ComputerScannerToken::Symbols oneArgumentFunctionSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::Abs
                                                                                                            << ComputerScannerToken::Exp
                                                                                                            << ComputerScannerToken::Log
                                                                                                            << ComputerScannerToken::Ceil
                                                                                                            << ComputerScannerToken::Floor
                                                                                                            << ComputerScannerToken::Factorial
                                                                                                            << ComputerScannerToken::Sin
                                                                                                            << ComputerScannerToken::Cos
                                                                                                            << ComputerScannerToken::Tan
                                                                                                            << ComputerScannerToken::SinH
                                                                                                            << ComputerScannerToken::CosH
                                                                                                            << ComputerScannerToken::TanH
                                                                                                            << ComputerScannerToken::ASin
                                                                                                            << ComputerScannerToken::ACos
                                                                                                            << ComputerScannerToken::ATan
                                                                                                            << ComputerScannerToken::ASinH
                                                                                                            << ComputerScannerToken::ACosH
                                                                                                            << ComputerScannerToken::ATanH;
    static const ComputerScannerToken::Symbols twoArgumentFunctionSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::ArbitraryLog
                                                                                                            << ComputerScannerToken::FactorOf
                                                                                                            << ComputerScannerToken::Pow
                                                                                                            << ComputerScannerToken::Quotient
                                                                                                            << ComputerScannerToken::Rem
                                                                                                            << ComputerScannerToken::XOr;
    static const ComputerScannerToken::Symbols xArgumentFunctionSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::GCD
                                                                                                          << ComputerScannerToken::LCM
                                                                                                          << ComputerScannerToken::Max
                                                                                                          << ComputerScannerToken::Min;

    if (pParser->scanner()->token().symbol() == ComputerScannerToken::Identifier) {
        // We are dealing with an identifier which corresponds to the name of a
        // parameter, so keep track of it and get the next token

        QString parameterName = pParser->scanner()->token().string();

        pParser->scanner()->getNextToken();

        // Check whether the current token is "["

        if (pParser->scanner()->token().symbol() == ComputerScannerToken::OpeningSquareBracket) {
            // We are dealing with an indirect parameter

            pParser->scanner()->getNextToken();

            // The current token must be an integer value

            int parameterIndex = -1;

            if (pParser->scanner()->token().symbol() == ComputerScannerToken::IntegerValue)
                // We got an integer value, so keep track of it

                parameterIndex = pParser->scanner()->token().string().toInt();

            if (parameterIndex < 0) {
                pParser->addError(QObject::tr("a positive integer"));

                return false;
            }

            pParser->scanner()->getNextToken();

            // The current token must be "]"

            if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingSquareBracket) {
                pParser->addError("']'");

                return false;
            }

            pParser->scanner()->getNextToken();

            // Everything went fine with the parsing of our indirect parameter,
            // so...

            pExpression = new ComputerEquation(parameterName, parameterIndex);
        } else {
            // We are dealing with a direct parameter, so...

            pExpression = new ComputerEquation(parameterName);
        }
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::IntegerValue) {
        // We are dealing with an integer value, i.e. a number, so...

        pExpression = new ComputerEquation(pParser->scanner()->token().string().toDouble());

        pParser->scanner()->getNextToken();
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::DoubleValue) {
        // We are dealing with a double value, i.e. a number, so...

        pExpression = new ComputerEquation(pParser->scanner()->token().string().toDouble());

        pParser->scanner()->getNextToken();
    } else if (oneArgumentFunctionSymbols.contains(pParser->scanner()->token().symbol())) {
        // We are dealing with a one-argument function, so...

        ComputerEquation::Type equationType = pParser->scanner()->token().equationType();

        pParser->scanner()->getNextToken();

        // The current token must be "("

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::OpeningBracket) {
            pParser->addError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argument = 0;

        if (!pParser->parseRhsEquation(pFunction, argument)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argument;

            return false;
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->addError("')'");

            delete argument;

            return false;
        }

        pParser->scanner()->getNextToken();

        // The parsing of our one-argument function went fine, so...

        pExpression = new ComputerEquation(equationType, argument);
    } else if (twoArgumentFunctionSymbols.contains(pParser->scanner()->token().symbol())) {
        // We are dealing with a two-argument function, so...

        ComputerEquation::Type equationType = pParser->scanner()->token().equationType();

        pParser->scanner()->getNextToken();

        // The current token must be "("

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::OpeningBracket) {
            pParser->addError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentOne = 0;

        if (!pParser->parseRhsEquation(pFunction, argumentOne)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argumentOne;

            return false;
        }

        // The current token must be ","

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
            pParser->addError("','");

            delete argumentOne;

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentTwo = 0;

        if (!pParser->parseRhsEquation(pFunction, argumentTwo)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argumentOne;
            delete argumentTwo;

            return false;
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->addError("')'");

            delete argumentOne;
            delete argumentTwo;

            return false;
        }

        pParser->scanner()->getNextToken();

        // The parsing of our two-argument function went fine, so...

        pExpression = new ComputerEquation(equationType,
                                           argumentOne, argumentTwo);
    } else if (xArgumentFunctionSymbols.contains(pParser->scanner()->token().symbol())) {
        // We are dealing with an X-argument function, so...

        ComputerEquation::Type equationType = pParser->scanner()->token().equationType();

        pParser->scanner()->getNextToken();

        // The current token must be "("

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::OpeningBracket) {
            pParser->addError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // The current token must an integer

        int argumentsCount = -1;

        if (pParser->scanner()->token().symbol() == ComputerScannerToken::IntegerValue)
            // We got an integer value, so set the number of arguments with it

            argumentsCount = pParser->scanner()->token().string().toInt();

        if (argumentsCount < 0) {
            pParser->addError(QObject::tr("a positive integer"));

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation and this as many times as there are
        // arguments according to argumentsCount

        ComputerEquation * arguments[argumentsCount];

        for (int i = 0; i < argumentsCount; ++i) {
            arguments[i] = 0;

            // The current token must be ","

            if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
                pParser->addError("','");

                for (int j = 0; j < i; ++j)
                    delete arguments[j];

                return false;
            }

            pParser->scanner()->getNextToken();

            // Parse the RHS of an equation

            if (!pParser->parseRhsEquation(pFunction, arguments[i])) {
                // Something went wrong with the parsing of the RHS of an
                // equation, so...

                for (int j = 0; j <= i; ++j)
                    delete arguments[j];

                return false;
            }
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->addError("')'");

            for (int i = 0; i < argumentsCount; ++i)
                delete arguments[i];

            return false;
        }

        pParser->scanner()->getNextToken();

        // The parsing of our X-argument function went fine, so...

        pExpression = new ComputerEquation(equationType,
                                           argumentsCount, arguments);
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::OpeningBracket) {
        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *equation = 0;

        if (!pParser->parseRhsEquation(pFunction, equation)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete equation;

            return false;
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->addError("')'");

            delete equation;

            return false;
        }

        pParser->scanner()->getNextToken();

        // The parsing of the RHS of an equation surrounded by parentheses went
        // fine, so...

        pExpression = equation;
    } else {
        // We didn't get any of the above symbols, so...

        return false;
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerParser::parseRhsEquation(ComputerFunction *pFunction,
                                      ComputerEquation * &pRhsEquation)
{
    // The EBNF grammar of an equation's RHS is as follows:
    //
    //   EquationRHS =   LogicalOrExpression
    //                 | ( LogicalOrExpression "?" EquationRHS ":" EquationRHS ) ;

    // Parse a logical Or expression

    ComputerEquation *mainOrConditionEquation = 0;

    if (!parseLogicalOrExpression(this, pFunction, mainOrConditionEquation)) {
        // Something went wrong with the parsing of a logical Or expression,
        // so...

        delete mainOrConditionEquation;

        return false;
    }

    // Check whether the current token is "?"

    if (mScanner->token().symbol() == ComputerScannerToken::QuestionMark) {
        // We got "?" which means that we should have a conditional statement

        mScanner->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *trueCaseEquation = 0;

        if (!parseRhsEquation(pFunction, trueCaseEquation)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete mainOrConditionEquation;
            delete trueCaseEquation;

            return false;
        }

        // The current token must be ":"

        if (mScanner->token().symbol() != ComputerScannerToken::Colon) {
            addError("':'");

            delete mainOrConditionEquation;
            delete trueCaseEquation;

            return false;
        }

        mScanner->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *falseCaseEquation = 0;

        if (!parseRhsEquation(pFunction, falseCaseEquation)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete mainOrConditionEquation;
            delete trueCaseEquation;
            delete falseCaseEquation;

            return false;
        }

        // Everything went fine with the parsing of our piecewise statement,
        // so...

        ComputerEquation *caseEquations = new ComputerEquation(ComputerEquation::PiecewiseCases,
                                                               trueCaseEquation, falseCaseEquation);

        pRhsEquation = new ComputerEquation(ComputerEquation::Piecewise,
                                            mainOrConditionEquation, caseEquations);
    } else {
        // We are dealing with a 'simple' equation, so...

        pRhsEquation = mainOrConditionEquation;
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

    ComputerEquation *equation = 0;

    if (!parseRhsEquation(pFunction, equation)) {
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

    // The parsing of our return statement went fine, so...

    pFunction->setReturnEquation(equation);

    // Everything went fine, so...

    return true;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
