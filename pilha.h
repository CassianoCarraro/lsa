#ifndef PILHA_H
#define PILHA_H

struct nodo {
	int dado;
	struct nodo *ant;
} elemento;

void push(struct nodo **topo, int valor);
struct nodo* pop(struct nodo **topo);
struct nodo* peek(struct nodo **topo);

#endif