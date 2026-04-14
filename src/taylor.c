#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "taylor.h"

double fattoriale(int n) {
	if (n <= 1) return 1.0;
	double res = 1.0;
	for (int i = 2; i <= n; i++) res *= i;
	return res;
}

NodoAST* serie_di_taylor(NodoAST* radice, const char* var_nome, int grado, TabellaSimboli* tabella) {
	if (radice == NULL || grado < 0) return crea_nodo_numero(0.0);

	int indice_var = -1;
	for (int i = 0; i < tabella->conteggio; i++) {
		if (strcmp(tabella->array[i].nome, var_nome) == 0) {
			indice_var = i;
			break;
		}
	}

	if (indice_var == -1) {
		printf("[ERROR] Variable not found for Taylor expansion.\n");
		return crea_nodo_numero(0.0);
	}

	double val_originale = tabella->array[indice_var].valore;
	tabella->array[indice_var].valore = 0.0;

	NodoAST* polinomio_finale = NULL;
	NodoAST* derivata_corrente = copia_albero(radice);

	for (int i = 0; i <= grado; i++) {
        double c_val = valuta_albero(derivata_corrente, tabella);
        if (fabs(c_val) > 1e-9) {
            double coeff = c_val / fattoriale(i);
            NodoAST* termine = NULL;
            if (i == 0) {
                termine = crea_nodo_numero(coeff);
            } else if (i == 1) {
                if (coeff == 1.0) termine = crea_nodo_variabile(var_nome);
                else if (coeff == -1.0) termine = crea_nodo_operatore('*', crea_nodo_numero(-1.0), crea_nodo_variabile(var_nome));
                else termine = crea_nodo_operatore('*', crea_nodo_numero(coeff), crea_nodo_variabile(var_nome));
            } else {
                NodoAST* potenza = crea_nodo_operatore('^', crea_nodo_variabile(var_nome), crea_nodo_numero((double)i));
                if (coeff == 1.0) termine = potenza;
                else if (coeff == -1.0) termine = crea_nodo_operatore('*', crea_nodo_numero(-1.0), potenza);
                else termine = crea_nodo_operatore('*', crea_nodo_numero(coeff), potenza);
            }

            if (polinomio_finale == NULL) {
                polinomio_finale = termine;
            } else {
                polinomio_finale = crea_nodo_operatore('+', polinomio_finale, termine);
            }
        }

        if (i < grado) {
            NodoAST* prossima_derivata = deriva(derivata_corrente, var_nome);
            prossima_derivata = riordina_albero(prossima_derivata);
            prossima_derivata = semplifica(prossima_derivata);

            libera_albero(derivata_corrente);
            derivata_corrente = prossima_derivata;
        }
    }

    libera_albero(derivata_corrente);
    tabella->array[indice_var].valore = val_originale;

    if (polinomio_finale == NULL) return crea_nodo_numero(0.0);
    
    return polinomio_finale;
}

