project(SUNDIALSPlugin)

# Name and version of our package

set(PACKAGE_NAME SUNDIALS)
set(PACKAGE_VERSION 5.0.0)

# Version of our package's libraries

set(MAJOR_ARKODE_LIBRARY_VERSION 4)
set(ARKODE_LIBRARY_VERSION ${MAJOR_ARKODE_LIBRARY_VERSION}.0.0)

set(MAJOR_CVODES_LIBRARY_VERSION 5)
set(CVODES_LIBRARY_VERSION ${MAJOR_CVODES_LIBRARY_VERSION}.0.0)

set(MAJOR_IDAS_LIBRARY_VERSION 4)
set(IDAS_LIBRARY_VERSION ${MAJOR_IDAS_LIBRARY_VERSION}.0.0)

set(MAJOR_KINSOL_LIBRARY_VERSION 5)
set(KINSOL_LIBRARY_VERSION ${MAJOR_KINSOL_LIBRARY_VERSION}.0.0)

set(MAJOR_NVEC_LIBRARY_VERSION 5)
set(NVEC_LIBRARY_VERSION ${MAJOR_NVEC_LIBRARY_VERSION}.0.0)

set(MAJOR_SUNLINSOL_LIBRARY_VERSION 3)
set(SUNLINSOL_LIBRARY_VERSION ${MAJOR_SUNLINSOL_LIBRARY_VERSION}.0.0)

set(MAJOR_SUNNONLINSOL_LIBRARY_VERSION 2)
set(SUNNONLINSOL_LIBRARY_VERSION ${MAJOR_SUNNONLINSOL_LIBRARY_VERSION}.0.0)

set(MAJOR_SUNMATRIX_LIBRARY_VERSION 3)
set(SUNMATRIX_LIBRARY_VERSION ${MAJOR_SUNMATRIX_LIBRARY_VERSION}.0.0)

# Git tag for our library

set(GIT_TAG v5.0.0)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(ARKODE_SHARED_LIBRARY sundials_arkode${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(ARKODE_IMPORT_LIBRARY sundials_arkode${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(CVODES_SHARED_LIBRARY sundials_cvodes${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CVODES_IMPORT_LIBRARY sundials_cvodes${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(IDAS_SHARED_LIBRARY sundials_idas${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IDAS_IMPORT_LIBRARY sundials_idas${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(KINSOL_SHARED_LIBRARY sundials_kinsol${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(KINSOL_IMPORT_LIBRARY sundials_kinsol${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(NVECSERIAL_SHARED_LIBRARY sundials_nvecserial${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(NVECSERIAL_IMPORT_LIBRARY sundials_nvecserial${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNLINSOLBAND_SHARED_LIBRARY sundials_sunlinsolband${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNLINSOLBAND_IMPORT_LIBRARY sundials_sunlinsolband${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNLINSOLDENSE_SHARED_LIBRARY sundials_sunlinsoldense${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNLINSOLDENSE_IMPORT_LIBRARY sundials_sunlinsoldense${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNLINSOLPCG_SHARED_LIBRARY sundials_sunlinsolpcg${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNLINSOLPCG_IMPORT_LIBRARY sundials_sunlinsolpcg${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNLINSOLSPBCGS_SHARED_LIBRARY sundials_sunlinsolspbcgs${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNLINSOLSPBCGS_IMPORT_LIBRARY sundials_sunlinsolspbcgs${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNLINSOLSPFGMR_SHARED_LIBRARY sundials_sunlinsolspfgmr${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNLINSOLSPFGMR_IMPORT_LIBRARY sundials_sunlinsolspfgmr${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNLINSOLSPGMR_SHARED_LIBRARY sundials_sunlinsolspgmr${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNLINSOLSPGMR_IMPORT_LIBRARY sundials_sunlinsolspgmr${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNLINSOLSPTFQMR_SHARED_LIBRARY sundials_sunlinsolsptfqmr${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNLINSOLSPTFQMR_IMPORT_LIBRARY sundials_sunlinsolsptfqmr${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY sundials_sunnonlinsolfixedpoint${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNNONLINSOLFIXEDPOINT_IMPORT_LIBRARY sundials_sunnonlinsolfixedpoint${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNNONLINSOLNEWTON_SHARED_LIBRARY sundials_sunnonlinsolnewton${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNNONLINSOLNEWTON_IMPORT_LIBRARY sundials_sunnonlinsolnewton${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNMATRIXBAND_SHARED_LIBRARY sundials_sunmatrixband${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNMATRIXBAND_IMPORT_LIBRARY sundials_sunmatrixband${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNMATRIXDENSE_SHARED_LIBRARY sundials_sunmatrixdense${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNMATRIXDENSE_IMPORT_LIBRARY sundials_sunmatrixdense${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNMATRIXSPARSE_SHARED_LIBRARY sundials_sunmatrixsparse${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNMATRIXSPARSE_IMPORT_LIBRARY sundials_sunmatrixsparse${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(ARKODE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_arkode${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_ARKODE_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(ARKODE_IMPORT_LIBRARY ${ARKODE_SHARED_LIBRARY})

    set(CVODES_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_cvodes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_CVODES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CVODES_IMPORT_LIBRARY ${CVODES_SHARED_LIBRARY})

    set(IDAS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_idas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_IDAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(IDAS_IMPORT_LIBRARY ${IDAS_SHARED_LIBRARY})

    set(KINSOL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_kinsol${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_KINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(KINSOL_IMPORT_LIBRARY ${KINSOL_SHARED_LIBRARY})

    set(NVECSERIAL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_nvecserial${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_NVEC_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(NVECSERIAL_IMPORT_LIBRARY ${NVECSERIAL_SHARED_LIBRARY})

    set(SUNLINSOLBAND_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolband${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNLINSOLBAND_IMPORT_LIBRARY ${SUNLINSOLBAND_SHARED_LIBRARY})

    set(SUNLINSOLDENSE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsoldense${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNLINSOLDENSE_IMPORT_LIBRARY ${SUNLINSOLDENSE_SHARED_LIBRARY})

    set(SUNLINSOLPCG_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolpcg${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNLINSOLPCG_IMPORT_LIBRARY ${SUNLINSOLPCG_SHARED_LIBRARY})

    set(SUNLINSOLSPBCGS_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspbcgs${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNLINSOLSPBCGS_IMPORT_LIBRARY ${SUNLINSOLSPBCGS_SHARED_LIBRARY})

    set(SUNLINSOLSPFGMR_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspfgmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNLINSOLSPFGMR_IMPORT_LIBRARY ${SUNLINSOLSPFGMR_SHARED_LIBRARY})

    set(SUNLINSOLSPGMR_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspgmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNLINSOLSPGMR_IMPORT_LIBRARY ${SUNLINSOLSPGMR_SHARED_LIBRARY})

    set(SUNLINSOLSPTFQMR_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolsptfqmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNLINSOLSPTFQMR_IMPORT_LIBRARY ${SUNLINSOLSPTFQMR_SHARED_LIBRARY})

    set(SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunnonlinsolfixedpoint${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_SUNNONLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNNONLINSOLFIXEDPOINT_IMPORT_LIBRARY ${SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY})

    set(SUNNONLINSOLNEWTON_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunnonlinsolnewton${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_SUNNONLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNNONLINSOLNEWTON_IMPORT_LIBRARY ${SUNNONLINSOLNEWTON_SHARED_LIBRARY})

    set(SUNMATRIXBAND_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixband${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNMATRIXBAND_IMPORT_LIBRARY ${SUNMATRIXBAND_SHARED_LIBRARY})

    set(SUNMATRIXDENSE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixdense${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNMATRIXDENSE_IMPORT_LIBRARY ${SUNMATRIXDENSE_SHARED_LIBRARY})

    set(SUNMATRIXSPARSE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixsparse${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNMATRIXSPARSE_IMPORT_LIBRARY ${SUNMATRIXSPARSE_SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${ARKODE_SHARED_LIBRARY}
    ${CVODES_SHARED_LIBRARY}
    ${IDAS_SHARED_LIBRARY}
    ${KINSOL_SHARED_LIBRARY}
    ${NVECSERIAL_SHARED_LIBRARY}
    ${SUNLINSOLBAND_SHARED_LIBRARY}
    ${SUNLINSOLDENSE_SHARED_LIBRARY}
    ${SUNLINSOLPCG_SHARED_LIBRARY}
    ${SUNLINSOLSPBCGS_SHARED_LIBRARY}
    ${SUNLINSOLSPFGMR_SHARED_LIBRARY}
    ${SUNLINSOLSPGMR_SHARED_LIBRARY}
    ${SUNLINSOLSPTFQMR_SHARED_LIBRARY}
    ${SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY}
    ${SUNNONLINSOLNEWTON_SHARED_LIBRARY}
    ${SUNMATRIXBAND_SHARED_LIBRARY}
    ${SUNMATRIXDENSE_SHARED_LIBRARY}
    ${SUNMATRIXSPARSE_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${ARKODE_SHARED_LIBRARY}
    lib/${CVODES_SHARED_LIBRARY}
    lib/${IDAS_SHARED_LIBRARY}
    lib/${KINSOL_SHARED_LIBRARY}
    lib/${NVECSERIAL_SHARED_LIBRARY}
    lib/${SUNLINSOLBAND_SHARED_LIBRARY}
    lib/${SUNLINSOLDENSE_SHARED_LIBRARY}
    lib/${SUNLINSOLPCG_SHARED_LIBRARY}
    lib/${SUNLINSOLSPBCGS_SHARED_LIBRARY}
    lib/${SUNLINSOLSPFGMR_SHARED_LIBRARY}
    lib/${SUNLINSOLSPGMR_SHARED_LIBRARY}
    lib/${SUNLINSOLSPTFQMR_SHARED_LIBRARY}
    lib/${SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY}
    lib/${SUNNONLINSOLNEWTON_SHARED_LIBRARY}
    lib/${SUNMATRIXBAND_SHARED_LIBRARY}
    lib/${SUNMATRIXDENSE_SHARED_LIBRARY}
    lib/${SUNMATRIXSPARSE_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${ARKODE_IMPORT_LIBRARY}
        lib/${CVODES_IMPORT_LIBRARY}
        lib/${IDAS_IMPORT_LIBRARY}
        lib/${KINSOL_IMPORT_LIBRARY}
        lib/${NVECSERIAL_IMPORT_LIBRARY}
        lib/${SUNLINSOLBAND_IMPORT_LIBRARY}
        lib/${SUNLINSOLDENSE_IMPORT_LIBRARY}
        lib/${SUNLINSOLPCG_IMPORT_LIBRARY}
        lib/${SUNLINSOLSPBCGS_IMPORT_LIBRARY}
        lib/${SUNLINSOLSPFGMR_IMPORT_LIBRARY}
        lib/${SUNLINSOLSPGMR_IMPORT_LIBRARY}
        lib/${SUNLINSOLSPTFQMR_IMPORT_LIBRARY}
        lib/${SUNNONLINSOLFIXEDPOINT_IMPORT_LIBRARY}
        lib/${SUNNONLINSOLNEWTON_IMPORT_LIBRARY}
        lib/${SUNMATRIXBAND_IMPORT_LIBRARY}
        lib/${SUNMATRIXDENSE_IMPORT_LIBRARY}
        lib/${SUNMATRIXSPARSE_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_SUNDIALS_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} d3364953ab3e961057a292d058123c4038f58247
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES e134a4720b566cb7a34840674364c0accb225b93
                                              cead49234464e6c2bc64405c1681ad472649d110
                                              1fb7e2f26d75d7e9735866b5fb53d9a8885f96f8
                                              7c792be9128218af5a001e3aeb8b25d1ab3ba5ef
                                              7a0318592ead717cb8ee49e90203b31d005dc1b9
                                              8c8eddad28f5515b8548feedce4e8af8befb09f2
                                              ea98516d2db412ab1ffba09fc8740aa904a28773
                                              f4dcda216936743afe2af9a79a260b299530da72
                                              12cd8879e86bdbc187848d076329ca2338184ce1
                                              9d6895ae73aefa910eb4786c6f36504fff98f7dc
                                              40cdb0e66db2e56bf60eea698bd7512d8bcf24d0
                                              b317df5450fabf52560075f6a9100f4b377fa0a6
                                              093b2ee8b01dad96160e26611893e7c53294a0c3
                                              107326b34f7a54c598e5c44b58d6f67b57441813
                                              45661075fb27dfdd5795b96941f5c1d558e64487
                                              59e15f44916a86aecce55c5ea0e1b894c1954a4b
                                              0343a4e459c5ec52d628fed0cbc71d72c2bf9090
                                              2c0d7058289b461f7d38bfaafe9b27cdb4ca34f1
                                              7bb62bfb500be03921764b6c6971460d4b9eb316
                                              79ceadac5abafc897cad81456870bf5c7c981d56
                                              7c2321bee43113ea985079212a8eac27f4739508
                                              16ee06430ef022f9da30f88207043d84a7f38a58
                                              9db3d61a5799e38ad98919cafe1d9b69d9fc6077
                                              09083a8b9ee6c8629163c9ec6a96dbd445db921a
                                              d3d568a6919ae26c020b43553b7d9889c194b3c0
                                              928fccf69c90330708bc71e8ca6adcb26333c424
                                              e98ab5950e6680308782fcd6b155a4f84843269d
                                              4cc8a1b25f6afc81282cc0f1e2e82fbb45ad1214
                                              f785b6c42703e5bcb8a2f4bd4d7066a3c8614d74
                                              687585beb5d8ee312c4cfe3146529e2548a5aab1
                                              8b68560d350851786efcfb12a876ef7a221f087c
                                              0ac8f8cf9814265e0037c6f6ccfe8d207de35e45
                                              bdc57e6dc65474b80d220bc5923d03c192d70c89
                                              5cf3b10006579ef89678cf56c67dcc0cbaf251a7)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} bda5f85f86938a22137e76cc26d20ff9318bdad0
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 828fc37023cdbc6dde9b0a43db25615a0cfc5038
                                              a7dccd96e7f62f08d717be1a16370d65f9151891
                                              2831a5ff6a702a82047a3966267343fa6fe5c0fa
                                              ec9ada18762e189141b3992e2592f1e0ff6a941b
                                              c4d2740aa43f62957e424192ace663e5c56aa272
                                              4c9dad88a233f16a358c6e06309a277bc5b5e537
                                              7d67165f26e3011f438a3a2793a319ef6cf81675
                                              12292188fdd2eb512a25ef4c7dc46ff7d929f4aa
                                              033fdaf239547f18ce3eb58d6626cc4e460f53de
                                              a5c648abcd5d2d114fa798b4d2413408bf0917e8
                                              c6da027b49c03bba47bb205c7b53f864174e6318
                                              b549cdff4cb3dc6b61b2a9ffad2855335a16a7c6
                                              7a8f520c50500a1f77fe9836ddee788e5803d633
                                              6931bf6946e83c358fed241c4428d9e4515e90ff
                                              3f3c5ca945ce3b95f74f8cad82e1d255af79b630
                                              a87c25a6bb3109c8552a2fbe3ffa5fde42d67391
                                              ddc035d735efb63e89d9146e240a54d7253e98cd
                                              2c0d7058289b461f7d38bfaafe9b27cdb4ca34f1
                                              7bb62bfb500be03921764b6c6971460d4b9eb316
                                              79ceadac5abafc897cad81456870bf5c7c981d56
                                              7c2321bee43113ea985079212a8eac27f4739508
                                              16ee06430ef022f9da30f88207043d84a7f38a58
                                              9db3d61a5799e38ad98919cafe1d9b69d9fc6077
                                              09083a8b9ee6c8629163c9ec6a96dbd445db921a
                                              d3d568a6919ae26c020b43553b7d9889c194b3c0
                                              928fccf69c90330708bc71e8ca6adcb26333c424
                                              e98ab5950e6680308782fcd6b155a4f84843269d
                                              4cc8a1b25f6afc81282cc0f1e2e82fbb45ad1214
                                              f785b6c42703e5bcb8a2f4bd4d7066a3c8614d74
                                              687585beb5d8ee312c4cfe3146529e2548a5aab1
                                              8b68560d350851786efcfb12a876ef7a221f087c
                                              0ac8f8cf9814265e0037c6f6ccfe8d207de35e45
                                              bdc57e6dc65474b80d220bc5923d03c192d70c89
                                              5cf3b10006579ef89678cf56c67dcc0cbaf251a7)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} eee3adeb77d9511feb31e13cb226a073a396e108
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 971d21dac86c2ab9778156daa34c198b0ea1d14f
                                          fc7033cd140faa747af39bf4a4520665fd75f1b2
                                          ed25cdda89be613d89adb9c3daf1505613732475
                                          cf34b474318d48b832ea741e6902cadf91559527
                                          6fd533213ce2efdeb9069cd9b90090173cd4d758
                                          3c1568a7780eb4262de240ee88b03127056907a0
                                          f8565cc16132f68f03e3e9991e41d16d4b76e9ff
                                          b774559134d89863f4c4c077886946e0e0aec2ff
                                          52c05ae4c8622008cc38430d2d3591ea5ff5129a
                                          c7cb9fa304532793537a414e2117bcb9836218e2
                                          779d498c3ef189544b0bf0ee93005a8eb797ea0b
                                          a85220d36ab73f308856d1cf55c5938750838152
                                          7a6ae3452fda90437a22b18469fb7df822bb305c
                                          f8ca7df8006612ff9fbab832aab97dde04a96a3d
                                          ca3de2d24e66165351436f8ed841a3c0ec978573
                                          41c87a55e9db850fc5dd45570f7efecbe0784544
                                          a82f46f6c7fb8d22b56a4caa4203cc9efa375353)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} f6adada52dc5524888a2f4f119b474a9bc121ef6
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 1df2e445230a37c7fd66d718a3ee44bb9e4f3bb1
                                          5ea71f5990bdf3eaf7284bec8906beeb2f83b9c7
                                          f11403670a73b114e8bce23f528d3b9646f86271
                                          adea8a6e4c7471ff214f003b0c071597b50171d6
                                          fd859bca3cecafeca52e882c5804d96e9fe47e80
                                          417c81ac4b93fe01e95f6865c2db83e6a0b6494d
                                          0aba846c3ec2524c400634b6930d9fd57a7f635a
                                          703b2ceb95e8d57d1207a6c067cdddf8e777f9c4
                                          b61d945d5524dcdc762908b45c8dc055ce7a61f9
                                          366550166697aaeace76b991af283cfaaf66be52
                                          bc8221cff89b934cc9819438dfe45d87dc98e7ed
                                          1186bba6042bc451b531b40109bcf985571e252e
                                          d5029573d63bc20dde520f4242b8513fc0f9e21d
                                          f988358c02d4dda835c64507f067c3ea5ca829db
                                          e34015490994e3566a772969bcf07913c0d09dae
                                          cfe2adbb5f41205bf3d33d8e7f4e84e8b8e8a2e7
                                          2ba94a9f4f12731cc17ec07bd346ffe174fc9144)
    endif()
else()
    # Ignore some warnings

    if(WIN32)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244")
    endif()

    # Build SUNDIALS as an external project
    # Note: the --no-warn-unused-cli argument is to prevent CMake from warning
    #       us about NO_FPRINTF_OUTPUT not being used...

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

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
            -DBUILD_CVODE=OFF
            -DBUILD_IDA=OFF
            -DBUILD_STATIC_LIBS=OFF
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DEXAMPLES_ENABLE_C=OFF
            -DNO_FPRINTF_OUTPUT=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ARKODE_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${CVODES_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${IDAS_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${KINSOL_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${NVECSERIAL_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNLINSOLBAND_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNLINSOLDENSE_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNLINSOLPCG_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNLINSOLSPBCGS_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNLINSOLSPFGMR_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNLINSOLSPGMR_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNLINSOLSPTFQMR_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNNONLINSOLFIXEDPOINT_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNNONLINSOLNEWTON_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNMATRIXBAND_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNMATRIXDENSE_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNMATRIXSPARSE_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyArkodeLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${ARKODE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_arkode${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${ARKODE_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${ARKODE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyCvodesLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CVODES_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_cvodes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CVODES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CVODES_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyIdasLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${IDAS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_idas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${IDAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${IDAS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyKinsolLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${KINSOL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_kinsol${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${KINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${KINSOL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyNvecserialLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${NVECSERIAL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_nvecserial${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${NVEC_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${NVECSERIAL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunlinsolbandLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLBAND_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolband${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLBAND_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunlinsoldenseLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLDENSE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsoldense${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLDENSE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunlinsolpcgLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLPCG_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolpcg${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLPCG_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunlinsolspbcgsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPBCGS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspbcgs${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPBCGS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunlinsolspfgmrLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPFGMR_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspfgmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPFGMR_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunlinsolspgmrLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPGMR_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspgmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPGMR_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunlinsolsptfqmrLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPTFQMR_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolsptfqmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPTFQMR_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunnonlinsolfixedpointLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunnonlinsolfixedpoint${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNNONLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunnonlinsolnewtonLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNNONLINSOLNEWTON_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunnonlinsolnewton${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNNONLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNNONLINSOLNEWTON_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunmatrixbandLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNMATRIXBAND_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixband${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNMATRIXBAND_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunmatrixdenseLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNMATRIXDENSE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixdense${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNMATRIXDENSE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copySunmatrixsparseLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNMATRIXSPARSE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixsparse${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNMATRIXSPARSE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)
    endif()

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our SUNDIALS package

set(SUNDIALS_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(SUNDIALS_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(SUNDIALS_ARKODE_LIBRARY ${EXTERNAL_BINARIES_DIR}/${ARKODE_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_CVODES_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CVODES_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_IDAS_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IDAS_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_KINSOL_LIBRARY ${EXTERNAL_BINARIES_DIR}/${KINSOL_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_NVECSERIAL_LIBRARY ${EXTERNAL_BINARIES_DIR}/${NVECSERIAL_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNLINSOLBAND_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNLINSOLBAND_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNLINSOLDENSE_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNLINSOLDENSE_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNLINSOLPCG_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNLINSOLPCG_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNLINSOLSPBCGS_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNLINSOLSPBCGS_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNLINSOLSPFGMR_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNLINSOLSPFGMR_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNLINSOLSPGMR_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNLINSOLSPGMR_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNLINSOLSPTFQMR_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNLINSOLSPTFQMR_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNNONLINSOLFIXEDPOINT_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNNONLINSOLFIXEDPOINT_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNNONLINSOLNEWTON_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNNONLINSOLNEWTON_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNMATRIXBAND_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNMATRIXBAND_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNMATRIXDENSE_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNMATRIXDENSE_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_SUNMATRIXSPARSE_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SUNMATRIXSPARSE_IMPORT_LIBRARY} PARENT_SCOPE)
set(SUNDIALS_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_ARKODE_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_CVODES_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_IDAS_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_KINSOL_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_NVECSERIAL_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNLINSOLBAND_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNLINSOLDENSE_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNLINSOLPCG_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNLINSOLSPBCGS_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNLINSOLSPFGMR_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNLINSOLSPGMR_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNLINSOLSPTFQMR_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNNONLINSOLFIXEDPOINT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNNONLINSOLNEWTON_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNMATRIXBAND_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNMATRIXDENSE_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SUNDIALS_SUNMATRIXSPARSE_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/sundialsplugin.cpp
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
