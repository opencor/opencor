#include "cda_compiler_support.h"
#ifdef IN_TeLICeMS_MODULE
#define TeLICeMS_PUBLIC_PRE CDA_EXPORT_PRE
#define TeLICeMS_PUBLIC_POST CDA_EXPORT_POST
#else
#define TeLICeMS_PUBLIC_PRE CDA_IMPORT_PRE
#define TeLICeMS_PUBLIC_POST CDA_IMPORT_POST
#endif
#include "IfaceTeLICeMS.hxx"

/**
 * Creates a new code generator bootstrap.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
TeLICeMS_PUBLIC_PRE
  already_AddRefd<iface::cellml_services::TeLICeMService>
  CreateTeLICeMService(void)
TeLICeMS_PUBLIC_POST;
