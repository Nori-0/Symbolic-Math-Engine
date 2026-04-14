#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "solver.h"

#define MAX_ITERAZIONI 100
#define TOLLERANZA 1e-6

void trova_zero_newton(NodoAST* radice, NodoAST* radice_d, TabellaSimboli* tabella, const char* var_nome) {
    char input[64];
    double x_corrente = 0.0;

    printf("\n--- CACCIATORE DI ZERI (Newton-Raphson) ---\n");
    printf("Inserisci un punto di partenza (ipotesi iniziale) per %s (es. 1.0): ", var_nome);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        x_corrente = strtod(input, NULL);
    }

    int indice_var = -1;
    for (int i = 0; i < tabella->conteggio; i++) {
        if (strcmp(tabella->array[i].nome, var_nome) == 0) {
            indice_var = i;
            break;
        }
    }
    if (indice_var == -1) return;

    double valore_originale = tabella->array[indice_var].valore;
    int iterazione = 0;
    double f_x, f_primo_x, x_successivo;

    printf("\nRicerca dello zero in corso...\n");

    while (iterazione < MAX_ITERAZIONI) {
        tabella->array[indice_var].valore = x_corrente;
        f_x = valuta_albero(radice, tabella);
        f_primo_x = valuta_albero(radice_d, tabella);

        if (fabs(f_primo_x) < 1e-12) {
            printf("[ERRORE] La derivata e' zero (tangente orizzontale) in %s = %g. Il metodo fallisce qui.\n", var_nome, x_corrente);
            break;
        }

        x_successivo = x_corrente - (f_x / f_primo_x);

        printf("Iterazione %2d: %s = %10.6f | f(%s) = %10.6f\n", iterazione + 1, var_nome, x_successivo, var_nome, f_x);

        if (fabs(x_successivo - x_corrente) < TOLLERANZA && fabs(f_x) < TOLLERANZA) {
            printf("\n[SUCCESSO] Zero trovato in un lampo!\n");
            printf("La curva taglia l'asse X esattamente nel punto: %s = %g\n", var_nome, x_successivo);
            tabella->array[indice_var].valore = valore_originale;
            return;
        }
        x_corrente = x_successivo;
        iterazione++;
    }

    printf("\n[ATTENZIONE] Il metodo non ha raggiunto la convergenza. Prova con un punto di partenza diverso.\n");
    tabella->array[indice_var].valore = valore_originale;
}
