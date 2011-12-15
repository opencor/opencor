PROJECT(CellMLSupportPlugin)

# Retrieve some CellML and LLVM settings

RETRIEVE_CELLML_SETTINGS()
RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellMLSupport
    SOURCES
        ../../fileinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/cellmlmodel.cpp
        src/cellmlmodelcommon.cpp
        src/cellmlmodelruntime.cpp
        src/cellmlmodelmanager.cpp
        src/cellmlsupportplugin.cpp
    HEADERS_MOC
        src/cellmlmodel.h
        src/cellmlsupportplugin.h
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        CellML
        LLVM
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${CELLML_EXTERNAL_DEPENDENCIES}
        ${LLVM_EXTERNAL_DEPENDENCIES}
)
