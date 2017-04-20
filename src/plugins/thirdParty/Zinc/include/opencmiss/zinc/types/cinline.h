/**
 * @file cinline.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_CINLINE_H__
#if defined (_MSC_VER)
	#define CMZN_C_INLINE __inline
#else
	#define CMZN_C_INLINE static inline
#endif
#define CMZN_CINLINE_H__
#endif
