#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ast.h"

NodoAST* crea_nodo_numero(double valore) {
	NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
	nodo->tipo = NODO_NUMERO;
	nodo->valore_numero = valore;
	nodo->sinistro = NULL;
	nodo->destro = NULL;
	return nodo;
}

NodoAST* crea_nodo_operatore(char op, NodoAST* sx, NodoAST* dx) {
	NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
	nodo->tipo = NODO_OPERATORE;
	nodo->operatore = op;
	nodo->sinistro = sx;
	nodo->destro = dx;
	return nodo;
}

NodoAST* crea_nodo_variabile(const char* nome) {
	NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
	nodo->tipo = NODO_VARIABILE;
	strcpy(nodo->nome, nome);
	nodo->sinistro = NULL;
	nodo->destro = NULL;
	return nodo;
}

NodoAST* crea_nodo_funzione(const char* nome, NodoAST* argomento) {
    NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
    nodo->tipo = NODO_FUNZIONE;
    strcpy(nodo->nome, nome);
    nodo->sinistro = argomento;
    nodo->destro = NULL;
    return nodo;
}

NodoAST* copia_albero(NodoAST* radice) {
	if (radice == NULL) return NULL;

	NodoAST* nuovo = (NodoAST*)malloc(sizeof(NodoAST));
	*nuovo = *radice;

	nuovo->sinistro = copia_albero(radice->sinistro);
	nuovo->destro = copia_albero(radice->destro);

	return nuovo;
}

void libera_albero(NodoAST* radice) {
	if (radice == NULL) return;
	libera_albero(radice->sinistro);
	libera_albero(radice->destro);
	free(radice);
}

void stampa_albero_ricorsiva(NodoAST* radice, int livello, int is_left, int* rami) {
	if (radice == NULL) return;

	for (int i = 0; i < livello - 1; i++) {
		if (rami[i]) {
			printf(" |  ");
		} else {
			printf("    ");
		}
	}

	if (livello > 0) {
        if (is_left) {
            printf(" ├─ ");
            rami[livello - 1] = 1;
        } else {
            printf(" └─");
            rami[livello - 1] = 0;
        }
    }

	if (radice->tipo == NODO_NUMERO) printf("[%.2f]\n", radice->valore_numero);
    else if (radice->tipo == NODO_OPERATORE) printf("[%c]\n", radice->operatore);
    else if (radice->tipo == NODO_VARIABILE) printf("[%s]\n", radice->nome);
    else if (radice->tipo == NODO_FUNZIONE) printf("[%s]\n", radice->nome);

    if (radice->sinistro && radice->destro) {
        stampa_albero_ricorsiva(radice->sinistro, livello + 1, 1, rami);
        stampa_albero_ricorsiva(radice->destro, livello + 1, 0, rami);
    }
    else if (radice->sinistro) {
        stampa_albero_ricorsiva(radice->sinistro, livello + 1, 0, rami);
    } 
    else if (radice->destro) {
        stampa_albero_ricorsiva(radice->destro, livello + 1, 0, rami);
    }
}

void stampa_albero(NodoAST* radice) {
    int rami[100] = {0}; 
    printf("\n");
    stampa_albero_ricorsiva(radice, 0, 0, rami);
    printf("\n");
}

void stampa_equazione_ricorsiva(NodoAST* radice, int prec_padre) {
    if (radice == NULL) return;

    if (radice->tipo == NODO_NUMERO) {
        if (radice->valore_numero == (int)radice->valore_numero) {
            printf("%d", (int)radice->valore_numero);
        } else {
            printf("%.2f", radice->valore_numero);
        }
    } 
    else if (radice->tipo == NODO_VARIABILE) {
        printf("%s", radice->nome);
    } 
    else if (radice->tipo == NODO_FUNZIONE) {
        printf("%s(", radice->nome);
        stampa_equazione_ricorsiva(radice->sinistro, 0); 
        printf(")");
    } 
    else if (radice->tipo == NODO_OPERATORE) {
        int prec_corrente = 0;
        if (radice->operatore == '+' || radice->operatore == '-') prec_corrente = 1;
        else if (radice->operatore == '*' || radice->operatore == '/') prec_corrente = 2;
        else if (radice->operatore == '^') prec_corrente = 3;

        int serve_parentesi = 0;
        if (prec_corrente <= prec_padre) {
            serve_parentesi = 1;
        }

        if (serve_parentesi) printf("(");
        stampa_equazione_ricorsiva(radice->sinistro, prec_corrente);

        if (radice->operatore == '^') printf("^");
        else printf(" %c ", radice->operatore);

        stampa_equazione_ricorsiva(radice->destro, prec_corrente);

        if (serve_parentesi) printf(")");
    }
}

void stampa_equazione(NodoAST* radice) {
    stampa_equazione_ricorsiva(radice, 0);
}
