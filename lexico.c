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
"TK_CONST"

#define TAB_SIZE 4

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int pos = 0;
int linha = 1;
int coluna = 1;
char exp1[500], lex[20];
int tk;

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
	{"double", TK_DOUBLE}
};

int palavra_reservada(char lex[]) {
	int postab = 0;
	for(int i = 0; i < (sizeof(lista_pal) / sizeof(lista_pal[0])) - 1; ++i) {
		if (strcmp(lex, lista_pal[postab].palavra) == 0) {
			return lista_pal[postab].tk;
		}

		postab++;
	}

	return TK_ID;
}

void incrementaLina(char c) {
	if (c == '\n') {
		linha++;
		coluna = 1;
	} else if (c == '\t') {
		coluna += TAB_SIZE - 1;
	}
}

void avancaPos() {
	pos++;
	coluna++;
}

void voltaPos(int offset) {
	while (offset > 0) {
		pos--;
		coluna--;

		offset--;
	}
}

int obterColuna() {
	return (coluna - strlen(lex));
}

void addLex(char **lex, int *pos, char c) {
	(*lex)[(*pos)++] = c;
	(*lex)[*pos] = '\0';
}

int le_token(char st[], char lex[]) {
	int estado = 0, posl = 0, fim = 0, constante = 0;

	while (!fim) {
		char c = st[pos];
		lex[posl++] = c;

		switch(estado) {
			case 0:
				if (c == '*' || c == '/' || c == '%' || c == '+' || c == '-' || c == '&' || c == '|' || c == '!' || c == '<' || c == '>') {
					avancaPos();
					char proxC = st[pos];
					avancaPos();

					if (c == '+' && proxC == '+') {addLex(&lex, &posl, proxC); return TK_INCREMENTO;}
					else if (c == '-' && proxC == '-') {addLex(&lex, &posl, proxC); return TK_DECREMENTO;}
					else if (c == '*' && proxC == '=') {addLex(&lex, &posl, proxC); return TK_INCR_MULT;}
					else if (c == '/' && proxC == '=') {addLex(&lex, &posl, proxC);	return TK_INCR_DIV;}
					else if (c == '%' && proxC == '=') {addLex(&lex, &posl, proxC);	return TK_INCR_RESTO;}
					else if (c == '+' && proxC == '=') {addLex(&lex, &posl, proxC);	return TK_INCR_SOMA;}
					else if (c == '-' && proxC == '=') {addLex(&lex, &posl, proxC);	return TK_INCR_SUB;}
					else if (c == '<' && proxC == '=') {addLex(&lex, &posl, proxC);	return TK_MENOR_IGUAL;}
					else if (c == '>' && proxC == '=') {addLex(&lex, &posl, proxC);	return TK_MAIOR_IGUAL;}
					else if (c == '=' && proxC == '=') {addLex(&lex, &posl, proxC); return TK_IGUAL;}
					else if (c == '!' && proxC == '=') {addLex(&lex, &posl, proxC);	return TK_DIFERENTE;}
					else if (c == '&' && proxC == '&') {addLex(&lex, &posl, proxC); return TK_E;}
					else if (c == '|' && proxC == '|') {addLex(&lex, &posl, proxC); return TK_OU;}
					else {
						voltaPos(2);
						c = st[pos];
					}
				}

				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9') {
					avancaPos();
					estado = 1;
					constante = (c >= '0' && c <= '9');

					break;
				}

				if(c==','){lex[posl]='\0';avancaPos();return TK_VIRGULA;}
				if(c=='('){lex[posl]='\0';avancaPos();return TK_ABRE_PAR;}
				if(c==')'){lex[posl]='\0';avancaPos();return TK_FECHAR_PAR;}
				if(c==':'){lex[posl]='\0';avancaPos();return TK_DOIS_PONTOS;}
				if(c=='?'){lex[posl]='\0';avancaPos();return TK_INTERROGACAO;}
				if(c=='='){lex[posl]='\0';avancaPos();return TK_ATRIB;}
				if(c==';'){lex[posl]='\0';avancaPos();return TK_PONTO_VIRG;}
				if(c=='{'){lex[posl]='\0';avancaPos();return TK_ABRE_CHAVE;}
				if(c=='}'){lex[posl]='\0';avancaPos();return TK_FECHA_CHAVE;}
				if(c=='*'){lex[posl]='\0';avancaPos();return TK_MULT;}
				if(c=='/'){lex[posl]='\0';avancaPos();return TK_DIV;}
				if(c=='%'){lex[posl]='\0';avancaPos();return TK_RESTO;}
				if(c=='+'){lex[posl]='\0';avancaPos();return TK_SOMA;}
				if(c=='-'){lex[posl]='\0';avancaPos();return TK_SUB;}
				if(c=='>'){lex[posl]='\0';avancaPos();return TK_MAIOR;}
				if(c=='<'){lex[posl]='\0';avancaPos();return TK_MENOR;}
				if(c=='!'){lex[posl]='\0';avancaPos();return TK_NEGACAO;}
				if(c==' ' || c == '\t' || c == '\n') {avancaPos();posl--;incrementaLina(c);break;}
				if(c=='\0') {
					return -1;
				}

				printf("Erro lexico: encontrou o caracter %c na lina: %d, coluna: %d\n", c, linha, coluna);

				return -1;
			case 1:
				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9') {
					avancaPos();

					if (constante) {
						constante = (c >= '0' && c <= '9');
					}

					break;
				}

				lex[--posl] = '\0';

				if (constante) {
					return TK_CONST;
				} else {
					return palavra_reservada(lex);
				}
		}
	}
}

void lexical() {
	char token_desc[47][16] = {TOKENS};
	FILE *fp;
	fp = fopen("Saida.lex", "w");
	char buffer[50];
	int file_pos = 0;

	printf("%16s \t %10s \t Linha \t Coluna", "Token", "Lexema");
	printf("\n=======================================================\n");

	while((tk = le_token(exp1, lex)) > 0) {
		sprintf(buffer, "%16s \t %10s \t %d \t %d\n", token_desc[tk-1], lex, linha, obterColuna());
		printf("%s", buffer);
		fwrite(buffer, strlen(buffer), 1, fp);
		file_pos += strlen(buffer);
	}

	printf("\n=======================================================\n");
}

int main() {
	FILE *fp;
	fp = fopen("program.c", "r");
	fread(exp1, 1, 500, fp);
	printf("Programa\n=======================================================\n%s \n=======================================================\n", exp1);

	lexical();
}