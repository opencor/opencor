PROJECT(CVODEPlugin)

# Add the plugin

ADD_PLUGIN(CVODE
    SOURCES
        ../../plugininfo.cpp

        src/cvodeplugin.cpp

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
        src/cvodeplugin.h
    DEFINITIONS
        BUILD_SUNDIALS_LIBRARY
    INCLUDE_DIRS
        include
        src
    QT_DEPENDENCIES
        QtCore
)
