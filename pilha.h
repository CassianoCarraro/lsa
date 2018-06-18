#ifndef PILHA_H
#define PILHA_H

union dadoNodo {
	int _int;
	char *_string;
	double _double;
} dadoNodo;

struct nodo {
	union dadoNodo dado;
	struct nodo *ant;
} elemento;

void push(struct nodo **topo, union dadoNodo dado);
struct nodo* pop(struct nodo **topo);
struct nodo* peek(struct nodo **topo);

#endif