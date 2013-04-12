/**
 * @file    Tree.hpp
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
 * A very specific B+ tree implementation in C++ for Slp.
 */
#ifndef SLP_TREE_H_
#define SLP_TREE_H_


#include <iostream>
#include <iterator>

#include <set>
#include <vector>

#include "coin/ClpModel.hpp"

/**
 * A structure representing each vertex in a B+ tree. This implementation is
 * very specific and therefore only intended for one specific use.
 */
struct vertex {
    ~vertex() { std::cout << "deleted\n"; }
    vertex() {};
    vertex(const vertex&) { std::cout << "copied\n"; }
    std::set<short> m;
    std::set<short> z;

    std::vector<struct vertex*> children;
};

/**
 * Return a vertex w in the tree where vertex v is a root, such that
 * the modifier m is a subset of the set of zeroes w.m.
 *
 * @param  m
 *         a modifier.
 * @param  v
 *         a vertex.
 * @return a vertex.
 */
struct vertex* find(const std::set<short>& m, struct vertex* v);

/**
 * Return a vector of vertices linked together in a tree structure such that
 * for each child vertex v_i of v, v.m is a subset of v_i.m. The first element
 * in the vector is the root.
 *
 * @param  model
 *         a ClpModel for an instance.
 * @return a vector of vertices.
 */
std::vector<struct vertex*> construct(ClpModel model, int breakdowns);

//template <typename Iterator>
//inline bool next_combination(const Iterator first, Iterator k, const Iterator last);
template <class BidIt>
bool next_combination(BidIt n_begin, BidIt n_end,
BidIt r_begin, BidIt r_end);

void printset(const std::set<short>& s);

#endif
