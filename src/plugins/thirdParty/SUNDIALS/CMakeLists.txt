PROJECT(SUNDIALSPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_SUNDIALS_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(${ARCHITECTURE} EQUAL 32)
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 8413cf57d5bc612aa53f81018ee3eced43c0a2d3)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib a09d5f9ae4c79343085e479b0b8ac08d4733d20c)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll a499f808fbd40846be875d001079a261530242f2)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib d3b9ce017e301e7342c0664b7384e860712735d6)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll b570f280273735a2dc968564061457c623c56273)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib 6f4e04a0f86a3cb2e75b0ce5fbd350babfeac617)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll a8bec2e91e8f06375a26a95f21d649fd9114dee4)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib b40923bd194bbd5f4fd4c4cb654021bbcfba845a)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.dylib 83ceceda3293ffa18b00e3df2f6690d5f9918d5b)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so 4cacd3623a263e81cb2ee8b2d8053acdb3f1e160)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so 4bcb0f75d107aa8d1c497a05ebad8b4bb89d439c)
        ENDIF()
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
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
    # Ignore some warnings on Windows

    IF(WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244")
    ENDIF()

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
