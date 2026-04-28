#include <stdio.h>
#include <math.h>
#include "complex_math.h"

Complex complex_create(double real, double imag){
	Complex c;
	c.re = real;
	c.im = imag;
	return c;
}

Complex complex_add(Complex a, Complex b){
	return complex_create(a.re + b.re, a.im + b.im);
}

Complex complex_sub(Complex a, Complex b){
	return complex_create(a.re - b.re, a.im - b.im);
}

Complex complex_mul(Complex a, Complex b){
	double real_part = (a.re * b.re) - (a.im * b.im);
	double imag_part = (a.re * b.im) + (a.im * b.re);
	return complex_create(real_part, imag_part);
}

Complex complex_div(Complex a, Complex b) {
	double denominator = (b.re * b.re) + (b.im * b.im);

	if (denominator == 0.0) {
		printf("[ERROR] Division by zero in the complex plane!\n");
		return complex_create(NAN, NAN);
	}

	double real_part = ((a.re * b.re) + (a.im * b.im)) / denominator;
	double imag_part = ((a.im * b.re) - (a.re * b.im)) / denominator;
	return complex_create(real_part, imag_part);
}

double complex_modulo(Complex a) {
	return sqrt((a.re * a.re) + (a.im * a.im));
}


/*Verify if a Complex number is almost zero*/
int complex_is_zero(Complex c) {
	return (fabs(c.re) < 1e-9 && fabs(c.im) < 1e-9);
}

/*Verify if a Complex number is almost one (only real part)*/
int complex_is_one(Complex c) {
	return (fabs(c.re - 1.0) < 1e-9 && fabs(c.im) < 1e-9);
}

/*Complex square root (De Moivre formula)*/
Complex complex_sqrt(Complex c) {
	double r = complex_modulo(c);
	double theta = atan2(c.im, c.re);
	return complex_create(sqrt(r) * cos(theta / 2.0), sqrt(r) * sin(theta / 2.0));
}

/*Complex power a^b = exp(b * ln(a))*/
Complex complex_pow(Complex a, Complex b) {
	if (complex_is_zero(a)) return complex_create(0,0);

	double r = complex_modulo(a);
	double theta = atan2(a.im, a.re);

	Complex ln_a = complex_create(log(r), theta);
	Complex exponent = complex_mul(b, ln_a);

	double exp_x = exp(exponent.re);
	return complex_create(exp_x * cos(exponent.im), exp_x * sin(exponent.im));
}

/*Complex exponential*/
Complex complex_exp(Complex c) {
	double exp_x = exp(c.re);
	return complex_create(exp_x * cos(c.im), exp_x * sin(c.im));
}

/*Complex logarithm*/
Complex complex_log(Complex c) {
	double r = complex_modulo(c);
	double theta = atan2(c.im, c.re);
	return complex_create(log(r), theta);
}

/*Complex sine*/
Complex complex_sin(Complex c) {
	return complex_create(sin(c.re) * cosh(c.im), cos(c.re) * sinh(c.im));
}

/*Complex cosine*/
Complex complex_cos(Complex c) {
	return complex_create(cos(c.re) * cosh(c.im), -sin(c.re) * sinh(c.im));
}

/*Complex tan*/
Complex complex_tan(Complex c) {
	return complex_div(complex_sin(c), complex_cos(c));
}

/*Printing funcition with semplifications*/
void complex_printf(Complex a) {
	if (fabs(a.re) < 1e-10) a.re = 0.0;
	if (fabs(a.im) < 1e-10) a.im = 0.0;

	if (a.im == 0.0) {
		printf("%g", a.re);
	} else if (a.re == 0.0) {
		printf("%gi", a.im);
	} else if (a.im > 0.0) {
		printf("%g + %gi", a.re, a.im);
	} else {
		printf("%g - %gi", a.re, -a.im);
	}
}


