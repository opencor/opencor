//==============================================================================
// Computer equation class
//==============================================================================

#include "computerequation.h"

//==============================================================================

#include <math.h>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEquation::ComputerEquation(ComputerEquation *pEquation)
{
    // Initialise the equation using pEquation

    initialiseFrom(pEquation);
}

//==============================================================================

ComputerEquation::ComputerEquation(const Type &pType,
                                   ComputerEquation *pLeft,
                                   ComputerEquation *pRight) :
    mType(pType),
    mParameterName(QString()),
    mParameterIndex(-1),
    mNumber(0),
    mLeft(pLeft),
    mRight(pRight)
{
}

//==============================================================================

ComputerEquation::ComputerEquation(const QString &pParameterName) :
    mType(DirectParameter),
    mParameterName(pParameterName),
    mParameterIndex(-1),
    mNumber(0),
    mLeft(0),
    mRight(0)
{
}

//==============================================================================

ComputerEquation::ComputerEquation(const QString &pParameterName,
                                   const int &pParameterIndex) :
    mType(IndirectParameter),
    mParameterName(pParameterName),
    mParameterIndex(pParameterIndex),
    mNumber(0),
    mLeft(0),
    mRight(0)
{
}

//==============================================================================

ComputerEquation::ComputerEquation(const double &pNumber) :
    mType(Number),
    mParameterName(QString()),
    mParameterIndex(-1),
    mNumber(pNumber),
    mLeft(0),
    mRight(0)
{
}

//==============================================================================

ComputerEquation::~ComputerEquation()
{
    // Delete some internal objects

    delete mLeft;
    delete mRight;
}

//==============================================================================

ComputerEquation::Type ComputerEquation::type() const
{
    // Return the equation's type

    return mType;
}

//==============================================================================

QString ComputerEquation::typeAsString() const
{
    // Return the equation's type as a string
    // Note: this method is mainly used for debugging purposes, though who knows
    //       it may someday prove useful in some other cases...

    switch (mType) {
    case Unknown:
        return "Unknown";
    case DirectParameter:
        return "DirectParameter";
    case IndirectParameter:
        return "IndirectParameter";
    case Number:
        return "Number";
    case Times:
        return "Times";
    case Divide:
        return "Divide";
    case Modulo:
        return "Modulo";
    case Plus:
        return "Plus";
    case Minus:
        return "Minus";
    case LogicalOr:
        return "LogicalOr";
    case LogicalAnd:
        return "LogicalAnd";
    case EqualEqual:
        return "EqualEqual";
    case NotEqual:
        return "NotEqual";
    case LowerThan:
        return "LowerThan";
    case GreaterThan:
        return "GreaterThan";
    case LowerOrEqualThan:
        return "LowerOrEqualThan";
    case GreaterOrEqualThan:
        return "GreaterOrEqualThan";
    case Abs:
        return "Abs";
    case Exp:
        return "Exp";
    case Sin:
        return "Sin";
    case Cos:
        return "Cos";
    case Tan:
        return "Tan";
    case SinH:
        return "SinH";
    case CosH:
        return "CosH";
    case TanH:
        return "TanH";
    case ASin:
        return "ASin";
    case ACos:
        return "ACos";
    case ATan:
        return "ATan";
    case ASinH:
        return "ASinH";
    case ACosH:
        return "ACosH";
    case ATanH:
        return "ATanH";
    case Pow:
        return "Pow";
    case Assign:
        return "Assign";
    case Not:
        return "Not";
    case Piecewise:
        return "Piecewise";
    case PiecewiseCases:
        return "PiecewiseCases";
    default:
        return "???";
    }
}

//==============================================================================

QString ComputerEquation::parameterName() const
{
    // Return the equation's parameter name

    return mParameterName;
}

//==============================================================================

int ComputerEquation::parameterIndex() const
{
    // Return the equation's parameter index

    return mParameterIndex;
}

//==============================================================================

double ComputerEquation::number() const
{
    // Return the equation's number

    return mNumber;
}

//==============================================================================

ComputerEquation * ComputerEquation::left() const
{
    // Return the equation's left branch

    return mLeft;
}

//==============================================================================

ComputerEquation * ComputerEquation::right() const
{
    // Return the equation's right branch

    return mRight;
}

//==============================================================================

void ComputerEquation::initialiseFrom(ComputerEquation *pEquation)
{
    // Initialise the equation using pEquation, if possible

    mType = pEquation?pEquation->type():Unknown;

    mParameterName  = pEquation?pEquation->parameterName():QString();
    mParameterIndex = pEquation?pEquation->parameterIndex():-1;

    mNumber = pEquation?pEquation->number():0;

    if (pEquation && pEquation->left()) {
        mLeft = new ComputerEquation();

        mLeft->initialiseFrom(pEquation->left());
    } else {
        mLeft = 0;
    }

    if (pEquation && pEquation->right()) {
        mRight = new ComputerEquation();

        mRight->initialiseFrom(pEquation->right());
    } else {
        mRight = 0;
    }
}

//==============================================================================

void ComputerEquation::simplify()
{
    // Simplify the equation starting from its top node

    simplifyNode(this);
}

//==============================================================================

void ComputerEquation::simplifyNode(ComputerEquation *pNode)
{
    // Make sure that the node is valid

    if (!pNode)
        // It isn't, so...

        return;

    // Simplify the left node

    simplifyNode(pNode->left());

    // Simplify the right node

    simplifyNode(pNode->right());

    // Simplify the current node based on the contents of its left and right
    // nodes

    switch (pNode->type()) {
    case Times:
        if ((pNode->left()->type() == Number) && (pNode->right()->type() == Number))
            // N1*N2

            replaceNodeWithNumber(pNode, pNode->left()->number()*pNode->right()->number());
        else if ((pNode->left()->type() == Number) && (pNode->left()->number() == 1))
            // 1*X ---> X

            replaceNodeWithChildNode(pNode, pNode->right());
        else if ((pNode->right()->type() == Number) && (pNode->right()->number() == 1))
            // X*1 ---> X

            replaceNodeWithChildNode(pNode, pNode->left());

        break;
    case Divide:
        if (   (pNode->left()->type() == Number)
            && (pNode->right()->type() == Number))
            // N1/N2

            replaceNodeWithNumber(pNode, pNode->left()->number()/pNode->right()->number());
        else if ((pNode->right()->type() == Number) && (pNode->right()->number() == 1))
            // X/1 ---> X

            replaceNodeWithChildNode(pNode, pNode->left());

        break;
    case Plus:
        if (!pNode->right())
            // +X ---> X

            replaceNodeWithChildNode(pNode, pNode->left());
        else if ((pNode->left()->type() == Number) && (pNode->right()->type() == Number))
            // N1+N2

            replaceNodeWithNumber(pNode, pNode->left()->number()+pNode->right()->number());
        else if ((pNode->left()->type() == Number) && (pNode->left()->number() == 0))
            // 0+X ---> X

            replaceNodeWithChildNode(pNode, pNode->right());
        else if ((pNode->right()->type() == Number) && (pNode->right()->number() == 0))
            // X+0 ---> X

            replaceNodeWithChildNode(pNode, pNode->left());

        break;
    case Minus:
        if (!pNode->right()) {
            if (pNode->left()->type() == Number)
                // -N

                replaceNodeWithNumber(pNode, -pNode->left()->number());
        } else if ((pNode->left()->type() == Number) && (pNode->right()->type() == Number)) {
            // N1-N2

            replaceNodeWithNumber(pNode, pNode->left()->number()-pNode->right()->number());
        } else if ((pNode->right()->type() == Number) && (pNode->right()->number() == 0)) {
            // X-0 ---> X

            replaceNodeWithChildNode(pNode, pNode->left());
        }

        break;
    case Abs:
        if (pNode->left()->type() == Number)
            // abs(N)

            replaceNodeWithNumber(pNode, fabs(pNode->left()->number()));

        break;
    case Exp:
        if (pNode->left()->type() == Number)
            // exp(N)

            replaceNodeWithNumber(pNode, exp(pNode->left()->number()));

        break;
    case Sin:
        if (pNode->left()->type() == Number)
            // sin(N)

            replaceNodeWithNumber(pNode, sin(pNode->left()->number()));

        break;
    case Cos:
        if (pNode->left()->type() == Number)
            // cos(N)

            replaceNodeWithNumber(pNode, cos(pNode->left()->number()));

        break;
    case Tan:
        if (pNode->left()->type() == Number)
            // tan(N)

            replaceNodeWithNumber(pNode, tan(pNode->left()->number()));

        break;
    case SinH:
        if (pNode->left()->type() == Number)
            // sinh(N)

            replaceNodeWithNumber(pNode, sinh(pNode->left()->number()));

        break;
    case CosH:
        if (pNode->left()->type() == Number)
            // cosh(N)

            replaceNodeWithNumber(pNode, cosh(pNode->left()->number()));

        break;
    case TanH:
        if (pNode->left()->type() == Number)
            // tanh(N)

            replaceNodeWithNumber(pNode, tanh(pNode->left()->number()));

        break;
    case ASin:
        if (pNode->left()->type() == Number)
            // asin(N)

            replaceNodeWithNumber(pNode, asin(pNode->left()->number()));

        break;
    case ACos:
        if (pNode->left()->type() == Number)
            // acos(N)

            replaceNodeWithNumber(pNode, acos(pNode->left()->number()));

        break;
    case ATan:
        if (pNode->left()->type() == Number)
            // atan(N)

            replaceNodeWithNumber(pNode, atan(pNode->left()->number()));

        break;
    case ASinH:
        if (pNode->left()->type() == Number)
            // asinh(N)

            replaceNodeWithNumber(pNode, asinh(pNode->left()->number()));

        break;
    case ACosH:
        if (pNode->left()->type() == Number)
            // acosh(N)

            replaceNodeWithNumber(pNode, acosh(pNode->left()->number()));

        break;
    case ATanH:
        if (pNode->left()->type() == Number)
            // atanh(N)

            replaceNodeWithNumber(pNode, atanh(pNode->left()->number()));

        break;
    case Pow:
        // Note: we could support further simplifications (well, optimisations)
        //       such as pow(x, 2) = x*x, but well... maybe someday...

        if ((pNode->left()->type() == Number) && (pNode->right()->type() == Number))
            // N1^N2

            replaceNodeWithNumber(pNode, pow(pNode->left()->number(), pNode->right()->number()));

        break;
    case Not:
        if (pNode->left()->type() == Number)
            // !N

            replaceNodeWithNumber(pNode, pNode->left()->number()?0:1);

        break;
    }
}

//==============================================================================

void ComputerEquation::replaceNodeWithNumber(ComputerEquation *pNode,
                                             const double &pNumber)
{
    // Replace the given node with a number

    pNode->mType = Number;

    pNode->mNumber = pNumber;

    delete pNode->mLeft;
    delete pNode->mRight;

    pNode->mLeft  = 0;
    pNode->mRight = 0;
}

//==============================================================================

void ComputerEquation::replaceNodeWithChildNode(ComputerEquation *pNode,
                                                ComputerEquation *pChildNode)
{
    // Replace the given node with one of its child nodes

    // Keep track of the node's child nodes

    ComputerEquation *leftChildNode  = pNode->left();
    ComputerEquation *rightChildNode = pNode->right();

    // Make sure that pChildNode is indeed one of the node's child nodes

    if ((pChildNode != leftChildNode) && (pChildNode != rightChildNode))
        // pChildNode is not one of the node's child nodes, so...

        return;

    // Initialise the node using its child node

    pNode->initialiseFrom(pChildNode);

    // Delete the child nodes now that they are not needed anymore

    delete leftChildNode;
    delete rightChildNode;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
