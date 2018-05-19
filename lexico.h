#ifndef LEXICO_H
#define LEXICO_H

#define TK_VIRGULA 1
#define TK_ABRE_PAR 2
#define TK_FECHAR_PAR 3
#define TK_DOIS_PONTOS 4
#define TK_INTERROGACAO 5
#define TK_ATRIB 6
#define TK_PONTO_VIRG 7
#define TK_ABRE_CHAVE 8
#define TK_FECHA_CHAVE 9
#define TK_MULT 10
#define TK_DIV 11
#define TK_RESTO 12
#define TK_SOMA 13
#define TK_SUB 14
#define TK_INCREMENTO 15
#define TK_DECREMENTO 16
#define TK_MAIOR 17
#define TK_MENOR 18
#define TK_MENOR_IGUAL 19
#define TK_MAIOR_IGUAL 20
#define TK_IGUAL 21
#define TK_DIFERENTE 22
#define TK_INCR_MULT 23
#define TK_INCR_DIV 24
#define TK_INCR_RESTO 25
#define TK_INCR_SOMA 26
#define TK_INCR_SUB 27
#define TK_E 28
#define TK_OU 29
#define TK_NEGACAO 30

#define TK_WHILE 31
#define TK_DO 32
#define TK_FOR 33
#define TK_CONTINUE 34
#define TK_IF 35
#define TK_ELSE 36
#define TK_SWITCH 37
#define TK_CASE 38
#define TK_DEFAULT 39
#define TK_BREAK 40
#define TK_ID 41
#define TK_VOID 42
#define TK_INT 43
#define TK_FLOAT 44
#define TK_DOUBLE 45
#define TK_CONST 46

#define TK_BITWISE_E 47
#define TK_BITWISE_OU 48
#define TK_BITWISE_XOU 49
#define TK_BITWISE_COMPL 50
#define TK_BITWISE_ESQ 51
#define TK_BITWISE_DIR 52
#define TK_RETURN 53

#define TOKENS "TK_VIRGULA", \
"TK_ABRE_PAR", \
"TK_FECHAR_PAR", \
"TK_DOIS_PONTOS", \
"TK_INTERROGACAO", \
"TK_ATRIB", \
"TK_PONTO_VIRG", \
"TK_ABRE_CHAVE", \
"TK_FECHA_CHAVE", \
"TK_MULT", \
"TK_DIV", \
"TK_RESTO", \
"TK_SOMA", \
"TK_SUB", \
"TK_INCREMENTO", \
"TK_DECREMENTO", \
"TK_MAIOR", \
"TK_MENOR", \
"TK_MENOR_IGUAL", \
"TK_MAIOR_IGUAL", \
"TK_IGUAL", \
"TK_DIFERENTE", \
"TK_INCR_MULT", \
"TK_INCR_DIV", \
"TK_INCR_RESTO", \
"TK_INCR_SOMA", \
"TK_INCR_SUB", \
"TK_E", \
"TK_OU", \
"TK_NEGACAO", \
"TK_WHILE", \
"TK_DO", \
"TK_FOR", \
"TK_CONTINUE", \
"TK_IF", \
"TK_ELSE", \
"TK_SWITCH", \
"TK_CASE", \
"TK_DEFAULT", \
"TK_BREAK", \
"TK_ID", \
"TK_VOID", \
"TK_INT", \
"TK_FLOAT", \
"TK_DOUBLE", \
"TK_CONST", \
"TK_BITWISE_E", \
"TK_BITWISE_OU", \
"TK_BITWISE_XOU", \
"TK_BITWISE_COMPL", \
"TK_BITWISE_ESQ", \
"TK_BITWISE_DIR", \
"TK_RETURN"

#define TAB_SIZE 4

int pos = 0;
int linha = 1;
int coluna = 1;
int tk;

FILE *arqin;

struct pal_res {
	char palavra[20];
	int tk;
};

struct pal_res lista_pal[] = {
	{"while", TK_WHILE},
	{"do", TK_DO},
	{"for", TK_FOR},
	{"continue", TK_CONTINUE},
	{"if", TK_IF},
	{"else", TK_ELSE},
	{"switch", TK_SWITCH},
	{"case", TK_CASE},
	{"default", TK_DEFAULT},
	{"break", TK_BREAK},
	{"void", TK_VOID},
	{"int", TK_INT},
	{"float", TK_FLOAT},
	{"double", TK_DOUBLE},
	{"return", TK_RETURN}
};

int le_token(char lex[]);
int obter_coluna(char *lex);

#endif