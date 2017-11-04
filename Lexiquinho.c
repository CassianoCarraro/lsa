#define TKId 1
#define TKVoid 2
#define TKInt 3
#define TKFloat 4
#define TKVirgula 5
#define TKDoisPontos 6
#define TKAbrePar 7
#define TKFechaPar 8
#define TKAtrib 9
#define TKPontoeVirg 10
#define TKAbreChave 11
#define TKFechaChave 12
#define TKSoma 13
#define TKDuploMais 14
#define TKChar 15
#define TKWhile 16
#define TKDo 17
#define TKFor 18
#define TKIf 19
#define TKSwitch 20
#define TKCase 21
#define TKDefault 22
#define TKMaior 23
#define TKMenor 24
#define TKConstante 25

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int pos = 0;
int linha = 1;
int coluna = 1;
char exp1[200], lex[20];
int tk;

struct pal_res {
	char palavra[20];
	int tk;
};

struct pal_res lista_pal[] = {
	{"void", TKVoid},
	{"int", TKInt},
	{"float", TKFloat},
	{"char", TKChar},
	{"while", TKWhile},
	{"do", TKDo},
	{"for", TKFor},
	{"if", TKIf},
	{"switch", TKSwitch},
	{"case", TKCase},
	{"default", TKDefault},
};

int palavra_reservada(char lex[])
{
	int postab = 0;
	for(int i = 0; i < (sizeof(lista_pal) / sizeof(lista_pal[0])) - 1; ++i) {
		if (strcmp(lex, lista_pal[postab].palavra) == 0) {
			return lista_pal[postab].tk;
		}

		postab++;
	}

	return TKId;
}

void incrementaLina(char c) {
	if (c == '\n') {
		linha++;
		coluna = 1;
	}
}

void avancaPos() {
	pos++;
	coluna++;
}

int obterColuna() {
	return (coluna - strlen(lex));
}

int le_token(char st[], char lex[])
{
	int estado = 0, posl = 0, fim = 0, constante = 0;

	while (!fim)
	{
		char c = st[pos];
		lex[posl++] = c;

		switch(estado) {
			case 0:
				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9') {
					avancaPos();
					estado = 1;
					constante = (c >= '0' && c <= '9');

					break;
				}

				if(c == '+') {
					avancaPos();
					c = st[pos];

					if (c=='+') {
					   lex[posl++]='+';
					   lex[posl]='\0';
					   avancaPos();
					   return TKDuploMais;
					} else {
					   lex[posl]='\0';
					   return TKSoma;
					}
				}

				if(c=='='){lex[posl]='\0';avancaPos();return TKAtrib;}
				if(c=='('){lex[posl]='\0';avancaPos();return TKAbrePar;}
				if(c==')'){lex[posl]='\0';avancaPos();return TKFechaPar;}
				if(c=='{'){lex[posl]='\0';avancaPos();return TKAbreChave;}
				if(c=='}'){lex[posl]='\0';avancaPos();return TKFechaChave;}
				if(c==','){lex[posl]='\0';avancaPos();return TKVirgula;}
				if(c==';'){lex[posl]='\0';avancaPos();return TKPontoeVirg;}
				if(c==':'){lex[posl]='\0';avancaPos();return TKDoisPontos;}
				if(c=='>'){lex[posl]='\0';avancaPos();return TKMaior;}
				if(c=='<'){lex[posl]='\0';avancaPos();return TKMenor;}
				if(c==' ' || c == '\n') {avancaPos();posl--;incrementaLina(c);break;}
				if(c=='\0') {
					return -1;
				}

				printf("Erro lexico: encontrou o caracter %c na posicao %d\n", c, pos);

				return -1;
				//break;
			case 1:
				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9') {
					avancaPos();

					if (constante) {
						constante = (c >= '0' && c <= '9');
					}

					break;
				}

				lex[--posl] = '\0';
				//printf("--%s\n", lex);

				if (constante) {
					return TKConstante;
				} else {
					return palavra_reservada(lex);
				}
		}
	}
}

int r() {
	tk = le_token(exp1, lex);
	if (tk == TKVirgula) {
		tk = le_token(exp1, lex);
		if (tk == TKId) {
			return r();
		} else {
			return false;
		}
	} else {
		return true;
	}
}

int lid() {
	tk = le_token(exp1, lex);
	if (tk == TKId) {
		if (r()) {
			return true;
		}
	}

	return false;
}

int tipo() {
	tk = le_token(exp1, lex);
	if (tk == TKInt || tk == TKFloat || tk == TKChar) {
		return true;
	}

	return false;
}

int dv() {
	if (tipo()) {
		if (lid()) {
			if (tk == TKPontoeVirg) {
				return true;
			} else {
				printf("Falta ponto e virgula\n");
				return false;
			}
		}
	}

	return false;
}

void lexical() {
	char token_desc[25][15] = {"TKId", "TKVoid", "TKInt", "TKFloat", "TKVirgula", "TKDoisPontos", "TKAbrePar", "TKFechaPar", "TKAtrib", "TKPontoeVirg", "TKAbreChave", "TKFechaChave", "TKSoma", "TKDuploMais", "TKChar", "KWhile", "TKDo", "TKFor", "TKIf", "TKSwitch", "TKCase", "TKDefault", "TKMaior", "TKMenor", "TKConstante"};
	FILE *fp;
	fp = fopen("Saida.lex", "w");
	char buffer[50];
	int file_pos = 0;

	printf("lexical analysis\n");
	printf("%12s \t %10s \t Linha \t Coluna", "Token", "Lexema");
	printf("\n===============================================\n");

	while((tk = le_token(exp1, lex)) > 0) {
		sprintf(buffer, "%12s \t %10s \t %d \t %d\n", token_desc[tk-1], lex, linha, obterColuna());
		printf("%s", buffer);
		fwrite(buffer, strlen(buffer), 1, fp);
		file_pos += strlen(buffer);
	}

	printf("\n===============================================\n");
}

int main()
{
	FILE *fp;
	fp = fopen("program.c", "r");
	fread(exp1, 1, 200, fp);
	printf("program\n===============================================\n%s \n===============================================\n", exp1);
	lexical();

	/*if (dv()) {
		printf("Programa correto\n");
	} else {
		printf("Programa INCORRETO!\n");
	}*/
}