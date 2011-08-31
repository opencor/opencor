PROJECT(CellMLPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Determine the location of the CellML binaries

IF(WIN32)
    SET(CELLML_DISTRIBUTION_DIR windows/x86)
ELSEIF(APPLE)
    SET(CELLML_DISTRIBUTION_DIR macosx)
ELSE()
    IF(64BIT_MODE)
        SET(CELLML_DISTRIBUTION_DIR linux/x64)
    ELSE()
        SET(CELLML_DISTRIBUTION_DIR linux/x86)
    ENDIF()
ENDIF()

# CellML header files

SET(CELLML_HEADERS)
SET(CELLML_HEADER_FILES
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
)

FOREACH(CELLML_HEADER_FILE ${CELLML_HEADER_FILES})
    SET(CELLML_HEADERS ${CELLML_HEADERS} src/${CELLML_DISTRIBUTION_DIR}/${CELLML_HEADER_FILE})
ENDFOREACH()

# Add the plugin

ADD_PLUGIN(CellML
    SOURCES
        ../../plugininterface.cpp

        src/cellmlplugin.cpp
    HEADERS
        ../../plugininterface.h
        ${CELLML_HEADERS}
    HEADERS_MOC
        src/cellmlplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
        QtGui
    RESOURCE_DIR
        res/${CELLML_DISTRIBUTION_DIR}
)
