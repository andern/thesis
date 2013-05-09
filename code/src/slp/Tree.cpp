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
#include <set>
#include <bitset>
#include <queue>
#include <iostream>

#include "coin/ClpCholeskyBase.hpp"
#include "coin/ClpInterior.hpp"
#include "coin/ClpModel.hpp"
#include "coin/ClpSimplex.hpp"

#include "slp/Algorithm.hpp"
#include "slp/Tree.hpp"



bool isSubset(const std::set<uint16_t>& a, const std::set<uint16_t>& b) {
    return std::includes(b.begin(), b.end(), a.begin(), a.end());
}

struct vertex* dfind(const std::set<uint16_t>& m, struct vertex* v) {
    if (isSubset(m, v->z)) return v;
    for (struct vertex* vi : v->children) {
        if (isSubset(vi->m, m)) {
            struct vertex* temp = dfind(m, vi);
            if (temp != 0) return temp;
        }
    }
    return 0;
}


static bool mfindrec(const std::set<uint16_t>& m, struct vertex* v, struct vertex*& ret) {
    if (isSubset(m, v->z)) return true;
    for (struct vertex* vi : v->children) {
        if (isSubset(vi->m, m)) {
            ret = vi;
            bool temp = mfind(m, vi, ret);
            if (temp) return true;
        }
    }
    return false;
}
bool mfind(const std::set<uint16_t>& m, struct vertex* v, struct vertex*& ret) {
    ret = v;
    return mfindrec(m, v, ret);
}


static struct vertex* finditer2(const std::set<uint16_t>& m, struct vertex* v, bool& found, struct vertex* ret) {
    found = false;
    for (unsigned long i = 0; i < v->children.size(); i++) {
        struct vertex* vi = v->children[i];
        if (isSubset(vi->m, m)) {
            ret = vi;
            if (isSubset(m, vi->z)) {
                found = true;
                return vi;
            } else {
                return finditer2(m, vi, found, ret);
            }
        }
    }
    return ret;
}



struct vertex* find2(const std::set<uint16_t>& m, struct vertex* v, bool& found) {
    found = false;
    if (v == 0) return 0;
    if (isSubset(v->m, m) && isSubset(m, v->z)) {
        found = true;
        return v;
    }
    return finditer2(m, v, found, v);
}



static struct vertex* finditer(const std::set<uint16_t>& m, struct vertex* v) {
    for (unsigned long i = 0; i < v->children.size(); i++) {
        struct vertex* vi = v->children[i];
        if (isSubset(vi->m, m)) {
            if (isSubset(m, vi->z)) {
                print(m.begin(), m.end());
                std::cout << " and ";
                print(vi->m.begin(), vi->m.end());
                std::cout << " share solution." << std::endl;
                return vi;
            } else {
                finditer(m, vi);
            }
        }
    }
    return 0;
}



struct vertex* find(const std::set<uint16_t>& m, struct vertex* v) {
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



std::vector<uint16_t> complement(const std::set<uint16_t>& z, uint16_t n) {
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



std::set<uint16_t> toZSet(const double* arr, uint16_t len, double epsilon) {
    std::set<uint16_t> s;

    for (uint16_t i = 0; i < len; i++)
        if (approximatelyZero(arr[i], epsilon))
            s.insert(i);

    return s;
}


/* Solve a ClpModel with a given modifier. Uses warm-start if possible */
static inline double mod_solve(const ClpModel& quad, ClpSimplex& lin,
const std::set<uint16_t>& m, const double* lower, const double* upper,
double* nlower, double* nupper,  double* x, double* x_old, double* T,
int maxIters, double tolerance, int numCols)
{
    for (std::set<uint16_t>::iterator it = m.begin(); it != m.end(); ++it) {
        nlower[*it] = 0;
        nupper[*it] = 0;
    }
    lin.dual();
    memcpy(x, lin.primalColumnSolution(), numCols*sizeof(double));

    double val = solve(quad, lin, x, x_old, T, maxIters, tolerance, numCols);

    for (std::set<uint16_t>::iterator it = m.begin(); it != m.end(); ++it) {
        nlower[*it] = lower[*it];
        nupper[*it] = upper[*it];
    }

    return val;
}



static inline double mod_solve_clp(ClpModel& model,
const std::set<uint16_t>& m, double* x, const double* lower,
const double* upper, double tolerance, int maxIters, int numCols)
{
    for (std::set<uint16_t>::iterator it = m.begin(); it != m.end(); ++it) {
        model.setColBounds(*it, 0, 0);
    }

    ClpInterior quad(model);
    quad.setPrimalTolerance(tolerance);
    quad.setDualTolerance(tolerance);
    quad.setMaximumBarrierIterations(maxIters);

    ClpCholeskyBase* cholesky = new ClpCholeskyBase;
    cholesky->setKKT(true);
    quad.setCholesky(cholesky);

    quad.primalDual();
    memcpy(x, quad.getColSolution(), numCols*sizeof(double));

    for (std::set<uint16_t>::iterator it = m.begin(); it != m.end(); ++it) {
        model.setColBounds(*it, lower[*it], upper[*it]);
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



struct vertex* construct(ClpModel& model, uint16_t breakdowns,
int maxIters, double tolerance)
{
    model.setLogLevel(0);
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
    v0->z = toZSet(v0->sol, cols, tolerance);

//    std::vector<uint16_t> n = complement(v0->z, cols); // z.size >= v0.size, always
    std::vector<uint16_t> n;
    for (uint16_t i = 0; i < cols; i++) n.push_back(i);
    uint16_t breaks = std::min((uint16_t)n.size(), breakdowns);

    struct vertex* ret;
    for (int i = 0; i < breaks; i++) {
        std::vector<uint16_t> f(n.begin(), n.begin()+(i+1));
        do {
            std::set<uint16_t> fset(f.begin(), f.end());
            bool found = mfind(fset, v0, ret);
            if (!found) {
                struct vertex* nv = new struct vertex;
                ret->children.push_back(nv);

                nv->m = fset;
                nv->sol = (double*) malloc(cols*sizeof(double));
                mod_solve(model, lin, nv->m, lower, upper, nlower, nupper, nv->sol, x_old, T, maxIters, tolerance, cols);
                nv->z = toZSet(nv->sol, cols, tolerance);
/*
                std::cout << value(nv->sol, model) << ": ";
                print(f.begin(), f.end());
                std::cout << std::endl;
                */
            } 
        } while(next_combination(n.begin(), n.end(), f.begin(), f.end()));
    }

    free(x_old);
    free(T);

    return v0;
}

void construct_all(ClpModel& model, uint16_t breakdowns,
int maxIters, double tolerance)
{
    model.setLogLevel(0);
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

    std::vector<uint16_t> n;
    for (uint16_t i = 0; i < cols; i++) n.push_back(i);
    uint16_t breaks = std::min((uint16_t)n.size(), breakdowns);

    std::cout.precision(20);
    for (int i = 0; i < breaks; i++) {
        std::vector<uint16_t> f(n.begin(), n.begin()+(i+1));
        do {
            std::set<uint16_t> fset(f.begin(), f.end());
            double* sol = (double*) malloc(cols*sizeof(double));
            mod_solve(model, lin, fset, lower, upper, nlower, nupper, sol, x_old, T, maxIters, tolerance, cols);

 /*           std::cout << value(sol, model) << ": ";
            print(f.begin(), f.end());
            std::cout << std::endl; */
            free(sol);
        } while(next_combination(n.begin(), n.end(), f.begin(), f.end()));
    }

    free(x_old);
    free(T);
}



void construct_all_clp(ClpModel& model, uint16_t breakdowns,
int maxIters, double tolerance)
{
    model.setLogLevel(0);
    uint16_t cols = (uint16_t) model.getNumCols();

    /* Get some pointers ready. */
    double* lower = (double*) malloc(cols*sizeof(double));
    double* upper = (double*) malloc(cols*sizeof(double));
    memcpy(lower, model.getColLower(), cols*sizeof(double));
    memcpy(upper, model.getColUpper(), cols*sizeof(double));

    std::vector<uint16_t> n;
    for (uint16_t i = 0; i < cols; i++) n.push_back(i);
    uint16_t breaks = std::min((uint16_t)n.size(), breakdowns);

    std::cout.precision(20);
    for (int i = 0; i < breaks; i++) {
        std::vector<uint16_t> f(n.begin(), n.begin()+(i+1));
        do {
            std::set<uint16_t> fset(f.begin(), f.end());
            double* sol = (double*) malloc(cols*sizeof(double));
            mod_solve_clp(model, fset, sol, lower, upper, tolerance, maxIters, cols);

 /*           std::cout << value(sol, model) << ": ";
            print(f.begin(), f.end());
            std::cout << std::endl; */
            free(sol);
        } while(next_combination(n.begin(), n.end(), f.begin(), f.end()));
    }

    free(lower);
    free(upper);
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
    mod_solve_clp(model, v0->m, v0->sol, lower, upper, tolerance, maxIters, cols);
    v0->z = toZSet(v0->sol, cols, tolerance);

//    std::vector<uint16_t> n = complement(v0->z, cols); // z.size >= v0.size, always
    std::vector<uint16_t> n;
    for (uint16_t i = 0; i < cols; i++) n.push_back(i);
    uint16_t breaks = std::min((uint16_t)n.size(), breakdowns);

    struct vertex* ret;
    int numNotNeededSolved = 0;
    for (int i = 0; i < breaks; i++) {
        std::vector<uint16_t> f(n.begin(), n.begin()+(i+1));
        do {
            std::set<uint16_t> fset(f.begin(), f.end());
            bool found = mfind(fset, v0, ret);
            if (!found) {
                struct vertex* nv = new struct vertex;
                ret->children.push_back(nv);

                nv->m = fset;
                nv->sol = (double*) malloc(cols*sizeof(double));
                mod_solve_clp(model, nv->m, nv->sol, lower, upper, tolerance, maxIters, cols);
                nv->z = toZSet(nv->sol, cols, tolerance);

/*                std::cout << value(nv->sol, model) << ": ";
                print(f.begin(), f.end());
                std::cout << std::endl; */

            } else {
                numNotNeededSolved++;
//                print(fset.begin(), fset.end());
//                std::cout << std::endl;
            }
        } while(next_combination(n.begin(), n.end(), f.begin(), f.end()));
    }
//    std::cout << "We skipped solving: " << numNotNeededSolved << std::endl;
    printf("%d +", numNotNeededSolved);

    free(lower);
    free(upper);

    return v0;
}
