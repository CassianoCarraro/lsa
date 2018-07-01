#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "sintatico.h"
#include "sintatico.c"

int main() {
	char *lex = (char *) malloc(sizeof(char) * TAMANHO_LEX);
	char *tk_rec = (char *) malloc(sizeof(char) * TAMANHO_LEX);
	int parse_res;

	arqin = fopen("program.c", "r");
	arqout = fopen("out.c3e", "w");

	parse_res = parse(lex, tk_rec);

	if (parse_res == 1) {
		printf("OK. Arquivo out.c3e gerado.\n");
	} else if (parse_res == 0) {
		printf("Erro sintatico: token %s não esperado na linha: %d, coluna: %d.\n", tk_rec, linha, obter_coluna(lex));
	} else if (parse_res == -1) {
		printf("Erro lexico: encontrou o caracter %s na linha: %d, coluna: %d\n", lex, linha, obter_coluna(lex));
	}

	free(lex);

	fflush(arqin);
	fclose(arqin);

	fflush(arqout);
	fclose(arqout);

	#ifdef _WIN32
		system("pause");
	#endif
}