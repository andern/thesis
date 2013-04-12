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
#include <set>
#include <vector>
#include <queue>
#include <iostream>

#include "slp/Tree.hpp"




static bool isSubset(const std::set<int16_t>& a, const std::set<int16_t>& b) {
    return std::includes(b.begin(), b.end(), a.begin(), a.end());
}



static struct vertex* finditer(const std::set<int16_t>& m, struct vertex* v) {
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



struct vertex* find(const std::set<int16_t>& m, struct vertex* v) {
    if (v == 0) return 0;
    if (isSubset(v->m, m) && isSubset(m, v->z)) return v;
    return finditer(m, v);
}



static bool essentiallyEqual(double a, double b, double epsilon) {
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}



std::set<int16_t> complement(const std::set<int16_t>& z, int16_t n) {
    std::set<int16_t> c;
    int16_t i = 0;
    for (std::set<int16_t>::iterator it = z.begin(); it != z.end(); ++it) {
        int16_t ele = *it;
        for(; i < ele; i++) {
            c.insert(i);
        }
        i++;
    }

    for (; i < n; i++)
        c.insert(i);

    return c;
}



std::set<int16_t> toZSet(const double* arr, int16_t len, double epsilon) {
    std::set<int16_t> s;

    for (int16_t i = 0; i < len; i++)
        if (essentiallyEqual(arr[i], 0, epsilon))
            s.insert(i);

    return s;
}

std::vector<struct vertex*> construct(ClpModel model, int breakdowns) {
    struct vertex* v0 = new struct vertex;


    std::queue<struct vertex*> q;
    q.push(v0);

    while (!q.empty()) {
        struct vertex* cur = q.pop();
    }
}
*/
