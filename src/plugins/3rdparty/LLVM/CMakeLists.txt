PROJECT(LLVMPlugin)

INCLUDE(common.cmake)

# Add the plugin as a binary
# Note: in the case of Windows, LLVM_BINARY_PLUGIN must refer to the
#       distribution directory as opposed to the main build directory since we
#       need the .lib file and it's only there, so...

IF(WIN32)
    SET(LLVM_BINARY_PLUGIN
        ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}LLVM${CMAKE_IMPORT_LIBRARY_SUFFIX}

        PARENT_SCOPE
    )
ELSE()
    SET(LLVM_BINARY_PLUGIN
        ${CMAKE_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}LLVM${CMAKE_SHARED_LIBRARY_SUFFIX}

        PARENT_SCOPE
    )
ENDIF()

ADD_PLUGIN_BINARY(LLVM
    INCLUDE_DIRS
        include
        src
        tools/clang/include
    QT_DEPENDENCIES
        QtCore
)
