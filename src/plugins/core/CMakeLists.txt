PROJECT(Core)

# Files that make up the Core plugin

SET(SOURCES
    src/commonwidget.cpp
    src/dockwidget.cpp
)

SET(HEADERS
    src/commonwidget.h
)

SET(HEADERS_MOC
    src/dockwidget.h
)

# Various include directories

SET(LOCAL_PLUGIN_INCLUDE_DIRS src)
SET(PLUGIN_INCLUDE_DIRS ${LOCAL_PLUGIN_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_PLUGIN_INCLUDE_DIRS})

# Rules to build the Core plugin

QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${SOURCES_MOC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
)

# Linker settings

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")

# Rules to package the Core plugin

PACKAGE_PLUGIN()
