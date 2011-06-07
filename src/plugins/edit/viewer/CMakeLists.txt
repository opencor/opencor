PROJECT(Viewer)

# Files that make up the Viewer plugin

SET(SOURCES
    src/mmlviewerwidget.cpp
    src/viewerwindow.cpp
)

SET(HEADERS_MOC
    src/mmlviewerwidget.h
    src/viewerwindow.h
)

SET(UIS
    src/viewerwindow.ui
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Rules to build the Viewer plugin

QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
QT4_WRAP_UI(SOURCES_UIS ${UIS})

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${SOURCES_MOC}
    ${SOURCES_UIS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}

    Core
    QtMmlWidget
)

# Linker settings

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")

# Rules to package the Viewer plugin

PACKAGE_PLUGIN()
