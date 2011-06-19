# Files that make up LibQxt

SET(SOURCES ${SOURCES}
    ${MAIN_SOURCE_LOCATION}/src/3rdparty/LibQxt/src/core/qxtcommandoptions.cpp
    ${MAIN_SOURCE_LOCATION}/src/3rdparty/LibQxt/src/core/qxtglobal.cpp
)

SET(HEADERS ${HEADERS}
    ${MAIN_SOURCE_LOCATION}/src/3rdparty/LibQxt/src/core/qxtcommandoptions.h
    ${MAIN_SOURCE_LOCATION}/src/3rdparty/LibQxt/src/core/qxtglobal.h
)

# Various include directories

INCLUDE_DIRECTORIES(${MAIN_SOURCE_LOCATION}/src/3rdparty/LibQxt/src/core)
INCLUDE_DIRECTORIES(${MAIN_SOURCE_LOCATION}/src/3rdparty/LibQxt/include/QxtCore)

# Compiler settings specific to LibQxt

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
