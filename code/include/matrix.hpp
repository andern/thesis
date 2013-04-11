/**
 * @file    matrix.hpp
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
 * A matrix structure that can be accessed both one-dimensionally and
 * two-dimensionally.
 * This code was written when the project was C only.
 */
#ifndef SLP_STRUCT_MATRIX
#define SLP_STRUCT_MATRIX

#include <dynarray.hpp>

typedef struct matrix {
  int rows;
  int cols;
  double *data;
  double **dptr;
} matrix;

matrix matfarr(double *arr, int rows, int cols);

matrix matcalloc(int rows, int cols);

matrix matmalloc(int rows, int cols);

matrix matfdyn(struct dynarray darr, int cols);
#endif
