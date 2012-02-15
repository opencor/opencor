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

class Test : public QObject
{
    Q_OBJECT

private:
    OpenCOR::Computer::ComputerEngine *mComputerEngine;

private slots:
    void initTestCase();
    void cleanupTestCase();
};

//==============================================================================
// End of file
//==============================================================================
