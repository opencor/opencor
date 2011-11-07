PROJECT(LLVMPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Retrieve some header files that were generated on each of our target
# platforms, but first determine the location of the files based on the
# architecture we are targetting

IF(WIN32)
    SET(DISTRIB_DIR windows/x86)
ELSEIF(APPLE)
    SET(DISTRIB_DIR macosx)
ELSE()
    IF(64BIT_MODE)
        SET(DISTRIB_DIR linux/x64)
    ELSE()
        SET(DISTRIB_DIR linux/x86)
    ENDIF()
ENDIF()

# Now, we can copy the Config header files

SET(LLVM_CONFIG_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include/llvm/Config)
SET(CONFIG_HEADER_FILES
    config.h
    llvm-config.h
)

FOREACH(CONFIG_HEADER_FILE ${CONFIG_HEADER_FILES})
    CONFIGURE_FILE(${LLVM_CONFIG_HEADERS_DIR}/${DISTRIB_DIR}/${CONFIG_HEADER_FILE}
                   ${LLVM_CONFIG_HEADERS_DIR}/${CONFIG_HEADER_FILE}
                   COPYONLY)
ENDFOREACH()

# Determine the location of the LLVM libraries

IF(WIN32)
    SET(LLVM_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/windows/x86)
ELSEIF(APPLE)
    SET(LLVM_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/macosx)
ELSE()
    IF(64BIT_MODE)
        SET(LLVM_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/linux/x64)
    ELSE()
        SET(LLVM_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/linux/x86)
    ENDIF()
ENDIF()

# LLVM libraries themselves

IF(WIN32)
    SET(CMAKE_SHARED_LIBRARY_PREFIX)
ENDIF()

SET(LLVM_LIBS
    ${CMAKE_SHARED_LIBRARY_PREFIX}LLVM-2.9${CMAKE_SHARED_LIBRARY_SUFFIX}
)

# Add the plugin

ADD_PLUGIN(LLVM
    SOURCES
        ../../plugininfo.cpp

        src/llvmplugin.cpp
    HEADERS
        ../../plugininfo.h
    HEADERS_MOC
        src/llvmplugin.h
    INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
    EXTERNAL_DEPENDENCIES_DIR
        ${LLVM_LIBS_DIR}
    EXTERNAL_DEPENDENCIES
        ${LLVM_LIBS}
)

# Package the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # The LLVM libraries

    FOREACH(LLVM_LIB ${LLVM_LIBS})
        # Copy the LLVM library to the build and build/bin folders, so we can
        # test the LLVM plugin without first having to deploy OpenCOR

        COPY_FILE_TO_BUILD_DIR(. ${LLVM_LIBS_DIR}/${LLVM_LIB})
        COPY_FILE_TO_BUILD_DIR(bin ${LLVM_LIBS_DIR}/${LLVM_LIB})

        # Install the LLVM library

        INSTALL(FILES ${LLVM_LIBS_DIR}/${LLVM_LIB} DESTINATION bin)
    ENDFOREACH()

    # The libstdc++-6.dll file
    # Note: this DLL comes from the official version of MinGW (as opposed to
    #       the Qt patched version) and is the version that is required since
    #       LLVM was built using that version of MinGW (thank god, that DLL is
    #       not required when deploying a Qt application, so we can safely
    #       deploy it to get the LLVM plugin to work)
#---GRY--- NEED TO CHECK WHETHER libstdc++-6.dll IS REQUIRED FOR LLVM...

#    SET(LIBSTDC++-6_FILE ${CMAKE_SOURCE_DIR}/distrib/windows/x86/libstdc++-6.dll)

#    COPY_FILE_TO_BUILD_DIR(. ${LIBSTDC++-6_FILE})
#    COPY_FILE_TO_BUILD_DIR(bin ${LIBSTDC++-6_FILE})

#    INSTALL(FILES ${LIBSTDC++-6_FILE} DESTINATION bin)
ELSEIF(APPLE)
    # Deploy the LLVM libraries together with their respective LLVM
    # dependencies, if any

    DEPLOY_MAC_OS_X_LIBRARY(libLLVM-2.9${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${LLVM_LIBS_DIR}
    )
ELSE()
    # The LLVM libraries

    FOREACH(LLVM_LIB ${LLVM_LIBS})
        INSTALL(FILES ${LLVM_LIBS_DIR}/${LLVM_LIB} DESTINATION lib)
    ENDFOREACH()
ENDIF()
