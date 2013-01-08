PROJECT(SUNDIALSPlugin)

# Add the plugin

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
    QT_DEPENDENCIES
        QtCore
)
