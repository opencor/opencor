/**
 * @file streamscene.h
 *
 * Public interface to scene stream objects.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_STREAMSCENE_H__
#define CMZN_STREAMSCENE_H__

#include "types/sceneid.h"
#include "types/scenefilterid.h"
#include "types/streamid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert a short name into an enum if the name matches any of the members in
 * the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum format if a match is found.
 */
ZINC_API enum cmzn_streaminformation_scene_io_format
	cmzn_streaminformation_scene_io_format_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum format from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param format  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_streaminformation_scene_io_format_enum_to_string(
	enum cmzn_streaminformation_scene_io_format format);

/**
 * Creates a stream information object for specifying files/resources and options
 * for writing scene data from a scene.
 *
 * @return  Handle to new stream information, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_streaminformation_id cmzn_scene_create_streaminformation_scene(
	cmzn_scene_id scene);

/**
 * Destroys handle to stream information scene.
 *
 * @param streaminformation_address  Address of handle to streaminformation
 * scene. Handle will be reset to NULL.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_destroy(
	cmzn_streaminformation_scene_id *streaminformation_address);

/**
 * If the streaminformation is of scene type, then this function returns
 * the derived scene stream information handle.
 *
 * @param streaminformation  Handle to streaminformation to cast.
 * @return  Handle to derived stream information scene, or NULL/invalid handle if
 * wrong type or failed.
 */
ZINC_API cmzn_streaminformation_scene_id cmzn_streaminformation_cast_scene(
	cmzn_streaminformation_id streaminformation);

/**
 * Cast cmzn_streaminformation_scene back to its base streaminformation and
 * return the streaminformation.
 * IMPORTANT NOTE: Returned streaminformation does not have incremented
 * reference count and must not be destroyed. Use cmzn_streaminformation_access()
 * to add a reference if maintaining returned handle beyond the lifetime of the
 * streaminformation_scene argument.
 *
 * @param streaminformation_scene  Handle to the streaminformation_scene to cast.
 * @return  Non-accessed handle to the base stream information or NULL if failed.
 */
ZINC_C_INLINE cmzn_streaminformation_id cmzn_streaminformation_scene_base_cast(
	cmzn_streaminformation_scene_id streaminformation_scene)
{
	return (cmzn_streaminformation_id)(streaminformation_scene);
}

/**
 * Return the scenefilter that is set for this streaminformation
 *
 * @param streaminformation  The streaminformation_scene to get the scenefilter from.
 * @return  Handle to scene filter, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_scenefilter_id cmzn_streaminformation_scene_get_scenefilter(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set the scenefilter for the streaminformation_scene. This filter determines
 * which graphics are included in the export.
 *
 * @param streaminformation  The streaminformation_scene to be modified.
 * @param filter  The scene filter to use, or NULL if none (all enable).
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_scenefilter(
	cmzn_streaminformation_scene_id streaminformation,
	cmzn_scenefilter_id scenefilter);

/**
 * Get the currently set export format of streaminformation_scene.
 *
 * @param streaminformation  The streaminformation_scene to query.
 * @return cmzn_streaminformation_scene_io_format on success,
 * CMZN_STREAMINFORMATION_SCENE_IO_FORMAT_INVALID on failure.
 */
ZINC_API enum cmzn_streaminformation_scene_io_format
	cmzn_streaminformation_scene_get_io_format(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Specifies the format for scene data to be output using this
 * streaminformation.
 *
 * @param streaminformation  The storage information object.
 * @param format  The format to export to.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_io_format(
	cmzn_streaminformation_scene_id streaminformation,
	enum cmzn_streaminformation_scene_io_format format);

/**
 * Get the currently set export data type of streaminformation_scene.
 *
 * @param streaminformation  The streaminformation_scene to query.
 * @return cmzn_streaminformation_scene_io_data_type on success,
 * CMZN_STREAMINFORMATION_SCENE_IO_DATA_TYPE_INVALID on failure.
 */
ZINC_API enum cmzn_streaminformation_scene_io_data_type
	cmzn_streaminformation_scene_get_io_data_type(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Specifies the data type for scene data to be output using this
 * streaminformation.
 *
 * @param streaminformation  The storage information object.
 * @param format  The data type to export.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_io_data_type(
	cmzn_streaminformation_scene_id streaminformation,
	enum cmzn_streaminformation_scene_io_data_type data_type);

/**
 * Get the number of time steps to export.
 *
 * @param streaminformation  The streaminformation_scene to query.
 * @return positive integer number on success, 0 on failure.
 */
ZINC_API int cmzn_streaminformation_scene_get_number_of_time_steps(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set the number of time steps to export. This value along with finish
 * time and initial time will determine at which time steps the graphics
 * will be export. If initial time = 0.0, finish time = 1.0 and
 * number of time steps = 11; graphics will be export at 0.0, 0.1, 0.2,
 * 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 and 1.0.
 * @see cmzn_streaminformation_scene_set_finish_time
 * @see cmzn_streaminformation_scene_set_initial_time
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @param numberOfTimeSteps  number of time steps to be set for export.
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_number_of_time_steps(
	cmzn_streaminformation_scene_id streaminformation,	int numberOfTimeSteps);

/**
 * Get the last time step to export.
 *
 * @param streaminformation  The streaminformation_scene to query.
 * @return positive integer number on success, 0 on failure.
 */
ZINC_API double cmzn_streaminformation_scene_get_finish_time(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set the last time step to export.
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @param finishTime  last time step to be set for export.
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_finish_time(
	cmzn_streaminformation_scene_id streaminformation, double finishTime);

/**
 * Get the first time step to export.
 *
 * @param streaminformation  The streaminformation_scene to query.
 * @return positive integer number on success, 0 on failure.
 */
ZINC_API double cmzn_streaminformation_scene_get_initial_time(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set the first time step to export.
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @param initialTime  first time step to be set for export.
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_initial_time(
	cmzn_streaminformation_scene_id streaminformation, double initialTime);

/**
 * Get the the number of resources required to fully export the scene.
 * Some formats require more than 1 resource to fully export the scene,
 * use this function to find the number of resources required.
 *
 * @param streaminformation  The streaminformation_scene to query.
 * @return positive integer number on success, 0 on failure.
 */
ZINC_API int cmzn_streaminformation_scene_get_number_of_resources_required(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set a flag which determines graphics should be removed before new graphics
 * settings are imported into scene.
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @param overwrite  flag to indicate either graphics should be overwrite.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_overwrite_scene_graphics(
	cmzn_streaminformation_scene_id streaminformation, int overwrite);

/**
 * Get the flag which specifies vertices should be output for each time step;
 * The default value is 1;
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @return  1 if vertices are set to be output for each time step, otherwise 0.
 */
ZINC_API int cmzn_streaminformation_scene_get_output_time_dependent_vertices(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set the flag which specifies vertices should be output for each time step;
 * If this flag is set to 1, vertices will be output at times determined by
 * initial_time, finish_time and number_of_time_steps set by the caller.
 * The default value is 1;
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @param outputTimeDependentVertices  value to be assigned to the flag.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_output_time_dependent_vertices(
	cmzn_streaminformation_scene_id streaminformation,
	int outputTimeDependentVertices);

/**
 * Get the flag which specifies colours should be output for each time step;
 * The default value is 0;
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @return  1 if colours are set to be output for each time step, otherwise 0.
 */
ZINC_API int cmzn_streaminformation_scene_get_output_time_dependent_colours(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set the flag which specifies colours should be output for each time step;
 * If this flag is set to 1, colours will be output at times determined by
 * initial_time, finish_time and number_of_time_steps set by the caller.
 * The default value is 0;
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @param outputTimeDependentColours  value to be assigned to the flag.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_output_time_dependent_colours(
	cmzn_streaminformation_scene_id streaminformation,
	int outputTimeDependentColours);

/**
 * Get the flag which specifies normals should be output for each time step;
 * The default value is 0;
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @return  1 if normals are set to be output for each time step, otherwise 0.
 */
ZINC_API int cmzn_streaminformation_scene_get_output_time_dependent_normals(
	cmzn_streaminformation_scene_id streaminformation);

/**
 * Set the flag which specifies normals should be output for each time step;
 * If this flag is set to 1, normals will be output at times determined by
 * initial_time, finish_time and number_of_time_steps set by the caller.
 * The default value is 0;
 *
 * @param streaminformation  The streaminformation_scene to modify.
 * @param outputTimeDependentNormals  value to be assigned to the flag.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_scene_set_output_time_dependent_normals(
	cmzn_streaminformation_scene_id streaminformation,
	int outputTimeDependentNormals);

#ifdef __cplusplus
}
#endif

#endif
