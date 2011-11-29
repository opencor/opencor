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
    HEADERS
        ../../plugininfo.h

        include/cvode/cvode.h
        include/cvode/cvode_band.h
        include/cvode/cvode_bandpre.h
        include/cvode/cvode_bbdpre.h
        include/cvode/cvode_dense.h
        include/cvode/cvode_diag.h
        include/cvode/cvode_direct.h
        include/cvode/cvode_spbcgs.h
        include/cvode/cvode_spgmr.h
        include/cvode/cvode_spils.h
        include/cvode/cvode_sptfqmr.h

        include/nvector/nvector_serial.h

        include/sundials/sundials_band.h
        include/sundials/sundials_config.h
        include/sundials/sundials_dense.h
        include/sundials/sundials_direct.h
        include/sundials/sundials_iterative.h
        include/sundials/sundials_math.h
        include/sundials/sundials_nvector.h
        include/sundials/sundials_spbcgs.h
        include/sundials/sundials_spgmr.h
        include/sundials/sundials_sptfqmr.h
        include/sundials/sundials_types.h

        src/cvode/cvode_bandpre_impl.h
        src/cvode/cvode_bbdpre_impl.h
        src/cvode/cvode_diag_impl.h
        src/cvode/cvode_direct_impl.h
        src/cvode/cvode_impl.h
        src/cvode/cvode_spils_impl.h
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
