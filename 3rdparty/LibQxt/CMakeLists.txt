PROJECT(LibQxt)

# Some in-house CMake functions/macros

INCLUDE(../../cmake/common.cmake)

# Initialise LibQxt

INITIALISE_THIRD_PARTY_LIBRARY()

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

SET(LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS src/core include/QxtCore)
SET(THIRD_PARTY_LIBRARY_INCLUDE_DIRS ${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS})

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

BUILD_THIRD_PARTY_LIBRARY(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
)

# Rules to package LibQxt

PACKAGE_THIRD_PARTY_LIBRARY()
