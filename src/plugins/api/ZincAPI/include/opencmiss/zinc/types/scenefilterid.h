/**
 * @file scenefilterid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SCENEFILTERID_H__
#define CMZN_SCENEFILTERID_H__

/**
 * @brief Module managing all scene filters.
 *
 * Module managing all scene filters.
 */
struct cmzn_scenefiltermodule;
typedef struct cmzn_scenefiltermodule *cmzn_scenefiltermodule_id;

/**
 * @brief Scene filters determines which graphics are drawn.
 *
 * Scene filters are Boolean functions determining which graphics are drawn on a
 * sceneviewer, or processed by a scenepicker or other tool.
 * Derived types implement operators including filtering by scene and graphics
 * visibility flags, region/scene, graphics name, graphics type, field domain
 * type, and logical and/or operators allowing expressions combining filters.
 */
struct cmzn_scenefilter;
typedef struct cmzn_scenefilter *cmzn_scenefilter_id;

/**
 * @brief An operator OR or AND specific scene filter type.
 *
 * An operator OR or AND specific scene filter type. Note that these are n-ary
 * operators, able to take any number of operands, and that each operand can be
 * individually set active or inactive, permitting them to be used as high level
 * visibility lists.
 */
struct cmzn_scenefilter_operator;
typedef struct cmzn_scenefilter_operator *cmzn_scenefilter_operator_id;

#endif
