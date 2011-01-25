PROJECT(LibQxt)

# Files that make up LibQxt

SET(SOURCES
    src/core/qxtcommandoptions.cpp
    src/core/qxtglobal.cpp
)

SET(HEADERS
    src/core/qxtcommandoptions.h
    src/core/qxtglobal.h
)

# Various include directories

SET(LOCAL_3RDPARTYLIB_SUBDIRS src/core)
SET(3RDPARTYLIB_SUBDIRS ${LOCAL_3RDPARTYLIB_SUBDIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_3RDPARTYLIB_SUBDIRS})

# Qt specific

FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

INCLUDE(${QT_USE_FILE})

# Rules to build LibQxt

ADD_DEFINITIONS(-DQXT_STATIC)
# Note: the above is just to 'fool' LibQxt in believing we want a static
#       version of the library. In fact, we want a shared version, but if we
#       were not to define QXT_STATIC, then LibQxt would define its classes to
#       be imported/exported (on Windows) while we prefer to take care of that
#       through the enable-auto-import option during linking (since this means
#       that we don't have to worry about imports/exports of DLL functions)

IF(APPLE)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
ELSE()
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
ENDIF()
# Note: the above is only required so that we can quickly test OpenCOR without
#       first having to package everything

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${SOURCES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
)

# Rules to package LibQxt

IF(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
ELSEIF(NOT APPLE)
    INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
ENDIF()
