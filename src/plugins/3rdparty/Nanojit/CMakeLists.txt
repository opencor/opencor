PROJECT(NanojitPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Determine the location of the CellML libraries

IF(64BIT_MODE)
    SET(NATIVE_FILEBASE src/nanojit/NativeX64)
ELSE()
    SET(NATIVE_FILEBASE src/nanojit/Nativei386)
ENDIF()

# Add the plugin

ADD_PLUGIN(Nanojit
    SOURCES
        ../../plugininfo.cpp

        src/nanojitplugin.cpp

        src/nanojit/Allocator.cpp
        src/nanojit/Assembler.cpp
        src/nanojit/CodeAlloc.cpp
        src/nanojit/Containers.cpp
        src/nanojit/Fragmento.cpp
        src/nanojit/LIR.cpp
        ${NATIVE_FILEBASE}.cpp
        src/nanojit/njconfig.cpp
        src/nanojit/RegAlloc.cpp
    HEADERS
        ../../plugininfo.h

        src/nanojit/Allocator.h
        src/nanojit/Assembler.h
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
    HEADERS_MOC
        src/nanojitplugin.h
    PLUGIN_INCLUDE_DIRS
        src/nanojit
    QT_DEPENDENCIES
        QtCore
)
