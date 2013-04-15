/**
 * @file    Algorithm.hpp
 * @author  Andreas Halle <andern@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2012, 2013 Andreas Halle
 *
 * Insert license.
 *
 * @section DESCRIPTION
 *
 * This header contains mostly mathematical methods needed for implementing the
 * SLP algorithm.
 */
#ifndef SLP_ALGORITHM_H_
#define SLP_ALGORITHM_H_

#include "coin/ClpModel.hpp"
#include "coin/ClpQuadraticObjective.hpp"
#include "coin/ClpSimplex.hpp"
#include "slp/Good.hpp"

/**
 * Solve several similar QP problems where only the bounds change between each
 * problem.
 *
 * @param  x
 *         Destination for an optimal solution of all the QP problems in in.
 * @param  in
 *         Struct containing all the necessary data.
 * @return an array containing the objective value to each of the QP problems.
 */
double* good_solve(double** destPoints, struct good_qp in, int maxIters,
                   double tolerance, double *initial);

double* good_solve2(double** destPoints, struct good_qp in, int maxIters,
                   double tolerance, double *initial);

double* good_solve_clp(double** destPoints, struct good_qp in, int maxIters,
                       double tolerance);

/**
 * Perform a Taylor-series expansion of the objective function of the
 * given QP problem at the given point.
 *
 * @param destCoeffs
 *        Pointer to coefficient destination.
 * @param point
 *        Point to perform the expansion at.
 * @param model
 *        ClpModel that has the objective function.
 */
void taylor(double* destCoeffs, const double* point, const ClpModel& model);

/**
 * Solve a QP problem using SLP with line search.
 *
 * @param  quad
 *         ClpModel containing the QP problem.
 * @param  x
 *         Array containing the initial guess vector. This array will be
 *         overwritten with a final solution.
 * @param  maxIters
 *         Number of iterations to perform if the stopping criteria is not met.
 * @param  tolerance
 *         Epsilon for the stopping criteria.
 * @return the objective value of the solved QP problem..
 */
//double solve(ClpModel quad, double* x, int maxIters, double tolerance);

double solve(const ClpModel& quad, ClpSimplex& lin, double* x,
double* x_old, double* T, int maxIters, double tolerance, int numCols);

/**
 * Evaluate the objective function of the given model at the given point.
 *
 * @param point
 *        Point to evaluate the objective function at.
 * @param model
 *        ClpModel that has the objective function to evaluate.
 */
double value(const double* point, const ClpModel& model);

/**
 * Perform a line search by finding the minimum objective value of the given
 * QP problem of all the points between the two given points.
 *
 * @param  p1
 *         Start point.
 * @param  p2
 *         End point.
 * @param  model
 *         ClpModel of the QP problem.
 * @return the step length alpha.
 */
double lineSearch(const double* p1, const double* p2, const ClpModel& model);


double solve(const ClpModel& quad, ClpSimplex& lin, double* x,
double* x_old, double* T, int maxIters, double tolerance, int numCols);
#endif
