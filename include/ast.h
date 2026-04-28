#ifndef AST_H
#define AST_H

#include "complex_math.h"

typedef enum {
	NODO_NUMERO,
	NODO_VARIABILE,
	NODO_OPERATORE,
	NODO_FUNZIONE
} TipoNodo;

typedef struct NodoAST {
    TipoNodo tipo;
    union {
	Complex valore_numero;
        char operatore;
        char nome[10]; 
    };
    struct NodoAST* sinistro;
    struct NodoAST* destro;
} NodoAST;

NodoAST* crea_nodo_numero(Complex valore);
NodoAST* crea_nodo_operatore(char op, NodoAST* sx, NodoAST* dx);
NodoAST* crea_nodo_variabile(const char* nome);
NodoAST* crea_nodo_funzione(const char* nome, NodoAST* argomento);
NodoAST* copia_albero(NodoAST* radice);

void libera_albero(NodoAST* radice);
void stampa_albero(NodoAST* radice);
void stampa_equazione(NodoAST* radice);


#endif
