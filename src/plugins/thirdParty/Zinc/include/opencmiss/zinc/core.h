/**
 * @file core.h
 *
 * The public interface to Zinc core memory handling functions.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_CORE_H__
#define CMZN_CORE_H__

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate memory using the Zinc internal heap.
 * Deprecated.
 *
 * @param bytes  The requested memory block size in bytes.
 * @return  Pointer to the memory block as a void *, or NULL if failed.
 */
ZINC_API void *cmzn_allocate(int bytes);

/**
 * Frees the memory allocated by the Zinc internal heap at the given pointer.
 * Used to free buffers allocated by Zinc APIs, usually string get_name functions.
 * Note that ptr is not cleared by this function.
 *
 * @param ptr  The pointer to the allocated memory block.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_GENERAL.
 */
ZINC_API int cmzn_deallocate(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* CMZN_CORE_H__ */
