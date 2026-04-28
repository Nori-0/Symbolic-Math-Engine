#ifndef MATH_ENGINE_H
#define MATH_ENGINE_H

#include "ast.h"
#include "complex_math.h"

typedef struct {
	char nome[10];
	Complex valore;
} Variabile;

typedef struct {
	Variabile array[50];
	int conteggio;
} TabellaSimboli;

NodoAST* deriva(NodoAST* radice, const char* variabile);
NodoAST* integra(NodoAST* radice, const char* var);
NodoAST* semplifica(NodoAST* radice);
NodoAST* riordina_albero(NodoAST* radice);
Complex valuta_albero(NodoAST* radice, TabellaSimboli* tabella);
void trova_variabili(NodoAST* radice, TabellaSimboli* tabella);
int alberi_uguali(NodoAST* a, NodoAST* b);


#endif
