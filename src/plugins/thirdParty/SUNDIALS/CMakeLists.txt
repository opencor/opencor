project(SUNDIALSPlugin)

# Name and version of our package

set(PACKAGE_NAME SUNDIALS)
set(PACKAGE_VERSION 4.1.0)

# Version of our package's libraries

set(MAJOR_ARKODE_LIBRARY_VERSION 3)
set(ARKODE_LIBRARY_VERSION ${MAJOR_ARKODE_LIBRARY_VERSION}.1.0)

set(MAJOR_CVODES_LIBRARY_VERSION 4)
set(CVODES_LIBRARY_VERSION ${MAJOR_CVODES_LIBRARY_VERSION}.1.0)

set(MAJOR_IDAS_LIBRARY_VERSION 3)
set(IDAS_LIBRARY_VERSION ${MAJOR_IDAS_LIBRARY_VERSION}.1.0)

set(MAJOR_KINSOL_LIBRARY_VERSION 4)
set(KINSOL_LIBRARY_VERSION ${MAJOR_KINSOL_LIBRARY_VERSION}.1.0)

set(MAJOR_NVEC_LIBRARY_VERSION 4)
set(NVEC_LIBRARY_VERSION ${MAJOR_NVEC_LIBRARY_VERSION}.1.0)

set(MAJOR_SUNLINSOL_LIBRARY_VERSION 2)
set(SUNLINSOL_LIBRARY_VERSION ${MAJOR_SUNLINSOL_LIBRARY_VERSION}.1.0)

set(MAJOR_SUNNONLINSOL_LIBRARY_VERSION 1)
set(SUNNONLINSOL_LIBRARY_VERSION ${MAJOR_SUNNONLINSOL_LIBRARY_VERSION}.1.0)

set(MAJOR_SUNMATRIX_LIBRARY_VERSION 2)
set(SUNMATRIX_LIBRARY_VERSION ${MAJOR_SUNMATRIX_LIBRARY_VERSION}.1.0)

# Git tag for our library

set(GIT_TAG v4.1.0)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

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

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 32fb748c269adbd08c580580e2b7078caf88e5d5
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES d6ec8cb0c89122f976254d1dea412d7d102b8e6d
                                              53dd5f33110bcef0c47441c58374ba6d94a5d74c
                                              18492b2145ec01e9b6d8e7bab6262a7af6e1cb72
                                              a39fb607e81a3fd92028b197cce7e8045d1e8c43
                                              02c3ef3ac3c088c2608f22679df8b15466a4dc57
                                              472c2fd55f6a9e51c80f5f254d854eec972fbf8f
                                              4595512d73d1f24bba0217e3d945412d606ffd5d
                                              f3d4234f91dc61280c28d76f5fb1bf0e694a2e2e
                                              9281538a2a40d36f88fc63421f21d10f3a4d81dc
                                              7631e362dcd95e1490c42d430390acefb649e3a4
                                              1d82951f4d2c31bc7bdcfd6876784d61c4c1b200
                                              4bb7f3766d442e51f2ea3ba745a80a2a76528bdb
                                              c5e513a5f2e99bc24d90a67a956d4f9d0e32826f
                                              05fad934b1716128c7996ebef63c66491ea61a14
                                              b444b940b6d071208504793d5a544984ce8255e8
                                              897061c2b0ce25e651bad5895897a373732e50ab
                                              a33dbc41c51dc2d885b3b338dc552e1907e4bf77
                                              0a276f3e8191f92f7c5891d4a8f5a3ac5d96a49a
                                              97f00a6fb6057373217f50c29d22b520f1951064
                                              a2850820263b05cc78c71c6c2901b6d8d2b1cba1
                                              22fe7bc63c0a8e6beb5f13d0ff8d1114f11e3eb4
                                              ee0c7c0885240edbd1a1bddd9a24ac75a387dee2
                                              2c6965488e803320ccb220413e0d00e8efd0deb0
                                              00aa5979ea0e09d202dda6825a73414e01f370dd
                                              665acc4548f869c2e41170021729f9528707f758
                                              41babc05ccffda1d6e3f1b9ab79513ac5e43f7b0
                                              c24bd32bb0fd8e4dc6e1f893b7e77994870540e5
                                              c0c791974cac1c568f7e3728e95fbe88393be007
                                              5da154edd86ddbbd51430b35438db6ece4f59b85
                                              2a89c4d9d00855b4dfa83745e20c8f4bfedc29bf
                                              1da70bacec01254041391d1b456f07766667cc0a
                                              707feb630c72bf47bddfafbc3fe754e07bfb0f6e
                                              f56130124ad8db4771727ba4853e9d4442f636c6
                                              154c3528340f6cccc1cef2a19dd737fd811c668a)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 49a882ac32d48186673ad93f8b7501ec4fb01f6e
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 197163e3fb945f6aa737d064ce734215ead58b00
                                              bcfbecd63319a184fa79c8c71be7efbb509f170d
                                              831d791159375c2befe44d739aac5b31074e5c54
                                              0d346120d4bce53ee47c7a3af90db81c1767cdaa
                                              8b0eb5419dbde4ee8cb9c46fbca40ad072dedd5e
                                              e54e92f9d7951d0544972bc570c1669cdd23b5d3
                                              dc9712d2ded02643c62e3cfc6275bf91267f0636
                                              5501e1f4e9fe29fea9fbfd878b904f65261f298b
                                              e4ac0653f804a64a4d4810978f5584fffa57d128
                                              65955c5840cde9edeab3b0bf5d7b009147a96f11
                                              792d0b4e6da0ab8fdce9dd3b6455eed547304b3a
                                              5b8d6c9eba4a6c961dba0de9d6291944ce55f973
                                              ec7d8ac5fb4c6e9cdddeccb91b4ceba28c4c3bc3
                                              bdc1ecc206c20ec66da719a8274c63b1703628f6
                                              01100f041f1c8c76e2435199a41ac4c38f34c141
                                              2076d8ab6b0872e2bb1585f3e584d3da7bf1ac7e
                                              01ba44b2022f2cda84a83663a3f39dc2f7f6f769
                                              70d2a22dea54764e3fcc8d15c53e664dd793f644
                                              28058576bb4bea054b890af7f01bcf0238c462bc
                                              ab7e63e5a461e4f22fd8431720f7fe0b8866664a
                                              c616c5cf5ef7ab1b594c7c60c78b2320ca09801e
                                              d23fe7d4487965faca9883ae4efecfa134c28714
                                              0b74468103c04b2bee2bd3b2ebb922ca555d1e0e
                                              711a663780c060dddf0406b72bbe29c84b3e378d
                                              b64dda230beaa861cd4fe8f9c6709f10a31a6686
                                              e8824017eac002d5b511bbfd8848bd720a0a5dab
                                              65562976122725ae92f1d2921415be063109ecce
                                              f853761335bcdaa62717111459f4355da9437eda
                                              2ecdb07008b87960f86fef282c8a4cf72fcf8afd
                                              b5225889924d1e21da5e645451df8f2f49c56488
                                              097c93fb3089362e10e375794c5bd6ff82e3932b
                                              894de0e50e00d59863557b8e268dba1dabf14708
                                              9c95abcb9253403a0b353bcf0bf833266ff4ef19
                                              07404bfd03309ea8786e620d3089ea081eb2c930)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 38ad835f294bf82ff7c8a6e7241acf198c8057e6
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 4486898d89e85aa758193d09c37ab996f7333da2
                                          c8f77f0030c9cfb0eeb0a95ff3e804b571f726ef
                                          f58043ede883815ab43214f879460b6545e6ca87
                                          4cec2cdeb5ed7f81778d535d1fc53194a18b749c
                                          e453232f2fef61ecafb5b39e361c2c8d847077bd
                                          102040d7943b3640b7a096378fc1559d7e95d00c
                                          d09a189c2200ff813601ecae1f0353f76f908d24
                                          9585060f27f0bf6664782d5290d316fc4375f6dd
                                          363271347c89bfefb5b640d160946934c018be42
                                          7de96939f98006ae4c04c013c8b73779dd2ac993
                                          ba99726bf9aee9fbbc7526645f63ac80d75a93bb
                                          16789da67eebf7422aa723266cb202a0bfc1a985
                                          7a00a81dd6d266c74aeae61a67d485b946dac9a2
                                          69a670e05bc9718c54bd2d841e0f9db590ffcd9c
                                          2b6c3d8dd7529743d6fb1cdadd3cfae27eda33f8
                                          7df762a35b908472108d1519df78533b4e5ef59f
                                          047ba21ed44712eb535dae156b4afa8d074cbba9)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 3d1dcaccff7e6d97b3655abde2a1f76f4633810e
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES e582e17ab70bad54297d12335b49ea9758e3270b
                                          ca8d7a57f2c7e17f4b8e022b5b7b02950fb1ff43
                                          398ae260acb42acbe437d7b33855175be52f3e8f
                                          270980a3b7b14aa5d6340c29b9086a7766a1f33c
                                          fc29f0aa96e562779fef2d9c6c1b149c1e0999da
                                          71831bd1eecf45c8e35424bf28457527a43d1e5d
                                          43109914af6ab7de66fcb2a1c62272f0acb473c4
                                          d39f632c89c2499d98d906ba1e6f6303b242749a
                                          ae4c0b3ed64dac0724a79459dcf31cc68e128585
                                          b0e34412031594bb9263eeaf199e5d8ab8bab41f
                                          7a26b9e433472a8dfebc54a56392a992ded2f98d
                                          ec4e780cae75382707d1ae959d6a9299012800ee
                                          aa1651ed9ca3031896434446d13228feee88aaa1
                                          d4ee2b9a884c5f763a1505e057a77a9e508b1c4c
                                          7f1dccf31d7bc0d9b0f9b76377e06db79f8aa5fb
                                          dcf4879289e3611797fbc47692369c8c867627fc
                                          dac6e6607d7d28796c8b420ca514b3dea98b9a0c)
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
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
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
