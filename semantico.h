#ifndef SEMANTICO_H
#define SEMANTICO_H

FILE *arqout;

union valor {
	int i;
	float f;
	double d;
} valor;

struct param {
	int tk;
	char *lex;
	char op;
	union valor *valor;
	int tipoValor;
} param;

void (*funcHerdado)(struct param *herdado);
void (*funcSintetizado)(struct param **sintetizado, struct param paramAux);
void addTabSim(char *lex, int tkTipo);
int existeTabSim(char *lex);

void tipo(struct param **sintetizado, struct param paramAux);
void setLex(struct param **sintetizado, struct param paramAux);
void setConst(struct param **sintetizado, struct param paramAux);
void expressaoSoma(struct param **sintetizado, struct param paramAux);
void expressaoSub(struct param **sintetizado, struct param paramAux);
void expressaoMult(struct param **sintetizado, struct param paramAux);
void expressaoDiv(struct param **sintetizado, struct param paramAux);
void expressaoResto(struct param **sintetizado, struct param paramAux);

void definiciaoVariavelId(struct param *herdado);
void expressaoPrimaria(struct param *herdado);
void expressaoPrimariaConst(struct param *herdado);
void comandoExpressao(struct param *herdado);

#endif