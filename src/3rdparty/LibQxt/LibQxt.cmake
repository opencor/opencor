# Files that make up LibQxt

SET(SOURCES ${SOURCES}
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/LibQxt/src/core/qxtcommandoptions.cpp
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/LibQxt/src/core/qxtglobal.cpp
)

SET(HEADERS ${HEADERS}
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/LibQxt/src/core/qxtcommandoptions.h
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/LibQxt/src/core/qxtglobal.h
)

# Various include directories

INCLUDE_DIRECTORIES(${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/LibQxt/src/core)
INCLUDE_DIRECTORIES(${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/LibQxt/include/QxtCore)

# Compiler settings specific to LibQxt

ADD_DEFINITIONS(-DQXT_STATIC)

IF(WIN32)
    ADD_DEFINITIONS(-DUNICODE)
    # Note: the above is required to get QxtTemporaryDir (well,
    #       QxtTemporaryDirPrivate) to compile fine on Windows
ENDIF()
