PROJECT(SUNDIALSPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_SUNDIALS_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 6652b8ad5709023d86c278e42419acadfff30ebc)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib 097f7d84902350b3aef203820804c88a5c7fb374)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 5593cfd0ca97172901adc76a6b0dd099f4a5052d)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib aaece47f62ac82de28ca7ca03dc4879fb58caea4)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.dylib 197a09c5206afc0e8b77c0540bf1546817664eb4)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so ce1822e9f5e700d4f5001a8a72ffd558f86d465f)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so e03ae4583a0eeaf5465a17054b1a9cd2e5edfb95)
        ENDIF()
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, must refer to the distribution directory as
    #       opposed to the main build directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(SUNDIALS_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}SUNDIALS${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(SUNDIALS_PLUGIN_BINARY ${PROJECT_BUILD_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}SUNDIALS${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(SUNDIALS
        INCLUDE_DIRS
            include
            src
        QT_LIBRARIES
            QtCore
    )
ELSE()
    # Add the plugin

    SET(SUNDIALS_PLUGIN SUNDIALS PARENT_SCOPE)

    ADD_PLUGIN(SUNDIALS
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            src/sundialsplugin.cpp

            src/cvode/cvode.c
            src/cvode/cvode_band.c
            src/cvode/cvode_bandpre.c
            src/cvode/cvode_bbdpre.c
            src/cvode/cvode_dense.c
            src/cvode/cvode_diag.c
            src/cvode/cvode_direct.c
            src/cvode/cvode_io.c
            src/cvode/cvode_spbcgs.c
            src/cvode/cvode_spgmr.c
            src/cvode/cvode_spils.c
            src/cvode/cvode_sptfqmr.c

            src/ida/ida.c
            src/ida/ida_band.c
            src/ida/ida_bbdpre.c
            src/ida/ida_dense.c
            src/ida/ida_direct.c
            src/ida/ida_ic.c
            src/ida/ida_io.c
            src/ida/ida_spbcgs.c
            src/ida/ida_spgmr.c
            src/ida/ida_spils.c
            src/ida/ida_sptfqmr.c

            src/kinsol/kinsol.c
            src/kinsol/kinsol_band.c
            src/kinsol/kinsol_bbdpre.c
            src/kinsol/kinsol_dense.c
            src/kinsol/kinsol_direct.c
            src/kinsol/kinsol_io.c
            src/kinsol/kinsol_spbcgs.c
            src/kinsol/kinsol_spgmr.c
            src/kinsol/kinsol_spils.c
            src/kinsol/kinsol_sptfqmr.c

            src/nvec_ser/nvector_serial.c

            src/sundials/sundials_band.c
            src/sundials/sundials_dense.c
            src/sundials/sundials_direct.c
            src/sundials/sundials_iterative.c
            src/sundials/sundials_math.c
            src/sundials/sundials_nvector.c
            src/sundials/sundials_spbcgs.c
            src/sundials/sundials_spgmr.c
            src/sundials/sundials_sptfqmr.c
        HEADERS_MOC
            src/sundialsplugin.h
        INCLUDE_DIRS
            include
            src
        DEFINITIONS
            BUILD_SUNDIALS_LIBRARY
            NO_FPRINTF_OUTPUT
        QT_MODULES
            Core
        QT_LIBRARIES
            QtCore
    )
ENDIF()
