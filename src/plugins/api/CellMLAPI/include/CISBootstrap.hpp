#include "cda_compiler_support.h"
#include "IfaceCIS.hxx"
#ifdef IN_CIS_MODULE
#define CIS_PUBLIC_PRE CDA_EXPORT_PRE
#define CIS_PUBLIC_POST CDA_EXPORT_POST
#else
#define CIS_PUBLIC_PRE CDA_IMPORT_PRE
#define CIS_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new CIS integrator.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
CIS_PUBLIC_PRE already_AddRefd<iface::cellml_services::CellMLIntegrationService>
CreateIntegrationService(void) CIS_PUBLIC_POST;
