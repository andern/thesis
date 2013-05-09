/**
 * @file    Good.cpp
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
#include <cstdio>
#include <fstream>

#include "coin/ClpPackedMatrix.hpp"
#include "coin/ClpSimplex.hpp"
#include "coin/CoinPackedMatrix.hpp"
#include "slp/Good.hpp"

static CoinPackedMatrix packMatrix(double** m, int rows, int cols) {
    std::vector<int> row_ind_vec;
    std::vector<int> col_ind_vec;
    std::vector<double> ele_vec;
    CoinBigIndex numels = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double num = m[i][j];
            if (num != 0.0) {
                row_ind_vec.push_back(i);
                col_ind_vec.push_back(j);
                ele_vec.push_back(num);
                numels++;
            }
        }
    }

    const int *rowIndices = &row_ind_vec[0];
    const int *colIndices = &col_ind_vec[0];
    const double *elements = &ele_vec[0];

    CoinPackedMatrix cpm(false, rowIndices, colIndices, elements, numels);
    cpm.setDimensions(rows, cols);

    return cpm;
}



ClpSimplex clpfromqp(struct good_qp& qp) {
    ClpSimplex out;

    CoinPackedMatrix Aeq = packMatrix(qp.Aeq, qp.rows, qp.cols);

    double* rowlb = (double*) calloc(qp.cols, sizeof(double));
    double* rowub = (double*) calloc(qp.cols, sizeof(double));
    out.loadProblem(Aeq, qp.LB, qp.UB, qp.ff, rowlb, rowub);
    out.loadQuadraticObjective(packMatrix(qp.H, qp.cols, qp.cols));

    return (out);
}


static double* readArray(const char *filepath) {
    std::ifstream fin(filepath);

    int elements = 0;
    fin >> elements;

    double* arr = (double*) malloc(elements*sizeof(double));

    int i = 0;
    char cNum[30];
    while (i < elements) {
        fin.getline(cNum, 256, ';');
        arr[i++] = atof(cNum);
    }

    fin.close();

    return arr;
}


static double** readMatrix(const char* filepath) {
    std::ifstream fin(filepath);

    int rows = 0;
    int cols = 0;
    fin >> rows;
    fin >> cols;

    double** arr = (double**) malloc(rows*sizeof(double*));
    for (int i = 0; i < rows; i++)
        arr[i] = (double*) malloc(cols * sizeof(double));

    int i = 0;
    int j = 0;
    char cNum[30];
    while (i < rows) {
        if (j >= cols) {
            i++;
            j = 0;
        }
        if (i >= rows) break;

        fin.getline(cNum, 256, ';');
        arr[i][j++] = atof(cNum);
    }

    fin.close();

    return arr;
}

ClpModel clpFromTxt() {
    int rows = 0;
    int cols = 0;
    std::ifstream fin("data/small/A.txt");
    fin >> rows;
    fin >> cols;
    fin.close();

    double** A = readMatrix("data/small/A.txt");
    double** F = readMatrix("data/small/F.txt");

    double* c = readArray("data/small/c.txt");
    double* x_L = readArray("data/small/x_L.txt");
    double* x_U = readArray("data/small/x_U.txt");

    ClpModel out;

    ClpPackedMatrix Aeq(packMatrix(A, rows, cols));
    CoinPackedMatrix H = packMatrix(F, cols, cols);

    for (int i = 0; i < rows; i++)
        free(A[i]);
    for (int i = 0; i < cols; i++)
        free(F[i]);
    free(A);
    free(F);


    double* rowlb = (double*) calloc(cols, sizeof(double));
    double* rowub = (double*) calloc(cols, sizeof(double));
    out.loadProblem(Aeq, x_L, x_U, c, rowlb, rowub);
    out.loadQuadraticObjective(H);

    free(c);
    free(x_L);
    free(x_U);
    free(rowlb);
    free(rowub);

    return out;

}


struct good_qp readTxt() {
    struct good_qp good;
    std::ifstream fin("data/small/A.txt");
    fin >> good.rows;
    fin >> good.cols;
    fin.close();

    double** A = readMatrix("data/small/A.txt");
    double** F = readMatrix("data/small/F.txt");

    double* c = readArray("data/small/c.txt");
    double* x_low = readArray("data/small/x_L.txt");
    double* x_up = readArray("data/small/x_U.txt");
/*
    double** A = (double**) malloc((good.rows+1)*sizeof(double*));
    for (int i = 0; i < good.cols; i++)
        A[i] = (double*) malloc(good.cols*sizeof(double));

    double** F = (double**) malloc(good.cols*sizeof(double*));
    for (int i = 0; i < good.cols; i++)
        F[i] = (double*) malloc(good.cols*sizeof(double));

    double* c = (double*) malloc(good.cols*sizeof(double));

    double* x_up = (double*) malloc(good.cols*sizeof(double));
    double* x_low = (double*) malloc(good.cols*sizeof(double));

    std::cout << "beginning" << std::endl;
    readMatrix(A, "data/vlarge/A.txt");
    std::cout << "A done" << std::endl;
    readMatrix(F, "data/vlarge/F.txt");
    std::cout << "F done" << std::endl;

    readArray(c, "data/vlarge/c.txt");
    std::cout << "c done" << std::endl;
    readArray(x_low, "data/vlarge/x_L.txt");
    std::cout << "x_L done" << std::endl;
    readArray(x_up, "data/vlarge/x_U.txt");
    std::cout << "x_U done" << std::endl; */


    std::cout << "Assigning pointers." << std::endl;
    good.Aeq = A;
    good.H = F;
    good.ff = c;
    good.LB = x_low;
    good.UB = x_up;

    std::cout << "Done assigning pointers." << std::endl;

    /* Temporarily no Mp matrix 
       good.numQp = 3000;
       double** Mp = (double**) malloc(1*sizeof(double*));
       Mp[0] = (double*) calloc(good.cols, sizeof(double));
       for (int i = 1; i < good.numQp; i++) {
       Mp[i] = (double*) calloc(good.cols, sizeof(double));
       Mp[i][i-1] = 1;
       }
       good.Mp = Mp;
       */

    /* Temporarily no Dt vector
       double* Dt = (double*) malloc(good.cols*sizeof(double));
       memset(Dt, 1, good.cols*sizeof(double));
       good.Dt = Dt;
       */

    return good;
}


static double g_randd(double min, double max) {
    double num = (double) rand() / RAND_MAX;
    return (min + (num * (max - min)));
}

static int g_randi(int min, int max) {
    return (rand() % (max-min) + min);
}


ClpModel randomInstance(int vertices, int edges, double Hzero, double bzero) {
//    srand((uint16_t)time(NULL));

    /* Matrix A */
    double** m = (double**) malloc(vertices*sizeof(double*));
    for (int i = 0; i < vertices; i++)
        m[i] = (double*) calloc(edges,sizeof(double));

    int row = 0;
    for (int i = 0; i < edges; i++) {
        m[row][i] = 1;
        int r = row;
        while (r == row) row = rand() % vertices;
        m[row][i] = -1;
    }
    CoinPackedMatrix A = packMatrix(m, vertices, edges);

    for (int i = 0; i < vertices; i++)
        free(m[i]);
    free(m);

    /* Matrix H */
    std::vector<int> row_ind_vec;
    std::vector<int> col_ind_vec;
    std::vector<double> ele_vec;
    int numels = 0;

    for (int i = 0; i < edges; i++) {
        /* If not 0 */
        if (((double)rand() / RAND_MAX) >= Hzero) {
            double num = g_randd(0.00001, 0.01);
            row_ind_vec.push_back(i);
            col_ind_vec.push_back(i);
            ele_vec.push_back(num);
            numels++;
        }
    }
    const int *rowIndices = &row_ind_vec[0];
    const int *colIndices = &col_ind_vec[0];
    const double *elements = &ele_vec[0];

    CoinPackedMatrix H(false, rowIndices, colIndices, elements, numels);
    H.setDimensions(edges, edges);

    /* Vector b */
    double* b = (double*) calloc(edges,sizeof(double));

    for (int i = 0; i < edges; i++) {
        /* If not 0 */
        if (((double)rand() / RAND_MAX) >= bzero) {
            double num = g_randi(10, 70);
            if (num >= 30) num = -num;
            b[i] = num;
        }
    }

    /* Vector lb */
    double* lb = (double*) malloc(edges*sizeof(double));
    for (int i = 0; i < edges; i++) {
        lb[i] = -g_randd(0.0, 1000.0);
    }

    /* Vector ub */
    double* ub = (double*) malloc(edges*sizeof(double));
    for (int i = 0; i < edges; i++) {
        ub[i] = g_randd(0.0, 1000.0);
    }

    ClpSimplex model;

    model.loadProblem(A, lb, ub, b, 0, 0);
    model.loadQuadraticObjective(H);

    free(b);
    free(lb);
    free(ub);

    return model;
}
