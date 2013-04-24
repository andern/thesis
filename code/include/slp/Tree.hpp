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

#include <vector>
#include <set>
#include <bitset>

#include "coin/ClpModel.hpp"
#include "coin/ClpSimplex.hpp"

/**
 * A structure representing each vertex in a B+ tree. This implementation is
 * very specific and therefore only intended for one specific use.
 */
struct vertex {
    vertex() {};
    vertex(const vertex&) { std::cout << "copied\n"; }
    ~vertex() {
        free(sol);
//        std::cout << "deleted\n";
        for (uint16_t i = 0; i < children.size(); i++) {
            delete children[i];
        }
    }
    std::set<uint16_t> m;
    std::set<uint16_t> z;
    double* sol;

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
struct vertex* find(const std::set<uint16_t>& m, struct vertex* v);
struct vertex* find2(const std::set<uint16_t>& m, struct vertex* v, bool& found);

/**
 * Return a vector of vertices linked together in a tree structure such that
 * for each child vertex v_i of v, v.m is a subset of v_i.m. The first element
 * in the vector is the root.
 *
 * @param  model
 *         a ClpModel for an instance.
 * @return a vector of vertices.
 */
struct vertex* construct(ClpModel& model, uint16_t breakdowns, int maxIters, double tolerance);
struct vertex* construct_clp(ClpModel& model, uint16_t breakdowns, int maxIters, double tolerance);
void construct_all(ClpModel& model, uint16_t breakdowns, int maxIters, double tolerance);
void construct_all_clp(ClpModel& model, uint16_t breakdowns, int maxIters, double tolerance);

std::vector<uint16_t> complement(const std::set<uint16_t>& z, uint16_t n);
std::set<uint16_t> toZSet(const double* arr, uint16_t len, double epsilon);
bool isSubset(const std::set<uint16_t>& a, const std::set<uint16_t>& b);

template <size_t N>
bool isSubset_bit(const std::bitset<N>& a, const std::bitset<N>& notb) {
    std::bitset<N> res(a);
    res &= notb;
    return (res.count() == 0);
//    return (res.to_ulong() == 0);
}

template <class BidIt>
inline bool next_combination(BidIt n_begin, BidIt n_end, BidIt r_begin, BidIt r_end)
{
    bool boolmarked=false;
    BidIt r_marked;

    BidIt n_it1=n_end;
    --n_it1;


    BidIt tmp_r_end=r_end;
    --tmp_r_end;

    for(BidIt r_it1=tmp_r_end; r_it1!=r_begin || r_it1==r_begin; --r_it1,--n_it1)
    {
        if(*r_it1==*n_it1 )
        {
            if(r_it1!=r_begin) //to ensure not at the start of r sequence
            {
                boolmarked=true;
                r_marked=(--r_it1);
                ++r_it1;//add it back again 
                continue;
            }
            else // it means it is at the start the sequence, so return false
                return false;      
        }
        else //if(*r_it1!=*n_it1 )
        {
            //marked code
            if(boolmarked==true)
            {
                //for loop to find which marked is in the first sequence
                BidIt n_marked;//mark in first sequence
                for (BidIt n_it2=n_begin;n_it2!=n_end;++n_it2)
                    if(*r_marked==*n_it2) {n_marked=n_it2;break;}


                BidIt n_it3=++n_marked;    
                for  (BidIt r_it2=r_marked;r_it2!=r_end;++r_it2,++n_it3)
                {
                    *r_it2=*n_it3;
                }
                return true;
            }
            for(BidIt n_it4=n_begin; n_it4!=n_end; ++n_it4)
                if(*r_it1==*n_it4)
                {
                    *r_it1=*(++n_it4);
                    return true;           
                }
        }
    }  

    return true;//will never reach here    
}

template<class Obj> void print(Obj begin, Obj end) {
    for (Obj it = begin; it != end; ++it)
        std::cout << *it << " ";
}
#endif
