static PyObject * closeSimulation(PyObject *pSelf, PyObject *pArgs)
{
    Q_UNUSED(pSelf)

    // Close a simulation

    if (PyTuple_Size(pArgs) == 1) {
#include "pythonbegin.h"
        PythonQtInstanceWrapper *wrappedSimulation = PythonQtSupport::getInstanceWrapper(PyTuple_GET_ITEM(pArgs, 0)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
#include "pythonend.h"

        if (wrappedSimulation != nullptr) {
#ifdef GUI_SUPPORT
            // Close the simulation by closing its file, raising an exception if
            // we were unable to do so

            auto simulation = static_cast<SimulationSupport::Simulation *>(wrappedSimulation->_objPointerCopy);

            if (!Core::guiCloseFile(simulation->fileName())) {
                PyErr_SetString(PyExc_IOError, qPrintable(QObject::tr("unable to close the simulation.")));

                return nullptr;
            }
#else
            // Close the simulation by asking our file and simulation managers
            // to umanage it

            auto simulation = static_cast<SimulationSupport::Simulation *>(wrappedSimulation->_objPointerCopy);
            QString fileName = simulation->fileName();

            Core::FileManager::instance()->unmanage(fileName);
            SimulationManager::instance()->unmanage(fileName);
#endif
        }
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}
