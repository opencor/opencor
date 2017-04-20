/**
 * @file fieldmeshoperatorsid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDMESHOPERATORSID_H__
#define CMZN_FIELDMESHOPERATORSID_H__

/**
 * @brief  A field calculating the integral over a mesh.
 *
 * A field calculating the integral over a mesh of integrand.dV for volume
 * in 3-D, alternatively dA for area in 2-D, or dL for length in 1-D.
 * An integrand of constant 1.0 combined with the RC coordinate field gives
 * the volume, area or length, depending on the mesh dimension.
 * Note: uses absolute value of dV/dA/dL so works with right- or left- handed
 * element local coordinate systems. However, cannot handle elements that are
 * partly or completely inverted from expected handedness.
 */
struct cmzn_field_mesh_integral;
typedef struct cmzn_field_mesh_integral *cmzn_field_mesh_integral_id;

#endif
