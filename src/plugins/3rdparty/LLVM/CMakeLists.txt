PROJECT(LLVMPlugin)

MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve LLVM settings

    IF(MSVC)
        SET(LLVM_MSVC_DEFINITIONS
            _SCL_SECURE_NO_WARNINGS
            # Note: the above definition prevents MSVC from generating C4996
            #       warnings...
        )
    ENDIF()

    SET(LLVM_DEFINITIONS
        __STDC_LIMIT_MACROS __STDC_CONSTANT_MACROS
        ${LLVM_MSVC_DEFINITIONS}
    )
ENDMACRO()

# Add the plugin

ADD_PLUGIN(LLVM
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/llvmplugin.cpp
    HEADERS_MOC
        src/llvmplugin.h
    INCLUDE_DIRS
        include
        src
    QT_DEPENDENCIES
        QtCore
)
