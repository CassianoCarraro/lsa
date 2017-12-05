#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tabela_slr_def.h"
#include "sintatico.h"
#include "lexico.h"
#include "lexico.c"
#include "pilha.h"
#include "pilha.c"

slr_cel * cel(char op, int estado, int qtd_desempilhar) {
	slr_cel *cel = (struct slr_cel*) malloc(sizeof(slr_cel));
	cel->op = op;
	cel->estado = estado;
	cel->qtd_desempilhar = qtd_desempilhar;

	return cel;
}

slr_busca * busca_cel(slr_tabela tabela, int estado, int tk) {
	slr_busca *busca = (struct slr_busca*) malloc(sizeof(slr_busca));
	int col, j;
	gramatica_elem g_elem;

	for (col = 0; col < QTD_ELEM_GRAMATICA + 1; col++) {
		g_elem = tabela.gramatica[col];

		for (j = 0; j < QTD_MAX_PROD + 1; j++) {
			if (g_elem.producoes[j] == tk) {
				busca->tk = &g_elem;
				busca->cel = tabela.slr_estados[estado][col];

				return busca;
			}
		}
	}

	return NULL;
}

int reduz(struct nodo **pilha, slr_tabela tabela, int *estado, int tk, int tk_gram, char op, int qtd_desempilhar) {
	slr_busca *busca = NULL;
	int i;
	int reconhece = 1;

	if (op == NEUTRO) {
		busca = busca_cel(tabela, *estado, tk);
	} else {
		for (i = qtd_desempilhar; i > 0; i--) {
			pop(pilha);
		}

		struct nodo * e = peek(pilha);
		busca = busca_cel(tabela, e->dado, (tk_gram == 0 ? tk : tk_gram));
	}

	if (busca != NULL && busca->cel != NULL) {
		if (busca->cel->op == EMPILHA) {
			push(pilha, busca->cel->estado);
			*estado = busca->cel->estado;
		} else if (busca->cel->op == REDUZ) {
			reconhece = reduz(pilha, tabela, estado, tk, -busca->cel->estado, busca->cel->op, busca->cel->qtd_desempilhar);
		} else if (busca->cel->op == NEUTRO) {
			push(pilha, busca->cel->estado);
			*estado = busca->cel->estado;
			reconhece = reduz(pilha, tabela, estado, tk, -busca->cel->estado, busca->cel->op, busca->cel->qtd_desempilhar);
		}
	} else {
		reconhece = 0;
	}

	return reconhece;
}

int parse(char *exp, char *lex, char *tk_rec) {
	slr_tabela tabela = {{GRAMATICA}, {TABELA_SLR}};

	struct nodo *pilha = NULL;
	slr_busca *busca = NULL;
	int estado = 0;
	int reconhece = 0;

	char token_desc[53][17] = {TOKENS};
	push(&pilha, 0);

	while((tk = le_token(exp, lex)) > 0) {
		strcpy(tk_rec, token_desc[tk-1]);

		busca = busca_cel(tabela, estado, tk);
		if (busca != NULL && busca->cel != NULL) {
			if (busca->cel->op == EMPILHA) {
				push(&pilha, busca->cel->estado);
				estado = busca->cel->estado;
			} else if (busca->cel->op == REDUZ) {
				reconhece = reduz(&pilha, tabela, &estado, tk, -busca->cel->estado, busca->cel->op, busca->cel->qtd_desempilhar);
			}
		} else {
			reconhece = 0;
			break;
		}
	}

	if (tk == -1) {
		reconhece = -1;
	} else {
		if (reconhece) {
			busca = busca_cel(tabela, estado, TK_FIM_SENTENCA);
			if (busca != NULL && busca->cel != NULL) {
				reconhece = reduz(&pilha, tabela, &estado, TK_FIM_SENTENCA, -busca->cel->estado, busca->cel->op, busca->cel->qtd_desempilhar);
			} else {
				reconhece = 0;
			}
		} else {
			reconhece = 0;
		}
	}

	return reconhece;
}