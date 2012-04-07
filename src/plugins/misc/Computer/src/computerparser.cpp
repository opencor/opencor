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
    mError(ComputerError())
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

ComputerError ComputerParser::error()
{
    // Return the computer parser's error

    return mError;
}

//==============================================================================

void ComputerParser::setError(const QString &pMessage,
                              const bool &pExpectedMessage,
                              const bool &pUseCurrentToken,
                              const ComputerScannerToken &pOtherToken)
{
    ComputerScannerToken token = pUseCurrentToken?mScanner->token():pOtherToken;

    if (pExpectedMessage)
        mError = ComputerError(tr("%1 was expected, but '%2' was found instead").arg(pMessage, (token.symbol() == ComputerScannerToken::Eof)?"EoF":token.string()),
                               token.line(), token.column());
    else
        mError = ComputerError(pMessage, token.line(), token.column());
}

//==============================================================================

ComputerFunction * ComputerParser::parseFunction(const QString &pFunction)
{
    // The EBNF grammar of a function is as follows:
    //
    //   Function       = VoidFunction | DoubleFunction ;
    //   VoidFunction   = "void" Identifier "(" FunctionParameters ")" "{" Equations "}" ;
    //   DoubleFunction = "double" Identifier "(" FunctionParameters ")" "{" Equations ReturnStatement "}" ;

    // Reset/initialise ourselves

    mError = ComputerError();

    mScanner->initialise(pFunction);

    // Retrieve the type of function that we are dealing with, i.e. a void or a
    // double function

    ComputerFunction *function = new ComputerFunction();

    if (mScanner->token().symbol() == ComputerScannerToken::Void) {
        // We are dealing with a void function, so set the function as such

        function->setType(ComputerFunction::Void);
    } else if (mScanner->token().symbol() == ComputerScannerToken::Double) {
        // We are dealing with a double function, so set the function as such

        function->setType(ComputerFunction::Double);
    } else {
        setError(tr("either 'void' or 'double'"));

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be an identifier

    if (mScanner->token().symbol() == ComputerScannerToken::Identifier) {
        // We got an identifier, so set the name of the function

        function->setName(mScanner->token().string());
    } else {
        setError(tr("an identifier"));

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be an opening bracket

    if (mScanner->token().symbol() != ComputerScannerToken::OpeningBracket) {
        setError("'('");

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
        setError("'double' or ')'");

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be an opening curly bracket

    if (mScanner->token().symbol() != ComputerScannerToken::OpeningCurlyBracket) {
        setError("'{'");

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
        && !parseReturnStatement(function)) {
        // Something went wrong with the parsing of the return statement, so...

        delete function;

        return 0;
    }

    // The current token must be a closing curly bracket

    if (mScanner->token().symbol() != ComputerScannerToken::ClosingCurlyBracket) {
        setError("'}'");

        delete function;

        return 0;
    }

    mScanner->getNextToken();

    // The current token must be EOF

    if (mScanner->token().symbol() != ComputerScannerToken::Eof) {
        setError("EOF");

        delete function;

        return 0;
    }

    // Everything went fine, so...
//---GRY--- NORMALLY, ALL WE WOULD DO IS RETURN THE FUNCTION, BUT BECAUSE WE
//          DON'T CURRENTLY SUPPORT DEFINITE INTEGRALS (BUT STILL ALLOW THEIR
//          PARSING), WE NEED TO CHECK THAT 'NO PARSING ERRORS' WERE FOUND...

return mError.isEmpty()?function:0;

//    return function;
}

//==============================================================================

bool ComputerParser::parseFunctionParameters(ComputerFunction *pFunction)
{
    // The EBNF grammar of a list of function parameters is as follows:
    //
    //   FunctionParameters = [ FunctionParameter { "," FunctionParameter } ] ;

    if (parseFunctionParameter(pFunction, false))
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
        // definition, but it should only be reported if an error has been
        // generated

        return mError.isEmpty();

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

            setError("'double'");

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

            setError(tr("there is already a function parameter called '%1'").arg(mScanner->token().string()), false);

            return false;
        }
    } else {
        setError(tr("a '*' or an identifier"));

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
            setError("'['");

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
            setError(tr("a positive integer"));

            return false;
        }

        mScanner->getNextToken();

        // The current token must be "]"

        if (mScanner->token().symbol() != ComputerScannerToken::ClosingSquareBracket) {
            setError("']'");

            return false;
        }

        mScanner->getNextToken();

        // The current token must be "="

        if (mScanner->token().symbol() != ComputerScannerToken::Equal) {
            setError("'='");

            return false;
        }

        mScanner->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *rhsEquation = 0;

        if (!parseRhsEquation(rhsEquation)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete rhsEquation;

            return false;
        }

        // The current token must be ";"

        if (mScanner->token().symbol() != ComputerScannerToken::SemiColon) {
            setError("';'");

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

typedef bool (*ParseGenericExpression)(ComputerParser *, ComputerEquation * &);

bool parseOrExpression(ComputerParser *pParser,
                       ComputerEquation * &pExpression);
bool parseAndExpression(ComputerParser *pParser,
                        ComputerEquation * &pExpression);
bool parseXorExpression(ComputerParser *pParser,
                        ComputerEquation * &pExpression);
bool parseEqualityExpression(ComputerParser *pParser,
                             ComputerEquation * &pExpression);
bool parseRelationalExpression(ComputerParser *pParser,
                               ComputerEquation * &pExpression);
bool parseAdditiveExpression(ComputerParser *pParser,
                             ComputerEquation * &pExpression);
bool parseMultiplicativeExpression(ComputerParser *pParser,
                                   ComputerEquation * &pExpression);
bool parseUnaryExpression(ComputerParser *pParser,
                          ComputerEquation * &pExpression);
bool parsePrimaryExpression(ComputerParser *pParser,
                            ComputerEquation * &pExpression);
bool parseParameter(ComputerParser *pParser, ComputerEquation * &pExpression);

//==============================================================================

bool parseGenericExpression(ComputerParser *pParser,
                            ComputerEquation * &pExpression,
                            const ComputerScannerToken::Symbols &pSymbols,
                            ParseGenericExpression pParseGenericExpression)
{
    // Parse the generic expression

    if (!pParseGenericExpression(pParser, pExpression))
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

        if (!pParseGenericExpression(pParser, otherExpression)) {
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

bool parseOrExpression(ComputerParser *pParser,
                       ComputerEquation *&pExpression)
{
    // The EBNF grammar of a Or expression is as follows:
    //
    //   OrExpression = [ OrExpression "||" ] AndExpression ;

    return parseGenericExpression(pParser, pExpression,
                                  ComputerScannerToken::Symbols() << ComputerScannerToken::Or,
                                  parseAndExpression);
}

//==============================================================================

bool parseAndExpression(ComputerParser *pParser,
                        ComputerEquation *&pExpression)
{
    // The EBNF grammar of a And expression is as follows:
    //
    //   AndExpression = [ AndExpression "&&" ] XorExpression ;

    return parseGenericExpression(pParser, pExpression,
                                  ComputerScannerToken::Symbols() << ComputerScannerToken::And,
                                  parseXorExpression);
}

//==============================================================================

bool parseXorExpression(ComputerParser *pParser,
                        ComputerEquation * &pExpression)
{
    // The EBNF grammar of a And expression is as follows:
    //
    //   XorExpression = [ XorExpression "^" ] EqualityExpression ;

    return parseGenericExpression(pParser, pExpression,
                                  ComputerScannerToken::Symbols() << ComputerScannerToken::Xor,
                                  parseEqualityExpression);
}

//==============================================================================

bool parseEqualityExpression(ComputerParser *pParser,
                             ComputerEquation * &pExpression)
{
    // The EBNF grammar of an equality expression is as follows:
    //
    //   EqualityExpression = [ EqualityExpression ( "==" | "!=" ) ] RelationalExpression ;

    return parseGenericExpression(pParser, pExpression,
                                  ComputerScannerToken::Symbols() << ComputerScannerToken::EqualEqual
                                                                  << ComputerScannerToken::NotEqual,
                                  parseRelationalExpression);
}

//==============================================================================

bool parseRelationalExpression(ComputerParser *pParser,
                               ComputerEquation * &pExpression)
{
    // The EBNF grammar of a relational expression is as follows:
    //
    //   RelationalExpression = [ RelationalExpression ( "<" | ">" | "<=" | ">=" ) ] AdditiveExpression ;

    return parseGenericExpression(pParser, pExpression,
                                  ComputerScannerToken::Symbols() << ComputerScannerToken::LowerThan
                                                                  << ComputerScannerToken::GreaterThan
                                                                  << ComputerScannerToken::LowerOrEqualThan
                                                                  << ComputerScannerToken::GreaterOrEqualThan,
                                  parseAdditiveExpression);
}

//==============================================================================

bool parseAdditiveExpression(ComputerParser *pParser,
                             ComputerEquation * &pExpression)
{
    // The EBNF grammar of an additive expression is as follows:
    //
    //   AdditiveExpression = [ AdditiveExpression ( "+" | "-" ) ] MultiplicativeExpression ;

    return parseGenericExpression(pParser, pExpression,
                                  ComputerScannerToken::Symbols() << ComputerScannerToken::Plus
                                                                  << ComputerScannerToken::Minus,
                                  parseMultiplicativeExpression);
}

//==============================================================================

bool parseMultiplicativeExpression(ComputerParser *pParser,
                                   ComputerEquation * &pExpression)
{
    // The EBNF grammar of a multiplicative expression is as follows:
    //
    //   MultiplicativeExpression = [ MultiplicativeExpression ( "*" | "/" | "%" ) ] UnaryExpression ;

    return parseGenericExpression(pParser, pExpression,
                                  ComputerScannerToken::Symbols() << ComputerScannerToken::Times
                                                                  << ComputerScannerToken::Divide
                                                                  << ComputerScannerToken::Modulo,
                                  parseUnaryExpression);
}

//==============================================================================

bool parseUnaryExpression(ComputerParser *pParser, ComputerEquation *&pExpression)
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

    if (!parsePrimaryExpression(pParser, pExpression))
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
                            ComputerEquation * &pExpression)
{
    // The EBNF grammar of a primary expression is as follows:
    //
    //   PrimaryExpression              =   Parameter
    //                                    | IntegerValue
    //                                    | DoubleValue
    //                                    | ( FunctionWithOneArgument "(" EquationRHS ")" ) ;
    //                                    | ( FunctionWithTwoArguments "(" EquationRHS "," EquationRHS ")" ) ;
    //                                    | ( FunctionWithTwoOrMoreArguments "(" EquationRHS "," EquationRHS { "," Equation } ")" ) ;
    //                                    | ( "defint" "(" EquationRHS "," EquationRHS "," EquationRHS "," Identifier ")" ) ;
    //                                    | ( "(" EquationRHS ")" ) ;
    //   FunctionWithOneArgument        =   "fabs" | "exp" | "log" | "ceil" | "floor" | "factorial"
    //                                    | "sin" | "cos" | "tan" | "sinh" | "cosh" | "tanh"
    //                                    | "asin" | "acos" | "atan" | "asinh" | "acosh" | "atanh" ;
    //   FunctionWithTwoArguments       = "arbitraryLog" | "pow" | "quot" | "rem" ;
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
                                                                                                            << ComputerScannerToken::Sinh
                                                                                                            << ComputerScannerToken::Cosh
                                                                                                            << ComputerScannerToken::Tanh
                                                                                                            << ComputerScannerToken::Asin
                                                                                                            << ComputerScannerToken::Acos
                                                                                                            << ComputerScannerToken::Atan
                                                                                                            << ComputerScannerToken::Asinh
                                                                                                            << ComputerScannerToken::Acosh
                                                                                                            << ComputerScannerToken::Atanh;
    static const ComputerScannerToken::Symbols twoArgumentFunctionSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::ArbitraryLog
                                                                                                            << ComputerScannerToken::Pow
                                                                                                            << ComputerScannerToken::Quot
                                                                                                            << ComputerScannerToken::Rem;
    static const ComputerScannerToken::Symbols twoOrMoreArgumentFunctionSymbols = ComputerScannerToken::Symbols() << ComputerScannerToken::Gcd
                                                                                                          << ComputerScannerToken::Lcm
                                                                                                          << ComputerScannerToken::Max
                                                                                                          << ComputerScannerToken::Min;

    if (pParser->scanner()->token().symbol() == ComputerScannerToken::Identifier) {
        // We are dealing with an identifier which corresponds to the name of a
        // parameter, so keep track of it and get the next token

        if (!parseParameter(pParser, pExpression))
            // Something went wrong with the parsing of a parameter, so...

            return false;
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
            pParser->setError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argument = 0;

        if (!pParser->parseRhsEquation(argument)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argument;

            return false;
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->setError("')'");

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
            pParser->setError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentOne = 0;

        if (!pParser->parseRhsEquation(argumentOne)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argumentOne;

            return false;
        }

        // The current token must be ","

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
            pParser->setError("','");

            delete argumentOne;

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentTwo = 0;

        if (!pParser->parseRhsEquation(argumentTwo)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argumentOne;
            delete argumentTwo;

            return false;
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->setError("')'");

            delete argumentOne;
            delete argumentTwo;

            return false;
        }

        pParser->scanner()->getNextToken();

        // The parsing of our two-argument function went fine, so...

        pExpression = new ComputerEquation(equationType,
                                           argumentOne, argumentTwo);
    } else if (twoOrMoreArgumentFunctionSymbols.contains(pParser->scanner()->token().symbol())) {
        // We are dealing with a two or more arguments function, so...

        ComputerEquation::Type equationType = pParser->scanner()->token().equationType();

        pParser->scanner()->getNextToken();

        // The current token must be "("

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::OpeningBracket) {
            pParser->setError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // The current token must an integer

        int argumentsCount = -1;

        if (pParser->scanner()->token().symbol() == ComputerScannerToken::IntegerValue)
            // We got an integer value, so set the number of arguments with it

            argumentsCount = pParser->scanner()->token().string().toInt();

        if (argumentsCount < 0) {
            pParser->setError(QObject::tr("a positive integer"));

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation and this as many times as there are
        // arguments according to argumentsCount

        ComputerEquations arguments;

        for (int i = 0; i < argumentsCount; ++i) {
            arguments << 0;

            // The current token must be ","

            if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
                pParser->setError("','");

                for (int j = 0; j < i; ++j)
                    delete arguments.at(j);

                return false;
            }

            pParser->scanner()->getNextToken();

            // Parse the RHS of an equation

            ComputerEquation *computerEquation = arguments.at(i);

            if (!pParser->parseRhsEquation(computerEquation)) {
                // Something went wrong with the parsing of the RHS of an
                // equation, so...

                for (int j = 0; j <= i; ++j)
                    delete arguments.at(j);

                return false;
            }
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->setError("')'");

            for (int i = 0; i < argumentsCount; ++i)
                delete arguments.at(i);

            return false;
        }

        pParser->scanner()->getNextToken();

        // The parsing of our X-argument function went fine, so...

        pExpression = new ComputerEquation(equationType, arguments);
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::DefInt) {
        // We are dealing with a definite integral function, so...

//---GRY--- THE BELOW CODE IS TO REMOVED ONCE DEFINITE INTEGRALS ARE SUPPORTED
//          BY OpenCOR...

pParser->setError(QObject::tr("definite integrals are not yet supported"), false);

        ComputerEquation::Type equationType = pParser->scanner()->token().equationType();

        pParser->scanner()->getNextToken();

        // The current token must be "("

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::OpeningBracket) {
            pParser->setError("'('");

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentOne = 0;

        if (!pParser->parseRhsEquation(argumentOne)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argumentOne;

            return false;
        }

        // The current token must be ","

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
            pParser->setError("','");

            delete argumentOne;

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentTwo = 0;

        if (!pParser->parseRhsEquation(argumentTwo)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argumentOne;
            delete argumentTwo;

            return false;
        }

        // The current token must be ","

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
            pParser->setError("','");

            delete argumentOne;
            delete argumentTwo;

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentThree = 0;

        if (!pParser->parseRhsEquation(argumentThree)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete argumentOne;
            delete argumentTwo;
            delete argumentThree;

            return false;
        }

        // The current token must be ","

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::Comma) {
            pParser->setError("','");

            delete argumentOne;
            delete argumentTwo;
            delete argumentThree;

            return false;
        }

        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *argumentFour = 0;

        if (!parseParameter(pParser, argumentFour)) {
            // Something went wrong with the parsing of a parameter, so...

            delete argumentOne;
            delete argumentTwo;
            delete argumentThree;
            delete argumentFour;

            return false;
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->setError("')'");

            delete argumentOne;
            delete argumentTwo;
            delete argumentThree;
            delete argumentFour;

            return false;
        }

        pParser->scanner()->getNextToken();

        // The parsing of our two-argument function went fine, so...

        pExpression = new ComputerEquation(equationType,
                                           argumentOne, argumentTwo,
                                           argumentThree, argumentFour);
    } else if (pParser->scanner()->token().symbol() == ComputerScannerToken::OpeningBracket) {
        pParser->scanner()->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *equation = 0;

        if (!pParser->parseRhsEquation(equation)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete equation;

            return false;
        }

        // The current token must be ")"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingBracket) {
            pParser->setError("')'");

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

bool parseParameter(ComputerParser *pParser, ComputerEquation * &pExpression)
{
    // The EBNF grammar of a function parameter is as follows:
    //
    //   Parameter = Identifier [ "[" IntegerValue "]" ]

    // The current token must be an identifier

    QString parameterName = QString();

    if (pParser->scanner()->token().symbol() == ComputerScannerToken::Identifier) {
        // We got an identifier, so retrieve its name

        parameterName = pParser->scanner()->token().string();
    } else {
        pParser->setError(QObject::tr("an identifier"));

        return false;
    }

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
            pParser->setError(QObject::tr("a positive integer"));

            return false;
        }

        pParser->scanner()->getNextToken();

        // The current token must be "]"

        if (pParser->scanner()->token().symbol() != ComputerScannerToken::ClosingSquareBracket) {
            pParser->setError("']'");

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

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerParser::parseRhsEquation(ComputerEquation * &pRhsEquation)
{
    // The EBNF grammar of an equation's RHS is as follows:
    //
    //   EquationRHS =   OrExpression
    //                 | ( OrExpression "?" EquationRHS ":" EquationRHS ) ;

    // Parse a Or expression

    ComputerEquation *mainOrConditionEquation = 0;

    if (!parseOrExpression(this, mainOrConditionEquation)) {
        // Something went wrong with the parsing of an Or expression, so...

        setError(tr("the RHS of an equation"));

        delete mainOrConditionEquation;

        return false;
    }

    // Check whether the current token is "?"

    if (mScanner->token().symbol() == ComputerScannerToken::QuestionMark) {
        // We got "?" which means that we should have a conditional statement

        mScanner->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *trueCaseEquation = 0;

        if (!parseRhsEquation(trueCaseEquation)) {
            // Something went wrong with the parsing of the RHS of an equation,
            // so...

            delete mainOrConditionEquation;
            delete trueCaseEquation;

            return false;
        }

        // The current token must be ":"

        if (mScanner->token().symbol() != ComputerScannerToken::Colon) {
            setError("':'");

            delete mainOrConditionEquation;
            delete trueCaseEquation;

            return false;
        }

        mScanner->getNextToken();

        // Parse the RHS of an equation

        ComputerEquation *falseCaseEquation = 0;

        if (!parseRhsEquation(falseCaseEquation)) {
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

bool ComputerParser::parseReturnStatement(ComputerFunction *pFunction)
{
    // The EBNF grammar of a return statement is as follows:
    //
    //   Return = "return" EquationRHS ";" ;

    // The current token must be "return"

    if (mScanner->token().symbol() != ComputerScannerToken::Return) {
        setError(tr("an identifier or 'return'"));

        return false;
    }

    mScanner->getNextToken();

    // Parse the equivalent of the RHS of an equation

    ComputerEquation *equation = 0;

    if (!parseRhsEquation(equation)) {
        // Something went wrong with the parsing of the RHS of an equation,
        // so...

        delete equation;

        return false;
    }

    // The current token must be ";"

    if (mScanner->token().symbol() != ComputerScannerToken::SemiColon) {
        setError("';'");

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
