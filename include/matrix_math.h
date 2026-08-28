#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include "complex_math.h"

typedef struct {
	int rows;
	int cols;
	Complex* data;
} Matrix;

Matrix* matrix_create(int rows, int cols);
void matrix_free(Matrix* m);

void matrix_set(Matrix* m, int r, int c, Complex valore);
Complex matrix_get(Matrix* m, int r, int c);
void matrix_print(Matrix* m);

Matrix* matrix_add(Matrix* a, Matrix* b);
Matrix* matrix_mul(Matrix* a, Matrix* b);
Matrix* matrix_sub(Matrix* a, Matrix* b);
Matrix* matrix_scalar_mul(Matrix* m, Complex scalar);

#endif
