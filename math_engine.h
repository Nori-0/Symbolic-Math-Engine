#ifndef MATH_ENGINE_H
#define MATH_ENGINE_H

#include "ast.h"
typedef struct {
	char nome[10];
	double valore;
} Variabile;

typedef struct {
	Variabile array[50];
	int conteggio;
} TabellaSimboli;

NodoAST* deriva(NodoAST* radice, const char* variabile);
NodoAST* integra(NodoAST* radice, const char* var);
NodoAST* semplifica(NodoAST* radice);
NodoAST* riordina_albero(NodoAST* radice);
double valuta_albero(NodoAST* radice, TabellaSimboli* tabella);
void trova_variabili(NodoAST* radice, TabellaSimboli* tabella);
int alberi_uguali(NodoAST* a, NodoAST* b);


#endif
