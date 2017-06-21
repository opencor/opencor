PROJECT(SUNDIALSPlugin)

# Name and version of our package

SET(PACKAGE_NAME SUNDIALS)
SET(PACKAGE_VERSION 2.7.0)

# Version of our package's libraries

SET(MAJOR_ARKODE_LIBRARY_VERSION 1)
SET(ARKODE_LIBRARY_VERSION ${MAJOR_ARKODE_LIBRARY_VERSION}.1.0)

SET(MAJOR_CVODE_LIBRARY_VERSION 2)
SET(CVODE_LIBRARY_VERSION ${MAJOR_CVODE_LIBRARY_VERSION}.9.0)

SET(MAJOR_CVODES_LIBRARY_VERSION 2)
SET(CVODES_LIBRARY_VERSION ${MAJOR_CVODES_LIBRARY_VERSION}.9.0)

SET(MAJOR_IDA_LIBRARY_VERSION 2)
SET(IDA_LIBRARY_VERSION ${MAJOR_IDA_LIBRARY_VERSION}.9.0)

SET(MAJOR_IDAS_LIBRARY_VERSION 1)
SET(IDAS_LIBRARY_VERSION ${MAJOR_IDAS_LIBRARY_VERSION}.3.0)

SET(MAJOR_KINSOL_LIBRARY_VERSION 2)
SET(KINSOL_LIBRARY_VERSION ${MAJOR_KINSOL_LIBRARY_VERSION}.9.0)

SET(MAJOR_NVECSERIAL_LIBRARY_VERSION 2)
SET(NVECSERIAL_LIBRARY_VERSION ${MAJOR_NVECSERIAL_LIBRARY_VERSION}.7.0)

# Git tag for our library

SET(GIT_TAG v2.7.0)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(ARKODE_SHARED_LIBRARY sundials_arkode${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ARKODE_IMPORT_LIBRARY sundials_arkode${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CVODE_SHARED_LIBRARY sundials_cvode${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CVODE_IMPORT_LIBRARY sundials_cvode${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(CVODES_SHARED_LIBRARY sundials_cvodes${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CVODES_IMPORT_LIBRARY sundials_cvodes${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(IDA_SHARED_LIBRARY sundials_ida${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IDA_IMPORT_LIBRARY sundials_ida${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(IDAS_SHARED_LIBRARY sundials_idas${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IDAS_IMPORT_LIBRARY sundials_idas${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(KINSOL_SHARED_LIBRARY sundials_kinsol${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(KINSOL_IMPORT_LIBRARY sundials_kinsol${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(NVECSERIAL_SHARED_LIBRARY sundials_nvecserial${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(NVECSERIAL_IMPORT_LIBRARY sundials_nvecserial${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(ARKODE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_arkode${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_ARKODE_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(ARKODE_IMPORT_LIBRARY ${ARKODE_SHARED_LIBRARY})

    SET(CVODE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_cvode${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CVODE_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CVODE_IMPORT_LIBRARY ${CVODE_SHARED_LIBRARY})

    SET(CVODES_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_cvodes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CVODES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CVODES_IMPORT_LIBRARY ${CVODES_SHARED_LIBRARY})

    SET(IDA_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_ida${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_IDA_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(IDA_IMPORT_LIBRARY ${IDA_SHARED_LIBRARY})

    SET(IDAS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_idas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_IDAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(IDAS_IMPORT_LIBRARY ${IDAS_SHARED_LIBRARY})

    SET(KINSOL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_kinsol${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_KINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(KINSOL_IMPORT_LIBRARY ${KINSOL_SHARED_LIBRARY})

    SET(NVECSERIAL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_nvecserial${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_NVECSERIAL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(NVECSERIAL_IMPORT_LIBRARY ${NVECSERIAL_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${ARKODE_SHARED_LIBRARY}
    ${CVODE_SHARED_LIBRARY}
    ${CVODES_SHARED_LIBRARY}
    ${IDA_SHARED_LIBRARY}
    ${IDAS_SHARED_LIBRARY}
    ${KINSOL_SHARED_LIBRARY}
    ${NVECSERIAL_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${ARKODE_SHARED_LIBRARY}
    lib/${CVODE_SHARED_LIBRARY}
    lib/${CVODES_SHARED_LIBRARY}
    lib/${IDA_SHARED_LIBRARY}
    lib/${IDAS_SHARED_LIBRARY}
    lib/${KINSOL_SHARED_LIBRARY}
    lib/${NVECSERIAL_SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${ARKODE_IMPORT_LIBRARY}
        lib/${CVODE_IMPORT_LIBRARY}
        lib/${CVODES_IMPORT_LIBRARY}
        lib/${IDA_IMPORT_LIBRARY}
        lib/${IDAS_IMPORT_LIBRARY}
        lib/${KINSOL_IMPORT_LIBRARY}
        lib/${NVECSERIAL_IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_SUNDIALS_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 9d43ca35e4c9e2fd8b43e9e6aa855976f01e588b
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES a8fb82dfaa68842509822a22cede007e9231a336
                                              bc3c32eb02d2e57620f14142e6a01be49cbd3782
                                              a0c06e21ae76f5cab8a6750037df0da3d3f89808
                                              c03b69c6674eb61ebdbd9f6919b983e0659a5b37
                                              b1b836bcbcdc7026a39a4bce2fcc482e1995a949
                                              3aa3803990175d2e84119494ac8380413ec2b247
                                              9597ce789d0ac17636c4649eb65294ca391b9a2e
                                              3268af524ad4a8705a8e3fc2b295cbbdec53f07f
                                              40a6ec5798f980125ce21b64f3a610ea8bfff15c
                                              7c1909283853a3e68f2a738c5460848572bef721
                                              9e75774f626042dc8c648fd8dff1dea3390e4c02
                                              30bad4f68af01abed35685a69479dc7951f4cf84
                                              58c63672456a4e9371d0b5d072487d07860a3eb0
                                              ddbd155406887c614384e3e2c337fc1e18640171)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 75d9ec874ead43a9be9b4765ee1133ca9380d5c1
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES c3e1583f21ca75f1dde63d6d55ee9119feafc41e
                                              e601a04e0b5848522b9c42241b996951c436ab5b
                                              c2591bc9d6fc41c9d74b4bd5d2f73b246c78efef
                                              19e50452318df2554dcafac4106f62562b15b75d
                                              bee74ca116cca95adb4cebf37d5d0768a47115f9
                                              acc1ca4d80c0be2f2403632a862679421d1786a4
                                              3d366ac08277d7c325a73c9550b43cbdc2719080
                                              0c8644a078305a28f221e74c254a02d746936956
                                              e50bdcaa5085442c566fb5b4a5dbd692594ef9be
                                              07c6c938182eb9b193f7ed7e91f02c2b26c472ae
                                              1a7c175e6f5db4e268cfaef9e65a1fe414cbd8b0
                                              d3daddcabcf7303b5d58325362e1676a914436ac
                                              7a8ee171507bea1120a845767f94cf28c07ca40c
                                              4033f46c9daec99c2b8afdd0f2d30819bcbfd5e4)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 1796c98ea737c207bd376e95c25bb3c224f630b0
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 4caa69790860135deea7ea3c4f91aae0dfc400fa
                                          c2cc5c1b20fabbc2748adb0e382fc429ed646421
                                          7005fcb4eb20ed1d0afee0e6273e2243ec91ede3
                                          a1e0ee55dd5f1ad4ab61065e64809afb053a2631
                                          8f82bf373800c0b8382d7d4ad8150a1e5e644a8a
                                          ea3e07230d3960d2fdf4bdc47b83a8f71da48877
                                          642e874feb5f36cca092ea7554b45facaccdc458)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 3b0551acc4b9ef7f72db17e3e9809250eefc9fe4
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 8285005ceb95212af91f51cee8d4be53849a4ee7
                                          64dacb60af2f993682bb766d49eef986e1be5e6a
                                          d02a46720eb935cd6342f5e0289551c77b139ddf
                                          2cdd1262fbe50108d11e18e2b8e2882ac5d0acf4
                                          b2a7ea41b5796582b536113f156bcee99bf73204
                                          b9b009f5513764cd0af2ecab8c9fce31d63cc334
                                          e4916654986ea7efc63cbd1cb510cc68582fc144)
    ENDIF()
ELSE()
    # Ignore some warnings

    IF(WIN32)
        SET(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244")
    ENDIF()

    # Build SUNDIALS as an external project
    # Note: the --no-warn-unused-cli argument is to prevent CMake from warning
    #       us about NO_FPRINTF_OUTPUT not being used...

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/sundials
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            --no-warn-unused-cli
            -DBUILD_STATIC_LIBS=OFF
            ${DEFINE_CMAKE_APPLE_OPTIONS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DEXAMPLES_ENABLE=OFF
            -DNO_FPRINTF_OUTPUT=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ARKODE_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CVODE_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CVODES_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${IDA_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${IDAS_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${KINSOL_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${NVECSERIAL_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    IF(NOT WIN32)
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyArkodeLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${ARKODE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_arkode${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${ARKODE_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${ARKODE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCvodeLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CVODE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_cvode${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CVODE_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CVODE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCvodesLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CVODES_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_cvodes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CVODES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CVODES_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyIdaLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${IDA_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_ida${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${IDA_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${IDA_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyIdasLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${IDAS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_idas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${IDAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${IDAS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyKinsolLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${KINSOL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_kinsol${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${KINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${KINSOL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyNvecserialLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${NVECSERIAL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_nvecserial${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${NVECSERIAL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${NVECSERIAL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our SUNDIALS package

SET(SUNDIALS_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(SUNDIALS_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(SUNDIALS_ARKODE_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ARKODE_IMPORT_LIBRARY} PARENT_SCOPE)
SET(SUNDIALS_CVODE_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CVODE_IMPORT_LIBRARY} PARENT_SCOPE)
SET(SUNDIALS_CVODES_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CVODES_IMPORT_LIBRARY} PARENT_SCOPE)
SET(SUNDIALS_IDA_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IDA_IMPORT_LIBRARY} PARENT_SCOPE)
SET(SUNDIALS_IDAS_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IDAS_IMPORT_LIBRARY} PARENT_SCOPE)
SET(SUNDIALS_KINSOL_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${KINSOL_IMPORT_LIBRARY} PARENT_SCOPE)
SET(SUNDIALS_NVECSERIAL_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${NVECSERIAL_IMPORT_LIBRARY} PARENT_SCOPE)
SET(SUNDIALS_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_ARKODE_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_CVODE_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_CVODES_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_IDA_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_IDAS_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_KINSOL_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_NVECSERIAL_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/sundialsplugin.cpp
    HEADERS_MOC
        src/sundialsplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
