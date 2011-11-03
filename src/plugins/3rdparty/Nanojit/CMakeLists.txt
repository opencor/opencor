PROJECT(NanojitPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Prevent all warnings
# Note: indeed, Nanojit generates many warnings, so...

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# Determine the location of the Nanojit libraries

IF(64BIT_MODE)
    SET(NANOJIT_MODE AVMPLUS_AMD64)

    SET(NATIVE_FILEBASE src/nanojit/NativeX64)
ELSE()
    SET(NANOJIT_MODE AVMPLUS_IA32)

    SET(NATIVE_FILEBASE src/nanojit/Nativei386)
ENDIF()

# Add the plugin

ADD_PLUGIN(Nanojit
    SOURCES
        ../../plugininfo.cpp

        src/nanojitplugin.cpp

        src/lirasm/lirasm.cpp 

        src/nanojit/Allocator.cpp
        src/nanojit/Assembler.cpp
        src/nanojit/avmplus.cpp
        src/nanojit/CodeAlloc.cpp
        src/nanojit/Containers.cpp
        src/nanojit/Fragmento.cpp
        src/nanojit/LIR.cpp
        ${NATIVE_FILEBASE}.cpp
        src/nanojit/njconfig.cpp
        src/nanojit/RegAlloc.cpp
        src/nanojit/VMPI.cpp

        src/vprof/vprof.cpp
    HEADERS
        ../../plugininfo.h

        src/nanojit/Allocator.h
        src/nanojit/Assembler.h
        src/nanojit/avmplus.h
        src/nanojit/CodeAlloc.h
        src/nanojit/Containers.h
        src/nanojit/Fragmento.h
        src/nanojit/LIR.h
        src/nanojit/nanojit.h
        src/nanojit/Native.h
        src/nanojit/NativeCommon.h
        ${NATIVE_FILEBASE}.h
        src/nanojit/njconfig.h
        src/nanojit/RegAlloc.h
        src/nanojit/VMPI.h

        src/vprof/vprof.h
    HEADERS_MOC
        src/nanojitplugin.h
    DEFINITIONS
        ${NANOJIT_MODE}
        FEATURE_NANOJIT
    PLUGIN_INCLUDE_DIRS
        src
        src/lirasm
        src/nanojit
        src/vprof
    QT_DEPENDENCIES
        QtCore
)
