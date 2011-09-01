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

SET(CELLML_HEADERS
    src/${CELLML_DISTRIBUTION_DIR}/AnnoToolsBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/CCGSBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/cda_compiler_support.h
    src/${CELLML_DISTRIBUTION_DIR}/cda_config.h
    src/${CELLML_DISTRIBUTION_DIR}/CeLEDSBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/CeLEDSExporterBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/cellml-api-cxx-support.hpp
    src/${CELLML_DISTRIBUTION_DIR}/CellMLBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/CeVASBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/CISBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/CUSESBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/DOMBootstrap.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceAnnoTools.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCCGS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCeLEDS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCeLEDSExporter.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCellML_APISPEC.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCellML_events.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCeVAS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCIS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceCUSES.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceDOM_APISPEC.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceDOM_events.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceMaLaES.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceMathML_content_APISPEC.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceRDF_APISPEC.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceSProS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceSRuS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceTeLICeMS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/IfaceVACSS.hxx
    src/${CELLML_DISTRIBUTION_DIR}/Ifacexpath.hxx
    src/${CELLML_DISTRIBUTION_DIR}/Ifacexpcom.hxx
    src/${CELLML_DISTRIBUTION_DIR}/MaLaESBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/SProSBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/SRuSBootstrap.hpp
    src/${CELLML_DISTRIBUTION_DIR}/TeLICeMService.hpp
    src/${CELLML_DISTRIBUTION_DIR}/VACSSBootstrap.hpp
)

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
        src/${CELLML_DISTRIBUTION_DIR}
    QT_DEPENDENCIES
        QtCore
        QtGui
    RESOURCE_DIR
        res/${CELLML_DISTRIBUTION_DIR}
)
