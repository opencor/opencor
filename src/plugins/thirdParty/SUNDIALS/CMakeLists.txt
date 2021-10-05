project(SUNDIALSPlugin)

# Name and version of our package

set(PACKAGE_NAME SUNDIALS)
set(PACKAGE_VERSION 5.8.0)

# Version of our package's libraries

set(MAJOR_ARKODE_LIBRARY_VERSION 4)
set(ARKODE_LIBRARY_VERSION ${MAJOR_ARKODE_LIBRARY_VERSION}.8.0)

set(MAJOR_CVODES_LIBRARY_VERSION 5)
set(CVODES_LIBRARY_VERSION ${MAJOR_CVODES_LIBRARY_VERSION}.8.0)

set(MAJOR_IDAS_LIBRARY_VERSION 4)
set(IDAS_LIBRARY_VERSION ${MAJOR_IDAS_LIBRARY_VERSION}.8.0)

set(MAJOR_KINSOL_LIBRARY_VERSION 5)
set(KINSOL_LIBRARY_VERSION ${MAJOR_KINSOL_LIBRARY_VERSION}.8.0)

set(MAJOR_NVEC_LIBRARY_VERSION 5)
set(NVEC_LIBRARY_VERSION ${MAJOR_NVEC_LIBRARY_VERSION}.8.0)

set(MAJOR_SUNLINSOL_LIBRARY_VERSION 3)
set(SUNLINSOL_LIBRARY_VERSION ${MAJOR_SUNLINSOL_LIBRARY_VERSION}.8.0)

set(MAJOR_SUNNONLINSOL_LIBRARY_VERSION 2)
set(SUNNONLINSOL_LIBRARY_VERSION ${MAJOR_SUNNONLINSOL_LIBRARY_VERSION}.8.0)

set(MAJOR_SUNMATRIX_LIBRARY_VERSION 3)
set(SUNMATRIX_LIBRARY_VERSION ${MAJOR_SUNMATRIX_LIBRARY_VERSION}.8.0)

# Git tag for our library

set(GIT_TAG v5.8.0)

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

    set(SUNMATRIXBAND_SHARED_LIBRARY sundials_sunmatrixband${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNMATRIXBAND_IMPORT_LIBRARY sundials_sunmatrixband${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNMATRIXDENSE_SHARED_LIBRARY sundials_sunmatrixdense${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNMATRIXDENSE_IMPORT_LIBRARY sundials_sunmatrixdense${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNMATRIXSPARSE_SHARED_LIBRARY sundials_sunmatrixsparse${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNMATRIXSPARSE_IMPORT_LIBRARY sundials_sunmatrixsparse${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY sundials_sunnonlinsolfixedpoint${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNNONLINSOLFIXEDPOINT_IMPORT_LIBRARY sundials_sunnonlinsolfixedpoint${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SUNNONLINSOLNEWTON_SHARED_LIBRARY sundials_sunnonlinsolnewton${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SUNNONLINSOLNEWTON_IMPORT_LIBRARY sundials_sunnonlinsolnewton${CMAKE_IMPORT_LIBRARY_SUFFIX})
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

    set(SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunnonlinsolfixedpoint${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNNONLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNNONLINSOLFIXEDPOINT_IMPORT_LIBRARY ${SUNNONLINSOLFIXEDPOINT_SHARED_LIBRARY})

    set(SUNNONLINSOLNEWTON_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sundials_sunnonlinsolnewton${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_SUNNONLINSOL_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SUNNONLINSOLNEWTON_IMPORT_LIBRARY ${SUNNONLINSOLNEWTON_SHARED_LIBRARY})
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
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 56109dfb801553df44413347e08b3b5b9cd276ea
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES d63a10cdd70ad44bb2b1eb4d1db700a1e254f4d5
                                              4fd664acca7958e7e966c8e03f0f34ef18b2107c
                                              0f04f6ad36611f1597b3e41741f5bb4485c4f947
                                              71ad3d9bcc042b75349694661eacadf56a1074af
                                              28ad52829dd87309687aaf293a2fb1597e81bee8
                                              1550335d9061c105e792267bade60805afe31bed
                                              da6ec83feb03242faef8a039fbe349ceb44ae67e
                                              8033f668d2b3d5e0fb7a82de09a1fefd127536f1
                                              49030d25d2921e97ca947d4488bfb36d30afcc39
                                              fee716a832cfa9d06b508009284b71150a221bbc
                                              1076692b4a9b84250cb03eb7aed0fd49ae5d5985
                                              76d08641468b160a4836b2daa13953f05ee46b41
                                              e5957495f6f37373fadc70e138f1e308a07dd670
                                              a32982fffc853ec3fa237c13e4ec8b8d2b44e7b4
                                              913ed1a5322e2c4c27c1949cc8f400c9ebad94d2
                                              eeb4a2798691e0f5474b7f991bffd7eab69eb9d1
                                              6468437618b758329dd0f7165170150d2e7727c9
                                              fb2ed033925d2aee8833cd606e655a051d5610b2
                                              fc4a27d6a0f00bf30bd410c5e3618fec68266938
                                              b44231ea57f94753e2baa2407d82cc5708492320
                                              700633db87b50e3b7603f22dfb47931792e0fb74
                                              252df683b6be69c83d91fdad85272db778424478
                                              f71c2e784a11d36daca35dde3431a161f702fe2d
                                              ad904ef6271bc00ed1850c6b3c2b62ff6c96d359
                                              1eef16f64c6ac691f4cc571cfca2d25610d6e9eb
                                              13bc26475fade5a1de0916c41a78b034c348e2e0
                                              be5dd64a0dc5ae15e387407ee267f1f61ba9a399
                                              905143120c3609c8db205a4b824de06ab3b8db9d
                                              eb5a06a856b37905aeec31a7e1e42cfd5b29730e
                                              964d360aed982af135237c0875820c43c6f35fac
                                              a90984df7b65fc64f5b10b9d38ee90c80847cccb
                                              dedc259c15ee1b08180766e2a0db68c7822246dd
                                              5d25f2dc9cffd4844fb43355e797f7fe2719456a
                                              d575aa465ea8dd10d39b6a72b0a4dea2e3fab888)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} f3c55d075550905c441f6c1a3215b5e370a53bc7
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES a448241479eb9a12814e899eb1d2c881eca72470
                                              2eafca000a73349faece52695cce71e191b2df27
                                              6407f77557bf982b9f9052a0b433539cc051ae5c
                                              75339a3a40efe393200d430924cb7bd28117a3e2
                                              12bbb012c1262732797a29afd4aeb45fb7e4e9d7
                                              859d46697d4ad154baa47bc8cf78a9f2fa407b07
                                              780ec262c26810b7da13ae490481388fbd99a6d2
                                              bd64acb3d25e1fa1a7613b75faa3760b482ef6c8
                                              184f24a133a81fdcf3253830c8cefa95e4786911
                                              9ef3f14993eb840965ad44817c185988f0cbe15e
                                              ed8ae1e8c7b9997816db1c3b4b58613f4fa81f5d
                                              04b24a8dca9b28e2554958e4ac4adc247fd16b36
                                              d08e3a5864b30bf6b9029d07dea23e7231b835b9
                                              9a4652fa338b20d88b727766dc716b615ff0a478
                                              b1de890192c313c4f0b64c30de3fa2377529d7a0
                                              f67cd45f9fd655d438370c94301c79f6b19654b0
                                              bf5e5c01138258dab2094443ae5d29f9ca9abfdf
                                              fb2ed033925d2aee8833cd606e655a051d5610b2
                                              fc4a27d6a0f00bf30bd410c5e3618fec68266938
                                              b44231ea57f94753e2baa2407d82cc5708492320
                                              700633db87b50e3b7603f22dfb47931792e0fb74
                                              252df683b6be69c83d91fdad85272db778424478
                                              f71c2e784a11d36daca35dde3431a161f702fe2d
                                              ad904ef6271bc00ed1850c6b3c2b62ff6c96d359
                                              1eef16f64c6ac691f4cc571cfca2d25610d6e9eb
                                              13bc26475fade5a1de0916c41a78b034c348e2e0
                                              be5dd64a0dc5ae15e387407ee267f1f61ba9a399
                                              905143120c3609c8db205a4b824de06ab3b8db9d
                                              eb5a06a856b37905aeec31a7e1e42cfd5b29730e
                                              964d360aed982af135237c0875820c43c6f35fac
                                              a90984df7b65fc64f5b10b9d38ee90c80847cccb
                                              dedc259c15ee1b08180766e2a0db68c7822246dd
                                              5d25f2dc9cffd4844fb43355e797f7fe2719456a
                                              d575aa465ea8dd10d39b6a72b0a4dea2e3fab888)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 297f4fd4af897559e786422deafbe1944ea4e374
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2178d895d5bdadfbff3c9947d9f85e1f7dfcc616
                                          4449f53334968ef199ea629691dd32d160ff2e9a
                                          e83732baa26c4b84403fc9f38afaf396aa5c3a66
                                          a407de69f57da0e08d944023ed1079f091d01870
                                          0af8caf603073690e4b26902da2b3804de1dc5fa
                                          b4264b78bd32c86d1a069eefc99f35ca2857abd3
                                          1137e9274ca277fc66c46c4fa2a5cda4a50ec1f4
                                          ef5cf1f865823bed1f217ab2438261f9591896e6
                                          d4a9f2e437a2ebd94fb92e810a7df30dd5562f8a
                                          c1b511f59480a537ee3ad151428fcdbf6c51ae7f
                                          9a3a515598c2d60919b5a888eb3523ac58e84dca
                                          597d8b3fb7437c98ecf7adafcdf0e0e29d1ad084
                                          d6793ffac75b948c23c8fd0d736da844817150d0
                                          0550770cfa950a4f4459c53251863525dc2be29f
                                          b4e2d9b465e0e4eb1a78130ef6994a55c8bb0c37
                                          26c00ffdf90eff14972b5ac434180749a21252a8
                                          c230fafff688c64d20d03b1ed8eae46c2b456f34)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} bb0f6aa9c3ebaac3fe92ccdce3db725109c327de
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 8e942c3da389e48591514fb3b6f6939c3b79e4b8
                                          c38f4e569b033525f6cc45534f5cd314f927287b
                                          7e1edf21a29242fa5f88590025193e2dda1f41b2
                                          9d1a74c96fb64b1d6c16b9a7b36660ed25526378
                                          d3e09c9f4dc6c8f46107e7fc15755e736af3cc50
                                          2fe0c6922eac43fa38d47ad7aa40349aba807349
                                          4accc35a62c10334b4f578f14dd0a2a704ef4728
                                          35780cbc0b701203903cb051b2ed9abb79412d42
                                          779967d9ce9f0f95f0c9479bf6d671a5cf21f7c2
                                          c5120af13314ed7497385a0110d8b01902193b84
                                          4af1d1c5b47a1389cbb862b8b5ab2ed5ceb8b534
                                          26f1a93d7a273fb6968b1aa2984a7602f1a8c394
                                          e9ebc5ebf4cbd6be8845466cc23989f1c0d4dbda
                                          67ad34a88ae1e7f9a6158e51a38b982960f020b8
                                          296953cb626ea6bb292fdc756bef3af958339d9c
                                          7a9d902a3b2da78485fd5fe5279839adaeded7d3
                                          a8417338489c8abdc50edf19088443bc9063f9b8)
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
