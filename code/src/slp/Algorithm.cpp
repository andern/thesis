/** 
 * @file    Algorithm.cpp 
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
 * See description in header file. 
 */
#include "coin/ClpModel.hpp"
#include "coin/ClpSimplex.hpp"
#include "slp/Algorithm.hpp"

static inline double lineSearch(const double* p1, const double* p2,
                const double* linCoeffs, const double* quadCoeffs,
                const CoinBigIndex* starts, const int* lengths, int numCols) {
    double num = 0;
    double denom = 0;

    for (int i = 0; i < numCols; i++) {
        double x = p1[i];
        double xhat = p2[i];
        num -= linCoeffs[i]*x;
        num += linCoeffs[i]*xhat;
        for (CoinBigIndex j=starts[i]; j < starts[i]+lengths[i]; j++) {
            double qCoeff = quadCoeffs[j] / 2;

            num += 2*xhat*qCoeff*xhat;
            num -= 2*x*qCoeff*xhat;

            denom += 2*x*qCoeff*x;
            denom -= 4*x*qCoeff*xhat;
            denom += 2*xhat*qCoeff*xhat;
        }
    }
    if (denom == 0) return (0);
    return (num / denom);
}


double lineSearch(const double* p1, const double* p2, const ClpModel& model) {
    const ClpQuadraticObjective* quadObj =
            (dynamic_cast<const ClpQuadraticObjective*>(
                    model.objectiveAsObject()));

    CoinPackedMatrix* quad = quadObj->quadraticObjective();

    return lineSearch(p1, p2, model.getObjCoefficients(),
                quad->getElements(), quad->getVectorStarts(),
                quad->getVectorLengths(), quadObj->numberColumns());
}




double solve(const ClpModel& quad, ClpSimplex& lin, double* x,
        double* x_old, double* T, int maxIters, double tolerance, int numCols)
{
    std::cout.precision(12);
    double objval = 0;
    double stop = 0;
    int k = 0;
    do {
        taylor(T, x, quad);

        lin.chgObjCoefficients(T);
//        lin.checkSolution(2);
        lin.primal(0, 1);

        const double* xhat = lin.primalColumnSolution();

        /*
         * If the step length sends us outside the feasible region,
         * select the feasible point closest to the suggested one.
         */
        double alpha = lineSearch(x, xhat, quad);
        if      (alpha < 0) alpha = 0;
        else if (alpha > 1) alpha = 1;

        /* x_{k+1} = a * x_k + (1-a) * xhat_k */
        for (int i = 0; i < numCols; i++) {
            x_old[i] = x[i];
            x[i] = x_old[i]*alpha + (1-alpha)*xhat[i];
        }

        objval = value(x, quad);

        stop = (value(x_old, quad) - objval);
        stop /= fabs(value(x_old, quad));
        if (k < 3) stop = tolerance + 1;
    } while(k++ < maxIters && stop > tolerance);

    return (objval);
}



static inline void taylor(double* destCoeffs, const double* point,
                const double* linCoeffs, const double* quadCoeffs,
                const CoinBigIndex* starts, const int* lengths, int numCols) {
    memcpy(destCoeffs, linCoeffs, numCols*sizeof(double));
    for (int i = 0; i < numCols; i++) {
        double a = point[i];
        for (CoinBigIndex j=starts[i]; j < starts[i]+lengths[i]; j++) {
            double qCoeff = quadCoeffs[j] / 2;
            destCoeffs[i] += 2*a*qCoeff;
        }
    }
}



static inline double value(const double* point, const double* linCoeffs,
                const double* quadCoeffs, const CoinBigIndex* starts,
                const int* lengths, int numCols) {
    double val = 0;

    for (int i = 0; i < numCols; i++) {
        double p = point[i];
        val += p * linCoeffs[i];
        for (CoinBigIndex j=starts[i]; j < starts[i]+lengths[i]; j++) {
            double qCoeff = quadCoeffs[j] / 2;
            val += p*p * qCoeff;
        }
    }

    return (val);
}



double value(const double* point, const ClpModel& model) {
    const ClpQuadraticObjective* quadObj =
            (dynamic_cast<const ClpQuadraticObjective*>(
                    model.objectiveAsObject()));

    CoinPackedMatrix* quad = quadObj->quadraticObjective();

    return (value(point, model.getObjCoefficients(),
                  quad->getElements(), quad->getVectorStarts(),
                  quad->getVectorLengths(), quadObj->numberColumns()));
}


void taylor(double* destCoeffs, const double* point, const ClpModel& model) {
    const ClpQuadraticObjective* quadObj =
            (dynamic_cast<const ClpQuadraticObjective*>(
                    model.objectiveAsObject()));

    CoinPackedMatrix* quad = quadObj->quadraticObjective();

    taylor(destCoeffs, point, model.getObjCoefficients(),
            quad->getElements(), quad->getVectorStarts(),
            quad->getVectorLengths(), quadObj->numberColumns());
}

