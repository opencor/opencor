# Files that make up LibQxt

SET(SOURCES ${SOURCES}
    src/3rdparty/LibQxt/src/core/qxtcommandoptions.cpp
    src/3rdparty/LibQxt/src/core/qxtglobal.cpp
)

SET(HEADERS ${HEADERS}
    src/3rdparty/LibQxt/src/core/qxtcommandoptions.h
    src/3rdparty/LibQxt/src/core/qxtglobal.h
)

# Various include directories

INCLUDE_DIRECTORIES(src/3rdparty/LibQxt/src/core)
INCLUDE_DIRECTORIES(src/3rdparty/LibQxt/include/QxtCore)

# Compiler settings specific to LibQxt

ADD_DEFINITIONS(-DQXT_STATIC)
