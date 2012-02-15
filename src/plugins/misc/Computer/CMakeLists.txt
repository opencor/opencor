PROJECT(ComputerPlugin)

# Retrieve some LLVM settings

RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(Computer
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/computerengine.cpp
        src/computerequation.cpp
        src/computererror.cpp
        src/computerexternalfunction.cpp
        src/computerfunction.cpp
        src/computermath.cpp
        src/computerparameter.cpp
        src/computerparser.cpp
        src/computerplugin.cpp
        src/computerscanner.cpp
    HEADERS_MOC
        src/computerengine.h
        src/computerparser.h
        src/computerplugin.h
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        LLVM
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${LLVM_EXTERNAL_DEPENDENCIES}
    TESTS
        test
)
