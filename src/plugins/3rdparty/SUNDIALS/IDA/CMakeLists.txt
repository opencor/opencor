PROJECT(IDAPlugin)

# Add the plugin

ADD_PLUGIN(IDA
    SOURCES
        ../../../plugininfo.cpp

        src/idaplugin.cpp

        ../src/ida/ida.c
        ../src/ida/ida_band.c
        ../src/ida/ida_bbdpre.c
        ../src/ida/ida_dense.c
        ../src/ida/ida_direct.c
        ../src/ida/ida_ic.c
        ../src/ida/ida_io.c
        ../src/ida/ida_spbcgs.c
        ../src/ida/ida_spgmr.c
        ../src/ida/ida_spils.c
        ../src/ida/ida_sptfqmr.c

        ../src/nvec_ser/nvector_serial.c

        ../src/sundials/sundials_band.c
        ../src/sundials/sundials_dense.c
        ../src/sundials/sundials_direct.c
        ../src/sundials/sundials_iterative.c
        ../src/sundials/sundials_math.c
        ../src/sundials/sundials_nvector.c
        ../src/sundials/sundials_spbcgs.c
        ../src/sundials/sundials_spgmr.c
        ../src/sundials/sundials_sptfqmr.c
    HEADERS_MOC
        src/idaplugin.h
    DEFINITIONS
        BUILD_SUNDIALS_LIBRARY
    INCLUDE_DIRS
        ../include
        ../src
    QT_DEPENDENCIES
        QtCore
)
