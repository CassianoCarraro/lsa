#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "lexico.h"
#include "lista.h"
#include "lista.c"

struct nodoLista *lista;

void tipo(struct param **sintetizado, struct param paramAux) {
	if (*sintetizado != NULL) {
		free(*sintetizado);
	}

	*sintetizado = (struct param *)malloc(sizeof(param));
	(*sintetizado)->tk = paramAux.tk;
	(*sintetizado)->lex = (char *)malloc(sizeof(char) * TAMANHO_LEX);
}

void setLex(struct param **sintetizado, struct param paramAux) {
	strcpy((*sintetizado)->lex, paramAux.lex);
}

void definiciaoVariavelId(struct param *herdado) {
	addTabSim(herdado->lex, herdado->tk);
}

void addTabSim(char *lex, int tkTipo) {
	struct simbolo *novoSimbolo = (struct simbolo*)malloc(sizeof(simb));

	strcpy(novoSimbolo->id, lex);
	novoSimbolo->tipo = tkTipo;

	if (in_array(&lista, *novoSimbolo) == -1) {
		array_push(&lista, novoSimbolo);
	} else {
		printf("Variável '%s' já definida!\n", lex);
		exit(EXIT_FAILURE);
	}
}