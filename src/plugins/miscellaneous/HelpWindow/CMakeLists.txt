project(HelpWindowPlugin)

# Generate the help files that will be embedded in OpenCOR as a resource
# Note: the documentation can only be built once we have handled our different
#       plugins (the Python and PythonPackages plugins to be more precise). Yet,
#       this plugin relies on the user documentation having been generated. So,
#       we have no choice but to hardcode the fact that our GenerateQtHelpFiles
#       target depends on userDocumentationBuild...

add_custom_target(GenerateQtHelpFiles)

add_dependencies(GenerateQtHelpFiles userDocumentationBuild)

add_custom_command(TARGET GenerateQtHelpFiles
                   COMMAND ${SILENTRUN}
                           ${QT_BINARY_DIR}/qhelpgenerator ${PROJECT_BUILD_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp
                                                        -o ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.qhc)

track_files(
    ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.qch
    ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.qhc
)

# Add the plugin

add_plugin(HelpWindow
    SOURCES
        ../../i18ninterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp
        ../../windowinterface.cpp

        src/helpwindowplugin.cpp
        src/helpwindowwidget.cpp
        src/helpwindowwindow.cpp
    UIS
        src/helpwindowwindow.ui
    PLUGINS
        Core
        WebViewerWidget
    QT_MODULES
        Help
        PrintSupport
    DEPENDS_ON
        GenerateQtHelpFiles
)
