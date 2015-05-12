PROJECT(libgit2Plugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_LIBGIT2_PLUGIN)
    # Retrieve the plugin's binary file(s)

    MESSAGE(FATAL_ERROR "Binaries do not yet exist for libgit2...")
ELSE()
    # Add the plugin

    SET(LIBGIT2_PLUGIN LIBGIT2 PARENT_SCOPE)

    ADD_PLUGIN(libgit2
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            src/libgit2plugin.cpp
        HEADERS_MOC
            src/libgit2plugin.h
        INCLUDE_DIRS
            src
        QT_MODULES
            Core
        QT_LIBRARIES
            QtCore
    )
ENDIF()
