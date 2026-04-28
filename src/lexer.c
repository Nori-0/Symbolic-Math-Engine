#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "complex_math.h"

Token lista_token[MAX_TOKENS];
int numero_token_estratti = 0;

int estrai_numero(const char* input, int indice, Token* token) {
	char buffer[64];
	int j = 0;
	int punti = 0;

	while (input[indice] != '\0' && (isdigit(input[indice]) || input[indice] == '.' || input[indice] == ',')) {
		char c = input[indice];

		if ( c == '.' || c == ',') {
			punti++;
			if (punti > 1) {
				printf("\n[SYNTAX ERROR] Invalid number format: too many decimal separators near %s\n", &input[indice - 2]);
					break;
				}
			buffer[j] = '.';
		} else {
			buffer[j] = c;
		}
		j++;
		indice++;
		if (j >= 63) {
			printf("\n[LIMIT ERROR] Number entered is too long.\n");
			break;
		}
	}

	buffer[j] = '\0';
	token->tipo = TOKEN_NUMERO;
	token->valore = complex_create(strtod(buffer, NULL), 0.0);

	return indice;
}

int estrai_testo(const char* input, int indice, Token* token) {
	char buffer[15];
	int j = 0;

	while (input[indice] != '\0' && isalpha(input[indice])) {
		if (j < 14) {
			buffer[j] = input[indice];
			j++;
		}
		indice++;
	}
	buffer[j] = '\0';

	if (strlen(buffer) == 1) {
		if (buffer[0] == 'i') {
			token->tipo = TOKEN_NUMERO;
			token->valore = complex_create(0.0, 1.0);
		} else {
			token->tipo = TOKEN_VARIABILE;
			strcpy(token->nome, buffer);
		}
	}

	else if (strcmp(buffer, "sin") == 0 || strcmp(buffer, "cos") == 0 || strcmp(buffer, "tan") == 0 || strcmp(buffer, "log") == 0 || strcmp(buffer, "sqrt") == 0 || strcmp(buffer, "exp") == 0) {
		token->tipo = TOKEN_FUNZIONE;
		strcpy(token->nome, buffer);
	} 
	else {
		printf("[WARNING] Unknown text: %s\n", buffer);
	}
	
	return indice;
}

void tokenizza(const char* input) {
	int i = 0;
	numero_token_estratti = 0;

	while(input[i] != '\0') {
		if (isspace(input[i])) {
			i++;
			continue;
		}
		if (isdigit(input[i]) || input[i] == '.' || input[i] == ','){
				i = estrai_numero(input, i, &lista_token[numero_token_estratti]);
				numero_token_estratti++;
				continue;
		}
		if (isalpha(input[i])) {
			i = estrai_testo(input, i, &lista_token[numero_token_estratti]);
			numero_token_estratti++;
			continue;
		}

		Token* t = &lista_token[numero_token_estratti];
		switch (input[i]) {
		case '+': t->tipo = TOKEN_PIU; break;
            	case '-':
			  if (numero_token_estratti == 0 ||
					  lista_token[numero_token_estratti - 1].tipo == TOKEN_PAREN_APERTA ||
					  (lista_token[numero_token_estratti - 1].tipo >= TOKEN_PIU && lista_token[numero_token_estratti - 1].tipo <= TOKEN_POTENZA)) {
				  t->tipo = TOKEN_MENO_UNARIO;
			  }else {
				  t->tipo = TOKEN_MENO;
			  }
			  break;

            	case '*': t->tipo = TOKEN_PER; break;
            	case '/': t->tipo = TOKEN_DIVISO; break;
            	case '^': t->tipo = TOKEN_POTENZA; break;
            	case '(': t->tipo = TOKEN_PAREN_APERTA; break;
            	case ')': t->tipo = TOKEN_PAREN_CHIUSA; break;
            	default:
                	printf("[ERROR] Unrecognized character %c\n", input[i]);
                	i++;
                	continue;
        	}
        	numero_token_estratti++;
        	i++;
    	}
}

//vedo cosa ha capito il lexer
void stampa_token() {
    printf("\n--- TOKEN ESTRATTI ---\n");
    for (int i = 0; i < numero_token_estratti; i++) {
        Token t = lista_token[i];
        switch (t.tipo) {
            case TOKEN_NUMERO:       printf("NUMERO:  ");
				     complex_printf(t.valore);
				     printf("\n");
				     break;
            case TOKEN_VARIABILE:    printf("VARIABILE: %s\n", t.nome); break;
            case TOKEN_FUNZIONE:     printf("FUNZIONE: %s\n", t.nome); break;
            case TOKEN_PIU:          printf("OPERATORE: +\n"); break;
            case TOKEN_MENO:         printf("OPERATORE: -\n"); break;
            case TOKEN_PER:          printf("OPERATORE: *\n"); break;
            case TOKEN_DIVISO:       printf("OPERATORE: /\n"); break;
            case TOKEN_POTENZA:      printf("OPERATORE: ^\n"); break;
            case TOKEN_PAREN_APERTA: printf("PARENTESI: (\n"); break;
            case TOKEN_PAREN_CHIUSA: printf("PARENTESI: )\n"); break;
            default:                 printf("SCONOSCIUTO\n"); break;
        }
    }
    printf("----------------------\n");
}

