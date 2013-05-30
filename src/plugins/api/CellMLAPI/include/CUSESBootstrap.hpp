#include "cda_compiler_support.h"
#ifdef IN_CUSES_MODULE
#define CUSES_PUBLIC_PRE CDA_EXPORT_PRE
#define CUSES_PUBLIC_POST CDA_EXPORT_POST
#else
#define CUSES_PUBLIC_PRE CDA_IMPORT_PRE
#define CUSES_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new annotation tools service.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
CUSES_PUBLIC_PRE
  iface::cellml_services::CUSESBootstrap* CreateCUSESBootstrap(void)
CUSES_PUBLIC_POST;
