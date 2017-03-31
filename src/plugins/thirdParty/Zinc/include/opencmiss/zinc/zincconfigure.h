
# /*OpenCMISS-Zinc Library
# *
# * This Source Code Form is subject to the terms of the Mozilla Public
# * License, v. 2.0. If a copy of the MPL was not distributed with this
# * file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#ifndef CMISS_ZINC_CONFIGURE_H
#define CMISS_ZINC_CONFIGURE_H

// Platform specific defines
#define UNIX
#define DARWIN
/* #undef CYGWIN */
/* #undef WIN32_SYSTEM */

// Operating specific defines
/* #undef SGI */
/* #undef GENERIC_PC */

// Graphics specific defines
#define OPENGL_API
/* #undef DM_BUFFERS */
/* #undef REPORT_GL_ERRORS */
/* #undef USE_PARAMETER_ON */

// Extension specific defines
/* #undef USE_OPENCASCADE */
#define ZINC_USE_IMAGEMAGICK
#define ZINC_USE_ITK
#define ZINC_USE_NETGEN
#define USE_GLEW
/* #undef ZINC_USE_PNG */

// Miscellaneous defines
#define HAVE_VFSCANF
/* #undef GLEW_STATIC */
#define USE_MSAA
#define OPTIMISED
/* #undef MEMORY_CHECKING */
/* #undef ZINC_NO_STDOUT */
#define HAVE_HEAPSORT

typedef double FE_value;
#define FE_VALUE_INPUT_STRING "%lf"
#define FE_VALUE_STRING "22.15le"
typedef double ZnReal;
typedef double COLOUR_PRECISION;
typedef double MATERIAL_PRECISION;

#endif

