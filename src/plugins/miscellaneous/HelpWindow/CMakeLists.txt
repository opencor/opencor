PROJECT(HelpWindowPlugin)

# Generate the help files which will be embedded in the executable as a resource

SET(QHCP_FILENAME ${PROJECT_BUILD_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp.in ${QHCP_FILENAME})

EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/qcollectiongenerator ${QHCP_FILENAME}
                                                           -o ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qhc
                ${OUTPUT_QUIET})

KEEP_TRACK_OF_FILE(${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qch)
KEEP_TRACK_OF_FILE(${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qhc)

# Add the plugin

ADD_PLUGIN(HelpWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp

        src/helpwindowplugin.cpp
        src/helpwindowwidget.cpp
        src/helpwindowwindow.cpp
    HEADERS_MOC
        src/helpwindowplugin.h
        src/helpwindowwidget.h
        src/helpwindowwindow.h
    UIS
        src/helpwindowwindow.ui
    INCLUDE_DIRS
        src
    PLUGINS
        Core
    QT_MODULES
        Help
        PrintSupport
)
