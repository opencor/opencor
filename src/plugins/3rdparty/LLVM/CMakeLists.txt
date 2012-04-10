PROJECT(LLVMPlugin)

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
