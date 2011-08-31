#include "cda_compiler_support.h"
#ifdef IN_CELEDSEXPORTER_MODULE
#define CELEDSEXPORT_PUBLIC_PRE CDA_EXPORT_PRE
#define CELEDSEXPORT_PUBLIC_POST CDA_EXPORT_POST
#else
#define CELEDSEXPORT_PUBLIC_PRE CDA_IMPORT_PRE
#define CELEDSEXPORT_PUBLIC_POST CDA_IMPORT_POST
#endif

/**
 * Creates a new CeLEDSExporter bootstrap
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
CELEDSEXPORT_PUBLIC_PRE
  iface::cellml_services::CeLEDSExporterBootstrap* 
  CreateCeLEDSExporterBootstrap(void)
CELEDSEXPORT_PUBLIC_POST;
