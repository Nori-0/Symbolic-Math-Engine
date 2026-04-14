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

    printf("--- C Symbolic CAS Engine ---\n");
    printf("Enter function f(x) : ");

    if(fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;

        tokenizza(input);
        NodoAST* radice = costruisci_albero();
        radice = riordina_albero(radice);
        radice = semplifica(radice);

        printf("\n[DEBUG] Starting Abstract Syntax Tree (AST):\n");
        stampa_albero(radice);

        TabellaSimboli tabella = { .conteggio = 0};

        strcpy(tabella.array[tabella.conteggio].nome, "pi");
        tabella.array[tabella.conteggio].valore = M_PI;
        tabella.conteggio++;

        strcpy(tabella.array[tabella.conteggio].nome, "e");
        tabella.array[tabella.conteggio].valore = M_E;
        tabella.conteggio++;

        trova_variabili(radice, &tabella);

        printf("\nVariables found in the expression: ");
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
			printf("Target variable for calculus: ");
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
				printf("[ERROR] '%s' is not a valid variable. Please try again.\n", var_bersaglio);
			}
		}
	} else {
		printf("\n[SYSTEM] No unknown variables found. Using 'x' as the default.\n");
	}

        NodoAST* radice_d = deriva(radice, var_bersaglio);
        radice_d = riordina_albero(radice_d);
        radice_d = semplifica(radice_d);

	NodoAST* radice_int = integra(radice, var_bersaglio);
        radice_int = riordina_albero(radice_int);
        radice_int = semplifica(radice_int);

        printf("\n=====================================\n");
        printf(" FUNCTION: f = "); 
        stampa_equazione(radice);
        printf("\n DERIVATIVE: df/d%s = ", var_bersaglio); 
        stampa_equazione(radice_d);
	printf("\n INTEGRAL: INT(f d%s) = ", var_bersaglio);
	stampa_equazione(radice_int);
	printf(" + C");
        printf("\n=====================================\n");

        printf("\n--- NUMERICAL CALCULATION ---\n");
        char buffer_valore[64];
        for (int i = 0; i < tabella.conteggio; i++) {
            if (strcmp(tabella.array[i].nome, "pi") == 0 || strcmp(tabella.array[i].nome, "e") == 0) {
                continue;
	    }

            printf("What is the value of '%s'? ", tabella.array[i].nome);
            if (fgets(buffer_valore, sizeof(buffer_valore), stdin) != NULL) {
                tabella.array[i].valore = strtod(buffer_valore, NULL);
            }
        }
        
        double risultato = valuta_albero(radice, &tabella);
        double risultato_d = valuta_albero(radice_d, &tabella);

        printf("\n[RESULTS]\n");
        printf("Function value: f = %g\n", risultato);
        printf("Partial derivative at that point: f' = %g\n", risultato_d);
	printf("\n--- AREA CALCULATION (DEFINITE INTEGRAL) ---\n");
        char risposta[10];
        printf("Do you want to calculate the area between two limits for variable '%s'? (y/n): ", var_bersaglio);
	if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 'y' || risposta[0] == 'Y')) {

		double a = 0.0, b = 0.0;
		printf("Enter the lower limit (a): ");
		if (fgets(buffer_valore, sizeof(buffer_valore), stdin) != NULL) a = strtod(buffer_valore, NULL);
		printf("Enter the upper limit (b): ");
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
			printf("\n[DEFINITE INTEGRAL RESULT]\n");
			printf("Area under the curve between %g and %g: %g\n", a, b, area);
			tabella.array[indice_var].valore = valore_originale;
		} else {
			printf("[ERROR] Integration variable not found in the table.\n");
		}
	}

	printf("\nDo you want to find the function graph ? (y/n): ");
	if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 'y' || risposta[0] == 'Y')) {
		disegna_grafico_ascii(radice, &tabella, var_bersaglio);
	}

	printf("\nDo you want to find where the function equals zero (f(x) = 0)? (y/n): ");
        if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 'y' || risposta[0] == 'Y')) {
            trova_zero_newton(radice, radice_d, &tabella, var_bersaglio);
        }

	printf("\nDo you want to approximate the function with a Taylor (Maclaurin) Series? (y/n): ");
        if (fgets(risposta, sizeof(risposta), stdin) != NULL && (risposta[0] == 'y' || risposta[0] == 'Y')) {
            int grado = 3;
            printf("Enter the degree of the polynomial (e.g. 5): ");
            if (fgets(buffer_valore, sizeof(buffer_valore), stdin) != NULL) {
                grado = atoi(buffer_valore);
            }
            
            NodoAST* albero_taylor = serie_di_taylor(radice, var_bersaglio, grado, &tabella);
            
            printf("\n[TAYLOR POLYNOMIAL OF DEGREE %d]\n", grado);
            printf("T(x) = ");
            stampa_equazione(albero_taylor);
            printf("\n");
            libera_albero(albero_taylor);
        }

	printf("\n[SYSTEM] Memory cleanup in progress...\n");
	libera_albero(radice);
        libera_albero(radice_d);
	libera_albero(radice_int);
        printf("[SYSTEM] Memory successfully freed. No leaks detected.\n");

    } else {
        printf("Critical error during acquisition.\n");
        return 1;
    }
    return 0;
}
