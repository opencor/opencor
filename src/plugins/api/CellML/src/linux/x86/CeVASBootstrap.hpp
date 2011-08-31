#include "cda_compiler_support.h"
#ifdef IN_CEVAS_MODULE
#define CEVAS_PUBLIC_PRE CDA_EXPORT_PRE
#define CEVAS_PUBLIC_POST CDA_EXPORT_POST
#else
#define CEVAS_PUBLIC_PRE CDA_IMPORT_PRE
#define CEVAS_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new C generator.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
CEVAS_PUBLIC_PRE
iface::cellml_services::CeVASBootstrap* CreateCeVASBootstrap(void)
CEVAS_PUBLIC_POST;
