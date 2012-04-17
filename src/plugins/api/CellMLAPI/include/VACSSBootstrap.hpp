#include "cda_compiler_support.h"
#ifdef IN_VACSS_MODULE
#define VACSS_PUBLIC_PRE CDA_EXPORT_PRE
#define VACSS_PUBLIC_POST CDA_EXPORT_POST
#else
#define VACSS_PUBLIC_PRE CDA_IMPORT_PRE
#define VACSS_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new Validation Against CellML Specification Service.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
VACSS_PUBLIC_PRE
already_AddRefd<iface::cellml_services::VACSService> CreateVACSService(void)
VACSS_PUBLIC_POST;
