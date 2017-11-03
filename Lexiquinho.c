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
#define TKMenor 23

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int pos = 0;
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
	{"fimtabela", TKId},
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
	for(int i = 0; i < (sizeof(lista_pal) / sizeof(lista_pal[0])) -1; ++i) {
		if (strcmp(lex,lista_pal[postab].palavra)==0) {
			return lista_pal[postab].tk;
		}
		postab++;
	}
	return TKId;
}

int le_token(char st[],char lex[])
{
	int estado=0, fim=0, posl=0;
	while (!fim)
	{
		char c = st[pos];
		lex[posl++] = c;
		switch(estado) {
			case 0:
				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_') {
					pos++;
					estado=1;
					break;
				}
				if(c == '+') {
					c = st[++pos];
					if (c=='+') {
					   lex[posl++]='+';
					   lex[posl]='\0';
					   pos++;
					   return TKDuploMais;
					} else {
					   lex[posl]='\0';
					   return TKSoma;
					}
				}
				if(c=='='){lex[posl]='\0';pos++;return TKAtrib;}
				if(c=='('){lex[posl]='\0';pos++;return TKAbrePar;}
				if(c==')'){lex[posl]='\0';pos++;return TKFechaPar;}
				if(c=='{'){lex[posl]='\0';pos++;return TKAbreChave;}
				if(c=='}'){lex[posl]='\0';pos++;return TKFechaChave;}
				if(c==','){lex[posl]='\0';pos++;return TKVirgula;}
				if(c==';'){lex[posl]='\0';pos++;return TKPontoeVirg;}
				if(c==':'){lex[posl]='\0';pos++;return TKDoisPontos;}
				if(c=='>'){lex[posl]='\0';pos++;return TKMaior;}
				if(c=='<'){lex[posl]='\0';pos++;return TKMenor;}
				if(c==' ' || c=='\n') {pos++;posl--;break;}
				if(c=='\0') {
					return -1;
				}
				printf("Erro lexico: encontrou o caracter %c na posicao %d\n",c,pos);
				return -1;
				break;
			case 1:
				if(c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9') {
					pos++;
					break;
				}
				lex[--posl] = '\0';
				return palavra_reservada(lex);
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
	char token_desc[24][15] = {"TKId", "TKVoid", "TKInt", "TKFloat", "TKVirgula", "TKDoisPontos", "TKAbrePar", "TKFechaPar", "TKAtrib", "TKPontoeVirg", "TKAbreChave", "TKFechaChave", "TKSoma", "TKDuploMais", "TKChar", "KWhile", "TKDo", "TKFor", "TKIf", "TKSwitch", "TKCase", "TKDefault", "TKMaior", "TKMenor"};
	FILE *fp;
	fp = fopen("out.lex", "w");
	char buffer[50];
	int file_pos = 0;
	printf("lexical analysis\n====================\n");
	while((tk = le_token(exp1, lex)) > 0) {
		sprintf(buffer, "%d %s \t %s\n", pos, lex, token_desc[tk-1]);
		printf("%s", buffer);
		fwrite(buffer, strlen(buffer), 1, fp);
		file_pos += strlen(buffer);
	}
	printf("\n====================\n");
}

int main()
{
	FILE *fp;
	fp = fopen("program.c", "r");
	fread(exp1, 1, 200, fp);
	printf("program\n====================\n%s \n====================\n", exp1);
	lexical();

	/*if (dv()) {
		printf("Programa correto\n");
	} else {
		printf("Programa INCORRETO!\n");
	}*/
}