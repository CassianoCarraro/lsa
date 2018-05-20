#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "lexico.h"
#include "lista.h"
#include "lista.c"
#include "pilha.h"

struct nodoLista *lista;
struct nodo *pilhaLex;

void tipo(struct param **sintetizado, struct param paramAux) {
	if (*sintetizado != NULL) {
		free(*sintetizado);
	}

	*sintetizado = (struct param *)malloc(sizeof(param));
	(*sintetizado)->tk = paramAux.tk;

	paramAux.lex = (char *)malloc(sizeof(char) * 2);
	strcpy(paramAux.lex, "0");
	setConst(sintetizado, paramAux);
}

void setLex(struct param **sintetizado, struct param paramAux) {
	if (paramAux.op == 'S') {
		struct nodo *nodo = peek(&pilhaLex);

		union dadoNodo dado;
		dado._string = (char *)malloc(sizeof(char) * TAMANHO_LEX);
		strcpy(dado._string, paramAux.lex);

		//printf("EMPILHOU: %s\n", dado._string);

		push(&pilhaLex, dado);
	}
}

void setConst(struct param **sintetizado, struct param paramAux) {
	union valor *v = (union valor *)malloc(sizeof(valor));

	switch ((*sintetizado)->tk) {
		case TK_INT:
			v->i = atoi(paramAux.lex);
			break;
		case TK_FLOAT:
			v->f = atof(paramAux.lex);
			break;
		case TK_DOUBLE:
			v->d = atof(paramAux.lex);
			break;
	}

	(*sintetizado)->valor = v;
}

void definiciaoVariavelId(struct param *herdado) {
	//mostraPilha(pilhaLex);

	struct nodo *nodo = pop(&pilhaLex);
	addTabSim(nodo->dado._string, herdado->tk);

	printf("%s := ", nodo->dado._string);

	switch (herdado->tk) {
		case TK_INT:
			printf("%d", herdado->valor->i);
			break;
		case TK_FLOAT:
			printf("%.9g", herdado->valor->f);
			break;
		case TK_DOUBLE:
			printf("%.17g", herdado->valor->d);
			break;
	}

	printf("\n");
}

void expressaoPrimaria(struct param *herdado) {
	pop(&pilhaLex);
}

void comandoExpressao(struct param *herdado) {
	pop(&pilhaLex);
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

void expressaoUnariaIncrDir(struct param **sintetizado, struct param paramAux) {
	struct nodo *nodo = pop(&pilhaLex);
	printf("%s := %s + 1\n", nodo->dado._string, nodo->dado._string, 1);
}