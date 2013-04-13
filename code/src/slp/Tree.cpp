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



static bool essentiallyEqual(double a, double b, double epsilon) {
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
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

/*    uint16_t i = 0;
    for (std::set<uint16_t>::iterator it = z.begin(); it != z.end(); ++it) {
        uint16_t ele = *it;
        for(; i < ele; i++) {
            c.push_back(i);
        }
        i++;
    }

    for (; i < n; i++)
        c.push_back(i);
*/
    return c;
}



std::vector<uint16_t> toZSet(const double* arr, uint16_t len, double epsilon) {
    std::vector<uint16_t> s;

    for (uint16_t i = 0; i < len; i++)
        if (essentiallyEqual(arr[i], 0, epsilon))
            s.push_back(i);

    return s;
}


/* Solve a ClpModel with a given modifier. Uses warm-start if possible */
static inline double
mod_solve(const ClpModel& quad, ClpSimplex& lin, const std::vector<uint16_t>& m,
double* x, double* x_old, double* T, int maxIters, double tolerance, int numCols)
{
    for (uint16_t ele : m) {
        lin.setColBounds(ele, 0, 0);
    }
//    for (std::set<uint16_t>::iterator it = m.begin(); it != m.end(); ++it) {
//        lin.setColBounds(*it, 0, 0);
//    }
    lin.checkSolution(1);

    double val = solve(quad, lin, x, x_old, T, maxIters, tolerance, numCols);
    for (uint16_t ele : m) {
        lin.setColBounds(ele, quad.getColLower()[ele], quad.getColUpper()[ele]);
    }
/*    for (std::set<uint16_t>::iterator it = m.begin(); it != m.end(); ++it) {
        lin.setColBounds(*it, quad.getColLower()[*it], quad.getColUpper()[*it]);
    } */
    return val;
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



std::vector<struct vertex*> construct(ClpModel& model, uint16_t breakdowns,
int maxIters, double tolerance)
{
    uint16_t cols = (uint16_t) model.getNumCols();

    /* Some temp-arrays needed to solve all QPs */
    double* x_old = (double*) malloc(cols*sizeof(double));
    double* T = (double*) malloc(cols*sizeof(double));

    /* A temporary linear model for running slp. */
    ClpSimplex lin(model);

    /* Get the root ready. */
    struct vertex* v0 = new struct vertex;
    v0->sol = (double*) malloc(cols*sizeof(double));
    mod_solve(model, lin, v0->m, v0->sol, x_old, T, maxIters, tolerance, cols);
    v0->z = toZSet(v0->sol, cols, tolerance);

    std::vector<struct vertex*> vertices;
    
    /* Create queue and start with the root. */
    std::queue<struct vertex*> q;
    q.push(v0);
    while (!q.empty()) {
        struct vertex* cur = q.front();
        q.pop();
        vertices.push_back(cur);

        uint16_t breaks = std::min((uint16_t)cur->m.size(), breakdowns);
        std::vector<uint16_t> n = complement(cur->z, cols); // z.size >= cur.size, always
        for (uint16_t i = 1; i < breaks; i++) {
            std::vector<uint16_t> f(n.begin(), n.begin()+i);
            do {
                if (find(f, v0) == 0) {
                    struct vertex* nv = new struct vertex;
                    cur->children.push_back(nv);
                    q.push(nv);
                    nv->m = f;
                    nv->sol = (double*) malloc(cols*sizeof(double));
                    mod_solve(model, lin, nv->m, nv->sol, x_old, T,
                              maxIters, tolerance, cols);
                }
            } while(next_combination(n.begin(), n.end(), f.begin(), f.end()));
        }
    }

    free(x_old);
    free(T);

    return vertices;
}
