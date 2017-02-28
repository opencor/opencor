/**
 * @file fieldfibres.h
 *
 * A field which extract a transformation matrix from a scene viewer for use in
 * field expressions.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDFIBRES_H__
#define CMZN_FIELDFIBRES_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a "fibre axes" field type which returns a 9-component (3 x 3 vector)
 * field representing an orthonormal coordinate system which is rotated by
 * 3 Euler angles supplied by a fibre field. Three resulting 3 axes are:
 * fibre  = fibre direction
 * sheet  = fibre normal in the plane of the sheet
 * normal = normal to the fibre sheet
 * Both the fibre and coordinate fields must have no more than 3 components. The
 * fibre field is expected to have a FIBRE coordinate_system, although this is
 * not enforced.
 * Note that this initial orientation of the coordinate system (i.e. for all
 * Euler angles zero) is right handed coordinate system:
 * fibre axis = aligned with d(coordinates)/dxi1
 * sheet axis = in plane of fibre axis and d(coordinates)/dxi2 but corrected to
 * be closest vector to d(coordinates)/dxi2 which is normal to fibre axes.
 * normal axis = cross product fibre (x) sheet
 * How the components of the fibre angles rotate the above system is described
 * with the FIBRE coordinate system.
 * @see cmzn_field_coordinate_system_type
 *
 * @param field_module  Region field module which will own new field.
 * @param fibre_field  The (numerical) fibre field with at most 3 components
 * @param coordinate_field  The (numerical) coordinate system with at most 3 components
 * @return  Handle to new 9-component field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_fibre_axes(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id fibre_field, cmzn_field_id coordinate_field);

#ifdef __cplusplus
}
#endif

#endif
