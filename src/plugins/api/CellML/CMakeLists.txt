PROJECT(CellMLPlugin)

# CellML libraries

SET(CELLML_LIBS_DIR ${PROJECT_SOURCE_DIR}/lib/${DISTRIB_DIR})

SET(CELLML_LIBS
    libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
    libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
    libceleds${CMAKE_SHARED_LIBRARY_SUFFIX}
    libceledsexporter${CMAKE_SHARED_LIBRARY_SUFFIX}
    libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
    libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
    libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
    libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    libspros${CMAKE_SHARED_LIBRARY_SUFFIX}
    libsrus${CMAKE_SHARED_LIBRARY_SUFFIX}
    libtelicems${CMAKE_SHARED_LIBRARY_SUFFIX}
    libvacss${CMAKE_SHARED_LIBRARY_SUFFIX}
    libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
)

# Add the plugin

ADD_PLUGIN(CellML
    SOURCES
        ../../coreinterface.cpp
        ../../fileinterface.cpp
        ../../plugininfo.cpp

        src/cellmlplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../fileinterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/cellmlplugin.h
    INCLUDE_DIRS
        src
    OPENCOR_DEPENDENCIES
        Core
    QT_DEPENDENCIES
        QtCore
        QtGui
    EXTERNAL_DEPENDENCIES_DIR
        ${CELLML_LIBS_DIR}
    EXTERNAL_DEPENDENCIES
        ${CELLML_LIBS}
)

# Package the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # The CellML libraries

    FOREACH(CELLML_LIB ${CELLML_LIBS})
        # Copy the CellML library to the build and build/bin folders, so we can
        # test the CellML plugin without first having to deploy OpenCOR

        COPY_FILE_TO_BUILD_DIR(. ${CELLML_LIBS_DIR}/${CELLML_LIB})
        COPY_FILE_TO_BUILD_DIR(bin ${CELLML_LIBS_DIR}/${CELLML_LIB})

        # Install the CellML library

        INSTALL(FILES ${CELLML_LIBS_DIR}/${CELLML_LIB} DESTINATION bin)
    ENDFOREACH()

    # The libstdc++-6.dll file
    # Note: this DLL comes from the official version of MinGW (as opposed to
    #       the Qt patched version) and is the version that is required since
    #       the CellML API was built using that version of MinGW (thank god,
    #       that DLL is not required when deploying a Qt application, so we can
    #       safely deploy it to get the CellML plugin to work)

    SET(LIBSTDC++-6_FILE ${CMAKE_SOURCE_DIR}/distrib/windows/x86/libstdc++-6.dll)

    COPY_FILE_TO_BUILD_DIR(. ${LIBSTDC++-6_FILE})
    COPY_FILE_TO_BUILD_DIR(bin ${LIBSTDC++-6_FILE})

    INSTALL(FILES ${LIBSTDC++-6_FILE} DESTINATION bin)
ELSEIF(APPLE)
    # Deploy the CellML libraries together with their respective CellML
    # dependencies, if any

    DEPLOY_MAC_OS_X_LIBRARY(libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libceleds${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libceledsexporter${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
            libceleds${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libspros${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libsrus${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libccgs${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcevas${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcis${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
            libmalaes${CMAKE_SHARED_LIBRARY_SUFFIX}
            libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libtelicems${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libvacss${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libannotools${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
            libcuses${CMAKE_SHARED_LIBRARY_SUFFIX}
    )

    DEPLOY_MAC_OS_X_LIBRARY(libxpath${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${CELLML_LIBS_DIR}
        LIBRARIES
            libcellml${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    # The CellML binaries

    FOREACH(CELLML_LIB ${CELLML_LIBS})
        INSTALL(FILES ${CELLML_LIBS_DIR}/${CELLML_LIB} DESTINATION lib)
    ENDFOREACH()
ENDIF()
