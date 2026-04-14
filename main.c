#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "math_engine.h"
#include "plotter.h"
#include "solver.h"
#include "taylor.h"

int main() {
    char input[MAX_INPUT_SIZE];

    printf("--- Calcolatore CAS in C ---\n");
    printf("Inserisci la funzione f (max %d caratteri): ", MAX_INPUT_SIZE - 1);

    if(fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;

        tokenizza(input);
        NodoAST* radice = costruisci_albero();
        radice = riordina_albero(radice);
        radice = semplifica(radice);

        printf("\n[DEBUG] Albero Sintattico Astratto (AST) di partenza:\n");
        stampa_albero(radice);

        TabellaSimboli tabella = { .conteggio = 0};

        strcpy(tabella.array[tabella.conteggio].nome, "pi");
        tabella.array[tabella.conteggio].valore = M_PI;
        tabella.conteggio++;

        strcpy(tabella.array[tabella.conteggio].nome, "e");
        tabella.array[tabella.conteggio].valore = M_E;
        tabella.conteggio++;

        trova_variabili(radice, &tabella);

        printf("\nVariabili trovate nell'espressione: ");
        for(int i=0; i < tabella.conteggio; i++) {
            if (strcmp(tabella.array[i].nome, "pi") != 0 && strcmp(tabella.array[i].nome, "e") != 0)
                printf("[%s] ", tabella.array[i].nome);
        }
        printf("\n");

        char var_bersaglio[10] = "x";

	int num_variabili_utente = 0;
        for (int i = 0; i < tabella.conteggio; i++) {
		if (strcmp(tabella.array[i].nome, "pi") != 0 && strcmp(tabella.array[i].nome, "e") != 0) {
			num_variabili_utente++;
		}
	}
	if (num_variabili_utente > 0) {
		int variabile_valida = 0;
		while (!variabile_valida) {
			printf("Rispetto a quale variabile vuoi operare? ");
			if (fgets(var_bersaglio, sizeof(var_bersaglio), stdin) != NULL) {
				var_bersaglio[strcspn(var_bersaglio, "\n")] = 0;
			}
			for (int i = 0; i < tabella.conteggio; i++) {
				if (strcmp(tabella.array[i].nome, var_bersaglio) == 0 &&
						strcmp(var_bersaglio, "pi") != 0 && strcmp(var_bersaglio, "e") != 0) {
					variabile_valida = 1;
					break;
				}
			}
			if (!variabile_valida) {
				printf("[ERRORE] '%s' non e' tra le variabili valide. Riprova.\n", var_bersaglio);
			}
		}
	} else {
		printf("\n[SISTEMA] Nessuna variabile incognita trovata. Uso 'x' come default.\n");
	}

        NodoAST* radice_d = deriva(radice, var_bersaglio);
        radice_d = riordina_albero(radice_d);
        radice_d = semplifica(radice_d);

	NodoAST* radice_int = integra(radice, var_bersaglio);
        radice_int = riordina_albero(radice_int);
        radice_int = semplifica(radice_int);

        printf("\n=====================================\n");
        printf(" FUNZIONE: f = "); 
        stampa_equazione(radice);
        printf("\n DERIVATA: df/d%s = ", var_bersaglio); 
        stampa_equazione(radice_d);
	printf("\n INTEGRALE: INT(f d%s) = ", var_bersaglio);
	stampa_equazione(radice_int);
	printf(" + C");
        printf("\n=====================================\n");

        printf("\n--- CALCOLO NUMERICO ---\n");
        char buffer_valore[64];
        for (int i = 0; i < tabella.conteggio; i++) {
            if (strcmp(tabella.array[i].nome, "pi") == 0 || strcmp(tabella.array[i].nome, "e") == 0) {
                continue;
	    }

            printf("Quanto vale '%s'? ", tabella.array[i].nome);
            if (fgets(buffer_valore, sizeof(buffer_valore), stdin) != NULL) {
                tabella.array[i].valore = strtod(buffer_valore, NULL);
            }
        }
        
        double risultato = valuta_albero(radice, &tabella);
        double risultato_d = valuta_albero(radice_d, &tabella);

        printf("\n[RISULTATI]\n");
        printf("Valore della funzione: f = %g\n", risultato);
        printf("Derivata parziale in quel punto: f' = %g\n", risultato_d);
	printf("\n--- CALCOLO AREA (INTEGRALE DEFINITO) ---\n");
        char risposta[10];
        printf("Vuoi calcolare l'area tra due estremi per la variabile '%s'? (s/n): ", var_bersaglio);
	if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 's' || risposta[0] == 'S')) {

		double a = 0.0, b = 0.0;
		printf("Inserisci l'estremo inferiore (a): ");
		if (fgets(buffer_valore, sizeof(buffer_valore), stdin) != NULL) a = strtod(buffer_valore, NULL);
		printf("Inserisci l'estremo superiore (b): ");
		if (fgets(buffer_valore, sizeof(buffer_valore), stdin) != NULL) b = strtod(buffer_valore, NULL);
		int indice_var = -1;
		for (int i = 0; i < tabella.conteggio; i++) {
			if (strcmp(tabella.array[i].nome, var_bersaglio) == 0) {
				indice_var = i;
				break;
			}
		}
		if (indice_var != -1) {
			double valore_originale = tabella.array[indice_var].valore;
			tabella.array[indice_var].valore = b;
			double f_di_b = valuta_albero(radice_int, &tabella);
			tabella.array[indice_var].valore = a;
			double f_di_a = valuta_albero(radice_int, &tabella);
			double area = f_di_b - f_di_a;
			printf("\n[RISULTATO INTEGRALE DEFINITO]\n");
			printf("Area sotto la curva tra %g e %g: %g\n", a, b, area);
			tabella.array[indice_var].valore = valore_originale;
		} else {
			printf("[ERRORE] Variabile di integrazione non trovata nella tabella.\n");
		}
	}

	printf("\nVuoi visualizzare il grafico della funzione? (s/n): ");
	if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 's' || risposta[0] == 'S')) {
		disegna_grafico_ascii(radice, &tabella, var_bersaglio);
	}

	printf("\nVuoi trovare in quale punto la funzione si azzera (f(x) = 0)? (s/n): ");
        if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 's' || risposta[0] == 'S')) {
            trova_zero_newton(radice, radice_d, &tabella, var_bersaglio);
        }

	printf("\nVuoi approssimare la funzione con una Serie di Taylor (Maclaurin)? (s/n): ");
        if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 's' || risposta[0] == 'S')) {
            int grado = 3;
            printf("Inserisci il grado del polinomio (es. 5): ");
            if (fgets(buffer_valore, sizeof(buffer_valore), stdin) != NULL) {
                grado = atoi(buffer_valore);
            }
            
            NodoAST* albero_taylor = serie_di_taylor(radice, var_bersaglio, grado, &tabella);
            
            printf("\n[POLINOMIO DI TAYLOR GRADO %d]\n", grado);
            printf("T(x) = ");
            stampa_equazione(albero_taylor);
            printf("\n");
            libera_albero(albero_taylor);
        }

	printf("\n[SISTEMA] Pulizia della memoria in corso...\n");
	libera_albero(radice);
        libera_albero(radice_d);
	libera_albero(radice_int);
        printf("[SISTEMA] Memoria liberata con successo. Nessun leak rilevato.\n");

    } else {
        printf("Errore critico in acquisizione input.\n");
        return 1;
    }
    return 0;
}
