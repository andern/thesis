/**
 * @file    Good.hpp
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
 * This header is for a company.
 */
#ifndef SLP_GOOD_H_
#define SLP_GOOD_H_

#include <cstdio>

#include "coin/CoinPackedMatrix.hpp"
#include "coin/ClpSimplex.hpp"

/**
 * A struct containing all the data needed to solve the following problem:
 *
 *     The QP solver solves the following problem:
 *         min   x'*H*x + ff'x
 *         s.t.: Aeq * x  = beq
 *               lb <= x <= ub
 *
 *     Several similar QP problems are solved in a row, like this:
 *         disconnected = (Dt == 0);
 *         for i = 1:size(Mp,1)
 *             out = (Mp(i,:) == 1);
 *             lb = LB;
 *             ub = UB;
 *
 *             ub(disconnected | out) = 0;
 *             lb(disconnected | out) = 0;
 *
 *             Solve the QP above with H,ff,Aeq,beq,lb and ub as input.
 *         end
 */
struct good_qp {
    ~good_qp() {
        for (int i = 0; i < rows; i++)
            free(Aeq[i]);
        for (int i = 0; i < cols; i++)
            free(H[i]);
        free(Aeq);
        free(H);
        free(ff);
        free(LB);
        free(UB);
    }
    int rows;
    int cols;
    int numQp;
    double** H;   // diagonal matrix of size cols  x cols
    double* ff;   // vector of size          cols
    double** Aeq; // matrix of size          rows  x cols
    double* beq;  // vector of size          rows
    double** Mp;  // matrix of size          numQP x cols
    double* Dt;   // vector of size          cols
    double* LB;   // vector of size          cols
    double* UB;   // vector of size          cols
};

ClpSimplex clpfromqp(struct good_qp& qp);

ClpModel clpFromTxt();

struct good_qp readTxt();
ClpModel randomInstance(int vertices, int edges, double Hzero, double bzero);

#endif
