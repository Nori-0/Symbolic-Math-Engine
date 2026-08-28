#include <stdio.h>
#include <stdlib.h>
#include "matrix_math.h"

Matrix* matrix_create(int rows, int cols) {
	Matrix* m = (Matrix*)malloc(sizeof(Matrix));
	m->rows = rows;
	m->cols = cols;
	m->data = (Complex*)calloc(rows * cols, sizeof(Complex));
	return m;
}

void matrix_free(Matrix* m) {
	if (m != NULL) {
		if (m->data != NULL) free(m->data);
		free(m);
	}
}

void matrix_set(Matrix* m, int r, int c, Complex value) {
	if (r >= 0 && r < m->rows && c >= 0 && c < m->cols) {
		m->data[r * m->cols + c] = value;
	}
}

Complex matrix_get(Matrix* m, int r, int c) {
	if (r >= 0 && r < m->rows && c >= 0 && c < m->cols) {
		return m->data[r * m->cols + c];
	}
	return complex_create(0.0, 0.0);
}

void matrix_print(Matrix* m) {
	if (m == NULL) return;
	printf("[");
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			complex_printf(matrix_get(m, i, j));
			if (j < m->cols - 1) printf(", ");
		}
		if (i < m->rows - 1) printf("; ");
	}
	printf("]");
}

Matrix* matrix_add(Matrix* a, Matrix* b) {
    if (a == NULL || b == NULL || a->rows != b->rows || a->cols != b->cols) {
        printf("[ERROR] Matrix addition dimension mismatch: (%dx%d) + (%dx%d)\n", 
               a ? a->rows : 0, a ? a->cols : 0, b ? b->rows : 0, b ? b->cols : 0);
        return NULL;
    }
    Matrix* res = matrix_create(a->rows, a->cols);
    for (int i = 0; i < a->rows * a->cols; i++) {
        res->data[i] = complex_add(a->data[i], b->data[i]);
    }
    return res;
}

Matrix* matrix_mul(Matrix* a, Matrix* b) {
    if (a == NULL || b == NULL || a->cols != b->rows) {
        printf("[ERROR] Matrix multiplication dimension mismatch: (%dx%d) * (%dx%d)\n", 
               a ? a->rows : 0, a ? a->cols : 0, b ? b->rows : 0, b ? b->cols : 0);
        return NULL;
    }
    Matrix* res = matrix_create(a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            Complex sum = complex_create(0.0, 0.0);
            for (int k = 0; k < a->cols; k++) {
                Complex prod = complex_mul(matrix_get(a, i, k), matrix_get(b, k, j));
                sum = complex_add(sum, prod);
            }
            matrix_set(res, i, j, sum);
        }
    }
    return res;
}

Matrix* matrix_sub(Matrix* a, Matrix* b) {
    if (a == NULL || b == NULL || a->rows != b->rows || a->cols != b->cols) {
        printf("[ERROR] Matrix subtraction dimension mismatch!\n");
        return NULL;
    }
    Matrix* res = matrix_create(a->rows, a->cols);
    for (int i = 0; i < a->rows * a->cols; i++) {
        res->data[i] = complex_sub(a->data[i], b->data[i]);
    }
    return res;
}

Matrix* matrix_scalar_mul(Matrix* m, Complex scalar) {
    if (m == NULL) return NULL;
    Matrix* res = matrix_create(m->rows, m->cols);
    for(int i = 0; i < m->rows * m->cols; i++){
        res->data[i] = complex_mul(m->data[i], scalar);
    }
    return res;
}
