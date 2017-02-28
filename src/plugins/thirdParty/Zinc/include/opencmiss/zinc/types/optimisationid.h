/**
 * @file optimisationid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_OPTIMISATIONID_H__
#define CMZN_OPTIMISATIONID_H__

/**
 * @brief A description of a non-linear optimisation problem.
 *
 * A description of a non-linear optimisation problem, consisting of the
 * objective fields (generally spatial sums or sum of squares) to be minimised,
 * independent fields whose parameters are to be modified in the optimisation,
 * the solution method and attributes controlling it.
 */
struct cmzn_optimisation;
typedef struct cmzn_optimisation* cmzn_optimisation_id;

/**
 * The optimisation methods available via the optimisation object.
 *
 * @todo Might be worth separating the non-linear problem setup from the optimisation algorithm to mirror
 * the underlying Opt++ structure?
 */
enum cmzn_optimisation_method
{
	CMZN_OPTIMISATION_METHOD_INVALID = 0,
	/*!< Invalid or unspecified optimisation method.
		*/
	CMZN_OPTIMISATION_METHOD_QUASI_NEWTON = 1,
	/*!< The default optimisation method. Suitable for most problems with a small
		* set of independent parameters.
		* Given a scalar valued objective function (scalar sum of all objective
		* fields' components), finds the set of DOFs for the independent field(s)
		* which minimises the objective function value.
		*/
	CMZN_OPTIMISATION_METHOD_LEAST_SQUARES_QUASI_NEWTON = 2
	/*!< A least squares method better suited to larger problems.
		* Finds the set of independent field(s) DOF values which minimises the
		* squares of the objective components supplied. Works specially with fields
		* giving sum-of-squares e.g. nodeset_sum_squares, nodeset_mean_squares to
		* supply individual terms before squaring to the optimiser.
		*/
};

/**
 * Labels of optimisation attributes which may be set or obtained using generic
 * get/set_attribute functions.
 */
enum cmzn_optimisation_attribute
{
	CMZN_OPTIMISATION_ATTRIBUTE_FUNCTION_TOLERANCE = 1,
	/*!< (Opt++ stopping tolerance) Assigns a stopping tolerance for an optimisation algorithm. Please
		* assign tolerances that make sense given the accuracy of your function. For example, setting
		* TOLERANCE to 1.e-4 in your problem means the optimisation algorithm converges when the function
		* value from one iteration to the next changes by 1.e-4 or less.
		*
		* Default value: 1.49012e-8
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_GRADIENT_TOLERANCE = 2,
	/*!< (Opt++ stopping tolerance) Assigns a stopping tolerance for an optimisation algorithm. Please
		* assign tolerances that make sense given your function accuracy. For example, setting
		* GRADIENT_TOLERANCE to 1.e-6 in your problem means the optimisation algorithm converges when the
		* absolute or relative norm of the gradient is 1.e-6 or less.
		*
		* Default value: 6.05545e-6
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_STEP_TOLERANCE = 3,
	/*!< (Opt++ stopping tolerance) Assigns a stopping tolerance for the optimisation algorithm. Please
		* set tolerances that make sense, given the accuracy of your function. For example, setting
		* STEP_TOLERANCE to 1.e-2 in your problem means the optimisation algorithm converges when the relative
		* steplength is 1.e-2 or less.
		*
		* Default value: 1.49012e-8
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_ITERATIONS = 4,
	/*!< (Opt++ stopping tolerance) Places a limit on the number of iterations of the optimisation algorithm.
		* It is useful when your
		* function is computationally expensive or you are debugging the optimisation algorithm. When
		* MAXIMUM_ITERATIONS iterations evaluations have been completed, the optimisation algorithm will stop
		* and report the solution it has reached at that point. It may not be the optimal solution, but it will
		* be the best it could provide given the limit on the number of iterations.
		*
		* Default value: 100.
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_FUNCTION_EVALUATIONS = 5,
	/*!< (Opt++ stopping tolerance) Places an upper bound on the number of function evaluations. The method
		* is useful when your function
		* is computationally expensive and you only have time to perform a limited number of evaluations. When
		* MAXIMUM_NUMBER_FUNCTION_EVALUATIONS function evaluations have been completed, the optimisation
		* algorithm will stop and report the solution it has reached at that point. It may not be the optimal
		* solution, but it will be the best it could provide given the limit on the number of function evaluations.
		*
		* Default value: 1000
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_STEP = 6,
	/*!< (Opt++ steplength control) Places an upper bound on the length of the step that can be taken at each
		* iteration of the optimisation
		* algorithm. If the scale of your optimisation parameters exceeds the bound, adjust accordingly. If you want
		* to be conservative in your search, you may want to set MAXIMUM_STEP to a smaller value than the default. In
		* our (Opt++) experience, the default value is generally fine.
		*
		* Default value: 1.0e3
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_MINIMUM_STEP = 7,
	/*!< (Opt++ steplength control) Places a lower bound on the length of the step that can be taken at each
		* iteration of the optimisation
		* algorithm. If the scale of your optimisation parameters exceeds the bound, adjust accordingly. If you
		* expect the optimisation algorithm to navigate some tricky areas, set MINIMUM_STEP to a smaller value than
		* the default. In our (Opt++) experience, the default value is generally fine.
		*
		* Default value: 1.49012e-8
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_LINESEARCH_TOLERANCE = 8,
	/*!< (Opt++ globalisation strategy parameter) In practice, the linesearch tolerance is set to a small value,
		* so that almost any decrease in the function value results in an acceptable step. Suggested values are
		* 1.e-4 for Newton methods and 1.e-1 for more exact line searches.
		*
		* Default value: 1.e-4
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_BACKTRACK_ITERATIONS = 9,
	/*!< (Opt++ globalisation strategy parameter) Only relevant when you use a algorithm with a linesearch
		* search strategy. The value places a limit on the number of iterations in the linesearch routine of the
		* optimisation algorithm. If the limit is reached before computing a step with acceptable decrease, the
		* algorithm terminates with an error message. The reported solution is not optimal, but the best one
		* given the number of linesearch iterations. Increasing the number of linesearch iterations may lead to
		* an acceptable step, but it also results in more function evaluations and a shorter steplength.
		*
		* Default value: 5
		*/
	CMZN_OPTIMISATION_ATTRIBUTE_TRUST_REGION_SIZE = 10
	/*!< (Opt++ globalisation strategy parameter) Only relevant when you are using an algorithm with a trust-region
		* or a trustpds search strategy. The value initialises the size of the trust region.
		*
		* Default value: 0.1?? (@todo Need to better initialise the default value, see https://software.sandia.gov/opt++/opt++2.4_doc/html/ControlParameters.html)
		*
		* If your problem is quadratic or close to it, you may want to initialise the size of the trust region to a
		* larger value.
		* @todo Reserving this one for when trust region methods are available via the API. Currently everything
		* uses linesearch methods only.
		*/
};

#endif
