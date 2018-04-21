#ifndef SEMANTICO_H
#define SEMANTICO_H

struct param {
	int tk;
	char *lex;
} param;

void (*funcHerdado)(struct param *herdado);
void (*funcSintetizado)(struct param **sintetizado, struct param paramAux);
void addTabSim(char *lex, int tkTipo);
int existeTabSim(char *lex);

void tipo(struct param **sintetizado, struct param paramAux);
void setLex(struct param **sintetizado, struct param paramAux);
void definiciaoVariavelId(struct param *herdado);

#endif