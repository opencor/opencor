#include "cda_compiler_support.h"
#ifdef IN_ANNOTOOLS_MODULE
#define AT_PUBLIC_PRE CDA_EXPORT_PRE
#define AT_PUBLIC_POST CDA_EXPORT_POST
#else
#define AT_PUBLIC_PRE CDA_IMPORT_PRE
#define AT_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new annotation tools service.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
AT_PUBLIC_PRE
  already_AddRefd<iface::cellml_services::AnnotationToolService> CreateAnnotationToolService(void)
AT_PUBLIC_POST;
