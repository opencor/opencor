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

SET(CELLML_HEADER_DIR src/${CELLML_DISTRIB_DIR})
SET(CELLML_IMPORT_DIR ${PROJECT_SOURCE_DIR}/lib/${CELLML_DISTRIB_DIR})
SET(CELLML_RES_DIR res/${CELLML_DISTRIB_DIR})

# Add the plugin

ADD_PLUGIN(CellML
    SOURCES
        ../../plugininterface.cpp

        src/cellmlplugin.cpp
    HEADERS
        ../../plugininterface.h
        ${CELLML_HEADER_DIR}/AnnoToolsBootstrap.hpp
        ${CELLML_HEADER_DIR}/CCGSBootstrap.hpp
        ${CELLML_HEADER_DIR}/cda_compiler_support.h
        ${CELLML_HEADER_DIR}/cda_config.h
        ${CELLML_HEADER_DIR}/CeLEDSBootstrap.hpp
        ${CELLML_HEADER_DIR}/CeLEDSExporterBootstrap.hpp
        ${CELLML_HEADER_DIR}/cellml-api-cxx-support.hpp
        ${CELLML_HEADER_DIR}/CellMLBootstrap.hpp
        ${CELLML_HEADER_DIR}/CeVASBootstrap.hpp
        ${CELLML_HEADER_DIR}/CISBootstrap.hpp
        ${CELLML_HEADER_DIR}/CUSESBootstrap.hpp
        ${CELLML_HEADER_DIR}/DOMBootstrap.hxx
        ${CELLML_HEADER_DIR}/IfaceAnnoTools.hxx
        ${CELLML_HEADER_DIR}/IfaceCCGS.hxx
        ${CELLML_HEADER_DIR}/IfaceCeLEDS.hxx
        ${CELLML_HEADER_DIR}/IfaceCeLEDSExporter.hxx
        ${CELLML_HEADER_DIR}/IfaceCellML_APISPEC.hxx
        ${CELLML_HEADER_DIR}/IfaceCellML_events.hxx
        ${CELLML_HEADER_DIR}/IfaceCeVAS.hxx
        ${CELLML_HEADER_DIR}/IfaceCIS.hxx
        ${CELLML_HEADER_DIR}/IfaceCUSES.hxx
        ${CELLML_HEADER_DIR}/IfaceDOM_APISPEC.hxx
        ${CELLML_HEADER_DIR}/IfaceDOM_events.hxx
        ${CELLML_HEADER_DIR}/IfaceMaLaES.hxx
        ${CELLML_HEADER_DIR}/IfaceMathML_content_APISPEC.hxx
        ${CELLML_HEADER_DIR}/IfaceRDF_APISPEC.hxx
        ${CELLML_HEADER_DIR}/IfaceSProS.hxx
        ${CELLML_HEADER_DIR}/IfaceSRuS.hxx
        ${CELLML_HEADER_DIR}/IfaceTeLICeMS.hxx
        ${CELLML_HEADER_DIR}/IfaceVACSS.hxx
        ${CELLML_HEADER_DIR}/Ifacexpath.hxx
        ${CELLML_HEADER_DIR}/Ifacexpcom.hxx
        ${CELLML_HEADER_DIR}/MaLaESBootstrap.hpp
        ${CELLML_HEADER_DIR}/SProSBootstrap.hpp
        ${CELLML_HEADER_DIR}/SRuSBootstrap.hpp
        ${CELLML_HEADER_DIR}/TeLICeMService.hpp
        ${CELLML_HEADER_DIR}/VACSSBootstrap.hpp
    HEADERS_MOC
        src/cellmlplugin.h
    PLUGIN_INCLUDE_DIRS
        src
        ${CELLML_HEADER_DIR}
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES
        ${CELLML_IMPORT_DIR}/libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libceleds${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libceledsexporter${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libspros${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libsrus${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libtelicems${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libvacss${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CELLML_IMPORT_DIR}/libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
    RESOURCE_DIR
        ${CELLML_RES_DIR}
)
