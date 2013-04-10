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

#include "slp/Tree.hpp"



bool isSubset(const std::set<short>& a, const std::set<short>& b) {
    return std::includes(b.begin(), b.end(), a.begin(), a.end());
}



struct vertex* find(const std::set<short>& m, struct vertex* v) {
    if (isSubset(m, v->z))
        return v;

    for (unsigned long i = 0; i < v->children.size(); i++) {
        struct vertex* vi = v->children[i];
        if (isSubset(vi->m, m))
            find(m, vi);
    }
    return 0;
}
