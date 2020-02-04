static PyObject * openSimulation(PyObject *pSelf, PyObject *pArgs)
{
    Q_UNUSED(pSelf)

    // Retrieve the file name of the simulation to open

    PyObject *bytes;

    if (PyArg_ParseTuple(pArgs, "O&", PyUnicode_FSConverter, &bytes) == 0) { // NOLINT(cppcoreguidelines-pro-type-vararg)
#include "pythonbegin.h"
        Py_RETURN_NONE;
#include "pythonend.h"
    }

    char *string;
    Py_ssize_t len;

    PyBytes_AsStringAndSize(bytes, &string, &len);

    bool isLocalFile;
    QString fileNameOrUrl;

    Core::checkFileNameOrUrl(QString::fromUtf8(string, int(len)), isLocalFile, fileNameOrUrl);

#include "pythonbegin.h"
    Py_DECREF(bytes);
#include "pythonend.h"

    // Open the simulation

    QString error = isLocalFile?
#ifdef GUI_SUPPORT
                        Core::guiOpenFile(fileNameOrUrl,
                                          Core::File::Type::Local, {}, false):
                        Core::guiOpenRemoteFile(fileNameOrUrl, false);
#else
                        Core::cliOpenFile(fileNameOrUrl):
                        Core::cliOpenRemoteFile(fileNameOrUrl);
#endif

    if (!error.isEmpty()) {
        PyErr_SetString(PyExc_IOError, qPrintable(error));

        return nullptr;
    }

    return initializeSimulation(isLocalFile?
                                    fileNameOrUrl:
                                    Core::FileManager::instance()->fileName(fileNameOrUrl));
}
