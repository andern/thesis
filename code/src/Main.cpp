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



std::vector<struct vertex*> buildTestTree();
std::vector<struct vertex*> buildTestTree() {
    struct vertex* v0 = new struct vertex;
    struct vertex* v2 = new struct vertex;
    struct vertex* v8 = new struct vertex;
    struct vertex* v16 = new struct vertex;
    struct vertex* v10 = new struct vertex;
    struct vertex* v3 = new struct vertex;
    struct vertex* v13 = new struct vertex;
    struct vertex* v11 = new struct vertex;
    struct vertex* v7 = new struct vertex;
    struct vertex* v28 = new struct vertex;
    struct vertex* v29 = new struct vertex;

    /* Connect the vertices */
    v0->children.push_back(v2);
    v0->children.push_back(v8);
    v0->children.push_back(v16);
    v0->children.push_back(v10);

    v2->children.push_back(v3);
    v3->children.push_back(v7);

    v8->children.push_back(v13);
    v13->children.push_back(v28);
    v28->children.push_back(v29);

    v10->children.push_back(v11);

    /* Add variables to the sets */
    v0->z.insert(1);
    v0->z.insert(3);

    v2->m.insert(2);
    v2->z.insert(2);
    v2->z.insert(3);
    v2->z.insert(5);

    v8->m.insert(4);
    v8->z.insert(1);
    v8->z.insert(4);
    v8->z.insert(5);

    v16->m.insert(5);
    v16->z.insert(1);
    v16->z.insert(3);
    v16->z.insert(5);

    v10->m.insert(2);
    v10->m.insert(4);
    v10->z.insert(1);
    v10->z.insert(2);
    v10->z.insert(3);
    v10->z.insert(4);

    v3->m.insert(1);
    v3->m.insert(2);
    v3->z.insert(1);
    v3->z.insert(2);
    v3->z.insert(4);
    v3->z.insert(5);

    v13->m.insert(3);
    v13->m.insert(4);
    v13->z.insert(1);
    v13->z.insert(3);
    v13->z.insert(4);

    v11->m.insert(1);
    v11->m.insert(2);
    v11->m.insert(4);
    v11->z.insert(1);
    v11->z.insert(2);
    v11->z.insert(3);
    v11->z.insert(4);

    v7->m.insert(1);
    v7->m.insert(2);
    v7->m.insert(3);
    v7->z.insert(1);
    v7->z.insert(2);
    v7->z.insert(3);
    v7->z.insert(5);

    v28->m.insert(3);
    v28->m.insert(4);
    v28->m.insert(5);
    v28->z.insert(2);
    v28->z.insert(3);
    v28->z.insert(4);
    v28->z.insert(5);

    v29->m.insert(1);
    v29->m.insert(3);
    v29->m.insert(4);
    v29->m.insert(5);
    v29->z.insert(1);
    v29->z.insert(2);
    v29->z.insert(3);
    v29->z.insert(4);
    v29->z.insert(5);

    std::vector<struct vertex*> vertices;
    vertices.push_back(v0);
    vertices.push_back(v2);
    vertices.push_back(v8);
    vertices.push_back(v16);
    vertices.push_back(v10);
    vertices.push_back(v3);
    vertices.push_back(v13);
    vertices.push_back(v11);
    vertices.push_back(v7);
    vertices.push_back(v28);
    vertices.push_back(v29); 

    return vertices;
}

int main() {
/*    std::vector<uint16_t> all;
    all.push_back(1);
    all.push_back(2);
    all.push_back(3);
    all.push_back(4);
    all.push_back(5);

    std::vector<uint16_t> test;

    std::cout << std::includes(all.begin(), all.end(), test.begin(), test.end()) << std::endl;

    
    
    
*/    
    ClpModel model = clpFromTxt();

    double t1 = omp_get_wtime();
//    struct vertex* root = construct(model, 2, 10000, 1e-4);
    construct_all(model, 2, 10000, 1e-6);
    double t2 = omp_get_wtime();
    std::cout.precision(20);
    std::cout << "Solved all cases in " << (t2-t1) << " seconds." << std::endl;
//    delete root;
//    std::vector<struct vertex*> vertices = buildTestTree();
//    struct vertex* root = vertices[0];

/*    std::set<uint16_t> m;
    m.insert(1);
    m.insert(2);
    m.insert(3);
    m.insert(4);
*/
    /*
    ClpModel mmodel = clpFromTxt();
    ClpInterior quad(mmodel);
    ClpSimplex model(mmodel);
    model.deleteQuadraticObjective();
    model.setLogLevel(0);

    ClpCholeskyBase* cholesky = new ClpCholeskyBase;
    cholesky->setKKT(true);
    quad.setCholesky(cholesky);

    const double* lower = mmodel.getColLower();
    const double* upper = mmodel.getColUpper();

    double* nlower = model.columnLower();
    double* nupper = model.columnUpper();

    quad.primalDual();

    double* x = (double*) calloc(model.getNumCols(),sizeof(double));
    memcpy(x, quad.getColSolution(), model.getNumCols()*sizeof(double));
    double* x_old = (double*) malloc(model.getNumCols()*sizeof(double));
    double* T = (double*) malloc(model.getNumCols()*sizeof(double));

    solve(quad, model, x, x_old, T, 1000, 1e-4, model.getNumCols());
    std::cout << model.statusOfProblem() << std::endl;
    std::cout << value(x, quad) << std::endl;

    nlower[8] = 0;
    nupper[8] = 0;

//    model.chgColumnLower(nlower);
//    model.chgColumnUpper(nupper);

    std::cout << model.statusOfProblem() << std::endl;
    model.dual();
    std::cout << value(model.primalColumnSolution(), quad) << std::endl;
    std::cout << model.statusOfProblem() << std::endl;

    memcpy(x, model.primalColumnSolution(), model.getNumCols()*sizeof(double));
    solve(quad, model, x, x_old, T, 10000, 1e-4, model.getNumCols());
    std::cout << model.statusOfProblem() << std::endl;
    std::cout << value(x, quad) << std::endl;


//    solve(quad, model, x, x_old, T, 1000, 1e-7, model.getNumCols());
    free(x);
    free(x_old);
    free(T);
    //free(nlower);
    //free(nupper);
*/

}
