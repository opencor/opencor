#include "cda_compiler_support.h"
#ifdef IN_CELEDS_MODULE
#define CELEDS_PUBLIC_PRE CDA_EXPORT_PRE
#define CELEDS_PUBLIC_POST CDA_EXPORT_POST
#else
#define CELEDS_PUBLIC_PRE CDA_IMPORT_PRE
#define CELEDS_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new CeLEDS bootstrap
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
CELEDS_PUBLIC_PRE
  iface::cellml_services::CeLEDSBootstrap* 
  CreateCeLEDSBootstrap(void)
CELEDS_PUBLIC_POST;
