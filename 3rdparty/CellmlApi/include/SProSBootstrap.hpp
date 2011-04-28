#include "cda_compiler_support.h"
#ifdef IN_SProS_MODULE
#define SProS_PUBLIC_PRE CDA_EXPORT_PRE
#define SProS_PUBLIC_POST CDA_EXPORT_POST
#else
#define SProS_PUBLIC_PRE CDA_IMPORT_PRE
#define SProS_PUBLIC_POST CDA_IMPORT_POST
#endif
#include "Utilities.hxx"
#include "IfaceSProS.hxx"

/**
 * Creates a new SED-ML processing service bootstrap.
 * This code is the external C++ interface which allows you to fetch the
 * implementation.
 */
SProS_PUBLIC_PRE
  iface::SProS::Bootstrap*
  CreateSProSBootstrap(void)
SProS_PUBLIC_POST;
