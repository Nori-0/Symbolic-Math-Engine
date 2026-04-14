#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

int precedenza(TipoToken tipo);
NodoAST* costruisci_albero();

#endif // PARSER_H
