project(SUNDIALSPlugin)

# Name and version of our package

set(PACKAGE_NAME SUNDIALS)
set(PACKAGE_VERSION 3.1.0)

# Version of our package's libraries

set(MAJOR_ARKODE_LIBRARY_VERSION 2)
set(ARKODE_LIBRARY_VERSION ${MAJOR_ARKODE_LIBRARY_VERSION}.1.0)

set(MAJOR_CVODES_LIBRARY_VERSION 3)
set(CVODES_LIBRARY_VERSION ${MAJOR_CVODES_LIBRARY_VERSION}.1.0)

set(MAJOR_IDAS_LIBRARY_VERSION 2)
set(IDAS_LIBRARY_VERSION ${MAJOR_IDAS_LIBRARY_VERSION}.1.0)

set(MAJOR_KINSOL_LIBRARY_VERSION 3)
set(KINSOL_LIBRARY_VERSION ${MAJOR_KINSOL_LIBRARY_VERSION}.1.0)

set(MAJOR_NVEC_LIBRARY_VERSION 3)
set(NVEC_LIBRARY_VERSION ${MAJOR_NVEC_LIBRARY_VERSION}.1.0)

set(MAJOR_SUNLINSOL_LIBRARY_VERSION 1)
set(SUNLINSOL_LIBRARY_VERSION ${MAJOR_SUNLINSOL_LIBRARY_VERSION}.1.0)

set(MAJOR_SUNMATRIX_LIBRARY_VERSION 1)
set(SUNMATRIX_LIBRARY_VERSION ${MAJOR_SUNMATRIX_LIBRARY_VERSION}.1.0)

# Git tag for our library

set(GIT_TAG v3.1.0)

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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 8f9caff8c990fc763f91fd34bdddf35b70bdf9cf
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 048fe548f131b145e3a74e5b41b4ec49ca05e931
                                              ddfe71d6a61ab75cbe95c443f540aa988bff713b
                                              0a61ce3d4d2cc95e04bf0f1dce52253574ad669f
                                              91aa28cf8e4f506e4e88c80abe320c62dd988b7f
                                              79822b547ae3ab4e6fbbd709df7ebb9d89e5d16e
                                              a11ecddee0eba9a18bbd9347297daec97ad317f7
                                              b73746482b235511109a29d7b7f03bc2b698d172
                                              88b72c172bcae568f666c990a4b757c3e5bed814
                                              52fa76f3ed74d4cf17911fbeb13cd3be842fd413
                                              939d8e342e090c42dd5851be7c116a4b36a870a9
                                              9a9697d442eb40b710a465531a59e04ab1a43161
                                              33a72daae041a4ae9a1b5fdb150130b6116c2a61
                                              0e915b6f27921e0e8c64deeae62eb7f428c52a56
                                              b72dc4ca01b426a1e720f259352483b21400332e
                                              aa94a75974f00bd23da61d1becebcbe82a2f181c
                                              8beb661030d24a5e675cb649c73f8fb4a4f6325c
                                              56af7b5276d4a159228c7baec0307866b09a8ff5
                                              314d107f7afef6283be6d92a94475a7d9d555443
                                              ee79fd1345366624a5a696b181bb8479528d319e
                                              7c4ca54139e1f0d53e41ae505bece0336ca30a0c
                                              8cf3983893bcd2a569ac54a471dca79f2bdbb816
                                              bd3c67b0dc2270fcb40f1351ea1841dbfdb9daf0
                                              bf6930867826e4f745087e4e722ae69ce5638a6a
                                              281358b68191e3acbe139102d595de9177a8510c
                                              fd4ad0c6767ae8f43aa5d3f0ad575f1c0db6179c
                                              9e3ceb2704095721efa9cf503155de86cd46d8a2
                                              94fa607df2ecf76d3325eb1bb19d81efdea38e3e
                                              8937a2c45f8c387b3938d4e3bd6ac09c5d25e5ae
                                              3e13e5a9d8074914169885364ff1a068fd9b66a5
                                              b32b73cf25f38ce5452468203b7815e57bc1a543)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} d4ea843e8f52c78d1ab4855234749718f97920ef
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 9faa146d7e890a1c026835ff45c8e4fecd0ebe91
                                              1175fbd417bd8e645fe8b411e9adb7cfa56f0ac2
                                              01b057be12b3d5117daa62ae770d9d4de30afecd
                                              1beaee93944a85f3500cd3aae46bd94522368c3f
                                              4a936bbdd94621f7080cb778de780ab0f041c1cf
                                              c6a6d15c53be271786ff3fe44039dea8dcc86dcd
                                              4ec7fe9ef505ec24a62a49ad226d201a0d00e7e1
                                              7e1597b8db3d5609bb26d7c340c50a4b3a64b048
                                              fcd2ccc78b5bd52ecc3ae87187ff936a13397dc6
                                              067abc153236aa2221b3e8f55f73e1f519c62e00
                                              d6727d81219a15ad72ec52c66bffb264f21bf503
                                              951839db2903292a2fa2e033494799301e3e3212
                                              fde19d634fa8d7844bc15dcc6516b7810895e28d
                                              6612209f31872f3ada022217f213febbe7448ed5
                                              841779c3cf3bef7eccb29ec59da20e475ff04d2f
                                              e01803aee275650f3844c73e541c471c086c95be
                                              3f35a475b8bace013b49242b8b802a284727c417
                                              f09ca06600b352ccdc0a3f48a4e0e1a2f91a900b
                                              6e3e4948f30669baae440264359042ec69397aae
                                              169d9d071e0db05975c50f6b59cd2e3fbde5a402
                                              3b580209ad74c6f0be7838669fad1505faec4536
                                              82ed7bed528ad7ce6c231d0ff27d0d09b72f0185
                                              a5e704489e82089d9d6fec7e4af9e1735418712e
                                              352908847fbb20e55d9cac3434e0a184d50e0adb
                                              89fe85e459b31245f45442f1dd52c28b96ffd6c3
                                              2d138240606f50762da18751976a036c41538fcf
                                              4b1732cd01832db16ef100b7f14e5a767d76c337
                                              03a6e804bbf45d19deb4d81afa8146b1ee071977
                                              c8924b2c5ecc67435ae6cafd2c70b3be96d6a6fc
                                              9bc26a3235512e5e58e6090d34d62f730728a5a6)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} c2ba5abe03c5fab7082bcd435154692a0d8d970c
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES d676c801951a67b09c461ab15871f22207e8d7d0
                                          d840a4fee4ec51c9ecb9d4399c246936c1996f84
                                          846dfc8faea43b40cf5c553eb7da5e1ef35b39c9
                                          78632e36664f4d6c6f32087e874dc513034d5d67
                                          b41010934919debe1d30430b268ceca42d5fbf30
                                          c080de1a9c8a808e708c8899595d14fae67438ac
                                          21f11fe6246a0a90965a45a259f2acfb40b68090
                                          54e2ca25623f19ba8a0a5556af70792d244b6e99
                                          ef67af16dcf6826c102a5a4328038a1c6a2db93b
                                          69e765532e6c4992716bd9caaba6aa36c550470a
                                          0d5079776894f3f76072a19b593d711278767ac2
                                          11636ae08ac37077754bec33e5064e8b781f9d1c
                                          eb51a13f09fc07ff7274948c395ed2114fc7773f
                                          e7a89a68efde3019927eea418ccfe34a1de9272b
                                          65ceee05175b8bd5e92b1d3d19fcc2f24d15c8aa)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} cecc2b5af4badea17a2cfc1c842270d0f7c487e6
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 73b854cb7a3e89886ba6e47385adca78be0d3b7a
                                          d0f812410d83fd568bea63afda83ac168f8afd91
                                          5673ca86fc8b6285e12f60289ccac6cc2b78bf63
                                          d3992dcc5d89e82495898f38bdceecfc446db50a
                                          4066693cf0e3bafe16cea00a10f1f84ca99bd6e4
                                          64b540ec2e1a2da4040d08a88bb12d7b77033630
                                          4ab9e3861b449b8eccc61d45ca6e6279399e98c9
                                          f77cf33187bf0e1ec40ae01401d66752f8f052c9
                                          9f1fded25d840a095a8bf287ecb168d5bb9bad2f
                                          6a29c9670c065ae14c06ec962d67e0e75ec055a4
                                          3ba2e15819196103ef57faa73ca701f8106e551d
                                          88792b4d789aa5c9b7923a9bfa00b3af02c0e307
                                          0fff38db8619cde80490d48621be1268c7a95c93
                                          4ed6cbcc50d9283d268ca6813539bea70332726a
                                          56c1f6ab185466d04f1c72323e59e30e0c73feb1)
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
            -DEXAMPLES_ENABLE=OFF
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
            <INSTALL_DIR>/lib/${SUNMATRIXBAND_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNMATRIXDENSE_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SUNMATRIXSPARSE_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyArkodeLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${ARKODE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_arkode${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${ARKODE_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${ARKODE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCvodesLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${CVODES_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_cvodes${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${CVODES_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${CVODES_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyIdasLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${IDAS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_idas${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${IDAS_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${IDAS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyKinsolLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${KINSOL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_kinsol${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${KINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${KINSOL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyNvecserialLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${NVECSERIAL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_nvecserial${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${NVEC_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${NVECSERIAL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunlinsolbandLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLBAND_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolband${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLBAND_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunlinsoldenseLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLDENSE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsoldense${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLDENSE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunlinsolpcgLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLPCG_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolpcg${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLPCG_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunlinsolspbcgsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPBCGS_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspbcgs${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPBCGS_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunlinsolspfgmrLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPFGMR_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspfgmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPFGMR_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunlinsolspgmrLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPGMR_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolspgmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPGMR_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunlinsolsptfqmrLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNLINSOLSPTFQMR_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunlinsolsptfqmr${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNLINSOLSPTFQMR_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunmatrixbandLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNMATRIXBAND_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixband${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNMATRIXBAND_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunmatrixdenseLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNMATRIXDENSE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixdense${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNMATRIXDENSE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopySunmatrixsparseLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SUNMATRIXSPARSE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunmatrixsparse${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${SUNMATRIX_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SUNMATRIXSPARSE_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)
    endif()

    # Package our external project

    CREATE_PACKAGE_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
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
