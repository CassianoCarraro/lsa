#include <stdlib.h>

#include "lista.h"

void array_push(struct nodoLista **inicio, struct simbolo *simb) {
	struct nodoLista *pnovo;

	pnovo = (struct nodoLista*)malloc(sizeof(elementoLista));
	pnovo->simb = simb;
	pnovo->prox = NULL;

	struct nodoLista *atual = *inicio;

	if (*inicio == NULL) {
		*inicio = pnovo;
	} else {
		while(atual->prox != NULL){
			atual = atual->prox;
		}

		atual->prox = pnovo;
	}
}

int in_array(struct nodoLista **inicio, struct simbolo simb) {
	int posicao = 0;
	struct nodoLista *atual = *inicio;

	while(atual != NULL){
		if(strcmp(atual->simb->id, simb.id) == 0){
			return posicao;
		} else {
			posicao++;
		}

		atual = atual->prox;
	}

	return -1;
}