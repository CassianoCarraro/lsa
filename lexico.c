#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexico.h"

int palavra_reservada(char lex[]) {
	for(int i = 0; i < (sizeof(lista_pal) / sizeof(lista_pal[0])); i++) {
		if (strcmp(lex, lista_pal[i].palavra) == 0) {
			return lista_pal[i].tk;
		}
	}

	return TK_ID;
}

void incrementa_linha(char c) {
	if (c == '\n') {
		linha++;
		coluna = 1;
	} else if (c == '\t') {
		coluna += TAB_SIZE - 1;
	}
}

void avanca_pos() {
	pos++;
	coluna++;
}

void volta_pos(int offset) {
	while (offset > 0) {
		pos--;
		coluna--;

		offset--;
	}
}

int obter_coluna(char *lex) {
	return (coluna - strlen(lex));
}

void add_lex(char **lex, int *pos, char c) {
	(*lex)[(*pos)++] = c;
	(*lex)[*pos] = '\0';
}

int le_token(char st[], char lex[]) {
	int estado = 0, posl = 0, fim = 0, constante = 0, decimalPoints = 0;

	while (!fim) {
		char c = st[pos];
		lex[posl++] = c;

		switch(estado) {
			case 0:
				if (c == '*' || c == '/' || c == '%' || c == '+' || c == '-' || c == '&' || c == '|' || c == '!' || c == '<' || c == '>' || c == '=') {
					avanca_pos();
					char proxC = st[pos];
					avanca_pos();

					if (c == '+' && proxC == '+') {add_lex(&lex, &posl, proxC); return TK_INCREMENTO;}
					else if (c == '-' && proxC == '-') {add_lex(&lex, &posl, proxC); return TK_DECREMENTO;}
					else if (c == '*' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_INCR_MULT;}
					else if (c == '/' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_INCR_DIV;}
					else if (c == '%' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_INCR_RESTO;}
					else if (c == '+' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_INCR_SOMA;}
					else if (c == '-' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_INCR_SUB;}
					else if (c == '<' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_MENOR_IGUAL;}
					else if (c == '>' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_MAIOR_IGUAL;}
					else if (c == '=' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_IGUAL;}
					else if (c == '!' && proxC == '=') {add_lex(&lex, &posl, proxC); return TK_DIFERENTE;}
					else if (c == '&' && proxC == '&') {add_lex(&lex, &posl, proxC); return TK_E;}
					else if (c == '|' && proxC == '|') {add_lex(&lex, &posl, proxC); return TK_OU;}
					else if (c == '<' && proxC == '<') {add_lex(&lex, &posl, proxC); return TK_BITWISE_ESQ;}
					else if (c == '>' && proxC == '>') {add_lex(&lex, &posl, proxC); return TK_BITWISE_DIR;}
					else {
						volta_pos(2);
						c = st[pos];
					}
				}
				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9' || c == '.' && (st[pos+1] >= '0' && st[pos+1] <= '9')) {
					avanca_pos();
					estado = 1;
					if(c >= '0' && c <= '9' || c == '.') {
						constante = 1;
						if(c == '.') {
							decimalPoints++;
						}
					}

					break;
				}

				if(c==','){lex[posl]='\0';avanca_pos();return TK_VIRGULA;}
				if(c=='('){lex[posl]='\0';avanca_pos();return TK_ABRE_PAR;}
				if(c==')'){lex[posl]='\0';avanca_pos();return TK_FECHAR_PAR;}
				if(c==':'){lex[posl]='\0';avanca_pos();return TK_DOIS_PONTOS;}
				if(c=='?'){lex[posl]='\0';avanca_pos();return TK_INTERROGACAO;}
				if(c=='='){lex[posl]='\0';avanca_pos();return TK_ATRIB;}
				if(c==';'){lex[posl]='\0';avanca_pos();return TK_PONTO_VIRG;}
				if(c=='{'){lex[posl]='\0';avanca_pos();return TK_ABRE_CHAVE;}
				if(c=='}'){lex[posl]='\0';avanca_pos();return TK_FECHA_CHAVE;}
				if(c=='*'){lex[posl]='\0';avanca_pos();return TK_MULT;}
				if(c=='/'){lex[posl]='\0';avanca_pos();return TK_DIV;}
				if(c=='%'){lex[posl]='\0';avanca_pos();return TK_RESTO;}
				if(c=='+'){lex[posl]='\0';avanca_pos();return TK_SOMA;}
				if(c=='-'){lex[posl]='\0';avanca_pos();return TK_SUB;}
				if(c=='>'){lex[posl]='\0';avanca_pos();return TK_MAIOR;}
				if(c=='<'){lex[posl]='\0';avanca_pos();return TK_MENOR;}
				if(c=='!'){lex[posl]='\0';avanca_pos();return TK_NEGACAO;}
				if(c=='&'){lex[posl]='\0';avanca_pos();return TK_BITWISE_E;}
				if(c=='|'){lex[posl]='\0';avanca_pos();return TK_BITWISE_OU;}
				if(c=='^'){lex[posl]='\0';avanca_pos();return TK_BITWISE_XOU;}
				if(c=='~'){lex[posl]='\0';avanca_pos();return TK_BITWISE_COMPL;}
				if(c==' ' || c == '\t' || c == '\n') {avanca_pos();posl--;incrementa_linha(c);break;}
				if(c=='\0') {
					return 0;
				}

				return -1;
			case 1:
				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9' || c == '.') {
					avanca_pos();

					if(constante) {
						constante = (c >= '0' && c <= '9' || c == '.');
						if(c == '.') {
							if(decimalPoints > 0) {
								return -1;
							} else {
								decimalPoints++;
							}
						}
					}

					break;
				}

				lex[--posl] = '\0';

				if(constante) {
					return TK_CONST;
				} else {
					return palavra_reservada(lex);
				}
		}
	}
}