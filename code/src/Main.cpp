#include <vector>
#include <iostream>
#include <algorithm>

#include <string>
#include <sstream>

#include "coin/ClpModel.hpp"
#include "coin/ClpInterior.hpp"
#include "coin/ClpCholeskyBase.hpp"

#include "omp.h"
#include "slp/Tree.hpp"
#include "slp/Good.hpp"
#include "slp/Algorithm.hpp"

#include <cstdlib>
#include <ctime>

#define N 65
#define SIZE 32*N
/*
static std::set<uint16_t>* copy(const std::bitset<SIZE>* a) {
    std::set<uint16_t>* res = new std::set<uint16_t>;
    for (uint16_t i = 0; i < SIZE; i++) {
        if (a->test(i)) res->insert(i);
    }
    return res;
}
*/

static struct vertex* tree() {
    struct vertex* v0 = new struct vertex;
    struct vertex* v2 = new struct vertex;
    struct vertex* v3 = new struct vertex;
    struct vertex* v7 = new struct vertex;
    struct vertex* v8 = new struct vertex;
    struct vertex* v10 = new struct vertex;
    struct vertex* v12 = new struct vertex;
    struct vertex* v15 = new struct vertex;
    struct vertex* v16 = new struct vertex;
    struct vertex* v28 = new struct vertex;
    struct vertex* v29 = new struct vertex;

    v0->children.push_back(v2);
    v0->children.push_back(v8);
    v0->children.push_back(v16);

    v2->children.push_back(v3);

    v3->children.push_back(v7);

    v8->children.push_back(v10);
    v8->children.push_back(v12);

    v12->children.push_back(v15);

    v16->children.push_back(v28);
    v28->children.push_back(v29);

    /* Add variables to the sets */
    v0->z.insert(1);
    v0->z.insert(3);

    v2->m.insert(2);
    v2->z.insert(2);
    v2->z.insert(3);
    v2->z.insert(5);

    v3->m.insert(1);
    v3->m.insert(2);
    v3->z.insert(1);
    v3->z.insert(2);
    v3->z.insert(4);
    v3->z.insert(5);

    v7->m.insert(1);
    v7->m.insert(2);
    v7->m.insert(3);
    v7->z.insert(1);
    v7->z.insert(2);
    v7->z.insert(3);
    v7->z.insert(5);

    v8->m.insert(4);
    v8->z.insert(4);
    v8->z.insert(1);
    v8->z.insert(5);

    v10->m.insert(2);
    v10->m.insert(4);
    v10->z.insert(2);
    v10->z.insert(3);
    v10->z.insert(4);
    v10->z.insert(5);

    v12->m.insert(3);
    v12->m.insert(4);
    v12->z.insert(4);
    v12->z.insert(3);
    v12->z.insert(1);

    v15->m.insert(1);
    v15->m.insert(2);
    v15->m.insert(3);
    v15->m.insert(4);
    v15->z.insert(1);
    v15->z.insert(2);
    v15->z.insert(3);
    v15->z.insert(4);

    v16->m.insert(5);
    v16->z.insert(5);
    v16->z.insert(1);
    v16->z.insert(3);

    v28->m.insert(3);
    v28->m.insert(4);
    v28->m.insert(5);
    v28->z.insert(3);
    v28->z.insert(4);
    v28->z.insert(5);
    v28->z.insert(2);

    v29->m.insert(1);
    v29->m.insert(3);
    v29->m.insert(4);
    v29->m.insert(5);
    v29->z.insert(1);
    v29->z.insert(3);
    v29->z.insert(4);
    v29->z.insert(5);
    v29->z.insert(2);

    return v0;
}


int main() {

    ClpModel model = clpFromTxt();

    double t1 = omp_get_wtime();
    struct vertex* root = construct_clp(model, 2, 10000, 1e-7);
//    construct_all(model, 2, 10000, 1e-7);
//    struct vertex* root = tree();
    double t2 = omp_get_wtime();
    std::cout.precision(20);
    std::cout << "Solved all cases in " << (t2-t1) << " seconds." << std::endl;

    delete root;
    
/*    std::set<uint16_t> lookFor;
    lookFor.insert(2);
    lookFor.insert(3);
    lookFor.insert(4);

    struct vertex* res;
    mfind(lookFor, root, res);
    std::cout << res << std::endl;
    print(res->m.begin(), res->m.end());
    std::cout << std::endl;
*/
/*    srand((unsigned int)time(NULL));

    std::vector<std::bitset<SIZE>*> bitsets;

    for (int i = 0; i < 3000; i++) {
        std::stringstream ss;
        for (int j = 0; j < N; j++) {
            std::bitset<32> bs(rand());
            ss << bs.to_string();
        }
        bitsets.push_back(new std::bitset<SIZE>(ss.str()));
    }

    std::vector<std::set<uint16_t>*> sets;
    for (int i = 0; i < 3000; i++) {
        sets.push_back(copy(bitsets[i]));
    }

    bool fool;

    double t1 = omp_get_wtime();
    for (uint16_t i = 0; i < sets.size(); i++) {
        for (uint16_t j = 0; j < sets.size(); j++) {
            fool = isSubset(*sets[i], *sets[j]);
        }
    }
    double t2 = omp_get_wtime();
    //std::cout << fool << std::endl;

    for (uint16_t i = 0; i < bitsets.size(); i++) {
        std::bitset<SIZE> notb = ~(*bitsets[i]);
        for (uint16_t j = 0; j < bitsets.size(); j++) {
            fool = isSubset_bit(notb, *bitsets[j]);
        }
    }
    double t3 = omp_get_wtime();
    //std::cout << fool << std::endl;

    std::cout << "(" << SIZE << ", " << (t2-t1) << ")\n";
    std::cout << "(" << SIZE << ", " << (t3-t2) << ")\n";
    */
}
