PROJECT(SUNDIALSPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_SUNDIALS_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(${ARCHITECTURE} EQUAL 32)
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll dc746cfc27a789cabdeb981ad380f4aa82b15889)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib addaf37b74c4737f61bc7b3dd06f5bf4958ad678)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 3077bdad8f3de5bf5ebd067520b656049ba0ed6c)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib 6761d6387a9f22a90b11942e8e9267b5de49626d)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 5658f0e885f785bf58511a3e0288cb12dbb7adc1)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib d6933a3e20ac941628d481329b0e796e7a8aed0e)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 59b96825fdbeeea0f789e8c9db268f808a3a7f67)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib f4060e12066720ce4d798f98f43d96392ad7bcba)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.dylib 2ccdae4f623f98535cfd5d51bc84f2076be98d62)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so 85e667cb002e9aebf73a638c32c690ab5eac7dfe)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so 53a816ab4c31685c9d4b95120733cbd7c50ecb25)
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
    )
ENDIF()
