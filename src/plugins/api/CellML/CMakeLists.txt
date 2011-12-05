PROJECT(CellMLPlugin)

MACRO(RETRIEVE_CELLML_SETTINGS)
    # Retrieve CellML settings

    SET(CELLML_EXTERNAL_DEPENDENCIES
        libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
        libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
        libceleds${CMAKE_SHARED_LIBRARY_SUFFIX}
        libceledsexporter${CMAKE_SHARED_LIBRARY_SUFFIX}
        libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
        libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
        libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
        libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
        libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
        libspros${CMAKE_SHARED_LIBRARY_SUFFIX}
        libsrus${CMAKE_SHARED_LIBRARY_SUFFIX}
        libtelicems${CMAKE_SHARED_LIBRARY_SUFFIX}
        libvacss${CMAKE_SHARED_LIBRARY_SUFFIX}
        libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ENDMACRO()

# CellML libraries

SET(CELLML_EXTERNAL_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/lib/${DISTRIB_DIR})

RETRIEVE_CELLML_SETTINGS()

# Add the plugin

ADD_PLUGIN(CellML
    SOURCES
        ../../coreinterface.cpp
        ../../fileinterface.cpp
        ../../plugininfo.cpp

        src/cellmlplugin.cpp
    HEADERS_MOC
        src/cellmlplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES_DIR
        ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
    EXTERNAL_DEPENDENCIES
        ${CELLML_EXTERNAL_DEPENDENCIES}
)

# Deploy the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # The CellML libraries

    FOREACH(CELLML_EXTERNAL_DEPENDENCY ${CELLML_EXTERNAL_DEPENDENCIES})
        DEPLOY_WINDOWS_BINARY_FILE(${CELLML_EXTERNAL_DEPENDENCIES_DIR}/${CELLML_EXTERNAL_DEPENDENCY})
    ENDFOREACH()

    # The libstdc++-6.dll file
    # Note: this DLL comes from the official version of MinGW (as opposed to
    #       the Qt patched version) and is required by the CellML API, so...

    DEPLOY_WINDOWS_BINARY_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/x86/libstdc++-6.dll)
ELSEIF(APPLE)
    # Deploy the CellML libraries together with their respective CellML
    # dependencies, if any

    DEPLOY_MAC_OS_X_LIBRARY(libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libceleds${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libceledsexporter${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
            libceleds${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libspros${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libsrus${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
            libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libtelicems${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libvacss${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    # The CellML binaries

    FOREACH(CELLML_EXTERNAL_DEPENDENCY ${CELLML_EXTERNAL_DEPENDENCIES})
        INSTALL(FILES ${CELLML_EXTERNAL_DEPENDENCIES_DIR}/${CELLML_EXTERNAL_DEPENDENCY} DESTINATION lib)
    ENDFOREACH()
ENDIF()
