#ifndef LEXER_H
#define LEXER_H

#define MAX_INPUT_SIZE 256
#define MAX_TOKENS 100

#include "complex_math.h"

typedef enum {
    TOKEN_NUMERO,
    TOKEN_VARIABILE,
    TOKEN_PIU,
    TOKEN_MENO,
    TOKEN_PER,
    TOKEN_DIVISO,
    TOKEN_POTENZA,
    TOKEN_PAREN_APERTA,
    TOKEN_PAREN_CHIUSA,
    TOKEN_FUNZIONE,
    TOKEN_MENO_UNARIO,
    TOKEN_FINE
} TipoToken;

typedef struct {
    TipoToken tipo;
    Complex valore;
    char nome[10];
} Token;


extern Token lista_token[MAX_TOKENS];
extern int numero_token_estratti;


void tokenizza(const char* input);
void stampa_token();

#endif // LEXER_H
