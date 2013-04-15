/**
 * @file    Tree.cpp
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
 * See header for description.
 */
#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>

#include "coin/ClpCholeskyBase.hpp"
#include "coin/ClpInterior.hpp"
#include "coin/ClpModel.hpp"
#include "coin/ClpSimplex.hpp"

#include "slp/Algorithm.hpp"
#include "slp/Tree.hpp"



static bool isSubset(const std::vector<uint16_t>& a, const std::vector<uint16_t>& b) {
    return std::includes(b.begin(), b.end(), a.begin(), a.end());
}



static struct vertex* finditer(const std::vector<uint16_t>& m, struct vertex* v) {
    for (unsigned long i = 0; i < v->children.size(); i++) {
        struct vertex* vi = v->children[i];
        if (isSubset(vi->m, m)) {
            if (isSubset(m, vi->z))
                return vi;
            else
                finditer(m, vi);
        }
    }
    return 0;
}



struct vertex* find(const std::vector<uint16_t>& m, struct vertex* v) {
    if (v == 0) return 0;
    if (isSubset(v->m, m) && isSubset(m, v->z)) return v;
    return finditer(m, v);
}



/* From Donald Knuth. */
//static bool essentiallyEqual(double a, double b, double epsilon) {
//    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
//}

//static bool approximatelyEqual(double a, double b, double epsilon) {
//    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
//}

static bool approximatelyZero(double num, double epsilon) {
    return fabs(num) <= epsilon;
}



std::vector<uint16_t> complement(const std::vector<uint16_t>& z, uint16_t n) {
    std::vector<uint16_t> c;

    uint16_t i = 0;
    for (uint16_t ele : z) {
        for(; i < ele; i++) {
            c.push_back(i);
        }
        i++;
    }

    for (; i < n; i++)
        c.push_back(i);

    return c;
}



std::vector<uint16_t> toZSet(const double* arr, uint16_t len, double epsilon) {
    std::vector<uint16_t> s;

    for (uint16_t i = 0; i < len; i++)
        if (approximatelyZero(arr[i], epsilon))
            s.push_back(i);

    return s;
}


/* Solve a ClpModel with a given modifier. Uses warm-start if possible */
static inline double mod_solve(const ClpModel& quad, ClpSimplex& lin,
const std::vector<uint16_t>& m, const double* lower, const double* upper,
double* nlower, double* nupper,  double* x, double* x_old, double* T,
int maxIters, double tolerance, int numCols)
{
    for (uint16_t ele : m) {
        nlower[ele] = 0;
        nupper[ele] = 0;
    }
    lin.dual();
    memcpy(x, lin.primalColumnSolution(), numCols*sizeof(double));

    double val = solve(quad, lin, x, x_old, T, maxIters, tolerance, numCols);

    for (uint16_t ele : m) {
        nlower[ele] = lower[ele];
        nupper[ele] = upper[ele];
    }

    return val;
}



static inline double mod_solve_clp(ClpModel& model,
const std::vector<uint16_t>& m, double* x, const double* lower,
const double* upper, int numCols)
{

    for (uint16_t ele : m) {
        model.setColBounds(ele, 0, 0);
    }

    ClpInterior quad(model);

    ClpCholeskyBase* cholesky = new ClpCholeskyBase;
    cholesky->setKKT(true);
    quad.setCholesky(cholesky);

    quad.primalDual();
    memcpy(x, quad.getColSolution(), numCols*sizeof(double));

    for (uint16_t ele : m) {
        model.setColBounds(ele, lower[ele], upper[ele]);
    }

    return quad.getObjValue();
}



/* Return a set with the first given number of elements of a given set.
 * Assume num <= s.size()
 */
static inline std::vector<uint16_t> get_first(std::vector<uint16_t>& s, uint16_t num) {
    std::vector<uint16_t> first;
    for (uint16_t i = 0; i < num; i++) {
        first.push_back(s[i]);
    }
    return first;
}



struct vertex* construct_clp(ClpModel& model, uint16_t breakdowns,
int maxIters, double tolerance)
{
    model.setLogLevel(0);
    uint16_t cols = (uint16_t) model.getNumCols();

    /* Get some pointers ready. */
    double* lower = (double*) malloc(cols*sizeof(double));
    double* upper = (double*) malloc(cols*sizeof(double));
    memcpy(lower, model.getColLower(), cols*sizeof(double));
    memcpy(upper, model.getColUpper(), cols*sizeof(double));

    /* Get the root ready. */
    struct vertex* v0 = new struct vertex;
    v0->sol = (double*) calloc(cols,sizeof(double));
    mod_solve_clp(model, v0->m, v0->sol, lower, upper, cols);
    v0->z = toZSet(v0->sol, cols, tolerance);

    /* Create queue and start with the root. */
    std::queue<struct vertex*> q;
    q.push(v0);
    std::cout.precision(15);
    while (!q.empty()) {
        struct vertex* cur = q.front();
        q.pop();

        uint16_t breaks = std::min((uint16_t)cur->z.size(), breakdowns);
        std::vector<uint16_t> n = complement(cur->z, cols); // z.size >= cur.size, always
        for (uint16_t i = (uint16_t) cur->m.size(); i < breaks; i++) {
            std::vector<uint16_t> f(n.begin(), n.begin()+(i+1));
            do {
                if (find(f, v0) == 0) {
                    struct vertex* nv = new struct vertex;
                    cur->children.push_back(nv);

                    nv->m = f; // Data is copied!
                    //print(nv->m.begin(), nv->m.end());
                    nv->sol = (double*) malloc(cols*sizeof(double));
                    mod_solve_clp(model, nv->m, nv->sol, lower, upper, cols);
                    nv->z = toZSet(nv->sol, cols, tolerance);

               //     print(f.begin(), f.end());

                    q.push(nv);
                } else {
                    //std::cout << "Had solution to: ";
                    print(f.begin(), f.end());
                }
            } while(next_combination(n.begin(), n.end(), f.begin(), f.end()));
        }
    }

    free(lower);
    free(upper);

    return v0;
}



struct vertex* construct(ClpModel& model, uint16_t breakdowns,
int maxIters, double tolerance)
{
    uint16_t cols = (uint16_t) model.getNumCols();

    /* Some temp-arrays needed to solve all QPs */
    double* x_old = (double*) malloc(cols*sizeof(double));
    double* T = (double*) malloc(cols*sizeof(double));

    /* A temporary linear model for running slp. */
    ClpSimplex lin(model);
    lin.setLogLevel(0);
    lin.deleteQuadraticObjective();

    /* Get some pointers ready. */
    const double* lower = model.getColLower();
    const double* upper = model.getColUpper();
    double* nlower = lin.columnLower();
    double* nupper = lin.columnUpper();

    /* Get the root ready. */
    struct vertex* v0 = new struct vertex;
    v0->sol = (double*) calloc(cols,sizeof(double));
    mod_solve(model, lin, v0->m, lower, upper, nlower, nupper, v0->sol, x_old, T, maxIters, tolerance, cols);
    v0->z = toZSet(v0->sol, 30, tolerance);

    /* Create queue and start with the root. */
    std::queue<struct vertex*> q;
    q.push(v0);
    std::cout.precision(15);
    while (!q.empty()) {
        struct vertex* cur = q.front();
        q.pop();

        uint16_t breaks = std::min((uint16_t)cur->z.size(), breakdowns);
        std::vector<uint16_t> n = complement(cur->z, 30); // z.size >= cur.size, always
        for (uint16_t i = (uint16_t) cur->m.size(); i < breaks; i++) {
            std::vector<uint16_t> f(n.begin(), n.begin()+(i+1));
            do {
                if (find(f, v0) == 0) {
                    struct vertex* nv = new struct vertex;
                    cur->children.push_back(nv);

                    nv->m = f; // Data is copied!
                    nv->sol = (double*) malloc(cols*sizeof(double));
                    mod_solve(model, lin, nv->m, lower, upper, nlower, nupper, nv->sol, x_old, T, maxIters, tolerance, cols);
                    nv->z = toZSet(nv->sol, 30, tolerance);

                    print(f.begin(), f.end());

                    q.push(nv);
                } else {
                    std::cout << "Had solution to: ";
                    print(f.begin(), f.end());
                }
            } while(next_combination(n.begin(), n.end(), f.begin(), f.end()));
        }
    }

    free(x_old);
    free(T);

    return v0;
}
