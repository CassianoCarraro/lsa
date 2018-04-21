#ifndef LISTA_H
#define LISTA_H

struct simbolo {
	char id[TAMANHO_LEX];
	int tipo;
} simb;

struct nodoLista {
	struct simbolo *simb;
	struct nodoLista *prox;
} elementoLista;

void array_push(struct nodoLista **lista, struct simbolo *simb);
int in_array(struct nodoLista **inicio, struct simbolo simb);

#endif