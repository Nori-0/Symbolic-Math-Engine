#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "math_engine.h"

int alberi_uguali(NodoAST* a, NodoAST* b) {
    if (a == NULL && b == NULL) return 1;
    if (a == NULL || b == NULL) return 0;
    if (a->tipo != b->tipo) return 0;
    if (a->tipo == NODO_NUMERO) {
        return (a->valore_numero.re == b->valore_numero.re && a->valore_numero.im == b->valore_numero.im);
    }
    if (a->tipo == NODO_VARIABILE) {
        return strcmp(a->nome, b->nome) == 0;
    }
    if (a->tipo == NODO_FUNZIONE) {
        return strcmp(a->nome, b->nome) == 0 && alberi_uguali(a->sinistro, b->sinistro);
    }
    if (a->tipo == NODO_OPERATORE) {
        return a->operatore == b->operatore && 
               alberi_uguali(a->sinistro, b->sinistro) && 
               alberi_uguali(a->destro, b->destro);
    }

    return 0;
}

int peso_nodo(NodoAST* nodo) {
	if (nodo == NULL) return 0;
	if (nodo->tipo == NODO_NUMERO) return 1;
	if (nodo->tipo == NODO_VARIABILE) return 2;
	if (nodo->tipo == NODO_FUNZIONE) return 3;
	if (nodo->tipo == NODO_OPERATORE) return 4;
	return 5;
}

NodoAST* deriva(NodoAST* radice, const char* var) {
	if (radice == NULL) return NULL;
	if (radice->tipo == NODO_NUMERO) {
		return crea_nodo_numero(complex_create(0.0, 0.0));
	}
	if (radice->tipo == NODO_VARIABILE) {
		if(strcmp(radice->nome, var) == 0) {
			return crea_nodo_numero(complex_create(1.0, 0.0));
		} else {
			return crea_nodo_numero(complex_create(0.0, 0.0));
		}
	}

	if (radice->tipo == NODO_OPERATORE) {
		if (radice->operatore == '+' || radice->operatore == '-') {
			NodoAST* u_primo = deriva(radice->sinistro, var);
			NodoAST* v_primo = deriva(radice->destro, var);
			return crea_nodo_operatore(radice->operatore, u_primo, v_primo);
		}
		if (radice->operatore == '*') {
			NodoAST* u_primo = deriva(radice->sinistro, var);
			NodoAST* v_copia = copia_albero(radice->destro);
			NodoAST* pezzo_sx = crea_nodo_operatore('*', u_primo, v_copia);
			NodoAST* u_copia = copia_albero(radice->sinistro);
			NodoAST* v_primo = deriva(radice->destro, var);
			NodoAST* pezzo_dx = crea_nodo_operatore('*', u_copia, v_primo);

			return crea_nodo_operatore('+', pezzo_sx, pezzo_dx);
		}
		if (radice->operatore == '^') {
			if (radice->destro->tipo == NODO_NUMERO) {
				double n = radice->destro->valore_numero.re;
				NodoAST* n_nodo = crea_nodo_numero(complex_create(n, 0.0));
				NodoAST* n_meno_uno = crea_nodo_numero(complex_create(n - 1.0, 0.0));
				NodoAST* u_copia = copia_albero(radice->sinistro);
				NodoAST* nuova_potenza = crea_nodo_operatore('^', u_copia, n_meno_uno);
				NodoAST* nx_potenza = crea_nodo_operatore('*', n_nodo, nuova_potenza);
				NodoAST* u_primo = deriva(radice->sinistro, var);
				return crea_nodo_operatore('*', nx_potenza, u_primo);
			}
			else {
				NodoAST* u_alla_v = copia_albero(radice);
				NodoAST* u_copia1 = copia_albero(radice->sinistro);
				NodoAST* u_copia2 = copia_albero(radice->sinistro);
				NodoAST* v_copia  = copia_albero(radice->destro);
				NodoAST* u_primo = deriva(radice->sinistro, var);
				NodoAST* v_primo = deriva(radice->destro, var);
				NodoAST* ln_u = crea_nodo_funzione("log", u_copia1);
				NodoAST* pezzo_sx = crea_nodo_operatore('*', v_primo, ln_u);
				NodoAST* v_per_uprimo = crea_nodo_operatore('*', v_copia, u_primo);
				NodoAST* pezzo_dx = crea_nodo_operatore('/', v_per_uprimo, u_copia2);
				NodoAST* somma = crea_nodo_operatore('+', pezzo_sx, pezzo_dx);
				return crea_nodo_operatore('*', u_alla_v, somma);
			}
		}
		if (radice->operatore == '/'){
			NodoAST* u_primo = deriva(radice->sinistro, var);
			NodoAST* v_copia1 = copia_albero(radice->destro);
			NodoAST* num_sx = crea_nodo_operatore('*', u_primo, v_copia1);

			NodoAST* u_copia = copia_albero(radice->sinistro);
			NodoAST* v_primo = deriva(radice->destro, var);
			NodoAST* num_dx = crea_nodo_operatore('*', u_copia, v_primo);

			NodoAST* numeratore = crea_nodo_operatore('-', num_sx, num_dx);

			NodoAST* v_copia2 = copia_albero(radice->destro);
			NodoAST* due = crea_nodo_numero(complex_create(2.0, 0.0));
			NodoAST* denominatore = crea_nodo_operatore('^', v_copia2, due);

			return crea_nodo_operatore('/', numeratore, denominatore);
		}
	}

		if (radice->tipo == NODO_FUNZIONE) {
			NodoAST* u_primo = deriva(radice->sinistro, var);
			NodoAST* u_copia = copia_albero(radice->sinistro);

			if (strcmp(radice->nome, "sin") == 0) {
				NodoAST* cos_u = crea_nodo_funzione("cos", u_copia);
				return crea_nodo_operatore('*', cos_u, u_primo);
			}

			if (strcmp(radice->nome, "cos") == 0) {
				NodoAST* meno_uno = crea_nodo_numero(complex_create(-1.0, 0.0));
				NodoAST* sin_u = crea_nodo_funzione("sin", u_copia);
				NodoAST* meno_sin_u = crea_nodo_operatore('*', meno_uno, sin_u);
				return crea_nodo_operatore('*', meno_sin_u, u_primo);
			}

			if (strcmp(radice->nome, "log") == 0) {
				NodoAST* uno = crea_nodo_numero(complex_create(1.0, 0.0));
				NodoAST* uno_su_u = crea_nodo_operatore('/', uno, u_copia);
				return crea_nodo_operatore('*', uno_su_u, u_primo);
			}

			if (strcmp(radice->nome, "sqrt") == 0) {
				NodoAST* due = crea_nodo_numero(complex_create(2.0, 0.0));
				NodoAST* sqrt_u = crea_nodo_funzione("sqrt", u_copia);
				NodoAST* denominatore = crea_nodo_operatore('*', due, sqrt_u);
				return crea_nodo_operatore('/', u_primo, denominatore);
			}

			if (strcmp(radice->nome, "exp") == 0) {
				NodoAST* exp_u = crea_nodo_funzione("exp", u_copia);
				return crea_nodo_operatore('*', exp_u, u_primo);
			}

			if (strcmp(radice->nome, "tan") == 0) {
				NodoAST* cos_u = crea_nodo_funzione("cos", u_copia);
				NodoAST* due = crea_nodo_numero(complex_create(2.0, 0.0));
				NodoAST* cos_quadro = crea_nodo_operatore('^', cos_u, due);
				return crea_nodo_operatore('/', u_primo, cos_quadro);
			}
		}

	printf("[WARNING] Differentiation rule not yet implemented for this node.\n");
	return crea_nodo_numero(complex_create(0.0, 0.0));
}

NodoAST* integra(NodoAST* radice, const char* var) {
	if (radice == NULL) return NULL;
	if (radice->tipo == NODO_NUMERO || (radice->tipo == NODO_VARIABILE && strcmp(radice->nome, var) != 0)) {
		if (radice->tipo == NODO_NUMERO && complex_is_zero(radice->valore_numero)) {
			return crea_nodo_numero(complex_create(0.0, 0.0));
		}
		NodoAST* c_copia = copia_albero(radice);
		NodoAST* x_nodo = crea_nodo_variabile(var);
		return crea_nodo_operatore('*', c_copia, x_nodo);
	}
	if (radice->tipo == NODO_VARIABILE && strcmp(radice->nome, var) == 0) {
		NodoAST* x_copia = copia_albero(radice);
		NodoAST* due_esponente = crea_nodo_numero(complex_create(2.0, 0.0));
		NodoAST* x_quadro = crea_nodo_operatore('^', x_copia, due_esponente);
		NodoAST* due_divisore = crea_nodo_numero(complex_create(2.0, 0.0));
		return crea_nodo_operatore('/', x_quadro, due_divisore);
	}
	if (radice->tipo == NODO_OPERATORE) {
		if (radice->operatore == '+' || radice->operatore == '-') {
			NodoAST* int_sx = integra(radice->sinistro, var);
			NodoAST* int_dx = integra(radice->destro, var);
			return crea_nodo_operatore(radice->operatore, int_sx, int_dx);
		}
		if (radice->operatore == '*') {
			if (radice->sinistro->tipo == NODO_NUMERO) {
				NodoAST* c_copia = copia_albero(radice->sinistro);
				NodoAST* int_dx = integra(radice->destro, var);
				return crea_nodo_operatore('*', c_copia, int_dx);
			}
		}
		if (radice->operatore == '^') {
			if (radice->sinistro->tipo == NODO_VARIABILE && strcmp(radice->sinistro->nome, var) == 0 && radice->destro->tipo == NODO_NUMERO) {
				double n = radice->destro->valore_numero.re;
				if (n == -1.0) {
					NodoAST* x_copia = copia_albero(radice->sinistro);
					return crea_nodo_funzione("log", x_copia);
				} else {
					NodoAST* x_copia = copia_albero(radice->sinistro);
					NodoAST* n_piu_uno = crea_nodo_numero(complex_create(n + 1.0, 0.0));
					NodoAST* n_piu_uno_div = crea_nodo_numero(complex_create(n + 1.0, 0.0));
					NodoAST* nuova_potenza = crea_nodo_operatore('^', x_copia, n_piu_uno);
					return crea_nodo_operatore('/', nuova_potenza, n_piu_uno_div);
				}
			}
		}
	}
	if (radice->tipo == NODO_FUNZIONE) {
		if (radice->sinistro->tipo == NODO_VARIABILE && strcmp(radice->sinistro->nome, var) == 0) {
			NodoAST* var_copia = copia_albero(radice->sinistro);
			if (strcmp(radice->nome, "sin") == 0) {
				NodoAST* meno_uno = crea_nodo_numero(complex_create(-1.0, 0.0));
				NodoAST* cos_x = crea_nodo_funzione("cos", var_copia);
				return crea_nodo_operatore('*', meno_uno, cos_x);
			}
			if (strcmp(radice->nome, "cos") == 0) {
				return crea_nodo_funzione("sin", var_copia);
			}
			if (strcmp(radice->nome, "exp") == 0) {
				return crea_nodo_funzione("exp", var_copia);
			}
		} else {
			printf("[SYSTEM] Analytical integration of composite functions ('%s(f(x))') is not supported by the base symbolic engine.\n", radice->nome);
			return crea_nodo_numero(complex_create(0.0, 0.0));
		}
	}
	printf("[WARNING] Integration not yet supported for this pattern.\n");
    return crea_nodo_numero(complex_create(0.0, 0.0));
}

NodoAST* semplifica(NodoAST* radice) {
	if (radice == NULL) return NULL;

	radice->sinistro = semplifica(radice->sinistro);
	radice->destro = semplifica(radice->destro);

	if (radice->tipo == NODO_OPERATORE) {
		if (radice->sinistro && radice->destro && radice->sinistro->tipo == NODO_NUMERO && radice->destro->tipo == NODO_NUMERO) {
			Complex sx = radice->sinistro->valore_numero;
			Complex dx = radice->destro->valore_numero;
			Complex risultato = complex_create(0.0, 0.0);
			int calcolo_valido = 1;

			switch (radice->operatore) {
				case '+': risultato = complex_add(sx, dx); break;
				case '-': risultato = complex_sub(sx, dx); break;
				case '*': risultato = complex_mul(sx, dx); break;
				case '/': 
					  if (!complex_is_zero(dx)) risultato = complex_div(sx, dx); 
					  else calcolo_valido = 0; 
					  break;
				case '^': risultato = complex_pow(sx, dx); break;
				default: calcolo_valido = 0; break;
			}

			if (calcolo_valido) {
				radice->tipo = NODO_NUMERO;
				radice->valore_numero = risultato;
				libera_albero(radice->sinistro);
				libera_albero(radice->destro);
				radice->sinistro = NULL;
				radice->destro = NULL;
				return radice;
			}
		}

		if (radice->operatore == '*') {
			if ((radice->sinistro && radice->sinistro->tipo == NODO_NUMERO && complex_is_zero(radice->sinistro->valore_numero)) ||
					(radice->destro && radice->destro->tipo == NODO_NUMERO && complex_is_zero(radice->destro->valore_numero))) {
				return crea_nodo_numero(complex_create(0.0, 0.0));
			}
			if (radice->sinistro && radice->sinistro->tipo == NODO_NUMERO && complex_is_one(radice->sinistro->valore_numero)) {
				return radice->destro;
			}
			if (radice->destro && radice->destro->tipo == NODO_NUMERO && complex_is_one(radice->destro->valore_numero)) {
				return radice->sinistro;
			}
		}
		if (radice->operatore == '+') {
			if (radice->sinistro && radice->sinistro->tipo == NODO_NUMERO && complex_is_zero(radice->sinistro->valore_numero)) {
				return radice->destro;
			}
			if (radice->destro && radice->destro->tipo == NODO_NUMERO && complex_is_zero(radice->destro->valore_numero)) {
				return radice->sinistro;
			}
		}
		if (radice->operatore == '/') {
			if (alberi_uguali(radice->sinistro, radice->destro)) {
				return crea_nodo_numero(complex_create(1.0, 0.0));
			}
		}
		if (radice->operatore == '^') {
			if (radice->destro && radice->destro->tipo == NODO_NUMERO && complex_is_one(radice->destro->valore_numero)) {
				return radice->sinistro;
			}
			if (radice->destro && radice->destro->tipo == NODO_NUMERO && complex_is_zero(radice->destro->valore_numero)) {
				return crea_nodo_numero(complex_create(1.0, 0.0));
			}
			if (radice->sinistro && radice->sinistro->tipo == NODO_NUMERO && complex_is_zero(radice->sinistro->valore_numero)) {
				return crea_nodo_numero(complex_create(0.0, 0.0));
			}
			if (radice->sinistro && radice->sinistro->tipo == NODO_NUMERO && complex_is_one(radice->sinistro->valore_numero)) {
				return crea_nodo_numero(complex_create(1.0, 0.0));
			}
		}
		if (radice->operatore == '-') {
			if (alberi_uguali(radice->sinistro, radice->destro)) {
				return crea_nodo_numero(complex_create(0.0, 0.0));
			}
			if (radice->destro && radice->destro->tipo == NODO_NUMERO && complex_is_zero(radice->destro->valore_numero)) {
				return radice->sinistro;
			}
			if (radice->sinistro && radice->sinistro->tipo == NODO_NUMERO && complex_is_zero(radice->sinistro->valore_numero)) {
				NodoAST* meno_uno = crea_nodo_numero(complex_create(-1.0, 0.0));
				return crea_nodo_operatore('*', meno_uno, radice->destro);
			}
		}
	}
	return radice;
}

NodoAST* riordina_albero(NodoAST* radice) {
	if (radice == NULL) return NULL;
	radice->sinistro = riordina_albero(radice->sinistro);
	radice->destro = riordina_albero(radice->destro);
	if (radice->tipo == NODO_OPERATORE && (radice->operatore == '+' || radice->operatore == '*')) {
		int peso_sx = peso_nodo(radice->sinistro);
		int peso_dx = peso_nodo(radice->destro);
		if (peso_dx < peso_sx) {
			NodoAST* temp = radice->sinistro;
			radice->sinistro = radice->destro;
			radice->destro = temp;
		}
	}
	return radice;
}

Complex valuta_albero(NodoAST* radice, TabellaSimboli* tabella) {
    if (radice == NULL) return complex_create(0.0, 0.0);
    if (radice->tipo == NODO_NUMERO) {
        return radice->valore_numero;
    }

    if (radice->tipo == NODO_VARIABILE) {
	    for (int i = 0; i < tabella->conteggio; i++) {
		    if (strcmp(tabella->array[i].nome, radice->nome) == 0) {
			    return tabella->array[i].valore;
		    }
	    }
	    printf("[ERROR] Variable undefined: %s\n", radice->nome);
	    return complex_create(0.0, 0.0);
    }

    if (radice->tipo == NODO_OPERATORE) {
        Complex sx = valuta_albero(radice->sinistro, tabella);
        Complex dx = valuta_albero(radice->destro, tabella);

        switch (radice->operatore) {
            case '+': return complex_add(sx, dx);
            case '-': return complex_sub(sx, dx);
	    case '*': return complex_mul(sx, dx);
            case '/': 
                if (complex_is_zero(dx)) {
                    printf("[WARNING] Division by zero!\n");
                    return complex_create(0.0, 0.0); 
                }
                return complex_div(sx, dx);
            case '^': return complex_pow(sx, dx);
            default: return complex_create(0.0, 0.0);
        }
    }
    if (radice->tipo == NODO_FUNZIONE) {
        Complex arg = valuta_albero(radice->sinistro, tabella);

        if (strcmp(radice->nome, "sin") == 0) return complex_sin(arg);
        if (strcmp(radice->nome, "cos") == 0) return complex_cos(arg);
        if (strcmp(radice->nome, "tan") == 0) return complex_tan(arg);
	if (strcmp(radice->nome, "exp") == 0) return complex_exp(arg);
	if (strcmp(radice->nome, "sqrt") == 0) return complex_sqrt(arg);
        if (strcmp(radice->nome, "log") == 0){
		if (complex_is_zero(arg)) {
			printf("[WARNING] Invalid domain: logarithm of zero!\n");

			return complex_create(0.0, 0.0);
		}
		return complex_log(arg);
	}
        
        return complex_create(0.0, 0.0);
    }

    return complex_create(0.0, 0.0);
}

void trova_variabili(NodoAST* radice, TabellaSimboli* tabella) {
	if (radice == NULL) return;

	if (radice->tipo == NODO_VARIABILE) {
		int trovata = 0;
		for (int i = 0; i < tabella->conteggio; i++) {
			if (strcmp(tabella->array[i].nome, radice->nome) == 0) {
				trovata = 1;
				break;
			}
		}
		
		if (!trovata && tabella->conteggio < 50) {
			strcpy(tabella->array[tabella->conteggio].nome, radice->nome);
			tabella->array[tabella->conteggio].valore = complex_create(0.0, 0.0);
			tabella->conteggio++;
		}
	}
	
	if (radice->sinistro) trova_variabili(radice->sinistro, tabella);
	if (radice->destro) trova_variabili(radice->destro, tabella);
}

