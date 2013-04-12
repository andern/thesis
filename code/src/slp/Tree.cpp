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

#include <iostream>



static bool isSubset(const std::set<short>& a, const std::set<short>& b) {
    return std::includes(b.begin(), b.end(), a.begin(), a.end());
}



static struct vertex* finditer(const std::set<short>& m, struct vertex* v) {
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



struct vertex* find(const std::set<short>& m, struct vertex* v) {
    if (v == 0) return 0;
    if (isSubset(v->m, m) && isSubset(m, v->z)) return v;
    return finditer(m, v);
}



void printset(const std::set<short>& s) {
    for (std::set<short>::iterator it=s.begin(); it!=s.end(); ++it)
        std::cout << *it;
    std::cout << std::endl;
}



static bool essentiallyEqual(double a, double b, double epsilon)
{
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}



std::set<short> toZSet(const double* arr, short len, double epsilon);
std::set<short> toZSet(const double* arr, short len, double epsilon) {
    std::set<short> s;

    for (short i = 0; i < len; i++)
        if (essentiallyEqual(arr[i], 0, epsilon))
            s.insert(i);

    return s;
}


/*
template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k, const Iterator last) {
    if ((first == last) || (first == k) || (last == k))
        return false;
    Iterator itr1 = first;
    Iterator itr2 = last;
    ++itr1;
    if (last == itr1)
        return false;
    itr1 = last;
    --itr1;
    itr1 = k;
    --itr2;
    while (first != itr1) {
        if (*--itr1 < *itr2) {
            Iterator j = k;
            while (!(*itr1 < *j)) ++j;
            std::iter_swap(itr1,j);
            ++itr1;
            ++j;
            itr2 = k;
            std::rotate(itr1,j,last);
            while (last != j) {
                ++j;
                ++itr2;
            }
            std::rotate(k,itr2,last);
            return true;
        }
    }
    std::rotate(first,k,last);
    return false;
}
*/


template <class BidIt>
bool next_combination(BidIt n_begin, BidIt n_end,
BidIt r_begin, BidIt r_end)
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


/*
   std::vector<struct vertex*> construct(ClpModel model, int breakdowns) {
   struct vertex* v0 = new struct vertex;

   std::queue<struct vertex*> q;
   q.push(v0);

   while (!q.empty()) {
   struct vertex* cur = q.pop();
   }
   }
   */
