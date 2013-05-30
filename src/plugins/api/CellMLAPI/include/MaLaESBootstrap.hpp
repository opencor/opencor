#include "cda_compiler_support.h"
#ifdef IN_MALAES_MODULE
#define MALAES_PUBLIC_PRE CDA_EXPORT_PRE
#define MALAES_PUBLIC_POST CDA_EXPORT_POST
#else
#define MALAES_PUBLIC_PRE CDA_IMPORT_PRE
#define MALAES_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new C generator.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
MALAES_PUBLIC_PRE
already_AddRefd<iface::cellml_services::MaLaESBootstrap> CreateMaLaESBootstrap(void)
MALAES_PUBLIC_POST;
