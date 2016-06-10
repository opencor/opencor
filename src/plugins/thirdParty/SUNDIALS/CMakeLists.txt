PROJECT(SUNDIALSPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_SUNDIALS_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 41a85a438084930025ac8af506a891b1453b3aa2)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib a138ea253d1c6a957ef9950b6962bd67e7050ec1)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.dll 4162fdfe05ccb458e2dbe85d91efe1d03a6b6ed8)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} SUNDIALS.lib a44541267edf988a1974ad41de6cf1d99d7bc826)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.dylib 4f5b7a8d51a0866a126e1ba5876f605210bb7eae)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libSUNDIALS.so 81924dec13a778d401c4e36f117ad58f3ce7a811)
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(SUNDIALS_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}SUNDIALS${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
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
