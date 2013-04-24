#include <vector>
#include <iostream>
#include <algorithm>

#include "coin/ClpModel.hpp"
#include "coin/ClpInterior.hpp"
#include "coin/ClpCholeskyBase.hpp"

#include "omp.h"
#include "slp/Tree.hpp"
#include "slp/Good.hpp"
#include "slp/Algorithm.hpp"

#include <cstdlib>
#include <ctime>

template <size_t N>
static std::set<uint16_t>* copy(const std::bitset<N>* a) {
    std::set<uint16_t>* res = new std::set<uint16_t>;
    for (uint16_t i = 0; i < N; i++) {
        if (a->test(i)) res->insert(i);
    }
    return res;
}

int main() {
/*
    ClpModel model = clpFromTxt();

    double t1 = omp_get_wtime();
//    struct vertex* root = construct(model, 2, 10000, 1e-7);
    construct_all(model, 2, 10000, 1e-7);
    double t2 = omp_get_wtime();
    std::cout.precision(20);
    std::cout << "Solved all cases in " << (t2-t1) << " seconds." << std::endl;
//    delete root;
    */
    srand((unsigned int)time(NULL));
    std::vector<std::bitset<32>*> tempsets;
    std::vector<std::bitset<32>*> bitsets;

    int size = 2;

    for (int i = 0; i < 1000; i++) {
        tempsets.push_back(new std::bitset<32>(rand()));
        for (int j = 0; j < 2; j++) {
        }
    }



    std::vector<std::set<uint16_t>*> sets;
    for (int i = 0; i < 1000; i++) {
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
    std::cout << fool << std::endl;

    for (uint16_t i = 0; i < bitsets.size(); i++) {
        std::bitset<2000> notb = ~(*bitsets[i]);
        for (uint16_t j = 0; j < bitsets.size(); j++) {
            fool = isSubset_bit(notb, *bitsets[j]);
        }
    }
    double t3 = omp_get_wtime();
    std::cout << fool << std::endl;

    std::cout << "sets: " << (t2-t1) << " seconds.\n";
    std::cout << "bitsets: " << (t3-t2) << " seconds.\n";
}
