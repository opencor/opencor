#include "cda_compiler_support.h"
#ifdef IN_CCGS_MODULE
#define CCGS_PUBLIC_PRE CDA_EXPORT_PRE
#define CCGS_PUBLIC_POST CDA_EXPORT_POST
#else
#define CCGS_PUBLIC_PRE CDA_IMPORT_PRE
#define CCGS_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new code generator bootstrap.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
CCGS_PUBLIC_PRE
  already_AddRefd<iface::cellml_services::CodeGeneratorBootstrap>
  CreateCodeGeneratorBootstrap(void)
CCGS_PUBLIC_POST;
