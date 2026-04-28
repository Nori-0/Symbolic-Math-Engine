#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"

int precedenza(TipoToken tipo) {
    if (tipo == TOKEN_PIU || tipo == TOKEN_MENO) return 1;
    if (tipo == TOKEN_PER || tipo == TOKEN_DIVISO) return 2;
    if (tipo == TOKEN_POTENZA) return 3;
    if (tipo == TOKEN_FUNZIONE || tipo == TOKEN_MENO_UNARIO) return 4;
    return 0;
}

NodoAST* stack_nodi[MAX_TOKENS];
int cima_nodi = 0;

Token stack_op[MAX_TOKENS];
int cima_op = 0;

void push_nodo(NodoAST* nodo) { 
	stack_nodi[cima_nodi++] = nodo; 
}
NodoAST* pop_nodo() { 
	return stack_nodi[--cima_nodi]; 
}

void push_op(Token op) { 
	stack_op[cima_op++] = op; 
}
Token pop_op() { 
	return stack_op[--cima_op]; 
}

void risolvi_operatore() {
    Token op = pop_op();
    
    if (op.tipo == TOKEN_FUNZIONE || op.tipo == TOKEN_MENO_UNARIO) {
        NodoAST* argomento = pop_nodo();
        if (op.tipo == TOKEN_FUNZIONE) {
            push_nodo(crea_nodo_funzione(op.nome, argomento));
        } else {
            NodoAST* meno_uno = crea_nodo_numero(complex_create(-1.0, 0.0));
            push_nodo(crea_nodo_operatore('*', meno_uno, argomento));
        }
    } 
    else {
        NodoAST* destro = pop_nodo(); 
        NodoAST* sinistro = pop_nodo();
        char simbolo = '?';
        
        if (op.tipo == TOKEN_PIU) simbolo = '+';
        else if (op.tipo == TOKEN_MENO) simbolo = '-';
        else if (op.tipo == TOKEN_PER) simbolo = '*';
        else if (op.tipo == TOKEN_DIVISO) simbolo = '/';
        else if (op.tipo == TOKEN_POTENZA) simbolo = '^';
        
        push_nodo(crea_nodo_operatore(simbolo, sinistro, destro));
    }
}

//quì vi è l'algoritmo Shutting-Yard principale
NodoAST* costruisci_albero() {
    cima_nodi = 0;
    cima_op = 0;

    for (int i = 0; i < numero_token_estratti; i++) {
        Token t = lista_token[i];

        //dritto nella pila dei nodi
        if (t.tipo == TOKEN_NUMERO) {
            push_nodo(crea_nodo_numero(t.valore));
        } 
        else if (t.tipo == TOKEN_VARIABILE) {
            push_nodo(crea_nodo_variabile(t.nome));
        }
        //nella pila operatori
        else if (t.tipo == TOKEN_FUNZIONE || t.tipo == TOKEN_PAREN_APERTA) {
            push_op(t);
        }
        //risolviamo tutto fino alla Parentesi Aperta
        else if (t.tipo == TOKEN_PAREN_CHIUSA) {
            while (cima_op > 0 && stack_op[cima_op - 1].tipo != TOKEN_PAREN_APERTA) {
                risolvi_operatore();
            }
            if (cima_op > 0) pop_op(); //rimozione parentesi aperta dalla pila
        }
        //in questo caso è un operatore
        else { 
            while (cima_op > 0 && stack_op[cima_op - 1].tipo != TOKEN_PAREN_APERTA &&
                   precedenza(stack_op[cima_op - 1].tipo) >= precedenza(t.tipo)) {
                //risoluzione in caso di forza maggiore dell'operatore in pila
                risolvi_operatore();
            }
            push_op(t);
        }
    }

    //risoluzione finale in caso di operatori rimasti
    while (cima_op > 0) {
        risolvi_operatore();
    }
    //restituzione albero completo
    return pop_nodo();
}
