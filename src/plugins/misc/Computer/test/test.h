//==============================================================================
// Computer test
//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

namespace OpenCOR {
namespace Computer {
    class ComputerEngine;
}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

namespace llvm {
    class Function;
}

//==============================================================================

class Test : public QObject
{
    Q_OBJECT

private:
    OpenCOR::Computer::ComputerEngine *mComputerEngine;

    llvm::Function * mFunction;
    double mResult;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void basicTests();

    void voidFunctionTests();

    void timesOperatorTests();
    void divideOperatorTests();
    void moduloOperatorTests();
    void plusOperatorTests();
    void minusOperatorTests();

    void notOperatorTests();
    void orOperatorTests();
    void xorOperatorTests();
    void andOperatorTests();
    void equalEqualOperatorTests();
    void notEqualOperatorTests();
    void lowerThanOperatorTests();
    void greaterThanOperatorTests();
    void lowerOrEqualThanOperatorTests();
    void greaterOrEqualThanOperatorTests();
};

//==============================================================================
// End of file
//==============================================================================
