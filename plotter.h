#ifndef PLOTTER_H
#define PLOTTER_H

#include "ast.h"
#include "math_engine.h"

#define PLOT_WIDTH 70
#define PLOT_HEIGHT 25


void disegna_grafico_ascii(NodoAST* radice, TabellaSimboli* tabella, const char* var_nome);

#endif
