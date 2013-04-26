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

static std::set<uint16_t>* copy(const std::bitset<SIZE>* a) {
    std::set<uint16_t>* res = new std::set<uint16_t>;
    for (uint16_t i = 0; i < SIZE; i++) {
        if (a->test(i)) res->insert(i);
    }
    return res;
}

int main() {

    ClpModel model = clpFromTxt();

    double t1 = omp_get_wtime();
    struct vertex* root = construct_clp(model, 2, 10000, 1e-7);
//    construct_all(model, 2, 10000, 1e-7);
    double t2 = omp_get_wtime();
    std::cout.precision(20);
    std::cout << "Solved all cases in " << (t2-t1) << " seconds." << std::endl;
    delete root;
    
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
