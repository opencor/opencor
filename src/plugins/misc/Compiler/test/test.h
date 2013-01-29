//==============================================================================
// Compiler test
//==============================================================================

#include <QtGlobal>

//==============================================================================

#ifdef Q_OS_MAC
    #pragma GCC diagnostic ignored "-Wunused-private-field"
#endif

#include <QtTest/QtTest>

#ifdef Q_OS_MAC
    #pragma GCC diagnostic warning "-Wunused-private-field"
#endif

//==============================================================================

namespace llvm {
    class Function;
}   // namespace llvm

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Compiler {
    class CompilerEngine;
}   // namespace Compiler

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

class Test : public QObject
{
    Q_OBJECT

private:
    OpenCOR::Compiler::CompilerEngine *mCompilerEngine;

private Q_SLOTS:
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

    void absFunctionTests();
    void expFunctionTests();
    void logFunctionTests();
    void ceilFunctionTests();
    void floorFunctionTests();
    void factorialFunctionTests();
    void sinFunctionTests();
    void cosFunctionTests();
    void tanFunctionTests();
    void sinhFunctionTests();
    void coshFunctionTests();
    void tanhFunctionTests();
    void asinFunctionTests();
    void acosFunctionTests();
    void atanFunctionTests();
    void asinhFunctionTests();
    void acoshFunctionTests();
    void atanhFunctionTests();

    void arbitraryLogFunctionTests();
    void powFunctionTests();

    void gcdFunctionTests();
    void lcmFunctionTests();
    void maxFunctionTests();
    void minFunctionTests();

    void defIntFunctionTests();
};

//==============================================================================
// End of file
//==============================================================================
