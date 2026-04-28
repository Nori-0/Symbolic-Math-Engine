#ifndef COMPLEX_MATH_H
#define COMPLEX_MATH_H

typedef struct {
	double re;
	double im;
} Complex;

Complex complex_create(double real, double imag);

Complex complex_add(Complex a, Complex b);
Complex complex_sub(Complex a, Complex b);
Complex complex_mul(Complex a, Complex b);
Complex complex_div(Complex a, Complex b);
Complex complex_pow(Complex a, Complex b);
Complex complex_sqrt(Complex c);
Complex complex_exp(Complex c);
Complex complex_log(Complex c);
Complex complex_sin(Complex c);
Complex complex_cos(Complex c);
Complex complex_tan(Complex c);

double complex_modulo(Complex a);

int complex_is_zero(Complex c);
int complex_is_one(Complex c);
void complex_printf(Complex a);

#endif
