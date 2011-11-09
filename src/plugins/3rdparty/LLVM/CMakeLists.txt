PROJECT(LLVMPlugin)

# Retrieve some header files that were generated on each of our target
# platforms

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

# LLVM library

SET(LLVM_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/${DISTRIB_DIR})

IF(WIN32)
    SET(CMAKE_SHARED_LIBRARY_PREFIX)
ENDIF()

SET(LLVM_LIB
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
        include
        src
    QT_DEPENDENCIES
        QtCore
    EXTERNAL_DEPENDENCIES_DIR
        ${LLVM_LIB_DIR}
    EXTERNAL_DEPENDENCIES
        ${LLVM_LIB}
)

# Package the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # Copy the LLVM library to the build and build/bin folders, so we can
    # test the LLVM plugin without first having to deploy OpenCOR

    COPY_FILE_TO_BUILD_DIR(. ${LLVM_LIB_DIR}/${LLVM_LIB})
    COPY_FILE_TO_BUILD_DIR(bin ${LLVM_LIB_DIR}/${LLVM_LIB})

    # Install the LLVM library

    INSTALL(FILES ${LLVM_LIB_DIR}/${LLVM_LIB} DESTINATION bin)
ELSEIF(APPLE)
    # Deploy the LLVM libraries together with their respective LLVM
    # dependencies, if any

    DEPLOY_MAC_OS_X_LIBRARY(libLLVM-2.9${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${LLVM_LIB_DIR}
    )
ELSE()
    # LLVM library

    INSTALL(FILES ${LLVM_LIB_DIR}/${LLVM_LIB} DESTINATION lib)
ENDIF()
