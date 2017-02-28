# /*OpenCMISS-Zinc Library
# *
# * This Source Code Form is subject to the terms of the Mozilla Public
# * License, v. 2.0. If a copy of the MPL was not distributed with this
# * file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#ifndef ZINC_SHARED_OBJECT_H
#define ZINC_SHARED_OBJECT_H

#define ZINC_C_INLINE static inline

// Handle the exporting of symbols from a dynamic shared object
// across different oses.

#if defined _WIN32 || defined __CYGWIN__
  #define ZINC_DSO_IMPORT __declspec(dllimport)
  #define ZINC_DSO_EXPORT __declspec(dllexport)
#else
  #if __GNUC__ >= 4
    #define ZINC_DSO_IMPORT __attribute__ ((visibility ("default")))
    #define ZINC_DSO_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define ZINC_DSO_IMPORT
    #define ZINC_DSO_EXPORT
  #endif
#endif

// Now we use the generic helper definitions above to define ZINC_API.
// ZINC_API is used for the public API symbols. It either DSO imports or DSO exports (or does nothing for static build)

#ifdef ZINC_SHARED_OBJECT // defined if Zinc is compiled as a shared object
  #ifdef ZINC_EXPORTS // defined if we are building the Zinc DSO (instead of using it)
    #define ZINC_API ZINC_DSO_EXPORT
  #else
    #define ZINC_API ZINC_DSO_IMPORT
  #endif // ZINC_EXPORTS
#else // ZINC_SHARED_OBJECT is not defined: this means Zinc is a static lib.
  #define ZINC_API
#endif // ZINC_SHARED_OBJECT

#endif

