PROJECT(CellMLPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Determine the location of the CellML files

IF(WIN32)
    SET(CELLML_DISTRIB_DIR windows/x86)
ELSEIF(APPLE)
    SET(CELLML_DISTRIB_DIR macosx)
ELSE()
    IF(64BIT_MODE)
        SET(CELLML_DISTRIB_DIR linux/x64)
    ELSE()
        SET(CELLML_DISTRIB_DIR linux/x86)
    ENDIF()
ENDIF()

# Determine the location of a few CellML specific directories

SET(CELLML_HEADERS_DIR src/${CELLML_DISTRIB_DIR})
SET(CELLML_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/${CELLML_DISTRIB_DIR})

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
        ../../apiinterface.cpp
        ../../plugininterface.cpp

        src/cellmlplugin.cpp
    HEADERS
        ../../apiinterface.h
        ../../plugininterface.h

        ${CELLML_HEADERS_DIR}/AnnoToolsBootstrap.hpp
        ${CELLML_HEADERS_DIR}/CCGSBootstrap.hpp
        ${CELLML_HEADERS_DIR}/cda_compiler_support.h
        ${CELLML_HEADERS_DIR}/cda_config.h
        ${CELLML_HEADERS_DIR}/CeLEDSBootstrap.hpp
        ${CELLML_HEADERS_DIR}/CeLEDSExporterBootstrap.hpp
        ${CELLML_HEADERS_DIR}/cellml-api-cxx-support.hpp
        ${CELLML_HEADERS_DIR}/CellMLBootstrap.hpp
        ${CELLML_HEADERS_DIR}/CeVASBootstrap.hpp
        ${CELLML_HEADERS_DIR}/CISBootstrap.hpp
        ${CELLML_HEADERS_DIR}/CUSESBootstrap.hpp
        ${CELLML_HEADERS_DIR}/DOMBootstrap.hxx
        ${CELLML_HEADERS_DIR}/IfaceAnnoTools.hxx
        ${CELLML_HEADERS_DIR}/IfaceCCGS.hxx
        ${CELLML_HEADERS_DIR}/IfaceCeLEDS.hxx
        ${CELLML_HEADERS_DIR}/IfaceCeLEDSExporter.hxx
        ${CELLML_HEADERS_DIR}/IfaceCellML_APISPEC.hxx
        ${CELLML_HEADERS_DIR}/IfaceCellML_events.hxx
        ${CELLML_HEADERS_DIR}/IfaceCeVAS.hxx
        ${CELLML_HEADERS_DIR}/IfaceCIS.hxx
        ${CELLML_HEADERS_DIR}/IfaceCUSES.hxx
        ${CELLML_HEADERS_DIR}/IfaceDOM_APISPEC.hxx
        ${CELLML_HEADERS_DIR}/IfaceDOM_events.hxx
        ${CELLML_HEADERS_DIR}/IfaceMaLaES.hxx
        ${CELLML_HEADERS_DIR}/IfaceMathML_content_APISPEC.hxx
        ${CELLML_HEADERS_DIR}/IfaceRDF_APISPEC.hxx
        ${CELLML_HEADERS_DIR}/IfaceSProS.hxx
        ${CELLML_HEADERS_DIR}/IfaceSRuS.hxx
        ${CELLML_HEADERS_DIR}/IfaceTeLICeMS.hxx
        ${CELLML_HEADERS_DIR}/IfaceVACSS.hxx
        ${CELLML_HEADERS_DIR}/Ifacexpath.hxx
        ${CELLML_HEADERS_DIR}/Ifacexpcom.hxx
        ${CELLML_HEADERS_DIR}/MaLaESBootstrap.hpp
        ${CELLML_HEADERS_DIR}/SProSBootstrap.hpp
        ${CELLML_HEADERS_DIR}/SRuSBootstrap.hpp
        ${CELLML_HEADERS_DIR}/TeLICeMService.hpp
        ${CELLML_HEADERS_DIR}/VACSSBootstrap.hpp
    HEADERS_MOC
        src/cellmlplugin.h
    PLUGIN_INCLUDE_DIRS
        src
        ${CELLML_HEADERS_DIR}
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES_DIR
        ${CELLML_LIBS_DIR}
    EXTERNAL_DEPENDENCIES
        ${CELLML_LIBS}
    RESOURCE_DIR
        res/${CELLML_DISTRIB_DIR}
)

# Package the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # The CellML binaries

    SET(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

    FOREACH(CELLML_LIB ${CELLML_LIBS})
        # Copy them to the build/bin folder, so we can test the CellML plugin
        # without first having to deploy OpenCOR
    
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${CELLML_LIBS_DIR}/${CELLML_LIB} ${BIN_DIR})

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
    
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${LIBSTDC++-6_FILE} ${BIN_DIR})

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
