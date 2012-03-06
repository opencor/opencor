//==============================================================================
// Solver interface
//==============================================================================

#ifndef SOLVERINTERFACE_H
#define SOLVERINTERFACE_H

//==============================================================================

#include "interface.h"

//==============================================================================

#include <QMap>
#include <QTranslator>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class SolverInterface : Interface
{
    friend class MainWindow;

public:
    enum SolverInterfaceType
    {
        Ode,
        Dae
    };

    enum SovlerInterfacePropertyType {
        Double
    };

    typedef QMap<QString, SovlerInterfacePropertyType> Properties;

    virtual QString name() const = 0;
    virtual SolverInterfaceType type() const = 0;
    virtual Properties properties() const = 0;

    virtual void * newSolver() const = 0;
};

//==============================================================================

typedef QList<SolverInterface *> SolverInterfaces;

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::SolverInterface, "OpenCOR.SolverInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
