PROJECT(HelpWindowPlugin)

# Generate the help files which will be embedded in the executable as a resource

EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/qcollectiongenerator ${PROJECT_BUILD_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp
                                                           -o ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qhc
                OUTPUT_QUIET)

KEEP_TRACK_OF_FILE(${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qch)
KEEP_TRACK_OF_FILE(${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.qhc)

# Add the plugin

ADD_PLUGIN(HelpWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/helpwindowplugin.cpp
        src/helpwindowwidget.cpp
        src/helpwindowwindow.cpp
    HEADERS_MOC
        src/helpwindowplugin.h
        src/helpwindowwidget.h
        src/helpwindowwindow.h
    UIS
        src/helpwindowwindow.ui
    PLUGINS
        Core
        WebViewerWidget
    QT_MODULES
        Help
        PrintSupport
)
