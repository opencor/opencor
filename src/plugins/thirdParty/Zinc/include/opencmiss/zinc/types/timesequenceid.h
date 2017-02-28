/**
 * @file timesequenceid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_TIMESEQUENCEID_H__
#define CMZN_TIMESEQUENCEID_H__

/**
 * @brief A non-decreasing list of times at which nodal parameters can be stored.
 *
 * A non-decreasing list of times at which nodal parameters can be stored.
 * Setting a timesequence for a field in a nodetemplate establishes storage
 * for N values of each component x type x version at the node (with N = the
 * number of times in the timesequence), indexed by looking up time in the
 * timesequence and linearly interpolating between stored times.
 * Note that if time-varying parameters are required, a time sequence must be
 * set when the node field is first defined: it is not currently possible to add
 * time variation to a node field that is not time-varying (but it is possible
 * to merge or add times to an existing time-varying node field).
 *
 * @see cmzn_fieldmodule_get_matching_timesequence
 * @see cmzn_nodetemplate_set_timesequence
 */
struct cmzn_timesequence;
typedef struct cmzn_timesequence *cmzn_timesequence_id;

#endif /* CMZN_TIME_SEQUENCE_ID_H */
