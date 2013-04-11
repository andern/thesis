/**
 * @file    dynarray.hpp
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
 * A dynamic array used for reading the xml-format.
 * This code was written when the project was C only.
 */
#ifndef SLP_STRUCT_DYNARRAY
#define SLP_STRUCT_DYNARRAY
typedef struct dynarray {
  double *arr;
  /* Number of allocated elements. */
  int capacity;
  /* Number of actual elements. */
  int size;
} dynarray;

void dynadd(dynarray *arr, double element);

void dynarray_destruct(dynarray *arr);

dynarray dynarray_struct(int capacity);
#endif
