#include <stdio.h>
#include <stdlib.h>

#include "pilha.h"

void push(struct nodo **topo, int valor){
	struct nodo *novo;
	struct nodo *atual = *topo;

	novo = (struct nodo*) malloc(sizeof(elemento));
	novo->dado = valor;
	novo->ant = NULL;

	if(*topo == NULL) {
		*topo = novo;
	} else {
		novo->ant = *topo;
		*topo = novo;
	}
}

struct nodo* pop(struct nodo **topo) {
	struct nodo *aux = *topo;
	if (*topo != NULL) {
		*topo = (*topo)->ant;
		aux->ant = NULL;
	}

	return aux;
}

struct nodo* peek(struct nodo **topo) {
	struct nodo* aux;
	aux = NULL;

	if (*topo != NULL) {
		aux = (struct nodo*) malloc(sizeof(struct nodo));
		aux->dado = (*topo)->dado;
		aux->ant = (*topo)->ant;
	}

	return aux;
}

void mostraPilha(struct nodo *pilha){
	struct nodo *aux = pilha;

	if(aux == NULL){
		printf("Pilha Vazia\n");
	} else {
		while(aux != NULL) {
			printf("Elemento: %d \n", aux->dado);
			aux=aux->ant;
		}
	}
}