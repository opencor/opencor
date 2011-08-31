#include "cda_compiler_support.h"
#ifndef IN_SRuS_MODULE
#define SRuS_PUBLIC_PRE CDA_EXPORT_PRE
#define SRuS_PUBLIC_POST CDA_EXPORT_POST
#else
#define SRuS_PUBLIC_PRE CDA_IMPORT_PRE
#define SRuS_PUBLIC_POST CDA_IMPORT_POST
#endif
#include "IfaceSRuS.hxx"

/**
 * Creates a new SED-ML Running Service Bootstrap.
 * This code is the external C++ interface which allows you to fetch
 * the implementation.
 */
SRuS_PUBLIC_PRE
  iface::SRuS::Bootstrap*
CreateSRuSBootstrap(void) throw()
SRuS_PUBLIC_POST;
