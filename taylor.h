#ifndef TAYLOR_H
#define TAYLOR_H

#include "ast.h"
#include "math_engine.h"

NodoAST* serie_di_taylor(NodoAST* radice, const char* var_nome, int grado, TabellaSimboli* tabella);

#endif
