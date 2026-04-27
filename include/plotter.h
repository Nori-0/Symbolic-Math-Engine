#ifndef PLOTTER_H
#define PLOTTER_H

#include "ast.h"
#include "math_engine.h"

#define PLOT_WIDTH 70
#define PLOT_HEIGHT 25


void draw_ascii_graph(NodoAST* root, TabellaSimboli* table, const char* var_name);

#endif
