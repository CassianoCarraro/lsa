#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "lexico.h"
#include "lista.h"
#include "lista.c"
#include "pilha.h"

struct nodoLista *lista;
struct nodo *pilhaLex;
struct nodo *pilhaPlace;

void empilhaPlace(char * valor) {
	union dadoNodo dado;
	dado._string = valor;
	push(&pilhaPlace, dado);
}

int qtdCaract(int num) {
	int n = 0;

	while (num) {
		num /= 10;
		n++;
	}

	return n;
}

char * itoa(int valor) {
	char * a = (char *)malloc(sizeof(char) * qtdCaract(valor));
	sprintf(a, "%d", valor);

	return a;
}

char * dtoa(double valor) {
	char * a = (char *)malloc(sizeof(char) * 20);
	sprintf(a, "%lf", valor);

	return a;
}

char * geraTemp() {
	static int idTemp = 1;
	char * temp = (char *)malloc(sizeof(char) * (qtdCaract(idTemp) + 1));
	sprintf(temp, "T%d", idTemp++);

	return temp;
}

char * geraLabel() {
	static int idLabel = 1;
	char * label = (char *)malloc(sizeof(char) * (qtdCaract(idLabel) + 2));
	sprintf(label, "L%d:", idLabel++);

	return label;
}

void geraCodExpressao(char op) {
	struct nodo *y = pop(&pilhaPlace);
	struct nodo *x = pop(&pilhaPlace);

	if (x != NULL && y != NULL) {
		char * temp = geraTemp();
		empilhaPlace(temp);
		fprintf(arqout, "%s := %s %c %s\n", temp, x->dado._string, op, y->dado._string);
	}
}


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

	if (pilhaPlace == NULL) {
		fprintf(arqout, "%s := ", nodo->dado._string);

		switch (herdado->tk) {
			case TK_INT:
				fprintf(arqout, "%d", herdado->valor->i);
				break;
			case TK_FLOAT:
				fprintf(arqout, "%.9g", herdado->valor->f);
				break;
			case TK_DOUBLE:
				fprintf(arqout, "%.17g", herdado->valor->d);
				break;
		}
	} else {
		struct nodo *temp = pop(&pilhaPlace);
		fprintf(arqout, "%s := %s", nodo->dado._string, temp->dado._string);
	}

	fprintf(arqout, "\n");
}

void expressaoPrimaria(struct param *herdado) {
	struct nodo *nodo = pop(&pilhaLex);
	empilhaPlace(nodo->dado._string);
}

void expressaoPrimariaConst(struct param *herdado) {
	switch (herdado->tk) {
		case TK_INT:
			empilhaPlace(itoa(herdado->valor->i));
			break;
		case TK_FLOAT:
			empilhaPlace(dtoa(herdado->valor->f));
			break;
		case TK_DOUBLE:
			empilhaPlace(dtoa(herdado->valor->d));
			break;
	}
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
	fprintf(arqout, "%s := %s + 1\n", nodo->dado._string, nodo->dado._string);
}

void expressaoUnariaDecrDir(struct param **sintetizado, struct param paramAux) {
	struct nodo *nodo = pop(&pilhaLex);
	fprintf(arqout, "%s := %s - 1\n", nodo->dado._string, nodo->dado._string);
}

void expressaoSoma(struct param **sintetizado, struct param paramAux) {
	geraCodExpressao('+');
}

void expressaoSub(struct param **sintetizado, struct param paramAux) {
	geraCodExpressao('-');
}

void expressaoMult(struct param **sintetizado, struct param paramAux) {
	geraCodExpressao('*');
}

void expressaoDiv(struct param **sintetizado, struct param paramAux) {
	geraCodExpressao('/');
}

void expressaoResto(struct param **sintetizado, struct param paramAux) {
	geraCodExpressao('%');
}