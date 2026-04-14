#ifndef SOLVER_H
#define SOLVER_H

#include "ast.h"
#include "math_engine.h"

void trova_zero_newton(NodoAST* radice, NodoAST* radice_d, TabellaSimboli* tabella, const char* var_nome);

#endif
