PROJECT(PythonQtPlugin)


# Qt 5.4 wrappers are used for Qt 5.6

set(generated_cpp_suffix "_54")


# Definitions for PythonQt

set(DEFINITIONS PYTHONQT_EXPORTS
                PYTHONQT_SUPPORT_NAME_PROPERTY
   )

option(PythonQt_DEBUG "Enable/Disable PythonQt debug output" OFF)
if(PythonQt_DEBUG)
  set(DEFINITIONS ${DEFINITIONS} PYTHONQT_DEBUG)
endif()


# PythonQt sources

set(PythonQt_SOURCES
    PythonQt/src/PythonQtBoolResult.cpp
    PythonQt/src/PythonQtClassInfo.cpp
    PythonQt/src/PythonQtClassWrapper.cpp
    PythonQt/src/PythonQtConversion.cpp
    PythonQt/src/PythonQt.cpp
    PythonQt/src/PythonQtImporter.cpp
    PythonQt/src/PythonQtInstanceWrapper.cpp
    PythonQt/src/PythonQtMethodInfo.cpp
    PythonQt/src/PythonQtMisc.cpp
    PythonQt/src/PythonQtObjectPtr.cpp
    PythonQt/src/PythonQtQFileImporter.cpp
    PythonQt/src/PythonQtSignalReceiver.cpp
    PythonQt/src/PythonQtSlot.cpp
    PythonQt/src/PythonQtSignal.cpp
    PythonQt/src/PythonQtStdDecorators.cpp
    PythonQt/src/PythonQtStdIn.cpp
    PythonQt/src/PythonQtStdOut.cpp
    PythonQt/src/gui/PythonQtScriptingConsole.cpp

    PythonQt/generated_cpp${generated_cpp_suffix}/PythonQt_QtBindings.cpp

    PythonQt/generated_cpp${generated_cpp_suffix}/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin0.cpp
    PythonQt/generated_cpp${generated_cpp_suffix}/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin_init.cpp
    PythonQt/generated_cpp${generated_cpp_suffix}/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin0.cpp
    PythonQt/generated_cpp${generated_cpp_suffix}/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin_init.cpp
)

# PythonQt moc headers

set(Python_HEADERS_MOC
    PythonQt/src/PythonQt.h
    PythonQt/src/PythonQtSignalReceiver.h
    PythonQt/src/PythonQtStdDecorators.h
    PythonQt/src/gui/PythonQtScriptingConsole.h

    PythonQt/generated_cpp${generated_cpp_suffix}/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin0.h
    PythonQt/generated_cpp${generated_cpp_suffix}/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin0.h
)

if(WIN32)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /bigobj")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()


# Add the plugin

ADD_PLUGIN(PythonQt
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/pythonqtplugin.cpp
        ${PythonQt_SOURCES}
    HEADERS_MOC
        src/pythonqtplugin.h
        ${PythonQt_HEADERS_MOC}
    INCLUDE_DIRS
        src
        PythonQt/src
        ${PYTHON_INCLUDE_DIRS}
    PLUGINS
        Core
        Python
    PLUGIN_BINARIES
        ${PYTHON_PLUGIN_BINARY}
    QT_MODULES
        Core
    DEFINITIONS
        ${DEFINITIONS}
)
