/**
 * @file    qp.hpp
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
 * A structure that contains all the information of a QP.
 * This code was written when the project was C only.
 */
#include "matrix.hpp"

#ifndef SLP_STRUCT_QP
#define SLP_STRUCT_QP
typedef struct qp {
  struct matrix H;
  struct matrix ff;
  struct matrix Aeq;
  struct matrix beq;
  struct matrix Mp;
  struct matrix Dt;
  struct matrix LB;
  struct matrix UB;
} qp;
#endif
