#include "tabela_slr_def.h"

#ifndef SINTATICO_H
#define SINTATICO_H

#define ACEITA 'A'
#define EMPILHA 'S'
#define NEUTRO ' '
#define REDUZ 'R'

int parse(char *exp, char *lex, char *tk_rec);
slr_cel * cel(char op, int estado, int qtd_desempilhar);
slr_busca * busca_cel(slr_tabela *tabela, int estado, int tk);

#endif