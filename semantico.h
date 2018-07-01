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
	int atrib;
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
void expressaoBitBitE(struct param **sintetizado, struct param paramAux);
void expressaoBitBitOu(struct param **sintetizado, struct param paramAux);
void expressaoBitBitXou(struct param **sintetizado, struct param paramAux);
void expressaoBitBitEsq(struct param **sintetizado, struct param paramAux);
void expressaoBitBitDir(struct param **sintetizado, struct param paramAux);

void expressaoIgualdadeIgual(struct param **sintetizado, struct param paramAux);
void expressaoIgualdadeDiferente(struct param **sintetizado, struct param paramAux);
void expressaoRelacionalMaior(struct param **sintetizado, struct param paramAux);
void expressaoRelacionalMenor(struct param **sintetizado, struct param paramAux);
void expressaoRelacionalMaiorIgual(struct param **sintetizado, struct param paramAux);
void expressaoRelacionalMenorIgual(struct param **sintetizado, struct param paramAux);
void expressaoAtrib(struct param **sintetizado, struct param paramAux);

void operadorAtrib(struct param **sintetizado, struct param paramAux);
void operadorAtribIncr(struct param **sintetizado, struct param paramAux);
void operadorAtribIncrMult(struct param **sintetizado, struct param paramAux);
void operadorAtribIncrDiv(struct param **sintetizado, struct param paramAux);
void operadorAtribIncrResto(struct param **sintetizado, struct param paramAux);
void operadorAtribIncrSub(struct param **sintetizado, struct param paramAux);

void expressao(struct param *herdado);

void definiciaoVariavelId(struct param *herdado);
void definicaoFuncaoCorpo(struct param *herdado);
void expressaoPrimaria(struct param *herdado);
void expressaoPrimariaConst(struct param *herdado);
void comandoExpressao(struct param **sintetizado, struct param paramAux);
void inicioBloco(struct param *herdado);

void comandoCondicaoIf(struct param *herdado);
void comandoCondicaoIfElse(struct param *herdado);
void comandoCondicaoSwitch(struct param *herdado);
void comandoCondicaoCase(struct param *herdado);

void comandoIteracaoWhile(struct param *herdado);
void comandoIteracaoDoWhile(struct param *herdado);
#endif