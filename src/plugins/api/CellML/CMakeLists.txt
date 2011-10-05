PROJECT(CellMLPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Determine the location of the CellML libraries

IF(WIN32)
    SET(CELLML_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/windows/x86)
ELSEIF(APPLE)
    SET(CELLML_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/macosx)
ELSE()
    IF(64BIT_MODE)
        SET(CELLML_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/linux/x64)
    ELSE()
        SET(CELLML_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/linux/x86)
    ENDIF()
ENDIF()

# CellML libraries themselves

SET(CELLML_LIBS
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

# Add the plugin

ADD_PLUGIN(CellML
    SOURCES
        ../../coreinterface.cpp
        ../../fileinterface.cpp
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/cellmlplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../fileinterface.h
        ../../i18ninterface.h
        ../../plugininfo.h

        AnnoToolsBootstrap.hpp
        CCGSBootstrap.hpp
        cda_compiler_support.h
        cda_config.h
        CeLEDSBootstrap.hpp
        CeLEDSExporterBootstrap.hpp
        cellml-api-cxx-support.hpp
        CellMLBootstrap.hpp
        CeVASBootstrap.hpp
        CISBootstrap.hpp
        CUSESBootstrap.hpp
        DOMBootstrap.hxx
        IfaceAnnoTools.hxx
        IfaceCCGS.hxx
        IfaceCeLEDS.hxx
        IfaceCeLEDSExporter.hxx
        IfaceCellML_APISPEC.hxx
        IfaceCellML_events.hxx
        IfaceCeVAS.hxx
        IfaceCIS.hxx
        IfaceCUSES.hxx
        IfaceDOM_APISPEC.hxx
        IfaceDOM_events.hxx
        IfaceMaLaES.hxx
        IfaceMathML_content_APISPEC.hxx
        IfaceRDF_APISPEC.hxx
        IfaceSProS.hxx
        IfaceSRuS.hxx
        IfaceTeLICeMS.hxx
        IfaceVACSS.hxx
        Ifacexpath.hxx
        Ifacexpcom.hxx
        MaLaESBootstrap.hpp
        SProSBootstrap.hpp
        SRuSBootstrap.hpp
        TeLICeMService.hpp
        VACSSBootstrap.hpp
    HEADERS_MOC
        src/cellmlplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES_DIR
        ${CELLML_LIBS_DIR}
    EXTERNAL_DEPENDENCIES
        ${CELLML_LIBS}
)

# Package the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # The CellML binaries

    FOREACH(CELLML_LIB ${CELLML_LIBS})
        # Copy them to the build and build/bin folders, so we can test the
        # CellML plugin without first having to deploy OpenCOR

        COPY_FILE_TO_BUILD_DIR(. ${CELLML_LIBS_DIR}/${CELLML_LIB})
        COPY_FILE_TO_BUILD_DIR(bin ${CELLML_LIBS_DIR}/${CELLML_LIB})

        # INstall them

        INSTALL(FILES ${CELLML_LIBS_DIR}/${CELLML_LIB} DESTINATION bin)
    ENDFOREACH()

    # The libstdc++-6.dll file
    # Note: this DLL comes from the official version of MinGW (as opposed to
    #       the Qt patched version) and is the version that is required since
    #       the CellML API was built using that version of MinGW (thank god,
    #       that DLL is not normally required when deploying a Qt application,
    #       so we can safely deploy it to get the CellML plugin to work)

    SET(LIBSTDC++-6_FILE ${CMAKE_SOURCE_DIR}/distrib/windows/x86/libstdc++-6.dll)

    COPY_FILE_TO_BUILD_DIR(. ${LIBSTDC++-6_FILE})
    COPY_FILE_TO_BUILD_DIR(bin ${LIBSTDC++-6_FILE})

    INSTALL(FILES ${LIBSTDC++-6_FILE} DESTINATION bin)
ELSEIF(APPLE)
    # Each CellML binary together with their respective CellML dependencies

    DEPLOY_MAC_OS_X_LIBRARY(libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
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
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libceledsexporter${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
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
            ${CELLML_LIBS_DIR}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
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
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libspros${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libsrus${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
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
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libvacss${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    # The CellML binaries

    FOREACH(CELLML_LIB ${CELLML_LIBS})
        INSTALL(FILES ${CELLML_LIBS_DIR}/${CELLML_LIB} DESTINATION lib)
    ENDFOREACH()
ENDIF()
