#ifndef SEMANTICO_H
#define SEMANTICO_H

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
} param;

void (*funcHerdado)(struct param *herdado);
void (*funcSintetizado)(struct param **sintetizado, struct param paramAux);
void addTabSim(char *lex, int tkTipo);
int existeTabSim(char *lex);

void tipo(struct param **sintetizado, struct param paramAux);
void setLex(struct param **sintetizado, struct param paramAux);
void setConst(struct param **sintetizado, struct param paramAux);

void definiciaoVariavelId(struct param *herdado);
void expressaoPrimaria(struct param *herdado);
void comandoExpressao(struct param *herdado);

#endif