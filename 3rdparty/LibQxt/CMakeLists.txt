PROJECT(LibQxt)

# Files that make up LibQxt

SET(SOURCES
    src/core/qxtcommandoptions.cpp
    src/core/qxtglobal.cpp
    src/core/qxttemporarydir.cpp
)

SET(HEADERS
    src/core/qxtcommandoptions.h
    src/core/qxtglobal.h
    src/core/qxttemporarydir.h
)

IF(WIN32)
    SET(SOURCES ${SOURCES}
        src/core/qxttemporarydir_win.cpp
    )
ELSE()
    SET(SOURCES ${SOURCES}
        src/core/qxttemporarydir_unix.cpp
    )
ENDIF()

# Various include directories

SET(LOCAL_THIRD_PARTY_LIBRARY_SUBDIRS src/core include/QxtCore)
SET(THIRD_PARTY_LIBRARY_SUBDIRS ${LOCAL_THIRD_PARTY_LIBRARY_SUBDIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_THIRD_PARTY_LIBRARY_SUBDIRS})

# Qt specific

FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

INCLUDE(${QT_USE_FILE})

# Rules to build LibQxt

ADD_DEFINITIONS(-DQXT_STATIC)
# Note: the above is just to 'fool' LibQxt in believing that we want a static
#       version of the library. In fact, we want a shared version, but if we
#       were not to define QXT_STATIC, then LibQxt would define its classes to
#       be imported/exported (on Windows) while we prefer to take care of that
#       through the enable-auto-import option during linking (since this means
#       that we don't have to worry about imports/exports of DLL functions),
#       so...

IF(WIN32)
    ADD_DEFINITIONS(-DUNICODE)
    # Note: the above is required to get QxtTemporaryDir (well,
    #       QxtTemporaryDirPrivate) to compile fine on Windows
ENDIF()

IF(APPLE)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
ELSE()
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
ENDIF()
# Note: the above is only required so that we can quickly test LibQxt without
#       first having to package everything

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${SOURCES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
)

# Compiler and linker options

IF(${DEBUG_MODE})
    SET(CMAKE_CXX_FLAGS "-g -O0")
ELSE()
    SET(CMAKE_CXX_FLAGS "-O2 -ffast-math")
ENDIF()

IF(NOT APPLE AND NOT ${DEBUG_MODE})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-s")
ENDIF()

# Rules to package LibQxt

IF(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
ELSEIF(NOT APPLE)
    INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
ENDIF()
