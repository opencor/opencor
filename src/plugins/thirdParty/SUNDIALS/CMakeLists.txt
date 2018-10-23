project(SUNDIALSPlugin)

# Name and version of our package

set(PACKAGE_NAME SUNDIALS)
set(PACKAGE_VERSION 3.2.1)

# Version of our package's libraries

set(MAJOR_ARKODE_LIBRARY_VERSION 2)
set(ARKODE_LIBRARY_VERSION ${MAJOR_ARKODE_LIBRARY_VERSION}.2.1)

set(MAJOR_CVODES_LIBRARY_VERSION 3)
set(CVODES_LIBRARY_VERSION ${MAJOR_CVODES_LIBRARY_VERSION}.2.1)

set(MAJOR_IDAS_LIBRARY_VERSION 2)
set(IDAS_LIBRARY_VERSION ${MAJOR_IDAS_LIBRARY_VERSION}.2.1)

set(MAJOR_KINSOL_LIBRARY_VERSION 3)
set(KINSOL_LIBRARY_VERSION ${MAJOR_KINSOL_LIBRARY_VERSION}.2.1)

set(MAJOR_NVEC_LIBRARY_VERSION 3)
set(NVEC_LIBRARY_VERSION ${MAJOR_NVEC_LIBRARY_VERSION}.2.1)

set(MAJOR_SUNLINSOL_LIBRARY_VERSION 1)
set(SUNLINSOL_LIBRARY_VERSION ${MAJOR_SUNLINSOL_LIBRARY_VERSION}.2.1)

set(MAJOR_SUNMATRIX_LIBRARY_VERSION 1)
set(SUNMATRIX_LIBRARY_VERSION ${MAJOR_SUNMATRIX_LIBRARY_VERSION}.2.1)

# Git tag for our library

set(GIT_TAG v3.2.1)

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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} ecfeb37085a7e81cbd84b2a930340f4666a595d9
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 274d9727d46723499920c2004f3554b719ad35e4
                                              00198fa3b96553c96a4d922d3f358f726123625f
                                              3886f20e20bdc8f0f891833df9ec5b03f951aa16
                                              a1cf451123757f346dbd7375a7088e8673315ae5
                                              3328153a400188fe382bdae0b9a7845a5a6a2b62
                                              6c0e341cbbfd5e587a51fa6e1c6d49810d28760d
                                              0b646e2458538ae9726ee4dc540c89b80970fa0c
                                              8c3d53316a270827d77042a74eacb8ac590d7c31
                                              6d94161e5b74d534956e8eec7d902fcff16b451f
                                              4cad654848ea1e7eb7ffd4b584712c3a2aeaf61e
                                              c2fe21ba2ca7ec291b1f2934d7e4b0094857ad0e
                                              c42e09b5f406f06045edb4d155c4f125fe0bc684
                                              0996074e7c58851968ad805b025395338d3f96be
                                              16ade14965efcc011208ad17daabfae8b5b03f60
                                              10209091cbe0985848c11a7c1cc7bf97d6413ac5
                                              48845723ed8f1cb761d59fac9bfc5476b9a0ec95
                                              3ab286feb20a13e48e9bfad24bbce572e9d2d7cd
                                              e5b07e7ebefb239e13995b510753d0284eaf9f30
                                              7da90b39186e6ced47b6c47367dad34487bb417e
                                              a44e3496c5dc48b7d1419d36568be151876edef9
                                              0e580ce7e3f8acd07358a7a00a13b04598c64886
                                              927827730fd4fbf32ea7e612b5ce82c5e663554b
                                              8dff350c6c92c3cd4d85293acc0337269fe96f18
                                              2338181edc897e5c3ffab2653b481944c156fd32
                                              a1470647eb08198ff46afc8b59a4e49c222aea02
                                              6ae54bdea809a4a88c78df6c7b37ccdce90e6e33
                                              88b272c61ee1e59495af0952858c2152d7198c34
                                              268df6f422c758e79244df83b0db3e53b71c9168
                                              807a890630d423aa89849f294572f3363be8ee3c
                                              bfddd33efb040ae9ea3aea9d28e166dba5acd0ec)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} e50d266abdae5b5cc3fde40520dbdf886c1af60f
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 8cb1bce32b87a8b32bb9fd9149f7f56cc0681011
                                              9a1b59155f5872e9821f58061662a270ec5354a8
                                              c02b3fc080c049a8b94abcc6bc6211f80f4a4289
                                              6b0a54f6400a502fed83a5fd9468600976baabfd
                                              58899aa2999ba28e4371b66bd2acaf45796acbd4
                                              b6ee0d77a189e8a9f3b466844ab62960c2c6254f
                                              4ab7f8cca35262870c2c3efc7d1e4a7c1c650ff7
                                              3c0d30fea54d0f98765cb934e1b73650648bb315
                                              c0bba6c7b7d517aab3b9b8710f52a068f9f65c3a
                                              27ad292763d377c4137dcd63f2e5a73ecb088457
                                              b6faa40e3551bbad361a89a3db4caccbb01cfe61
                                              412008e0df141183eccccf147b4bfd8fc50f0873
                                              2656170410f585018c9d2bfbdf42fa87e4b4560c
                                              1c1dd83db7dd16ab9b9858b1c5aabc44f0100bc8
                                              9143a4c317d9cc6745cf1cea91e4df0c58883350
                                              03c9ec1e5f762e9c57ecb1115e1bb1273c96568c
                                              2050cb313bc09f0b8881173069580a895acb179f
                                              3b4461173a39a065621b90f2edc3c031d53e8022
                                              3de57414bc9b95377dcbbee580727e4340398696
                                              510568fbd5f74475796b6a2118f9c5ffce7f4f38
                                              f7a89addce9ee983e1ef7fd9d471d6c2634c3e6f
                                              d3b499161b769123c137e8c675b7cf724c72760e
                                              e6ad4c05cc6b75b1cf7ebd2a3799a6cda9a49360
                                              b299659ec7e946bad2623b2af99693d7368e5f7b
                                              19bd818b214414030c66a6fae127b9e815151859
                                              fed9fbdd445a208c4bfa36ac9798b96191ca7dd9
                                              06e4772e3cd0b707745b9dd25cfb94eb832051b7
                                              7b57e974cc753d50929b9ccf0905efcb9dafe8a9
                                              7c67880c58a1d45f63b05e85b5473a89ae908e01
                                              55a2d9983b67cec380e53742fe5ca9feeaa75d28)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 1f15d2087aa11bf6796ac0fcbfe9ed48b3612a2f
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES afc2b203c3660c78a3d9d428276c535969dc8cde
                                          ed579b3441c05e555d39e2c348084b67b594baa7
                                          a838efdc350cf1123e775dbbecb684028429d3cc
                                          d97a03ff8b0776a04135ad0be7fcf535049241f0
                                          6629fe010a35285ca9f47030dacfc2cdea504198
                                          cef4ff4cd8ff2cd110570e1adde3f41871c03d72
                                          bf801c792e26404cee613729713bc20d248fe55e
                                          20836ed707abff1be0d0b6e87d08f228335f787a
                                          7a025df1de8fad516378c5b516e81da71b008627
                                          682bdd488b2c7126a0e2e146344c4bb783b04138
                                          1e724feff8553e99cad27dcce0e502eddebff922
                                          f40341f4a6174384eeb34f15799cbf6375357c3b
                                          dac6ce77edbea9d426d85451fd905eec11519759
                                          b2aab52c7a75dcb49cad8c2aae82b8ca9ad6800f
                                          a7080ef1f6450f0a3521ead2d0adfadc5720b430)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 2a810ef12dcb0ff843250aee6be5cd092644c978
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 07a602cf9982623ac89795ad8ee6b900d561bcc7
                                          d647bcb68ba644574a620de0c28a09d4f43f9434
                                          7eecaa8d9c5c244f8a4b1230a805ea28d2183130
                                          bd8932c0a184c0de87338a78f1a44114beeadfcd
                                          42a41a2869a1a3d975fb00c189a270f666c58b41
                                          368abefe9433750defea6dfae81e93d314ce3c6f
                                          7bad809272dd4ffae5ba450bddf88311c3bc3f65
                                          ce1c6ac75d1aa06506c5e884ffc9b20e2fd5328e
                                          8cfcff62241b8bae74623f3c54bad1d4fdf68973
                                          f963887a46f2d523c47003e201f0dfa6936ee9a8
                                          1b1c4e064a9f633fe6881d3f49eac81d6fb15547
                                          7b688d11bda1ad91270e419c4e04fa910ac35e49
                                          88efe51b8c9aa5e23ba3c6ab2942293c36877ccc
                                          351c5a8acac9db8136bbdec2a456e5b1e11e3a36
                                          54c5434e24c99d0948796552e5c51524498129b8)
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
