//==============================================================================
// Computer equation class
//==============================================================================

#include "computerequation.h"

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

    // Simplify the current node based on the contents of the left and right
    // nodes

    switch (pNode->type()) {
    case Times:
        // Simplification of a multiplication requires both the left and right
        // nodes to be numbers

        if (   (pNode->left()->type() == Number)
            && (pNode->right()->type() == Number))
            // We can simplify, so...

            resetNodeAsNumber(pNode, pNode->left()->number()*pNode->right()->number());

        break;
    }
}

//==============================================================================

void ComputerEquation::resetNodeAsNumber(ComputerEquation *pNode,
                                         const double &pNumber)
{
    // Reset the given node as a number

    pNode->mType = Number;

    pNode->mNumber = pNumber;

    delete pNode->mLeft;
    delete pNode->mRight;

    pNode->mLeft  = 0;
    pNode->mRight = 0;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
