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
struct nodo *pilhaCod;
struct nodo *pilhaLbCase;
char * fimSwitch;

void gravaCod(char * cod) {
	if (pilhaCod == NULL) {
		fprintf(arqout, "%s\n", cod);
	} else {
		struct nodo *topoCod = peek(&pilhaCod);
		strcat(cod, "\n");
		strcat(topoCod->dado._string, cod);
	}
}

void geraC3E(char * varAtrib, char * var1, char * op, char * var2) {
	char * c3e;

	if (op == NULL && var2 == NULL) {
		c3e = (char *)malloc(sizeof(char) * (strlen(varAtrib) + strlen(var1) + 5));
		sprintf(c3e, "%s := %s", varAtrib, var1);
	} else {
		c3e = (char *)malloc(sizeof(char) * (strlen(varAtrib) + strlen(var1) + strlen(op) + strlen(var2) + 8));
		sprintf(c3e, "%s := %s %s %s", varAtrib, var1, op, var2);
	}

	gravaCod(c3e);
}

void geraC3EControle(int tk, char * cod, char * var1, char * lbFim, char * lbElse, char * lbInicio, char * codTeste) {
	char * c3e;

	switch (tk) {
		case TK_IF:
			if (lbElse == NULL) {
				c3e = (char *)malloc(sizeof(char) * (strlen(var1) + strlen(cod) + strlen(lbFim) * 2 + 21));
				sprintf(c3e, "if %s == false goto %s\n%s%s:", var1, lbFim, cod, lbFim);
			} else {
				c3e = (char *)malloc(sizeof(char) * (strlen(var1) + strlen(cod) + strlen(lbFim) + strlen(lbElse) + 25));
				sprintf(c3e, "if %s == false goto %s\n%sgoto %s", var1, lbElse, cod, lbFim);
			}
			break;
		case TK_ELSE:
			c3e = (char *)malloc(sizeof(char) * (strlen(cod) + strlen(lbFim) + strlen(lbElse) + 4));
			sprintf(c3e, "%s:\n%s%s:", lbElse, cod, lbFim);
			break;
		case TK_WHILE:
			c3e = (char *)malloc(sizeof(char) * (strlen(lbInicio) * 2 + strlen(var1) + strlen(lbFim) * 2 + strlen(cod) + 28));
			sprintf(c3e, "%s:\nif %s == false goto %s\n%sgoto %s\n%s", lbInicio, var1, lbFim, cod, lbInicio, lbFim);

			break;
		case TK_DO:
			c3e = (char *)malloc(sizeof(char) * (strlen(lbInicio) * 2 + strlen(var1) + strlen(lbFim) * 2 + strlen(cod) + 20));
			sprintf(c3e, "%s:\n%sif %s == true goto %s", lbInicio, cod, var1, lbInicio);

			break;
		case TK_SWITCH:
			c3e = (char *)malloc(sizeof(char) * (strlen(lbInicio) * 2 + strlen(cod) + strlen(codTeste) + strlen(lbFim) + 10));
			sprintf(c3e, "goto %s\n%s%s:\n%s%s", lbInicio, cod, lbInicio, codTeste, lbFim);

			break;
		case TK_DEFAULT:
		case TK_CASE:
			c3e = (char *)malloc(sizeof(char) * (strlen(lbInicio) + strlen(cod) + strlen(lbFim) + 8));
			sprintf(c3e, "%s:\n%sgoto %s", lbInicio, cod, lbFim);

			break;
	}

	gravaCod(c3e);
}

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

char * geraLabel(char * posFix) {
	static int idLabel = 1;
	char * label;

	if (posFix != NULL) {
		label = (char *)malloc(sizeof(char) * (strlen(posFix) + qtdCaract(idLabel) + 2));
		sprintf(label, "L%s%d", posFix, idLabel++);
	} else {
		label = (char *)malloc(sizeof(char) * (qtdCaract(idLabel) + 2));
		sprintf(label, "L%d", idLabel++);
	}

	return label;
}

void geraCodExpressao() {
	struct nodo *op = pop(&pilhaPlace);
	struct nodo *y = pop(&pilhaPlace);
	struct nodo *x = pop(&pilhaPlace);

	if (x != NULL && y != NULL) {
		char * temp = geraTemp();
		empilhaPlace(temp);
		geraC3E(temp, x->dado._string, op->dado._string, y->dado._string);
	}
}

void geraCodAtribuicao(char * var, int tkGramAtrib) {
	if (tkGramAtrib == 1) {
		struct nodo *temp = pop(&pilhaPlace);
		geraC3E(var, temp->dado._string, NULL, NULL);
	} else {
		struct nodo *incr = pop(&pilhaPlace);
		struct nodo *op = pop(&pilhaPlace);
		geraC3E(var, var, op->dado._string, incr->dado._string);
	}
}

void tipo(struct param **sintetizado, struct param paramAux) {
	if (*sintetizado != NULL) {
		free(*sintetizado);
	}

	*sintetizado = (struct param *)malloc(sizeof(param));
	(*sintetizado)->tk = paramAux.tk;
	(*sintetizado)->atrib = 0;

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
	struct nodo *nodo = pop(&pilhaLex);
	if (nodo == NULL) {
		return;
	}

	addTabSim(nodo->dado._string, herdado->tk);

	if (pilhaPlace == NULL) {
		char * constTmp = (char *) malloc(sizeof(char) * 25);

		switch (herdado->tk) {
			case TK_INT:
				sprintf(constTmp, "%d", herdado->valor->i);
				break;
			case TK_FLOAT:
				sprintf(constTmp, "%.9g", herdado->valor->f);
				break;
			case TK_DOUBLE:
				sprintf(constTmp, "%.17g", herdado->valor->d);
				break;
		}

		geraC3E(nodo->dado._string, constTmp, NULL, NULL);
	} else {
		geraCodAtribuicao(nodo->dado._string, 1);
	}
}

void definicaoFuncaoCorpo(struct param *herdado) {
	struct nodo *nodoCod = pop(&pilhaCod);
	if (nodoCod != NULL) {
		gravaCod(nodoCod->dado._string);
	}
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

void comandoExpressao(struct param **sintetizado, struct param paramAux) {
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

void inicioBloco(struct param *herdado) {
	union dadoNodo dado;
	dado._string = (char *) malloc(sizeof(char) * 1000000);
	push(&pilhaCod, dado);
}

void expressaoUnariaIncrDir(struct param **sintetizado, struct param paramAux) {
	struct nodo *nodo = pop(&pilhaLex);
	geraC3E(nodo->dado._string, nodo->dado._string, "+", "1");
	empilhaPlace(nodo->dado._string);
}

void expressaoUnariaDecrDir(struct param **sintetizado, struct param paramAux) {
	struct nodo *nodo = pop(&pilhaLex);
	geraC3E(nodo->dado._string, nodo->dado._string, "-", "1");
	empilhaPlace(nodo->dado._string);
}

void expressaoSoma(struct param **sintetizado, struct param paramAux) {
	empilhaPlace("+");
	geraCodExpressao();
}

void expressaoSub(struct param **sintetizado, struct param paramAux) {
	empilhaPlace("-");
	geraCodExpressao();
}

void expressaoMult(struct param **sintetizado, struct param paramAux) {
	empilhaPlace("*");
	geraCodExpressao();
}

void expressaoDiv(struct param **sintetizado, struct param paramAux) {
	empilhaPlace("/");
	geraCodExpressao();
}

void expressaoResto(struct param **sintetizado, struct param paramAux) {
	empilhaPlace("%");
	geraCodExpressao();
}

void expressaoBitBitE(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("&");
	geraCodExpressao();
}

void expressaoBitBitOu(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("|");
	geraCodExpressao();
}

void expressaoBitBitXou(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("^");
	geraCodExpressao();
}

void expressaoBitBitEsq(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("<<");
	geraCodExpressao();
}

void expressaoBitBitDir(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace(">>");
	geraCodExpressao();
}

void expressaoIgualdadeIgual(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("==");
	geraCodExpressao();
}

void expressaoIgualdadeDiferente(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("!=");
	geraCodExpressao();
}

void expressaoRelacionalMaior(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 0;
	empilhaPlace(">");
	geraCodExpressao();
}

void expressaoRelacionalMenor(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("<");
	geraCodExpressao();
}

void expressaoRelacionalMaiorIgual(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace(">=");
	geraCodExpressao();
}

void expressaoRelacionalMenorIgual(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
	empilhaPlace("<=");
	geraCodExpressao();
}

void expressaoAtrib(struct param **sintetizado, struct param paramAux) {
	if ((*sintetizado)->atrib == 0) {
		(*sintetizado)->atrib = 1;
	}
}

void expressao(struct param *herdado) {
	if (herdado->atrib) {
		struct nodo *nodo = pop(&pilhaLex);
		if (nodo != NULL) {
			geraCodAtribuicao(nodo->dado._string, herdado->atrib);
		}
	}
}

void operadorAtrib(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = 1;
}

void operadorAtribIncrMult(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = paramAux.tk;
	empilhaPlace("*");
}

void operadorAtribIncrDiv(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = paramAux.tk;
	empilhaPlace("/");
}

void operadorAtribIncrResto(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = paramAux.tk;
	empilhaPlace("%");
}

void operadorAtribIncrSoma(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = paramAux.tk;
	empilhaPlace("+");
}

void operadorAtribIncrSub(struct param **sintetizado, struct param paramAux) {
	(*sintetizado)->atrib = paramAux.tk;
	empilhaPlace("-");
}

void comandoCondicaoIf(struct param *herdado) {
	struct nodo *nodo = pop(&pilhaPlace);
	struct nodo *blocoComandosIf = pop(&pilhaCod);

	geraC3EControle(TK_IF, blocoComandosIf->dado._string, nodo->dado._string, geraLabel("fim"), NULL, NULL, NULL);
}

void comandoCondicaoIfElse(struct param *herdado) {
	struct nodo *nodo = pop(&pilhaPlace);
	struct nodo *blocoComandosElse = pop(&pilhaCod);
	struct nodo *blocoComandosIf = pop(&pilhaCod);

	char * lbElse = geraLabel("else");
	char * lbFim = geraLabel("fim");

	geraC3EControle(TK_IF, blocoComandosIf->dado._string, nodo->dado._string, lbFim, lbElse, NULL, NULL);
	geraC3EControle(TK_ELSE, blocoComandosElse->dado._string, NULL, lbFim, lbElse, NULL, NULL);
}

void comandoCondicaoSwitch(struct param *herdado) {
	struct nodo *nodo = pop(&pilhaPlace);
	struct nodo *blocoComandosSwtich = pop(&pilhaCod);

	char * codTeste = (char *) malloc(sizeof(char) * 5000);
	char * codTmp = (char *) malloc(sizeof(char) * 1000);

	while(pilhaLbCase != NULL) {
		struct nodo *lbCase = pop(&pilhaLbCase);
		sprintf(codTmp, "if %s == 1 goto %s\n", nodo->dado._string, lbCase->dado._string);
		strcat(codTeste, codTmp);
	}

	geraC3EControle(TK_SWITCH, blocoComandosSwtich->dado._string, NULL, fimSwitch, NULL, geraLabel("teste"), codTeste);

	free(fimSwitch);
	free(codTeste);
	free(codTmp);
	fimSwitch = NULL;
}

void comandoCondicaoCase(struct param *herdado) {
	struct nodo *blocoComandosCase = pop(&pilhaCod);

	if (fimSwitch == NULL) {
		fimSwitch = geraLabel("fim");
	}

	union dadoNodo dado;
	dado._string = geraLabel(NULL);
	push(&pilhaLbCase, dado);

	geraC3EControle(TK_CASE, blocoComandosCase->dado._string, NULL, fimSwitch, NULL, dado._string, NULL);
}

void comandoIteracaoWhile(struct param *herdado) {
	struct nodo *nodo = pop(&pilhaPlace);
	struct nodo *blocoComandosWhile = pop(&pilhaCod);

	geraC3EControle(TK_WHILE, blocoComandosWhile->dado._string, nodo->dado._string, geraLabel("fim"), NULL, geraLabel("inicio"), NULL);
}

void comandoIteracaoDoWhile(struct param *herdado) {
	struct nodo *nodo = pop(&pilhaPlace);
	struct nodo *blocoComandosDo = pop(&pilhaCod);

	geraC3EControle(TK_DO, blocoComandosDo->dado._string, nodo->dado._string, geraLabel("fim"), NULL, geraLabel("inicio"), NULL);
}