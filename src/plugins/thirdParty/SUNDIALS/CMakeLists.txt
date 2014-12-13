PROJECT(SUNDIALSPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_SUNDIALS_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(${ARCHITECTURE} EQUAL 32)
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 0cab863a5652b6862a1d8629599f08ddc86e2d09)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib 16c88df3c7e9c9ec6962acfc8ac104190925f011)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 9f02933547dfb915302e7a6ce235638568b30687)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib 13964f18f7bf0ec6f6f34a69652272343f9eda05)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll e6bf5c94ac43c576c56dbc479bcbce37495d843d)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib 4da6db52a91fe388868971bd079e45ed43ba3ca9)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll c06af4a0405440b98c6dcf78a7fae3c41c96abe8)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib 8733f34ac3f4555bfd0736ad36b86674fdf2fd67)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.dylib 12aa8840aec437ddededdab6cb74dae350244acd)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so 76dec5435298332354a5745bcc78fea9f530f82f)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so 13889b62ad143fbfdc469ebb03a1880ca974d85b)
        ENDIF()
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, must refer to the distribution directory as
    #       opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(SUNDIALS_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}SUNDIALS${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(SUNDIALS_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}SUNDIALS${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
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
